#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <windows.h>
#include <direct.h>
#include <gl/glu.h>
#include <io.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>

#include "camera.h"

#define OPENCRAFT_VERSION "0.0.3a"

BOOL inverted_y;

// Текстуры

int cobblestone_texture;
int grass_top_texture;
int grass_side_texture;
int dirt_texture;
int planks_texture;
int stone_texture;
int sapling_texture;

int man_texture;

int ascii_texture;

int icons_texture;

BOOL DirectoryExists(const char* absolutePath);

float camera_z_in_jump;

int timer = 0;
int timer_r = 0;
int timer_g = 0;
int timer_y = 0;

int select_inv = 1;

float size = 20.f;

int jump_tmp = 0;
BOOL jump_down = FALSE;
float jump_go[10] = {1.10, 1.20, 1.30, 1.40, 1.50, 1.60, 1.70, 1.80, 1.90, 1.95};

BOOL is_jumping = FALSE;

int dist_x = 0;
int dist_y = 0;

GLuint filter;
GLuint fogMode[] = {GL_EXP, GL_EXP2, GL_LINEAR};
GLuint fogfilter = 2;
GLfloat fogColor[4] = {0.6, 0.8, 1, 1};

// Debug-переменные

int draw_cnt = 0;

LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
void EnableOpenGL(HWND hwnd, HDC*, HGLRC*);
void DisableOpenGL(HWND, HDC, HGLRC);

float theta = 0.0f;

float block[] = {0,0,0, 1,0,0, 1,1,0, 0,1,0, 0,0,1, 1,0,1, 1,1,1, 0,1,1};
float sprite_vertex[] = {0,0,0, 0.1,0,0, 0.1,0.1,0, 0,0.1,0, 0,0,0.1, 0.1,0,0.1, 0.1,0.1,0.1, 0,0.1,0.1};
GLuint blockInd[] = {0,1,5, 5,4,0, 1,2,6, 6,5,1, 2,3,7, 7,6,2, 3,0,4, 4,7,3};
float block_texture[] = {0,1, 1,1, 0,1, 1,1, 0,0, 1,0, 0,0, 1,0};

float sprite_texture[] = {0,0.1, 0.1,0.1, 0,0.1, 0.1,0.1, 0,0, 0.1,0, 0,0, 0.1,0};

float sprite_2_texture[] = {0.5,0.6, 0.6,0.6, 0.5,0.6, 0.6,0.6, 0.5,0.5, 0.6,0.5, 0.5,0.5, 0.6,0.5};

float sprite_3_texture[] = {0.9,1, 1,1, 0.9,1, 1,1, 0.9,0.9, 1,0.9, 0.9,0.9, 1,0.9};


int blockIncCnt = sizeof(blockInd) / sizeof(GLuint);
float normal[] = {-1,-1,3, 1, -1, 3, 1,1,3, -1, 1, 3, -1,-1,3, 1, -1, 3, 1,1,3, -1, 1, 3};

float block_texture_uad[] = {0,1, 1,1, 1,0, 0,0, 0,1, 1,1, 1,0, 0,0};
GLuint block_Ind_uad[] = {0,1,2, 2,3,0, 4,5,6, 6,7,4};
int blockIncCnt_uad = sizeof(block_Ind_uad) / sizeof(GLuint);

float block_texture_down[] = {0,1, 1,1, 1,0, 0,0, 0,1, 1,1, 1,0, 0,0};
GLuint block_Ind_down[] = {0,1,2, 2,3,0};

float block_texture_up[] = {0,1, 1,1, 1,0, 0,0, 0,1, 1,1, 1,0, 0,0};
GLuint block_Ind_up[] = {4,5,6, 6,7,4};
int blockIncCnt_up = sizeof(block_Ind_up) / sizeof(GLuint);
int blockIncCnt_down = sizeof(block_Ind_down) / sizeof(GLuint);


// Entities

// Man

float entity_man_leg[] = {0,0,0, 0.25,0,0, 0.25,0.25,0, 0,0.25,0, 0,0,0.8, 0.25,0,0.8, 0.25,0.25,0.8, 0,0.25,0.8};
GLuint entity_man_leg_ind[] = {0,1,5, 5,4,0, 1,2,6, 6,5,1, 2,3,7, 7,6,2, 3,0,4, 4,7,3};
int entity_man_leg_ind_cnt = sizeof(entity_man_leg_ind) / sizeof(GLuint);

float entity_man_leg_up[] = {0,0.25, 0.25,0.25, 0.25,0, 0,0, 0,0.25, 0.25,0.25, 0.25,0, 0,0};
GLuint entity_man_leg_ind_up[] = {4,5,6, 6,7,4};
int entity_man_leg_ind_up_cnt = sizeof(entity_man_leg_ind_up) / sizeof(GLuint);

float man_right_leg_texture[] = {0.750,1, 0.8125,1, 0.750,1, 0.8125,1, 0.750,0.66, 0.8125,0.66, 0.750,0.66, 0.8125,0.66};
float man_right_leg_texture_2[] = {0.8125,1, 0.8745,1, 0.8125,1, 0.8745,1, 0.8125,0.66, 0.8745,0.66, 0.8125,0.66, 0.8745,0.66};
float man_right_leg_texture_3[] = {0.8745,1, 0.937,1, 0.8745,1, 0.937,1, 0.8745,0.66, 0.937,0.66, 0.8745,0.66, 0.937,0.66};
float man_right_leg_texture_4[] = {0.937,1, 1,1, 0.937,1, 1,1, 0.937,0.66, 1,0.66, 0.937,0.66, 1,0.66};
float man_right_leg_texture_up[] = {0.6875,0.78, 0.750,0.78, 0.6875,0.78, 0.750,0.78, 0.6875,0.66, 0.750,0.66, 0.6875,0.66, 0.750,0.66};
float man_right_leg_texture_down[] = {0.750,0.905, 0.6875,0.905, 0.6875,0.785, 0.750,0.785, 0.750,0.905, 0.6875,0.905, 0.6875,0.785, 0.750,0.785};

float man_left_leg_texture[] = {0.750,1, 0.8125,1, 0.750,1, 0.8125,1, 0.750,0.66, 0.8125,0.66, 0.750,0.66, 0.8125,0.66};
float man_left_leg_texture_2[] = {0.8125,1, 0.8745,1, 0.8125,1, 0.8745,1, 0.8125,0.66, 0.8745,0.66, 0.8125,0.66, 0.8745,0.66};
float man_left_leg_texture_3[] = {0.8745,1, 0.937,1, 0.8745,1, 0.937,1, 0.8745,0.66, 0.937,0.66, 0.8745,0.66, 0.937,0.66};
float man_left_leg_texture_4[] = {0.937,1, 1,1, 0.937,1, 1,1, 0.937,0.66, 1,0.66, 0.937,0.66, 1,0.66};
float man_left_leg_texture_up[] = {0.6875,0.78, 0.750,0.78, 0.6875,0.78, 0.750,0.78, 0.6875,0.66, 0.750,0.66, 0.6875,0.66, 0.750,0.66};
float man_left_leg_texture_down[] = {0.750,0.905, 0.6875,0.905, 0.6875,0.785, 0.750,0.785, 0.750,0.905, 0.6875,0.905, 0.6875,0.785, 0.750,0.785};

float entity_man_body[] = {0,0,0, 0.5,0,0, 0.5,0.25,0, 0,0.25,0, 0,0,0.7, 0.5,0,0.7, 0.5,0.25,0.7, 0,0.25,0.7};
GLuint entity_man_body_ind[] = {0,1,5, 5,4,0, 1,2,6, 6,5,1, 2,3,7, 7,6,2, 3,0,4, 4,7,3};
int entity_man_body_ind_cnt = sizeof(entity_man_body_ind) / sizeof(GLuint);

float man_body_texture[] = {0,0.50, 0.125,0.50, 0,0.50, 0.125,0.50, 0,0.25, 0.125,0.25, 0,0.25, 0.125,0.25};
float man_body_texture_2[] = {0.125,0.50, 0.1875,0.50, 0.125,0.50, 0.1875,0.50, 0.125,0.25, 0.1875,0.25, 0.125,0.25, 0.1875,0.25};
float man_body_texture_3[] = {0.375,0.50, 0.5,0.50, 0.375,0.50, 0.5,0.50, 0.375,0.25, 0.5,0.25, 0.375,0.25, 0.5,0.25};
float man_body_texture_4[] = {0.1875,0.50, 0.375,0.50, 0.1875,0.50, 0.375,0.50, 0.1875,0.25, 0.375,0.25, 0.1875,0.25, 0.375,0.25};

float man_body_up_texture[] = {0.375,0.50, 0.500,0.50, 0.375,0.50, 0.500,0.50, 0.375,0.25, 0.500,0.25, 0.375,0.25, 0.500,0.25};

float entity_man_body_up[] = {0,1, 1,1, 1,0, 0,0, 0,1, 1,1, 1,0, 0,0};
GLuint entity_man_body_ind_up[] = {4,5,6, 6,7,4};
int entity_man_body_ind_cnt_up = sizeof(entity_man_body_ind_up) / sizeof(GLuint);

float entity_man_head[] = {0,0,0, 0.5,0,0, 0.5,0.5,0, 0,0.5,0, 0,0,0.5, 0.5,0,0.5, 0.5,0.5,0.5, 0,0.5,0.5};
GLuint entity_man_head_ind_1[] = {2,3,7, 7,6,2};
GLuint entity_man_head_ind_2[] = {1,2,6, 6,5,1};
GLuint entity_man_head_ind_3[] = {0,1,5, 5, 4, 0};
GLuint entity_man_head_ind_4[] = {3,0,4, 4,7,3};
int entity_man_head_ind_cnt_1 = sizeof(entity_man_head_ind_1) / sizeof(GLuint);
int entity_man_head_ind_cnt_2 = sizeof(entity_man_head_ind_2) / sizeof(GLuint);
int entity_man_head_ind_cnt_3 = sizeof(entity_man_head_ind_3) / sizeof(GLuint);
int entity_man_head_ind_cnt_4 = sizeof(entity_man_head_ind_4) / sizeof(GLuint);

