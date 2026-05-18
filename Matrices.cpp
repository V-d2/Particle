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

    ///Call the parent constructor to create a 2x2 matrix
            ///Then assign each element as follows:
            /*
            scale   0
            0       scale
            */
            ///scale represents the size multiplier
    ScalingMatrix::ScalingMatrix(double scale) : Matrix(2, 2) {
        a[0][0] = scale; a[0][1] = 0;
        a[1][0] = 0;     a[1][1] = scale;
    }

    ///Call the parent constructor to create a 2xn matrix
            ///Then assign each element as follows:
            /*
            xShift  xShift  xShift  ...
            yShift  yShift  yShift  ...
            */
            ///paramaters are xShift, yShift, and nCols
            ///nCols represents the number of columns in the matrix
            ///where each column contains one (x,y) coordinate pair
    TranslationMatrix::TranslationMatrix(double xShift, double yShift, int nCols) : Matrix(2, nCols) {
        for (int i = 0; i < nCols; ++i) {
            a[0][i] = xShift;
            a[1][i] = yShift;
        }
    }

    // Matrix addition
    Matrix operator+(const Matrix& a, const Matrix& b) {

        Matrix temp(a.getRows(), a.getCols()); // temporary Matrix, because parameters are const
        if (a.getRows() != b.getRows() && a.getCols() != b.getCols()) {
            temp = a; // in case matrix difrent, return the same left matrix
        }
        else {
           
            for (int i = 0; i < a.getRows(); ++i) {
                for (int j = 0; j < a.getCols(); ++j) {
                    temp(i, j) = a(i, j) + b(i, j); // here is used const double& operator()(int i, int j) const{ return a.at(i).at(j); }

                }
            }
        }
        return temp;
    }

    // Matrix multiplication
    Matrix operator*(const Matrix& a, const Matrix& b) {   
        if (a.getCols() != b.getRows()) {
            cout << "Matrix dimension dont match" << std::endl;
            return a; // if are difrent, return the same same left matrix
        }
        Matrix result(a.getRows(), b.getCols());
        
        for (int i = 0; i < a.getRows(); i++) {
            for (int j = 0; j < b.getCols(); j++) {
                for (int k = 0; k < a.getCols(); k++){
                    result(i, j) += a(i, k) * b(k, j);
                }
                //result(i, j) = a(i, 0) * b(0, j) + a(i, 1) * b(1, j);
            }
        }


        /*
        for (int i = 0; i < a.getRows(); ++i) {
            for (int j = 0; j < b.getCols(); ++j) {
                result(i, j) = 0;
                for (int k = 0; k < a.getCols(); ++k) {
                    result(i, j) += a(i, k) * b(k, j);
                }
            }
        }
        */
        return result;  
    }

    // Matrix comparison (optional for unit test)
    bool operator==(const Matrix& a, const Matrix& b) { 
        bool ret = false;
        if (a.getRows() == b.getRows() && a.getCols() == b.getCols()) {
            bool ret = true;
        }
        return ret;
    }
 


    bool operator!=(const Matrix& a, const Matrix& b) { 
        bool ret = true;
        if (a.getRows() != b.getRows() && a.getCols() != b.getCols()) {
            bool ret = false;
        }
        return ret;
    }

}