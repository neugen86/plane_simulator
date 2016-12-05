#include "iterative.h"

namespace utils
{
Iterative::Iterative(types::duration_t duration)
    : boost::noncopyable()
    , m_paused(false)
    , m_stopped(true)
    , m_pThread(nullptr)
    , m_duration(duration)
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
        m_condResume.notify_one();
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
        m_condResume.notify_one();
    }

    m_pThread->join();
    m_pThread.reset();

    return true;
}

void Iterative::loop()
{
    for (;;)
    {
        handleTimeout();
        handleResume();

        if (handleStop())
            return;

        try
        {
            iterate();
        }
        catch (...)
        {
            stop();
            return;
        }
    }
}

void Iterative::handleTimeout()
{
    using namespace boost::chrono;
    static nanoseconds ns =
            duration_cast<nanoseconds>(milliseconds(m_duration));
    boost::this_thread::sleep_for(ns);
}

void Iterative::handleResume()
{
    boost::mutex::scoped_lock lock(m_pauseGuard);
    while(m_paused) m_condResume.wait(lock);
}

bool Iterative::handleStop()
{
    boost::mutex::scoped_lock lock(m_stopGuard);
    return m_stopped;
}
} // namespace utils
