#ifndef SUBSCRIPTION_H
#define SUBSCRIPTION_H

#include <list>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition_variable.hpp>

#include "physics/types.h"
#include "concurrent/lock.h"
#include "concurrent/event.h"

namespace interchange
{
typedef std::list<physics::ObjectPtr> ObjectList;

class SubscriptionBase
{
public:
    virtual ~SubscriptionBase() {}
};

class SubscriptionReader
        : public virtual SubscriptionBase
{
public:
    virtual ObjectList get() const = 0;
    virtual void wait() const = 0;
};

class SubscriptionWriter
        : public virtual SubscriptionBase
{
public:
    virtual void set(const ObjectList& list) = 0;
    virtual void notify() const = 0;
};

class Subscription
        : public SubscriptionReader
        , public SubscriptionWriter
{
    ObjectList m_list;

    concurrent::event m_event;

    mutable concurrent::spinlock m_listLock;
    mutable concurrent::spinlock m_eventLock;

public:
    void set(const ObjectList& list);
    ObjectList get() const;

    void notify() const;
    void wait() const;

};
} // namespace interchange

#endif // SUBSCRIPTION_H
