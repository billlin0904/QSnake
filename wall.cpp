#include <QPainter>

#include "gamestage.h"
#include "food.h"
#include "wall.h"

Wall::Wall(QPointF const &pt) {
    setPos(pt);
    setData(static_cast<int>(GameObjectType::Object), static_cast<int>(GameObjectValue::Wall));
}

QRectF Wall::boundingRect() const {
    return QRectF(-kSnakeSize,    -kSnakeSize,
                  kSnakeSize * 2, kSnakeSize * 2 );
}

void Wall::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {
    painter->setRenderHint(QPainter::Antialiasing);
    painter->fillPath(shape(), Qt::gray);
    painter->drawRect(boundingRect());
}

QPainterPath Wall::shape() const {
    QPainterPath path;
    path.setFillRule(Qt::WindingFill);
    path.addRect(QRectF(0, 0, kSnakeSize, kSnakeSize));
    return path;
}
