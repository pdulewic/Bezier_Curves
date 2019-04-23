#ifndef CUBIC_BEZIER_H
#define CUBIC_BEZIER_H

#include <QPointF>
#include <QObject>
#include <QVector>
#include <array>

class QCustomPlot;
class QCPCurve;
class MovableCurve;

const int CUBIC_BEZIER_SIZE = 4;


/*
 * This class implements a QCustomPlot plottable representing 3rd order
 * Bezier curve.
 *
 * Class is designed to use with Qt framework, so it uses Qt structures
 * like QPointF. Order of Bezier curve is fixed to 3 for efficiency and
 * simplicity. 3rd order Bezier curves are quite convinient to use, and
 * by resigning from generalization of the class, we avoid using
 * templates.
 */

class CubicBezier: public QObject{
  Q_OBJECT

  std::array<QPointF, CUBIC_BEZIER_SIZE> controlPoints;
  QCPCurve* bezierCurve;
  MovableCurve* bezierPolygon;
  QCustomPlot *plot;

  void swap(CubicBezier& other);
public:
  // standard constructors
  explicit CubicBezier(QObject* parent = nullptr);
  CubicBezier(QCustomPlot *_plot, std::initializer_list<QPointF> l, QObject* parent = nullptr);
  // rule of 5 (in fact 4 - we cover two assignment cases in one constructor)
  CubicBezier(const CubicBezier& cb);
  CubicBezier& operator=(CubicBezier cb);
  CubicBezier(CubicBezier&& tmp);
  ~CubicBezier();

  QPointF valueAt(float t) const;
  QPointF& operator [](int idx){ return controlPoints[idx]; }
  QPointF operator [](int idx) const { return controlPoints[idx]; }
  QCustomPlot* getPlot() const {return plot;}
  QCPCurve* getBezierCurve() const {return bezierCurve;}
  MovableCurve* getBezierPolygon() const {return bezierPolygon;}
  void setPlot(QCustomPlot* _plot){plot = _plot;}
  void setBezierCurve(QCPCurve* _bezierCurve){bezierCurve = _bezierCurve;}
  void setBezierPolygon(MovableCurve* _bezierPolygon){bezierPolygon = _bezierPolygon;}

  // used in move constructor to prepare object for being destroyed
  void clearCurves();

  // translates whole curve by translation vector
  void translate(QPointF translationVector);
  // divides Bezier curve into two curves at point t
  QVector<CubicBezier> subdivide(float t) const;
  // updates curve according to control points and calls QCustomPlot::replot()
  void draw() const;
private slots:
  void updateControlPoints(int index, QPointF newValue);
};

#endif // CUBIC_BEZIER_H
