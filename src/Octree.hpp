#ifndef OCTREEHEADER
#define OCTREEHEADER

#include <vector>
#include <tuple>
#include <limits>
#include <algorithm>
#include "AABB.hpp"
#include "Point.hpp"
#include "Shapes.hpp"

class OctreeNode {
private:
    void subdivide() {
        Point min = bounds.min;
        Point max = bounds.max;
        Point mid = (min + max) * 0.5;

        for (int i = 0; i < 8; i++) {
            Point newMin(
                (i & 1) ? mid.getX() : min.getX(),
                (i & 2) ? mid.getY() : min.getY(),
                (i & 4) ? mid.getZ() : min.getZ()
            );
            Point newMax(
                (i & 1) ? max.getX() : mid.getX(),
                (i & 2) ? max.getY() : mid.getY(),
                (i & 4) ? max.getZ() : mid.getZ()
            );
            children[i] = new OctreeNode(AABB(newMin, newMax), depth + 1, maxDepth, maxObjects);
        }
    }

public:    
    AABB bounds;
    std::vector<Material*> nodeObjs;
    OctreeNode* children[8] = {nullptr};

    int maxDepth;
    int maxObjects;
    int depth;

    OctreeNode(const AABB& bounds, int depth = 0, int maxDepth = 8, int maxObjects = 5)
        : bounds(bounds), depth(depth), maxDepth(maxDepth), maxObjects(maxObjects) {}

    ~OctreeNode() {
        for (int i = 0; i < 8; i++) {
            delete children[i];
        }
    }

    void insert(Material* obj) {
        AABB objBox = obj->getShape()->getBoundingBox();

        if (!bounds.intersectsBox(objBox)) return;

        if ((nodeObjs.size() < maxObjects || depth >= maxDepth) && children[0] == nullptr) {
            nodeObjs.push_back(obj);
            return;
        }

        if (children[0] == nullptr) {
            subdivide();
            // Re-distribuir objetos já existentes
            std::vector<Material*> toReinsert = nodeObjs;
            nodeObjs.clear();
            for (Material* m : toReinsert) {
                insert(m);
            }
        }

        for (int i = 0; i < 8; i++) {
            if (children[i]->bounds.intersectsBox(objBox)) {
                children[i]->insert(obj);
            }
        }
    }

    std::tuple<Material*, double> nearest(const Ray& ray) {
        if (!bounds.intersects(ray)) return {nullptr, INFINITY};

        Material* hit = nullptr;
        double closestT = INFINITY;

        for (Material* obj : nodeObjs) {
            double t = obj->getShape()->rayIntersect(const_cast<Ray&>(ray));
            if (t > almostZero && t < closestT) {
                closestT = t;
                hit = obj;
            }
        }

        if (children[0]) {
            for (int i = 0; i < 8; i++) {
                Material* childHit;
                double t;
                std::tie(childHit, t) = children[i]->nearest(ray);
                if (childHit && t < closestT) {
                    closestT = t;
                    hit = childHit;
                }
            }
        }

        return {hit, closestT};
    }

};

#endif

OctreeNode* octree = nullptr;
