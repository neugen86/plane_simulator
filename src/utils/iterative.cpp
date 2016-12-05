#include "iterative.h"

namespace utils
{
Iterative::Iterative(duration_t duration)
    : boost::noncopyable()
    , m_paused(false)
    , m_stopped(true)
    , m_duration(duration)
    , m_pThread(nullptr)
{
}

Iterative::~Iterative()
{
    stop();
}

bool Iterative::start()
{
    boost::mutex::scoped_lock lock(m_guard);

    if (!m_stopped)
        return false;

    if (m_paused)
    {
        boost::mutex::scoped_lock lock(m_pauseGuard);

        m_paused = false;
        m_condResume.notify_one();
    }
    else
    {
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

    if (m_stopped)
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
        boost::this_thread::sleep_for(boost::chrono::milliseconds(m_duration));

        {
            boost::mutex::scoped_lock lock(m_pauseGuard);
            while(m_paused) m_condResume.wait(lock);
        }

        {
            boost::mutex::scoped_lock lock(m_stopGuard);
            if (m_stopped) return;
        }

        try
        {
            iterate();
        }
        catch (...)
        {
        }
    }
}
} // namespace utils
