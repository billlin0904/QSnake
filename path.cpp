#include <QPainter>

#include "gamestage.h"
#include "food.h"
#include "path.h"

Path::Path(QPointF const& pt, int index)
	: index_(index) {
    setPos(pt);
    setData(static_cast<int>(GameObjectType::Object), static_cast<int>(GameObjectValue::Path));
}

QRectF Path::boundingRect() const {
    return QRectF(-kSnakeSize, -kSnakeSize,
        kSnakeSize * 2, kSnakeSize * 2);
}

void Path::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*) {
    QColor color = Qt::white;
    color.setAlpha(20);
    painter->setRenderHint(QPainter::Antialiasing);    
    painter->fillPath(shape(), color);
}

QPainterPath Path::shape() const {
    QPainterPath path;
    path.setFillRule(Qt::WindingFill);
    path.addRect(QRectF(0, 0, kSnakeSize, kSnakeSize));
    return path;
}
