#include <GL/gl.h>

#include "camera.h"
#include <windows.h>
#include <math.h>

struct SCamera camera = {128,128,65,0,0};

void Camera_Apply()
{
    glRotatef(-camera.Xrot, 1,0,0);
    glRotatef(-camera.Zrot, 0,0,1);
    float z = -camera.z - 1.7;
    glTranslatef(-camera.x, -camera.y, z);
}

void Camera_Rotation(float xAngle, float zAngle)
{
    camera.Zrot += zAngle;
    if(camera.Zrot < 0) camera.Zrot += 360;
    if(camera.Zrot > 360) camera.Zrot -= 360;
    camera.Xrot += xAngle;
    if(camera.Xrot < 0) camera.Xrot = 0;
    if(camera.Xrot > 180) camera.Xrot = 180;
}

void Camera_AutoMoveByMouse(int centerX, int centerY, float speed)
{
    POINT cur;
    POINT base = {centerX, centerY};
    GetCursorPos(&cur);
    Camera_Rotation((base.y - cur.y) * speed, (base.x - cur.x) * speed);
    SetCursorPos(base.x, base.y);
}

void Camera_MoveDirection(int forwardMove, int rightMove, float speed)
{
    float ugol = -camera.Zrot / 180 * M_PI;

    if(forwardMove > 0) ugol += rightMove > 0 ? M_PI_4 : (rightMove < 0 ? -M_PI_4 : 0);
    if(forwardMove < 0) ugol += M_PI + (rightMove > 0 ? -M_PI_4 : (rightMove < 0 ? M_PI_4 : 0));
    if(forwardMove == 0)
    {
        ugol += rightMove > 0 ? M_PI_2 : -M_PI_2;
        if(rightMove == 0) speed = 0;
    }
    if(speed != 0)
    {
        float new_cx = camera.x + (sin(ugol) * speed);
        float new_cy = camera.y + (cos(ugol) * speed);
        int chunkx = new_cx / 16;
        int chunky = new_cy / 16;
        int dcx = 16*chunkx;
        int dcy = 16*chunky;
        float x = new_cx - dcx;
        float y = new_cy - dcy;
        if(new_cx < 0 || new_cy < 0 || new_cx >= 256 || new_cy >= 256 || camera.z <= -1 || camera.z >= 256)
        {
            camera.x = new_cx;
            camera.y = new_cy;
            if(new_cx <= 256.3 && new_cx >= 256 && camera.z < 45) camera.x = 256.3;
            if(new_cy <= 256.3 && new_cy >= 256 && camera.z < 45) camera.y = 256.3;
            if(new_cx >= -0.3 && new_cx < 0 && camera.z < 45) camera.x = -0.4;
            if(new_cy >= -0.3 && new_cy < 0 && camera.z < 45) camera.y = -0.4;
        }
        else if(world[chunkx][chunky][(int)x][(int)y][(int)camera.z] == 0 && world[chunkx][chunky][(int)x][(int)y][(int)camera.z+1] == 0)
        {
            float ccx = (int)new_cx;
            float ccy = (int)new_cy;
            if(new_cx < camera.x && GetBlockID((int)ccx - 1, (int)new_cy, (int)camera.z) != 0)
            {
                if(new_cx <= ccx + 0.3) new_cx = ccx + 0.3;
            }
            if(new_cx > camera.x && GetBlockID((int)ccx + 1, (int)new_cy, (int)camera.z) != 0)
            {
                if(new_cx >= ccx - 0.3) new_cx = ccx + 0.3;
            }
            if(new_cy < camera.y && GetBlockID((int)ccx, (int)new_cy - 1, (int)camera.z) != 0)
            {
                if(new_cy <= ccy + 0.3) new_cy = ccy + 0.3;
            }
            if(new_cy > camera.y && GetBlockID((int)ccx, (int)new_cy + 1, (int)camera.z) != 0)
            {
                if(new_cy >= ccy + 0.3) new_cy = ccy + 0.3;
            }

            if(new_cx < camera.x && GetBlockID((int)ccx - 1, (int)new_cy, (int)camera.z+1) != 0)
            {
                if(new_cx <= ccx + 0.3) new_cx = ccx + 0.3;
            }
            if(new_cx > camera.x && GetBlockID((int)ccx + 1, (int)new_cy, (int)camera.z+1) != 0)
            {
                if(new_cx >= ccx - 0.3) new_cx = ccx + 0.3;
            }
            if(new_cy < camera.y && GetBlockID((int)ccx, (int)new_cy - 1, (int)camera.z+1) != 0)
            {
                if(new_cy <= ccy + 0.3) new_cy = ccy + 0.3;
            }
            if(new_cy > camera.y && GetBlockID((int)ccx, (int)new_cy + 1, (int)camera.z+1) != 0)
            {
                if(new_cy >= ccy + 0.3) new_cy = ccy + 0.3;
            }
            camera.x = new_cx;
            camera.y = new_cy;
        }
    }
}
