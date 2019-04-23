#ifndef DISTANCE_CALCULATOR_H
#define DISTANCE_CALCULATOR_H

#include "cubic_bezier.h"

class QCustomPlot;
class QCPItemLine;

/*
 * This class calculates shortest distance between two
 * Bezier curves and draws it on the QCustomPlot.
 */

class DistanceCalculator{
  QCustomPlot* plot;
  QCPItemLine* distanceLine;
public:
  DistanceCalculator(QCustomPlot* _plot=nullptr) {setPlot(_plot);}
  void setPlot(QCustomPlot* _plot);
  float calculate(const CubicBezier& first, const CubicBezier& second);
};

#endif // DISTANCE_CALCULATOR_H
