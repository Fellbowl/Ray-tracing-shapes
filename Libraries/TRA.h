//
// Created by Juan Pablo on 4/4/2025.
//

#ifndef PROYECTO1_TRA_H
#define PROYECTO1_TRA_H

#include "VEC.h"

typedef struct {
    VEC3_T o, d;
} RAY_T;

typedef struct {
    VEC3_T c;
    float r;
} SPH_T;

typedef struct {
    VEC3_T c;
    VEC3_T n;
    float r;
    float h;
} CYL_T;

typedef struct {
    VEC3_T c;
    VEC3_T n;
    float r;
    float h;
}CONE_T;


float ShadingSphere(float a, int b, RAY_T c, SPH_T d, VEC3_T e, SPH_T g[], int h, CYL_T i[], int j, CONE_T k[], int l);
float ShadingCylinder(float a, int b, RAY_T c, CYL_T d, VEC3_T e, SPH_T g[], int h, CYL_T i[], int j, CONE_T k[], int l);
float ShadingCone(float a, int b, RAY_T c, CONE_T d, VEC3_T e, SPH_T g[], int h, CYL_T i[], int j, CONE_T k[], int l);
float IntersectionSphere(RAY_T a, SPH_T b);
float IntersectionCylinder(RAY_T a, CYL_T b);
float IntersectionCone(RAY_T a, CONE_T b);

#endif //PROYECTO1_TRA_H
