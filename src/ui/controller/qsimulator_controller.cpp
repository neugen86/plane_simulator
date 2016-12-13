#include "qsimulator_controller.h"

#include <QPainter>

#include "physics/physics.h"

static const int FrameBorderWidth(1);

namespace masses
{
    static const types::value_t Light(10.);
    static const types::value_t Heavy(30.);
    static const types::value_t Extra(50.);
} // namespace masses

namespace radiuses
{
    static const types::value_t Small(50.);
    static const types::value_t Big(70.);
    static const types::value_t Extra(90.);
} // namespace radiuses

namespace colors
{
    static const QColor Gray("#404040");
    static const QColor Black("#000000");
    static const QColor White("#F4F4F4");

    static const QColor color0("#BDE3FF");
    static const QColor color1("#5493E3");
    static const QColor color2("#FFA273");
    static const QColor color3("#FAD300");
    static const QColor color4("#89D011");
    static const QColor color5("#FF70D2");
    static const QColor color6("#FF5050");
    static const QColor color7("#00B19E");
    static const QColor color8("#9554D2");
} // namespace colors

QColor bodyColor(types::value_t mass, types::value_t radius)
{
    if (less(mass, masses::Light))
    {
        if (less(radius, radiuses::Small)) return colors::color0;
        if (less(radius, radiuses::Big)) return colors::color1;
        return colors::color2;
    }
    if (less(mass, masses::Heavy))
    {
        if (less(radius, radiuses::Small)) return colors::color3;
        if (less(radius, radiuses::Big)) return colors::color4;
        return colors::color5;
    }
    if (less(radius, radiuses::Small)) return colors::color6;
    if (less(radius, radiuses::Big)) return colors::color7;
    return colors::color8;
}

