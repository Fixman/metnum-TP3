#include <matrix/matrix.h>
#include <cassert>
#include <iomanip>
#include <vector>
#include <tuple>
#include <utility>

Vector &operator+= (Vector &a, const Vector &b)
{
    // Solo se pueden sumar vectores del mismo tamano.
    assert(a.size() == b.size());

    for (uint i = 0; i < a.size(); i++)
        a[i] += b[i];

    return a;
}

Vector &operator-= (Vector &a, const Vector &b)
{
    return a += b * -1;

}

Vector &operator*= (Vector &a, double x)
{
    for (double &i : a)
        i *= x;

    return a;
}

Vector &operator/= (Vector &a, double x)
{
    return a *= 1 / x;
}

Vector operator- (Vector a, const Vector &x)
{
    return a -= x;
}

Vector operator* (Vector a, double x)
{
    return a *= x;
}

double dot(const Vector &a, const Vector &b)
{
    assert(a.size() == b.size());

    double x = 0;
    for (uint i = 0; i < a.size(); i++)
        x += a[i] * b[i];

    return x;
}

bool operator== (const Vector &a, const Vector &b)
{
    if (a.size() != b.size())
        return false;

    for (uint i = 0; i < a.size(); i++)
    {
        if (abs(a[i] - b[i]) > epsilon)
            return false;
    }

    return true;
}

// Construct I.
// I could make I a member and make this function return a const reference,
// but that would be even more Java-like.
Matrix Matrix::I (int N)
{
    Matrix I(N, N);
    for (int i = 0; i < N; i++)
        I[i][i] = 1;

    return I;
}

Matrix Matrix::Z (int N)
{
    return Matrix(N, N);
}

Vector& Matrix::operator[] (int n)
{
    assert(n < N);
    return values[n];
}

const Vector& Matrix::operator[] (int n) const
{
    assert(n < N);
    return values[n];
}

bool Matrix::operator== (const Matrix &m) const
{
    if (N != m.N || M != m.M)
        return false;

    for (int i = 0; i < N; i++)
    {
        if (values[i] != m.values[i])
            return false;
    }

    return true;
}

Vector Matrix::operator* (const Vector &b) const
{
    assert(M == (int)b.size());

    const Matrix &A = *this;
    Vector x(N);
    for (int i = 0; i < N; i++)
        x[i] = dot(A[i], b);

    return x;
}


Matrix Matrix::operator* (const Matrix &B) const
{
    // Dimensions must match for multiplication.
    assert(M == B.N);

    const Matrix &A = *this;
    Matrix R(N, B.M);
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < B.M; j++)
        {
            for (int k = 0; k < M; k++)
                R[i][j] += A[i][k] * B[k][j];
        }
    }

    return R;
}

Vector Matrix::EG_tradicional(Vector b) const
{
    // Solo se puede hacer EG en matrices cuadradas.
    assert(N == M);

    // Solo se puede hacer EG en vectores del mismo alto que mi matriz.
    assert(N == (int)b.size());

    Matrix A = *this;

    for (int i = 0; i < N; i++)
    {
        // m[i][i] != 0 sii esta parte de la descomposicion se puede resolver
        // sin pivoteo.
        assert(A[i][i] != 0);

        for (int j = i+1; j < N; j++)
        {
            double multiplier = A[j][i] / A[i][i];
            A[j][i] = 0; // Evitar errors de presicion

            //A[j] -= A[i] * multiplier;
            for(int k=i+1;k<N;k++)
                A[j][k] -= A[i][k]*multiplier;
            b[j] -= b[i] * multiplier;
        }
    }

    return A.backward_substitution(b);
}

Vector Matrix::EG_optimizada(Vector b) const
{
    // Solo se puede hacer EG en matrices cuadradas.
    assert(N == M);

    // Solo se puede hacer EG en vectores del mismo alto que mi matriz.
    assert(N == (int)b.size());

    Matrix A = *this;

    for (int i = 0; i < N; i++)
    {
        // m[i][i] != 0 sii esta parte de la descomposicion se puede resolver
        // sin pivoteo.
        assert(A[i][i] != 0);

        for (int j = 0; j < N; j++)
        {
            if (i != j)
            {
                double multiplier = A[j][i] / A[i][i];
                A[j] -= A[i] * multiplier;
                b[j] -= b[i] * multiplier;

                A[j][i] = 0; // Evitar errores de precision
            }
        }

        b[i] /= A[i][i];
        A[i] /= A[i][i];
    }

    // Postcondicion: La matriz "final" debe ser I.
    // Esto puede ser dar falsos negativos por temas de presicion. Revisar
    // igualdad de punto flotante con epsilon.

    // std::cerr << A << std::endl;
    // assert(A == I(N));
    return b;
}

