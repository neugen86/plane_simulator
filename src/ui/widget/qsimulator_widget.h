#ifndef QSIMULATORWIDGET_H
#define QSIMULATORWIDGET_H

#include <QWidget>

#include "interchange/qsubscriber.h"
#include "ui/controller/qsimulator_controller.h"

class QSimulatorWidget
        : public QWidget
{
    Q_OBJECT

    bool m_leftButtonPressed;

    QSimulatorController m_controller;
    interchange::QSubscriber m_subscriber;

public:
    QSimulatorWidget(const QControllableContainerPtr& pContainer,
                     const QBroadcasterPtr& pBroadcaster,
                     QWidget *parent = 0);

    void setDuration(types::duration_t duration);
    types::duration_t duration() const;

    void clear();

protected:
    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void resizeEvent(QResizeEvent* event);
    void paintEvent(QPaintEvent* event);

private slots:
    void onUpdate();

};

#endif // QSIMULATORWIDGET_H
