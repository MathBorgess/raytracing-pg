#ifndef AABBHEADER
#define AABBHEADER
class AABB {
public:
    Point min;
    Point max;

    AABB() {}
    AABB(const Point& min, const Point& max) : min(min), max(max) {}

    bool intersects(const Ray& ray) const {
        double tmin = (min.getX() - ray.from.getX()) / ray.direction.getX();
        double tmax = (max.getX() - ray.from.getX()) / ray.direction.getX();

        if (tmin > tmax) std::swap(tmin, tmax);

        double tymin = (min.getY() - ray.from.getY()) / ray.direction.getY();
        double tymax = (max.getY() - ray.from.getY()) / ray.direction.getY();

        if (tymin > tymax) std::swap(tymin, tymax);

        if ((tmin > tymax) || (tymin > tmax)) return false;

        if (tymin > tmin) tmin = tymin;
        if (tymax < tmax) tmax = tymax;

        double tzmin = (min.getZ() - ray.from.getZ()) / ray.direction.getZ();
        double tzmax = (max.getZ() - ray.from.getZ()) / ray.direction.getZ();

        if (tzmin > tzmax) std::swap(tzmin, tzmax);

        return !((tmin > tzmax) || (tzmin > tmax));
    }

    bool intersectsBox(const AABB& other) const {
        return (min.getX() <= other.max.getX() && max.getX() >= other.min.getX()) &&
               (min.getY() <= other.max.getY() && max.getY() >= other.min.getY()) &&
               (min.getZ() <= other.max.getZ() && max.getZ() >= other.min.getZ());
    }
    
    static AABB surroundingBox(const AABB& box0, const AABB& box1) {
        Point small = minBound(box0.min, box1.min);
        Point big = maxBound(box0.max, box1.max);

        return AABB(small, big);
    }
};

#endif