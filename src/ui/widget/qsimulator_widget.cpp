#include "qsimulator_widget.h"

#include <QPainter>
#include <QMouseEvent>
#include <QResizeEvent>

static const QSize MinSize(320, 240);

QSimulatorWidget::QSimulatorWidget(const QControllableContainerPtr& pContainer,
                                   const QBroadcasterPtr& pBroadcaster,
                                   QWidget* parent)
    : QWidget(parent)
    , m_mass(BodyMass::Light)
    , m_radius(BodyRadius::Small)
    , m_leftButtonPressed(false)
    , m_controller(pContainer, m_mass, m_radius, this)
    , m_subscriber(pBroadcaster)
{
    connect(&m_subscriber, SIGNAL(updated()), this, SLOT(onUpdate()));
    m_subscriber.activate();

    setMinimumSize(MinSize);
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

void QSimulatorWidget::setBodyMass(BodyMass mass)
{
    m_mass = mass;
    m_controller.setupBody(m_mass, m_radius);
}

BodyMass QSimulatorWidget::bodyMass() const
{
    return m_mass;
}

void QSimulatorWidget::setBodyRadius(BodyRadius radius)
{
    m_radius = radius;
    m_controller.setupBody(m_mass, m_radius);
}

BodyRadius QSimulatorWidget::bodyRadius() const
{
    return m_radius;
}

void QSimulatorWidget::clear()
{
    m_controller.clear();
}

void QSimulatorWidget::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::MouseButton::LeftButton)
    {
        m_leftButtonPressed = true;
        mouseMoveEvent(event);
    }

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

    /**
      This update call is needed for interacting with objects
      when scene state is not playing
    */
    update();
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