float man_head_texture[] = {0,0.0, 0.125,0.25, 0,0.25, 0.125,0.25, 0,0, 0.125,0, 0,0, 0.125,0};
float man_head_texture_2[] = {0.125,0.0, 0.25,0.25, 0.125,0.25, 0.25,0.25, 0.125,0, 0.25,0, 0.125,0, 0.25,0};
float man_head_texture_3[] = {0.375,0.25, 0.5,0.25, 0.375,0.25, 0.5,0.25, 0.375,0, 0.5,0, 0.375,0, 0.5,0};
float man_head_texture_4[] = {0.250,0.25, 0.375,0.25, 0.250,0.25, 0.375,0.25, 0.250,0, 0.375,0, 0.250,0, 0.375,0};
float man_head_up_texture[] = {0.5,0.25, 0.625,0.25, 0.625,0, 0.5,0, 0.5,0.25, 0.625,0.25, 0.625,0, 0.5,0};
float man_head_down_texture[] = {0.625,0.25, 0.750,0.25, 0.750,0, 0.625,0, 0.625,0.25, 0.750,0.25, 0.750,0, 0.625,0};

float entity_man_head_up[] = {0,0.5, 0.5,0.5, 0.5,0, 0,0, 0,0.5, 0.5,0.5, 1,0, 0,0};
GLuint entity_man_head_ind_up[] = {4,5,6, 6,7,4};
int entity_man_head_up_cnt = sizeof(entity_man_head_ind_up) / sizeof(GLuint);

float entity_man_hand[] = {0,0,0, 0.25,0,0, 0.25,0.25,0, 0,0.25,0, 0,0,0.7, 0.25,0,0.7, 0.25,0.25,0.7, 0,0.25,0.7};
GLuint entity_man_hand_ind[] = {0,1,5, 5,4,0, 1,2,6, 6,5,1, 2,3,7, 7,6,2, 3,0,4, 4,7,3};
int entity_man_hand_ind_cnt = sizeof(entity_man_hand_ind) / sizeof(GLuint);

float man_right_hand_texture[] = {0,1, 0.0625,1, 0,1, 0.0625,1, 0,0.66, 0.0625,0.66, 0,0.66, 0.0625,0.66};
float man_right_hand_texture_2[] = {0.0625,1, 0.125,1, 0.0625,1, 0.125,1, 0.0625,0.66, 0.125,0.66, 0.0625,0.66, 0.125,0.66};
float man_right_hand_texture_3[] = {0.125,1, 0.1875,1, 0.125,1, 0.1875,1, 0.125,0.66, 0.1875,0.66, 0.125,0.66, 0.1875,0.66};
float man_right_hand_texture_4[] = {0.1875,1, 0.25,1, 0.1875,1, 0.25,1, 0.1875,0.66, 0.25,0.66, 0.1875,0.66, 0.25,0.66};
float man_right_hand_up_texture[] = {0.25,0.78, 0.3125,0.78, 0.3125,0.66, 0.25,0.66, 0.25,0.78, 0.3125,0.78, 0.3125,0.66, 0.25,0.66};
float man_right_hand_down_texture[] = {0.25,0.905, 0.3125,0.905, 0.3125,0.785, 0.25,0.785, 0.25,0.905, 0.3125,0.905, 0.3125,0.785, 0.25,0.785};

float man_left_hand_texture[] = {0.3125,1, 0.375,1, 0.3125,1, 0.375,1, 0.3125,0.66, 0.375,0.66, 0.3125,0.66, 0.375,0.66};
float man_left_hand_texture_2[] = {0.375,1, 0.4375,1, 0.375,1, 0.4375,1, 0.375,0.66, 0.4375,0.66, 0.375,0.66, 0.4375,0.66};
float man_left_hand_texture_3[] = {0.4375,1, 0.5,1, 0.4375,1, 0.5,1, 0.4375,0.66, 0.5,0.66, 0.4375,0.66, 0.5,0.66};
float man_left_hand_texture_4[] = {0.5,1, 0.5625,1, 0.5,1, 0.5625,1, 0.5,0.66, 0.5625,0.66, 0.5,0.66, 0.5625,0.66};
float man_left_hand_up_texture[] = {0.5625,0.78, 0.625,0.78, 0.625,0.66, 0.5625,0.66, 0.5625,0.78, 0.625,0.78, 0.625,0.66, 0.5625,0.66};
float man_left_hand_down_texture[] = {0.5625,0.905, 0.625,0.785, 0.625,0.66, 0.5625,0.66, 0.5625,0.905, 0.625,0.905, 0.625,0.785, 0.5625,0.785};

float cursor[] = {0,0, 32,0, 32,32, 0,32};
float cursor_texture[] = {0,0, 0.06455,0, 0.06455,0.06455, 0,0.06455};

float block_inventory[] = {0,0, 32, 16, 32, 50, 0, 37};
float block_inventory_2[] = {32, 16, 64, 0, 64, 37, 32, 50};
float block_inventory_3[] = {0,0, 32, -16, 64, 0, 32, 16};

float block_inventory_texture[] = {0,0, 1,0, 1,1, 0,1};

float plant[] = {-0.5, 0, 0, 0.5, 0, 0, 0.5,0,1, -0.5,0,1, 0,-0.5,0, 0,0.5,0, 0,0.5,1, 0, -0.5, 1};
float plant_texture[] = {0,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0};
GLuint plantInd[] = {0,1,2, 2,3,0, 4,5,6, 6,7,4};

int plantIndCnt = sizeof(plantInd) / sizeof(GLuint);

float block_inventory_flat[] = {0,0, 64,0, 64,64, 0,64};

float rectCoord[] = {0,0, 16,0, 16,16, 0,16};
float rectTex[] = {0,0, 1,0, 1,1, 0,1};

void GenerateNewChunk(int chunkx, int chunky)
{
    int coolz = 0;
    int oldcoolz = 0;
    int gen_climbs = 0;
    for(int y = 0; y <= 15; y++)
    {
        for(int x = 0; x <= 15; x++)
        {
            coolz = 0;
            int randomcool = rand();
            randomcool %= 1000;
            if(randomcool == 1 && gen_climbs == 0)
            {
                gen_climbs = 1;
            }
            if(x == 0 && y == 0 && chunkx == 0 && chunky == 0) coolz = 45 + rand() % 5;
            else if(x == 0 && chunkx != 0)
            {
                int zr = 0;
                int zxx = 0;
                for(int zq = 255; zq >= 0; zq--)
                {
                    if(y != 0)
                    {
                        if(world[chunkx][chunky][x][y-1][zq] != 0)
                        {
                            zr = zq;
                            break;
                        }
                    }
                    else if(chunky != 0 && y == 0)
                    {
                        if(world[chunkx][chunky-1][x][15][zq] != 0)
                        {
                            zr = zq;
                            break;
                        }
                    }
                }
                for(int zx = 255; zx >= 0; zx--)
                {
                    if(world[chunkx-1][chunky][15][y][zx] != 0)
                    {
                        zxx = zx;
                        break;
                    }
                }
                int oror = rand() % 500;
                int we = 0;
                we = zxx;
                if(gen_climbs == 0)
                {
                    if(oror <= 30) coolz = we + 1;
                    else if(oror >= 30 && oror <= 60) coolz = we - 1;
                    else coolz = we;
                }
                else
                {
                    if(oror > 0 && oror < 300) coolz = we + 2;
                    else coolz = we + 1;
                    if(coolz >= 100) gen_climbs = 0;
                }
            }
            else if((y != 0 && chunky == 0) || chunky != 0)
            {
                int zr = 0;
                int zxx = 0;
                for(int zq = 255; zq >= 0; zq--)
                {
                    if(chunky == 0 || chunky != 0 && y != 0)
                    {
                        if(world[chunkx][chunky][x][y-1][zq] != 0)
                        {
                            zr = zq;
                            break;
                        }
                    }
                    else if(chunky != 0 && y == 0)
                    {
                        if(world[chunkx][chunky-1][x][15][zq] != 0)
                        {
                            zr = zq;
                            break;
                        }
                    }
                }
                for(int zx = 255; zx >= 0; zx--)
                {
                    if(chunkx == 0)
                    {
                        if(world[chunkx][chunky][x-1][y][zx] != 0)
                        {
                            zxx = zx;
                            break;
                        }
                    }
                    else if(chunkx != 0 && x == 0 || chunkx != 0 && x != 0)
                    {
                        if(world[chunkx-1][chunky][15][y][zx] != 0)
                        {
                            zxx = zx;
                            break;
                        }
                    }
                }
                int oror = rand() % 500;
                int we = 0;
                if(x != 0) we = avgnoov(zr, zxx);
                else we = zr;
                if(gen_climbs == 0)
                {
                    if(oror <= 30) coolz = we + 1;
                    else if(oror >= 30 && oror <= 60) coolz = we - 1;
                    else coolz = we;
                }
                else
                {
                    if(oror > 0 && oror < 300) coolz = we + 2;
                    else coolz = we + 1;
                    if(coolz >= 100) gen_climbs = 0;
                }
            }
            else
            {
                int oror = rand() % 500;
                int ze = 0;
                if(x != 0)
                {
                    for(int zq = 255; zq >= 0; zq--)
                    {
                        if(world[chunkx][chunky][x-1][y][zq] != 0)
                        {
                            ze = zq;
                            break;
                        }
                    }
                    if(gen_climbs == 0)
                    {
                        if(oror <= 30) coolz = ze + 1;
                        else if(oror >= 30 && oror <= 60) coolz = ze - 1;
                        else coolz = ze;
                    }
                    else
                    {
                        if(oror > 0 && oror < 300) coolz = ze + 2;
                        else coolz = ze + 1;
                        if(coolz >= 100) gen_climbs = 0;
                    }
                }
                else
                {
                    for(int zq = 255; zq >= 0; zq--)
                    {
                        if(world[chunkx-1][chunky][x][y][zq] != 0)
                        {
                            ze = zq;
                            break;
                        }
                    }
                    if(gen_climbs == 0)
                    {
                        if(oror <= 30) coolz = ze + 1;
                        else if(oror >= 30 && oror <= 60) coolz = ze - 1;
                        else coolz = ze;
                    }
                    else
                    {
                        if(oror > 0 && oror < 300) coolz = ze + 2;
                        else coolz = ze + 1;
                        if(coolz >= 100) gen_climbs = 0;
                    }
                }
            }
            for(int z = 0; z <= 255; z++)
            {
                if(z < coolz && z - coolz >= -3) world[chunkx][chunky][x][y][z] = 3;
                if(z < coolz && z - coolz < -3) world[chunkx][chunky][x][y][z] = 5;
                if(z == coolz) world[chunkx][chunky][x][y][z] = 2;
                if(z > coolz) world[chunkx][chunky][x][y][z] = 0;
                oldcoolz = coolz;
            }
        }
    }
}

