#include "Matrices.h"

namespace Matrices {

    // Matrix constructor
    Matrix::Matrix(int _rows, int _cols) : rows(_rows), cols(_cols) {
        a.resize(rows, std::vector<double>(cols, 0.0));
    }

    // RotationMatrix constructor
    RotationMatrix::RotationMatrix(double theta) : Matrix(2, 2) {
        a[0][0] = cos(theta);
        a[0][1] = -sin(theta);
        a[1][0] = sin(theta);
        a[1][1] = cos(theta);
    }

    // ScalingMatrix constructor
    ScalingMatrix::ScalingMatrix(double scale) : Matrix(2, 2) {
        a[0][0] = scale; a[0][1] = 0;
        a[1][0] = 0;     a[1][1] = scale;
    }

    // TranslationMatrix constructor
    TranslationMatrix::TranslationMatrix(double xShift, double yShift, int nCols) : Matrix(2, nCols) {
        for (int i = 0; i < nCols; ++i) {
            a[0][i] = xShift;
            a[1][i] = yShift;
        }
    }

    // Matrix addition
    Matrix operator+(const Matrix& m1, const Matrix& m2) {
        Matrix result(m1.getRows(), m1.getCols());
        for (int i = 0; i < m1.getRows(); ++i)
            for (int j = 0; j < m1.getCols(); ++j)
                result(i, j) = m1(i, j) + m2(i, j);
        return result;
    }

    // Matrix multiplication
    Matrix operator*(const Matrix& m1, const Matrix& m2) {
        Matrix result(m1.getRows(), m2.getCols());
        for (int i = 0; i < m1.getRows(); ++i)
            for (int j = 0; j < m2.getCols(); ++j)
                for (int k = 0; k < m1.getCols(); ++k)
                    result(i, j) += m1(i, k) * m2(k, j);
        return result;
    }

    // Matrix comparison (optional for unit test)
    bool operator==(const Matrix& m1, const Matrix& m2) { return false; }
    bool operator!=(const Matrix& m1, const Matrix& m2) { return true; }

}