FactLU Matrix::LU_tradicional() const
{
    // Solo se puede hacer LU en matrices cuadradas.
    assert(N == M);
    FactLU res(I(N),Matrix(*this));
    Matrix& U = std::get<1>(res);
    Matrix& L = std::get<0>(res);

    for (int i = 0; i < N; i++)
    {
        // m[i][i] != 0 sii esta parte de la descomposicion se puede resolver
        // sin pivoteo.
        assert(U[i][i] != 0);

        for (int j = i+1; j < N; j++)
        {
            L[j][i] = U[j][i] / U[i][i];
            U[j][i] = 0;/*evito errores de precision, pues ya se que debe
                         *ser 0 matematicamente
                         */
            //A[j] -= A[i] * multiplier;
            for(int k=i+1;k<N;k++) U[j][k] -= U[i][k]*L[j][i];
        }
    }
    //assert(L * U == *this);
    return res;
}

// Esto usa un algoritmo penssado a mano. Seguramente deberia haberme copiado
// el algoritmo de Doolittle en Wikipedia, pero aunque sea es O(N^3) que es la
// mejor complejidad para algoritmos razonables.
std::pair <Matrix, Matrix> Matrix::LU_optimizado() const
{
    // Solo se puede hacer LU en matrices cuadradas.
    assert(N == M);

    // Calculations are easier when using A instead of implicit *this.
    const Matrix &A = *this;

    Matrix L = I(N), U = Z(N);
    for (int i = 0; i < N; i++)
    {
        Vector q(N, 0);

        for (int j = 0; j < i; j++)
        {
            double psum = 0;
            for (int k = 0; k < j; k++)
                psum += L[i][k] * U[k][j];

            L[i][j] = (A[i][j] - psum) / U[j][j];

            q += U[j] * L[i][j];
        }

        U[i] = A[i] - q;
    }

    // assert(L * U == A);
    return std::make_pair(L, U);
}

Vector Matrix::backward_substitution(const Vector& b) const
{
    //La matriz debe ser triangular superior
    for(int i=0;i<N;i++)
    {
        for(int j=0;j<i;j++)
        {
            assert(values[i][j] == 0);
        }
    }

    //El vector debe tener la misma altura que la matriz
    assert(N == (int)b.size());

    Vector res(N,0);
    for(int i=N-1;i>=0;i--)
    {
        assert(values[i][i] != 0);
        res[i] = b[i] - dot(Vector(values[i].cbegin()+i+1, values[i].cend()),
                Vector(res.cbegin()+i+1, res.cend()));
        res[i] /= values[i][i];
    }

    return res;
}

Vector Matrix::forward_substitution(const Vector& b) const
{
    //La matriz debe ser triangular inferior
    for(int i=0;i<N;i++)
    {
        for(int j=i+1;j<N;j++)
        {
            assert(values[i][j] == 0);
        }
    }

    //El vector debe tener la misma altura que la matriz
    assert(N == (int)b.size());

    Vector res(N,0);
    for(int i=0;i<N;i++)
    {
        assert(values[i][i] != 0);
        res[i] = b[i] - dot(Vector(values[i].cbegin(), values[i].cbegin()+i),
                Vector(res.cbegin(), res.cbegin()+i));
        res[i] /= values[i][i];
    }

    return res;
}

Matrix::Matrix (const std::vector <Vector> &values) : values(values), N(values.size()), M(values[0].size())
{
}

Matrix::Matrix (int N, int M) : N(N), M(M)
{
    values.resize(N);
    for (Vector &v : values)
        v.resize(M, 0);
}

std::ostream &operator<< (std::ostream &os, const Vector &v)
{
    for (uint i = 0; i < v.size(); i++)
        os << std::setprecision(10) << v[i] << std::endl;

    return os;
}

std::ostream &operator<< (std::ostream &os, const Matrix &m)
{
    for (int i = 0; i < m.N; i++)
        os << m[i] << "\n";

    return os;
}

