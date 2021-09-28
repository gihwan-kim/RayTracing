#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "vec3.h"


class sphere : public hittable
{
public:
	point3 center;
	double radius;
    shared_ptr<material> mat_ptr;

public:
	sphere() {}
	sphere(point3 cen, double r, shared_ptr<material> m) : center(cen), radius(r), mat_ptr(m) {};


	virtual bool hit(const ray &r, double t_min,
						double t_max, hit_record &rec) const override;
};


// ray 가 원과 만나는지
// hit_record
//	만나는 지점에 대한 정보

// 충돌 지점의 범위 t_min ~ t_max
//		좀더 가까운 것의 normal vector 를 사용


/*  이차방정식 판별식
** 판별식 : b^2 - 4 *a *c
**     양수    : 해 2개
**     음수    : 해가 없다.
**     0       : 해가 1개
*/
bool sphere::hit(const ray& r, double t_min, double t_max, hit_record& rec) const
{
	auto	origin_sub_center = r.origin() - center;
	auto	a = dot(r.direction(), r.direction());
	auto	half_b = dot(origin_sub_center, r.direction());
	auto	c = dot(origin_sub_center, origin_sub_center) - radius * radius;
	auto	discriminant = half_b * half_b - a * c;

	if (discriminant < 0)
		return (false);


/*
** 	현재 root 가 특점 범위에 속하지 않으면 다른 root 사용
** 	두 root 중 가장 가까이에 있는 값을 선택한다.
** 	처음 root 가 - 니까 무조건 가까이 있으니 먼저 범위를 검사한다.
*/
	auto	sqrt_discriminant = sqrt(discriminant);
	auto	root = (- half_b - sqrt_discriminant) / a ;
	if (root < t_min || root > t_max)
	{
		root = (- half_b + sqrt_discriminant) / a ;
		if (root < t_min || root > t_max)
			return (false);
	}

	rec.t = root;
    rec.p = r.at(rec.t);
	rec.set_face_normal(r, (rec.p - center) / radius);
    // rec.normal = (rec.p - center) / radius;

	// 물체가 충돌했을때 어떤 material 인지 알 수 있도록 record 에 기록해줌
	// 이후 color 값을 정하기위해 record 사용할때 material ptr 의 정보를 통해 결정
	rec.mat_ptr = mat_ptr;

	return (true);
}

#endif