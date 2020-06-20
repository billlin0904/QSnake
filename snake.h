#pragma once

#include <memory>
#include <deque>

#include <QGraphicsItem>

enum class Direction : uint8_t {
    NoMove,
    MoveLeft,
    MoveRight,
    MoveUp,
    MoveDown
};

class GameStage;

class Snake final : public QGraphicsItem {
public:
    explicit Snake(GameStage* stage);

    void setDirection(Direction dir) noexcept;

    Direction direction() const noexcept;

    QRectF boundingRect() const override;

    QPainterPath shape() const override;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) override;

    void advance(int step) override;

    void setPause(bool pause = true);

    QPointF head() const;

    QList<QPointF> const & tail() const;

private:
    QPointF moveLeft();

    QPointF moveRight();

    QPointF moveUp();

    QPointF moveDown();

    bool is_pause_{false};
    Direction dir_;
    int32_t growing_;
    GameStage* stage_;
    QPointF head_;
    QList<QPointF> tail_;
};

