#ifndef LOCK_H
#define LOCK_H

#include <boost/thread/lock_guard.hpp>
#include <boost/smart_ptr/detail/spinlock.hpp>

namespace concurrent
{
typedef boost::detail::spinlock spinlock;
typedef spinlock::scoped_lock guard;
} // namespace concurrent

#endif // LOCK_H
