#ifndef SUBSCRIPTION_H
#define SUBSCRIPTION_H

#include <list>

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

class SubscriptionConsumer
        : public SubscriptionBase
{
public:
    virtual ObjectList get() const = 0;
};

class SubscriptionProducer
        : public SubscriptionBase
{
public:
    virtual void set(const ObjectList& list) = 0;
};

class Subscription
        : public SubscriptionConsumer
        , public SubscriptionProducer
{
    ObjectList m_list;
    concurrent::event m_event;
    mutable concurrent::spinlock m_lock;

public:
    Subscription();
    ~Subscription();

    ObjectList get() const;
    void set(const ObjectList& list);

private:
    void wait() const ;
    void notify() const;

};
} // namespace interchange

#endif // SUBSCRIPTION_H
