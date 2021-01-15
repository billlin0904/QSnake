#include <QPainter>

#include "gamestage.h"
#include "food.h"

Food::Food(QPointF const &pt) {
    setPos(pt);
    setData((int)GameObjectType::Object, (int)GameObjectValue::Food);
}

QRectF Food::boundingRect() const {
    return QRectF(-kSnakeSize,    -kSnakeSize,
                  kSnakeSize * 2, kSnakeSize * 2 );
}

void Food::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {
    painter->setRenderHint(QPainter::Antialiasing);
    painter->fillPath(shape(), Qt::red);
}

QPainterPath Food::shape() const {
    QPainterPath path;
    path.setFillRule(Qt::WindingFill);
    path.addRect(QRectF(0, 0, kSnakeSize, kSnakeSize));
    return path;
}
