#include "iterative.h"

namespace utils
{
Iterative::Iterative(types::duration_t timeout)
    : boost::noncopyable()
    , m_paused(false)
    , m_stopped(true)
    , m_pThread(nullptr)
    , m_timeout(timeout)
{
}

Iterative::~Iterative()
{
    stop();
}

bool Iterative::start()
{
    boost::mutex::scoped_lock lock(m_guard);

    if (m_paused)
    {
        boost::mutex::scoped_lock lock(m_pauseGuard);

        m_paused = false;
        m_resumeCondition.notify_one();
    }
    else
    {
        if (!m_stopped)
            return false;

        try
        {
            m_stopped = false;
            m_pThread.reset(new boost::thread(&Iterative::loop, this));
        }
        catch (...)
        {
            m_stopped = true;
            return false;
        }
    }

    return true;
}

bool Iterative::pause()
{
    boost::mutex::scoped_lock lock(m_guard);

    if (m_stopped || m_paused)
        return false;

    {
        boost::mutex::scoped_lock lock(m_pauseGuard);
        m_paused = true;
    }

    return true;
}

bool Iterative::stop()
{
    boost::mutex::scoped_lock lock(m_guard);

    if (m_stopped)
        return false;

    {
        boost::mutex::scoped_lock lock(m_stopGuard);
        m_stopped = true;
    }

    if (m_paused)
    {
        boost::mutex::scoped_lock lock(m_pauseGuard);

        m_paused = false;
        m_resumeCondition.notify_one();
    }

    m_pThread->join();
    m_pThread.reset();

    return true;
}

void Iterative::loop()
{
    using namespace boost::chrono;
    typedef high_resolution_clock clock_t;

    for (;;)
    {
        const clock_t::time_point start = clock_t::now();

        try
        {
            iterate();
        }
        catch (...)
        {
            stop();
            return;
        }

        milliseconds spent = duration_cast<milliseconds>(clock_t::now() - start);
        handleTimeout(m_timeout - spent);

        if (handleStop())
            return;

        handleResume();
    }
}

void Iterative::handleTimeout(boost::chrono::milliseconds timeout)
{
    if (timeout.count() > 0)
    {
        using namespace boost::chrono;
        boost::this_thread::sleep_for(duration_cast<nanoseconds>(timeout));
    }
}

void Iterative::handleResume()
{
    boost::mutex::scoped_lock lock(m_pauseGuard);
    while(m_paused) m_resumeCondition.wait(lock);
}

bool Iterative::handleStop()
{
    boost::mutex::scoped_lock lock(m_stopGuard);
    return m_stopped;
}
} // namespace utils
