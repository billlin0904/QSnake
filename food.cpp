#include <QPainter>

#include "gamestage.h"
#include "food.h"

Food::Food(QPointF const &pt) {
    setPos(pt);
    setData((int)GameObjectType::Object, (int)GameObjectValue::Food);
}

QRectF Food::boundingRect() const {
    return QRectF(-SNAKE_SIZE,    -SNAKE_SIZE,
                  SNAKE_SIZE * 2, SNAKE_SIZE * 2 );
}

void Food::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {
    painter->setRenderHint(QPainter::Antialiasing);
    painter->fillPath(shape(), Qt::red);
}

QPainterPath Food::shape() const {
    QPainterPath path;
    path.setFillRule(Qt::WindingFill);
    path.addRect(QRectF(0, 0, SNAKE_SIZE, SNAKE_SIZE));
    return path;
}
