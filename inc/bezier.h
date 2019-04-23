#ifndef BEZIER_H
#define BEZIER_H

#include <QVector>
#include <QPointF>
#include <array>
#include <cmath>
#include <initializer_list>

class QCustomPlot;

//-------------------------
// auxiliary classes

template<size_t N>
class BinomialCoefficients{
public:
  BinomialCoefficients();
  const size_t operator [](size_t idx) const{return mCoefficients[idx];}
private:
  float mCoefficients[N+1];
};

struct PolynomialPair{
  size_t t = 0;
  size_t one_minus_t = 0;

  double valueAt(float t) const
  {
    return pow(1 - t, one_minus_t) * pow(t, this->t);
  }
};

template<size_t N>
class PolynomialCoefficients{
public:
  PolynomialCoefficients();
  double valueAt(size_t pos, float t) const{return mPolynomialPairs[pos].valueAt(t);}
  const PolynomialPair& operator [](size_t idx) const {return mPolynomialPairs[idx]; }
private:
  PolynomialPair mPolynomialPairs[N+1];
};

//--------------------------------------------
// main class

/*
 * This class represents Nth order Bezier curve. Class is designed
 * to use with Qt framework, so it uses Qt structures like QPointF
 * or QVector. Using QVector also makes it easier to work with 
 * QCustomPlot
 */


template <size_t N>
class Bezier{
  std::array<QPointF, N+1> controlPoints;

public:
  Bezier();
  Bezier(std::initializer_list<QPointF> l);
  float valueAt(float t, int axis);
  QPointF valueAt(float t);
  QPointF& operator [](int idx){ return controlPoints[idx]; }
  QPointF operator [](int idx) const { return controlPoints[idx]; }
  void draw(QCustomPlot *plot);

  static const BinomialCoefficients<N> binomialCoefficients;
  static const PolynomialCoefficients<N> polynomialCoefficients;
};

template<size_t N>
const BinomialCoefficients<N> Bezier<N>::binomialCoefficients = BinomialCoefficients<N>();

template<size_t N>
const PolynomialCoefficients<N> Bezier<N>::polynomialCoefficients = PolynomialCoefficients<N>();




#endif // BEZIER_H
