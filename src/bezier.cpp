#include "inc/bezier.h"
#include "inc/qcustomplot.h"

template<size_t N>
Bezier<N>::Bezier(){
  for(int i=0; i<N+1; ++i)
    controlPoints[i] = QPointF(0.0, 0.0);
}

template<size_t N>
Bezier<N>::Bezier(std::initializer_list<QPointF> l){
  int i=0;
  for(auto x : l)
    controlPoints[i++] = x;
}

template<size_t N>
float Bezier<N>::valueAt(float t, int axis){
  float sum = 0;
  switch (axis) {
    case 0:  // x axis
      for(int i=0; i<N+1; ++i)
        sum += binomialCoefficients[i] * polynomialCoefficients[i].valueAt(t) + controlPoints[i].x();
      break;
    case 1:  // y axis
      for(int i=0; i<N+1; ++i)
        sum += binomialCoefficients[i] * polynomialCoefficients[i].valueAt(t) + controlPoints[i].y();
      break;
  }
  return sum;
}

template<size_t N>
QPointF Bezier<N>::valueAt(float t)
{
  QPointF p;
  p.setX(valueAt(t,0));
  p.setY(valueAt(t,1));
  return p;
}

template<size_t N>
void Bezier<N>::draw(QCustomPlot *plot)
{
  QVector<double> x(101), y(101), t(101); // initialize with entries 0..100
  for (int i=0; i<101; ++i){
    t[i] = i;
    x[i] = valueAt(i/static_cast<float>(100.0), 0);
    y[i] = valueAt(i/static_cast<float>(100.0), 1);
  }
  QCPCurve* bezierCurve = new QCPCurve(plot->xAxis, plot->yAxis);
  bezierCurve->setData(t,x,y,true);

  QVector<double> x2(N+1), y2(N+1), t2(N+1);
  for(int i=0; i<N+1; ++i){
    t2[i] = i;
    x2[i] = controlPoints[i].x();
    y2[i] = controlPoints[i].y();
  }

  QCPCurve* bezierPolygon = new QCPCurve(plot->xAxis, plot->yAxis);
  bezierPolygon->setData(t2,x2,y2,true);
  bezierPolygon->setPen(QPen(Qt::red));
  bezierPolygon->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 5));

  plot->replot();
}

//----------------------------------
// auxiliary

double faculty(size_t n)
{
  double d = 1.0;
  if (n == 0 || n == 1)
    return d;
  for (size_t i = 2; i <= n; i++)
    d *= (double) i;
  return d;
}

// Note: Using faculty function seems to be way faster than the recursive one
// given at https://en.wikipedia.org/wiki/Binomial_coefficient
double binomial(size_t n, size_t k)
{
  return faculty(n) / (faculty(k) * faculty(n - k));
}


template<size_t N>
BinomialCoefficients<N>::BinomialCoefficients()
{
  size_t center = N / 2;
  size_t k = 0;

  while (k <= center)  {
    mCoefficients[k] = binomial(N, k);
    k++;
  }

  // Utilize the symmetrical nature of the binomial coefficients.
  while (k <= N)  {
    mCoefficients[k] = mCoefficients[N - k];
    k++;
  }
}

template<size_t N>
PolynomialCoefficients<N>::PolynomialCoefficients()
{
  for (size_t i = 0; i <= N; i++)
  {
    mPolynomialPairs[i].t = i;
    mPolynomialPairs[i].one_minus_t = N - i;
  }
}
