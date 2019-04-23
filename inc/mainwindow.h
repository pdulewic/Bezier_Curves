#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "cubic_bezier.h"

class MainWindow : public QMainWindow{
    Q_OBJECT
    CubicBezier cubicBezier;
    CubicBezier cubicBezier2;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
protected:
    QSize sizeHint() const {return QSize(800,600); }
};

#endif // MAINWINDOW_H
