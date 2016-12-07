#include "iterative.h"

namespace scene
{
namespace utils
{
Iterative::Iterative()
    : boost::noncopyable()
    , m_paused(false)
    , m_stopped(true)
    , m_finishFlag(true)
    , m_lock()
    , m_finishLock()
    , m_pThread(nullptr)
{
}

Iterative::~Iterative()
{
    stop();
}

bool Iterative::start()
{
    concurrent::guard guard(m_lock);

    if (m_paused)
    {
        m_resumeEvent.reset();
        m_paused = false;
        return true;
    }

    if (!m_stopped)
        return false;

    try
    {
        m_stopped = false;

        {
            concurrent::guard guard(m_finishLock);
            m_finishFlag = false;
        }

        m_pThread.reset(new boost::thread(&Iterative::loop, this));
    }
    catch (...)
    {
        m_stopped = true;
        return false;
    }

    return true;
}

bool Iterative::pause()
{
    concurrent::guard guard(m_lock);

    if (m_stopped || m_paused)
        return false;

    m_resumeEvent.set();
    m_paused = true;

    return true;
}

bool Iterative::stop()
{
    concurrent::guard guard(m_lock);

    if (m_stopped)
        return false;

    {
        concurrent::guard guard(m_finishLock);
        m_finishFlag = true;
    }

    if (m_paused)
        m_resumeEvent.reset();

    m_pThread->join();
    m_pThread.reset();

    m_stopped = true;

    return true;
}

void Iterative::loop()
{
    for (;;)
    {
        try
        {
            iterate();
        }
        catch (...)
        {
            stop();
            return;
        }

        {
            concurrent::guard guard(m_finishLock);
            if (m_finishFlag) return;
        }

        m_resumeEvent.wait();
    }
}
} // namespace utils
} // namespace scene
