#include <math.h>
#include "TRA.h"

#include <stdio.h>

#define SKY 1e+30
#define EPSILON 1e-4


float ShadingSphere(float dst, int closest_sphere_idx, RAY_T ray, SPH_T sphere, VEC3_T light, SPH_T spheres[], int num_spheres, CYL_T cylinders[], int num_cylinders, CONE_T cones[], int num_cones) {
  if (dst == SKY)
    return 0.0;
  VEC3_T intersection = VEC3_add(ray.o, VEC3_scale(ray.d, dst));
  VEC3_T normal = VEC3_normalize(VEC3_sub(intersection, sphere.c));

  float diffuse = VEC3_dot(normal, light);
  diffuse = (diffuse < 0.0) ? -diffuse : diffuse;
  VEC3_T offset = VEC3_scale(VEC3_normalize(light), EPSILON);
  RAY_T shadow_ray = { VEC3_add(intersection, offset), VEC3_scale(VEC3_normalize(light), -1.0f) };
  for (int i = 0; i < num_spheres; i++) {
    if (i != closest_sphere_idx && IntersectionSphere(shadow_ray, spheres[i]) < SKY) {
      diffuse = 0.0;
      break;
    }
  }
  for (int i = 0; i < num_cylinders; i++) {
    if (IntersectionCylinder(shadow_ray, cylinders[i]) < SKY) {
      diffuse = 0.0;
      break;
    }
  }
  for (int i = 0; i < num_cones; i++) {
    if (IntersectionCone(shadow_ray, cones[i]) < SKY) {
      diffuse = 0.0;
      break;
    }
  }
  return diffuse;
}

float IntersectionSphere(RAY_T ray, SPH_T sph){
  ray.d = VEC3_normalize(ray.d);
  //printf("Ray: %f %f %f\n", ray.d.x, ray.d.y, ray.d.z);
  float dst, dv, disc, t1, t2;
  VEC3_T vco = sph.c;
  vco = VEC3_sub(vco,ray.o);
  dv = VEC3_dot(ray.d,vco);
  disc = dv*dv - VEC3_dot(vco,vco) + sph.r*sph.r;
  if(disc < 0.0)
    return SKY;
  else{
    dst = sqrt( disc );
    t2 = dv+dst;
    t1 = dv-dst;
    dst =  t2<0.0 ? SKY : ( t1>0.0 ? t1 : t2 );
  }
  //printf("dst: %f\n", dst);
  return dst;
}

float IntersectionCylinder(RAY_T ray, CYL_T cyl){
  VEC3_T co = VEC3_sub(ray.o, cyl.c);
  float dn = VEC3_dot(ray.d, cyl.n);
  float con = VEC3_dot(co, cyl.n);

  // Components perpendicular to the axis
  VEC3_T d_perp = VEC3_sub(ray.d, VEC3_scale(cyl.n, dn));
  VEC3_T co_perp = VEC3_sub(co, VEC3_scale(cyl.n, con));

  float a = VEC3_dot(d_perp, d_perp);
  float b = 2.0f * VEC3_dot(d_perp, co_perp);
  float c = VEC3_dot(co_perp, co_perp) - cyl.r * cyl.r;

  float disc = b * b - 4 * a * c;
  if (disc < 0.0f || fabsf(a) < 1e-6f) return SKY;

  float sqrt_disc = sqrtf(disc);
  float t1 = (-b - sqrt_disc) / (2 * a);
  float t2 = (-b + sqrt_disc) / (2 * a);

  // Choose nearest valid t
  float t = (t1 > 0.0f) ? t1 : ((t2 > 0.0f) ? t2 : SKY);
  if (t == SKY) return SKY;

  VEC3_T p = VEC3_add(ray.o, VEC3_scale(ray.d, t));
  float proj = VEC3_dot(VEC3_sub(p, cyl.c), cyl.n);

  if (proj < 0.0f || proj > cyl.h) return SKY;  // Outside height
  //printf("t: %f\n", t);
  return t;
}

