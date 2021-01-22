#pragma once

#include <QGraphicsItem>
#include <QPropertyAnimation>

class Food final : public QObject, public QGraphicsItem {
Q_OBJECT
    Q_PROPERTY(qreal opacity READ opacity WRITE setOpacity)
    Q_INTERFACES(QGraphicsItem)
public:
    explicit Food(QPointF const & pt);

    QRectF boundingRect() const override;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) override;

    QPainterPath shape() const override;

    qreal opacity() const;
	
    void setOpacity(qreal value);
private:
    qreal opacity_{ 0 };
    QPropertyAnimation* animation_;
};
