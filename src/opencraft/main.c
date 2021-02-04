#define STB_IMAGE_IMPLEMENTATION

#define SAVE_GAME TRUE /* Если Вы пишите код, ставьте FALSE, чтобы экономить время сохранения и загрузки мира */

#include "../libs/stb_image/stb_image.h"
#include "../libs/zlib/zlib.h"

#include "blocks.h"

#include <windows.h>
#include <direct.h>
#include <gl/glu.h>
#include <io.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>

#include "camera.h"

#define OPENCRAFT_VERSION "0.0.14a"

#define GAME_GENLWORLD 0
#define GAME_PAUSE 1
#define GAME_PLAY 2

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
int menu_gradient_texture;
int widgets_texture;

BOOL DirectoryExists(const char* absolutePath);

float camera_z_in_jump;

int timer = 0;
int timer_r = 0;
int timer_g = 0;
int timer_y = 0;
int timer_f = 0;
int timer_n = 0;
int timer_pl = 0;

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
GLfloat fogColorLava[4] = {1, 0, 0, 1};
GLfloat fogColorWater[4] = {0, 0, 1, 1};

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

float d2_coord[] = {0,0, 1,0, 1,1, 0,1};

BOOL world_visible[32][32][16][16][256];

int render_distance = 1;

int tmp = 0;

int menu_page = 0;

typedef struct{
    float x,y,z;
    int block_id;
} SLava;
SLava LavaVisible[67108864];

int lava_req = 0;

int update_chunks = 0;
int timer_del_chunks = 60;

BOOL chunk_update[32][32];

BOOL afk = FALSE;

BOOL place_blocks = FALSE;

RECT rcta;
RECT rctb;

HDC hDC;

int GameState = GAME_GENLWORLD;

static WORD xwmmove, ywmmove;

float button[] = {0,0, 405,0, 405,45, 0,45};

float lava_UV[] = {0,0, 1,1, 0,1, 1,1, 0,0, 1,0, 0,0, 1,0};
float lava_UV_2[] = {0,0.0, 1,1, 0,1, 1,1, 0,0, 1,0, 0,0, 1,0};
float lava_UV_3[] = {0,1, 1,1, 0,1, 1,1, 0,0, 1,0, 0,0, 1,0};
float lava_UV_4[] = {0,1, 1,1, 0,1, 1,1, 0,0, 1,0, 0,0, 1,0};
float lava_UV_5[] = {0,1, 1,1, 1,0, 0,0, 0,1, 1,1, 1,0, 0,0};
float lava_UV_updn[] = {0,1, 1,1, 1,0, 0,0, 0,1, 1,1, 1,0, 0,0};

float button_coord_closed[] = {0,0.18, 0.78,0.18, 0.78,0.26, 0,0.26};
float button_coord[] = {0,0.26, 0.78,0.26, 0.78,0.335, 0,0.335};
float button_coord_active[] = {0,0.338, 0.78,0.338, 0.78,0.413, 0,0.413};

typedef struct {
    int x,y;
    int xe, ye;
    int state;
} SButtons;

SButtons Buttons[10];

BOOL cursorShow = FALSE;

BOOL map_changed = FALSE;

BOOL space = FALSE;

BOOL lbutton_timer = FALSE;

int worldsizex, worldsizey = 0;

typedef struct {
    int x, y, z;
    int id;
} SWorld;

SWorld *slworld = NULL;
SWorld slwmemcheck[1];

int cxtmp = 0;
int cytmp = 0;
int dcxtmp = 0;
int dcytmp = 0;

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
            if(x == 0 && y == 0 && chunkx == 0 && chunky == 0) coolz = 62 + rand() % 5;
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
                if(z == 0) world[chunkx][chunky][x][y][z] = 9;
                else if(z < coolz && z - coolz >= -3) world[chunkx][chunky][x][y][z] = 3;
                else if(z < coolz && z - coolz < -3) world[chunkx][chunky][x][y][z] = 5;
                else if(z == coolz && z <= 50) world[chunkx][chunky][x][y][z] = 10;
                else if(z == coolz && z > 50) world[chunkx][chunky][x][y][z] = 2;
                else if(z > coolz) world[chunkx][chunky][x][y][z] = 0;
                oldcoolz = coolz;
            }
        }
    }
}

void GenerateCaves()
{
    GenMenu_Show("Генерация пещер...", sizeof("Генерация пещер..."), TRUE);
    SwapBuffers(hDC);
    int g3 = 0;
    for(int y = 0; y < worldsizey; y++)
    {
        for(int x = 0; x < worldsizex; x++)
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
                        for(int zz = zx; zz >= 5; zz--)
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
                        if(g3 >= 7) return;
                    }
                }
            }
        }
    }
}

void GenerateWaterAndLava()
{
    GenMenu_Show("Генерация воды и лавы...", sizeof("Генерация воды и лавы..."), TRUE);
    SwapBuffers(hDC);
    int z = 0;
    for(int y = 0; y < worldsizey; y++)
    {
        for(int x = 0; x < worldsizex; x++)
        {
            int randomcool = rand();
            randomcool %= 10000;
            if(randomcool == 9999)
            {
                for(int zx = 255; zx >= 0; zx--)
                {
                    if(GetBlockID(x, y, zx) != 0)
                    {
                        z = zx;
                        break;
                    }
                }
                int coolrde = rand() % 100;
                int blocks = 0;
                if(coolrde <= 70) blocks = 8;
                else
                {
                    if(z <= 64) blocks = 7;
                    else continue;
                }
                int xvr = rand() % 10;
                int yvr = rand() % 10;
                for(int xv = 0; xv < xvr; xv++)
                {
                    for(int yv = 0; yv < yvr; yv++)
                    {
                        if(xvr < 5) xvr = 5;
                        if(yvr < 5) yvr = 5;
                        for(int zx = 255; zx >= 0; zx--)
                        {
                            if(GetBlockID(x+xv, y+yv, zx) != 0)
                            {
                                z = zx;
                                break;
                            }
                        }
                        SetBlock(0, x+xv, y+yv, z);
                        SetBlock(0, x+xv, y+yv, z-1);
                        SetBlock(blocks, x+xv, y+yv, z-2);
                        SetBlock(0, x+xv, y+yv, z-3);
                        SetBlock(0, x+xv, y+yv, z-4);
                    }
                }
                for(int z = 0; z < 256; z++)
                {
                    for(int xv = 0; xv < xvr; xv++)
                    {
                        for(int yv = 0; yv < yvr; yv++)
                        {
                            if(GetBlockID(x+xv, y+yv, z) == 7 || GetBlockID(x+xv, y+yv, z) == 8)
                            {
                                int block_g = GetBlockID(x+xv, y+yv, z);
                                if(GetBlockID(x+xv+1, y+yv, z) == 0 && x+xv+1 < x+xvr && y+yv < y+yvr && x+xv+1 >= x && y+yv >= y) SetBlock(block_g, x+xv+1, y+yv, z);
                                if(GetBlockID(x+xv-1, y+yv, z) == 0 && x+xv-1 < x+xvr && y+yv < y+yvr && x+xv-1 >= x && y+yv >= y) SetBlock(block_g, x+xv-1, y+yv, z);
                                if(GetBlockID(x+xv, y+yv+1, z) == 0 && x+xv < x+xvr && y+yv+1 < y+yvr && x+xv >= x && y+yv+1 >= y) SetBlock(block_g, x+xv, y+yv+1, z);
                                if(GetBlockID(x+xv, y+yv-1, z) == 0 && x+xv < x+xvr && y+yv-1 < y+yvr && x+xv >= x && y+yv-1 >= y) SetBlock(block_g, x+xv, y+yv-1, z);
                                if(GetBlockID(x+xv, y+yv, z-1) == 0 && x+xv < x+xvr && y+yv < y+yvr && x+xv >= x && y+yv >= y) SetBlock(block_g, x+xv, y+yv, z-1);
                            }
                        }
                    }
                }
            }
        }
    }
}

void GenerateAdditionalProcessing()
{
    GenMenu_Show("Дополнительная обработка...", sizeof("Дополнительная обработка..."), TRUE);
    SwapBuffers(hDC);
    for(int z = 0; z < 256; z++)
    {
        for(int y = 0; y < worldsizey; y++)
        {
            for(int x = 0; x < worldsizex; x++)
            {
                if(GetBlockID(x, y, z) == 7 || GetBlockID(x, y, z) == 8)
                {
                    int block_id = 0;
                    if(GetBlockID(x, y, z) == 7) block_id = 5;
                    else block_id = 3;
                    if(GetBlockID(x, y+1, z) == 0)
                    {
                        SetBlock(block_id, x, y+1, z);
                    }

                    if(GetBlockID(x, y-1, z) == 0)
                    {
                        SetBlock(block_id, x, y-1, z);
                    }

                    if(GetBlockID(x+1, y, z) == 0)
                    {
                        SetBlock(block_id, x+1, y, z);
                    }

                    if(GetBlockID(x-1, y, z) == 0)
                    {
                        SetBlock(block_id, x-1, y, z);
                    }
                    if(GetBlockID(x, y, z-1) == 0)
                    {
                        SetBlock(block_id, x, y, z-1);
                    }
                }
                if(z < 11 && GetBlockID(x, y, z) == 0) SetBlock(7, x, y, z);
            }
        }
    }
}

