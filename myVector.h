#ifndef MYVECTOR_H
#define MYVECTOR_H
#include<cmath>
//#include<algorithm>
using namespace std;

bool equal(const float a, const float b){
    return a == b;
}

class Vector2D
{
public:
    float x,y;

    // constructors
    Vector2D():x(0.0f), y(0.0f) {}
    Vector2D(float newX, float newY):x(newX), y(newY){}
    Vector2D(const float * rhs):x(*rhs), y((*rhs)+1) {}
    Vector2D(const Vector2D & rhs):x(rhs.x), y(rhs.y){}
    ~Vector2D() = default;

    // setter,getter
    void set(float newX, float newY){x=newX;y=newY;	}
    void setX(float newX) {x = newX;}
    void setY(float newY) {y = newY;}
    float getX() const {return x;}
    float getY() const {return y;}

    // normalization
    void normalize(){
        float len = getLength();
        x = x/len;
        y = y/len;
    }
    Vector2D getNormalize()const;

    // length
    float getLength() const { return static_cast<float>(sqrt(x*x + y*y));}
    float getSquaredLength()const{return static_cast<float>(x*x + y*y);}

    // overloaded operators
    Vector2D operator+(const Vector2D &rhs) const {return Vector2D(x + rhs.x, y + rhs.y);}
    Vector2D operator-(const Vector2D &rhs) const {return Vector2D(x - rhs.x, y - rhs.y);}
    Vector2D operator*(const float rhs) const {return Vector2D(x*rhs, y*rhs);}
    Vector2D operator/(const float rhs) const {return (rhs==0) ? Vector2D(0.0f, 0.0f) : Vector2D(x / rhs, y / rhs);}

    bool operator==(const Vector2D &rhs) const {return (x == rhs.x) && (y == rhs.y);}
    bool operator!=(const Vector2D &rhs) const {return !((*this)==rhs);}

    void operator+=(const Vector2D &rhs){x+=rhs.x;	y+=rhs.y;}
    void operator-=(const Vector2D &rhs){x-=rhs.x;	y-=rhs.y;}
    void operator*=(const float rhs){x*=rhs;y*=rhs;}
    void operator/=(const float rhs){if(!(rhs == 0.0)){x/=rhs;y/=rhs;}}

    Vector2D operator-() const {return Vector2D(-x, -y);}
    Vector2D operator+() const {return *this;}

    // interpolation
    Vector2D lerp(const Vector2D &v2,const float factor)const {return (*this)*(1.0f - factor) + v2*factor;}
    Vector2D quadraticInterpolate(const Vector2D & v2, const Vector2D & v3, const float factor) const
    {return (*this)*(1.0f-factor)*(1.0f-factor) + v2*2.0f*factor*(1.0f-factor) + v3*factor*factor;}

};

class Vector3D
{
public:
    float x,y,z;

    // constructors
    Vector3D():x(0.0f), y(0.0f), z(0.0f){}
    Vector3D(float newX, float newY, float newZ):x(newX), y(newY), z(newZ){}
    Vector3D(const float * rhs):x(*rhs), y(*(rhs+1)), z(*(rhs+2)){}
    Vector3D(const Vector3D &rhs):x(rhs.x), y(rhs.y), z(rhs.z){}
    ~Vector3D() = default;

    // setter,getter
    void set(float newX, float newY, float newZ){x=newX;y=newY;z=newZ;}
    void setX(float newX) {x = newX;}
    void setY(float newY) {y = newY;}
    void setZ(float newZ) {z = newZ;}
    float getX() const {return x;}
    float getY() const {return y;}
    float getZ() const {return z;}

    // normalization
    void normalize(){
        float len = getLength();
        x = x/len;
        y = y/len;
        z = z/len;
    }
    Vector3D getNormalized() const;

    // length caculation
    float getLength() const {return static_cast<float>(sqrt(x*x+y*y+z*z));}
    float getSquaredLength() const {return x*x+y*y+z*z;}

    // product
    float dotProduct(const Vector3D &rhs) const {return x*rhs.x + y*rhs.y + z*rhs.z;}
    Vector3D crossProduct(const Vector3D &rhs) const
    {return Vector3D(y*rhs.z - z*rhs.y, z*rhs.x - x*rhs.z, x*rhs.y - y*rhs.x);}

    // linear interpolation
    Vector3D lerp(const Vector3D &v2, float factor) const {return (*this)*(1.0f-factor) + v2*factor;}
    Vector3D QuadraticInterpolate(const Vector3D &v2, const Vector3D &v3, float factor) const
    {return (*this)*(1.0f-factor)*(1.0f-factor) + v2*2.0f*factor*(1.0f-factor) + v3*factor*factor;}

    // overloaded operators
    Vector3D operator+(const Vector3D &rhs) const {return Vector3D(x + rhs.x, y + rhs.y, z + rhs.z);}
    Vector3D operator-(const Vector3D &rhs) const {return Vector3D(x - rhs.x, y - rhs.y, z - rhs.z);}
    Vector3D operator*(const float rhs) const {return Vector3D(x*rhs, y*rhs, z*rhs);}
    Vector3D operator*(const Vector3D r) const {return Vector3D(x*r.x, y*r.y, z*r.z);}
    Vector3D operator/(const float rhs) const {return ((rhs == 0.0f))?Vector3D(0.0f, 0.0f, 0.0f):Vector3D(x/rhs, y/rhs, z/rhs);}

