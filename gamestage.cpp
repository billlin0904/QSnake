#include <random>

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

static GridLocation toLocation(QPointF const& p) {
    return {
        static_cast<int>(p.x()),
        static_cast<int>(p.y()),
    };
}

static GridRect fromQRectF(QRectF const& r) {
    return {
        static_cast<int>(r.left()),
        static_cast<int>(r.top()),
        static_cast<int>(r.right()),
        static_cast<int>(r.bottom()),
    };
}

static QPointF toPoint(GridLocation const& p) {
    return QPointF{
        static_cast<qreal>(p.x()),
        static_cast<qreal>(p.y()),
    };
}

GameStage::GameStage(QGraphicsScene * scene, QObject *object)
    : QObject(object)
    , scene_(scene) {
    reset();
    scene_->installEventFilter(this);
    timer_.start(33);
    QObject::connect(&timer_,
                     &QTimer::timeout,
                     scene_,
                     &QGraphicsScene::advance);
    searchPath();
}

void GameStage::clear() {
    removePath();
	if (snake_ != nullptr) {
        scene_->removeItem(snake_);
	}
    if (food_ != nullptr) {
        scene_->removeItem(food_);
    }    
}

void GameStage::removePath() {
    for (auto& path_obj : paths_) {
        scene_->removeItem(path_obj.get());
    }
    paths_.clear();
}

void GameStage::searchPath() {
	SquareGrid<> grid{600, 600};
    
    std::unordered_map<GridLocation, GridLocation> came_from;
    std::unordered_map<GridLocation, double> cost_so_far;

    auto start(toLocation(snake_->pos()));
    auto goal(toLocation(food_->pos()));

    ManhattanDistance heuristic;

    auto result = AStart::search(
        grid,
        start,
        goal,
        came_from,
        cost_so_far,
        heuristic);
    
    removePath();

    auto i = 0;

	for (auto path : result) {
		if (i++ % kSnakeSize == 0) {
            auto path_obj = std::make_unique<Path>(toPoint(path), i);
            scene_->addItem(path_obj.get());
            paths_.push_back(std::move(path_obj));
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
    clear();
    scene_->clear();    
    snake_=new Snake(this);
    scene_->addItem(snake_);
    addFood();
    addWall();
}

void GameStage::addFood() {
    auto x = 0;
    auto y = 0;

    const auto snake_shape = snake_->shape();

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(-1.0, 1.0);
	
    do {
        x = static_cast<int>(dis(gen) * 300);
        y = static_cast<int>(dis(gen) * 300);
    } while (snake_shape.contains(snake_->mapFromScene(QPointF(x + kSnakeSize, y + kSnakeSize))));

    qDebug() << "Add food " << x << "," << y;
	
    food_ = new Food(QPointF(x, y));	
    scene_->addItem(food_);
}

bool GameStage::eventFilter(QObject *object, QEvent *event) {
    if (event->type() == QEvent::KeyPress) {
        auto* key_event = dynamic_cast<QKeyEvent*>(event);
        switch (key_event->key()) {
        case Qt::Key_Left:
            snake_->setDirection(Direction::MoveLeft);
            searchPath();
            break;
        case Qt::Key_Right:
            snake_->setDirection(Direction::MoveRight);
            searchPath();
            break;
        case Qt::Key_Up:
            snake_->setDirection(Direction::MoveUp);
            searchPath();
            break;
        case Qt::Key_Down:
            snake_->setDirection(Direction::MoveDown);
            searchPath();
            break;
        case Qt::Key_Space:
            snake_->setPause(false);
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
            searchPath();
            //snake_->setPause();
            break;
        case GameObjectValue::Wall:
            snake_->setPause();
            paths_.clear();
            QTimer::singleShot(0, this, SLOT(gameOver()));
            return 0;
        }
    }
    //searchPath();	
    return growing;
}
