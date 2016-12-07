#ifndef PERIODIC_H
#define PERIODIC_H

#include <boost/chrono.hpp>

#include "physics/types.h"
#include "concurrent/lock.h"

namespace scene
{
namespace interface
{
class Periodic
{
    static const types::duration_t DefaultDuration;

    mutable concurrent::spinlock m_lock;
    mutable concurrent::spinlock m_trueLock;

    boost::chrono::milliseconds m_duration;
    mutable boost::chrono::milliseconds m_trueDuration;

    mutable boost::chrono::high_resolution_clock::time_point m_then;

public:
    Periodic(types::duration_t duration = DefaultDuration);
    virtual ~Periodic() {}

    void setDuration(types::duration_t duration);
    types::duration_t trueDuration() const;
    types::duration_t duration() const;

protected:
    bool expired() const;

};
} // namespace interface
} // namespace scene

#endif // PERIODIC_H
