//
// Created by Juan Pablo on 4/4/2025.
//

#ifndef PROYECTO1_VEC_H
#define PROYECTO1_VEC_H

typedef struct {
    float x,y,z;
} VEC3_T;

VEC3_T VEC3_add(VEC3_T a, VEC3_T b);
VEC3_T VEC3_sub(VEC3_T a, VEC3_T b);
float VEC3_dot(VEC3_T a, VEC3_T b);
VEC3_T VEC3_cross(VEC3_T a, VEC3_T b);
VEC3_T VEC3_normalize(VEC3_T a);
VEC3_T VEC3_scale(VEC3_T a, float b);
VEC3_T VEC3_zero();
float VEC3_lenght(VEC3_T a);
VEC3_T VEC3_set(float x, float y, float z);
float VEC3_get_x(VEC3_T a);
float VEC3_get_y(VEC3_T a);
float VEC3_get_z(VEC3_T a);
VEC3_T VEC3_rotateX(VEC3_T a, float b);
VEC3_T VEC3_rotateY(VEC3_T a, float b);
VEC3_T VEC3_rotateZ(VEC3_T a, float b);
void VEC3_print(VEC3_T a);

#endif //PROYECTO1_VEC_H



