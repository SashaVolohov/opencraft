#ifndef CAMERA_H_INCLUDED
#define CAMERA_H_INCLUDED

#include <windows.h>

int world[32][32][16][16][256];
POINT scrSize;
float scrKoef;

struct SCamera{
    float x,y,z;
    float Xrot, Zrot;
} camera;

typedef struct {
    float x, y, z;
    float Xrot, Zrot;
    int entity_id;
    float Zrotplus;
    float hand_left, hand_right, leg_left, leg_right;
    float hand_left_c, hand_right_c, leg_left_c, leg_right_c;
    float hand_left_z, hand_right_z, leg_left_z, leg_right_z;
    int anim_step;
    int is_jumping;
    int jump_tmp;
    float z_in_jump;
    int jump_down;
} entity;

typedef struct {
    float x[10], y[10], z[10];
    float block_id;
    int step;
} sprite;

entity Entities[500];
sprite Sprites[100];

void Camera_Apply();
void Camera_Rotation(float xAngle, float zAngle);
void Camera_AutoMoveByMouse(int centerX, int centerY, float speed);
void Camera_MoveDirection(int forwardMove, int rightMove, float speed);
void GenerateNewWorld();
void EntityAI(int j);
void SaveWorld();
int GetBlockID(int x, int y, int z);

#endif // CAMERA_H_INCLUDED
