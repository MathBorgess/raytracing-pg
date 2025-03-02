#include "Vector.hpp"
#include <vector>
#include <cmath>

#ifndef TRANSFORMHPP
#define TRANSFORMHPP

#define PI 3.141592

//TODO verificar se é preciso garantir que cos e sin sejam maiores que almostZero
// Classe de matrizes com transformacoes afim
class Matrix {
    private:
        int rows;
        int cols;
        std::vector<std::vector<double>> data;
    
    public:
    
        // valores das células da matriz inicia como zero por padrão
        Matrix(int rows, int cols, double initVal = 0.0) : rows(rows), cols(cols) {
            data.resize(rows, std::vector<double>(cols, initVal));
        }
    
        double& operator()(int row, int col) {
            return data[row][col];
        }
    
        // aplica matriz no vetor
        Vector operator*(const Vector& v) const {
            double x = data[0][0] * v.x + data[0][1] * v.y + data[0][2] * v.z + data[0][3];
            double y = data[1][0] * v.x + data[1][1] * v.y + data[1][2] * v.z + data[1][3];
            double z = data[2][0] * v.x + data[2][1] * v.y + data[2][2] * v.z + data[2][3];
            return Vector(x, y, z);
        }
    
        // retorna um operador de translacao
        static Matrix translate(double tx, double ty, double tz) {
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
    
        // retorna um operador de rotação em torno do eixo x. Recebe angulo em graus
        static Matrix rotateX(double angle, bool clockwise = false) {
            Matrix M(4, 4);
            angle = angle * PI / 180; 
            M(0, 0) = 1;
            M(1, 1) = cos(angle);
            M(1, 2) = clockwise ? sin(angle) : -sin(angle);
            M(2, 1) = clockwise ? -sin(angle) : sin(angle);
            M(2, 2) = cos(angle);
            M(3, 3) = 1;
            return M;
        }
    
        // retorna um operador de rotação em torno do eixo y. Recebe angulo em graus
        static Matrix rotateY(double angle, bool clockwise = false) {
            Matrix M(4, 4);
            angle = angle * PI / 180; 
            M(0, 0) = cos(angle);
            M(0, 2) = clockwise ? -sin(angle) : sin(angle);
            M(1, 1) = 1;
            M(2, 0) = clockwise ? sin(angle) : -sin(angle);
            M(2, 2) = cos(angle);
            M(3, 3) = 1;
            return M;
        }
    
        // retorna um operador de rotação em torno do eixo z. Recebe angulo em graus
        static Matrix rotateZ(double angle, bool clockwise = false) {
            Matrix M(4, 4);
            angle = angle * PI / 180;
            M(0, 0) = cos(angle);
            M(0, 1) = clockwise ? sin(angle) : -sin(angle);
            M(1, 0) = clockwise ? -sin(angle) : sin(angle);
            M(1, 1) = cos(angle);
            M(2, 2) = 1;
            M(3, 3) = 1;
            return M;
        }

        // retorna um operador de escala
        static Matrix scale(double sx, double sy, double sz) {
            Matrix M(4,4);
            M(0,0) = sx; 
            M(1,1) = sy;
            M(2,2) = sz;
            M(3,3) = 1;
            return M;
        }

        // retorna um operador de cisalhamento
        static Matrix shear(double xy, double xz, double yx, double yz, double zx, double zy){
            Matrix M(4,4);
            M(0,0) = 1, M(0,1) = xy, M(0,2) = xz;
            M(1,0) = yx, M(1,1) = 1, M(1,2) = yz;
            M(2,0) = zx, M(2,1) = zy, M(2,2) = 1;
            M(3,3) = 1;
            return M;
        }

};

    
#endif