void GenerateCaves()
{
    int g3 = 0;
    for(int y = 0; y < 256; y++)
    {
        for(int x = 0; x < 256; x++)
        {
            int randomcool = rand();
            randomcool %= 10000;
            if(randomcool == 9999)
            {
                int randomc = rand();
                randomc %= 100;
                for(int zx = 255; zx >= 0; zx--)
                {
                    if(GetBlockID(x, y, zx) != 0)
                    {
                        for(int zz = zx; zz >= 11; zz--)
                        {
                            for(int xx = 0; xx <= 4; xx++)
                            {
                                for(int zy = 0; zy <= 4; zy++)
                                {
                                    SetBlock(0, x, y+xx, zz+zy);
                                }
                            }
                            if(randomc > 0 && randomc < 50) x++;
                            else x--;
                            int randomc2 = rand();
                            randomc2 %= 100;
                            if(randomc2 < 30) y++;
                            else if(randomc2 < 60) y--;

                        }
                        g3++;
                        if(g3 >= 3) return;
                    }
                }
            }
        }
    }
}

void GenerateNewWorld()
{
    for(int y = 0; y <= 15; y++)
    {
        for(int x = 0; x <= 15; x++)
        {
            GenerateNewChunk(x, y);
        }
    }
    GenerateCaves();
    for(int zx = 255; zx >= 0; zx--)
    {
        if(GetBlockID((int)camera.x, (int)camera.y, zx) != 0)
        {
            camera.z = zx+1;
            break;
        }
    }
    SaveWorld();
}
void LoadTexture(char *filename, int *target)
{
    int width, height, cnt;
    unsigned char *data = stbi_load(filename, &width, &height, &cnt, 0);

    glGenTextures(1, target);
    glBindTexture(GL_TEXTURE_2D, *target);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, cnt == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, data);

    glBindTexture(GL_TEXTURE_2D, 0);

    stbi_image_free(data);
}

void Game_Init()
{
    LoadTexture("textures/blocks/cobblestone.png", &cobblestone_texture);
    LoadTexture("textures/blocks/grass_top.png", &grass_top_texture);
    LoadTexture("textures/blocks/grass_side.png", &grass_side_texture);
    LoadTexture("textures/blocks/dirt.png", &dirt_texture);
    LoadTexture("textures/blocks/planks.png", &planks_texture);
    LoadTexture("textures/blocks/stone.png", &stone_texture);
    LoadTexture("textures/blocks/sapling.png", &sapling_texture);

    LoadTexture("textures/entity/man.png", &man_texture);

    LoadTexture("textures/font/ascii.png", &ascii_texture);

    LoadTexture("textures/gui/icons.png", &icons_texture);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_NORMALIZE);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_FOG);
    glFogi(GL_FOG_MODE, fogMode[fogfilter]);
    glFogfv(GL_FOG_COLOR, fogColor);
    glFogf(GL_FOG_DENSITY, 0.35f);
    glHint(GL_FOG_HINT, GL_DONT_CARE);
    glFogf(GL_FOG_START, 7.0f);
    glFogf(GL_FOG_END, 8.0f);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.99);

    srand(time(NULL));

    TCHAR buffer[MAX_PATH];
    GetCurrentDirectory(sizeof(buffer), buffer);
    char path[MAX_PATH];
    sprintf(path, "%s\\saves\\world", buffer);
    if(DirectoryExists(path))
    {
        FILE *level;
        FILE *file_world;
        FILE *file_version;

        sprintf(path, "%s\\saves\\world\\level.dat", buffer);

        level = fopen(path, "r");

        if(level != NULL)
        {
            fread(&camera, 1, sizeof(camera), level);
            fclose(level);
        }

        sprintf(path, "%s\\saves\\world\\world.ocw", buffer);

        file_world = fopen(path, "r");

        if(file_world != NULL)
        {
            fread(world, 1, sizeof(world), file_world);
            fclose(file_world);
        }

        char version[50];

        sprintf(path, "%s\\saves\\world\\version.dat", buffer);

        file_version = fopen(path, "r");

        if(file_version != NULL)
        {
            fread(version, 1, sizeof(version), file_version);
            fclose(file_version);
        }
        else
        {
            for(int chunkx = 0; chunkx < 16; chunkx++)
            {
                for(int chunky = 0; chunky < 16; chunky++)
                {
                    for(int z = 0; z < 256; z++)
                    {
                        for(int y = 0; y < 16; y++)
                        {
                            for(int x = 0; x < 16; x++)
                            {
                                if(world[chunkx][chunky][x][y][z] != 0) world[chunkx][chunky][x][y][z] = 5;
                            }
                        }
                    }
                }
            }
        }

    }
    else GenerateNewWorld();

    for(int i = 0; i < 100; i++)
    {
        int f = rand() % 255;
        int s = rand() % 255;
        Entities[i].x = (float)f;
        Entities[i].y = (float)s;
        for(int zx = 255; zx >= 0; zx--)
        {
            if(GetBlockID((int)Entities[i].x, (int)Entities[i].y, zx) != 0)
            {
                Entities[i].z = zx+1;
                break;
            }
        }
        Entities[i].entity_id = 1;
    }
}
void WndResize(int x, int y)
{
    glViewport(0,0,x,y);
    scrKoef = x / (float)y;
    scrSize.x = x;
    scrSize.y = y;
}

void Player_Move()
{
    Camera_MoveDirection(GetKeyState('W') < 0 ? 1: (GetKeyState('S') < 0 ? -1 : 0)
                         ,GetKeyState('D') < 0 ? 1 : (GetKeyState('A') < 0 ? -1: 0)
                         ,0.2);
    Camera_AutoMoveByMouse(scrSize.x/2, scrSize.y/2, 0.2);
}

