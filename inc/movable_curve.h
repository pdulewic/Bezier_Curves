#ifndef MOVABLE_CURVE_H
#define MOVABLE_CURVE_H

#include "qcustomplot.h"

// manhattan length from point in pixels, in which a click on a graph is considered a click in a point
const double POINT_SELECTING_RANGE = 7.0;

// the distance by which the movement of the mouse is noticed
const double MOUSE_ACCURACY_TRESHOLD = 2.0;


class MovableCurve: public QCPCurve{
  Q_OBJECT
  bool isPointBeingDragged;
  int indexOfDraggedPoint;
public:
  MovableCurve(QCPAxis *_keyAxis, QCPAxis *_valueAxis);
protected:
  virtual void mousePressEvent(QMouseEvent *event, const QVariant &details);
  virtual void mouseMoveEvent(QMouseEvent *event, const QPointF &startPos);
  virtual void mouseReleaseEvent(QMouseEvent *event, const QPointF &startPos);
signals:
  // emitted when point at given index changes its coordinates to newPoint
  void curveChanged(int index, QPointF newPoint);
};

#endif // MOVABLE_CURVE_H
