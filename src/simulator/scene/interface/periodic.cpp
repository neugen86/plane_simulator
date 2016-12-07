#include "periodic.h"

namespace scene
{
namespace interface
{
using namespace boost::chrono;
typedef high_resolution_clock clock_t;

const types::duration_t Periodic::DefaultDuration(50);

Periodic::Periodic(types::duration_t duration)
    : m_lock()
    , m_trueLock()
    , m_duration(duration)
{
    m_then = clock_t::now();
}

void Periodic::setDuration(types::duration_t duration)
{
    concurrent::guard guard(m_lock);
    m_duration = milliseconds(duration);
}

types::duration_t Periodic::trueDuration() const
{
    concurrent::guard guard(m_trueLock);
    return m_trueDuration.count();
}

types::duration_t Periodic::duration() const
{
    concurrent::guard guard(m_lock);
    return m_duration.count();
}

bool Periodic::expired() const
{
    static clock_t::time_point now = clock_t::now();

    const milliseconds spent =
            duration_cast<milliseconds>(now - m_then);

    {
        concurrent::guard guard(m_lock);
        if (m_duration > spent) return false;
    }
    {
        concurrent::guard guard(m_trueLock);
        m_trueDuration = spent;
    }

    m_then = now;

    return true;
}
} // namespace interface
} // namespace scene
