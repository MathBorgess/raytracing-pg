#include "Vector.hpp"

#ifndef POINTHEADER
#define POINTHEADER
/*
Classe de pontos.

A saber que:
    - x = componente x
    - y = componente y
    - z = componente z

A classe precisa ser instanciada passando as componentes x, y e z
*/

class Point
{

public:
    double x;
    double y;
    double z;
    // Construtores
    Point() {}
    Point(double x, double y, double z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    // Implemente os métodos de pontos aqui

    Vector operator-(Point other)
    {
        return Vector(x - other.x, y - other.y, z - other.z);
    }

    Point operator+(Vector vec)
    {
        return Point(x + vec.getX(), y + vec.getY(), z + vec.getZ());
    }

    Point operator-(Vector vec)
    {
        return Point(x - vec.getX(), y - vec.getY(), z - vec.getZ());
    }

    Point operator+(Point other)
    {
        return Point(x + other.x, y + other.y, z + other.z);
    }

    Point operator/(double scalar)
    {
        return Point(x / scalar, y / scalar, z / scalar);
    }

    // Print do vetor no formato (x, y, z)
    std::string print()
    {
        return "(" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ")";
    }

    double distance(Point other) const
    {
        return sqrt(pow(x - other.x, 2) + pow(y - other.y, 2) + pow(z - other.z, 2));
    }

    // Getters
    double getX() const { return x; }
    double getY() const { return y; }
    double getZ() const { return z; }
};

inline std::istream &operator>>(std::istream &is, Point &t)
{
    is >> t.x >> t.y >> t.z;
    return is;
}

inline std::ostream &operator<<(std::ostream &os, const Point &t)
{
    os << "(" << t.x << "," << t.y << "," << t.z << ")";
    return os;
}

inline Point operator*(Point p, double scalar)
{
    return Point(p.x * scalar, p.y * scalar, p.z * scalar);
}

inline Point operator*(double scalar, Point p)
{
    return Point(p.x * scalar, p.y * scalar, p.z * scalar);
}

inline Point minBound(const Point& a, const Point& b) {
    return Point(
        std::min(a.getX(), b.getX()),
        std::min(a.getY(), b.getY()),
        std::min(a.getZ(), b.getZ())
    );
}

inline Point maxBound(const Point& a, const Point& b) {
    return Point(
        std::max(a.getX(), b.getX()),
        std::max(a.getY(), b.getY()),
        std::max(a.getZ(), b.getZ())
    );
}

inline Point minBound(const std::vector<Point>& points) {
    if (points.empty()) return Point(0, 0, 0); 

    double minX = points[0].getX();
    double minY = points[0].getY();
    double minZ = points[0].getZ();

    for (const Point& p : points) {
        minX = std::min(minX, p.getX());
        minY = std::min(minY, p.getY());
        minZ = std::min(minZ, p.getZ());
    }

    return Point(minX, minY, minZ);
}

inline Point maxBound(const std::vector<Point>& points) {
    if (points.empty()) return Point(0, 0, 0);

    double maxX = points[0].getX();
    double maxY = points[0].getY();
    double maxZ = points[0].getZ();

    for (const Point& p : points) {
        maxX = std::max(maxX, p.getX());
        maxY = std::max(maxY, p.getY());
        maxZ = std::max(maxZ, p.getZ());
    }

    return Point(maxX, maxY, maxZ);
}

#endif