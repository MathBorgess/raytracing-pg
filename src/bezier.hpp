class Bezier
{
public:
    static int binomialCoefficient(int n, int k)
    {
        if (k == 0 || k == n)
            return 1;
        return binomialCoefficient(n - 1, k - 1) + binomialCoefficient(n - 1, k);
    }

    static float bernstein(int i, int n, float t)
    {
        return binomialCoefficient(n, i) * pow(t, i) * pow(1 - t, n - i);
    }

    static Vector curvePoint(const std::vector<Vector> &controlPoints, float t)
    {
        int n = controlPoints.size() - 1;
        Vector point(0, 0, 0);

        for (int i = 0; i <= n; i++)
        {
            float b = bernstein(i, n, t);
            point = point + (controlPoints[i] * b);
        }

        return point;
    }

    static void testCurve(const std::vector<Vector> &controlPoints, int resolution, std::vector<Material> &objects, Face &face)
    {
        for (int i = 0; i <= resolution; i++)
        {
            float t = float(i) / resolution;
            Vector point = curvePoint(controlPoints, t);
            Point p(point.x, point.y, point.z);
            Sphere *sphere = new Sphere(p, 0.05);
            objects.emplace_back(sphere, face.kd * 255, face.ka, face.kd, face.ks, face.ke, 1 - face.d, face.ns, face.ni);
        }
    }

    static void generateRevolution(const std::vector<Vector> &controlPoints,
                                   int resolution, std::vector<Material> &objects, Face &face, int steps)
    {
        std::vector<Vector> surfacePoints;
        for (int i = 0; i <= resolution; i++)
        {
            float t = float(i) / resolution;
            surfacePoints.push_back(curvePoint(controlPoints, t));
        }
        int angleStep = 360 / steps;
        for (int phi = 0; phi <= 360; phi += angleStep)
        {
            float angle = phi;
            float nextAngle = (phi + angleStep);

            for (int i = 0; i < surfacePoints.size() - 1; i++)
            {
                Matrix rot1 = Matrix().rotateY(angle);
                Matrix rot2 = Matrix().rotateY(nextAngle);

                Vector p1 = rot1 * surfacePoints[i];
                Vector p2 = rot1 * surfacePoints[i + 1];
                Vector p3 = rot2 * surfacePoints[i];
                Vector p4 = rot2 * surfacePoints[i + 1];

                Point p1Point(p1.x, p1.y, p1.z);
                Point p2Point(p2.x, p2.y, p2.z);
                Point p3Point(p3.x, p3.y, p3.z);
                Point p4Point(p4.x, p4.y, p4.z);

                Vector edge1T1 = p2Point - p1Point;
                Vector edge2T1 = p3Point - p1Point;
                Vector normal1 = edge1T1.cross(edge2T1).normalize();

                Vector edge1T2 = p4Point - p2Point;
                Vector edge2T2 = p3Point - p2Point;
                Vector normal2 = edge1T2.cross(edge2T2).normalize();

                Triangle *t1 = new Triangle(p1Point, p2Point, p3Point, normal1);
                Triangle *t2 = new Triangle(p2Point, p4Point, p3Point, normal2);

                objects.emplace_back(t1, face.kd * 255, face.ka, face.kd, face.ks, face.ke, 1 - face.d, face.ns, face.ni);
                objects.emplace_back(t2, face.kd * 255, face.ka, face.kd, face.ks, face.ke, 1 - face.d, face.ns, face.ni);
            }
        }
    }
    //                      int resolution, std::vector<triangle *> &triangles, Material material)
    // {
    //     std::vector<std::vector<Vector>> surfacePoints;
    //     for (int i = 0; i <= resolution; i++)
    //     {
    //         float u = float(i) / resolution;
    //         surfacePoints.push_back({});
    //         for (int j = 0; j <= resolution; j++)
    //         {
    //             float v = float(j) / resolution;
    //             surfacePoints[i].push_back(surfacePoint(controlPoints, u, v));
    //         }
    //     }
    //     for (int i = 0; i < resolution; i++)
    //     {
    //         for (int j = 0; j < resolution; j++)
    //         {
    //             Vector p1 = surfacePoints[i][j];
    //             Vector p2 = surfacePoints[i + 1][j];
    //             Vector p3 = surfacePoints[i][j + 1];
    //             Vector p4 = surfacePoints[i + 1][j + 1];
    //             triangles.push_back(new triangle(p1, p2, p3, Vector(1, 0, 0), material));
    //             triangles.push_back(new triangle(p2, p4, p3, Vector(1, 0, 0), material));
    //         }
    //     }
    // }
};