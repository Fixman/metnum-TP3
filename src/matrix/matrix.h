#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>
#include <string>
#include <vector>

class Matrix;
typedef std::pair<Matrix,Matrix> FactLU;
typedef std::vector <double> Vector;
typedef unsigned int uint;

const double epsilon = 1e-7;

Vector &operator+= (Vector &, const Vector &);
Vector &operator-= (Vector &, const Vector &);
Vector &operator*= (Vector &, double);
Vector &operator/= (Vector &, double);
Vector operator- (Vector, const Vector &);
Vector operator* (Vector, double);

// Maybe this should be operator*
double dot (const Vector &a, const Vector &b);

bool operator== (const Vector &, const Vector &);

class Matrix
{
    std::vector <Vector> values;

    public:
    int N, M;

    static Matrix I(int N);
    static Matrix Z(int N);

    Vector& operator[] (int);
    const Vector& operator[] (int) const;
    bool operator== (const Matrix &) const;

    Vector operator* (const Vector &) const;
    Matrix operator* (const Matrix &) const;

    // Dado un Vector b, calcula x tal que Ax = b
    Vector EG_tradicional(Vector) const;
    Vector EG_optimizada(Vector) const;

    // Devuelve un par <L, R> tal que LR = A
    FactLU LU_tradicional() const;
    FactLU LU_optimizado() const;

    //Para matrices ya trianguladas
    Vector backward_substitution(const Vector&) const;
    Vector forward_substitution(const Vector&) const;

    explicit Matrix (const std::vector <Vector> &);
    Matrix (int, int);
};

std::ostream &operator<< (std::ostream &, const Vector &);
std::ostream &operator<< (std::ostream &, const Matrix &);

#endif
