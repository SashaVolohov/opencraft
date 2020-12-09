#ifndef CAMERA_H_INCLUDED
#define CAMERA_H_INCLUDED

int world[16][16][16][16][256];

struct SCamera{
    float x,y,z;
    float Xrot, Zrot;
} camera;

void Camera_Apply();
void Camera_Rotation(float xAngle, float zAngle);
void Camera_AutoMoveByMouse(int centerX, int centerY, float speed);
void Camera_MoveDirection(int forwardMove, int rightMove, float speed);

#endif // CAMERA_H_INCLUDED
