#ifndef QPAINTCONTROLLER_H
#define QPAINTCONTROLLER_H

#include <QSize>
#include <QObject>

#include "interchange/subscription.h"

class QPainter;

namespace controller
{
template <typename T>
bool less(T lhs, T rhs)
{
    return rhs - lhs > std::numeric_limits<T>::epsilon();
}

class QPaintController
        : public QObject
{
    Q_OBJECT

    class Rectangular
    {
        double m_ratio;
        double m_width;
        double m_height;

    public:
        explicit Rectangular(double width = 0., double height = 0.)
            : m_ratio(width / height)
            , m_width(width), m_height(height) {}
        double ratio() const { return m_ratio; }
        double width() const { return m_width; }
        double height() const { return m_height; }
        void setWidth(double width) { m_width = width; }
        void setHeight(double height) { m_height = height; }
    };

    double m_ratio;

    Rectangular m_rectProjection;
    const Rectangular m_rectSource;

public:
    QPaintController(double sourceWidth, double sourceHeight, QObject *parent = 0);

    void setCanvasSize(const QSize& size);
    void setData(const interchange::ObjectList& list);
    void paint(QPainter& painter);

private:

};
} // namespace controller

#endif // QPAINTCONTROLLER_H