void Game_Show()
{
    float sz = 0.1;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-scrKoef*sz,scrKoef*sz, -sz, sz, sz*2, 100);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);

    glClearColor(0.6, 0.8, 1, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    int chunkx = camera.x / 16;
    int chunky = camera.y / 16;
    int cameraz = (int)camera.z - 1;
    int jdcx = 16*chunkx;
    int jdcy = 16*chunky;
    float xjump = camera.x - jdcx;
    float yjump = camera.y - jdcy;

    BOOL is_fly = FALSE;

    if(is_jumping)
    {
        float ste = 2 * jump_go[jump_tmp];
        camera.z = (camera_z_in_jump - 2)+(ste);
        if(camera.x >= 0 && camera.x < 256 && camera.y >= 0 && camera.y < 256 && camera.z >= 0 && camera.z < 256)
        {
            if(!jump_down) jump_tmp++;
            else jump_tmp--;
            if(jump_tmp == 9) jump_down = TRUE;
            if(jump_tmp == 0 && jump_down)
            {
                is_jumping = FALSE;
                jump_down = FALSE;
                timer = 3;
                jump_tmp = 0;
            }
            if(jump_down == TRUE && world[chunkx][chunky][(int)xjump][(int)yjump][(int)camera.z] != 0 && world[chunkx][chunky][(int)xjump][(int)yjump][(int)camera.z] != 6)
            {
                is_jumping = FALSE;
                jump_down = FALSE;
                timer = 3;
                jump_tmp = 0;
            }
            if(jump_down == FALSE && world[chunkx][chunky][(int)xjump][(int)yjump][(int)camera.z + 2] != 0)
            {
                is_jumping = FALSE;
                jump_down = FALSE;
                timer = 3;
                jump_tmp = 0;
            }
        }
        else
        {
            is_jumping = FALSE;
            jump_down = FALSE;
            timer = 3;
            jump_tmp = 0;
        }
    }
    if(chunkx > 15 || chunky > 15 || camera.x < 0 || camera.y < 0 || cameraz <= -1 || chunkx < 0 || chunky < 0)
    {
        if(!is_jumping)
        {
            camera.z -= 0.3;
            is_fly = TRUE;
        }
    }
    else if(world[chunkx][chunky][(int)xjump][(int)yjump][(int)cameraz] == 0 && !is_jumping || world[chunkx][chunky][(int)xjump][(int)yjump][(int)cameraz] == 6 && !is_jumping)
    {
        camera.z -= 0.3;
        is_fly = TRUE;
    }
    if(chunkx <= 15 && chunky <= 15 && chunkx >= 0 && chunky >= 0)
    {
        if(GetKeyState(' ') < 0 && !is_jumping && world[chunkx][chunky][(int)xjump][(int)yjump][(int)cameraz] != 0 && camera.x > 0 && camera.x < 256 && camera.y > 0 && camera.y < 256 && camera.z >= 0 && camera.z < 256 && !is_fly && ((int)camera.z - cameraz) == 1 && timer <= 0)
        {
            is_jumping = TRUE;
            camera_z_in_jump = camera.z;
        }
    }
    if(!is_fly && !is_jumping && cameraz != camera.z + 1)
    {
        camera.z -= 0.3;
        if(camera.z < cameraz + 1)
        {
            camera.z = cameraz + 1;
        }
    }

    if(GetKeyState('R') < 0 && timer_r <= 0)
    {
        BOOL success = FALSE;
        while(success == FALSE)
        {
            int x = rand();
            int y = rand();
            x %= 255;
            y %= 255;
            for(int i = 0; i < 256; i++)
            {
                if(GetBlockID(x, y, i) == 0 && GetBlockID(x, y, i+1) == 0)
                {
                    camera.x = x + 0.5;
                    camera.y = y + 0.5;
                    camera.z = i;
                    success = TRUE;
                    timer_r = 10;
                    break;
                }
            }
        }
    }
    if(GetKeyState('G') < 0 && timer_g <= 0)
    {
        for(int y = 0; y < 500; y++)
        {
            if(Entities[y].entity_id == 0)
            {
                Entities[y].entity_id = 1;
                Entities[y].x = camera.x;
                Entities[y].y = camera.y;
                Entities[y].z = camera.z;
                timer_g = 10;
                break;
            }
        }
    }
    if(GetKeyState('Y') < 0 && timer_y <= 0)
    {
        if(!inverted_y) inverted_y = TRUE;
        else inverted_y = FALSE;
        timer_y = 10;
    }
    if(GetKeyState('1') < 0) select_inv = 1;
    if(GetKeyState('2') < 0) select_inv = 2;
    if(GetKeyState('3') < 0) select_inv = 3;
    if(GetKeyState('4') < 0) select_inv = 4;
    if(GetKeyState('6') < 0) select_inv = 6;

    int dcnt = 0;
    glPushMatrix();
        Camera_Apply();
        glPushMatrix();
            glRotatef(theta, 0,1,0);
            float position[] = {0,0,257,0};
            glLightfv(GL_LIGHT0, GL_POSITION, position);
        glPopMatrix();
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glEnableClientState(GL_NORMAL_ARRAY);
        int mychunkx = camera.x / 16 - 1;
        int mychunky = camera.y / 16 - 1;
        if(mychunkx > 15) mychunkx = 14;
        if(mychunky > 15) mychunky = 14;
        if(mychunky < 0) mychunky = 0;
        if(mychunkx < 0) mychunkx = 0;
        for(int chunky = mychunky; chunky <= mychunky + 2; chunky++)
        {
            for(int chunkx = mychunkx; chunkx <= mychunkx + 2; chunkx++)
            {
                if(chunkx > 15 || chunky > 15 || chunkx < 0 || chunky < 0) break;
                for(int z = 0; z <= 255; z++)
                {
                    for(int y = 0; y <= 15; y++)
                    {
                        for(int x = 0; x <= 15; x++)
                        {
                            int dcx = 16*chunkx;
                            int dcy = 16*chunky;
                            if(camera.x + 9 < x + dcx || camera.x - 9 > x + dcx || camera.y + 9 < y + dcy || camera.y - 9 > y + dcy || camera.z + 9 < z || camera.z - 9 > z) continue;
                            if(world[chunkx][chunky][x][y][z] != 0 && world[chunkx][chunky][x+1][y][z] != 0 && world[chunkx][chunky][x-1][y][z] != 0 && world[chunkx][chunky][x][y+1][z] != 0 && world[chunkx][chunky][x][y-1][z] != 0 && world[chunkx][chunky][x][y][z+1] != 0 && world[chunkx][chunky][x][y][z-1] != 0 && x != 0 && x != 15 &&  y != 0 && y != 15 && z != 0 && z != 255 && world[chunkx][chunky][x][y][z] != 6 && world[chunkx][chunky][x+1][y][z] != 6 && world[chunkx][chunky][x-1][y][z] != 6 && world[chunkx][chunky][x][y+1][z] != 6 && world[chunkx][chunky][x][y-1][z] != 6 && world[chunkx][chunky][x][y][z+1] != 6 && world[chunkx][chunky][x][y][z-1] != 6) continue;
                            if(world[chunkx][chunky][x][y][z] == 1)
                            {
                                glVertexPointer(3, GL_FLOAT, 0, block);
                                glNormalPointer(GL_FLOAT, 0, normal);
                                glNormal3f(0,0,1);
                                glColor3f(0.7, 0.7, 0.7);
                                glBindTexture(GL_TEXTURE_2D, cobblestone_texture);
                                glTexCoordPointer(2, GL_FLOAT, 0, block_texture);
                                glPushMatrix();
                                    glTranslatef(x+dcx, y+dcy, z);
                                    glDrawElements(GL_TRIANGLES, blockIncCnt, GL_UNSIGNED_INT, blockInd);
                                    glTexCoordPointer(2, GL_FLOAT, 0, block_texture_uad);
                                    glDrawElements(GL_TRIANGLES, blockIncCnt_uad, GL_UNSIGNED_INT, block_Ind_uad);
                                glPopMatrix();
                            }
                            else if(world[chunkx][chunky][x][y][z] == 2)
                            {
                                glVertexPointer(3, GL_FLOAT, 0, block);
                                glNormalPointer(GL_FLOAT, 0, normal);
                                glNormal3f(0,0,1);
                                glColor3f(0.7, 0.7, 0.7);
                                glBindTexture(GL_TEXTURE_2D, grass_side_texture);
                                glTexCoordPointer(2, GL_FLOAT, 0, block_texture);
                                glPushMatrix();
                                    glTranslatef(x+dcx, y+dcy, z);
                                    glDrawElements(GL_TRIANGLES, blockIncCnt, GL_UNSIGNED_INT, blockInd);
                                    glBindTexture(GL_TEXTURE_2D, grass_top_texture);
                                    glTexCoordPointer(2, GL_FLOAT, 0, block_texture_up);
                                    glDrawElements(GL_TRIANGLES, blockIncCnt_up, GL_UNSIGNED_INT, block_Ind_up);
                                    glBindTexture(GL_TEXTURE_2D, dirt_texture);
                                    glTexCoordPointer(2, GL_FLOAT, 0, block_texture_down);
                                    glDrawElements(GL_TRIANGLES, blockIncCnt_down, GL_UNSIGNED_INT, block_Ind_down);
                                glPopMatrix();
                            }
                            else if(world[chunkx][chunky][x][y][z] == 3)
                            {
                                glVertexPointer(3, GL_FLOAT, 0, block);
                                glNormalPointer(GL_FLOAT, 0, normal);
                                glNormal3f(0,0,1);
                                glColor3f(0.7, 0.7, 0.7);
                                glBindTexture(GL_TEXTURE_2D, dirt_texture);
                                glTexCoordPointer(2, GL_FLOAT, 0, block_texture);
                                glPushMatrix();
                                    glTranslatef(x+dcx, y+dcy, z);
                                    glDrawElements(GL_TRIANGLES, blockIncCnt, GL_UNSIGNED_INT, blockInd);
                                    glTexCoordPointer(2, GL_FLOAT, 0, block_texture_uad);
                                    glDrawElements(GL_TRIANGLES, blockIncCnt_uad, GL_UNSIGNED_INT, block_Ind_uad);
                                glPopMatrix();
                            }
                            else if(world[chunkx][chunky][x][y][z] == 4)
                            {
                                glVertexPointer(3, GL_FLOAT, 0, block);
                                glNormalPointer(GL_FLOAT, 0, normal);
                                glNormal3f(0,0,1);
                                glColor3f(0.7, 0.7, 0.7);
                                glBindTexture(GL_TEXTURE_2D, planks_texture);
                                glTexCoordPointer(2, GL_FLOAT, 0, block_texture);
                                glPushMatrix();
                                    glTranslatef(x+dcx, y+dcy, z);
                                    glDrawElements(GL_TRIANGLES, blockIncCnt, GL_UNSIGNED_INT, blockInd);
                                    glTexCoordPointer(2, GL_FLOAT, 0, block_texture_uad);
                                    glDrawElements(GL_TRIANGLES, blockIncCnt_uad, GL_UNSIGNED_INT, block_Ind_uad);
                                glPopMatrix();
                            }
                            else if(world[chunkx][chunky][x][y][z] == 5)
                            {
                                glVertexPointer(3, GL_FLOAT, 0, block);
                                glNormalPointer(GL_FLOAT, 0, normal);
                                glNormal3f(0,0,1);
                                glColor3f(0.7, 0.7, 0.7);
                                glBindTexture(GL_TEXTURE_2D, stone_texture);
                                glTexCoordPointer(2, GL_FLOAT, 0, block_texture);
                                glPushMatrix();
                                    glTranslatef(x+dcx, y+dcy, z);
                                    glDrawElements(GL_TRIANGLES, blockIncCnt, GL_UNSIGNED_INT, blockInd);
                                    glTexCoordPointer(2, GL_FLOAT, 0, block_texture_uad);
                                    glDrawElements(GL_TRIANGLES, blockIncCnt_uad, GL_UNSIGNED_INT, block_Ind_uad);
                                glPopMatrix();
                            }
                            else if(world[chunkx][chunky][x][y][z] == 6)
                            {
                                glVertexPointer(3, GL_FLOAT, 0, plant);
                                glNormal3f(0,0,1);
                                glColor3f(0.7, 0.7, 0.7);
                                glBindTexture(GL_TEXTURE_2D, sapling_texture);
                                glTexCoordPointer(2, GL_FLOAT, 0, plant_texture);
                                glPushMatrix();
                                    glTranslatef(x+dcx+0.5, y+dcy+0.5, z);
                                    glDrawElements(GL_TRIANGLES, plantIndCnt, GL_UNSIGNED_INT, plantInd);
                                glPopMatrix();
                                if(world[chunkx][chunky][x][y][z+1] != 0 && world[chunkx][chunky][x][y][z+1] != 6) world[chunkx][chunky][x][y][z] = 0;
                                if(world[chunkx][chunky][x][y][z-1] != 2 && world[chunkx][chunky][x][y][z-1] != 3) world[chunkx][chunky][x][y][z] = 0;
                            }
                            int randome = rand();
                            randome %= 500;
                            if(randome == 1)
                            {
                                if(world[chunkx][chunky][x][y][z] == 3)
                                {
                                    for(int zz = z+1; zz < 256; zz++)
                                    {
                                        if(world[chunkx][chunky][x][y][zz] != 0 && world[chunkx][chunky][x][y][z+1] != 6) break;
                                        if(zz == 255)
                                        {
                                            if(GetBlockID(x+dcx+1, y+dcy, z) == 2 || GetBlockID(x+dcx-1, y+dcy, z) == 2 || GetBlockID(x+dcx, y+dcy+1, z) == 2 || GetBlockID(x+dcx, y+dcy-1, z) == 2 || GetBlockID(x+dcx+1, y+dcy, z+1) == 2 || GetBlockID(x+dcx-1, y+dcy, z+1) == 2 || GetBlockID(x+dcx, y+dcy+1, z+1) == 2 || GetBlockID(x+dcx, y+dcy-1, z+1) == 2 || GetBlockID(x+dcx+1, y+dcy, z-1) == 2 || GetBlockID(x+dcx-1, y+dcy, z-1) == 2 || GetBlockID(x+dcx, y+dcy+1, z-1) == 2 || GetBlockID(x+dcx, y+dcy-1, z-1) == 2) world[chunkx][chunky][x][y][z] = 2;
                                        }
                                    }
                                }
                                if(world[chunkx][chunky][x][y][z] == 2)
                                {
                                    if(world[chunkx][chunky][x][y][z+1] != 0 && world[chunkx][chunky][x][y][z+1] != 6) world[chunkx][chunky][x][y][z] = 3;
                                }
                            }
                            if(world[chunkx][chunky][x][y][z] != 0) dcnt++;
                        }
                    }
                }
            }
        }
        for(int j = 0; j < 500; j++)
        {
            if(camera.x + 9 < Entities[j].x || camera.x - 9 > Entities[j].x || camera.y + 9 < Entities[j].y || camera.y - 9 > Entities[j].y || camera.z + 9 < Entities[j].z || camera.z - 9 > Entities[j].z) continue;
            if(Entities[j].entity_id == 1)
            {
                glVertexPointer(3, GL_FLOAT, 0, entity_man_leg);
                glNormalPointer(GL_FLOAT, 0, normal);
                glBindTexture(GL_TEXTURE_2D, man_texture);
                glNormal3f(0,0,1);
                glColor3f(0.7, 0.7, 0.7);
                glPushMatrix();
                    glTranslatef(Entities[j].x, Entities[j].y, Entities[j].z);
                    glRotatef(Entities[j].Xrot, 1,0,0);
                    glRotatef(Entities[j].Zrot, 0,0,1);
                    glTranslatef(0, Entities[j].leg_left_c, Entities[j].leg_left_z);
                    glRotatef(Entities[j].leg_left, 1,0,0);

                    glTexCoordPointer(2, GL_FLOAT, 0, man_left_leg_texture);
                    glDrawElements(GL_TRIANGLES, entity_man_head_ind_cnt_1, GL_UNSIGNED_INT, entity_man_head_ind_1);

                    glTexCoordPointer(2, GL_FLOAT, 0, man_left_leg_texture_2);
                    glDrawElements(GL_TRIANGLES, entity_man_head_ind_cnt_2, GL_UNSIGNED_INT, entity_man_head_ind_2);

                    glTexCoordPointer(2, GL_FLOAT, 0, man_left_leg_texture_3);
                    glDrawElements(GL_TRIANGLES, entity_man_head_ind_cnt_3, GL_UNSIGNED_INT, entity_man_head_ind_3);

                    glTexCoordPointer(2, GL_FLOAT, 0, man_left_leg_texture_4);
                    glDrawElements(GL_TRIANGLES, entity_man_head_ind_cnt_4, GL_UNSIGNED_INT, entity_man_head_ind_4);

                    glTexCoordPointer(2, GL_FLOAT, 0, man_left_leg_texture_up);
                    glDrawElements(GL_TRIANGLES, entity_man_head_up_cnt, GL_UNSIGNED_INT, entity_man_head_ind_up);

                    glTranslatef(0.0, 0.0, -0.8);
                    glTexCoordPointer(2, GL_FLOAT, 0, man_left_leg_texture_down);
                    glDrawElements(GL_TRIANGLES, entity_man_head_up_cnt, GL_UNSIGNED_INT, entity_man_head_ind_up);


                    glTranslatef(0.0, 0, 0.8);

                    glRotatef(-Entities[j].leg_left, 1,0,0);
                    glTranslatef(0, -Entities[j].leg_left_c, -Entities[j].leg_left_z);

                    glTranslatef(0.25, 0, 0);

                    glTranslatef(0, Entities[j].leg_right_c, Entities[j].leg_right_z);
                    glRotatef(Entities[j].leg_right, 1,0,0);

                    glVertexPointer(3, GL_FLOAT, 0, entity_man_leg);
                    glTexCoordPointer(2, GL_FLOAT, 0, man_right_leg_texture);
                    glDrawElements(GL_TRIANGLES, entity_man_head_ind_cnt_1, GL_UNSIGNED_INT, entity_man_head_ind_1);

                    glTexCoordPointer(2, GL_FLOAT, 0, man_right_leg_texture_2);
                    glDrawElements(GL_TRIANGLES, entity_man_head_ind_cnt_2, GL_UNSIGNED_INT, entity_man_head_ind_2);

                    glTexCoordPointer(2, GL_FLOAT, 0, man_right_leg_texture_3);
                    glDrawElements(GL_TRIANGLES, entity_man_head_ind_cnt_3, GL_UNSIGNED_INT, entity_man_head_ind_3);

                    glTexCoordPointer(2, GL_FLOAT, 0, man_right_leg_texture_4);
                    glDrawElements(GL_TRIANGLES, entity_man_head_ind_cnt_4, GL_UNSIGNED_INT, entity_man_head_ind_4);

                    glTexCoordPointer(2, GL_FLOAT, 0, man_right_leg_texture_up);
                    glDrawElements(GL_TRIANGLES, entity_man_head_up_cnt, GL_UNSIGNED_INT, entity_man_head_ind_up);

                    glTranslatef(0.0, 0.0, -0.8);
                    glTexCoordPointer(2, GL_FLOAT, 0, man_right_leg_texture_down);
                    glDrawElements(GL_TRIANGLES, entity_man_head_up_cnt, GL_UNSIGNED_INT, entity_man_head_ind_up);

                    glTranslatef(0.0, 0.0, 0.8);

                    glRotatef(-Entities[j].leg_right, 1,0,0);
                    glTranslatef(0, -Entities[j].leg_right_c, -Entities[j].leg_right_z);

                    glTranslatef(-0.25, 0, 0.8);
                    glColor3f(0.8, 0.8, 0.8);
                    glVertexPointer(3, GL_FLOAT, 0, entity_man_body);

                    glTexCoordPointer(2, GL_FLOAT, 0, man_body_texture);

                    glDrawElements(GL_TRIANGLES, entity_man_head_ind_cnt_1, GL_UNSIGNED_INT, entity_man_head_ind_1);
                    glTexCoordPointer(2, GL_FLOAT, 0, man_body_texture_2);
                    glDrawElements(GL_TRIANGLES, entity_man_head_ind_cnt_2, GL_UNSIGNED_INT, entity_man_head_ind_2);
                    glTexCoordPointer(2, GL_FLOAT, 0, man_body_texture_3);
                    glDrawElements(GL_TRIANGLES, entity_man_head_ind_cnt_3, GL_UNSIGNED_INT, entity_man_head_ind_3);
                    glTexCoordPointer(2, GL_FLOAT, 0, man_body_texture_4);
                    glDrawElements(GL_TRIANGLES, entity_man_head_ind_cnt_4, GL_UNSIGNED_INT, entity_man_head_ind_4);
                    glTexCoordPointer(2, GL_FLOAT, 0, man_body_up_texture);
                    glDrawElements(GL_TRIANGLES, entity_man_body_ind_cnt_up, GL_UNSIGNED_INT, entity_man_body_ind_up);

                    glTranslatef(0.0, 0.0, -0.7);
                    glDrawElements(GL_TRIANGLES, entity_man_body_ind_cnt_up, GL_UNSIGNED_INT, entity_man_body_ind_up);

                    glTranslatef(0.0, 0.0, 0.7);
                    glTranslatef(0, -0.1, 0.7);
                    glVertexPointer(3, GL_FLOAT, 0, entity_man_head);
                    glTexCoordPointer(2, GL_FLOAT, 0, man_head_texture);
                    glDrawElements(GL_TRIANGLES, entity_man_head_ind_cnt_1, GL_UNSIGNED_INT, entity_man_head_ind_1);

                    glTexCoordPointer(2, GL_FLOAT, 0, man_head_texture_2);
                    glDrawElements(GL_TRIANGLES, entity_man_head_ind_cnt_2, GL_UNSIGNED_INT, entity_man_head_ind_2);

                    glTexCoordPointer(2, GL_FLOAT, 0, man_head_texture_3);
                    glDrawElements(GL_TRIANGLES, entity_man_head_ind_cnt_3, GL_UNSIGNED_INT, entity_man_head_ind_3);

                    glTexCoordPointer(2, GL_FLOAT, 0, man_head_texture_4);
                    glDrawElements(GL_TRIANGLES, entity_man_head_ind_cnt_4, GL_UNSIGNED_INT, entity_man_head_ind_4);

                    glTexCoordPointer(2, GL_FLOAT, 0, man_head_up_texture);
                    glDrawElements(GL_TRIANGLES, entity_man_head_up_cnt, GL_UNSIGNED_INT, entity_man_head_ind_up);

                    glTranslatef(0.0, 0.0, -0.5);
                    glTexCoordPointer(2, GL_FLOAT, 0, man_head_down_texture);
                    glDrawElements(GL_TRIANGLES, entity_man_head_up_cnt, GL_UNSIGNED_INT, entity_man_head_ind_up);

                    glTranslatef(0.0, 0.0, 0.5);

                    glTranslatef(0.5, 0.1, -0.7);

                    glTranslatef(0, Entities[j].hand_right_c, Entities[j].hand_right_z);
                    glRotatef(Entities[j].hand_right, 1,0,0);

                    glVertexPointer(3, GL_FLOAT, 0, entity_man_hand);

                    glTexCoordPointer(2, GL_FLOAT, 0, man_left_hand_texture);
                    glDrawElements(GL_TRIANGLES, entity_man_head_ind_cnt_1, GL_UNSIGNED_INT, entity_man_head_ind_1);

                    glTexCoordPointer(2, GL_FLOAT, 0, man_left_hand_texture_2);
                    glDrawElements(GL_TRIANGLES, entity_man_head_ind_cnt_2, GL_UNSIGNED_INT, entity_man_head_ind_2);

                    glTexCoordPointer(2, GL_FLOAT, 0, man_left_hand_texture_3);
                    glDrawElements(GL_TRIANGLES, entity_man_head_ind_cnt_3, GL_UNSIGNED_INT, entity_man_head_ind_3);

                    glTexCoordPointer(2, GL_FLOAT, 0, man_left_hand_texture_4);
                    glDrawElements(GL_TRIANGLES, entity_man_head_ind_cnt_4, GL_UNSIGNED_INT, entity_man_head_ind_4);

                    glTexCoordPointer(2, GL_FLOAT, 0, man_left_hand_up_texture);
                    glDrawElements(GL_TRIANGLES, entity_man_head_up_cnt, GL_UNSIGNED_INT, entity_man_head_ind_up);

                    glTranslatef(0.0, 0.0, -0.7);
                    glTexCoordPointer(2, GL_FLOAT, 0, man_left_hand_down_texture);
                    glDrawElements(GL_TRIANGLES, entity_man_head_up_cnt, GL_UNSIGNED_INT, entity_man_head_ind_up);

                    glTranslatef(0.0, 0.0, 0.7);

                    glRotatef(-Entities[j].hand_right, 1,0,0);
                    glTranslatef(0, -Entities[j].hand_right_c, -Entities[j].hand_right_z);

                    glTranslatef(-0.75, 0, 0);

                    glTranslatef(0, Entities[j].hand_left_c, Entities[j].hand_left_z);
                    glRotatef(Entities[j].hand_left, 1,0,0);


                    glTexCoordPointer(2, GL_FLOAT, 0, man_right_hand_texture);
                    glDrawElements(GL_TRIANGLES, entity_man_head_ind_cnt_1, GL_UNSIGNED_INT, entity_man_head_ind_1);

                    glTexCoordPointer(2, GL_FLOAT, 0, man_right_hand_texture_2);
                    glDrawElements(GL_TRIANGLES, entity_man_head_ind_cnt_2, GL_UNSIGNED_INT, entity_man_head_ind_2);

                    glTexCoordPointer(2, GL_FLOAT, 0, man_right_hand_texture_3);
                    glDrawElements(GL_TRIANGLES, entity_man_head_ind_cnt_3, GL_UNSIGNED_INT, entity_man_head_ind_3);

                    glTexCoordPointer(2, GL_FLOAT, 0, man_right_hand_texture_4);
                    glDrawElements(GL_TRIANGLES, entity_man_head_ind_cnt_4, GL_UNSIGNED_INT, entity_man_head_ind_4);

                    glTexCoordPointer(2, GL_FLOAT, 0, man_right_hand_up_texture);
                    glDrawElements(GL_TRIANGLES, entity_man_head_up_cnt, GL_UNSIGNED_INT, entity_man_head_ind_up);

                    glTranslatef(0.0, 0.0, -0.7);
                    glTexCoordPointer(2, GL_FLOAT, 0, man_right_hand_down_texture);
                    glDrawElements(GL_TRIANGLES, entity_man_head_up_cnt, GL_UNSIGNED_INT, entity_man_head_ind_up);
                glPopMatrix();
            }
            EntityAI(j);
        }
        for(int q = 0; q < 100; q++)
        {
            if(Sprites[q].block_id != 0)
            {
                for(int e = 0; e < 10; e++)
                {
                    glVertexPointer(3, GL_FLOAT, 0, sprite_vertex);
                    glNormalPointer(GL_FLOAT, 0, normal);
                    glNormal3f(0,0,1);
                    glColor3f(0.7, 0.7, 0.7);
                    if(Sprites[q].block_id == 1) glBindTexture(GL_TEXTURE_2D, cobblestone_texture);
                    if(Sprites[q].block_id == 2) glBindTexture(GL_TEXTURE_2D, grass_top_texture);
                    if(Sprites[q].block_id == 3) glBindTexture(GL_TEXTURE_2D, dirt_texture);
                    if(Sprites[q].block_id == 4) glBindTexture(GL_TEXTURE_2D, planks_texture);
                    if(Sprites[q].block_id == 5) glBindTexture(GL_TEXTURE_2D, stone_texture);
                    if(Sprites[q].block_id == 6) glBindTexture(GL_TEXTURE_2D, sapling_texture);
                    if(e == 0 || e == 3 || e == 9) glTexCoordPointer(2, GL_FLOAT, 0, sprite_texture);
                    else if(e == 1 || e == 4 || e == 8) glTexCoordPointer(2, GL_FLOAT, 0, sprite_2_texture);
                    else glTexCoordPointer(2, GL_FLOAT, 0, sprite_3_texture);
                    glPushMatrix();
                        glTranslatef(Sprites[q].x[e], Sprites[q].y[e], Sprites[q].z[e]);
                        glDrawElements(GL_TRIANGLES, blockIncCnt, GL_UNSIGNED_INT, blockInd);
                        glTexCoordPointer(2, GL_FLOAT, 0, block_texture_uad);
                        glDrawElements(GL_TRIANGLES, blockIncCnt_uad, GL_UNSIGNED_INT, block_Ind_uad);
                    glPopMatrix();
                }
            }
        }
        glDisableClientState(GL_NORMAL_ARRAY);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        glDisableClientState(GL_VERTEX_ARRAY);
    glPopMatrix();
    draw_cnt = dcnt;
}