void GenerateTrees()
{
    GenMenu_Show("Генерация деревьев...", sizeof("Генерация деревьев..."), TRUE);
    SwapBuffers(hDC);
    for(int y = 0; y < worldsizey; y++)
    {
        for(int x = 0; x < worldsizex; x++)
        {
            int random = rand() % 200;
            if(random <= 1)
            {
                int z = GetHighestBlock(x, y) + 1;
                if(GetBlockID(x, y, z-1) != 2 && GetBlockID(x, y, z-1) != 3) continue;
                SetBlock(15, x, y, z);
                SetBlock(15, x, y, z+1);
                SetBlock(15, x, y, z+2);
                z += 3;
                int new_x = x - 2;
                int new_y = y - 2;
                for(int q = 0; q <= 1; q++)
                {
                    for(int xx = 0; xx < 5; xx++)
                    {
                        for(int yy = 0; yy < 5; yy++)
                        {
                            if(xx == 2 && yy == 2) SetBlock(15, new_x+xx, new_y+yy, z);
                            else SetBlock(16, new_x+xx, new_y+yy, z);
                        }
                    }
                    z += 1;
                }
                new_x += 1;
                new_y += 1;
                for(int xx = 0; xx < 3; xx++)
                {
                    for(int yy = 0; yy < 3; yy++)
                    {
                        if(xx == 1 && yy == 1) SetBlock(15, new_x+xx, new_y+yy, z);
                        else SetBlock(16, new_x+xx, new_y+yy, z);
                    }
                }
                z += 1;
                for(int xx = 0; xx < 3; xx++)
                {
                    for(int yy = 0; yy < 3; yy++)
                    {
                        if(xx == 1) SetBlock(16, new_x+xx, new_y+yy, z);
                        else if(xx == 0 && yy == 1) SetBlock(16, new_x+xx, new_y+yy, z);
                        else if(xx == 2 && yy == 1) SetBlock(16, new_x+xx, new_y+yy, z);
                    }
                }
            }
        }
    }
}

void GenerateOres()
{
    GenMenu_Show("Генерация руд...", sizeof("Генерация руд..."), TRUE);
    SwapBuffers(hDC);
    for(int z = 0; z < 256; z++)
    {
        for(int y = 0; y < worldsizey; y++)
        {
            for(int x = 0; x < worldsizex; x++)
            {
                int random = rand() % 500;
                if(random <= 1 && GetBlockID(x, y, z) == 5)
                {
                    int block = 0;
                    int randomcool = rand() % 250;
                    if(randomcool <= 150) block = 14;
                    else if(randomcool > 200 && randomcool <= 215) block = 13;
                    else block = 12;
                    SetBlock(block, x, y, z);
                    int random[6];
                    int ch = 90;
                    for(int i = 0; i < 7; i++)
                    {
                        for(int r = 0; r < 6; r++)
                        {
                            random[r] = rand() % 100;
                        }
                        if(random[0] >= ch && GetBlockID(x+1, y, z) == 5) SetBlock(block, x+1, y, z);
                        if(random[1] >= ch && GetBlockID(x-1, y, z) == 5) SetBlock(block, x-1, y, z);
                        if(random[2] >= ch && GetBlockID(x, y+1, z) == 5) SetBlock(block, x, y+1, z);
                        if(random[3] >= ch && GetBlockID(x, y-1, z) == 5) SetBlock(block, x, y-1, z);
                        if(random[4] >= ch && GetBlockID(x, y, z+1) == 5) SetBlock(block, x, y, z+1);
                        if(random[5] >= ch && GetBlockID(x, y, z-1) == 5) SetBlock(block, x, y, z-1);
                        ch -= rand() % 10;
                        int randomf = rand() % 100;
                        if(randomf <= 50) x++;
                        else x--;
                        randomf = rand() % 100;
                        if(randomf <= 50) y++;
                        else y--;
                    }
                }
            }
        }
    }
}

