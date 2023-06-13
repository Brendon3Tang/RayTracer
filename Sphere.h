#ifndef SPHERE_H
#define SPHERE_H
#include "Hitable.h"
#include "myFunc.h"
class Sphere : public Hitable{
public:
    Sphere(const Vector3D &cen, const float r, shared_ptr<Material> mat_ptr) : m_center(cen), m_radius(r), mat_ptr(mat_ptr){}
    ~Sphere() = default;
    virtual bool hit(const Ray &ray, const float &t_min, const float &t_max, HitRecord &ret) const;
//private:    
    Vector3D m_center;
    float m_radius;
    shared_ptr<Material> mat_ptr;
};

bool Sphere::hit(const Ray &ray, const float &t_min, const float &t_max, HitRecord &ret) const {
    Vector3D dir = ray.getDirection();
    Vector3D ori = ray.getOrigin();
    Vector3D oc = ori - m_center;

    float a = dir.dotProduct(dir);
    float b = 2*(dir.dotProduct(oc));
    float c = oc.dotProduct(oc) - m_radius*m_radius;

    float discriminant = b*b - 4*a*c;

    if(discriminant > 0){//有两个解
        float tempT;
        //先求近的解，如果近的解符合条件就用这个解
        tempT = (-b - sqrt(discriminant))/(2*a);
        if(tempT > t_min && tempT < t_max){
            ret.m_t = tempT;
            ret.m_position = ray.PointAt(tempT);
            ret.m_normal = (ret.m_position - m_center)/(m_radius);
            ret.mat_ptr = mat_ptr;
            return true;
        }
        
        //如果近的解不符合条件，那么久用远的解
        tempT = (-b + sqrt(discriminant))/(2*a);
        if(tempT > t_min && tempT < t_max){
            ret.m_t = tempT;
            ret.m_position = ray.PointAt(tempT);
            ret.m_normal = (ret.m_position - m_center)/(m_radius);
            ret.mat_ptr = mat_ptr;
            return true;
        }

    }
    return false;
}
#endif