void ShowDebugInfo()
{
    static float framesPerSecond = 0.0f;
    static float lastTime = 0.0f;
    static float fps = 0.0f;
    float currentTime = GetTickCount() * 0.001f;
    ++framesPerSecond;
    if(currentTime - lastTime > 1.0f)
    {
        lastTime = currentTime;
        fps = framesPerSecond;
        framesPerSecond = 0;
        draw_cnt = 0;
    }
    system("cls");
    printf("FPS: %d\nBlocks rendered: %d\nis_jumping = %s\n", (int)fps, draw_cnt, is_jumping ? "TRUE" : "FALSE");
}

void ClientToOpenGL(int x, int y, double *ox, double *oy, double *oz)
{
    int vp[4];
    double mMatrix[16], pMatrix[16];
    float z;

    glGetIntegerv(GL_VIEWPORT, vp);
    y = vp[3] - y - 1;

    glGetDoublev(GL_MODELVIEW_MATRIX, mMatrix);
    glGetDoublev(GL_PROJECTION_MATRIX, pMatrix);
    glReadPixels(x, y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &z);
    gluUnProject(x, y, z, mMatrix, pMatrix, vp, ox, oy, oz);
}

int PlayerSetBlock(BOOL create)
{
    float sz = 0.1;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-scrKoef*sz,scrKoef*sz, -sz, sz, sz*2, 100);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    if(camera.x < 0 || camera.x >= 256 || camera.y < 0 || camera.y >= 256 || camera.z < 0 || camera.z >= 256) return FALSE;
    double bx, by, bz;
    int X, Y, Z;
    glPushMatrix();
        Camera_Apply();
        ClientToOpenGL(scrSize.x/2, scrSize.y/2, &bx, &by, &bz);
        int chunkx = bx / 16;
        int chunky = by / 16;
        int dcx = 16 * chunkx;
        int dcy = 16 * chunky;
        X = bx - dcx;
        Y = by - dcy;
        Z = bz;

        if(chunkx <= 15 && chunkx >= 0 && chunky <= 15 && chunky >= 0)
        {
            if(create == FALSE)
            {
                for(int i = 0; i < 100; i++)
                {
                    if(Sprites[i].block_id == 0)
                    {
                        int a = 0;
                        for(int zf = 0; zf < 3; zf++)
                        {
                            for(int xf = 0; xf < 3; xf++)
                            {
                                Sprites[i].x[a] = (int)X + (0.3 * xf) + dcx;
                                Sprites[i].y[a] = (int)Y + dcy;
                                Sprites[i].z[a] = (int)Z + (0.3 * zf);
                                a++;
                            }
                        }

                        Sprites[i].block_id = world[chunkx][chunky][X][Y][Z];
                        Sprites[i].step = 0;

                        break;
                    }
                }
                world[chunkx][chunky][X][Y][Z] = 0;
            }
            else
            {
                bx += (camera.x - bx) / 10.0;
                by += (camera.y - by) / 10.0;
                bz += (camera.z - bz) / 10.0;
                chunkx = bx / 16;
                chunky = by / 16;
                dcx = 16 * chunkx;
                dcy = 16 * chunky;
                X = bx - dcx;
                Y = by - dcy;
                Z = bz;
                if(world[chunkx][chunky][X][Y][Z] == 0)
                {
                    if(select_inv == 1) world[chunkx][chunky][X][Y][Z] = 5;
                    if(select_inv == 2) world[chunkx][chunky][X][Y][Z] = 3;
                    if(select_inv == 3) world[chunkx][chunky][X][Y][Z] = 1;
                    if(select_inv == 4) world[chunkx][chunky][X][Y][Z] = 4;
                    if(select_inv == 6) world[chunkx][chunky][X][Y][Z] = 6;
                }
            }
        }
    glPopMatrix();
}

