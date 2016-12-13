#ifndef QSimulatorController_H
#define QSimulatorController_H

#include <QMap>
#include <QSize>
#include <QMutex>
#include <QRectF>
#include <QSharedPointer>

#include "interchange/subscription.h"
#include "scene/interface/controllable_container.h"

class QPainter;

enum class BodyMass { Light, Heavy };
enum class BodyRadius { Small, Big };

typedef QSharedPointer<scene::interface::ControllableContainer> QControllableContainerPtr;

class QSimulatorController
        : public QObject
{
    Q_OBJECT

    typedef physics::Object PhysObject;
    typedef QSharedPointer<PhysObject> QPhysObjectPtr;

    QPoint m_pos;

    QMutex m_lock;

    QRectF m_frameRect;

    types::value_t m_bodyMass;
    types::value_t m_bodyRadius;

    types::value_t m_lambda;

    const types::value_t m_ratio;
    const types::value_t m_sourceWidth;
    const types::value_t m_sourceHeight;

    QMap<types::obj_id, QPhysObjectPtr> m_objects;

    const QControllableContainerPtr m_pContainer;

    PhysObject* m_pSelectedObj;

public:
    QSimulatorController(const QControllableContainerPtr& pContainer,
                         BodyMass mass, BodyRadius radius,
                         QObject *parent = 0);

    void paint(QPainter& painter);

    void setupBody(BodyMass mass, BodyRadius radius);

    void setCanvasSize(const QSize& size);
    void setData(const SubscriptionData& data);

    void setMousePos(const QPoint& point, bool fixed);

    void insertAt(const QPoint& point);
    void releaseSelected();
    void removeSelected();

    void clear();

};

#endif // QSimulatorController_H
