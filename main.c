#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "Libraries/VEC.h"
#include "Libraries/TRA.h"
#include "Libraries/LEC.h"

#define WIDTH  500
#define HEIGHT 500
#define FRAMES 60

#define SCALE 255
#define SKY 1e+30

int main(void) {
    int NUM_SPHERES = 1, NUM_LIGHTS = 1, NUM_CYLINDERS = 1, NUM_CONES = 1;

    SPH_T* spheres = (SPH_T *) malloc(sizeof(SPH_T) * NUM_SPHERES);
    spheres[0] = (SPH_T){VEC3_set(0,0,0),1.0f};
    if (!spheres) perror("No hay esferas");
    //printf("Spheres: x:%f y:%f z:%f r:%f n:%d \n", spheres[0].c.x, spheres[0].c.y, spheres[0].c.z, spheres[0].r,NUM_SPHERES);
    CYL_T* cylinders = (CYL_T *) malloc(sizeof(CYL_T) * NUM_CYLINDERS);
    cylinders[0] = (CYL_T){VEC3_set(-2.5,-1,0),VEC3_set(0,1,0),1.0f,2.0f};
    if (!cylinders) perror("No hay cilindros");
    CONE_T* cones = (CONE_T *) malloc(sizeof(CONE_T) * NUM_CONES);
    cones[0] = (CONE_T){VEC3_set(2.5,1,0),VEC3_set(0,-1,0),1.0f,2.0f};
    if (!cones) perror("No hay conos");

    VEC3_T origen = VEC3_set(0., 0, 12);
    VEC3_T foco = VEC3_set(0, 0, 8);
    VEC3_T techo = VEC3_set(0, 1, 0);
    // Se define el rayo que se lanzará desde el origen de la cámara
    RAY_T ray = { origen, VEC3_zero() };

    float count = 0;
    for(int t = 0; t < FRAMES ; t++){
        //printf("Frame %03d\n", t);
        char name[100];
        snprintf(name, sizeof(name), "Animations/RAY_TRACING_%03d.pgm", t);
        FILE *file = fopen(name, "w");
        if (!file) {
            perror("Error al abrir el archivo (RAY)");
            return 1;
        }
        fprintf(file, "P2\n%d %d\n%d\n", WIDTH, HEIGHT, SCALE);

        if(t!=0) {
            ray.o = VEC3_rotateY(ray.o, M_PI*2/FRAMES);
            foco = VEC3_rotateY(foco, M_PI*2/FRAMES);
        }
        VEC3_T right = VEC3_normalize(VEC3_cross( techo, ray.o));
        //printf("Right: %f %f %f \n",right.x, right.y, right.z);
        VEC3_T up =  VEC3_normalize(VEC3_cross( ray.o,right));
        //printf("Up: %f %f %f \n",up.x, up.y, up.z);

        VEC3_T l_d = VEC3_sub(foco, ray.o);
        l_d = VEC3_normalize(l_d);
        for (int i = 0; i < HEIGHT; i++) {
            for (int j = 0; j < WIDTH; j++) {
                float x = ((float)j - WIDTH/2.0f) / WIDTH;
                float y = ((float)i - HEIGHT/2.0f) / HEIGHT;
                RAY_T ray_p = ray;
                ray_p.d= VEC3_add(VEC3_scale(right,x),VEC3_scale(up,y));
                ray_p.d = VEC3_add(ray_p.d, VEC3_normalize(VEC3_scale(foco, -1)));
                ray_p.d = VEC3_normalize(ray_p.d);
                //printf("Direccion: %.2f %.2f %.2f\n", ray_p.d.x, ray_p.d.y, ray_p.d.z);

                float dst_s = SKY;
                float dst_cy = SKY;
                float dst_co = SKY;
                float diffuse = 0.0f;

                int closest_sphere_idx = 0;
                int closest_cylinder_idx = 0;
                int closest_cone_idx = 0;

                // Check spheres
                for (int k = 0; k < NUM_SPHERES; k++) {
                    float d = IntersectionSphere(ray_p, spheres[k]);
                    //printf("d: %f\n", d);
                    if (d < dst_s) {
                        dst_s = d;
                        closest_sphere_idx = k;
                        //printf("Closest sphere: %f",d);
                    }
                }

                // Check cylinders
                for (int k = 0; k < NUM_CYLINDERS; k++) {
                    float d = IntersectionCylinder(ray_p, cylinders[k]);
                    //printf("d: %f\n", d);
                    if (d < dst_cy) {
                        dst_cy = d;
                        closest_cylinder_idx = k;
                        //printf("Closest cylinder: %f",d);
                    }
                }

                // Check cones
                for (int k = 0; k < NUM_CONES; k++) {
                    float d = IntersectionCone(ray_p, cones[k]);
                    //printf("d: %f\n", d);
                    if (d < dst_co) {
                        dst_co = d;
                        closest_cone_idx = k;
                        //printf("Closest cone: %f",d);
                    }
                }

                // Choose closest object
                if ((dst_s < dst_cy) && (dst_s < dst_co)) {
                    //printf("Difuse sphere");
                    diffuse = ShadingSphere(dst_s, closest_sphere_idx, ray_p, spheres[closest_sphere_idx], l_d, spheres, NUM_SPHERES, cylinders, NUM_CYLINDERS, cones, NUM_CONES);
                } else if ((dst_cy < dst_s) && (dst_cy < dst_co)) {
                    //printf("Difuse cylinder");
                    diffuse = ShadingCylinder(dst_cy, closest_cylinder_idx, ray_p, cylinders[closest_cylinder_idx], l_d, spheres, NUM_SPHERES, cylinders, NUM_CYLINDERS, cones, NUM_CONES);
                } else if ((dst_co < dst_s )&&( dst_co < dst_cy)) {
                    //printf("Difuse cone");
                    diffuse = ShadingCone(dst_co, closest_cone_idx, ray_p, cones[closest_cone_idx], l_d, spheres, NUM_SPHERES, cylinders, NUM_CYLINDERS, cones, NUM_CONES);
                }

                if (diffuse > 1.0f) diffuse = 1.0f;
                unsigned char u = (unsigned char)(diffuse * 255);
                fprintf(file, "%d ", u);
                count++;
                printf("\rProgreso: %.2f%% ", count/(FRAMES*WIDTH*HEIGHT) * 100);
                fflush(stdout);
            }
            fprintf(file, "\n");
        }        fclose(file);
    }
    return 0;
}