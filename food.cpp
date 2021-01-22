#include <QPainter>
#include <QGraphicsOpacityEffect>

#include "gamestage.h"
#include "food.h"

Food::Food(QPointF const &pt) {
    setPos(pt);
    setData(static_cast<int>(GameObjectType::Object), static_cast<int>(GameObjectValue::Food));
    animation_ = new QPropertyAnimation(this, "opacity");
    animation_->setDuration(200);
    animation_->setStartValue(0.3);
    animation_->setEndValue(1.0);
    animation_->setLoopCount(-1);
    animation_->start();    
}

qreal Food::opacity() const {
    return opacity_;
}

void Food::setOpacity(qreal value) {
    opacity_ = value;
    update();
}

QRectF Food::boundingRect() const {
    return QRectF(-kSnakeSize,    -kSnakeSize,
                  kSnakeSize * 2, kSnakeSize * 2 );
}

void Food::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setOpacity(opacity());
    painter->setBrush(Qt::red);
    painter->drawEllipse(QRectF(0, 0, kSnakeSize, kSnakeSize));
}

QPainterPath Food::shape() const {
    QPainterPath path;
    path.setFillRule(Qt::WindingFill);
    path.addRect(QRectF(0, 0, kSnakeSize, kSnakeSize));
    return path;
}
