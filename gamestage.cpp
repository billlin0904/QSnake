#include <QGraphicsItem>
#include <QEvent>
#include <QKeyEvent>

#include <QDebug>

#include "a_start.h"
#include "wall.h"
#include "snake.h"
#include "food.h"
#include "gamestage.h"

GameStage::GameStage(QGraphicsScene *screne, QObject *object)
    : QObject(object)
    , screne_(screne)
    , snake_(new Snake(this)) {
    food_ = new Food(QPointF(0, -50));
    screne_->addItem(food_);
    screne_->addItem(snake_);
    screne_->installEventFilter(this);
    timer_.start(1000 / 33);
    addWall();
    QObject::connect(&timer_,
                     &QTimer::timeout,
                     screne_,
                     &QGraphicsScene::advance);
}

void GameStage::calcPath() {
    SquareGrid grid{ 200, 200 };
    Heuristic heuristic;

    std::unordered_map<GridLocation, GridLocation> came_from;
    std::unordered_map<GridLocation, double> cost_so_far;

    auto start = GridLocation{
    	static_cast<int>(snake_->pos().x()),
        static_cast<int>(snake_->pos().y()),
    };

    auto goal = GridLocation{
        static_cast<int>(food_->pos().x()),
        static_cast<int>(food_->pos().y()),
    };
	
    astart(grid, start, goal, came_from, cost_so_far, heuristic);
    auto path = reconstructPath(start, goal, came_from);
}

void GameStage::addWall() {
    // Y
    for (auto i = -300; i < 600; i+= SNAKE_SIZE) {
        screne_->addItem(new Wall(QPointF(-300, -i)));
    }
    for (auto i = -300; i < 600; i+= SNAKE_SIZE) {
        screne_->addItem(new Wall(QPointF(300 - SNAKE_SIZE, -i)));
    }
    // X
    for (auto i = -300; i < 600; i+= SNAKE_SIZE) {
        screne_->addItem(new Wall(QPointF(-i, -300)));
    }
    for (auto i = -300; i < 600; i+= SNAKE_SIZE) {
        screne_->addItem(new Wall(QPointF(-i, 300 - SNAKE_SIZE)));
    }
}

void GameStage::reset() {
    screne_->clear();
    snake_ = new Snake(this);
    screne_->addItem(snake_);
    addFood();
    addWall();
}

void GameStage::addFood() {
    auto x = 0;
    auto y = 0;

    auto snake_shape = snake_->shape();

    do {
        x = static_cast<int>(qrand() % 200) / 10 - 10;
        y = static_cast<int>(qrand() % 200) / 10 - 10;
        x *= 10;
        y *= 10;
    } while (snake_shape.contains(snake_->mapFromScene(QPointF(x + 5, y + 5))));

    qDebug() << "Add food " << x << "," << y;

    food_ = new Food(QPointF(x, y));
    screne_->addItem(food_);
}

bool GameStage::eventFilter(QObject *object, QEvent *event) {
    if (event->type() == QEvent::KeyPress) {
        auto* key_event = static_cast<QKeyEvent*>(event);

        switch (key_event->key()) {
        case Qt::Key_Left:
            snake_->setDirection(Direction::MoveLeft);
            break;
        case Qt::Key_Right:
            snake_->setDirection(Direction::MoveRight);
            break;
        case Qt::Key_Up:
            snake_->setDirection(Direction::MoveUp);
            break;
        case Qt::Key_Down:
            snake_->setDirection(Direction::MoveDown);
            break;
        case Qt::Key_Space:
            break;
        }

        calcPath();

        return true;
    } else {
        return QObject::eventFilter(object, event);
    }
}

int32_t GameStage::collision(Snake const *item, QPointF const &target) const {
    if (item->tail().contains(target)) {
        snake_->setPause();
        QTimer::singleShot(0, this, SLOT(gameOver()));
        return 0;
    }

    auto growing = 0;
    foreach (auto * colliding_item, item->collidingItems()) {
        switch (static_cast<GameObjectValue>(colliding_item->data(static_cast<int>(GameObjectType::Object)).toInt())) {
        case GameObjectValue::Food:
            screne_->removeItem(colliding_item);
            QTimer::singleShot(0, this, SLOT(addFood()));
            growing += 30;
            qDebug() << "eat food " << colliding_item->x() << "," << colliding_item->y();
            break;
        case GameObjectValue::Wall:
            snake_->setPause();
            QTimer::singleShot(0, this, SLOT(gameOver()));
            return 0;
        }
    }
    return growing;
}
