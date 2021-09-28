#ifndef CAMERA_H
#define CAMERA_H

#include "rtweekend.h"

class camera
{

private:
    point3  origin;
    point3  lower_left_corner;  // 시작점
    vec3    horizontal;         // viewport 가로 길이를 벡터단위로 표현
    vec3    vertical;           // viewport 세로 길이를 벡터단위로 표현
    vec3    u, v, w;
    double  lens_radius;

public:
    // vfov : vertiacal fov
    // aspect_ratio : 종횡비
    camera(
        point3  lookfrom,       // camera 위치
        point3  lookat,         // camera 렌즈가 바라보는 방향
        vec3    vup,            // 카메라를 설치하는 방향 (행성이 자전하는 것처럼)
        double  vfov,           // vertical fov
        double  aspect_ratio,    // 종횡비
        double aperture,
        double focus_dist
        )
    {
        auto theta = degrees_to_radians(vfov);
        auto h = tan(theta / 2);                    // fov 를 사용해서 세로 거리를 구할 수있음

        // viewport 비율
        auto viewport_height = 2.0 * h;
        auto viewport_width = aspect_ratio * viewport_height;

        // orthonormal basis (x, y, z 축 처럼)
        w = unit_vector(lookfrom - lookat);
        u = unit_vector(cross(vup, w));
        v = cross(w, u);

        origin = lookfrom;
        horizontal = viewport_width * u * focus_dist;
        vertical = viewport_height * v * focus_dist;
        lower_left_corner = origin - horizontal / 2 - vertical / 2 - w * focus_dist;

        lens_radius = aperture / 2;
    }

/*
**    (t)
**     ^
**     |
**     |
**     Lower_left_corner  =====>(s)
*/
    ray get_ray(double s, double t) const
    {
        // P(t) =  o + tP (0 <= t <= 1)
        vec3 rd = lens_radius * random_in_unit_disk();
        vec3 offset = u * rd.x() + v * rd.y();

        return ray(
            origin + offset,
            lower_left_corner + s * horizontal + t * vertical - origin - offset);
    }
};
#endif