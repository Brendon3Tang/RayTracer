#ifndef RAY_H
#define RAY_H
#include "myVector.h"

class Ray{
private:
    Vector3D m_origin;
    Vector3D m_direction;

public:
    Ray() = default;
    ~Ray() = default;
    Ray(const Vector3D &orig, const Vector3D &dir) : m_origin(orig), m_direction(dir){
        m_direction.normalize();
    }

    //Getter
    Vector3D getOrigin() const  {   return m_origin; }
    Vector3D getDirection() const { return m_direction; }

    // p(t) = origin + t*dir;
    Vector3D PointAt(const float &t) const  {   return m_origin + m_direction * t; }
};
#endif