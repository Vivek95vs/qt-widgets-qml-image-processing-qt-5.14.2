#ifndef MYQVTKWIDGET_H
#define MYQVTKWIDGET_H

#include <QObject>
#include <QVTKOpenGLWidget.h>
#include<QWidget>
#include<QMouseEvent>


class MyQvtkWidget : public QVTKOpenGLWidget
{
    Q_OBJECT
public:
    explicit MyQvtkWidget(QWidget *parent = nullptr);
protected:
//    void mouseDoubleClickEvent(QMouseEvent *) VTK_OVERRIDE;

//    bool eventFilter(QObject *object, QEvent *event);



signals:
//    void doubleClicked();

//    void ScaleDown(QEvent *event);
private:

};

#endif // MYQVTKWIDGET_H
