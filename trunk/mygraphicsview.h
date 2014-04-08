#ifndef MYGRAPHICSVIEW_H
#define MYGRAPHICSVIEW_H

#include <QGraphicsView>
#include <QMouseEvent>
#include <QtOpenGL/QtOpenGL>

class MyGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit MyGraphicsView(QObject *parent = 0);

signals:
    void clicked();
public slots:
    void mousePressEvent(QMouseEvent * e);
};

#endif // MYGRAPHICSVIEW_H
