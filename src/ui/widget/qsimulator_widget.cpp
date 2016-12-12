#include "qsimulator_widget.h"

#include <QPainter>
#include <QMouseEvent>
#include <QResizeEvent>

QSimulatorWidget::QSimulatorWidget(const QContainerPtr& pContainer,
                                   const QBroadcasterPtr& pBroadcaster,
                                   QWidget* parent)
    : QWidget(parent)
    , m_pressedButton(Button::None)
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
    {
        m_pressedButton = Button::Left;
    }
    else if (event->button() == Qt::MouseButton::RightButton)
    {
        m_pressedButton = Button::Right;
    }
    else return;

    m_pressedPos = event->pos();
}

void QSimulatorWidget::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() == Qt::MouseButton::LeftButton &&
            Button::Left == m_pressedButton && event->pos() == m_pressedPos)
    {
        m_controller.insertAt(event->pos());
    }
    else if (event->button() == Qt::MouseButton::RightButton &&
             Button::Right == m_pressedButton && event->pos() == m_pressedPos)
    {
        m_controller.removeSelected();
    }
    else return;

    m_pressedButton = Button::None;
}

void QSimulatorWidget::mouseMoveEvent(QMouseEvent* event)
{
    m_controller.setMousePos(event->pos());
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
