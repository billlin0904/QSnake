#include <QPainter>

#include "gamestage.h"
#include "food.h"
#include "wall.h"

Wall::Wall(QPointF const &pt) {
    setPos(pt);
    setData((int)GameObjectType::Object, (int)GameObjectValue::Wall);
}

QRectF Wall::boundingRect() const {
    return QRectF(-SNAKE_SIZE,    -SNAKE_SIZE,
                  SNAKE_SIZE * 2, SNAKE_SIZE * 2 );
}

void Wall::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {
    painter->setRenderHint(QPainter::Antialiasing);
    painter->fillPath(shape(), Qt::gray);
}

QPainterPath Wall::shape() const {
    QPainterPath path;
    path.setFillRule(Qt::WindingFill);
    path.addRect(QRectF(0, 0, SNAKE_SIZE, SNAKE_SIZE));
    return path;
}
