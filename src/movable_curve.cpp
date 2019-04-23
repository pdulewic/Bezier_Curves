#include "inc/movable_curve.h"
#include <QDebug>

MovableCurve::MovableCurve(QCPAxis *_keyAxis, QCPAxis *_valueAxis):
  QCPCurve(_keyAxis, _valueAxis),
  isPointBeingDragged(false)
{}

//=====================================================================

void MovableCurve::mousePressEvent(QMouseEvent *event, const QVariant &details)
{
  Q_UNUSED(details)
  QVariant closestPoint;
  // determining closest point to a mouse click
  selectTest(QPointF(event->pos()),false, &closestPoint);
  QCPDataSelection dataSelection = closestPoint.value<QCPDataSelection>();
  if(!dataSelection.isEmpty())
  {
    auto dataRange = dataSelection.dataRange(0);
    int index = dataRange.begin();
    QPointF polygonPoint = dataPixelPosition(index);
    // checking if mouse was pressed close enough to the point
    if((polygonPoint - event->pos()).manhattanLength() < POINT_SELECTING_RANGE)
    {
      // if yes, flag is set and event does not propagate further
      indexOfDraggedPoint = index;
      isPointBeingDragged = true;
      event->accept();
    }
    else
    {
      // if no, nothing happens
      event->ignore();
    }
  }
}

//=====================================================================

void MovableCurve::mouseMoveEvent(QMouseEvent *event, const QPointF &startPos)
{
  Q_UNUSED(startPos);
  // if mouse was not pressed before - nothing happens
  if(!isPointBeingDragged)
  {
    event->ignore();
    return;
  }
  QPointF draggedPoint = dataPixelPosition(indexOfDraggedPoint);
  if((draggedPoint - event->pos()).manhattanLength() > MOUSE_ACCURACY_TRESHOLD)
  {
    // if we dragged selected point by a significant distance, we update its position
    double key, value;
    pixelsToCoords(event->pos(),key,value);
    auto destination = data()->at(indexOfDraggedPoint);
    auto it = data()->begin();
    while(it != destination)
      it++;
    it->value = value;
    it->key = key;
    emit curveChanged(indexOfDraggedPoint,QPointF(key,value));
  }
  event->accept();
}

//=====================================================================

void MovableCurve::mouseReleaseEvent(QMouseEvent *event, const QPointF &startPos)
{
  Q_UNUSED(startPos)
  if(isPointBeingDragged)
  {
    isPointBeingDragged = false;
    event->accept();
  }
  else
    event->ignore();

}