float IntersectionCone(RAY_T ray, CONE_T cone) {
  VEC3_T apex = VEC3_add(cone.c, VEC3_scale(cone.n, cone.h));
  VEC3_T co = VEC3_sub(ray.o, apex);

  float cos2 = (cone.r / cone.h) * (cone.r / cone.h); // tan²(θ)

  float dn = VEC3_dot(ray.d, cone.n);
  float con = VEC3_dot(co, cone.n);

  VEC3_T d_perp = VEC3_sub(ray.d, VEC3_scale(cone.n, dn));
  VEC3_T co_perp = VEC3_sub(co, VEC3_scale(cone.n, con));

  float a = VEC3_dot(d_perp, d_perp) - cos2 * dn * dn;
  float b = 2.0f * (VEC3_dot(d_perp, co_perp) - cos2 * dn * con);
  float c = VEC3_dot(co_perp, co_perp) - cos2 * con * con;

  float disc = b * b - 4 * a * c;
  if (disc < 0.0f || fabsf(a) < 1e-6f) return SKY;

  float sqrt_disc = sqrtf(disc);
  float t1 = (-b - sqrt_disc) / (2 * a);
  float t2 = (-b + sqrt_disc) / (2 * a);

  float t = (t1 > 0.0f) ? t1 : ((t2 > 0.0f) ? t2 : SKY);
  if (t == SKY) return SKY;

  VEC3_T p = VEC3_add(ray.o, VEC3_scale(ray.d, t));
  float height = VEC3_dot(VEC3_sub(p, cone.c), cone.n);

  if (height < 0.0f || height > cone.h) return SKY;
  //printf("t: %f\n", t);
  return t;
}

float ShadingCylinder(float dst, int index, RAY_T ray, CYL_T cylinder, VEC3_T light, SPH_T spheres[], int num_spheres, CYL_T cylinders[], int num_cylinders, CONE_T cones[], int num_cones)
{
  if (dst >= SKY)
    return 0.0;

  VEC3_T intersection = VEC3_add(ray.o, VEC3_scale(ray.d, dst));
  VEC3_T v = VEC3_sub(intersection, cylinder.c);
  float proj = VEC3_dot(v, cylinder.n);
  VEC3_T proj_vec = VEC3_scale(cylinder.n, proj);
  VEC3_T normal = VEC3_normalize(VEC3_sub(v, proj_vec));

  float diffuse = VEC3_dot(normal, light);
  diffuse = (diffuse < 0.0f) ? -diffuse : diffuse;

  VEC3_T offset = VEC3_scale(VEC3_normalize(light), EPSILON);
  RAY_T shadow_ray = { VEC3_add(intersection, offset), VEC3_scale(VEC3_normalize(light), -1.0f) };

  for (int i = 0; i < num_spheres; i++) {
    if (IntersectionSphere(shadow_ray, spheres[i]) < SKY) {
      diffuse = 0.0f;
      break;
    }
  }
  for (int i = 0; i < num_cylinders; i++) {
    if (i != index && IntersectionCylinder(shadow_ray, cylinders[i]) < SKY) {
      diffuse = 0.0f;
      break;
    }
  }
  for (int i = 0; i < num_cones; i++) {
    if (IntersectionCone(shadow_ray, cones[i]) < SKY) {
      diffuse = 0.0f;
      break;
    }
  }

  return diffuse;
}

float ShadingCone(float dst, int index, RAY_T ray, CONE_T cone, VEC3_T light, SPH_T spheres[], int num_spheres, CYL_T cylinders[], int num_cylinders, CONE_T cones[], int num_cones)
{
  if (dst >= SKY)
    return 0.0;

  VEC3_T apex = VEC3_add(cone.c, VEC3_scale(cone.n, cone.h));
  VEC3_T intersection = VEC3_add(ray.o, VEC3_scale(ray.d, dst));
  VEC3_T v = VEC3_sub(intersection, apex);
  float proj = VEC3_dot(v, cone.n);
  VEC3_T axis_comp = VEC3_scale(cone.n, proj);
  VEC3_T normal = VEC3_normalize(VEC3_sub(v, axis_comp));

  float diffuse = VEC3_dot(normal, light);
  diffuse = (diffuse < 0.0f) ? -diffuse : diffuse;

  VEC3_T offset = VEC3_scale(VEC3_normalize(light), EPSILON);
  RAY_T shadow_ray = { VEC3_add(intersection, offset), VEC3_scale(VEC3_normalize(light), -1.0f) };

  for (int i = 0; i < num_spheres; i++) {
    if (IntersectionSphere(shadow_ray, spheres[i]) < SKY) {
      diffuse = 0.0f;
      break;
    }
  }
  for (int i = 0; i < num_cylinders; i++) {
    if (IntersectionCylinder(shadow_ray, cylinders[i]) < SKY) {
      diffuse = 0.0f;
      break;
    }
  }
  for (int i = 0; i < num_cones; i++) {
    if (i != index && IntersectionCone(shadow_ray, cones[i]) < SKY) {
      diffuse = 0.0f;
      break;
    }
  }

  return diffuse;
}

