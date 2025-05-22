//
// Created by Juan Pablo on 4/4/2025.
//
#include <stdio.h>
#include <math.h>
#include "VEC.h"

VEC3_T VEC3_add(VEC3_T a, VEC3_T b){
    return (VEC3_T){a.x + b.x, a.y + b.y, a.z + b.z};
}

VEC3_T VEC3_sub(VEC3_T a, VEC3_T b){
    return (VEC3_T){a.x - b.x, a.y - b.y, a.z - b.z};
}

float VEC3_dot(VEC3_T a, VEC3_T b){
    return a.x*b.x + a.y*b.y + a.z*b.z;
}

VEC3_T VEC3_cross(VEC3_T a, VEC3_T b){
    return (VEC3_T){a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z,a.x*b.y - a.y*b.x};
}

float VEC3_lenght(VEC3_T a){
    return sqrt(VEC3_dot(a,a));
}

VEC3_T VEC3_normalize(VEC3_T a){
    float len = VEC3_lenght(a);
    return (len > 0) ? VEC3_scale(a, 1/len) : VEC3_zero();
}

VEC3_T VEC3_scale(VEC3_T a, float b){
    return (VEC3_T){a.x*b, a.y*b, a.z*b};
}

VEC3_T VEC3_zero(){
    return (VEC3_T){0,0,0};
}

VEC3_T VEC3_set(float x, float y, float z){
    return (VEC3_T){x,y,z};
}

float VEC3_get_x(VEC3_T a){
    return a.x;
}

float VEC3_get_y(VEC3_T a){
    return a.y;
}

float VEC3_get_z(VEC3_T a){
    return a.z;
}

VEC3_T VEC3_rotateX(VEC3_T a, float b) {
    // Paso 2: rotación alrededor del eje X
    float cosb = cosf(b);
    float sinb = sinf(b);
    float ry = a.y * cosb - a.z * sinb;
    float rz = a.y * sinb + a.z * cosb;
    float rx = a.x; // X no cambia en rotación sobre eje X

    VEC3_T result = (VEC3_T) {rx, ry, rz};

    return result;
}


VEC3_T VEC3_rotateY(VEC3_T a, float b) {
    // Paso 2: rotar alrededor del eje Y
    float cosb = cosf(b);
    float sinb = sinf(b);
    float rx = a.x * cosb + a.z * sinb;
    float rz = -a.x * sinb + a.z * cosb;
    float ry = a.y; // Y no cambia en rotación alrededor de Y

    // Paso 3: re-trasladar al punto original
    VEC3_T result = (VEC3_T) {rx, ry, rz};

    return result;
}

VEC3_T VEC3_rotateZ(VEC3_T a, float b) {
    // Paso 2: rotación alrededor del eje Z
    float cosb = cosf(b);
    float sinb = sinf(b);
    float rx = a.x * cosb - a.y * sinb;
    float ry = a.x * sinb + a.y * cosb;
    float rz = a.z; // Z no cambia en rotación sobre eje Z

    // Paso 3: trasladar de vuelta
    VEC3_T result = (VEC3_T) {rx, ry, rz};

    return result;
}

void VEC3_print(VEC3_T a){
    printf("(%f, %f, %f)\n", a.x, a.y, a.z);
}