int GetBlockID(int x, int y, int z)
{
    if(x < 0 || x >= 256 || y < 0 || y >= 256 || z < 0 || z >= 256) return 0;
    int chunkx = x / 16;
    int chunky = y / 16;
    int dcx = 16*chunkx;
    int dcy = 16*chunky;
    int fx = x - dcx;
    int fy = y - dcy;
    return world[chunkx][chunky][fx][fy][z];
}

int SetBlock(int id, int x, int y, int z)
{
    if(x < 0 || x >= 256 || y < 0 || y >= 256 || z < 0 || z >= 256) return 0;
    int chunkx = x / 16;
    int chunky = y / 16;
    int dcx = 16*chunkx;
    int dcy = 16*chunky;
    int fx = x - dcx;
    int fy = y - dcy;
    world[chunkx][chunky][fx][fy][z] = id;
}

BOOL DirectoryExists(const char* absolutePath)
{
    if(_access(absolutePath, 0) == 0)
    {
        struct stat status;
        stat(absolutePath, &status);
        return(status.st_mode & S_IFDIR) != 0;
    }
    return FALSE;
}

void SaveWorld()
{
    TCHAR buffer[MAX_PATH];
    GetCurrentDirectory(sizeof(buffer), buffer);
    char path[MAX_PATH];
    sprintf(path, "%s\\saves", buffer);
    if(!DirectoryExists(path)) mkdir(path);
    sprintf(path, "%s\\saves\\world", buffer);
    if(!DirectoryExists(path)) mkdir(path);
    FILE *level;
    FILE *file_world;
    FILE *file_version;

    sprintf(path, "%s\\saves\\world\\level.dat", buffer);

    level = fopen(path, "w");

    if(level == NULL) creat(path, S_IREAD|S_IWRITE);

    level = fopen(path, "w");

    fwrite(&camera, 1, sizeof(camera), level);
    fclose(level);

    sprintf(path, "%s\\saves\\world\\world.ocw", buffer);

    file_world = fopen(path, "w");

    if(file_world == NULL) creat(path, S_IREAD|S_IWRITE);

    level = fopen(path, "w");

    fwrite(world, 1, sizeof(world), file_world);
    fclose(file_world);

    sprintf(path, "%s\\saves\\world\\version.dat", buffer);

    file_version = fopen(path, "w");

    if(file_version == NULL) creat(path, S_IREAD|S_IWRITE);

    file_version = fopen(path, "w");

    char version[] = OPENCRAFT_VERSION;

    fwrite(version, 1, sizeof(version), file_version);
    fclose(file_version);
}

