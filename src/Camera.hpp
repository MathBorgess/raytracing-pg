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

    int vres, hres;

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

    void render(double f, int ttl)
    {
        std::ofstream ppm;
        ppm.open("render.ppm");
        std::cout << "Rendering..." << std::endl;
        auto start = high_resolution_clock::now();

        Point topleftPixel = camPosition + W * (f / 2) + (V * (vres - 1) * pixelH + U * (hres - 1) * pixelW) / 2.0;
        ppm << "P3" << std::endl;
        ppm << hres << ' ' << vres << std::endl;
        ppm << 255 << std::endl;
        int k = 0;
        for (int i = 0; i < vres; i++)
        {
            for (int j = 0; j < hres; j++)
            {
                Point pixelPosition = topleftPixel - unitUp * i - unitLeft * j;
                Vector pixelColor = rayTrace(Ray(camPosition, pixelPosition), ttl) * 255.0;
                ppm << pixelColor << std::endl;
            }
        }
        ppm.close();

        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<seconds>(stop - start);
        std::cout << "Rendered in: " << duration.count() << " seconds." << std::endl;
    }
    ~Camera() {};
};

#endif