#ifndef HITTABLE_H
#define HITTABLE_H
#include "Ray.h"
#include "myFunc.h"
class Material;

// HitRecord有参数t，并且记录了hit点的位置、法线、材质信息
struct HitRecord{
    float m_t;
    Vector3D m_position;
    Vector3D m_normal;
    shared_ptr<Material> mat_ptr;
};

// Hitable是能够被射线碰到物体的抽象类。他有一个接口hit，会根据ray和t更新HitRecord
class Hitable{
public:
    Hitable() = default;
    virtual ~Hitable() {}
    virtual bool hit(const Ray &ray, const float &t_min, const float &t_max, HitRecord &ret) const = 0;
};

#endif