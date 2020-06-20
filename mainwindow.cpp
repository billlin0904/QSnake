#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMessageBox>

#include "gamestage.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , scene_(new QGraphicsScene(this))
    , view_(new QGraphicsView(scene_, this))
    , stage_(new GameStage(scene_, this)) {
    ui->setupUi(this);
    setCentralWidget(view_);
    setFixedSize(600, 600);
    setWindowTitle("QSnake");

    //QPixmap bg(SNAKE_SIZE, SNAKE_SIZE);
    //QPainter p(&bg);
    //p.setBrush(QBrush(Qt::gray));
    //p.drawRect(0, 0, SNAKE_SIZE, SNAKE_SIZE);
    //view_->setBackgroundBrush(QBrush(bg));
    view_->setBackgroundBrush(QBrush(Qt::black));
    view_->viewport()->setFocusProxy(nullptr);

    scene_->setSceneRect(-100, -100, 200, 200);
    QObject::connect(stage_, &GameStage::gameOver, this, &MainWindow::onGameOver);
}

void MainWindow::adjustViewSize() {
    view_->fitInView(scene_->sceneRect(), Qt::KeepAspectRatioByExpanding);
}

void MainWindow::onGameOver() {
    QMessageBox::information(nullptr,
                             "",
                             "GameOver",
                             QMessageBox::Yes | QMessageBox::No,
                             QMessageBox::Yes);
    stage_->reset();
}

MainWindow::~MainWindow() {
    delete ui;
}