void EntityAI(int j)
{
    int random = rand();
    random %= 200;
    if(random == 1 && Entities[j].Zrotplus == 0)
    {
        int z = rand();
        z %= 360;
        float x = z;
        Entities[j].Zrotplus = x;
    }

    int random2 = rand();
    random2 %= 200;
    if(random2 == 1 && Entities[j].is_jumping == 0)
    {
        Entities[j].is_jumping = 1;
        Entities[j].z_in_jump = Entities[j].z;
    }
    if(Entities[j].Zrotplus != 0)
    {
        if(Entities[j].Zrotplus > Entities[j].Zrot) Entities[j].Zrot++;
        if(Entities[j].Zrotplus < Entities[j].Zrot) Entities[j].Zrot--;
        if(Entities[j].Zrotplus == Entities[j].Zrot) Entities[j].Zrotplus = 0;
    }

    if(Entities[j].anim_step == 0)
    {
        Entities[j].leg_left += 1.0;
        Entities[j].leg_left_c += 0.016;
        Entities[j].leg_left_z += 0.006;
        Entities[j].leg_right -= 1.0;
        Entities[j].leg_right_c -= 0.016;
        Entities[j].leg_right_z += 0.006;

        Entities[j].hand_right -= 1.0;
        Entities[j].hand_right_c -= 0.0106;
        Entities[j].hand_right_z += 0.003;

        Entities[j].hand_left += 1.0;
        Entities[j].hand_left_c += 0.0106;
        Entities[j].hand_left_z -= 0.003;

        if(Entities[j].leg_left == 30) Entities[j].anim_step = 1;
    }
    if(Entities[j].anim_step == 1)
    {
        Entities[j].leg_left -= 1.0;
        Entities[j].leg_left_c -= 0.016;
        Entities[j].leg_left_z -= 0.006;
        Entities[j].leg_right += 1.0;
        Entities[j].leg_right_c += 0.016;
        Entities[j].leg_right_z -= 0.006;

        Entities[j].hand_right += 1.0;
        Entities[j].hand_right_c += 0.0106;
        Entities[j].hand_right_z -= 0.003;

        Entities[j].hand_left -= 1.0;
        Entities[j].hand_left_c -= 0.0106;
        Entities[j].hand_left_z += 0.003;
        if(Entities[j].leg_left == 0) Entities[j].anim_step = 2;
    }
    if(Entities[j].anim_step == 2)
    {
        Entities[j].leg_left -= 1.0;
        Entities[j].leg_left_c -= 0.016;
        Entities[j].leg_left_z += 0.006;
        Entities[j].leg_right += 1.0;
        Entities[j].leg_right_c += 0.016;
        Entities[j].leg_right_z += 0.006;

        Entities[j].hand_right += 1.0;
        Entities[j].hand_right_c += 0.0106;
        Entities[j].hand_right_z -= 0.003;

        Entities[j].hand_left -= 1.0;
        Entities[j].hand_left_c -= 0.0106;
        Entities[j].hand_left_z += 0.003;
        if(Entities[j].leg_left == -30) Entities[j].anim_step = 3;
    }
    if(Entities[j].anim_step == 3)
    {
        Entities[j].leg_left += 1.0;
        Entities[j].leg_left_c += 0.016;
        Entities[j].leg_left_z -= 0.006;
        Entities[j].leg_right -= 1.0;
        Entities[j].leg_right_c -= 0.016;
        Entities[j].leg_right_z -= 0.006;

        Entities[j].hand_right -= 1.0;
        Entities[j].hand_right_c -= 0.0106;
        Entities[j].hand_right_z += 0.003;

        Entities[j].hand_left += 1.0;
        Entities[j].hand_left_c += 0.0106;
        Entities[j].hand_left_z -= 0.003;
        if(Entities[j].leg_left == 0) Entities[j].anim_step = 0;
    }

    float ugol = -Entities[j].Zrot / 180 * M_PI;

    ugol += 0 > 0 ? M_PI_4 : (0 < 0 ? -M_PI_4 : 0);
    float new_cx = Entities[j].x + (sin(ugol) * 0.05);
    float new_cy = Entities[j].y + (cos(ugol) * 0.05);
    int chunkx = new_cx / 16;
    int chunky = new_cy / 16;
    int dcx = 16*chunkx;
    int dcy = 16*chunky;
    float x = new_cx - dcx;
    float y = new_cy - dcy;
    if(new_cx < 0 || new_cy < 0 || new_cx >= 256 || new_cy >= 256 || Entities[j].z <= -1 || Entities[j].z >= 256)
    {
        Entities[j].x = new_cx;
        Entities[j].y = new_cy;
        if(new_cx <= 256.3 && new_cx >= 256 && Entities[j].z < 45) Entities[j].x = 256.3;
        if(new_cy <= 256.3 && new_cy >= 256 && Entities[j].z < 45) Entities[j].y = 256.3;
        if(new_cx >= -0.3 && new_cx < 0 && Entities[j].z < 45) Entities[j].x = -0.4;
        if(new_cy >= -0.3 && new_cy < 0 && Entities[j].z < 45) Entities[j].y = -0.4;
    }
    else if(world[chunkx][chunky][(int)x][(int)y][(int)Entities[j].z] == 0 || world[chunkx][chunky][(int)x][(int)y][(int)Entities[j].z] == 6)
    {
        if(world[chunkx][chunky][(int)x][(int)y][(int)Entities[j].z+1] == 0 || world[chunkx][chunky][(int)x][(int)y][(int)Entities[j].z+1] == 6)
        {
            float ccx = (int)new_cx;
            float ccy = (int)new_cy;
            if(GetBlockID((int)ccx - 1, (int)new_cy, (int)Entities[j].z) != 6)
            {
                if(new_cx < Entities[j].x && GetBlockID((int)ccx - 1, (int)new_cy, (int)Entities[j].z) != 0)
                {
                    if(new_cx <= ccx + 0.3) new_cx = ccx + 0.3;
                }
            }
            if(GetBlockID((int)ccx + 1, (int)new_cy, (int)Entities[j].z) != 6)
            {
                if(new_cx > Entities[j].x && GetBlockID((int)ccx + 1, (int)new_cy, (int)Entities[j].z) != 0)
                {
                    if(new_cx >= ccx - 0.3) new_cx = ccx + 0.3;
                }
            }
            if(GetBlockID((int)ccx, (int)new_cy - 1, (int)Entities[j].z) != 6)
            {
                if(new_cy < Entities[j].y && GetBlockID((int)ccx, (int)new_cy - 1, (int)Entities[j].z) != 0)
                {
                    if(new_cy <= ccy + 0.3) new_cy = ccy + 0.3;
                }
            }
            if(GetBlockID((int)ccx, (int)new_cy + 1, (int)Entities[j].z) != 6)
            {
                if(new_cy > Entities[j].y && GetBlockID((int)ccx, (int)new_cy + 1, (int)Entities[j].z) != 0)
                {
                    if(new_cy >= ccy + 0.3) new_cy = ccy + 0.3;
                }
            }

            if(new_cx < Entities[j].x && GetBlockID((int)ccx - 1, (int)new_cy, (int)Entities[j].z + 1) != 0)
            {
                if(new_cx <= ccx + 0.3) new_cx = ccx + 0.3;
            }
            if(new_cx > Entities[j].x && GetBlockID((int)ccx + 1, (int)new_cy, (int)Entities[j].z + 1) != 0)
            {
                if(new_cx >= ccx - 0.3) new_cx = ccx + 0.3;
            }
            if(new_cy < Entities[j].y && GetBlockID((int)new_cx, (int)ccy - 1, (int)Entities[j].z + 1) != 0)
            {
                if(new_cy <= ccy + 0.3) new_cy = ccy + 0.3;
            }
            if(new_cy > Entities[j].y && GetBlockID((int)new_cx, (int)ccy + 1, (int)Entities[j].z + 1) != 0)
            {
                if(new_cy >= ccy - 0.3) new_cy = ccy + 0.3;
            }
            Entities[j].x = new_cx;
            Entities[j].y = new_cy;
        }
    }
    if(Entities[j].x <= -0.3 || Entities[j].y <= -0.3 || Entities[j].x >= 256 || Entities[j].y >= 256 || Entities[j].z <= -0.01 || Entities[j].z >= 256)
    {
        Entities[j].z -= 0.3;
    }
    else if(GetBlockID((int)Entities[j].x, (int)Entities[j].y, (int)Entities[j].z-1) == 0 || GetBlockID((int)Entities[j].x, (int)Entities[j].y, (int)Entities[j].z-1) == 6)
    {
        if(Entities[j].is_jumping == 0)Entities[j].z -= 0.3;
    }
    float old_z = Entities[j].z;
    if(GetBlockID((int)Entities[j].x, (int)Entities[j].y, (int)Entities[j].z-1) != 0 && (int)Entities[j].z < Entities[j].z && Entities[j].is_jumping == 0 && GetBlockID((int)Entities[j].x, (int)Entities[j].y, (int)Entities[j].z-1) != 6)
    {
        Entities[j].z -= 0.3;
        if(Entities[j].z < (int)old_z) Entities[j].z = (int)old_z;
    }
    if(Entities[j].is_jumping)
    {
        float ste = 2 * jump_go[Entities[j].jump_tmp];
        Entities[j].z = (Entities[j].z_in_jump - 2)+(ste);
        if(Entities[j].x >= 0 && Entities[j].x < 256 && Entities[j].y >= 0 && Entities[j].y < 256 && Entities[j].z >= 0 && Entities[j].z < 256)
        {
            if(Entities[j].jump_down == 0) Entities[j].jump_tmp++;
            else Entities[j].jump_tmp--;
            if(Entities[j].jump_tmp == 9) Entities[j].jump_down = 1;
            if(Entities[j].jump_tmp == 0 && Entities[j].jump_down == 1)
            {
                Entities[j].is_jumping = 0;
                Entities[j].jump_down = 0;
                Entities[j].jump_tmp = 0;
            }
            if(Entities[j].jump_down == 1 && GetBlockID((int)Entities[j].x, (int)Entities[j].y, (int)Entities[j].z) != 0 && GetBlockID((int)Entities[j].x, (int)Entities[j].y, (int)Entities[j].z-1) != 6)
            {
                Entities[j].is_jumping = 0;
                Entities[j].jump_down = 0;
                Entities[j].jump_tmp = 0;
            }
            if(Entities[j].jump_down == 0 && GetBlockID((int)Entities[j].x, (int)Entities[j].y, (int)Entities[j].z+2) != 0)
            {
                Entities[j].is_jumping = 0;
                Entities[j].jump_down = 0;
                Entities[j].jump_tmp = 0;
            }
        }
        else
        {
            Entities[j].is_jumping = 0;
            Entities[j].jump_down = 0;
            Entities[j].jump_tmp = 0;
        }
    }
}

