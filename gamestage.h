#pragma once

#include <cstdint>
#include <QTimer>
#include <memory>

#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QGraphicsItemAnimation>
#include <QTimeLine>

static constexpr auto kSnakeSize = 15;

enum class GameObjectType {
    Object,
};

enum class GameObjectValue {
    Food,
    Wall,
	Path,
};

class Snake;
class Food;
class Path;

class GameStage : public QObject {
    Q_OBJECT
public:
    explicit GameStage(QGraphicsScene* scene, QObject *object);

    void reset();

    void clear();

    void searchPath();

    int32_t collision(Snake const* item, QPointF const& target);

    bool eventFilter(QObject* object, QEvent* event) override;

signals:
    void gameOver();

public slots:
    void addFood();

private:    
    void addWall();

    void removePath();
    
    QGraphicsScene* scene_{nullptr};
    Snake* snake_{ nullptr };
    Food* food_{ nullptr };
    QTimer timer_;
    std::vector<std::unique_ptr<Path>> paths_;
};