void GenerateNewWorld()
{
    GenMenu_Show("Генерация ландшафта...", sizeof("Генерация ландшафта..."), TRUE);
    SwapBuffers(hDC);
    int y_need = worldsizey / 16 - 1;
    int x_need = worldsizex / 16 - 1;
    for(int y = 0; y <= y_need; y++)
    {
        for(int x = 0; x <= x_need; x++)
        {
            GenerateNewChunk(x, y);
        }
    }
    GenerateTrees();
    GenerateOres();
    GenerateWaterAndLava();
    GenerateAdditionalProcessing();
    GenerateCaves();
    camera.x = worldsizex / 2 + 0.5;
    camera.y = worldsizey / 2 + 0.5;
    for(int zx = 255; zx >= 0; zx--)
    {
        if(GetBlockID((int)camera.x, (int)camera.y, zx) != 0)
        {
            camera.z = zx+1;
            break;
        }
    }
    /*for(int i = 0; i < 100; i++)
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
    }*/
    int cx = worldsizex / 16;
    int cy = worldsizey / 16;
    for(int chunky = 0; chunky < cy; chunky++)
    {
        for(int chunkx = 0; chunkx < cx; chunkx++)
        {
            UpdateChunk(chunkx, chunky);
        }
    }
    cursorShow = FALSE;
    while (ShowCursor(FALSE) >= 0);
    afk = FALSE;
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

    registerBlock(1, 1, "textures/blocks/cobblestone.png");
    registerBlock(2, 2, "textures/blocks/grass_side.png", "textures/blocks/grass_top.png", "textures/blocks/dirt.png");
    registerBlock(3, 1, "textures/blocks/dirt.png");
    registerBlock(4, 1, "textures/blocks/planks.png");
    registerBlock(5, 1, "textures/blocks/stone.png");
    registerBlock(6, 3, "textures/blocks/sapling.png");
    registerBlock(7, 4, "textures/blocks/lava.png");
    registerBlock(8, 4, "textures/blocks/water.png");
    registerBlock(9, 1, "textures/blocks/bedrock.png");
    registerBlock(10, 1, "textures/blocks/sand.png");
    registerBlock(11, 1, "textures/blocks/gravel.png");
    registerBlock(12, 1, "textures/blocks/gold_ore.png");
    registerBlock(13, 1, "textures/blocks/iron_ore.png");
    registerBlock(14, 1, "textures/blocks/coal_ore.png");
    registerBlock(15, 2, "textures/blocks/log.png", "textures/blocks/log_top.png", "textures/blocks/log_top.png");
    registerBlock(16, 5, "textures/blocks/leaves.png");

    LoadTexture("textures/entity/man.png", &man_texture);

    LoadTexture("textures/font/ascii.png", &ascii_texture);

    LoadTexture("textures/gui/icons.png", &icons_texture);
    LoadTexture("textures/gui/menu_gradient.png", &menu_gradient_texture);
    LoadTexture("textures/gui/widgets.png", &widgets_texture);
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
    glFogf(GL_FOG_START, 8.0f*render_distance);
    glFogf(GL_FOG_END, 9.0f*render_distance);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.01);

    srand(time(NULL));

    TCHAR buffer[MAX_PATH];
    GetCurrentDirectory(sizeof(buffer), buffer);
    char path[MAX_PATH];
    sprintf(path, "%s\\saves\\world", buffer);
    if(DirectoryExists(path) && SAVE_GAME == TRUE)
    {
        FILE *level;
        FILE *file_world;
        FILE *file_version;
        FILE *file_entity;
        FILE *file_borders;

        GenMenu_Show("Загрузка border.dat...", sizeof("Загрузка border.dat..."), FALSE);

        SwapBuffers(hDC);

        sprintf(path, "%s\\saves\\world\\border.dat", buffer);

        file_borders = fopen(path, "r");

        if(file_borders == NULL) creat(path, S_IREAD|S_IWRITE);

        file_borders = fopen(path, "r");

        fread(&worldsizex, 1, sizeof(worldsizex), file_borders);
        fclose(file_borders);

        worldsizey = worldsizex;

        GenMenu_Show("Загрузка level.dat...", sizeof("Загрузка level.dat..."), FALSE);

        SwapBuffers(hDC);

        sprintf(path, "%s\\saves\\world\\level.dat", buffer);

        level = fopen(path, "r");

        if(level != NULL)
        {
            fread(&camera, 1, sizeof(camera), level);
            fclose(level);
        }

        GenMenu_Show("Загрузка world.ocw...", sizeof("Загрузка world.ocw..."), FALSE);

        SwapBuffers(hDC);

        sprintf(path, "%s\\saves\\world\\world.ocw", buffer);

        file_world = fopen(path, "r");

        if(file_world != NULL)
        {
            fread(world, 1, sizeof(world), file_world);
            fclose(file_world);
        }

        char version[50];

        GenMenu_Show("Загрузка version.dat...", sizeof("Загрузка version.dat..."), FALSE);

        SwapBuffers(hDC);

        sprintf(path, "%s\\saves\\world\\version.dat", buffer);

        file_version = fopen(path, "r");

        if(file_version != NULL)
        {
            fread(version, 1, sizeof(version), file_version);
            fclose(file_version);
        }
        else
        {
            for(int chunkx = 0; chunkx < 32; chunkx++)
            {
                for(int chunky = 0; chunky < 32; chunky++)
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

        GenMenu_Show("Загрузка entity.dat...", sizeof("Загрузка entity.dat..."), FALSE);

        SwapBuffers(hDC);

        sprintf(path, "%s\\saves\\world\\entity.dat", buffer);

        file_entity = fopen(path, "rb");

        if(file_entity != NULL)
        {
            fread(Entities, sizeof(Entities[0]), 500, file_entity);
            fclose(file_entity);
        }

    }
    else
    {
        worldsizex = 512;
        worldsizey = 512;
        GenerateNewWorld();
    }

    /*for(int i = 0; i < 100; i++)
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
    }*/
    int cx = worldsizex / 16;
    int cy = worldsizey / 16;
    for(int chunky = 0; chunky < cy; chunky++)
    {
        for(int chunkx = 0; chunkx < cx; chunkx++)
        {
           UpdateChunk(chunkx, chunky);
        }
    }
    cursorShow = FALSE;
    while (ShowCursor(FALSE) >= 0);
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
    if(GetBlockID((int)camera.x, (int)camera.y, (int)camera.z) == 7 || GetBlockID((int)camera.x, (int)camera.y, (int)camera.z) == 8) Camera_MoveDirection(GetKeyState('W') < 0 ? 1: (GetKeyState('S') < 0 ? -1 : 0)
                         ,GetKeyState('D') < 0 ? 1 : (GetKeyState('A') < 0 ? -1: 0)
                         ,0.02);
    else Camera_MoveDirection(GetKeyState('W') < 0 ? 1: (GetKeyState('S') < 0 ? -1 : 0)
        ,GetKeyState('D') < 0 ? 1 : (GetKeyState('A') < 0 ? -1: 0)
        ,0.12);
    if(afk == FALSE) Camera_AutoMoveByMouse((int)rctb.left + scrSize.x/2, (int)rctb.top + scrSize.y/2, 0.2);
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

    if(GetBlockID((int)camera.x, (int)camera.y, (int)camera.z + 1) == 7 ) glClearColor(1, 0, 0, 0);
    else if(GetBlockID((int)camera.x, (int)camera.y, (int)camera.z + 1) == 8 ) glClearColor(0, 0, 1, 0);
    else glClearColor(0.6, 0.8, 1, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if(GetBlockID((int)camera.x, (int)camera.y, (int)camera.z + 1) == 7 || GetBlockID((int)camera.x, (int)camera.y, (int)camera.z + 1) == 8)
    {
        if(GetBlockID((int)camera.x, (int)camera.y, (int)camera.z + 1) == 7)
        {
            glFogf(GL_FOG_START, 1.0f);
            glFogf(GL_FOG_END, 2.0f);
        }
        else
        {
            glFogf(GL_FOG_START, 5.0f);
            glFogf(GL_FOG_END, 6.0f);
        }
        if(GetBlockID((int)camera.x, (int)camera.y, (int)camera.z + 1) == 7) glFogfv(GL_FOG_COLOR, fogColorLava);
        else glFogfv(GL_FOG_COLOR, fogColorWater);
    }
    else
    {
        glFogf(GL_FOG_START, 8.0f*render_distance);
        glFogf(GL_FOG_END, 9.0f*render_distance);
        glFogfv(GL_FOG_COLOR, fogColor);
    }
    int chunkx = camera.x / 16;
    int chunky = camera.y / 16;
    int cameraz = (int)camera.z - 1;
    int jdcx = 16*chunkx;
    int jdcy = 16*chunky;
    float xjump = camera.x - jdcx;
    float yjump = camera.y - jdcy;

    int lava_reqshow = 0;

    if(GetKeyState('1') < 0 && afk == FALSE) select_inv = 1;
    if(GetKeyState('2') < 0 && afk == FALSE) select_inv = 2;
    if(GetKeyState('3') < 0 && afk == FALSE) select_inv = 3;
    if(GetKeyState('4') < 0 && afk == FALSE) select_inv = 4;
    if(GetKeyState('6') < 0 && afk == FALSE) select_inv = 6;

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
        int cx = worldsizex / 16 - 1;
        int cy = worldsizey / 16 - 1;
        if(mychunkx > cx) mychunkx = cx-1;
        if(mychunky > cy) mychunky = cy-1;
        if(mychunky < 0) mychunky = 0;
        if(mychunkx < 0) mychunkx = 0;
        int rd = 9*render_distance;
        for(int chunky = mychunky; chunky <= mychunky + 2; chunky++)
        {
            for(int chunkx = mychunkx; chunkx <= mychunkx + 2; chunkx++)
            {
                if(chunkx > cx || chunky > cy || chunkx < 0 || chunky < 0) break;
                for(int z = 0; z <= 255; z++)
                {
                    for(int y = 0; y <= 15; y++)
                    {
                        for(int x = 0; x <= 15; x++)
                        {
                            int dcx = 16*chunkx;
                            int dcy = 16*chunky;
                            if(camera.x + rd < x + dcx || camera.x - rd > x + dcx || camera.y + rd < y + dcy || camera.y - rd > y + dcy || camera.z + rd < z || camera.z - rd > z) continue;
                            if(camera.Xrot > 20)
                            {
                                if(camera.Zrot >= 220 && camera.Zrot <= 320 && camera.x - 3 > x + dcx) continue;
                                if(camera.Zrot >= 30 && camera.Zrot <= 150 && camera.x + 3 < x + dcx) continue;
                                if(camera.Zrot >= 130 && camera.Zrot <= 230 && camera.y + 3 < y + dcy) continue;
                                if(camera.Zrot >= 300 && camera.y - 3 > y + dcy) continue;
                                if(camera.Zrot <= 50 && camera.y - 3 > y + dcy) continue;
                                if(camera.Xrot <= 50 && camera.z + 9*2 < z) continue;
                                if(camera.Xrot >= 110 && camera.z - 3 > z) continue;
                            }
                            if(!world_visible[chunkx][chunky][x][y][z]) continue;
                            if(blocks[world[chunkx][chunky][x][y][z]].type == 1)
                            {
                                glVertexPointer(3, GL_FLOAT, 0, block);
                                glNormalPointer(GL_FLOAT, 0, normal);
                                glNormal3f(0,0,1);
                                glColor3f(0.7, 0.7, 0.7);
                                glBindTexture(GL_TEXTURE_2D, blocks[world[chunkx][chunky][x][y][z]].texture[0]);
                                glTexCoordPointer(2, GL_FLOAT, 0, block_texture);
                                glPushMatrix();
                                    glTranslatef(x+dcx, y+dcy, z);
                                    if(GetBlockID(x+dcx, y+dcy+1, z) == 0 || GetBlockID(x+dcx, y+dcy+1, z) == 6 || GetBlockID(x+dcx, y+dcy+1, z) == 7 || GetBlockID(x+dcx, y+dcy+1, z) == 8 || GetBlockID(x+dcx, y+dcy+1, z) == 16)
                                    {
                                        glTexCoordPointer(2, GL_FLOAT, 0, lava_UV);
                                        glDrawElements(GL_TRIANGLES, entity_man_head_ind_cnt_1, GL_UNSIGNED_INT, entity_man_head_ind_1);
                                    }

                                    if(GetBlockID(x+dcx+1, y+dcy, z) == 0 || GetBlockID(x+dcx+1, y+dcy, z) == 6 || GetBlockID(x+dcx+1, y+dcy, z) == 7 || GetBlockID(x+dcx+1, y+dcy, z) == 8 || GetBlockID(x+dcx+1, y+dcy, z) == 16)
                                    {
                                        glTexCoordPointer(2, GL_FLOAT, 0, lava_UV_2);
                                        glDrawElements(GL_TRIANGLES, entity_man_head_ind_cnt_2, GL_UNSIGNED_INT, entity_man_head_ind_2);
                                    }

                                    if(GetBlockID(x+dcx, y+dcy-1, z) == 0 || GetBlockID(x+dcx, y+dcy-1, z) == 6 || GetBlockID(x+dcx, y+dcy-1, z) == 7 || GetBlockID(x+dcx, y+dcy-1, z) == 8 || GetBlockID(x+dcx, y+dcy-1, z) == 16)
                                    {
                                        glTexCoordPointer(2, GL_FLOAT, 0, lava_UV_3);
                                        glDrawElements(GL_TRIANGLES, entity_man_head_ind_cnt_3, GL_UNSIGNED_INT, entity_man_head_ind_3);
                                    }

                                    if(GetBlockID(x+dcx-1, y+dcy, z) == 0 || GetBlockID(x+dcx-1, y+dcy, z) == 6 || GetBlockID(x+dcx-1, y+dcy, z) == 7 || GetBlockID(x+dcx-1, y+dcy, z) == 8 || GetBlockID(x+dcx-1, y+dcy, z) == 16)
                                    {
                                        glTexCoordPointer(2, GL_FLOAT, 0, lava_UV_4);
                                        glDrawElements(GL_TRIANGLES, entity_man_head_ind_cnt_4, GL_UNSIGNED_INT, entity_man_head_ind_4);
                                    }

                                    if(GetBlockID(x+dcx, y+dcy, z+1) == 0 || GetBlockID(x+dcx, y+dcy, z+1) == 6 || GetBlockID(x+dcx, y+dcy, z+1) == 7 || GetBlockID(x+dcx, y+dcy, z+1) == 8 || GetBlockID(x+dcx, y+dcy, z+1) == 16)
                                    {
                                        glTexCoordPointer(2, GL_FLOAT, 0, lava_UV_5);
                                        glDrawElements(GL_TRIANGLES, entity_man_head_up_cnt, GL_UNSIGNED_INT, entity_man_head_ind_up);
                                    }

                                    if(GetBlockID(x+dcx, y+dcy, z-1) == 0 || GetBlockID(x+dcx, y+dcy, z-1) == 6 || GetBlockID(x+dcx, y+dcy, z-1) == 7 || GetBlockID(x+dcx, y+dcy, z-1) == 8 || GetBlockID(x+dcx, y+dcy, z-1) == 16)
                                    {
                                        glTranslatef(0.0, 0.0, -1);
                                        glTexCoordPointer(2, GL_FLOAT, 0, lava_UV_updn);
                                        glDrawElements(GL_TRIANGLES, entity_man_head_up_cnt, GL_UNSIGNED_INT, entity_man_head_ind_up);
                                    }
                                glPopMatrix();
                            }
                            else if(blocks[world[chunkx][chunky][x][y][z]].type == 2)
                            {
                                glVertexPointer(3, GL_FLOAT, 0, block);
                                glNormalPointer(GL_FLOAT, 0, normal);
                                glNormal3f(0,0,1);
                                glColor3f(0.7, 0.7, 0.7);
                                glBindTexture(GL_TEXTURE_2D, blocks[world[chunkx][chunky][x][y][z]].texture[0]);
                                glTexCoordPointer(2, GL_FLOAT, 0, block_texture);
                                glPushMatrix();
                                    glTranslatef(x+dcx, y+dcy, z);
                                    if(GetBlockID(x+dcx, y+dcy+1, z) == 0 || GetBlockID(x+dcx, y+dcy+1, z) == 6 || GetBlockID(x+dcx, y+dcy+1, z) == 7 || GetBlockID(x+dcx, y+dcy+1, z) == 8 || GetBlockID(x+dcx, y+dcy+1, z) == 16)
                                    {
                                        glTexCoordPointer(2, GL_FLOAT, 0, lava_UV);
                                        glDrawElements(GL_TRIANGLES, entity_man_head_ind_cnt_1, GL_UNSIGNED_INT, entity_man_head_ind_1);
                                    }

                                    if(GetBlockID(x+dcx+1, y+dcy, z) == 0 || GetBlockID(x+dcx+1, y+dcy, z) == 6 || GetBlockID(x+dcx+1, y+dcy, z) == 7 || GetBlockID(x+dcx+1, y+dcy, z) == 8 || GetBlockID(x+dcx+1, y+dcy, z) == 16)
                                    {
                                        glTexCoordPointer(2, GL_FLOAT, 0, lava_UV_2);
                                        glDrawElements(GL_TRIANGLES, entity_man_head_ind_cnt_2, GL_UNSIGNED_INT, entity_man_head_ind_2);
                                    }

                                    if(GetBlockID(x+dcx, y+dcy-1, z) == 0 || GetBlockID(x+dcx, y+dcy-1, z) == 6 || GetBlockID(x+dcx, y+dcy-1, z) == 7 || GetBlockID(x+dcx, y+dcy-1, z) == 8 || GetBlockID(x+dcx, y+dcy-1, z) == 16)
                                    {
                                        glTexCoordPointer(2, GL_FLOAT, 0, lava_UV_3);
                                        glDrawElements(GL_TRIANGLES, entity_man_head_ind_cnt_3, GL_UNSIGNED_INT, entity_man_head_ind_3);
                                    }

                                    if(GetBlockID(x+dcx-1, y+dcy, z) == 0 || GetBlockID(x+dcx-1, y+dcy, z) == 6 || GetBlockID(x+dcx-1, y+dcy, z) == 7 || GetBlockID(x+dcx-1, y+dcy, z) == 8 || GetBlockID(x+dcx-1, y+dcy, z) == 16)
                                    {
                                        glTexCoordPointer(2, GL_FLOAT, 0, lava_UV_4);
                                        glDrawElements(GL_TRIANGLES, entity_man_head_ind_cnt_4, GL_UNSIGNED_INT, entity_man_head_ind_4);
                                    }

                                    if(GetBlockID(x+dcx, y+dcy, z+1) == 0 || GetBlockID(x+dcx, y+dcy, z+1) == 6 || GetBlockID(x+dcx, y+dcy, z+1) == 7 || GetBlockID(x+dcx, y+dcy, z+1) == 8 || GetBlockID(x+dcx, y+dcy, z+1) == 16)
                                    {
                                        glBindTexture(GL_TEXTURE_2D, blocks[world[chunkx][chunky][x][y][z]].texture[1]);
                                        glTexCoordPointer(2, GL_FLOAT, 0, lava_UV_5);
                                        glDrawElements(GL_TRIANGLES, entity_man_head_up_cnt, GL_UNSIGNED_INT, entity_man_head_ind_up);
                                    }
                                    if(GetBlockID(x+dcx, y+dcy, z-1) == 0 || GetBlockID(x+dcx, y+dcy, z-1) == 6 || GetBlockID(x+dcx, y+dcy, z-1) == 7 || GetBlockID(x+dcx, y+dcy, z-1) == 8 || GetBlockID(x+dcx, y+dcy, z-1) == 16)
                                    {
                                        glBindTexture(GL_TEXTURE_2D, blocks[world[chunkx][chunky][x][y][z]].texture[2]);
                                        glTranslatef(0.0, 0.0, -1);
                                        glTexCoordPointer(2, GL_FLOAT, 0, lava_UV_updn);
                                        glDrawElements(GL_TRIANGLES, entity_man_head_up_cnt, GL_UNSIGNED_INT, entity_man_head_ind_up);
                                    }
                                glPopMatrix();
                            }
                            else if(blocks[world[chunkx][chunky][x][y][z]].type == 3)
                            {
                                glVertexPointer(3, GL_FLOAT, 0, plant);
                                glNormal3f(0,0,1);
                                glColor3f(0.7, 0.7, 0.7);
                                glBindTexture(GL_TEXTURE_2D, blocks[world[chunkx][chunky][x][y][z]].texture[0]);
                                glTexCoordPointer(2, GL_FLOAT, 0, plant_texture);
                                glPushMatrix();
                                    glTranslatef(x+dcx+0.5, y+dcy+0.5, z);
                                    glDrawElements(GL_TRIANGLES, plantIndCnt, GL_UNSIGNED_INT, plantInd);
                                glPopMatrix();
                                if(world[chunkx][chunky][x][y][z+1] != 0 && world[chunkx][chunky][x][y][z+1] != 6) world[chunkx][chunky][x][y][z] = 0;
                                if(world[chunkx][chunky][x][y][z-1] != 2 && world[chunkx][chunky][x][y][z-1] != 3) world[chunkx][chunky][x][y][z] = 0;
                            }
                            else if(blocks[world[chunkx][chunky][x][y][z]].type == 4 && world[chunkx][chunky][x][y][z] == 7)
                            {
                                LavaVisible[lava_reqshow].x = x+dcx;
                                LavaVisible[lava_reqshow].y = y+dcy;
                                LavaVisible[lava_reqshow].z = z;
                                LavaVisible[lava_reqshow].block_id = 1;
                                lava_reqshow++;
                                lava_req++;
                                if(GetBlockID(x+dcx+1, y+dcy, z) == 0) SetBlock(7, x+dcx+1, y+dcy, z);
                                if(GetBlockID(x+dcx-1, y+dcy, z) == 0) SetBlock(7, x+dcx-1, y+dcy, z);
                                if(GetBlockID(x+dcx, y+dcy+1, z) == 0) SetBlock(7, x+dcx, y+dcy+1, z);
                                if(GetBlockID(x+dcx, y+dcy-1, z) == 0) SetBlock(7, x+dcx, y+dcy-1, z);
                                if(GetBlockID(x+dcx, y+dcy, z-1) == 0) SetBlock(7, x+dcx, y+dcy, z-1);
                            }
                            else if(blocks[world[chunkx][chunky][x][y][z]].type == 4 && world[chunkx][chunky][x][y][z] == 8)
                            {
                                LavaVisible[lava_reqshow].x = x+dcx;
                                LavaVisible[lava_reqshow].y = y+dcy;
                                LavaVisible[lava_reqshow].z = z;
                                LavaVisible[lava_reqshow].block_id = 2;
                                lava_reqshow++;
                                lava_req++;
                                if(GetBlockID(x+dcx+1, y+dcy, z) == 0) SetBlock(8, x+dcx+1, y+dcy, z);
                                if(GetBlockID(x+dcx-1, y+dcy, z) == 0) SetBlock(8, x+dcx-1, y+dcy, z);
                                if(GetBlockID(x+dcx, y+dcy+1, z) == 0) SetBlock(8, x+dcx, y+dcy+1, z);
                                if(GetBlockID(x+dcx, y+dcy-1, z) == 0) SetBlock(8, x+dcx, y+dcy-1, z);
                                if(GetBlockID(x+dcx, y+dcy, z-1) == 0) SetBlock(8, x+dcx, y+dcy, z-1);
                            }
                            else if(blocks[world[chunkx][chunky][x][y][z]].type == 5)
                            {
                                glVertexPointer(3, GL_FLOAT, 0, block);
                                glNormalPointer(GL_FLOAT, 0, normal);
                                glNormal3f(0,0,1);
                                glColor3f(0.7, 0.7, 0.7);
                                glBindTexture(GL_TEXTURE_2D, blocks[world[chunkx][chunky][x][y][z]].texture[0]);
                                glTexCoordPointer(2, GL_FLOAT, 0, block_texture);
                                glPushMatrix();
                                    glTranslatef(x+dcx, y+dcy, z);
                                    if(GetBlockID(x+dcx, y+dcy+1, z) == 0 || GetBlockID(x+dcx, y+dcy+1, z) == 6 || GetBlockID(x+dcx, y+dcy+1, z) == 7 || GetBlockID(x+dcx, y+dcy+1, z) == 8)
                                    {
                                        glTexCoordPointer(2, GL_FLOAT, 0, lava_UV);
                                        glDrawElements(GL_TRIANGLES, entity_man_head_ind_cnt_1, GL_UNSIGNED_INT, entity_man_head_ind_1);
                                    }

                                    if(GetBlockID(x+dcx+1, y+dcy, z) == 0 || GetBlockID(x+dcx+1, y+dcy, z) == 6 || GetBlockID(x+dcx+1, y+dcy, z) == 7 || GetBlockID(x+dcx+1, y+dcy, z) == 8)
                                    {
                                        glTexCoordPointer(2, GL_FLOAT, 0, lava_UV_2);
                                        glDrawElements(GL_TRIANGLES, entity_man_head_ind_cnt_2, GL_UNSIGNED_INT, entity_man_head_ind_2);
                                    }

                                    if(GetBlockID(x+dcx, y+dcy-1, z) == 0 || GetBlockID(x+dcx, y+dcy-1, z) == 6 || GetBlockID(x+dcx, y+dcy-1, z) == 7 || GetBlockID(x+dcx, y+dcy-1, z) == 8 || GetBlockID(x+dcx, y+dcy-1, z) == 16)
                                    {
                                        glTexCoordPointer(2, GL_FLOAT, 0, lava_UV_3);
                                        glDrawElements(GL_TRIANGLES, entity_man_head_ind_cnt_3, GL_UNSIGNED_INT, entity_man_head_ind_3);
                                    }

                                    if(GetBlockID(x+dcx-1, y+dcy, z) == 0 || GetBlockID(x+dcx-1, y+dcy, z) == 6 || GetBlockID(x+dcx-1, y+dcy, z) == 7 || GetBlockID(x+dcx-1, y+dcy, z) == 8 || GetBlockID(x+dcx-1, y+dcy, z) == 16)
                                    {
                                        glTexCoordPointer(2, GL_FLOAT, 0, lava_UV_4);
                                        glDrawElements(GL_TRIANGLES, entity_man_head_ind_cnt_4, GL_UNSIGNED_INT, entity_man_head_ind_4);
                                    }

                                    if(GetBlockID(x+dcx, y+dcy, z+1) == 0 || GetBlockID(x+dcx, y+dcy, z+1) == 6 || GetBlockID(x+dcx, y+dcy, z+1) == 7 || GetBlockID(x+dcx, y+dcy, z+1) == 8)
                                    {
                                        glTexCoordPointer(2, GL_FLOAT, 0, lava_UV_5);
                                        glDrawElements(GL_TRIANGLES, entity_man_head_up_cnt, GL_UNSIGNED_INT, entity_man_head_ind_up);
                                    }

                                    if(GetBlockID(x+dcx, y+dcy, z-1) == 0 || GetBlockID(x+dcx, y+dcy, z-1) == 6 || GetBlockID(x+dcx, y+dcy, z-1) == 7 || GetBlockID(x+dcx, y+dcy, z-1) == 8 || GetBlockID(x+dcx, y+dcy, z-1) == 16)
                                    {
                                        glTranslatef(0.0, 0.0, -1);
                                        glTexCoordPointer(2, GL_FLOAT, 0, lava_UV_updn);
                                        glDrawElements(GL_TRIANGLES, entity_man_head_up_cnt, GL_UNSIGNED_INT, entity_man_head_ind_up);
                                    }
                                glPopMatrix();
                            }
                            int randome = rand();
                            randome %= 500;
                            if(randome == 1 && afk == FALSE)
                            {
                                if(world[chunkx][chunky][x][y][z] == 3)
                                {
                                    for(int zz = z+1; zz < 256; zz++)
                                    {
                                        if(world[chunkx][chunky][x][y][zz] != 0 && world[chunkx][chunky][x][y][z+1] != 6) break;
                                        if(zz == 255)
                                        {
                                            if(GetBlockID(x+dcx+1, y+dcy, z) == 2 || GetBlockID(x+dcx-1, y+dcy, z) == 2 || GetBlockID(x+dcx, y+dcy+1, z) == 2 || GetBlockID(x+dcx, y+dcy-1, z) == 2 || GetBlockID(x+dcx+1, y+dcy, z+1) == 2 || GetBlockID(x+dcx-1, y+dcy, z+1) == 2 || GetBlockID(x+dcx, y+dcy+1, z+1) == 2 || GetBlockID(x+dcx, y+dcy-1, z+1) == 2 || GetBlockID(x+dcx+1, y+dcy, z-1) == 2 || GetBlockID(x+dcx-1, y+dcy, z-1) == 2 || GetBlockID(x+dcx, y+dcy+1, z-1) == 2 || GetBlockID(x+dcx, y+dcy-1, z-1) == 2 && z >= 64)
                                            {
                                                world[chunkx][chunky][x][y][z] = 2;
                                                map_changed = TRUE;
                                            }
                                        }
                                    }
                                }
                                if(world[chunkx][chunky][x][y][z] == 2)
                                {
                                    if(world[chunkx][chunky][x][y][z+1] != 0 && world[chunkx][chunky][x][y][z+1] != 6)
                                    {
                                        world[chunkx][chunky][x][y][z] = 3;
                                        map_changed = TRUE;
                                    }
                                }
                            }
                            if(world[chunkx][chunky][x][y][z] != 0) dcnt++;
                        }
                    }
                }
            }
        }
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_LIGHTING);

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
        }
        for(int q = 0; q < 100; q++)
        {
            if(Sprites[q].block_id != 0)
            {
                for(int e = 0; e < 10; e++)
                {
                    int bid = (int)Sprites[q].block_id;
                    glVertexPointer(3, GL_FLOAT, 0, sprite_vertex);
                    glNormalPointer(GL_FLOAT, 0, normal);
                    glNormal3f(0,0,1);
                    glColor3f(0.7, 0.7, 0.7);
                    glBindTexture(GL_TEXTURE_2D, blocks[bid].texture[0]);
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

        for(int w = 0; w < lava_req; w++)
        {
            int x = LavaVisible[w].x;
            int y = LavaVisible[w].y;
            int z = LavaVisible[w].z;
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glVertexPointer(3, GL_FLOAT, 0, block);
            glNormalPointer(GL_FLOAT, 0, normal);
            glNormal3f(0,0,1);
            if(LavaVisible[w].block_id == 1)
            {
                glBindTexture(GL_TEXTURE_2D, blocks[7].texture[0]);
                glColor4f(0.7, 0.7, 0.7, 0.9);
            }
            else
            {
                glBindTexture(GL_TEXTURE_2D, blocks[8].texture[0]);
                glColor4f(0.7, 0.7, 0.7, 0.7);
            }
            glPushMatrix();
                glTranslatef(x, y, z);
                if(GetBlockID(x, y+1, z) == 0)
                {
                    glTexCoordPointer(2, GL_FLOAT, 0, lava_UV);
                    glDrawElements(GL_TRIANGLES, entity_man_head_ind_cnt_1, GL_UNSIGNED_INT, entity_man_head_ind_1);
                }

                if(GetBlockID(x+1, y, z) == 0)
                {
                    glTexCoordPointer(2, GL_FLOAT, 0, lava_UV_2);
                    glDrawElements(GL_TRIANGLES, entity_man_head_ind_cnt_2, GL_UNSIGNED_INT, entity_man_head_ind_2);
                }

                if(GetBlockID(x, y-1, z) == 0)
                {
                    glTexCoordPointer(2, GL_FLOAT, 0, lava_UV_3);
                    glDrawElements(GL_TRIANGLES, entity_man_head_ind_cnt_3, GL_UNSIGNED_INT, entity_man_head_ind_3);
                }

                if(GetBlockID(x-1, y, z) == 0)
                {
                    glTexCoordPointer(2, GL_FLOAT, 0, lava_UV_4);
                    glDrawElements(GL_TRIANGLES, entity_man_head_ind_cnt_4, GL_UNSIGNED_INT, entity_man_head_ind_4);
                }

                if(GetBlockID(x, y, z+1) == 0)
                {
                    glTexCoordPointer(2, GL_FLOAT, 0, lava_UV_5);
                    glDrawElements(GL_TRIANGLES, entity_man_head_up_cnt, GL_UNSIGNED_INT, entity_man_head_ind_up);
                }

                if(GetBlockID(x, y, z-1) == 0)
                {
                    glTranslatef(0.0, 0.0, -1);
                    glTexCoordPointer(2, GL_FLOAT, 0, lava_UV_updn);
                    glDrawElements(GL_TRIANGLES, entity_man_head_up_cnt, GL_UNSIGNED_INT, entity_man_head_ind_up);
                }
            glPopMatrix();
            glColor3f(0.7, 0.7, 0.7);
            glDisable(GL_BLEND);
        }


        lava_req = 0;
        lava_reqshow = 0;


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

int PlayerSetBlock()
{
    float sz = 0.1;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-scrKoef*sz,scrKoef*sz, -sz, sz, sz*2, 100);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    if(camera.x < 0 || camera.x >= worldsizex || camera.y < 0 || camera.y >= worldsizey || camera.z < 0 || camera.z >= 256) return FALSE;
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

        int cx = worldsizex / 16 - 1;
        int cy = worldsizey / 16 - 1;

        if(chunkx <= cx && chunkx >= 0 && chunky <= cy && chunky >= 0)
        {
            if(place_blocks == FALSE)
            {
                if(world[chunkx][chunky][X][Y][Z] != 7 && world[chunkx][chunky][X][Y][Z] != 8 && world[chunkx][chunky][X][Y][Z] != 9)
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
                    map_changed = TRUE;
                    UpdateChunk(chunkx, chunky);
                    if(X == 0) UpdateChunk(chunkx-1, chunky);
                    if(X == 15) UpdateChunk(chunkx+1, chunky);
                    if(Y == 0) UpdateChunk(chunkx, chunky-1);
                    if(Y == 15) UpdateChunk(chunkx, chunky+1);
               }
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
                    if((int)camera.x == (int)bx && (int)camera.y == (int)by && (int)camera.z == (int)bz) return;
                    if((int)camera.x == (int)bx && (int)camera.y == (int)by && (int)camera.z+1 == (int)bz) return;
                    if(select_inv == 1) world[chunkx][chunky][X][Y][Z] = 5;
                    if(select_inv == 2) world[chunkx][chunky][X][Y][Z] = 3;
                    if(select_inv == 3) world[chunkx][chunky][X][Y][Z] = 1;
                    if(select_inv == 4) world[chunkx][chunky][X][Y][Z] = 4;
                    if(select_inv == 6) world[chunkx][chunky][X][Y][Z] = 6;
                    map_changed = TRUE;
                    UpdateChunk(chunkx, chunky);
                    if(X == 0) UpdateChunk(chunkx-1, chunky);
                    if(X == 15) UpdateChunk(chunkx+1, chunky);
                    if(Y == 0) UpdateChunk(chunkx, chunky-1);
                    if(Y == 15) UpdateChunk(chunkx, chunky+1);
                }
            }
        }
    glPopMatrix();
}

int GetBlockID(int x, int y, int z)
{
    if(x < 0 || x >= worldsizex || y < 0 || y >= worldsizey || z < 0 || z >= 256) return -1;
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
    if(x < 0 || x >= worldsizex || y < 0 || y >= worldsizey || z < 0 || z >= 256) return 0;
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
    if(SAVE_GAME == FALSE) return;
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
    FILE *file_entity;
    FILE *file_borders;

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

    fwrite(world, sizeof(world), 1, file_world);
    fclose(file_world);


    free(slworld);
    slworld = NULL;


    sprintf(path, "%s\\saves\\world\\version.dat", buffer);

    file_version = fopen(path, "w");

    if(file_version == NULL) creat(path, S_IREAD|S_IWRITE);

    file_version = fopen(path, "w");

    char version[] = OPENCRAFT_VERSION;

    fwrite(version, 1, sizeof(version), file_version);
    fclose(file_version);

    sprintf(path, "%s\\saves\\world\\entity.dat", buffer);

    file_entity = fopen(path, "w");

    if(file_entity == NULL) creat(path, S_IREAD|S_IWRITE);

    file_entity = fopen(path, "wb");

    fwrite(Entities, sizeof(Entities[0]), 500, file_entity);
    fclose(file_entity);

    sprintf(path, "%s\\saves\\world\\border.dat", buffer);

    file_borders = fopen(path, "wb");

    if(file_borders == NULL) creat(path, S_IREAD|S_IWRITE);

    file_borders = fopen(path, "wb");

    fwrite(&worldsizex, 1, sizeof(worldsizex), file_borders);
    fclose(file_borders);


    map_changed = FALSE;
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
    if(new_cx < 0 || new_cy < 0 || new_cx >= worldsizex || new_cy >= worldsizey || Entities[j].z <= -1 || Entities[j].z >= 256)
    {
        Entities[j].x = new_cx;
        Entities[j].y = new_cy;
        float wsx = worldsizex + 0.3;
        float wsy = worldsizey + 0.3;
        if(new_cx <= wsx && new_cx >= worldsizex && Entities[j].z < 45) Entities[j].x = wsx;
        if(new_cy <= wsy && new_cy >= worldsizey && Entities[j].z < 45) Entities[j].y = wsy;
        if(new_cx >= -0.3 && new_cx < 0 && Entities[j].z < 45) Entities[j].x = -0.4;
        if(new_cy >= -0.3 && new_cy < 0 && Entities[j].z < 45) Entities[j].y = -0.4;
    }
    else if(world[chunkx][chunky][(int)x][(int)y][(int)Entities[j].z] == 0 || world[chunkx][chunky][(int)x][(int)y][(int)Entities[j].z] == 6 || world[chunkx][chunky][(int)x][(int)y][(int)Entities[j].z] == 7 || world[chunkx][chunky][(int)x][(int)y][(int)Entities[j].z] == 8)
    {
        if(world[chunkx][chunky][(int)x][(int)y][(int)Entities[j].z+1] == 0 || world[chunkx][chunky][(int)x][(int)y][(int)Entities[j].z+1] == 6 || world[chunkx][chunky][(int)x][(int)y][(int)Entities[j].z+1] == 7 || world[chunkx][chunky][(int)x][(int)y][(int)Entities[j].z+1] == 8)
        {
            float ccx = (int)new_cx;
            float ccy = (int)new_cy;
            if(GetBlockID((int)ccx - 1, (int)new_cy, (int)Entities[j].z) != 6 && GetBlockID((int)ccx - 1, (int)new_cy, (int)Entities[j].z) != 7 && GetBlockID((int)ccx - 1, (int)new_cy, (int)Entities[j].z) != 8)
            {
                if(new_cx < Entities[j].x && GetBlockID((int)ccx - 1, (int)new_cy, (int)Entities[j].z) != 0)
                {
                    if(new_cx <= ccx + 0.3) new_cx = ccx + 0.3;
                }
            }
            if(GetBlockID((int)ccx + 1, (int)new_cy, (int)Entities[j].z) != 6 && GetBlockID((int)ccx + 1, (int)new_cy, (int)Entities[j].z) != 7 && GetBlockID((int)ccx + 1, (int)new_cy, (int)Entities[j].z) != 8)
            {
                if(new_cx > Entities[j].x && GetBlockID((int)ccx + 1, (int)new_cy, (int)Entities[j].z) != 0)
                {
                    if(new_cx >= ccx - 0.3) new_cx = ccx + 0.3;
                }
            }
            if(GetBlockID((int)ccx, (int)new_cy - 1, (int)Entities[j].z) != 6 && GetBlockID((int)ccx, (int)new_cy - 1, (int)Entities[j].z) != 7 && GetBlockID((int)ccx, (int)new_cy - 1, (int)Entities[j].z) != 8)
            {
                if(new_cy < Entities[j].y && GetBlockID((int)ccx, (int)new_cy - 1, (int)Entities[j].z) != 0)
                {
                    if(new_cy <= ccy + 0.3) new_cy = ccy + 0.3;
                }
            }
            if(GetBlockID((int)ccx, (int)new_cy + 1, (int)Entities[j].z) != 6 && GetBlockID((int)ccx, (int)new_cy + 1, (int)Entities[j].z) != 7 && GetBlockID((int)ccx, (int)new_cy + 1, (int)Entities[j].z) != 8)
            {
                if(new_cy > Entities[j].y && GetBlockID((int)ccx, (int)new_cy + 1, (int)Entities[j].z) != 0)
                {
                    if(new_cy >= ccy + 0.3) new_cy = ccy + 0.3;
                }
            }

            if(new_cx < Entities[j].x && GetBlockID((int)ccx - 1, (int)new_cy, (int)Entities[j].z + 1) != 0 && GetBlockID((int)ccx - 1, (int)new_cy, (int)Entities[j].z + 1) != 7 && GetBlockID((int)ccx - 1, (int)new_cy, (int)Entities[j].z + 1) != 8)
            {
                if(new_cx <= ccx + 0.3) new_cx = ccx + 0.3;
            }
            if(new_cx > Entities[j].x && GetBlockID((int)ccx + 1, (int)new_cy, (int)Entities[j].z + 1) != 0 && GetBlockID((int)ccx + 1, (int)new_cy, (int)Entities[j].z + 1) != 7 && GetBlockID((int)ccx + 1, (int)new_cy, (int)Entities[j].z + 1) != 8)
            {
                if(new_cx >= ccx - 0.3) new_cx = ccx + 0.3;
            }
            if(new_cy < Entities[j].y && GetBlockID((int)new_cx, (int)ccy - 1, (int)Entities[j].z + 1) != 0 && GetBlockID((int)new_cx, (int)ccy - 1, (int)Entities[j].z + 1) != 7 && GetBlockID((int)new_cx, (int)ccy - 1, (int)Entities[j].z + 1) != 8)
            {
                if(new_cy <= ccy + 0.3) new_cy = ccy + 0.3;
            }
            if(new_cy > Entities[j].y && GetBlockID((int)new_cx, (int)ccy + 1, (int)Entities[j].z + 1) != 0 && GetBlockID((int)new_cx, (int)ccy + 1, (int)Entities[j].z + 1) != 7 && GetBlockID((int)new_cx, (int)ccy + 1, (int)Entities[j].z + 1) != 8)
            {
                if(new_cy >= ccy - 0.3) new_cy = ccy + 0.3;
            }
            Entities[j].x = new_cx;
            Entities[j].y = new_cy;
        }
    }
    if(Entities[j].x <= -0.3 || Entities[j].y <= -0.3 || Entities[j].x >= worldsizex || Entities[j].y >= worldsizey || Entities[j].z <= -0.01 || Entities[j].z >= 256)
    {
        Entities[j].z -= 0.3;
    }
    else if(GetBlockID((int)Entities[j].x, (int)Entities[j].y, (int)Entities[j].z-1) == 0 || GetBlockID((int)Entities[j].x, (int)Entities[j].y, (int)Entities[j].z-1) == 6 || GetBlockID((int)Entities[j].x, (int)Entities[j].y, (int)Entities[j].z-1) == 7 || GetBlockID((int)Entities[j].x, (int)Entities[j].y, (int)Entities[j].z-1) == 8)
    {
        if(Entities[j].is_jumping == 0) Entities[j].z -= 0.3;
    }
    float old_z = Entities[j].z;
    if(GetBlockID((int)Entities[j].x, (int)Entities[j].y, (int)Entities[j].z-1) != 0 && (int)Entities[j].z < Entities[j].z && Entities[j].is_jumping == 0 && GetBlockID((int)Entities[j].x, (int)Entities[j].y, (int)Entities[j].z-1) != 6 && GetBlockID((int)Entities[j].x, (int)Entities[j].y, (int)Entities[j].z-1) != 7 && GetBlockID((int)Entities[j].x, (int)Entities[j].y, (int)Entities[j].z-1) != 8)
    {
        Entities[j].z -= 0.3;
        if(Entities[j].z < (int)old_z) Entities[j].z = (int)old_z;
    }
    if(Entities[j].is_jumping)
    {
        float ste = 2 * jump_go[Entities[j].jump_tmp];
        Entities[j].z = (Entities[j].z_in_jump - 2)+(ste);
        if(Entities[j].x >= 0 && Entities[j].x < worldsizex && Entities[j].y >= 0 && Entities[j].y < worldsizey && Entities[j].z >= 0 && Entities[j].z < 256)
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
            if(Entities[j].jump_down == 1 && GetBlockID((int)Entities[j].x, (int)Entities[j].y, (int)Entities[j].z-1) != 0 && GetBlockID((int)Entities[j].x, (int)Entities[j].y, (int)Entities[j].z-1) != 6 && GetBlockID((int)Entities[j].x, (int)Entities[j].y, (int)Entities[j].z-1) != 7 && GetBlockID((int)Entities[j].x, (int)Entities[j].y, (int)Entities[j].z-1) != 8)
            {
                Entities[j].is_jumping = 0;
                Entities[j].jump_down = 0;
                Entities[j].jump_tmp = 0;
            }
            if(Entities[j].jump_down == 0 && GetBlockID((int)Entities[j].x, (int)Entities[j].y, (int)Entities[j].z+2) != 0 && GetBlockID((int)Entities[j].x, (int)Entities[j].y, (int)Entities[j].z+2) != 7 && GetBlockID((int)Entities[j].x, (int)Entities[j].y, (int)Entities[j].z+2) != 8)
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
    glDisable(GL_LIGHT0);
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

    static float framesPerSecond = 0.0f;
    static float lastTime = 0.0f;
    static int fps = 60;
    float currentTime = GetTickCount() * 0.001f;
    ++framesPerSecond;
    if(currentTime - lastTime > 1.0f)
    {
        lastTime = currentTime;
        fps = (int)framesPerSecond;
        framesPerSecond = 0;
        draw_cnt = 0;
    }

     glPushMatrix();
        glTranslatef(0,15,0);
        char line[50];
        sprintf(line, "%d fps, %d обновлений чанков", fps, update_chunks);
        Text_Out(line);
    glPopMatrix();

    if(afk == TRUE)
    {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        float background[] = {0,0, (float)scrSize.x,0, (float)scrSize.x,(float)scrSize.y, 0,(float)scrSize.y};
        glVertexPointer(2, GL_FLOAT, 0, background);
        glTexCoordPointer(2, GL_FLOAT, 0, d2_coord);
        glBindTexture(GL_TEXTURE_2D, menu_gradient_texture);
        glPushMatrix();
            glTranslatef(0, 0, 0);
            glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
        glPopMatrix();
        glDisable(GL_BLEND);
        glPushMatrix();

        int scrX = (scrSize.x / 2) - (405 / 2);
        int scrY = (scrSize.y / 2) - 120;
        if(menu_page == 0)
        {
            glPushMatrix();
                glTranslatef((scrSize.x / 2) - 45, scrY - 80, 0);
                Text_Out("Меню игры");
            glPopMatrix();
            ShowButton("Создать новый мир", sizeof("Создать новый мир") - 1, scrX, scrY, 0);
            ShowButton("Загрузить мир", sizeof("Загрузить мир") - 1, scrX, scrY+65, 1);
            ShowButton("Сохранить мир", sizeof("Сохранить мир") - 1, scrX, scrY+130, 2);
            ShowButton("Вернуться к игре", sizeof("Вернуться к игре") - 1, scrX, scrY+195, 3);

            glPopMatrix();
        }
        else
        {
            glPushMatrix();
                glTranslatef((scrSize.x / 2) - 85, scrY - 80, 0);
                Text_Out("Создать новый мир");
            glPopMatrix();
            ShowButton("Маленький", sizeof("Маленький") - 1, scrX, scrY, 4);
            ShowButton("Нормальный", sizeof("Нормальный") - 1, scrX, scrY+65, 5);
            ShowButton("Большой", sizeof("Большой") - 1, scrX, scrY+130, 6);
            ShowButton("Назад", sizeof("Назад") - 1, scrX, scrY+195, 7);

            glPopMatrix();
        }
    }

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
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

void UpdateChunk(int chunkx, int chunky)
{
    int cx = worldsizex / 16 - 1;
    int cy = worldsizey / 16 - 1;
    if(chunkx < 0 || chunkx > cx || chunky < 0 || chunky > cy) return;
    for(int z = 0; z < 256; z++)
    {
        for(int y = 0; y < 16; y++)
        {
            for(int x = 0; x < 16; x++)
            {
                int dcx = 16*chunkx;
                int dcy = 16*chunky;
                if(GetBlockID(x+dcx, y+dcy, z) != 0 && GetBlockID(x+dcx+1, y+dcy, z) != 0 && GetBlockID(x+dcx-1, y+dcy, z) != 0 && GetBlockID(x+dcx, y+dcy+1, z) != 0 && GetBlockID(x+dcx, y+dcy-1, z) != 0 && GetBlockID(x+dcx, y+dcy, z+1) != 0 && GetBlockID(x+dcx, y+dcy, z-1) != 0 && GetBlockID(x+dcx, y+dcy, z) != 6 && GetBlockID(x+dcx+1, y+dcy, z) != 6 && GetBlockID(x+dcx-1, y+dcy, z) != 6 && GetBlockID(x+dcx, y+dcy+1, z) != 6 && GetBlockID(x+dcx, y+dcy-1, z) != 6 && GetBlockID(x+dcx, y+dcy, z+1) != 6 && GetBlockID(x+dcx, y+dcy, z-1) != 6)
                {
                    if(GetBlockID(x+dcx, y+dcy, z) != 7 && GetBlockID(x+dcx+1, y+dcy, z) != 7 && GetBlockID(x+dcx-1, y+dcy, z) != 7 && GetBlockID(x+dcx, y+dcy+1, z) != 7 && GetBlockID(x+dcx, y+dcy-1, z) != 7 && GetBlockID(x+dcx, y+dcy, z+1) != 7 && GetBlockID(x+dcx, y+dcy, z-1) != 7)
                    {
                        if(GetBlockID(x+dcx, y+dcy, z) != 8 && GetBlockID(x+dcx+1, y+dcy, z) != 8 && GetBlockID(x+dcx-1, y+dcy, z) != 8 && GetBlockID(x+dcx, y+dcy+1, z) != 8 && GetBlockID(x+dcx, y+dcy-1, z) != 8 && GetBlockID(x+dcx, y+dcy, z+1) != 8 && GetBlockID(x+dcx, y+dcy, z-1) != 8)
                        {
                            if(GetBlockID(x+dcx, y+dcy, z) != 16 && GetBlockID(x+dcx+1, y+dcy, z) != 16 && GetBlockID(x+dcx-1, y+dcy, z) != 16 && GetBlockID(x+dcx, y+dcy+1, z) != 16 && GetBlockID(x+dcx, y+dcy-1, z) != 16 && GetBlockID(x+dcx, y+dcy, z+1) != 16 && GetBlockID(x+dcx, y+dcy, z-1) != 16) world_visible[chunkx][chunky][x][y][z] = FALSE;
                            else world_visible[chunkx][chunky][x][y][z] = TRUE;
                        }
                        else world_visible[chunkx][chunky][x][y][z] = TRUE;
                    }
                    else world_visible[chunkx][chunky][x][y][z] = TRUE;
                }
                else world_visible[chunkx][chunky][x][y][z] = TRUE;
            }
        }
    }
    if(chunk_update[chunkx][chunky] == FALSE)
    {
        update_chunks++;
        chunk_update[chunkx][chunky] = TRUE;
    }
}

void GenMenu_Show(char secondLine[], int slSize, BOOL genWorld)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, scrSize.x, scrSize.y, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    float background[] = {0,0, (float)scrSize.x,0, (float)scrSize.x,(float)scrSize.y, 0,(float)scrSize.y};
    float background_texture[] = {0,0, scrSize.x/16,0, scrSize.x/16,scrSize.y/16, 0,scrSize.y/16};

    glVertexPointer(2, GL_FLOAT, 0, background);
    glTexCoordPointer(2, GL_FLOAT, 0, background_texture);
    glBindTexture(GL_TEXTURE_2D, dirt_texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glPushMatrix();
        glTranslatef(0, 0, 0);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glPopMatrix();

    float curx = 0.0;

    glPushMatrix();
        if(!genWorld)
        {
            curx = (scrSize.x / 2) - ((sizeof("Загрузка мира") - 1)*10 / 2);
            glTranslatef(curx, (scrSize.y / 2) - 20, 0);
            Text_Out("Загрузка мира");
        }
        else
        {
            curx = (scrSize.x / 2) - ((sizeof("Создание мира") - 1)*10 / 2);
            glTranslatef(curx, (scrSize.y / 2) - 20, 0);
            Text_Out("Создание мира");
        }
    glPopMatrix();
    glPushMatrix();
        curx = (scrSize.x / 2) - ((slSize - 1)*10 / 2);
        glTranslatef(curx, (scrSize.y / 2) + 5, 0);
        Text_Out(secondLine);
    glPopMatrix();

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

void ShowButton(char name[], int tSize, int x, int y, int ID)
{
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glPushMatrix();
        glTranslatef(x, y, 0);
        glBindTexture(GL_TEXTURE_2D, widgets_texture);
        glVertexPointer(2, GL_FLOAT, 0, button);
        if(Buttons[ID].state == 0) glTexCoordPointer(2, GL_FLOAT, 0, button_coord);
        else if(Buttons[ID].state == 1) glTexCoordPointer(2, GL_FLOAT, 0, button_coord_active);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
        int curx = (scrSize.x / 2) - (((tSize)*10) / 2);
        float cury = (45 / 2) - 10;
    glPopMatrix();
        glPushMatrix();
            glTranslatef(curx, y+cury, 0);
            Text_Out(name);
        glPopMatrix();
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
    Buttons[ID].x = x;
    Buttons[ID].y = y;
    Buttons[ID].xe = x + 405;
    Buttons[ID].ye = y + 45;
}

BOOL CursorInButton(int x, int y, int ID)
{
    return (x >= Buttons[ID].x) && (x <= Buttons[ID].xe) && (y >= Buttons[ID].y) && (y <= Buttons[ID].ye);
}

int GetHighestBlock(int x, int y)
{
    for(int z = 255; z >= 0; z--)
    {
        if(GetBlockID(x, y, z) != 0)
        {
            return z;
        }
    }
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

    SetCursor(wcex.hCursor);


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
                          Horres / 2 - 854 / 2,
                          Vertres / 2 - 480 / 2,
                          854,
                          480,
                          NULL,
                          NULL,
                          hInstance,
                          NULL);

    ShowWindow(hwnd, nCmdShow);

    /* enable OpenGL for the window */
    EnableOpenGL(hwnd, &hDC, &hRC);

    GetClientRect(hwnd, &rcta);
    WndResize(rcta.right, rcta.bottom);

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

            glPushMatrix();
                Game_Show();
                Menu_Show();

            glPopMatrix();

            SwapBuffers(hDC);

            if(timer_del_chunks == 0)
            {
                update_chunks = 0;
                timer_del_chunks = 60;
                for(int y = 0; y < 16; y++)
                {
                    for(int x = 0; x < 16; x++)
                    {
                        chunk_update[x][y] = FALSE;
                    }
                }
            }
            static float lastTime = 0.0f;
            float currentTime = GetTickCount() * 0.001f;
            if(currentTime - lastTime > 0.01f)
            {
                float qwe = 0.0;
                if(currentTime - lastTime <= 1) qwe = (currentTime - lastTime) / 0.01f;
                else qwe = 1;
                lastTime = currentTime;
                for(int cnt = 1; cnt <= (int)qwe; cnt++)
                {
                    glPushMatrix();
                        if (GetForegroundWindow() == hwnd && afk == FALSE) Player_Move();
                    glPopMatrix();
                    if(afk == FALSE) for(int qwe = 0; qwe < 500; qwe++) EntityAI(qwe);
                    int chunkx = camera.x / 16;
                    int chunky = camera.y / 16;
                    int cameraz = (int)camera.z - 1;
                    int jdcx = 16*chunkx;
                    int jdcy = 16*chunky;
                    float xjump = camera.x - jdcx;
                    float yjump = camera.y - jdcy;

                    BOOL is_fly = FALSE;
                    BOOL is_up = FALSE;

                    if(is_jumping && afk == FALSE)
                    {
                        float ste = 2 * jump_go[jump_tmp];
                        camera.z = (camera_z_in_jump - 2)+(ste);
                        if(camera.x >= 0 && camera.x < worldsizex && camera.y >= 0 && camera.y < worldsizey && camera.z >= 0 && camera.z < 256)
                        {
                            if(!jump_down) jump_tmp++;
                            else jump_tmp--;
                            timer = 10;
                            if(jump_tmp == 9)
                            {
                                is_jumping = FALSE;
                                jump_down = FALSE;
                                jump_tmp = 0;
                            }
                            if(jump_tmp == 0 && jump_down)
                            {
                                is_jumping = FALSE;
                                jump_down = FALSE;
                                jump_tmp = 0;
                            }
                            if(jump_down == TRUE && world[chunkx][chunky][(int)xjump][(int)yjump][(int)camera.z] != 0 && world[chunkx][chunky][(int)xjump][(int)yjump][(int)camera.z] != 6)
                            {
                                is_jumping = FALSE;
                                jump_down = FALSE;
                                jump_tmp = 0;
                            }
                            if(jump_down == FALSE && world[chunkx][chunky][(int)xjump][(int)yjump][(int)camera.z + 2] != 0)
                            {
                                is_jumping = FALSE;
                                jump_down = FALSE;
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
                    if(GetBlockID((int)camera.x, (int)camera.y, (int)camera.z) == 7 && GetKeyState(' ') < 0 && timer_pl <= 0 || GetBlockID((int)camera.x, (int)camera.y, (int)camera.z) == 8 && GetKeyState(' ') < 0 && timer_pl <= 0)
                    {
                        if(GetBlockID((int)camera.x, (int)camera.y, (int)camera.z+2) != 0 && GetBlockID((int)camera.x, (int)camera.y, (int)camera.z+2) != 7 && GetBlockID((int)camera.x, (int)camera.y, (int)camera.z+2) != 8) {}
                        else
                        {
                            is_up = TRUE;
                            int z_old = (int)camera.z;
                            float z_new = camera.z + 0.03;
                            float ccx = (int)camera.x;
                            float ccy = (int)camera.y;
                            if((int)z_new > z_old && GetBlockID((int)camera.x, (int)camera.y, (int)z_new) == 0)
                            {
                                if(GetBlockID((int)camera.x - 1, (int)camera.y, (int)camera.z) != 0 && GetBlockID((int)camera.x - 1, (int)camera.y, (int)camera.z) != 7 && GetBlockID((int)camera.x - 1, (int)camera.y, (int)camera.z) != 8)
                                {
                                    if(camera.x <= ccx + 0.5) camera.z += 0.03;
                                    else timer_pl = 20;
                                }
                                else if(GetBlockID((int)camera.x + 1, (int)camera.y, (int)camera.z) != 0 && GetBlockID((int)camera.x + 1, (int)camera.y, (int)camera.z) != 7 && GetBlockID((int)camera.x + 1, (int)camera.y, (int)camera.z) != 8)
                                {
                                    if(camera.x >= ccx - 0.5) camera.z += 0.03;
                                    else timer_pl = 20;
                                }
                                else if(GetBlockID((int)camera.x, (int)camera.y+1, (int)camera.z) != 0 && GetBlockID((int)camera.x, (int)camera.y+1, (int)camera.z) != 7 && GetBlockID((int)camera.x, (int)camera.y+1, (int)camera.z) != 8)
                                {
                                    if(camera.y <= ccy + 0.5) camera.z += 0.03;
                                    else timer_pl = 20;
                                }
                                else if(GetBlockID((int)camera.x, (int)camera.y-1, (int)camera.z) != 0 && GetBlockID((int)camera.x, (int)camera.y-1, (int)camera.z) != 7 && GetBlockID((int)camera.x, (int)camera.y-1, (int)camera.z) != 8)
                                {
                                    if(camera.y >= ccy - 0.5) camera.z += 0.03;
                                    else timer_pl = 20;
                                }


                                else timer_pl = 20;
                            }
                            else camera.z += 0.03;
                        }
                    }
                    int cx = worldsizex / 16 - 1;
                    int cy = worldsizey / 16 - 1;
                    if(chunkx > cx || chunky > cy || camera.x < 0 || camera.y < 0 || cameraz <= -1 || chunkx < 0 || chunky < 0)
                    {
                        if(!is_jumping && afk == FALSE)
                        {
                            camera.z -= 0.3;
                            is_fly = TRUE;
                        }
                    }
                    else if(world[chunkx][chunky][(int)xjump][(int)yjump][(int)cameraz] == 0 && !is_jumping || world[chunkx][chunky][(int)xjump][(int)yjump][(int)cameraz] == 6 && !is_jumping || world[chunkx][chunky][(int)xjump][(int)yjump][(int)cameraz] == 7 && !is_jumping || world[chunkx][chunky][(int)xjump][(int)yjump][(int)cameraz] == 8 && !is_jumping)
                    {
                        if(afk == FALSE)
                        {
                            if(GetBlockID((int)camera.x, (int)camera.y, (int)camera.z - 1) != 7 && GetBlockID((int)camera.x, (int)camera.y, (int)camera.z - 1) != 8)
                            {
                                camera.z -= 0.3;
                                is_fly = TRUE;
                            }
                            else if(GetBlockID((int)camera.x, (int)camera.y, (int)camera.z - 1) == 7 && is_up == FALSE)
                            {
                                camera.z -= 0.02;
                                is_fly = TRUE;
                            }
                            else if(GetBlockID((int)camera.x, (int)camera.y, (int)camera.z - 1) == 8 && is_up == FALSE)
                            {
                                camera.z -= 0.02;
                                is_fly = TRUE;
                            }
                        }
                    }
                    if(chunkx <= cx && chunky <= cy && chunkx >= 0 && chunky >= 0 && afk == FALSE)
                    {
                        if(GetKeyState(' ') < 0 && !is_jumping && world[chunkx][chunky][(int)xjump][(int)yjump][(int)cameraz] != 0 && camera.x > 0 && camera.x < worldsizex && camera.y > 0 && camera.y < worldsizey && camera.z >= 0 && camera.z < 256 && !is_fly && ((int)camera.z - cameraz) == 1 && timer <= 0 && space == FALSE)
                        {
                            if(GetBlockID((int)camera.x, (int)camera.y, (int)camera.z) != 7 && GetBlockID((int)camera.x, (int)camera.y, (int)camera.z) != 8)
                            {
                                is_jumping = TRUE;
                                camera_z_in_jump = camera.z;
                                space = TRUE;
                            }
                        }
                    }

                    if(GetBlockID((int)camera.x, (int)camera.y, (int)camera.z - 1) != 0 && GetBlockID((int)camera.x, (int)camera.y, (int)camera.z - 1) != 6 && !is_fly && !is_jumping && !is_up)
                    {
                        float new_z = camera.z - 0.3;
                        if((int)new_z < (int)camera.z) camera.z = (int)camera.z;
                        else
                        {
                            camera.z -= 0.3;
                        }
                    }

                    if(GetKeyState('R') < 0 && timer_r <= 0 && afk == FALSE)
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
                    if(GetKeyState('G') < 0 && timer_g <= 0 && afk == FALSE)
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
                    if(GetKeyState('Y') < 0 && timer_y <= 0 && afk == FALSE)
                    {
                        if(!inverted_y) inverted_y = TRUE;
                        else inverted_y = FALSE;
                        timer_y = 10;
                    }
                    if(GetKeyState('F') < 0 && timer_f <= 0 && afk == FALSE)
                    {
                        if(render_distance < 4) render_distance++;
                        else render_distance = 1;
                        glFogf(GL_FOG_START, 8.0f*render_distance);
                        glFogf(GL_FOG_END, 9.0f*render_distance);
                        timer_f = 10;
                    }
                    if(GetKeyState('N') < 0 && timer_n <= 0 && afk == FALSE && map_changed == TRUE)
                    {
                        SaveWorld();
                        timer_n = 10;
                    }
                    if(afk == FALSE) SpritesManage();

                    theta += 0,1080000108;
                }
                timer--;
                timer_r--;
                timer_g--;
                timer_y--;
                timer_f--;
                timer_n--;
                timer_pl--;
                timer_del_chunks--;
            }
        }
        if(GetKeyState(' ') > 0) space = FALSE;
        static float lastTimeLM = 0.0f;
        float currentTimeLM = GetTickCount() * 0.001f;
        if(currentTimeLM - lastTimeLM > 0.25f)
        {
            lastTimeLM = currentTimeLM;
            lbutton_timer = FALSE;
        }
        if(GetKeyState(VK_LBUTTON) < 0 && lbutton_timer == FALSE)
        {
            if(afk == FALSE)
            {
                PlayerSetBlock();
                lbutton_timer = TRUE;
            }
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

        case WM_RBUTTONDOWN:
            if(afk == FALSE)
            {
                if(place_blocks == FALSE) place_blocks = TRUE;
                else place_blocks = FALSE;
            }
        break;

        case WM_MOVE:
            GetWindowRect(hwnd, &rctb);
        break;

        case WM_LBUTTONDOWN:
            if(afk)
            {
                if(menu_page == 0)
                {
                    if(CursorInButton(LOWORD(lParam), HIWORD(lParam), 0)) menu_page = 1;
                    if(CursorInButton(LOWORD(lParam), HIWORD(lParam), 2))
                    {
                        SaveWorld();
                        afk = FALSE;
                        cursorShow = FALSE;
                        while (ShowCursor(FALSE) >= 0);
                    }
                    if(CursorInButton(LOWORD(lParam), HIWORD(lParam), 3))
                    {
                        afk = FALSE;
                        cursorShow = FALSE;
                        while (ShowCursor(FALSE) >= 0);
                    }
                }
                else
                {
                    if(CursorInButton(LOWORD(lParam), HIWORD(lParam), 0))
                    {
                        worldsizex = 128;
                        worldsizey = 128;
                        menu_page = 0;
                        GenerateNewWorld();
                    }
                    if(CursorInButton(LOWORD(lParam), HIWORD(lParam), 1))
                    {
                        worldsizex = 256;
                        worldsizey = 256;
                        menu_page = 0;
                        GenerateNewWorld();
                    }
                    if(CursorInButton(LOWORD(lParam), HIWORD(lParam), 2))
                    {
                        worldsizex = 512;
                        worldsizey = 512;
                        menu_page = 0;
                        GenerateNewWorld();
                    }
                    if(CursorInButton(LOWORD(lParam), HIWORD(lParam), 3))
                    {
                        menu_page = 0;
                    }
                }
            }
        break;

        case WM_DESTROY:
            return 0;

        case WM_SETCURSOR:
            ShowCursor(cursorShow);
        break;

        case WM_KEYDOWN:
        {
            switch (wParam)
            {
                case VK_ESCAPE:
                {
                    if(afk == FALSE)
                    {
                        afk = TRUE;
                        cursorShow = TRUE;
                        while (ShowCursor(TRUE) <= 0);
                    }
                    else
                    {
                        afk = FALSE;
                        cursorShow = FALSE;
                        while (ShowCursor(FALSE) >= 0);
                    }
                }
            }
        }

        case WM_MOUSEMOVE:
        {
            if(afk)
            {
                for(int i = 0; i < 10; i++)
                {
                    if(CursorInButton(LOWORD(lParam), HIWORD(lParam), i)) Buttons[i].state = 1;
                    else Buttons[i].state = 0;
                }
            }
        }
        break;

        case WM_MOUSEWHEEL:
            tmp = (int)wParam;
            if(tmp > 0)
            {
                select_inv++;
                if(select_inv > 6) select_inv = 1;
                if(select_inv == 5) select_inv = 6;
            }
            else
            {
                select_inv--;
                if(select_inv < 1) select_inv = 6;
                if(select_inv == 5) select_inv = 4;
            }
        break;

        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    return 0;
}
