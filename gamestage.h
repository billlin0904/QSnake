#pragma once

#include <cstdint>
#include <QTimer>

#include <QGraphicsScene>
#include <QGraphicsItem>

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

    void searchPath();

    int32_t collision(Snake const* item, QPointF const& target);

    bool eventFilter(QObject* object, QEvent* event) override;

signals:
    void gameOver();

public slots:
    void addFood();

private:    
    void addWall();   
    
    QGraphicsScene* scene_;
    Snake* snake_;
    Food* food_;
    QTimer timer_;
    std::vector<Path*> paths_;
};

