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

void Snake::setDirection(const Direction dir) noexcept {
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
    auto min_x = head_.x();
    auto min_y = head_.y();
    auto max_x = head_.x();
    auto max_y = head_.y();

    for (auto p : tail_) {
        max_x = p.x() > max_x ? p.x() : max_x;
        max_y = p.y() > max_y ? p.y() : max_y;
        min_x = p.x() < min_x ? p.x() : min_x;
        min_y = p.y() < min_y ? p.y() : min_y;
    }

    const auto tl = mapFromScene(QPointF(min_x, min_y));
    const auto br = mapFromScene(QPointF(max_x, max_y));

    return QRectF(tl.x(),
                 tl.y(),
                 br.x() - tl.x() + kSnakeSize,
                 br.y() - tl.y() + kSnakeSize);
}

QPainterPath Snake::shape() const {
    QPainterPath path;

    path.setFillRule(Qt::WindingFill);
    path.addRect(QRectF(0, 0, kSnakeSize, kSnakeSize));

    for (auto p : tail_) {
        auto scene_pos = mapFromScene(p);
        path.addRect(QRectF(scene_pos.x(), scene_pos.y(), kSnakeSize, kSnakeSize));
    }
    return path;
}

void Snake::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {
    painter->fillPath(shape(), Qt::darkGreen);
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
    auto targe = head_;
    targe.rx() -= speed_;
    return targe;
}

QPointF Snake::moveRight() {
    auto targe = head_;
    targe.rx() += speed_;
    return targe;
}

QPointF Snake::moveUp() {
	auto targe = head_;
    targe.ry() -= speed_;
    return targe;
}

QPointF Snake::moveDown() {
    auto targe = head_;
    targe.ry() += speed_;
    return targe;
}
