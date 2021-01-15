#include <QGraphicsItem>
#include <QEvent>
#include <QKeyEvent>

#include <QDebug>

#include "a_start.h"
#include "wall.h"
#include "snake.h"
#include "food.h"
#include "path.h"
#include "gamestage.h"

GameStage::GameStage(QGraphicsScene * scene, QObject *object)
    : QObject(object)
    , scene_(scene)
    , snake_(new Snake(this)) {
    addFood();
    scene_->addItem(food_);
    scene_->addItem(snake_);
    scene_->installEventFilter(this);
    timer_.start(1000 / 33);
    addWall();
    QObject::connect(&timer_,
                     &QTimer::timeout,
                     scene_,
                     &QGraphicsScene::advance);
    searchPath();
}

static GridLocation toLocation(QPointF const &p) {
    return {
        static_cast<int>(p.x()),
        static_cast<int>(p.y()),
    };
}

static QPointF toPoint(GridLocation const& p) {
    return QPointF{
        static_cast<qreal>(p.x()),
        static_cast<qreal>(p.y()),
    };
}

void GameStage::searchPath() {
	SquareGrid<> grid{600, 600};
    
    std::unordered_map<GridLocation, GridLocation> came_from;
    std::unordered_map<GridLocation, double> cost_so_far;

    auto start(toLocation(snake_->pos()));
    auto goal(toLocation(food_->pos()));

    ManhattanDistance heuristic;

    grid.clearWalls();
	for (auto point : snake_->tail()) {
        point.setX(point.x() + kSnakeSize);
        point.setY(point.y() + kSnakeSize);
        grid.addWalls(toLocation(point));
	}

    auto result = AStart::search(
        grid,
        start,
        goal,
        came_from,
        cost_so_far,
        heuristic);
    
	for (auto* path_obj : paths_) {
        scene_->removeItem(path_obj);
	}
    paths_.clear();

    auto i = 0;

	for (auto path : result) {
		if (i++ % kSnakeSize == 0) {
            auto* path_obj = new Path(toPoint(path));
            scene_->addItem(path_obj);
            paths_.push_back(path_obj);
		}
	}
}

void GameStage::addWall() {
    // Y
    for (auto i = -300; i < 600; i+= kSnakeSize) {
        scene_->addItem(new Wall(QPointF(-300, -i)));
    }
    for (auto i = -300; i < 600; i+= kSnakeSize) {
        scene_->addItem(new Wall(QPointF(300 - kSnakeSize, -i)));
    }
    // X
    for (auto i = -300; i < 600; i+= kSnakeSize) {
        scene_->addItem(new Wall(QPointF(-i, -300)));
    }
    for (auto i = -300; i < 600; i+= kSnakeSize) {
        scene_->addItem(new Wall(QPointF(-i, 300 - kSnakeSize)));
    }
}

void GameStage::reset() {
    scene_->clear();
    snake_ = new Snake(this);
    scene_->addItem(snake_);
    addFood();
    addWall();
}

void GameStage::addFood() {
    auto x = 0;
    auto y = 0;

    const auto snake_shape = snake_->shape();

    do {
        x = static_cast<int>(qrand() % 200) / 10 - 10;
        y = static_cast<int>(qrand() % 200) / 10 - 10;
        x *= 10;
        y *= 10;
    } while (snake_shape.contains(snake_->mapFromScene(QPointF(x + kSnakeSize, y + kSnakeSize))));

    qDebug() << "Add food " << x << "," << y;

    food_ = new Food(QPointF(x, y));

    searchPath();
	
    scene_->addItem(food_);
}

bool GameStage::eventFilter(QObject *object, QEvent *event) {
    if (event->type() == QEvent::KeyPress) {
        auto* key_event = dynamic_cast<QKeyEvent*>(event);

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
        default:
            return QObject::eventFilter(object, event);
        }
        return true;
    }
	return QObject::eventFilter(object, event);
}

int32_t GameStage::collision(Snake const *item, QPointF const &target) {
    if (item->tail().contains(target)) {
        snake_->setPause();
        QTimer::singleShot(0, this, SLOT(gameOver()));
        return 0;
    }

    auto growing = 0;
    foreach (auto * colliding_item, item->collidingItems()) {
	    const auto hit_type = static_cast<GameObjectValue>(colliding_item->data(static_cast<int>(GameObjectType::Object)).toInt());
        switch (hit_type) {
        case GameObjectValue::Food:
            scene_->removeItem(colliding_item);
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
