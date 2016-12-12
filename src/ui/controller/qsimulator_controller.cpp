#include "qsimulator_controller.h"

#include <QPainter>

#include "physics/physics.h"

static const int FrameBorderWidth(1);

QSimulatorController::QSimulatorController(const QControllableContainerPtr& pContainer, QObject* parent)
    : QObject(parent)
    , m_lambda(physics::constants::PositiveUnit)
    , m_rectSource(pContainer->width(), pContainer->height())
    , m_pContainer(pContainer)
    , m_pSelectedObj(nullptr)
{
}

void QSimulatorController::paint(QPainter& painter)
{
    QPainterPath framePath;
    framePath.addRect(m_offset.x(), m_offset.y(),
                      m_rectProjection.width(), m_rectProjection.height());

    painter.save();

    painter.setPen(QPen(QColor(0, 0, 0), FrameBorderWidth,Qt::SolidLine,
                        Qt::FlatCap, Qt::MiterJoin));
    painter.setBrush(QColor(250, 250, 250));

    painter.drawPath(framePath);

    painter.restore();

    {
        m_pSelectedObj = nullptr;

        QMutexLocker guard(&m_lock);

        foreach (QPhysObjectPtr pObj, m_objects.values())
        {
            const types::value_t radius = pObj->radius() * m_lambda;

            const QPointF pos(pObj->position().x() * m_lambda + m_offset.x(),
                              pObj->position().y() * m_lambda + m_offset.y());

            const types::value_t d = distance(algebra::Point(pos.x(), pos.y()),
                                              algebra::Point(m_pos.x(), m_pos.y()));

            const bool selected = !m_pSelectedObj && less(d, radius);

            if (selected)
                m_pSelectedObj = pObj.data();

            painter.save();

            painter.setPen(QPen(QColor(0, 0, 0), 3 * m_lambda, Qt::SolidLine,
                                Qt::FlatCap, Qt::MiterJoin));
            painter.setBrush(selected ? QColor(200, 50, 50) : QColor(200, 200, 200));

            painter.drawEllipse(pos, radius, radius);

            painter.restore();
        }
    }
}

void QSimulatorController::setCanvasSize(const QSize& size)
{
    const types::value_t sourceRatio = m_rectSource.ratio();
    const Rectangular rectCanvas(size.width(), size.height());

    const bool sourceRatioLessThanUnit =
            less(sourceRatio, physics::constants::PositiveUnit);
    const bool canvasRatioLessSourceRatio =
            less(rectCanvas.ratio(), sourceRatio);

    const bool byHeight = (sourceRatioLessThanUnit && canvasRatioLessSourceRatio) ||
            (!sourceRatioLessThanUnit && !canvasRatioLessSourceRatio);

    m_lambda = byHeight
            ? rectCanvas.height() / m_rectSource.height()
            : rectCanvas.width() / m_rectSource.width();

    m_rectProjection = Rectangular(m_rectSource.width() * m_lambda - FrameBorderWidth - 1,
                                   m_rectSource.height() * m_lambda - FrameBorderWidth - 1);

    m_offset.setX((size.width() - m_rectProjection.width()) / 2);
    m_offset.setY((size.height() - m_rectProjection.height()) / 2);
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
    m_pos = point;

    if (m_pSelectedObj && fixed)
    {
        const algebra::Point pos((point.x() - m_offset.x()) / m_lambda,
                                 (point.y() - m_offset.y()) / m_lambda);

        m_pContainer->grabObject(m_pSelectedObj->id(), pos);
    }
}

void QSimulatorController::insertAt(const QPoint& point)
{
    const algebra::Point pos((point.x() - m_offset.x()) / m_lambda,
                             (point.y() - m_offset.y()) / m_lambda);
    m_pContainer->insertObject(PhysObject(pos, 50.));
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
