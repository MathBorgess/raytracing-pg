#include <iostream>
#include <tuple>
#include <vector>
#include <math.h>
#include <cmath>
#include "src/Shapes.hpp"
#include "src/Scene.hpp"
#include "src/Material.hpp"
#include "src/Camera.hpp"
#include "src/Point.hpp"
#include "src/Ray.hpp"
#include "src/Vector.hpp"
#include "src/MaterialObjReader.hpp"
#include "src/bezier.hpp"
#include "src/Octree.hpp"

using namespace std;

inline Point intersectPlane(Ray ray, Plane& plane) {
    double t = plane.rayIntersect(ray);
    if (t > almostZero) {
        return ray.getPoint(t);
    }
    return ray.from;
}

inline AABB computePlaneBoundingBoxFromCamera(Camera& camera, Point& P0, const Vector& normal, double f) {
    // Dimensões de pixel
    double pixelW = 1.0 / camera.hres;
    double pixelH = 1.0 / camera.vres;

    Vector unitLeft = camera.U * pixelW;
    Vector unitUp = camera.V * pixelH;

    // Canto superior esquerdo da tela em 3D
    Point topleft = camera.camPosition + camera.W * (f / 2.0) + 
        (camera.V * (camera.vres - 1) * pixelH + camera.U * (camera.hres - 1) * pixelW) / 2.0;

    // Cálculo dos 4 cantos da tela
    Point topLeft     = topleft;
    Point topRight    = topleft - unitLeft * (camera.hres - 1);
    Point bottomLeft  = topleft - unitUp * (camera.vres - 1);
    Point bottomRight = bottomLeft - unitLeft * (camera.hres - 1);

    // Interseções com o plano
    Plane plane(normal, P0);
    Point p1 = intersectPlane(Ray(camera.camPosition, topLeft), plane);
    Point p2 = intersectPlane(Ray(camera.camPosition, topRight), plane);
    Point p3 = intersectPlane(Ray(camera.camPosition, bottomLeft), plane);
    Point p4 = intersectPlane(Ray(camera.camPosition, bottomRight), plane);

    // Bounding box
    std::vector<Point> corners = {p1, p2, p3, p4};
    Point small = minBound(corners);
    Point big = maxBound(corners);

    return AABB(small, big);
}

int main()
{

    Camera *camera = nullptr;
    int vres, hres;
    double f;
    int ttl;
    while (cin)
    {
        char input;
        cin >> input;

        if (input == 'c')
        {
            Point c, m;
            Vector up;
            cin >> hres >> vres >> f >> up >> c >> m >> ttl;
            camera = new Camera(c, m, up, vres, hres);
        }
        else if (input == 's')
        {
            Vector o;
            Point center;
            double radius;
            double kd, ks, ka, kr, kt, ior;
            int roughness;
            cin >> center >> radius >> o >> kd >> ks >> ka >> kr >> kt >> roughness >> ior;
            objects.emplace_back(new Sphere(center, radius), o, Vector(ka, ka, ka), Vector(kd, kd, kd), Vector(ks, ks, ks), Vector(kr, kr, kr), kt, roughness, ior);
        }
        else if (input == 'p')
        {
            Vector n, o;
            Point p0;
            double kd, ks, ka, kr, kt, ior;
            int roughness;
            cin >> p0 >> n >> o >> kd >> ks >> ka >> kr >> kt >> roughness >> ior;
            
            Plane* p = new Plane(n, p0);
            p->setPlaneBB(computePlaneBoundingBoxFromCamera(*camera, p0, n, f));

            objects.emplace_back(p, o, Vector(ka, ka, ka), Vector(kd, kd, kd), Vector(ks, ks, ks), Vector(kr, kr, kr), kt, roughness, ior);
        }
        else if (input == 'o')
        {
            string filename, rest;
            cin >> filename >> rest;

            ObjReader objReader = ObjReader(filename);
            objReader.attachMaterials(objects);
        }
        else if (input == 'l')
        {
            Point pos;
            Vector color;
            double intensity;

            cin >> pos >> intensity >> color;
            Light light = Light(pos, color, intensity / 255.0);

            lights.push_back(light);
        }
        else if (input == 'a')
        {
            cin >> ambientLight;
            ambientLight = ambientLight / 255.0;
        }
        else if (input == 'b')
        {
            int resolution, points, curves;
            cin >> resolution >> points >> curves;
            std::vector<Vector> controlPoints;
            for (int i = 0; i < points; i++)
            {
                Vector p;
                cin >> p;
                controlPoints.push_back(p);
            }

            Face face;

            face.ns = 250;
            face.ka = Vector(0.1, 0.1, 0.1);
            face.kd = Vector(0.800836, 0.0, 0.005772);
            face.ks = Vector(0.5, 0.5, 0.5);
            face.ke = Vector(0.2, 0.2, 0.2);
            face.ni = 1.45;
            face.d = 1.0;
            Bezier::generateRevolution(controlPoints, resolution, objects, face, curves);
        }
        else if (input == 'e')
        {
            break;
        }
    }
    
    AABB sceneBounds = OctreeNode::computeSceneBounds(objects);
    octree = new OctreeNode(sceneBounds);
    for (Material& material : objects) {
        octree->insert(&material);
    }
    camera->render(f, ttl);

    return 0;
}
