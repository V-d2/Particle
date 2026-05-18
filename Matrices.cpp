#include "Matrices.h"

namespace Matrices {

    ///Construct a matrix of the specified size.
    ///Initialize each element to 0.
    Matrix::Matrix(int _rows, int _cols) : rows(_rows), cols(_cols) {
        a.resize(rows, std::vector<double>(cols, 0.0));
    }

    ///Call the parent constructor to create a 2x2 matrix
    ///Then assign each element as follows:
    /*
    cos(theta)  -sin(theta)
    sin(theta)   cos(theta)
    */
    ///theta represents the angle of rotation in radians, counter-clockwise
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
    Matrix operator+(const Matrix& a, const Matrix& b) {
        Matrix result(a.getRows(), a.getCols());
        for (int i = 0; i < a.getRows(); ++i) {
            for (int j = 0; j < a.getCols(); ++j) {
                result(i, j) = a(i, j) + b(i, j);
            }
        }
        return result;
    }

    // Matrix multiplication
    Matrix operator*(const Matrix& a, const Matrix& b) {   
        if (a.getCols() != b.getRows()) {
            std::cerr << "Matrix dimensions do not match for multiplication!" << std::endl;
            exit(1);
        }
        Matrix result(a.getRows(), b.getCols());
        for (int i = 0; i < a.getRows(); ++i) {
            for (int j = 0; j < b.getCols(); ++j) {
                result(i, j) = 0;
                for (int k = 0; k < a.getCols(); ++k) {
                    result(i, j) += a(i, k) * b(k, j);
                }
            }
        }
        return result;  
    }

    // Matrix comparison (optional for unit test)
    bool operator==(const Matrix& m1, const Matrix& m2) { 
           return false; 
    }
    bool operator!=(const Matrix& m1, const Matrix& m2) { 
        return true; 
    }

}