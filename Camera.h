#ifndef CAMERA_H
#define CAMERA_H
#include "myFunc.h"

//#include "./lib/glm/glm.hpp"
class Camera
{
public:
    Vector3D m_pos;
    Vector3D m_target;
    Vector3D m_lowerLeftCorner;
    Vector3D m_horizontal;
    Vector3D m_vertical;
    float m_fovy, m_aspect;
    float m_lensRadius, m_focusDist;
    Vector3D m_axisX, m_axisY, m_axisZ;

    Camera(const Vector3D &cameraPos, const Vector3D &target,float vfov,
        float aspect,float aperture, float focus_dist);

    // Getter.
    Ray getRay(const float &s, const float &t) const;
    Vector3D getPosition() const { return m_pos; }
    Vector3D getTarget() const { return m_target; }
    Vector3D getAxisX() const { return m_axisX; }
    Vector3D getAxisY() const { return m_axisY; }
    Vector3D getAxisZ() const { return m_axisZ; }

    // Setter.
    void setPosition(const Vector3D &pos) { m_pos = pos; update(); }
    void setTarget(const Vector3D &_tar) { m_target = _tar; update(); }
    void setFovy(const float &fov) { m_fovy = fov; update(); }
    void setAspect(const float &asp) { m_aspect = asp; update(); }

private:
    void update();
};


#include "Camera.h"

Camera::Camera(const Vector3D &cameraPos, const Vector3D &target,
    float vfov, float aspect, float aperture, float focus_dist){
    m_pos = cameraPos;
    m_target = target;
    m_fovy = vfov;
    m_aspect = aspect;
    m_lensRadius = aperture * 0.5f;
    m_focusDist = focus_dist;
    update();
}

Ray Camera::getRay(const float &s, const float &t) const
{
    return Ray(m_pos , m_lowerLeftCorner + m_horizontal * s + m_vertical * t - m_pos );
}

//需要牢记计算摄像机的三个坐标轴，以及近平面的位置 
void Camera::update()
{
    const Vector3D worldUp(0.0f, 1.0f, 0.0f);
    // frustum.
    float theta = degrees_to_radians(m_fovy);
    float half_height = static_cast<float>(tan(theta * 0.5f)) * m_focusDist;
    float half_width = m_aspect * half_height;

    // camera coordinate system.
    m_axisZ = m_pos - m_target;
    m_axisZ.normalize();
    m_axisX = worldUp.crossProduct(m_axisZ);
    m_axisX.normalize();
    m_axisY = m_axisZ.crossProduct(m_axisX);
    m_axisY.normalize();

    // view port.
    m_lowerLeftCorner = m_pos - m_axisX * half_width - m_axisY * half_height - m_axisZ * m_focusDist;
    m_horizontal = m_axisX * 2.0f * half_width;
    m_vertical = m_axisY * 2.0f * half_height;
}

#endif