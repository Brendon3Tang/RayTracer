#ifndef MATERIAL_H
#define MATERIAL_H
#include "myFunc.h"


struct HitRecord;

// Class of Material
class Material{
public:
    Material() = default;
    virtual ~Material() = default;

    virtual bool scatter(const Ray &in, const HitRecord &hitRec, Vector3D &attenuation, Ray &scatterRay) const = 0;
};

//----------------------Lambertian反射材质--------------------------

class Lambertian : public Material{
public:
    Lambertian(const Vector3D color) : m_albedo(color){};
    virtual ~Lambertian() = default;

    virtual bool scatter(const Ray &in, const HitRecord &hitRec, Vector3D &attenuation, Ray &scatterRay) const override{
        Vector3D C_S1 = hitRec.m_position + hitRec.m_normal;    //与平面相切于交点P的的球的球心C_S1
        Vector3D E_prime = C_S1 + Vector3D::randomInUnitSphere(); //得到S1中的随机一点E'
    
        scatterRay = Ray(hitRec.m_position, E_prime - hitRec.m_position);
        attenuation = m_albedo;
        return true;
    }
private:
    Vector3D m_albedo;
};

//----------------------End of Lambertian反射材质--------------------------

//----------------------金属镜面反射材质--------------------------
class Metal : public Material{
public:
    Metal(const Vector3D &al, const float &f) : m_albedo(al), m_fuzz(f){   if(f > 1.0f)   m_fuzz = 1.0f;}
    virtual ~Metal() = default;

    virtual bool scatter(const Ray &in, const HitRecord &hitRec, Vector3D &attenuation, Ray &scatterRay) const override{
        Vector3D inDir = in.getDirection();
        Vector3D norm = hitRec.m_normal;
        Vector3D refDir = inDir - norm * (norm.dotProduct(inDir)) * 2.0f;

        scatterRay = Ray(hitRec.m_position, refDir + Vector3D::randomInUnitSphere() * m_fuzz);
        attenuation = m_albedo;

        return (scatterRay.getDirection().dotProduct(norm) > 0.0f);
    }
private:
    float m_fuzz;
    Vector3D m_albedo;
};



class Dielectric : public Material
{
private:
    float refIdx;

public:
    Dielectric(float ri) : refIdx(ri) {}
    virtual ~Dielectric() = default;

    virtual bool scatter(const Ray &in, const HitRecord &rec,
        Vector3D &attenuation, Ray &scattered) const;

    float schlick(float cosine, float ref_idx) const{
        float r0 = (1.0f - ref_idx) / (1.0f + ref_idx);
        r0 = r0 * r0;
        return r0 + (1.0f - r0) * pow((1.0f - cosine), 5.0f);
    }
};

bool Dielectric::scatter(const Ray &in, const HitRecord &rec,
    Vector3D &attenuation, Ray &scattered) const
{
    Vector3D outward_normal;
    Vector3D reflected = Vector3D::reflect(in.getDirection(), rec.m_normal);
    float ni_over_nt;
    attenuation = Vector3D(1.0f, 1.0f, 1.0f);
    Vector3D refracted;
    float reflect_prob;
    float cosine;
    // from inside to outside.
    if (in.getDirection().dotProduct(rec.m_normal) > 0.0f)
    {
        outward_normal = -rec.m_normal;
        ni_over_nt = refIdx;
        cosine = refIdx * in.getDirection().dotProduct(rec.m_normal) / in.getDirection().getLength();
    }
    // from outside to inside.
    else
    {
        outward_normal = rec.m_normal;
        ni_over_nt = 1.0 / refIdx;
        cosine = -in.getDirection().dotProduct(rec.m_normal) / in.getDirection().getLength();
    }

    if (Vector3D::refract(in.getDirection(), outward_normal, ni_over_nt, refracted))
    {
        reflect_prob = schlick(cosine, refIdx);
    }
    else
    {
        scattered = Ray(rec.m_position, reflected);
        reflect_prob = 1.0f;
    }
    if (drand48() < reflect_prob)
        scattered = Ray(rec.m_position, reflected);
    else
        scattered = Ray(rec.m_position, refracted);
    return true;
}



#endif

