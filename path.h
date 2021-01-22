#pragma once

#include <QGraphicsItem>

class Path final : public QGraphicsItem {
public:
    explicit Path(QPointF const& pt, int index);

    QRectF boundingRect() const override;

    void paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*) override;

    QPainterPath shape() const override;
private:
    int index_;
};