#include "Vector.hpp"
#include "Point.hpp"
#include <vector>
#include <cmath>

#ifndef TRANSFORMHPP
#define TRANSFORMHPP

const double PI = std::acos(-1);
constexpr double zeroTrig = 1e-6;

inline bool isAlmostZero(double value) {
    return std::abs(value) <= zeroTrig;
}

// Classe de matrizes 4x4 com transformacoes afim
class Matrix {
    private:
    std::vector<std::vector<double>> data;

    public:
        int rows;
        int cols;

        // inicia como uma matriz identidate 4x4
        Matrix() : rows(4), cols(4) {
            data.resize(4, std::vector<double>(4, 0));
            for (int i = 0; i<4; i++){
                data[i][i] = 1;
            }
        }

        // inicia como uma matriz nula rowsxcols
        Matrix(int rows, int cols, double initVal = 0.0) : rows(rows), cols(cols) {
            data.resize(rows, std::vector<double>(cols, initVal));
        }

        // incia matriz a partir de um vetor de vetores
        Matrix(const std::vector<std::vector<double>> &data) : data(data){};
    
        double& operator()(int row, int col) {
            return data[row][col];
        }
        
        double operator()(int row, int col) const {
            return data[row][col];
        }
        
        // aplica matriz no vetor
        Vector operator*(const Vector& v) const {
            double x = data[0][0] * v.x + data[0][1] * v.y + data[0][2] * v.z + data[0][3];
            double y = data[1][0] * v.x + data[1][1] * v.y + data[1][2] * v.z + data[1][3];
            double z = data[2][0] * v.x + data[2][1] * v.y + data[2][2] * v.z + data[2][3];
            return Vector(x, y, z);
        }

        // aplica matriz no ponto, garantindo coordenadas homogeneas
        Point operator*(const Point& p) const {
            double x = data[0][0] * p.x + data[0][1] * p.y + data[0][2] * p.z + data[0][3];
            double y = data[1][0] * p.x + data[1][1] * p.y + data[1][2] * p.z + data[1][3];
            double z = data[2][0] * p.x + data[2][1] * p.y + data[2][2] * p.z + data[2][3];
            double w = data[3][0] * p.x + data[3][1] * p.y + data[3][2] * p.z + data[3][3];
            if (w != 0 && w != 1) { x /= w; y /= w; z /= w; }
            return Point(x, y, z);
        }

        // multiplica duas matrizes 
        Matrix operator*(const Matrix& M) const {
            Matrix result = Matrix(rows, M.cols);
            for (int i = 0; i < rows; i++){
                for (int k = 0; k < cols; k++){
                    for (int j = 0; j < M.cols; j++){
                        result(i, j)+=data[i][k]*M(k, j);
                    }
                }
            }  
            return result;
        }

        // multiplica duas matrizes 
        Matrix operator*(Matrix& M) const {
            Matrix result = Matrix(rows, M.cols);
            for (int i = 0; i < rows; i++){
                for (int k = 0; k < cols; k++){
                    for (int j = 0; j < M.cols; j++){
                        result(i, j)+=data[i][k]*M(k, j);
                    }
                }
            }  
            return result;
        }

        // soma de matrizes
        Matrix operator+(const Matrix& M) const {
            Matrix result = Matrix(rows, cols);
            for (int i = 0; i < rows; i++) {
                for (int j = 0; j < cols; j++) {
                    result(i, j) = data[i][j] + M.data[i][j];
                }
            }
            return result;
        }

        // retorna um operador de translacao
        static Matrix translation(double tx, double ty, double tz) {
            Matrix M(4, 4);
            M(0, 0) = 1; 
            M(1, 1) = 1; 
            M(2, 2) = 1;
            M(3, 3) = 1;
            M(0, 3) = tx;
            M(1, 3) = ty;
            M(2, 3) = tz;
            return M;
        }

        static Matrix translation(Point p) {
            Matrix M(4, 4);
            M(0, 0) = 1; 
            M(1, 1) = 1; 
            M(2, 2) = 1;
            M(3, 3) = 1;
            M(0, 3) = p.x;
            M(1, 3) = p.y;
            M(2, 3) = p.z;
            return M;
        }
        
        // retorna operador de reflexao em torno do plano yz
        static Matrix reflectionYZ(){
            Matrix M(4, 4);
            M(0,0) = -1;
            M(1,1) = 1;
            M(2,2) = 1;
            M(3,3) = 1;
            return M;
        }
        
        // retorna operador de reflexao em torno do plano xy
        static Matrix reflectionXY(){
            Matrix M(4, 4);
            M(0,0) = 1;
            M(1,1) = -1;
            M(2,2) = 1;
            M(3,3) = 1;
            return M;
        }
        
        // retorna operador de reflexao em torno do plano xz
        static Matrix reflectionXZ(){
            Matrix M(4, 4);
            M(0,0) = 1;
            M(1,1) = 1;
            M(2,2) = -1;
            M(3,3) = 1;
            return M;
        }

