#pragma once

#include <QGraphicsItem>

class Path final : public QGraphicsItem {
public:
    explicit Path(QPointF const& pt);

    QRectF boundingRect() const override;

    void paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*) override;

    QPainterPath shape() const override;
};