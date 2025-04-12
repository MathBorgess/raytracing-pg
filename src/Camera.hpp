#ifndef CAMERAHEADER
#define CAMERAHEADER

#include "RayTrace.hpp"
#include <fstream>
#include <chrono>

using namespace std::chrono;


class Camera
{
private:
    double pixelH, pixelW;
    Vector unitUp;
    Vector unitLeft;

public:
    Point camPosition;
    Point target;
    Vector Vup, U, V, W;
    Point topLeft, topRight, bottomLeft, bottomRight;
    int vres, hres;
    double f; // distancia do plano de projecao

    Camera(Point CenterCam, Point Aim, Vector Vup, int vres, int hres) : vres(vres), hres(hres), Vup(Vup)
    {
        camPosition = CenterCam;
        target = Aim;
        W = (target - camPosition).normalize();
        U = (Vup.cross(W)).normalize(); // aponta para esquerda
        V = W.cross(U);                 // aponta para cima
        pixelH = 1.0 / vres;
        pixelW = 1.0 / hres;
        unitUp = V * pixelH;
        unitLeft = U * pixelW;
    }
    ~Camera() {};

    void setupProjection(const double& f_) {
        f = f_;
        Point topleftPixel = camPosition + W * (f / 2.0) +
            (V * (vres - 1) * pixelH + U * (hres - 1) * pixelW) / 2.0;

        topLeft = topleftPixel;
        topRight = topleftPixel - unitLeft * (hres - 1);
        bottomLeft = topleftPixel - unitUp * (vres - 1);
        bottomRight = bottomLeft - unitLeft * (hres - 1);
    }
    
    void render(int ttl)
    {
        std::ofstream ppm;
        ppm.open("render.ppm");
        std::cout << "Rendering..." << std::endl;
        auto start = high_resolution_clock::now();

        ppm << "P3" << std::endl;
        ppm << hres << ' ' << vres << std::endl;
        ppm << 255 << std::endl;
        int k = 0;
        for (int i = 0; i < vres; i++)
        {
            for (int j = 0; j < hres; j++)
            {
                Point pixelPosition = topLeft - unitUp * i - unitLeft * j;
                Vector pixelColor = rayTrace(Ray(camPosition, pixelPosition), ttl) * 255.0;
                ppm << pixelColor << std::endl;
            }
        }
        ppm.close();

        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>(stop - start);
        std::cout << "Rendered in: " << duration.count() / 1000.0 << " seconds." << std::endl;
    }

};


inline AABB computePlaneBB(const Camera& cam, Plane& plane) {
    Point p1 = plane.pointIntersect(Ray(cam.camPosition, cam.topLeft));
    Point p2 = plane.pointIntersect(Ray(cam.camPosition, cam.topRight));
    Point p3 = plane.pointIntersect(Ray(cam.camPosition, cam.bottomLeft));
    Point p4 = plane.pointIntersect(Ray(cam.camPosition, cam.bottomRight));

    std::vector<Point> corners = {p1, p2, p3, p4};
    Point small = minBound(corners);
    Point big = maxBound(corners);

    return AABB(small, big);
}

#endif