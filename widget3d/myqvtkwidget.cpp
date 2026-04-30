#include "myqvtkwidget.h"
#include<QDebug>


MyQvtkWidget::MyQvtkWidget(QWidget *parent)
    : QVTKOpenGLWidget(parent)
{
//    auto rw = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
//    this->SetRenderWindow(rw);
}

//void MyQvtkWidget::mouseDoubleClickEvent(QMouseEvent *)
//{
//    emit doubleClicked();
//}

//bool MyQvtkWidget::eventFilter(QObject *object, QEvent *event)
//{
//emit ScaleDown(event);
//    return true;
//}



