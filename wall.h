#pragma once

#include <QGraphicsItem>

class Wall final : public QGraphicsItem {
public:
    explicit Wall(QPointF const & pt);

    QRectF boundingRect() const override;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) override;

    QPainterPath shape() const override;
};