void SpritesManage()
{
    for(int i = 0; i < 100; i++)
    {
        if(Sprites[i].block_id != 0)
        {
            Sprites[i].x[0] -= 0.05;
            Sprites[i].z[0] += 0.05;
            Sprites[i].z[1] += 0.05;
            Sprites[i].x[2] += 0.05;
            Sprites[i].z[2] += 0.05;
            Sprites[i].x[3] -= 0.05;
            Sprites[i].y[4] -= 0.05;
            Sprites[i].x[5] += 0.05;

            Sprites[i].x[6] -= 0.05;
            Sprites[i].z[6] -= 0.05;
            Sprites[i].z[7] -= 0.05;
            Sprites[i].x[8] += 0.05;
            Sprites[i].z[8] -= 0.05;

            Sprites[i].step++;
            if(Sprites[i].step >= 30) Sprites[i].block_id = 0;
        }
    }
}

void Menu_Show()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, scrSize.x, scrSize.y, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);

    int curx = scrSize.x / 2 - 16;
    int cury = scrSize.y / 2 - 16;

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glVertexPointer(2, GL_FLOAT, 0, cursor);
    glTexCoordPointer(2, GL_FLOAT, 0, cursor_texture);
    glBindTexture(GL_TEXTURE_2D, icons_texture);
    glPushMatrix();
        glTranslatef(curx, cury, 0);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glPopMatrix();

    curx = scrSize.x - 64 - 5;
    cury = 20;

    if(select_inv == 1) glBindTexture(GL_TEXTURE_2D, stone_texture);
    if(select_inv == 2) glBindTexture(GL_TEXTURE_2D, dirt_texture);
    if(select_inv == 3) glBindTexture(GL_TEXTURE_2D, cobblestone_texture);
    if(select_inv == 4) glBindTexture(GL_TEXTURE_2D, planks_texture);
    if(select_inv == 6) glBindTexture(GL_TEXTURE_2D, sapling_texture);

    if(select_inv != 6) glVertexPointer(2, GL_FLOAT, 0, block_inventory);
    else glVertexPointer(2, GL_FLOAT, 0, block_inventory_flat);
    glTexCoordPointer(2, GL_FLOAT, 0, block_inventory_texture);
    glPushMatrix();
        if(select_inv != 6) glTranslatef(curx, cury, 0);
        else glTranslatef(curx, cury-16, 0);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glPopMatrix();

    if(select_inv != 6)
    {
        glVertexPointer(2, GL_FLOAT, 0, block_inventory_2);
        glTexCoordPointer(2, GL_FLOAT, 0, block_inventory_texture);
        glPushMatrix();
            glTranslatef(curx, cury, 0);
            glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
        glPopMatrix();

        glVertexPointer(2, GL_FLOAT, 0, block_inventory_3);
        glTexCoordPointer(2, GL_FLOAT, 0, block_inventory_texture);
        glPushMatrix();
            glTranslatef(curx, cury, 0);
            glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
        glPopMatrix();
    }

    glPushMatrix();
        glTranslatef(0,0,0);
        Text_Out(OPENCRAFT_VERSION);
    glPopMatrix();

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);

    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
}

int avgnoov(int si_a, int si_b)
{
    if ((si_b > 0) && (si_a > (INT_MAX - si_b)))
    {
        if (si_a >= si_b) return si_b + (si_a - si_b) / 2;
        else return si_a + (si_b - si_a) / 2;
    }
    else if ((si_b < 0) && (si_a < (INT_MIN - si_b)))
    {
        if(si_a <= si_b) return si_b + (si_a - si_b) / 2;
        else return si_a + (si_b - si_a) / 2;
    }
    else
    {
        return (si_a + si_b) / 2;
    }
}

void Text_Out(char *text)
{
    glPushMatrix();
        glBindTexture(GL_TEXTURE_2D, ascii_texture);
        glVertexPointer(2, GL_FLOAT, 0, rectCoord);
        glTexCoordPointer(2, GL_FLOAT, 0, rectTex);

        static float charSize = 1/16.0;
        while(*text)
        {
            char c = *text;
            int y = c >> 4;
            int x = c & 0b1111;
            struct {float left, right, top, bottom} rct;
            rct.left = x * charSize;
            rct.right = rct.left + charSize;
            rct.bottom = y * charSize;
            rct.top = rct.bottom + charSize;

            rectTex[0] = rectTex[6] = rct.left;
            rectTex[2] = rectTex[4] = rct.right;
            rectTex[1] = rectTex[3] = rct.bottom;
            rectTex[5] = rectTex[7] = rct.top;

            glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
            text++;
            glTranslatef(10,0,0);
        }
    glPopMatrix();
}

void EnableOpenGL(HWND hwnd, HDC* hDC, HGLRC* hRC)
{
    PIXELFORMATDESCRIPTOR pfd;

    int iFormat;

    *hDC = GetDC(hwnd);

    ZeroMemory(&pfd, sizeof(pfd));

    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW |
                  PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24;
    pfd.cDepthBits = 16;
    pfd.iLayerType = PFD_MAIN_PLANE;

    iFormat = ChoosePixelFormat(*hDC, &pfd);

    SetPixelFormat(*hDC, iFormat, &pfd);

    *hRC = wglCreateContext(*hDC);

    wglMakeCurrent(*hDC, *hRC);
}

void DisableOpenGL (HWND hwnd, HDC hDC, HGLRC hRC)
{
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(hRC);
    ReleaseDC(hwnd, hDC);
}

int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow)
{
    WNDCLASSEX wcex;
    HWND hwnd;
    HDC hDC;
    HGLRC hRC;
    MSG msg;
    BOOL bQuit = FALSE;

    /* register window class */
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_OWNDC;
    wcex.lpfnWndProc = WindowProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = "Opencraft";
    wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);;


    if (!RegisterClassEx(&wcex))
        return 0;

    HDC hDCScreen = GetDC(NULL);
    int Horres = GetDeviceCaps(hDCScreen, HORZRES);
    int Vertres = GetDeviceCaps(hDCScreen, VERTRES);
    ReleaseDC(NULL, hDCScreen);

    char title[50];
    sprintf(title, "Opencraft %s", OPENCRAFT_VERSION);

    hwnd = CreateWindowEx(0,
                          "Opencraft",
                          title,
                          WS_OVERLAPPEDWINDOW,
                          CW_USEDEFAULT,
                          CW_USEDEFAULT,
                          800,
                          600,
                          NULL,
                          NULL,
                          hInstance,
                          NULL);

    ShowWindow(hwnd, nCmdShow);

    /* enable OpenGL for the window */
    EnableOpenGL(hwnd, &hDC, &hRC);

    RECT rct;
    GetClientRect(hwnd, &rct);
    WndResize(rct.right, rct.bottom);

    Game_Init();

    /* program main loop */
    while (!bQuit)
    {
        /* check for messages */
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            /* handle or dispatch messages */
            if (msg.message == WM_QUIT)
            {
                bQuit = TRUE;
            }
            else
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        else
        {
            glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            glPushMatrix();
                if (GetForegroundWindow() == hwnd) Player_Move();
                Game_Show();
                Menu_Show();

            glPopMatrix();

            SwapBuffers(hDC);

            //ShowDebugInfo();

            SpritesManage();

            theta += 0,1080000108;

            timer--;
            timer_r--;
            timer_g--;
            timer_y--;
        }
    }

    /* shutdown OpenGL */
    DisableOpenGL(hwnd, hDC, hRC);

    /* destroy the window explicitly */
    DestroyWindow(hwnd);

    return msg.wParam;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_CLOSE:
            SaveWorld();
            PostQuitMessage(0);
        break;

        case WM_SIZE:
            WndResize(LOWORD(lParam), HIWORD(lParam));
        break;

        case WM_SETCURSOR:
            ShowCursor(FALSE);
        break;

        case WM_RBUTTONDOWN:
            PlayerSetBlock(FALSE);
        break;

        case WM_LBUTTONDOWN:
            PlayerSetBlock(TRUE);
        break;

        case WM_DESTROY:
            return 0;

        case WM_KEYDOWN:
        {
            switch (wParam)
            {
                case VK_ESCAPE:
                    SaveWorld();
                    PostQuitMessage(0);
                break;
            }
        }
        break;

        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    return 0;
}
