#pragma once

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class QGraphicsView;
class QGraphicsScene;
class GameStage;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);

    ~MainWindow();

public slots:
    void onGameOver();

private:
    void adjustViewSize();

    Ui::MainWindow *ui;
    QGraphicsScene *scene_;
    QGraphicsView *view_;
    GameStage *stage_;
};