       // retorna um operador de rotação em torno do eixo x. Recebe angulo em graus
       static Matrix rotationX(double angle, bool clockwise = false) {
        Matrix M(4, 4);
        angle = angle * PI / 180; 
        double c = cos(angle);
        double s = sin(angle);

        c = isAlmostZero(c) ? 0.0 : (isAlmostZero(c - 1) ? 1.0 : (isAlmostZero(c + 1) ? -1.0 : c));
        s = isAlmostZero(s) ? 0.0 : (isAlmostZero(s - 1) ? 1.0 : (isAlmostZero(s + 1) ? -1.0 : s));
        
        M(0, 0) = 1;
        M(1, 1) = c;
        M(1, 2) = clockwise ? s : -s;
        M(2, 1) = clockwise ? -s : s;
        M(2, 2) = c;
        M(3, 3) = 1;
        return M;
    }

    // retorna um operador de rotação em torno do eixo y. Recebe angulo em graus
    static Matrix rotationY(double angle, bool clockwise = false) {
        Matrix M(4, 4);
        angle = angle * PI / 180; 
        double c = cos(angle);
        double s = sin(angle);

        c = isAlmostZero(c) ? 0.0 : (isAlmostZero(c - 1) ? 1.0 : (isAlmostZero(c + 1) ? -1.0 : c));
        s = isAlmostZero(s) ? 0.0 : (isAlmostZero(s - 1) ? 1.0 : (isAlmostZero(s + 1) ? -1.0 : s));
        
        M(0, 0) = c;
        M(0, 2) = clockwise ? -s : s;
        M(1, 1) = 1;
        M(2, 0) = clockwise ? s : -s;
        M(2, 2) = c;
        M(3, 3) = 1;
        return M;
    }

    // retorna um operador de rotação em torno do eixo z. Recebe angulo em graus
    static Matrix rotationZ(double angle, bool clockwise = false) {
        Matrix M(4, 4);
        angle = angle * PI / 180;
        double c = cos(angle);
        double s = sin(angle);

        c = isAlmostZero(c) ? 0.0 : (isAlmostZero(c - 1) ? 1.0 : (isAlmostZero(c + 1) ? -1.0 : c));
        s = isAlmostZero(s) ? 0.0 : (isAlmostZero(s - 1) ? 1.0 : (isAlmostZero(s + 1) ? -1.0 : s));
        
        M(0, 0) = c;
        M(0, 1) = clockwise ? s : -s;
        M(1, 0) = clockwise ? -s : s;
        M(1, 1) = c;
        M(2, 2) = 1;
        M(3, 3) = 1;
        return M;
    }

        // retorna um operador de escala
        static Matrix scaleOp(double sx, double sy, double sz) {
            Matrix M(4,4);
            M(0,0) = sx; 
            M(1,1) = sy;
            M(2,2) = sz;
            M(3,3) = 1;
            return M;
        }

        // retorna um operador de cisalhamento
        static Matrix shearOp(double xy, double xz, double yx, double yz, double zx, double zy){
            Matrix M(4,4);
            M(0,0) = 1, M(0,1) = xy, M(0,2) = xz;
            M(1,0) = yx, M(1,1) = 1, M(1,2) = yz;
            M(2,0) = zx, M(2,1) = zy, M(2,2) = 1;
            M(3,3) = 1;
            return M;
        }

        Matrix translate(double tx, double ty, double tz) {
            return translation(tx, ty, tz) * (*this);
        }

        Matrix translate(Point p){
            return translation(p) * (*this);
        }

        Matrix reflectYZ() {
            return reflectionYZ() * (*this);
        }

        Matrix reflectXY() {
            return reflectionXY() * (*this);
        }

        Matrix reflectXZ() {
            return reflectionXZ() * (*this);
        }

        Matrix rotateX(double angle, bool clockwise = false) {
            return rotationX(angle, clockwise) * (*this);
        }

        Matrix rotateY(double angle, bool clockwise = false) {
            return rotationY(angle, clockwise) * (*this);
        }

        Matrix rotateZ(double angle, bool clockwise = false) {
            return rotationZ(angle, clockwise) * (*this);
        }

        Matrix scale(double sx, double sy, double sz) {
            return scaleOp(sx, sy, sz) * (*this);
        }

        Matrix shear(double xy, double xz, double yx, double yz, double zx, double zy) {
            return shearOp(xy, xz, yx, yz, zx, zy) * (*this);
        }

};

// multiplicar matriz por escalar 
inline Matrix operator*(const double& scalar, const Matrix& M) {
    Matrix result = M;
    for (int i = 0; i<M.rows; i++){
        for(int j = 0; j<M.cols; j++) {
            result(i,j) *= scalar;
        }
    }
    return result;
}

inline Matrix operator*(const Matrix& M, const double& scalar) {
    Matrix result = M;
    for (int i = 0; i<M.rows; i++){
        for(int j = 0; j<M.cols; j++) {
            result(i,j) *= scalar;
        }
    }
    return result;
}

inline std::ostream& operator<<(std::ostream& os, Matrix& M){
    for (int i = 0; i<M.rows; i++){
        for(int j = 0; j<M.cols; j++) {
            os << M(i,j) <<" ";
        }
        os << std::endl;
    }
    return os;
}

#endif