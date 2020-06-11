#include "color.h"
#include "vec3.h"
#include "ray.h"

#include <iostream>

using namespace std;

double hit_sphere(const point3& center, double radius, const ray& r) {
    vec3 oc = r.origin() - center;
    double a = dot(r.direction(), r.direction());
    double half_b = dot(oc,r.direction());
    double c = dot(oc,oc) - radius*radius;
    double disc = half_b*half_b - a*c;

    if(disc < 0) {
        return -1.0;
    } else {
        return (-half_b - sqrt(disc)) / a;
    }
}

color ray_color(const ray& r) {
    point3 sphere_center(0,0,-1);
    double t = hit_sphere(sphere_center, 0.5, r);
    if (t > 0.0) {
        vec3 N = unit_vector(r.at(t) - sphere_center);
        return 0.5*color(N.x()+1, N.y()+1, N.z()+1);
    }

    vec3 unit_dir = unit_vector(r.direction());
    t = 0.5*(unit_dir.y() + 1.0);
    return (1.0-t)*color(1.0,1.0,1.0) + t*color(0.5,0.7,1.0);
}

int main(int argc, char **argv) {
    const auto aspect_ratio = 16.0/9.0;
    const int img_width = 3820;
    const int img_height = static_cast<int>(img_width / aspect_ratio);

    cout << "P3\n" << img_width << ' ' << img_height << "\n255\n";

    auto viewport_height = 2.0;
    auto viewport_width = aspect_ratio * viewport_height;
    auto focal_length = 1.0;

    auto origin = point3(0,0,0);
    auto horizontal = vec3(viewport_width,0,0);
    auto vertical = vec3(0,viewport_height,0);
    auto lower_left_corner = origin - (horizontal/2) - (vertical/2) - vec3(0,0,focal_length);

    for (int r=img_height-1; r>=0; --r) {
        cerr << "\rScanlines remaining: " << r << ' ' << std::flush;
        for (int c=0; c<img_width; ++c) {
            double u = double(c) / (img_width-1);
            double v = double(r) / (img_height-1);
            ray R(origin,lower_left_corner + u*horizontal + v*vertical - origin);
            color pixel_color = ray_color(R);
            write_color(std::cout, pixel_color);
        }
    }

    cerr << "\nDone.\n";

    return EXIT_SUCCESS;
}
