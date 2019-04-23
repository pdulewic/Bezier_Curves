#include "inc/mainwindow.h"
#include "inc/qcustomplot.h"

#include <QLabel>
#include <QVector>
#include <QPointF>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent){
  QCustomPlot* customPlot = new QCustomPlot;
  cubicBezier = CubicBezier(customPlot, { QPointF(5, 5), QPointF(8, 12), QPointF(1, 10), QPointF(3, 2)});
  cubicBezier2 = CubicBezier(customPlot, { QPointF(15, 6), QPointF(11, 17), QPointF(21, 3), QPointF(17, 12)});

  customPlot->xAxis->setLabel("x");
  customPlot->yAxis->setLabel("y");
  // set axes ranges, so we see all data:
  customPlot->xAxis->setRange(0, 10);
  customPlot->yAxis->setRange(0, 15);
  customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);

  cubicBezier.draw();
  cubicBezier2.draw();
  setCentralWidget(customPlot);
}

//=====================================================================

MainWindow::~MainWindow(){

}
