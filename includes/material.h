#ifndef MATERIAL_H
#define MATERIAL_H

#include "rtweekend.h"
#include "hittable.h"

struct hit_record;

// 물체마다 재질이 다르기 때문에 Material 추상 클래스를 만들어 준다.

class material
{
public:
	// Attenuation : scatter 될 때 감쇠되는 양 1 ~ 0 (1: 모두 감쇠)
	virtual bool scatter(
		const ray &r_in, const hit_record &rec, color &attenuation, ray &scattered) const = 0;
};

class lambertian : public material
{
public:
	lambertian(const color &a) : albedo(a) {}

	virtual bool scatter(
		const ray &r_in, const hit_record &rec, color &attenuation, ray &scattered) const override
	{
		auto scatter_direction = rec.normal + random_unit_vector();
		// Catch degenerate scatter direction
		if (scatter_direction.near_zero())
			scatter_direction = rec.normal;
		scattered = ray(rec.p, scatter_direction);
		attenuation = albedo;
		(void)r_in;
		return true;
	}

public:
	color albedo;
};

class metal : public material
{
public:
	color albedo;
	double fuzz;

public:
	metal(const color &a, double f) : albedo(a), fuzz(f < 1 ? f : 1) {}

	virtual bool scatter(
		const ray &r_in, const hit_record &rec, color &attenuation, ray &scattered) const override
	{
		vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
		scattered = ray(rec.p, reflected + fuzz * random_in_unit_sphere());
		attenuation = albedo;
		return (dot(scattered.direction(), rec.normal) > 0);
	}
};

// --- metal 까지는 이해가 되는데 굴절 부터는 물리 법칙을 사용해서 구현해야해서 솔직히 이해안됨 ---

/*
** 굴절되는 물질
** 유전체의 경우 굴절이 되는데 snell law 를 사용해서 구현
** 법칙 그대로 가져와서 쓴거임
**     Snell's law(스넬의 법칙)
**         굴절에 관한 물리법칙
**         a sinA = b sinB
**         a, b : 위, 아래 매질의 굴절계수
**         A : 법선, 광원 벡터가 이루는 각
**         B : 법선, 굴절 벡터가 이루는 각
*/
class dielectric : public material
{
public:
	double ir; // Index of Refraction
public:
	dielectric(double index_of_refraction) : ir(index_of_refraction) {}

	virtual bool scatter(
		const ray &r_in, const hit_record &rec, color &attenuation, ray &scattered) const override
	{
		// 유리 같은 Dielectric Surface 는 빛을 흡수하지 않기 때문에 1 로 설정
		attenuation = color(1.0, 1.0, 1.0);
		double refraction_ratio = rec.front_face ? (1.0 / ir) : ir;

		vec3 unit_direction = unit_vector(r_in.direction());

		double cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);
		double sin_theta = sqrt(1.0 - cos_theta * cos_theta);

		bool cannot_refract = refraction_ratio * sin_theta > 1.0;
		vec3 direction;

		if (cannot_refract)
			direction = reflect(unit_direction, rec.normal);
		else
			direction = refract(unit_direction, rec.normal, refraction_ratio);

		scattered = ray(rec.p, direction);
		return true;
	}
private:
	// schlick 근사 공식을 썼는데 솔직히 뭔소리인지 모르겠음
	static double reflectance(double cosine, double ref_idx)
	{
		// Use Schlick's approximation for reflectance.
		auto r0 = (1 - ref_idx) / (1 + ref_idx);
		r0 = r0 * r0;
		return r0 + (1 - r0) * pow((1 - cosine), 5);
	}
};
#endif