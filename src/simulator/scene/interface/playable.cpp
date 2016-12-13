#include "playable.h"

namespace scene
{
namespace interface
{
Playable::Playable(types::duration_t duration)
    : boost::noncopyable()
    , m_paused(false)
    , m_stopped(true)
    , m_finishFlag(true)
    , m_state(PlaybackState::STOPPED)
    , m_lock()
    , m_finishLock()
    , m_pThread(nullptr)
    , m_realDuration(duration)
    , m_duration(duration)
{
}

Playable::~Playable()
{
    stop(true);
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

    m_state = PlaybackState::STARTED;

    onStart();

    return true;
}

bool Playable::pause()
{
    concurrent::guard guard(m_lock);

    if (m_stopped || m_paused)
        return false;

    m_resumeEvent.reset();

    m_state = PlaybackState::PAUSED;
    m_paused = true;

    m_realDuration = m_duration;

    onPause();

    return true;
}

bool Playable::stop(bool fromDestructor)
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

    m_state = PlaybackState::STOPPED;
    m_stopped = true;
    m_paused = false;

    m_realDuration = m_duration;

    if (!fromDestructor)
        onStop();

    return true;
}

bool Playable::finished() const
{
    concurrent::guard guard(m_finishLock);
    return m_finishFlag;
}

void Playable::loop()
{
    using namespace boost::chrono;
    typedef boost::chrono::high_resolution_clock clock_t;

    for (;;)
    {
        m_resumeEvent.wait();

        const clock_t::time_point now = clock_t::now();

        if (finished())
            break;

        try
        {
            iteration();
        }
        catch(...)
        {
            stop();
            break;
        }

        const milliseconds spent =
                duration_cast<milliseconds>(clock_t::now() - now);

        if (m_duration > spent)
        {
            m_realDuration = m_duration;

            const nanoseconds rest =
                    duration_cast<nanoseconds>(m_duration - spent);
            boost::this_thread::sleep_for(rest);
        }
        else
        {
            m_realDuration = spent;
        }
    }
}
} // namespace interface
} // namespace scene
