#include "qsimulator_widget.h"

#include <QPainter>
#include <QMouseEvent>
#include <QResizeEvent>

QSimulatorWidget::QSimulatorWidget(const QControllableContainerPtr& pContainer,
                                   const QBroadcasterPtr& pBroadcaster,
                                   QWidget* parent)
    : QWidget(parent)
    , m_leftButtonPressed(false)
    , m_controller(pContainer, this)
    , m_subscriber(pBroadcaster)
{
    connect(&m_subscriber, SIGNAL(updated()), this, SLOT(onUpdate()));
    m_subscriber.activate();

    setMouseTracking(true);
}

void QSimulatorWidget::setDuration(types::duration_t duration)
{
    m_subscriber.setDuration(duration);
}

types::duration_t QSimulatorWidget::duration() const
{
    return m_subscriber.duration();
}

void QSimulatorWidget::clear()
{
    m_controller.clear();
}

void QSimulatorWidget::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::MouseButton::LeftButton)
        m_leftButtonPressed = true;

    if (event->button() == Qt::MouseButton::RightButton)
    {
        m_leftButtonPressed = false;
        m_controller.removeSelected();
    }
}

void QSimulatorWidget::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() == Qt::MouseButton::LeftButton && m_leftButtonPressed)
    {
        m_leftButtonPressed = false;
        m_controller.releaseSelected();
        m_controller.insertAt(event->pos());
    }
}

void QSimulatorWidget::mouseMoveEvent(QMouseEvent* event)
{
    m_controller.setMousePos(event->pos(), m_leftButtonPressed);
}

void QSimulatorWidget::resizeEvent(QResizeEvent* event)
{
    m_controller.setCanvasSize(event->size());
}

void QSimulatorWidget::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    m_controller.paint(painter);
}

void QSimulatorWidget::onUpdate()
{
    m_controller.setData(m_subscriber.data());
    update();
}
