#ifndef SUBSCRIPTION_H
#define SUBSCRIPTION_H

#include "physics/types.h"

#include <list>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition_variable.hpp>

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
    virtual ObjectList get() = 0;
    virtual void wait() = 0;
};

class SubscriptionWriter
        : public virtual SubscriptionBase
{
public:
    virtual void set(const ObjectList& list) = 0;
    virtual void notify() = 0;
};

class Subscription
        : public SubscriptionReader
        , public SubscriptionWriter
{
    bool m_notify;

    ObjectList m_list;

    boost::mutex m_listGuard;
    boost::mutex m_notifyGuard;

    boost::condition_variable m_notifyCondition;

public:
    void set(const ObjectList& list);
    ObjectList get();

    void notify();
    void wait();

};
} // namespace interchange

#endif // SUBSCRIPTION_H
