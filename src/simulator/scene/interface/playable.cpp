#include "playable.h"

namespace scene
{
namespace interface
{
Playable::Playable()
    : boost::noncopyable()
    , m_paused(false)
    , m_stopped(true)
    , m_finishFlag(true)
    , m_lock()
    , m_finishLock()
    , m_pThread(nullptr)
{
}

Playable::~Playable()
{
    stop();
}

bool Playable::start()
{
    concurrent::guard guard(m_lock);

    if (m_paused)
    {
        m_resumeEvent.set();
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

        m_pThread.reset(new boost::thread(&Playable::loop, this));
        m_resumeEvent.set();
    }
    catch (...)
    {
        m_stopped = true;
        return false;
    }

    return true;
}

bool Playable::pause()
{
    concurrent::guard guard(m_lock);

    if (m_stopped || m_paused)
        return false;

    m_resumeEvent.reset();
    m_paused = true;

    return true;
}

bool Playable::stop()
{
    concurrent::guard guard(m_lock);

    if (m_stopped)
        return false;

    {
        concurrent::guard guard(m_finishLock);
        m_finishFlag = true;
    }

    if (m_paused)
        m_resumeEvent.set();

    m_pThread->join();
    m_pThread.reset();

    m_stopped = true;

    return true;
}

bool Playable::finished() const
{
    concurrent::guard guard(m_finishLock);
    return m_finishFlag;
}

void Playable::loop()
{
    for (;;)
    {
        m_resumeEvent.wait();

        if (finished())
            break;

        try
        {
            play();
        }
        catch (...)
        {
            stop();
            break;
        }
    }
}
} // namespace interface
} // namespace scene
