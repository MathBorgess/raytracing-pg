#include <tuple>

class Material
{

public:
    Shape *shape;
    Vector color;
    Vector ka, kd, ks, kr;
    int roughness;
    double kt, ior;

    Material(Shape *shape, Vector color, Vector ka, Vector kd, Vector ks, Vector kr, double kt, int roughness, double ior) : shape(shape), color(color / 255.0), ka(ka), kd(kd), ks(ks), kr(kr), kt(kt), roughness(roughness), ior(ior)
    {
    }

    Shape *getShape() const
    {
        return shape;
    }

    Vector getColor() const
    {
        return color;
    }

    Vector shade(Point *point, Vector view, Vector *normal);

    static std::tuple<Material *, double> nearest(Ray ray);
};

std::vector<Material> objects;

Vector Material::shade(Point *point, Vector view, Vector *normal)
{
    // Vector resColor = color.elementWiseMultiplication(ambientLight.elementWiseMultiplication(ka));
    Vector resColor = ambientLight.elementWiseMultiplication(ka);

    for (Light light : lights)
    {
        Vector lightDirection = (light.position - *point).normalize();

        Vector r = ((*normal) * 2.0 * normal->dot(lightDirection)) - lightDirection;

        double t;
        Material *shadow;
        std::tie(shadow, t) = Material::nearest(Ray(*point, light.position));

        if (shadow == nullptr || lightDirection.dot(light.position - *point) < t)
        {
            double dotdiff = lightDirection.dot(*normal);
            if (dotdiff > almostZero)
            {
                resColor = resColor + color.elementWiseMultiplication(light.color).elementWiseMultiplication(kd) * dotdiff * light.intensity;
                // resColor = resColor + light.color.elementWiseMultiplication(kd) * dotdiff * light.intensity;
            }

            double dotspec = r.dot(view);
            if (dotspec > almostZero)
            {
                resColor = resColor + light.color.elementWiseMultiplication(ks) * pow(dotspec, roughness) * light.intensity;
            }
        }
    }
    return resColor;
}

std::tuple<Material *, double> Material::nearest(Ray ray)
{
    Material *hit = nullptr;
    double intersectT = INFINITY;

    for (Material &material : objects)
    {
        double t = material.getShape()->rayIntersect(ray);

        if (t > almostZero && t < intersectT)
        {
            intersectT = t;
            hit = &material;
        }
    }
    if (intersectT != INFINITY)
    {
        return std::make_tuple(hit, intersectT);
    }
    return std::make_tuple(hit, INFINITY);
}