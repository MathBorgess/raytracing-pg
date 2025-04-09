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
        Point small(fmin(box0.min.getX(), box1.min.getX()),
                    fmin(box0.min.getY(), box1.min.getY()),
                    fmin(box0.min.getZ(), box1.min.getZ()));

        Point big(fmax(box0.max.getX(), box1.max.getX()),
                  fmax(box0.max.getY(), box1.max.getY()),
                  fmax(box0.max.getZ(), box1.max.getZ()));

        return AABB(small, big);
    }

    static AABB computeSceneBounds(std::vector<Material> objects) {
        if (objects.empty()) return AABB();
    
        AABB firstBox = objects[0].getShape()->getBoundingBox();
        AABB sceneBox = firstBox;
    
        for (size_t i = 1; i < objects.size(); ++i) {
            AABB box = objects[i].getShape()->getBoundingBox();
            sceneBox = AABB::surroundingBox(sceneBox, box);
        }
    
        return sceneBox;
    }
};
