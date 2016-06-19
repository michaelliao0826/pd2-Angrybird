#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QList>
#include <QDesktopWidget>
#include <QTimer>
#include <Box2D/Box2D.h>
#include <QMouseEvent>
#include <iostream>
#include <QPushButton>
#include <QLabel>
#include <QMediaPlayer>

#include <gameitem.h>
#include <land.h>
#include <bird.h>
#include <obstacle.h>
#include <bluebird.h>
#include <yellowbird.h>
#include <greenbird.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void showEvent(QShowEvent *);
    bool eventFilter(QObject *,QEvent *event);
    void closeEvent(QCloseEvent *);
    void startGame();
    void restartGame();
    void pigHit();

signals:
    // Signal for closing the game
    void quitGame();

private slots:
    void tick();
    // For debug slot
    void QUITSLOT();
private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    b2World *world;
    QList<GameItem *> itemList;
    QTimer timer;
    QGraphicsPixmapItem *gameBg;
    QGraphicsPixmapItem *gameCatapult;
    QPushButton *restart;
    QPushButton *exit;
    QLabel *scoreBoard;
    QMediaPlayer *bgm;
    QMediaPlayer *wee;

    Bird *bird[4];
    int birdType;
    Bird *pig[2];
    Obstacle *building[6];

    int score = 0, timecount = 0;
    double x, y, pressX, pressY, r;
    float pig1_vx, pig1_vy, pig1_angle, pig2_vx, pig2_vy, pig2_angle;
    bool playGame;
};

#endif // MAINWINDOW_H
