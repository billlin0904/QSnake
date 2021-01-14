#include <QPainter>

#include "gamestage.h"
#include "snake.h"

Snake::Snake(GameStage *stage)
    : dir_(Direction::NoMove)
    , growing_(7)
    , stage_(stage) {
}

QPointF Snake::head() const {
    return head_;
}

void Snake::setPause(bool pause) {
    is_pause_ = pause;
}

QList<QPointF> const & Snake::tail() const {
    return tail_;
}

Direction Snake::direction() const noexcept {
    return dir_;
}

void Snake::setDirection(Direction dir) noexcept {
    if (dir_ == Direction::MoveLeft && dir == Direction::MoveRight) {
        return;
    }
    if (dir_ == Direction::MoveRight && dir == Direction::MoveLeft) {
        return;
    }
    if (dir_ == Direction::MoveUp && dir == Direction::MoveDown) {
        return;
    }
    if (dir_ == Direction::MoveDown && dir == Direction::MoveUp) {
        return;
    }
    dir_ = dir;
}

QRectF Snake::boundingRect() const {
    auto minX = head_.x();
    auto minY = head_.y();
    auto maxX = head_.x();
    auto maxY = head_.y();

    for (auto p : tail_) {
        maxX = p.x() > maxX ? p.x() : maxX;
        maxY = p.y() > maxY ? p.y() : maxY;
        minX = p.x() < minX ? p.x() : minX;
        minY = p.y() < minY ? p.y() : minY;
    }

    const auto tl = mapFromScene(QPointF(minX, minY));
    const auto br = mapFromScene(QPointF(maxX, maxY));

    return QRectF(tl.x(),
                 tl.y(),
                 br.x() - tl.x() + SNAKE_SIZE,
                 br.y() - tl.y() + SNAKE_SIZE);
}

QPainterPath Snake::shape() const {
    QPainterPath path;

    path.setFillRule(Qt::WindingFill);
    path.addRect(QRectF(0, 0, SNAKE_SIZE, SNAKE_SIZE));

    for (auto p : tail_) {
        auto itemp = mapFromScene(p);
        path.addRect(QRectF(itemp.x(), itemp.y(), SNAKE_SIZE, SNAKE_SIZE));
    }
    return path;
}

void Snake::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {
    painter->fillPath(shape(), Qt::yellow);
}

void Snake::advance(int step) {
    if (dir_ == Direction::NoMove) {
        return;
    }

    if (is_pause_) {
        return;
    }

    if (growing_ > 0) {
        auto tail = head_;
        tail_ << tail;
        growing_ -= 1;
    } else {
        tail_.removeFirst();
        tail_ << head_;
    }

    QPointF target;

    switch (dir_) {
    case Direction::MoveUp:
        target = moveUp();
        break;
    case Direction::MoveDown:
        target = moveDown();
        break;
    case Direction::MoveLeft:
        target = moveLeft();
        break;
    case Direction::MoveRight:
        target = moveRight();
        break;
    case Direction::NoMove:
        break;
    }

    head_ = target;
    setPos(head_);

    growing_ += stage_->collision(this, target);
}

QPointF Snake::moveLeft() {
    QPointF targe = head_;
    targe.rx() -= 2;
    return targe;
}

QPointF Snake::moveRight() {
    QPointF targe = head_;
    targe.rx() += 2;
    return targe;
}

QPointF Snake::moveUp() {
    QPointF targe = head_;
    targe.ry() -= 2;
    return targe;
}

QPointF Snake::moveDown() {
    QPointF targe = head_;
    targe.ry() += 2;
    return targe;
}
