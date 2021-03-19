#include <GL/gl.h>

#include "camera.h"
#include "blocks.h"
#include <windows.h>
#include <math.h>
#include "../../libs/bass/bass.h"

BOOL inverted_y = FALSE;
HSAMPLE sampstep;
HCHANNEL chstep;
BOOL is_x;

RECT rcta;
RECT rctb;

struct SCamera camera = {128.5,128.5,65,0,0};

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
    if(inverted_y == FALSE) camera.Xrot += xAngle;
    else camera.Xrot -= xAngle;
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
        float ccx = (int)new_cx;
        float ccy = (int)new_cy;
        if(GetBlockID((int)ccx - 1, (int)new_cy, (int)camera.z) != 6 && GetBlockID((int)ccx - 1, (int)new_cy, (int)camera.z) != 7 && GetBlockID((int)ccx - 1, (int)new_cy, (int)camera.z) != 8 && blocks[GetBlockID((int)ccx - 1, (int)new_cy, (int)camera.z)].type != 3)
        {
            if(new_cx < camera.x && GetBlockID((int)ccx - 1, (int)new_cy, (int)camera.z) != 0)
            {
                if(new_cx <= ccx + 0.3) new_cx = ccx + 0.3;
            }
        }
        if(GetBlockID((int)ccx + 1, (int)new_cy, (int)camera.z) != 6 && GetBlockID((int)ccx + 1, (int)new_cy, (int)camera.z) != 7 && GetBlockID((int)ccx + 1, (int)new_cy, (int)camera.z) != 8 && blocks[GetBlockID((int)ccx + 1, (int)new_cy, (int)camera.z)].type != 3)
        {
            if(new_cx > camera.x && GetBlockID((int)ccx + 1, (int)new_cy, (int)camera.z) != 0)
            {
                if(new_cx >= ccx - 0.3) new_cx = ccx + 0.7;
            }
        }
        if(GetBlockID((int)new_cx, (int)ccy - 1, (int)camera.z) != 6 && GetBlockID((int)new_cx, (int)ccy - 1, (int)camera.z) != 7 && GetBlockID((int)new_cx, (int)ccy - 1, (int)camera.z) != 8 && blocks[GetBlockID((int)ccx, (int)new_cy - 1, (int)camera.z)].type != 3)
        {
            if(new_cy < camera.y && GetBlockID((int)ccx, (int)new_cy - 1, (int)camera.z) != 0)
            {
                if(new_cy <= ccy + 0.3) new_cy = ccy + 0.3;
            }
        }
        if(GetBlockID((int)new_cx, (int)ccy + 1, (int)camera.z) != 6 && GetBlockID((int)new_cx, (int)ccy + 1, (int)camera.z) != 7 && GetBlockID((int)new_cx, (int)ccy + 1, (int)camera.z) != 8 && blocks[GetBlockID((int)ccx, (int)new_cy + 1, (int)camera.z)].type != 3)
        {
            if(new_cy > camera.y && GetBlockID((int)ccx, (int)new_cy + 1, (int)camera.z) != 0)
            {
                if(new_cy >= ccy + 0.3) new_cy = ccy + 0.7;
            }
        }

        if(GetBlockID((int)ccx - 1, (int)new_cy, (int)camera.z+1) != 6 && GetBlockID((int)ccx - 1, (int)new_cy, (int)camera.z+1) != 7 && GetBlockID((int)ccx - 1, (int)new_cy, (int)camera.z+1) != 8 && blocks[GetBlockID((int)ccx - 1, (int)new_cy, (int)camera.z+1)].type != 3)
        {
            if(new_cx < camera.x && GetBlockID((int)ccx - 1, (int)new_cy, (int)camera.z+1) != 0)
            {
                if(new_cx <= ccx + 0.3) new_cx = ccx + 0.3;
            }
        }
        if(GetBlockID((int)ccx + 1, (int)new_cy, (int)camera.z+1) != 6 && GetBlockID((int)ccx + 1, (int)new_cy, (int)camera.z+1) != 7 && GetBlockID((int)ccx + 1, (int)new_cy, (int)camera.z+1) != 8 && blocks[GetBlockID((int)ccx + 1, (int)new_cy, (int)camera.z+1)].type != 3)
        {
            if(new_cx > camera.x && GetBlockID((int)ccx + 1, (int)new_cy, (int)camera.z+1) != 0)
            {
                if(new_cx >= ccx - 0.3) new_cx = ccx + 0.7;
            }
        }
        if(GetBlockID((int)new_cx, (int)ccy - 1, (int)camera.z+1) != 6 && GetBlockID((int)new_cx, (int)ccy - 1, (int)camera.z+1) != 7 && GetBlockID((int)new_cx, (int)ccy - 1, (int)camera.z+1) != 8 && blocks[GetBlockID((int)ccx, (int)new_cy - 1, (int)camera.z+1)].type != 3)
        {
            if(new_cy < camera.y && GetBlockID((int)ccx, (int)new_cy - 1, (int)camera.z+1) != 0)
            {
                if(new_cy <= ccy + 0.3) new_cy = ccy + 0.3;
            }
        }
        if(GetBlockID((int)new_cx, (int)ccy + 1, (int)camera.z+1) != 6 && GetBlockID((int)new_cx, (int)ccy + 1, (int)camera.z+1) != 7 && GetBlockID((int)new_cx, (int)ccy + 1, (int)camera.z+1) != 8 && blocks[GetBlockID((int)ccx, (int)new_cy + 1, (int)camera.z+1)].type != 3)
        {
            if(new_cy > camera.y && GetBlockID((int)ccx, (int)new_cy + 1, (int)camera.z+1) != 0)
            {
                if(new_cy >= ccy + 0.3) new_cy = ccy + 0.7;
            }
        }
        if(GetBlockID((int)new_cx, (int)new_cy, camera.z-1) >= 19 && GetBlockID((int)new_cx, (int)new_cy, camera.z-1) <= 34)
        {
            if(!sampstep || BASS_ChannelIsActive(chstep) == BASS_ACTIVE_STOPPED)
            {
                char filename[] = "sounds\\step\\cloth.ogg";
                sampstep = BASS_SampleLoad(FALSE, filename, 0, 0, 1, BASS_SAMPLE_MONO);
                chstep = BASS_SampleGetChannel(sampstep, FALSE);
                BASS_ChannelPlay(chstep, FALSE);
            }
        }
        if(GetBlockID((int)new_cx, (int)new_cy, camera.z-1) == 18)
        {
            if(!sampstep || BASS_ChannelIsActive(chstep) == BASS_ACTIVE_STOPPED)
            {
                char filename[] = "sounds\\step\\glass.ogg";
                sampstep = BASS_SampleLoad(FALSE, filename, 0, 0, 1, BASS_SAMPLE_MONO);
                chstep = BASS_SampleGetChannel(sampstep, FALSE);
                BASS_ChannelPlay(chstep, FALSE);
            }
        }
        if(GetBlockID((int)new_cx, (int)new_cy, camera.z-1) == 2)
        {
            if(!sampstep || BASS_ChannelIsActive(chstep) == BASS_ACTIVE_STOPPED)
            {
                char filename[] = "sounds\\step\\grass.ogg";
                sampstep = BASS_SampleLoad(FALSE, filename, 0, 0, 1, BASS_SAMPLE_MONO);
                chstep = BASS_SampleGetChannel(sampstep, FALSE);
                BASS_ChannelPlay(chstep, FALSE);
            }
        }

        is_x = TRUE;

        camera.x = new_cx;
        camera.y = new_cy;
    }
}
