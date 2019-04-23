#include "inc/cubic_bezier.h"
#include "inc/movable_curve.h"
#include <QDebug>

//=====================================================================

CubicBezier::CubicBezier(QObject *parent):
  QObject(parent),
  bezierCurve(nullptr),
  bezierPolygon(nullptr),
  plot(nullptr)
{}

//=====================================================================

void CubicBezier::swap(CubicBezier &other)
{
  QCustomPlot* tmp1 = other.getPlot();
  other.setPlot(getPlot());
  setPlot(tmp1);

  QCPCurve* tmp2 = other.getBezierCurve();
  other.setBezierCurve(getBezierCurve());
  setBezierCurve(tmp2);

  MovableCurve* tmp3 = other.getBezierPolygon();
  if(tmp3 != nullptr)
  {
    disconnect(tmp3,SIGNAL(curveChanged(int,QPointF)),&other,SLOT(updateControlPoints(int,QPointF)));
    connect(tmp3,SIGNAL(curveChanged(int,QPointF)),this,SLOT(updateControlPoints(int,QPointF)));
  }
  if(bezierPolygon != nullptr)
  {
    disconnect(bezierPolygon,SIGNAL(curveChanged(int,QPointF)),this,SLOT(updateControlPoints(int,QPointF)));
    connect(bezierPolygon,SIGNAL(curveChanged(int,QPointF)),&other,SLOT(updateControlPoints(int,QPointF)));
  }
  other.setBezierPolygon(getBezierPolygon());
  setBezierPolygon(tmp3);


  for(int i=0; i<CUBIC_BEZIER_SIZE; ++i)
  {
    auto tmp = other[i];
    other[i] = controlPoints[i];
    controlPoints[i] = tmp;
  }
}

//=====================================================================

CubicBezier::CubicBezier(QCustomPlot *_plot, std::initializer_list<QPointF> l, QObject *parent):
  QObject(parent),
  plot(_plot)
{
  int i=0;
  for(auto x : l)
    controlPoints[i++] = x;
  bezierCurve = new QCPCurve(plot->xAxis, plot->yAxis);
  bezierPolygon = new MovableCurve(plot->xAxis, plot->yAxis);
  connect(bezierPolygon,SIGNAL(curveChanged(int,QPointF)),this,SLOT(updateControlPoints(int,QPointF)));
}

//=====================================================================

CubicBezier::CubicBezier(const CubicBezier &cb): QObject(nullptr)
{
  for(int i=0; i<CUBIC_BEZIER_SIZE; ++i)
    controlPoints[i] = cb[i];
  plot = cb.getPlot();
  bezierCurve = new QCPCurve(plot->xAxis, plot->yAxis);
  bezierPolygon = new MovableCurve(plot->xAxis, plot->yAxis);
  connect(bezierPolygon,SIGNAL(curveChanged(int,QPointF)),this,SLOT(updateControlPoints(int,QPointF)));
}

//=====================================================================

CubicBezier &CubicBezier::operator=(CubicBezier cb)
{
  // copy (or move) constructor was already used for creating cb, so we only have to swap member variables
  swap(cb);
  return *this;
}

//=====================================================================

CubicBezier::CubicBezier(CubicBezier &&tmp)
{
  for(int i=0; i<CUBIC_BEZIER_SIZE; ++i)
    controlPoints[i] = tmp[i];
  plot = tmp.getPlot();
  bezierCurve = tmp.getBezierCurve();
  bezierPolygon = tmp.getBezierPolygon();
  connect(bezierPolygon,SIGNAL(curveChanged(int,QPointF)),this,SLOT(updateControlPoints(int,QPointF)));
  tmp.clearCurves();
}

//=====================================================================

CubicBezier::~CubicBezier()
{
  if(bezierCurve != nullptr)
    plot->removePlottable(bezierCurve);
  if(bezierPolygon != nullptr)
    plot->removePlottable(bezierPolygon);
  // memory is realesed in removePlottable() function
}

//=====================================================================

QPointF CubicBezier::valueAt(float t) const
{
  float u = 1.0 - t;
  float t2 = t * t;
  float u2 = u * u;
  float u3 = u2 * u;
  float t3 = t2 * t;

  return u3 * controlPoints[0] + (3.0 * u2 * t) * controlPoints[1] +
      (3.0 * u * t2) * controlPoints[2] + t3 * controlPoints[3];
}

//=====================================================================

void CubicBezier::clearCurves()
{
  bezierCurve = nullptr;
  bezierPolygon = nullptr;
}

//=====================================================================

void CubicBezier::translate(QPointF translationVector)
{
  for(int i=0; i<CUBIC_BEZIER_SIZE; ++i)
    controlPoints[i] += translationVector;
}

//=====================================================================

QVector<CubicBezier> CubicBezier::subdivide(float t) const
{
  // rigid implementation of DeCasteljau algotithm
  QPointF p21 = controlPoints[0] + (controlPoints[1] - controlPoints[0]) * t;
  QPointF p22 = controlPoints[1] + (controlPoints[2] - controlPoints[1]) * t;
  QPointF p23 = controlPoints[2] + (controlPoints[3] - controlPoints[2]) * t;

  QPointF p31 = p21 + (p22 - p21) * t;
  QPointF p32 = p22 + (p23 - p22) * t;

  QPointF p4 = p31 + (p32 - p31) * t;

  QVector<CubicBezier> result(2);
  result[0] = CubicBezier(plot, {controlPoints[0], p21, p31, p4});
  result[1] = CubicBezier(plot, {p4, p32, p23, controlPoints[3]});

  return result;
}

//=====================================================================

void CubicBezier::draw() const
{
  QVector<double> x(101), y(101), t(101);
  for (int i=0; i<101; ++i)
  {
    float time = i/static_cast<float>(100.0);
    t[i] = time;
    auto point = valueAt(time);
    x[i] = point.x();
    y[i] = point.y();
  }

  bezierCurve->setData(t,x,y,true);

  QVector<double> x2(CUBIC_BEZIER_SIZE), y2(CUBIC_BEZIER_SIZE), t2(CUBIC_BEZIER_SIZE);
  for(int i=0; i<CUBIC_BEZIER_SIZE; ++i)
  {
    t2[i] = i;
    x2[i] = controlPoints[i].x();
    y2[i] = controlPoints[i].y();
  }

  bezierPolygon->setData(t2,x2,y2,true);
  bezierPolygon->setPen(QPen(Qt::red));
  bezierPolygon->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 5));

  plot->replot();
}

//=====================================================================

void CubicBezier::updateControlPoints(int index, QPointF newValue)
{
  controlPoints[index] = newValue;
  draw();
}
