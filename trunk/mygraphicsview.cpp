#include "mygraphicsview.h"
#include <QDebug>

MyGraphicsView::MyGraphicsView(QObject *parent) :
    QGraphicsView()
{
    //connect(this)
}

void MyGraphicsView::mousePressEvent(QMouseEvent *e){
    emit clicked();
}
