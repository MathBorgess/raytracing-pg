#include <tuple>

struct RayCastResult
{
    Material *hit;
    Vector color;
    Ray reflectedRay;

    RayCastResult(Material *hit, Vector color, Ray reflectedRay) : hit(hit), color(color), reflectedRay(reflectedRay) {}
};

RayCastResult rayCast(Ray ray)
{
    Vector color = ambientLight;
    Material *hit = nullptr;
    double intersectT = INFINITY;
    Point hitPoint;
    Vector normal;
    std::tie(hit, intersectT) = Material::nearest(ray);
    if (hit)
    {
        hitPoint = ray.getPoint(intersectT);
        normal = hit->getShape()->getNormal(ray, intersectT);
        color = hit->shade(&hitPoint, -1 * ray.direction, &normal);
    }
    return RayCastResult(hit, color, Ray(hitPoint, hitPoint + (ray.direction - 2 * normal * normal.dot(ray.direction)) ) );
}

Vector rayTrace(Ray ray, int ttl)
{
    RayCastResult cast = rayCast(ray);
    Vector color = cast.color;
    
    if (!cast.hit || (cast.reflectedRay.from.distance(ray.from) <= almostZero) || ttl == 0)
    {
        return color;
    }
    
    try
    {
        if (cast.hit->kr.getX() > almostZero || cast.hit->kr.getY() > almostZero || cast.hit->kr.getZ() > almostZero)
        {
            Vector reflectedColor = rayTrace(cast.reflectedRay, ttl - 1);
            color = color + reflectedColor.elementWiseMultiplication(cast.hit->kr);
        }
        if (cast.hit->kt > almostZero)
        {
            Ray refractedRay = ray.refract(cast.hit->getShape()->getNormal(ray, 0), cast.hit->ior);
            color = color + rayTrace(refractedRay, ttl - 1) * cast.hit->kt;
        }
    }
    catch (int e) // refraction failed (total internal reflection)
    {
        color = color + rayTrace(cast.reflectedRay, ttl - 1);   
    }
    
    return color;
}
