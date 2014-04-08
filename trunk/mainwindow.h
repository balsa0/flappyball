#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include <QGlobal.h>

#include <QGraphicsScene>
#include <mygraphicsview.h>

#include <QTimer>
#include <QDebug>
#include <QGraphicsEllipseItem>
#include <QVariant>
#include <QVariantAnimation>
#include <QVector>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QGraphicsScene* scene;
    MyGraphicsView*  view;
private:
    //bg
    int bgcount;
    QTimer* bgTimer;
    QPixmap* bg;

    //Ball
    QGraphicsEllipseItem* ball;
    QVariantAnimation* flapAni;
    QTimer* fallTimer;
    int fallCnt;

    //logic
    QTimer* logicTimer;
    QList<QGraphicsItem *> collision;
    bool disableFlap;

    //walls
    QVector<QGraphicsRectItem*> walls;
    QVector<QVariantAnimation*> wallsAni;
    QTimer* wallsTimer;
    QTimer* wallGenTimer;

    //score
    int score;
    QTimer* scoreTimer;
    QGraphicsTextItem* scoreTxt;

    //timers
    int wallgenTime;
    int wallStepTime;
    int logicTime;
    int fallTime;
    int bgTime;

private slots:
    void changeBg();
    void flap();
    void setBallPos(int y);
    void setBallPos(QVariant y);
    void fallBall();
    void restoreFalling();
    void gameLogic();
    void createWall();
    void moveWalls();
    void resetGame();
    void scored();
};

#endif // MAINWINDOW_H