    bool operator==(const Vector3D &rhs) const {return ((x == rhs.x) && (y == rhs.y) && (z == rhs.z));}
    bool operator!=(const Vector3D &rhs) const {return !((*this)==rhs);}

    void operator+=(const Vector3D &rhs) {x+=rhs.x;y+=rhs.y;z+=rhs.z;}
    void operator-=(const Vector3D & rhs) {x-=rhs.x;y-=rhs.y;z-=rhs.z;}
    void operator*=(const float rhs){x*=rhs;y*=rhs;z*=rhs;}
    void operator/=(const float rhs){if(!(rhs == 0.0f)){x/=rhs; y/=rhs; z/=rhs;}}

    Vector3D operator-() const {return Vector3D(-x, -y, -z);}
    Vector3D operator+() const {return *this;}

    /*randomInUnitSphere会在以点C(1,1,1)为球心的单位球中随机生成一个
    点E，然后返回方位向量CE
    */
    static Vector3D randomInUnitSphere(){
        Vector3D pos;

        //E点要乘2是因为球心在(1,1,1)，所以球中点的取值范围是[0,2]。而drand48是个生成[0, 1)之间随机数的函数
        do{
            pos = Vector3D(drand48(), drand48(), drand48()) * 2.0f - Vector3D(1.0f, 1.0f, 1.0f);
        }
        while(pos.getSquaredLength() >= 1.0f);

        return pos;
    }

    static Vector3D reflect(const Vector3D &ray, const Vector3D &normal)
    {
        return ray - normal * (ray.dotProduct(normal)) * 2.0f;
    }

    static bool refract(const Vector3D &ray, const Vector3D &normal,
    float niOvernt, Vector3D &refracted)
    {
        Vector3D uv = ray;
        uv.normalize();
        float dt = uv.dotProduct(normal);
        float discriminant = 1.0f - niOvernt * niOvernt * (1.0f - dt * dt);
        if (discriminant > 0.0f)
        {
            refracted = (uv - normal * dt) * niOvernt - normal * sqrt(discriminant);
            return true;
        }
        else
            return false;
    }
};

class Vector4D
{
public:
    float x,y,z,w;

    // constructors
    Vector4D():x(0.0f), y(0.0f), z(0.0f), w(0.0f){}
    Vector4D(float newX, float newY, float newZ, float newW):x(newX), y(newY), z(newZ), w(newW){}
    Vector4D(const float * rhs):x(*rhs), y(*(rhs+1)), z(*(rhs+2)), w(*(rhs+3)){}
    Vector4D(const Vector4D &rhs):x(rhs.x), y(rhs.y), z(rhs.z), w(rhs.w){}
    Vector4D(const Vector3D & rhs):	x(rhs.x), y(rhs.y), z(rhs.z), w(1.0f){}
    ~Vector4D() = default;

    // setter,getter
    void set(float newX, float newY, float newZ, float newW){x=newX;y=newY;z=newZ;w=newW;}
    void setX(float newX) {x = newX;}
    void setY(float newY) {y = newY;}
    void setZ(float newZ) {z = newZ;}
    void setW(float newW) {w = newW;}
    float getX() const {return x;}
    float getY() const {return y;}
    float getZ() const {return z;}
    float getW() const {return w;}

    // product
    float dotProduct(const Vector4D &rhs) const {return x*rhs.x + y*rhs.y + z*rhs.z + w*rhs.w;}

    // linear interpolation
    Vector4D lerp(const Vector4D &v2, float factor) const {return (*this)*(1.0f-factor) + v2*factor;}
    Vector4D QuadraticInterpolate(const Vector4D &v2, const Vector4D &v3, float factor) const
    {return (*this)*(1.0f-factor)*(1.0f-factor)+v2*2.0f*factor*(1.0f-factor)+v3*factor*factor;}

    // overloaded operators
    Vector4D operator+(const Vector4D &rhs) const {return Vector4D(x+rhs.x, y+rhs.y, z+rhs.z, w+rhs.w);}
    Vector4D operator-(const Vector4D &rhs) const {return Vector4D(x-rhs.x, y-rhs.y, z-rhs.z, w-rhs.w);}
    Vector4D operator*(const float rhs) const {return Vector4D(x*rhs, y*rhs, z*rhs, w*rhs);}
    Vector4D operator*(const Vector4D r) const {return Vector4D(x*r.x, y*r.y, z*r.z, w*r.w);}
    Vector4D operator/(const float rhs) const
    {return ((rhs == 0.0f))?Vector4D(0.0f, 0.0f, 0.0f, 0.0f):Vector4D(x/rhs, y/rhs, z/rhs, w/rhs);}

    bool operator==(const Vector4D &rhs) const {return ((x == rhs.x)&& (y == rhs.y)&& (z == rhs.z)&& (w == rhs.w));}
    bool operator!=(const Vector4D &rhs) const {return !((*this)==rhs);}

    void operator+=(const Vector4D &rhs) {x+=rhs.x;y+=rhs.y;z+=rhs.z;w+=rhs.w;}
    void operator-=(const Vector4D & rhs) {x-=rhs.x;y-=rhs.y;z-=rhs.z;w-=rhs.w;}
    void operator*=(const float rhs){x*=rhs;y*=rhs;z*=rhs;w*=rhs;}
    void operator/=(const float rhs){if(!(rhs == 0.0f)){x/=rhs; y/=rhs; z/=rhs; w/=rhs;}}

    Vector4D operator-() const {return Vector4D(-x, -y, -z, -w);}
    Vector4D operator+() const {return *this;}
};

#endif