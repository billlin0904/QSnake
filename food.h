#pragma once

#include <QGraphicsItem>

class Food final : public QGraphicsItem {
public:
    explicit Food(QPointF const & pt);

    QRectF boundingRect() const override;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) override;

    QPainterPath shape() const override;
private:
};