void drawPath(QPainter& painter, const QPainterPath& path, qreal width)
{
    painter.save();
    painter.setPen(QPen(colors::Gray, width, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
    painter.setBrush(colors::White);
    painter.drawPath(path);
    painter.restore();
}

void drawCircle(QPainter& painter, const QPointF& pos, qreal radius, const QColor& color, qreal width)
{
    painter.save();
    painter.setPen(QPen(colors::Gray, width, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
    painter.setBrush(color);
    painter.drawEllipse(pos, radius, radius);
    painter.restore();
}

QSimulatorController::QSimulatorController(const QControllableContainerPtr& pContainer,
                                           BodyMass mass, BodyRadius radius,
                                           QObject* parent)
    : QObject(parent)
    , m_lambda(physics::constants::PositiveUnit)
    , m_ratio(pContainer->width() / pContainer->height())
    , m_sourceWidth(pContainer->width())
    , m_sourceHeight(pContainer->height())
    , m_pContainer(pContainer)
    , m_pSelectedObj(nullptr)
{
    setupBody(mass, radius);
}

void QSimulatorController::paint(QPainter& painter)
{
    QPainterPath framePath;
    framePath.addRect(m_frameRect);

    const qreal lineWidth = m_lambda * 3;

    drawPath(painter, framePath, FrameBorderWidth);

    {
        bool selectedIsInFocus = false;
        PhysObject* pSelectedObj = m_pSelectedObj;

        m_pSelectedObj = nullptr;

        QMutexLocker guard(&m_lock);

        foreach (QPhysObjectPtr pObj, m_objects.values())
        {
            const types::value_t radius = pObj->radius() * m_lambda;

            const QPointF pos(m_frameRect.x() + pObj->position().x() * m_lambda,
                              m_frameRect.y() + pObj->position().y() * m_lambda);

            const types::value_t d = distance(algebra::Point(pos.x(), pos.y()),
                                              algebra::Point(m_pos.x(), m_pos.y()));

            const bool currentIsInFocus = less(d, radius);

            if (pObj.data() == pSelectedObj && currentIsInFocus)
                selectedIsInFocus = true;

            if (!selectedIsInFocus && !m_pSelectedObj && currentIsInFocus)
                m_pSelectedObj = pObj.data();

            const QColor color = bodyColor(pObj->mass(), pObj->radius());
            drawCircle(painter, pos, radius, color, lineWidth);
        }

        if (selectedIsInFocus)
            m_pSelectedObj = pSelectedObj;

        if (m_pSelectedObj != 0)
        {
            const types::value_t radius = m_pSelectedObj->radius() * m_lambda;

            const QPointF pos(m_frameRect.x() + m_pSelectedObj->position().x() * m_lambda,
                              m_frameRect.y() + m_pSelectedObj->position().y() * m_lambda);

            drawCircle(painter, pos, radius, colors::Gray, lineWidth);
        }
    }
}

void QSimulatorController::setupBody(BodyMass mass, BodyRadius radius)
{
    switch (mass)
    {
    case BodyMass::Light:
        m_bodyMass = masses::Light;
        break;
    case BodyMass::Heavy:
        m_bodyMass = masses::Heavy;
        break;
    default:
        m_bodyMass = masses::Extra;
    }

    switch (radius)
    {
    case BodyRadius::Small:
        m_bodyRadius = radiuses::Small;
        break;
    case BodyRadius::Big:
        m_bodyRadius = radiuses::Big;
        break;
    default:
        m_bodyRadius = radiuses::Extra;
    }
}

void QSimulatorController::setCanvasSize(const QSize& size)
{
    const types::value_t canvasWidth = size.width();
    const types::value_t canvasHeight = size.height();
    const types::value_t canvasRatio = canvasWidth / canvasHeight;

    const bool canvasRatioLessSourceRatio = less(canvasRatio, m_ratio);
    const bool sourceRatioLessThanUnit = less(m_ratio, physics::constants::PositiveUnit);

    const bool byHeight = (sourceRatioLessThanUnit && canvasRatioLessSourceRatio) ||
            (!sourceRatioLessThanUnit && !canvasRatioLessSourceRatio);

    m_lambda = byHeight
            ? canvasHeight / m_sourceHeight
            : canvasWidth / m_sourceWidth;

    const types::value_t newWidth = m_sourceWidth * m_lambda - FrameBorderWidth - 1;
    const types::value_t newHeight = m_sourceHeight * m_lambda - FrameBorderWidth - 1;

    m_frameRect.setX((canvasWidth - newWidth) / 2);
    m_frameRect.setY((canvasHeight - newHeight) / 2);

    m_frameRect.setWidth(newWidth);
    m_frameRect.setHeight(newHeight);
}

void QSimulatorController::setData(const SubscriptionData& data)
{
    QMutexLocker guard(&m_lock);

    for (auto it = data.removedIds.begin(); it != data.removedIds.end(); ++it)
    {
        m_objects.remove(*it);
    }

    foreach (const physics::ObjectPtr& pObj, data.objectList)
    {
        const types::obj_id id = pObj->id();
        auto existing = m_objects.find(id);

        if (existing != m_objects.end())
        {
            (*existing)->setPosition(pObj->position());
        }
        else
        {
            m_objects.insert(id, QPhysObjectPtr(new PhysObject(*pObj)));
        }
    }
}

void QSimulatorController::setMousePos(const QPoint& point, bool fixed)
{
    if (!m_frameRect.contains(point.x(), point.y()))
        return;

    m_pos = point;

    if (m_pSelectedObj && fixed)
    {
        const algebra::Point pos((point.x() - m_frameRect.x()) / m_lambda,
                                 (point.y() - m_frameRect.y()) / m_lambda);

        m_pContainer->grabObject(m_pSelectedObj->id(), pos);
    }
}

void QSimulatorController::insertAt(const QPoint& point)
{
    if (!m_frameRect.contains(point.x(), point.y()))
        return;

    const algebra::Point pos((point.x() - m_frameRect.x()) / m_lambda,
                             (point.y() - m_frameRect.y()) / m_lambda);
    m_pContainer->insertObject(PhysObject(pos, m_bodyRadius, m_bodyMass));
}

void QSimulatorController::releaseSelected()
{
    if (!m_pSelectedObj)
        return;

    m_pContainer->releaseObject(m_pSelectedObj->id());
}

void QSimulatorController::removeSelected()
{
    if (!m_pSelectedObj)
        return;

    m_pContainer->removeObject(m_pSelectedObj->id());
}

void QSimulatorController::clear()
{
    m_pSelectedObj = nullptr;
    m_pContainer->removeAll();
    m_objects.clear();
}
