#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"

class material;


// 파라미터가 많아지는 것을 피하기 위해 구조체 형식으로 담아서 데이터를 정리
struct hit_record {
    point3 p;           // 충돌 지점
    vec3 normal;        // 법선 벡터
    double t;
    bool front_face;
    shared_ptr<material> mat_ptr;   // ray 가 물체에 hit 한 경우 반사되는 ray 의 경로를 계산하기 위해

/*
** 	dot(r.direction(), outward_normal)
**      음수 : 방향이 다르다, ray 가 object 밖에서 부터 시작
**      양수 : 방향이 같다., ray 가 obejct 안에서 부터 시작
*/
    inline void set_face_normal(const ray& r, const vec3& outward_normal) {
        front_face = dot(r.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal :-outward_normal;
    }
};

class hittable
{
public:
    virtual bool hit(const ray &r, double t_min,
                     double t_max, hit_record &rec) const = 0;
};

#endif