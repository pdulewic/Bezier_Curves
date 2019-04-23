#include "inc/distance_calculator.h"
#include "inc/qcustomplot.h"

void DistanceCalculator::setPlot(QCustomPlot *_plot)
{
  plot = _plot;
  if(plot != nullptr)
    distanceLine = new QCPItemLine(plot);
}

float DistanceCalculator::calculate(const CubicBezier &first, const CubicBezier &second)
{
  return 1.0;
}
