#ifndef TRACER_H
#define TRACER_H
#include "HitableList.h"
#include "Sphere.h"
#include "Camera.h"
#include "Material.h"

 class Tracer{
    class Setting{
    public:
        int m_maxDepth;
        int m_width, m_height, m_channel;

        Setting() : m_maxDepth(50), m_channel(4){}
    };

    Setting m_config;
    unsigned char *m_image = nullptr;

public:
    Tracer() = default;
    ~Tracer() = default;

    void initialize(int w, int h, int c = 4);
    int getMaxDepth() const{    return m_config.m_maxDepth; }
    int getWidth() const{   return m_config.m_width;    }
    int getHeight() const{  return m_config.m_height;   }
    int getChannel() const{ return m_config.m_channel;  }
    unsigned char* getImage() const{    return m_image; }
    
    unsigned char *render(int samples);

    void setMaxDepth(int depth)  {   m_config.m_maxDepth = depth;    }

private:
    HitableList randomScene();    //返回一个hitable物体的基类指针，因此可以指向不同的物体
    Vector4D tracing(const Ray &r, HitableList world, int depth);
    void drawPixel(unsigned int x, unsigned int y, const Vector4D &color);
 };

 
void Tracer::initialize(int w, int h, int c){
    m_config.m_width = w;
    m_config.m_height = h;
    m_config.m_channel = c;
    if (m_image != nullptr) delete m_image;
    m_image = new unsigned char[m_config.m_width * m_config.m_height * m_config.m_channel];
}

unsigned char *Tracer::render(int samples){
    //设置viewPort
    Vector3D lower_left(-2.0, -1.0, -1.0);
    Vector3D horizontal(4.0, 0.0, 0.0);
    Vector3D vertical(0.0, 2.0, 0.0);
    Vector3D origin(0.0, 0.0, 0.0);

    //scene
    HitableList world = randomScene();

    Vector3D camPos(3, 4, 10);
    Vector3D lookat(0, 0, 0);
    float dist_to_focus = 10.0f;
    float aperture = 0.0f;
    Camera camera(camPos, lookat, 45, static_cast<float>(m_config.m_width) / m_config.m_height, aperture, dist_to_focus);

    for(int row = m_config.m_height - 1; row >= 0; --row){
        for(int col = 0; col < m_config.m_width; col++){
            Vector4D color;
            for(int i = 0; i < samples; i++){
                float u = static_cast<float>(col + drand48())/static_cast<float>(m_config.m_width);
                float v = static_cast<float>(row + drand48())/static_cast<float>(m_config.m_height);
                Ray ray = camera.getRay(u, v);
                color += tracing(ray, world, 0);
            }
            color /= static_cast<float>(samples);
            color.w = 1.f;
            // gamma correction.
            color = Vector4D(sqrt(color.x), sqrt(color.y), sqrt(color.z), color.w);
            drawPixel(col, row, color);
        }
    }
    world.clearHitable();
    return m_image;
}

void Tracer::drawPixel(unsigned int x, unsigned int y, const Vector4D &color){
    if(x < 0 || x >= m_config.m_width || y < 0 || y >= m_config.m_height)
        return;
    unsigned int index = (y*m_config.m_width + x) * m_config.m_channel;
    m_image[index] = static_cast<unsigned char>(255 * color.x);
    m_image[index + 1] = static_cast<unsigned char>(255 * color.y);
    m_image[index + 2] = static_cast<unsigned char>(255 * color.z);
    m_image[index + 3] = static_cast<unsigned char>(255 * color.w);
}


HitableList Tracer::randomScene(){
    HitableList list;
    list.addHitable(make_shared<Sphere>(Vector3D(0, -1000.0, 0), 1000, make_shared<Lambertian>(Vector3D(0.5, 0.5, 0.5))));
    for (int a = -11; a < 11; ++a){
        for (int b = -11; b < 11; ++b){
            float choose_mat = drand48();
            Vector3D center(a + 0.9*drand48(), 0.2, b + 0.9*drand48());
            if ((center - Vector3D(4, 0.2, 0)).getLength() > 0.9){
                // diffuse.
                if (choose_mat < 0.4f)
                    list.addHitable(make_shared<Sphere>(center, 0.2, make_shared<Lambertian>
                    (Vector3D(drand48()*drand48(),
                        drand48()*drand48(),
                        drand48()*drand48()))));
                // metal
                else if (choose_mat < 0.6f)
                    list.addHitable(make_shared<Sphere>(center, 0.2, make_shared<Metal>(Vector3D(0.5f*(1.0f + drand48()),0.5f*(1.0f + drand48()),0.5f*(1.0f + drand48())),
                        0.5f*drand48())));
                // glass
                else
                    list.addHitable(make_shared<Sphere>(center, 0.2, make_shared<Dielectric>(1.5f)));
            }
        }
    }
    list.addHitable(make_shared<Sphere>(Vector3D(0, 1, 0), 1.0, make_shared<Dielectric>(1.5f)));
    list.addHitable(make_shared<Sphere>(Vector3D(-4, 1, 0), 1.0, make_shared<Lambertian>(Vector3D(0.4, 0.2, 0.1))));
    list.addHitable(make_shared<Sphere>(Vector3D(4, 1, 0), 1.0, make_shared<Metal>(Vector3D(0.7, 0.6, 0.5), 0.0f)));
    return list;
}

Vector4D Tracer::tracing(const Ray &r, HitableList world, int depth){
    HitRecord rec;

    if(world.hit(r, 0.001f, __FLT_MAX__, rec)){
        Ray scatterRay;
        Vector3D attenuation;
        if(depth < m_config.m_maxDepth && rec.mat_ptr->scatter(r, rec, attenuation, scatterRay)){
            return tracing(scatterRay, world, depth+1) * Vector4D(attenuation.x, attenuation.y, attenuation.z, 1.0f);
        }
        else{
            return Vector4D(0.f, 0.f, 0.f, 1.f);
        }
    }
    else{    
        float t = 0.5f * (r.getDirection().y + 1.f);
        Vector3D tmp = (Vector3D(1.f, 1.f, 1.f)*(1-t) + Vector3D(0.5f, 0.7f, 1.0f)*t);
        return Vector4D(tmp.x, tmp.y, tmp.z, 1.0f);
    }
}
 #endif