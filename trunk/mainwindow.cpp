#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{

    QTime time = QTime::currentTime();
    qsrand((uint)time.msec());

    view = new MyGraphicsView(this);
    this->setCentralWidget(view);
    this->setFixedSize(480,800);
    view->setFixedSize(480,800);

    scene = new QGraphicsScene(this);
    scene->setBackgroundBrush(QColor(0,0,139));
    scene->setSceneRect(0,0,480,800);
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);

    view->setScene(scene);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setRenderHints(QPainter::Antialiasing);
    view->setSceneRect(0,0,480,800);
    view->setDragMode(QGraphicsView::NoDrag);
    view->setViewport(new QGLWidget(QGLFormat(QGL::SampleBuffers)));
    view->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);

    //timers
    wallgenTime = 3000;
    wallStepTime = 10;
    bgTime = 70;
    logicTime = 10;
    fallTime = 5;

    //background
    bg = new QPixmap(":/gfx/bg/o_68bfe7468307d7de-0");
    scene->setBackgroundBrush(*bg);
    bgcount = 0;
    bgTimer = new QTimer(this);
    connect(bgTimer,SIGNAL(timeout()),this,SLOT(changeBg()));
    bgTimer->start(bgTime);

    //Ball
    ball = new QGraphicsEllipseItem(100,view->height()/2-30,60,60);
    ball->setBrush(QBrush(QPixmap(":/gfx/ball")));
    scene->addItem(ball);

        //flap
    disableFlap = false;
    flapAni = new QVariantAnimation(this);
    flapAni->setEasingCurve(QEasingCurve::OutCubic);
    flapAni->setDuration(400);
    connect(flapAni,SIGNAL(valueChanged(QVariant)),this,SLOT(setBallPos(QVariant)));
    connect(flapAni,SIGNAL(finished()),this,SLOT(restoreFalling()));

        //fall
    fallCnt = 0;
    fallTimer = new QTimer(this);
    connect(fallTimer,SIGNAL(timeout()),this,SLOT(fallBall()));

        //click
    connect(view,SIGNAL(clicked()),this,SLOT(flap()));

    //logic
    logicTimer = new QTimer(this);
    connect(logicTimer,SIGNAL(timeout()),this,SLOT(gameLogic()));
    logicTimer->start(logicTime);

    //wall generator
    wallGenTimer = new QTimer(this);
    connect(wallGenTimer,SIGNAL(timeout()),this,SLOT(createWall()));
    wallGenTimer->start(wallgenTime);

    //wall mover
    wallsTimer = new QTimer(this);
    connect(wallsTimer,SIGNAL(timeout()),this,SLOT(moveWalls()));
    wallsTimer->start(wallStepTime);

    //score
    score = 0;
    scoreTimer = new QTimer(this);
    connect(scoreTimer,SIGNAL(timeout()),this,SLOT(scored()));
    scoreTimer->start(wallgenTime*3 + 100);

    QFont f1;
    f1.setFamily("Terminal");
    f1.setPixelSize(70);

    scoreTxt = new QGraphicsTextItem();
    scoreTxt->setPlainText("0");
    scoreTxt->setFont(f1);
    scoreTxt->setDefaultTextColor(Qt::yellow);
    scoreTxt->setZValue(1000);  //always visible
    scene->addItem(scoreTxt);

}

MainWindow::~MainWindow()
{

}

void MainWindow::changeBg(){
    bgcount++;
    if(bgcount > 11)
        bgcount = 0;
    bg->load(":/gfx/bg/o_68bfe7468307d7de-"+QString::number(bgcount));
    scene->setBackgroundBrush(*bg);
    bgTimer->start(bgTime);
}

void MainWindow::flap(){
    //qWarning() << "flap";
    if(disableFlap){
        if(flapAni->state() != QAbstractAnimation::Running)
            resetGame();
        return;
    }

    //stop falling
    fallTimer->stop();

    //stop active flapping ani
    if(flapAni->state() == QAbstractAnimation::Running)
        flapAni->stop();

    //start flapping
    flapAni->setStartValue(ball->y());
    flapAni->setEndValue(ball->y()-180);
    flapAni->start();
}

void MainWindow::setBallPos(int y){
    ball->setPos(ball->x(),y);
}

void MainWindow::setBallPos(QVariant y){
    setBallPos(y.toInt());
}

void MainWindow::fallBall(){
    ball->setPos(ball->x(),ball->y()+(fallCnt/5));
    fallTimer->start(fallTime);

    fallCnt++;
}

void MainWindow::restoreFalling(){
    fallTimer->start(fallTime);
    fallCnt = 0;
}

void MainWindow::gameLogic(){

    //collision detection
    collision = scene->collidingItems(ball,Qt::IntersectsItemShape);
    if(collision.size() != 0){
        logicTimer->stop();
        wallsTimer->stop();
        wallGenTimer->stop();
        bgTimer->stop();
        disableFlap = true;
        scoreTimer->stop();
        scoreTxt->setPlainText(QString::number(score)+" GAME OVER!");
        qWarning() << "GAME OVER";
        return;
    }

    logicTimer->start(logicTime);
}


void MainWindow::createWall(){

    int tt2 = this->height() /6;    //1/6 of screen
    int tt = 2 * this->height() / 3 -(2.5*tt2); //hole interval on screen

    //random hole position
    int hole = qrand() % tt;

    //top of hole
    int top = tt2 + hole;
    //bottom of hole
    int bottom = top + tt2*2.5;

    QGraphicsRectItem* w1 = scene->addRect(this->width()+150,0,120,top);
    QGraphicsRectItem* w2 = scene->addRect(this->width()+150,bottom,120,this->height()-bottom);

    w1->setBrush(QBrush(QPixmap(":/gfx/wall")));
    w2->setBrush(QBrush(QPixmap(":/gfx/wall")));

    walls.append(w1);
    walls.append(w2);

    wallGenTimer->start(wallgenTime);

    //scene->addRect(this->width()-100,2*part + part*hole,30,(4-hole)*part);
}

void MainWindow::moveWalls(){
    for(int i = 0; i < walls.size();i++){

        //actual wall
        QGraphicsRectItem* w = walls.at(i);

        int x = w->x();
        int y = w->y();

        //remove walls out of screen
        if(x < - 1500){
            delete walls[i];
            walls.removeAt(i);
        }

        w->setPos(x-1,y);

    }
    wallsTimer->start(wallStepTime);
}

void MainWindow::resetGame(){
    wallsTimer->stop();
    logicTimer->stop();
    fallTimer->stop();
    bgTimer->stop();
    scoreTimer->stop();

    flapAni->stop();

    //remove walls
    foreach (QGraphicsRectItem* w, walls) {
        delete w;
    }
    walls.clear();

    //reset Ball pos
    ball->setPos(100,view->height()/2-30);

    //reset score
    score = 0;

    //reset scene
    view->setSceneRect(0,0,480,800);
    scene->setSceneRect(0,0,480,800);

    //restart timers
    bgTimer->start(bgTime);
    logicTimer->start(logicTime);
    wallsTimer->start(wallStepTime);
    wallGenTimer->start(wallgenTime);

    //reset score
    scoreTimer->start(wallgenTime*3-this->width()*2);
    scoreTxt->setPlainText("0");

    //enable flapping
    disableFlap = false;

    ball->setPos(100,view->height()/2-30);
}

void MainWindow::scored(){
    score++;
    scoreTxt->setPlainText(QString::number(score));
    scoreTimer->start(wallgenTime);
    qWarning() << score;
}
