#pragma once

#include <cstdint>
#include <QTimer>

#include <QGraphicsScene>
#include <QGraphicsItem>

static constexpr auto SNAKE_SIZE = 15;

enum class GameObjectType {
    Object,
};

enum class GameObjectValue {
    Food,
    Wall,
};

class Snake;
class Food;

class GameStage : public QObject {
    Q_OBJECT
public:
    explicit GameStage(QGraphicsScene* screen, QObject *object);

    int32_t collision(Snake const *item, QPointF const &target);

    bool eventFilter(QObject *object, QEvent *event) override;

    void reset();

Q_SIGNALS:
    void gameOver();

public slots:
    void addFood();

private:
    void addWall();

    QGraphicsScene* screne_;
    Snake* snake_;
    Food* food_;
    QTimer timer_;
};

