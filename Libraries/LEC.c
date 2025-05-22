//
// Created by Juan Pablo on 4/5/2025.
//

#include <stdio.h>
#include <stdlib.h>
#include "LEC.h"



SPH_T* LlenarSpheres(const char* nombreArchivo, int* num_spheres){
    FILE *archivo = fopen(nombreArchivo, "r");
    if (archivo == NULL) {
        perror("Error al abrir el archivo spheres");
        *num_spheres = 0;
        return NULL;
    }
    if (fscanf(archivo, "%d", num_spheres) != 1 || *num_spheres <= 0) {
        fclose(archivo);
        *num_spheres = 0;
        return NULL;
    }
    SPH_T* spheres = (SPH_T*)malloc(*num_spheres * sizeof(SPH_T));
    if (!spheres) {
        perror("Error al asignar memoria para spheres");
        fclose(archivo);
        *num_spheres = 0;
        return NULL;
    }
    for (int i = 0; i < *num_spheres ; i++) {
        if (fscanf(archivo, "%f %f %f %f", &spheres[i].c.x, &spheres[i].c.y, &spheres[i].c.z, &spheres[i].r) != 4) {
            printf("Error al leer los datos de la línea %d\n", i + 1);
            free(spheres);
            fclose(archivo);
            *num_spheres = 0;
            return NULL;
        }
        printf("%f %f %f %f", &spheres[i].c.x, &spheres[i].c.y, &spheres[i].c.z, &spheres[i].r);
    }

    fclose(archivo);
    return spheres;
}

CYL_T* LlenarCylinders(const char* nombreArchivo, int* num_cylinders) {
    FILE *archivo = fopen(nombreArchivo, "r");
    if (archivo == NULL) {
        perror("Error al abrir el archivo cylinders");
        *num_cylinders = 0;
        return NULL;
    }

    if (fscanf(archivo, "%d", num_cylinders) != 1 || *num_cylinders <= 0) {
        fclose(archivo);
        *num_cylinders = 0;
        return NULL;
    }

    CYL_T* cylinders = (CYL_T*)malloc(*num_cylinders * sizeof(CYL_T));
    if (!cylinders) {
        perror("Error al asignar memoria para cylinders");
        fclose(archivo);
        *num_cylinders = 0;
        return NULL;
    }

    for (int i = 0; i < *num_cylinders; i++) {
        if (fscanf(archivo, "%f %f %f %f %f %f %f %f",
                   &cylinders[i].c.x, &cylinders[i].c.y, &cylinders[i].c.z,
                   &cylinders[i].n.x, &cylinders[i].n.y, &cylinders[i].n.z,
                   &cylinders[i].r, &cylinders[i].h) != 8) {
            printf("Error al leer los datos de la línea %d\n", i + 1);
            free(cylinders);
            fclose(archivo);
            *num_cylinders = 0;
            return NULL;
        }
    }

    fclose(archivo);
    return cylinders;
}

CONE_T* LlenarCones(const char* nombreArchivo, int* num_cones) {
    FILE *archivo = fopen(nombreArchivo, "r");
    if (archivo == NULL) {
        perror("Error al abrir el archivo cones");
        *num_cones = 0;
        return NULL;
    }

    if (fscanf(archivo, "%d", num_cones) != 1 || *num_cones <= 0) {
        fclose(archivo);
        *num_cones = 0;
        return NULL;
    }

    CONE_T* cones = (CONE_T*)malloc(*num_cones * sizeof(CONE_T));
    if (!cones) {
        perror("Error al asignar memoria para cones");
        fclose(archivo);
        *num_cones = 0;
        return NULL;
    }

    for (int i = 0; i < *num_cones; i++) {
        if (fscanf(archivo, "%f %f %f %f %f %f %f %f",
                   &cones[i].c.x, &cones[i].c.y, &cones[i].c.z,
                   &cones[i].n.x, &cones[i].n.y, &cones[i].n.z,
                   &cones[i].r, &cones[i].h) != 8) {
            printf("Error al leer los datos de la línea %d\n", i + 1);
            free(cones);
            fclose(archivo);
            *num_cones = 0;
            return NULL;
        }
    }

    fclose(archivo);
    return cones;
}

