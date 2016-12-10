#ifndef SUBSCRIPTION_H
#define SUBSCRIPTION_H

#include <list>

#include <boost/chrono.hpp>

#include "physics/types.h"
#include "concurrent/lock.h"
#include "concurrent/event.h"

namespace interchange
{
typedef std::list<physics::ObjectPtr> ObjectList;

class SubscriptionConsumer
{
public:
    virtual ~SubscriptionConsumer() {}

    virtual void setDuration(types::duration_t duration) = 0;
    virtual types::duration_t duration() const = 0;

    virtual ObjectList get() const = 0;
};

class SubscriptionProducer
{
public:
    virtual ~SubscriptionProducer() {}

    virtual void set(const ObjectList& list) = 0;

    virtual bool expired() const = 0;
};

class Subscription
        : public SubscriptionConsumer
        , public SubscriptionProducer
{
    ObjectList m_list;

    concurrent::event m_event;

    boost::chrono::milliseconds m_duration;

    mutable concurrent::spinlock m_dataLock;
    mutable concurrent::spinlock m_durationLock;

    mutable boost::chrono::high_resolution_clock::time_point m_then;

public:
    static const types::duration_t DefaultDuration;

    Subscription(types::duration_t duration = DefaultDuration);
    ~Subscription();

    virtual void setDuration(types::duration_t duration);
    virtual types::duration_t duration() const;

    void set(const ObjectList& list);
    ObjectList get() const;

    bool expired() const;

private:
    void wait() const ;
    void notify() const;

};
} // namespace interchange

#endif // SUBSCRIPTION_H
