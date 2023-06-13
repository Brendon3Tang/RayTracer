#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H
#include "myFunc.h"
#include "Hitable.h"
#include <vector>
#include <memory>

class HitableList : public Hitable{
public:
    HitableList() = default;
    HitableList(shared_ptr<Hitable> obj) {addHitable(obj);}
    ~HitableList() = default;

    void addHitable(shared_ptr<Hitable> object){  m_list.push_back(object); }

    void clearHitable(){
        m_list.clear();
    }

    virtual bool hit(const Ray &ray, const float &t_min, const float &t_max, HitRecord& ret) const;

    std::vector<shared_ptr<Hitable>> m_list;
};

bool HitableList::hit(const Ray &ray, const float &t_min, const float &t_max, HitRecord& ret) const{
    HitRecord tempRec;
    float ClosetSoFar = t_max;
    bool isHit = false;

    for(int i = 0; i < m_list.size(); i++){
        if(m_list[i]->hit(ray, t_min, ClosetSoFar, tempRec)){
            ret = tempRec;
            isHit = true;
            ClosetSoFar = tempRec.m_t;
        }
    }
    return isHit;
}
#endif