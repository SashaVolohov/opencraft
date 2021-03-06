#define STB_IMAGE_IMPLEMENTATION

#define SAVE_GAME TRUE /* ���� �� ������ ���, ������� FALSE, ����� ��������� ����� ���������� � �������� ���� */
#define SENSOR_MODE FALSE

#include "../../libs/stb_image/stb_image.h"
#include "../../libs/zlib/zlib.h"
#include "../../libs/curl/curl.h"
#include "../../libs/bass/bass.h"

#include "blocks.h"

#include <windows.h>
#include <direct.h>
#include <gl/glu.h>
#include <io.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <winsock2.h>
#include <pthread.h>
#include <mmsystem.h>

#include "camera.h"

#define OPENCRAFT_VERSION "0.24_SURVIVAL_TEST"

#define GAME_GENLWORLD 0
#define GAME_PAUSE 1
#define GAME_PLAY 2

BOOL inverted_y;

// ��������

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
int timer_enter = 0;
int timer_b = 0;

int select_inv = 0;

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

// Debug-����������

int draw_cnt = 0;

LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
void EnableOpenGL(HWND hwnd, HDC*, HGLRC*);
void DisableOpenGL(HWND, HDC, HGLRC);

float theta = 0.0f;

float block[] = {0,0,0, 1,0,0, 1,1,0, 0,1,0, 0,0,1, 1,0,1, 1,1,1, 0,1,1};
float block_small[] = {0,0,0, 0.5,0,0, 0.5,0.5,0, 0,0.5,0, 0,0,0.5, 0.5,0,0.5, 0.5,0.5,0.5, 0,0.5,0.5};
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
float cursor_texture[] = {0,0, 0.064,0, 0.064,0.06455, 0,0.06455};

float block_inventory[] = {0,0, 14, 8, 14, 23, 0, 18.5};
float block_inventory_2[] = {14, 8, 28, 0, 28, 18.5, 14, 23};
float block_inventory_3[] = {0,0, 14, -8, 28, 0, 14, 8};

float block_inventory_texture[] = {0,0, 1,0, 1,1, 0,1};

float plant[] = {-0.5, 0, 0, 0.5, 0, 0, 0.5,0,1, -0.5,0,1, 0,-0.5,0, 0,0.5,0, 0,0.5,1, 0, -0.5, 1};
float plant_texture[] = {0,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0};
GLuint plantInd[] = {0,1,2, 2,3,0, 4,5,6, 6,7,4};

int plantIndCnt = sizeof(plantInd) / sizeof(GLuint);

float block_inventory_flat[] = {0,0, 28,0, 28,28, 0,28};

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
float lava_UV_5[] = {0,0, 1,1, 1,0, 0,0, 0,1, 1,1, 1,0, 0,0};
float lava_UV_updn[] = {0,1, 1,1, 1,0, 0,0, 0,1, 1,1, 1,0, 0,0};

float real_lava_UV[] = {0,0, 1,0.035, 0,0.035, 1,0.035, 0,0, 1,0, 0,0, 1,0};
float real_lava_UV_2[] = {0,0.0, 1,0.035, 0,0.035, 1,0.035, 0,0, 1,0, 0,0, 1,0};
float real_lava_UV_3[] = {0,0.035, 1,0.035, 0,0.035, 1,0.035, 0,0, 1,0, 0,0, 1,0};
float real_lava_UV_4[] = {0,0.035, 1,0.035, 0,0.035, 1,0.035, 0,0, 1,0, 0,0, 1,0};
float real_lava_UV_5[] = {0,0, 1,0.035, 1,0, 0,0, 0,0.035, 1,0.035, 1,0, 0,0};
float real_lava_UV_updn[] = {0,0.035, 1,0.035, 1,0, 0,0, 0,0.035, 1,0.035, 1,0, 0,0};

float button_coord_closed[] = {0,0.18, 0.78,0.18, 0.78,0.26, 0,0.26};
float button_coord[] = {0,0.26, 0.78,0.26, 0.78,0.335, 0,0.335};
float button_coord_active[] = {0,0.338, 0.78,0.338, 0.78,0.413, 0,0.413};

typedef struct {
    int x,y;
    int xe, ye;
    int state;
} SButtons;

SButtons Buttons[30];

BOOL cursorShow = FALSE;

BOOL map_changed = FALSE;

BOOL space = FALSE;

BOOL lbutton_timer = FALSE;

int worldsizex, worldsizey = 0;

typedef struct {
    int x, y, z;
    int id;
} SWorld;

int cxtmp = 0;
int cytmp = 0;
int dcxtmp = 0;
int dcytmp = 0;

struct SSpawn {
    float x, y, z, Xrot, Zrot;
} spawn;

struct SServer {
    char ip[100];
    int port;
    char server_name[512];
    char motd[512];
} server;

WSADATA ws;
SOCKET s;

BOOL on_server = FALSE;
BOOL server_error = FALSE;

char ser_error_text[512];
char ser_error_text_output[512];

BOOL bQuit = FALSE;

BOOL sendGetWorld = FALSE;
BOOL server_loaded = FALSE;

float old_x, old_y, old_z, old_Xrot, old_Zrot;

float chat_enter[] = {0,0, 1,0, 1,24, 0,24};
BOOL chat_open = FALSE;
char chat_string[256];
int chat_size = 0;
BOOL show_chat_cursor = FALSE;

char nickname[32];
typedef struct {
    char text[512];
    BOOL dVisible;
    BOOL join_message;
} SChat;

SChat chat[10];
int chat_max = 0;

float lastTimeChat = 0.0f;

float nickname_vertex[] = {0,0,0, 0.3,0,0, 0.3,0,0.3, 0, 0, 0.3};

BOOL F1_12_pressed = FALSE;

float player_list[] = {0,0, 500,0, 500,300, 0,300};

BOOL enter_password = FALSE;

int type_chat = 0;

char skin[512];

float panel_fast[] = {0,0, 365,0, 365,45, 0,45};
float panel_texture[] = {0,0, 0.8,0, 0.8, 0.085, 0, 0.085};

float panel_select_fast[] = {0,0, 45,0, 45,48, 0,48};
float panel_select_texture[] = {0,0.09, 0.1,0.09, 0.1, 0.18, 0, 0.18};

typedef struct {
    int id;
} SSlot;

SSlot Inventory[9];

int anim_lw = 0;

float creative_inventory[] = {0,0, 480,0, 480,300, 0,300};
BOOL open_inventory = FALSE;
typedef struct {
    float vertex[4];
    int id;
} SCreativeSlot;

SCreativeSlot createInventoryVertex[256];
int selectCreateInventory = -1;

float load_line[] = {0,0, 200,0, 200,5, 0,5};
float done_load_line[] = {0,0, 0,0, 0,10, 0,10};

HSAMPLE sampstep;
HCHANNEL chstep;

BOOL is_x;

BOOL set_music = TRUE;
HSAMPLE mus;
HCHANNEL chmus;
BOOL set_sound = TRUE;
BOOL set_showfps = FALSE;

char conW = 'W';
char conA = 'A';
char conD = 'D';
char conS = 'S';
char conR = 'R';
char conEnter = VK_RETURN;
char conT = 'T';
char conSpace = ' ';
int selCon;

BOOL fly = FALSE;

float rx = 0;
float ry = 1;
float rz = 0;
int rstep = 0;

int hp = 20;
float heart[] = {0,0, 15,0, 15,15, 0,15};
float heart_null_texture[] = {0.065,0, 0.0975,0, 0.0975,0.03, 0.065,0.03};
float heart_null_new_texture[] = {0.0975,0, 0.13,0, 0.13,0.03, 0.0975,0.03};
float heart_double_texture[] = {0.13,0, 0.1625,0, 0.1625,0.03, 0.13,0.03};
float heart_double_new_texture[] = {0.1625,0, 0.195,0, 0.195,0.03, 0.1625,0.03};
float heart_texture[] = {0.205,0, 0.24,0, 0.24,0.03, 0.205,0.03};
float heart_new_texture[] = {0.24,0, 0.275,0, 0.275,0.03, 0.24,0.03};

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
                /*if(z == 0) world[chunkx][chunky][x][y][z] = 9;
                else if(z < coolz && z - coolz >= -3) world[chunkx][chunky][x][y][z] = 3;
                else if(z < coolz && z - coolz < -3) world[chunkx][chunky][x][y][z] = 5;
                else if(z == coolz && z <= 50) world[chunkx][chunky][x][y][z] = 10;
                else if(z == coolz && z > 50) world[chunkx][chunky][x][y][z] = 2;
                else if(z > coolz) world[chunkx][chunky][x][y][z] = 0;*/
                if(z == 20) world[chunkx][chunky][x][y][z] = 2;
                else world[chunkx][chunky][x][y][z] = 0;
                oldcoolz = coolz;
            }
        }
    }
}

void GenerateCaves()
{
    GenMenu_Show("��������� �����...", sizeof("��������� �����..."), 1);
    SwapBuffers(hDC);
    float lineonestep = 0;
    if(worldsizex == 512) lineonestep = 0.0007;
    if(worldsizex == 256) lineonestep = 0.003;
    if(worldsizex == 128) lineonestep = 0.012;
    int cnt_steps = 0;
    glEnableClientState(GL_VERTEX_ARRAY);
    glDisable(GL_TEXTURE_2D);
    BOOL exit_f = FALSE;
    int g3 = 0;
    for(int y = 0; y < worldsizey; y++)
    {
        if(exit_f == TRUE) break;
        for(int x = 0; x < worldsizex; x++)
        {
            if(exit_f == TRUE) break;
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
                        if(g3 >= 7) exit_f = TRUE;
                    }
                }
            }
            cnt_steps++;
        }
        glPushMatrix();
            glTranslatef(scrSize.x / 2 - 100, scrSize.y / 2 + 30, 0);
            float done_load_line[] = {0,0, lineonestep*cnt_steps,0, lineonestep*cnt_steps,5, 0,5};
            glVertexPointer(2, GL_FLOAT, 0, load_line);
            glColor3f(1, 1, 1);
            glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
            glVertexPointer(2, GL_FLOAT, 0, done_load_line);
            glColor3f(0, 1, 0);
            glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
        glPopMatrix();
        SwapBuffers(hDC);
    }
    glPushMatrix();
        glTranslatef(scrSize.x / 2 - 100, scrSize.y / 2 + 30, 0);
        float done_load_line[] = {0,0, 200,0, 200,5, 0,5};
        glVertexPointer(2, GL_FLOAT, 0, load_line);
        glColor3f(1, 1, 1);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
        glVertexPointer(2, GL_FLOAT, 0, done_load_line);
        glColor3f(0, 1, 0);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glPopMatrix();
    SwapBuffers(hDC);
    glEnable(GL_TEXTURE_2D);
    glDisableClientState(GL_VERTEX_ARRAY);
    glColor3f(1, 1, 1);
}

void GenerateWaterAndLava()
{
    GenMenu_Show("��������� ���� � ����...", sizeof("��������� ���� � ����..."), 1);
    SwapBuffers(hDC);
    float lineonestep = 0;
    if(worldsizex == 512) lineonestep = 0.0007;
    if(worldsizex == 256) lineonestep = 0.003;
    if(worldsizex == 128) lineonestep = 0.012;
    int cnt_steps = 0;
    glEnableClientState(GL_VERTEX_ARRAY);
    glDisable(GL_TEXTURE_2D);
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
            cnt_steps++;
        }
        glPushMatrix();
            glTranslatef(scrSize.x / 2 - 100, scrSize.y / 2 + 30, 0);
            float done_load_line[] = {0,0, lineonestep*cnt_steps,0, lineonestep*cnt_steps,5, 0,5};
            glVertexPointer(2, GL_FLOAT, 0, load_line);
            glColor3f(1, 1, 1);
            glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
            glVertexPointer(2, GL_FLOAT, 0, done_load_line);
            glColor3f(0, 1, 0);
            glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
        glPopMatrix();
        SwapBuffers(hDC);
    }
    glEnable(GL_TEXTURE_2D);
    glDisableClientState(GL_VERTEX_ARRAY);
    glColor3f(1, 1, 1);
}

void GenerateAdditionalProcessing()
{
    GenMenu_Show("�������������� ���������...", sizeof("�������������� ���������..."), 1);
    SwapBuffers(hDC);
    float lineonestep = 0;
    if(worldsizex == 512) lineonestep = 0.00035;
    if(worldsizex == 256) lineonestep = 0.0015;
    if(worldsizex == 128) lineonestep = 0.006;
    int cnt_steps = 0;
    glEnableClientState(GL_VERTEX_ARRAY);
    glDisable(GL_TEXTURE_2D);
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
            cnt_steps++;
        }
        glPushMatrix();
            glTranslatef(scrSize.x / 2 - 100, scrSize.y / 2 + 30, 0);
            float done_load_line[] = {0,0, lineonestep*cnt_steps,0, lineonestep*cnt_steps,5, 0,5};
            glVertexPointer(2, GL_FLOAT, 0, load_line);
            glColor3f(1, 1, 1);
            glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
            glVertexPointer(2, GL_FLOAT, 0, done_load_line);
            glColor3f(0, 1, 0);
            glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
        glPopMatrix();
        SwapBuffers(hDC);
    }
    glEnable(GL_TEXTURE_2D);
    glDisableClientState(GL_VERTEX_ARRAY);
    glColor3f(1, 1, 1);
}

void GenerateTrees()
{
    GenMenu_Show("��������� ��������...", sizeof("��������� ��������..."), 1);
    SwapBuffers(hDC);
    float lineonestep = 0;
    if(worldsizex == 512) lineonestep = 0.0007;
    if(worldsizex == 256) lineonestep = 0.003;
    if(worldsizex == 128) lineonestep = 0.012;
    int cnt_steps = 0;
    glEnableClientState(GL_VERTEX_ARRAY);
    glDisable(GL_TEXTURE_2D);
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
            cnt_steps++;
        }
        glPushMatrix();
            glTranslatef(scrSize.x / 2 - 100, scrSize.y / 2 + 30, 0);
            float done_load_line[] = {0,0, lineonestep*cnt_steps,0, lineonestep*cnt_steps,5, 0,5};
            glVertexPointer(2, GL_FLOAT, 0, load_line);
            glColor3f(1, 1, 1);
            glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
            glVertexPointer(2, GL_FLOAT, 0, done_load_line);
            glColor3f(0, 1, 0);
            glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
        glPopMatrix();
        SwapBuffers(hDC);
    }
    glEnable(GL_TEXTURE_2D);
    glDisableClientState(GL_VERTEX_ARRAY);
    glColor3f(1, 1, 1);
}

void GenerateOres()
{
    GenMenu_Show("��������� ���...", sizeof("��������� ���..."), 1);
    SwapBuffers(hDC);
    float lineonestep = 0;
    if(worldsizex == 512) lineonestep = 0.00035;
    if(worldsizex == 256) lineonestep = 0.0015;
    if(worldsizex == 128) lineonestep = 0.006;
    int cnt_steps = 0;
    glEnableClientState(GL_VERTEX_ARRAY);
    glDisable(GL_TEXTURE_2D);
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
            cnt_steps++;
        }
        glPushMatrix();
            glTranslatef(scrSize.x / 2 - 100, scrSize.y / 2 + 30, 0);
            float done_load_line[] = {0,0, lineonestep*cnt_steps,0, lineonestep*cnt_steps,5, 0,5};
            glVertexPointer(2, GL_FLOAT, 0, load_line);
            glColor3f(1, 1, 1);
            glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
            glVertexPointer(2, GL_FLOAT, 0, done_load_line);
            glColor3f(0, 1, 0);
            glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
        glPopMatrix();
        SwapBuffers(hDC);
    }
    glEnable(GL_TEXTURE_2D);
    glDisableClientState(GL_VERTEX_ARRAY);
    glColor3f(1, 1, 1);
}

void GenerateNewWorld()
{
    GenMenu_Show("��������� ���������...", sizeof("��������� ���������..."), 1);
    SwapBuffers(hDC);
    float lineonestep = 0;
    int y_need = worldsizey / 16 - 1;
    int x_need = worldsizex / 16 - 1;
    if(x_need == 31) lineonestep = 0.1953125;
    if(x_need == 15) lineonestep = 1.28;
    if(x_need == 7) lineonestep = 3.125;
    int cnt_steps = 0;
    glEnableClientState(GL_VERTEX_ARRAY);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, scrSize.x, scrSize.y, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    for(int y = 0; y <= y_need; y++)
    {
        for(int x = 0; x <= x_need; x++)
        {
            glPushMatrix();
                glTranslatef(scrSize.x / 2 - 100, scrSize.y / 2 + 30, 0);
                float done_load_line[] = {0,0, lineonestep*cnt_steps,0, lineonestep*cnt_steps,5, 0,5};
                glDisable(GL_TEXTURE_2D);
                glVertexPointer(2, GL_FLOAT, 0, load_line);
                glColor3f(1, 1, 1);
                glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
                glVertexPointer(2, GL_FLOAT, 0, done_load_line);
                glColor3f(0, 1, 0);
                glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
                glEnable(GL_TEXTURE_2D);
                glColor3f(1, 1, 1);
            glPopMatrix();
            SwapBuffers(hDC);
            GenerateNewChunk(x, y);
            cnt_steps++;
        }
    }
    glDisableClientState(GL_VERTEX_ARRAY);
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
    spawn.x = camera.x;
    spawn.y = camera.y;
    spawn.z = camera.z;
    spawn.Xrot = camera.Xrot;
    spawn.Zrot = camera.Zrot;
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
    SetCursorPos((int)rctb.left + scrSize.x/2, (int)rctb.top + scrSize.y/2);
    while (ShowCursor(FALSE) >= 0);
    afk = FALSE;
    SaveWorld();
    glEnable(GL_TEXTURE_2D);
}

size_t write_data(void *ptr, size_t size, size_t nmemb, FILE* stream)
{
    size_t written = fwrite(ptr, size, nmemb, stream);
    return written;
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
    LoadTexture("textures/blocks/dirt.png", &dirt_texture);

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
    registerBlock(17, 1, "textures/blocks/sponge.png");
    registerBlock(18, 1, "textures/blocks/glass.png");
    registerBlock(19, 1, "textures/blocks/wool_colored_white.png");
    registerBlock(20, 1, "textures/blocks/wool_colored_light_gray.png");
    registerBlock(21, 1, "textures/blocks/wool_colored_dark_gray.png");
    registerBlock(22, 1, "textures/blocks/wool_colored_red.png");
    registerBlock(23, 1, "textures/blocks/wool_colored_orange.png");
    registerBlock(24, 1, "textures/blocks/wool_colored_yellow.png");
    registerBlock(25, 1, "textures/blocks/wool_colored_charteros.png");
    registerBlock(26, 1, "textures/blocks/wool_colored_green.png");
    registerBlock(27, 1, "textures/blocks/wool_colored_spring_green.png");
    registerBlock(28, 1, "textures/blocks/wool_colored_turquoise.png");
    registerBlock(29, 1, "textures/blocks/wool_colored_capri.png");
    registerBlock(30, 1, "textures/blocks/wool_colored_ultramarine.png");
    registerBlock(31, 1, "textures/blocks/wool_colored_purple.png");
    registerBlock(32, 1, "textures/blocks/wool_colored_magenta.png");
    registerBlock(33, 1, "textures/blocks/wool_colored_crimson.png");
    registerBlock(34, 1, "textures/blocks/wool_colored_pink.png");
    registerBlock(35, 3, "textures/blocks/dandelion.png");
    registerBlock(36, 3, "textures/blocks/rose.png");
    registerBlock(37, 3, "textures/blocks/brown_mushroom.png");
    registerBlock(38, 3, "textures/blocks/red_mushroom.png");
    registerBlock(39, 1, "textures/blocks/gold_block.png");

    Inventory[0].id = 5;
    Inventory[1].id = 3;
    Inventory[2].id = 17;
    Inventory[3].id = 4;
    Inventory[4].id = 6;
    Inventory[5].id = 15;
    Inventory[6].id = 16;
    Inventory[7].id = 18;
    Inventory[8].id = 11;

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

    if(HIWORD(BASS_GetVersion()) != BASSVERSION)
    {
        MessageBox(NULL, "������ ������ BASS!", NULL, 0);
        PostQuitMessage(0);
        return 0;
    }


    if(!BASS_Init(-1, 22050, BASS_DEVICE_3D, 0, NULL))
    {
        MessageBox(NULL, "������ ������������� BASS!", NULL, 0);
        PostQuitMessage(0);
        return 0;
    }

    glEnableClientState(GL_VERTEX_ARRAY);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, scrSize.x, scrSize.y, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glDisable(GL_LIGHTING);
    glDisable(GL_LIGHT0);
    glDisable(GL_DEPTH_TEST);

    FILE *settings;

    char argument[1000];
    char value[1000];
    char sp[1000];

    settings = fopen("options.txt", "rt");

    if(settings != NULL)
    {
        while(!feof(settings))
        {
            fgets(sp, 1000, settings);
            if(sp[0] == '#') continue;
            BOOL exit = FALSE;
            for(int i = 0; i < 1000; i++)
            {
                if(sp[i] == ':')
                {
                    for(int iw = 0; iw < 1000; iw++)
                    {
                        argument[iw] = 0;
                        value[iw] = 0;
                    }
                    for(int iq = 0; iq < i; iq++)
                    {
                        argument[iq] = sp[iq];
                    }
                    int cnt = 0;
                    for(int ie = i+1; ie < 1000; ie++)
                    {
                        if(sp[ie] == '\n') break;
                        value[cnt] = sp[ie];
                        cnt++;
                    }
                    if(strcmp(argument, "music") == 0)
                    {
                        if(strcmp("false", value) == 0) set_music = FALSE;
                        else set_music = TRUE;
                    }
                    else if(strcmp(argument, "sound") == 0)
                    {
                        if(strcmp("false", value) == 0) set_sound = FALSE;
                        else set_sound = TRUE;
                    }
                    else if(strcmp(argument, "invertYMouse") == 0)
                    {
                        if(strcmp("false", value) == 0) inverted_y = FALSE;
                        else inverted_y = TRUE;
                    }
                    else if(strcmp(argument, "showFrameRate") == 0)
                    {
                        if(strcmp("false", value) == 0) set_showfps = FALSE;
                        else set_showfps = TRUE;
                    }
                    else if(strcmp(argument, "viewDistance") == 0)
                    {
                        render_distance = atoi(value);
                    }
                    else if(strcmp(argument, "key_Forward") == 0)
                    {
                        conW = atoi(value);
                    }
                    else if(strcmp(argument, "key_Left") == 0)
                    {
                        conA = atoi(value);
                    }
                    else if(strcmp(argument, "key_Right") == 0)
                    {
                        conD = atoi(value);
                    }
                    else if(strcmp(argument, "key_Back") == 0)
                    {
                        conS = atoi(value);
                    }
                    else if(strcmp(argument, "key_Load location") == 0)
                    {
                        conR = atoi(value);
                    }
                    else if(strcmp(argument, "key_Save location") == 0)
                    {
                        conEnter = atoi(value);
                    }
                    else if(strcmp(argument, "key_Chat") == 0)
                    {
                        conT = atoi(value);
                    }
                    else if(strcmp(argument, "key_Jump") == 0)
                    {
                        conSpace = atoi(value);
                    }
                    exit = TRUE;
                }
                if(exit == TRUE) break;
            }
        }
        fclose(settings);
    }

    if(on_server == FALSE)
    {
        TCHAR buffer[MAX_PATH];
        GetCurrentDirectory(sizeof(buffer), buffer);
        char path[MAX_PATH];
        sprintf(path, "%s\\saves\\world", buffer);
        float lineonestep = 33.3;
        int cnt_steps = 0;
        if(DirectoryExists(path) && SAVE_GAME == TRUE)
        {
            FILE *level;
            FILE *file_spawn;
            FILE *file_world;
            FILE *file_version;
            FILE *file_entity;
            FILE *file_borders;

            GenMenu_Show("�������� border.dat...", sizeof("�������� border.dat..."), 0);

            glPushMatrix();
                glDisable(GL_TEXTURE_2D);
                glTranslatef(scrSize.x / 2 - 100, scrSize.y / 2 + 30, 0);
                float done_load_line[] = {0,0, lineonestep*cnt_steps,0, lineonestep*cnt_steps,5, 0,5};
                glVertexPointer(2, GL_FLOAT, 0, load_line);
                glColor3f(1, 1, 1);
                glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
                glVertexPointer(2, GL_FLOAT, 0, done_load_line);
                glColor3f(0, 1, 0);
                glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
                glEnable(GL_TEXTURE_2D);
            glPopMatrix();
            SwapBuffers(hDC);

            cnt_steps++;

            sprintf(path, "%s\\saves\\world\\border.dat", buffer);

            file_borders = fopen(path, "r");

            if(file_borders == NULL) creat(path, S_IREAD|S_IWRITE);

            file_borders = fopen(path, "r");

            fread(&worldsizex, 1, sizeof(worldsizex), file_borders);
            fclose(file_borders);

            worldsizey = worldsizex;

            GenMenu_Show("�������� level.dat...", sizeof("�������� level.dat..."), 0);

            glPushMatrix();
                glDisable(GL_TEXTURE_2D);
                glTranslatef(scrSize.x / 2 - 100, scrSize.y / 2 + 30, 0);
                done_load_line[2] = lineonestep*cnt_steps;
                done_load_line[4] = lineonestep*cnt_steps;
                glVertexPointer(2, GL_FLOAT, 0, load_line);
                glColor3f(1, 1, 1);
                glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
                glVertexPointer(2, GL_FLOAT, 0, done_load_line);
                glColor3f(0, 1, 0);
                glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
                glEnable(GL_TEXTURE_2D);
            glPopMatrix();
            SwapBuffers(hDC);

            cnt_steps++;

            sprintf(path, "%s\\saves\\world\\level.dat", buffer);

            level = fopen(path, "r");

            if(level != NULL)
            {
                fread(&camera, 1, sizeof(camera), level);
                fclose(level);
            }

            GenMenu_Show("�������� spawn.dat...", sizeof("�������� spawn.dat..."), 0);

            glPushMatrix();
                glDisable(GL_TEXTURE_2D);
                glTranslatef(scrSize.x / 2 - 100, scrSize.y / 2 + 30, 0);
                done_load_line[2] = lineonestep*cnt_steps;
                done_load_line[4] = lineonestep*cnt_steps;
                glVertexPointer(2, GL_FLOAT, 0, load_line);
                glColor3f(1, 1, 1);
                glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
                glVertexPointer(2, GL_FLOAT, 0, done_load_line);
                glColor3f(0, 1, 0);
                glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
                glEnable(GL_TEXTURE_2D);
            glPopMatrix();
            SwapBuffers(hDC);

            cnt_steps++;

            sprintf(path, "%s\\saves\\world\\spawn.dat", buffer);

            file_spawn = fopen(path, "r");

            if(file_spawn != NULL)
            {
                fread(&spawn, 1, sizeof(spawn), file_spawn);
                fclose(file_spawn);
            }

            GenMenu_Show("�������� world.ocw...", sizeof("�������� world.ocw..."), 0);

            glPushMatrix();
                glDisable(GL_TEXTURE_2D);
                glTranslatef(scrSize.x / 2 - 100, scrSize.y / 2 + 30, 0);
                done_load_line[2] = lineonestep*cnt_steps;
                done_load_line[4] = lineonestep*cnt_steps;
                glVertexPointer(2, GL_FLOAT, 0, load_line);
                glColor3f(1, 1, 1);
                glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
                glVertexPointer(2, GL_FLOAT, 0, done_load_line);
                glColor3f(0, 1, 0);
                glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
                glEnable(GL_TEXTURE_2D);
            glPopMatrix();
            SwapBuffers(hDC);
            cnt_steps++;

            sprintf(path, "%s\\saves\\world\\world.ocw", buffer);

            file_world = fopen(path, "r");

            if(file_world != NULL)
            {
                fread(world, 1, sizeof(world), file_world);
                fclose(file_world);
            }

            char version[50];

            GenMenu_Show("�������� version.dat...", sizeof("�������� version.dat..."), 0);

            glPushMatrix();
                glDisable(GL_TEXTURE_2D);
                glTranslatef(scrSize.x / 2 - 100, scrSize.y / 2 + 30, 0);
                done_load_line[2] = lineonestep*cnt_steps;
                done_load_line[4] = lineonestep*cnt_steps;
                glVertexPointer(2, GL_FLOAT, 0, load_line);
                glColor3f(1, 1, 1);
                glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
                glVertexPointer(2, GL_FLOAT, 0, done_load_line);
                glColor3f(0, 1, 0);
                glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
                glEnable(GL_TEXTURE_2D);
            glPopMatrix();
            SwapBuffers(hDC);

            cnt_steps++;

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

            GenMenu_Show("�������� entity.dat...", sizeof("�������� entity.dat..."), 0);

            glPushMatrix();
                glDisable(GL_TEXTURE_2D);
                glTranslatef(scrSize.x / 2 - 100, scrSize.y / 2 + 30, 0);
                done_load_line[2] = lineonestep*cnt_steps;
                done_load_line[4] = lineonestep*cnt_steps;
                glVertexPointer(2, GL_FLOAT, 0, load_line);
                glColor3f(1, 1, 1);
                glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
                glVertexPointer(2, GL_FLOAT, 0, done_load_line);
                glColor3f(0, 1, 0);
                glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
                glEnable(GL_TEXTURE_2D);
            glPopMatrix();
            SwapBuffers(hDC);

            cnt_steps++;

            sprintf(path, "%s\\saves\\world\\entity.dat", buffer);

            file_entity = fopen(path, "rb");

            if(file_entity != NULL)
            {
                fread(Entities, sizeof(Entities[0]), 500, file_entity);
                fclose(file_entity);
            }

            glPushMatrix();
                glDisable(GL_TEXTURE_2D);
                glTranslatef(scrSize.x / 2 - 100, scrSize.y / 2 + 30, 0);
                done_load_line[2] = lineonestep*cnt_steps;
                done_load_line[4] = lineonestep*cnt_steps;
                glVertexPointer(2, GL_FLOAT, 0, load_line);
                glColor3f(1, 1, 1);
                glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
                glVertexPointer(2, GL_FLOAT, 0, done_load_line);
                glColor3f(0, 1, 0);
                glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
                glEnable(GL_TEXTURE_2D);
            glPopMatrix();
            SwapBuffers(hDC);

        }
        else
        {
            if(SAVE_GAME == FALSE)
            {
                worldsizex = 128;
                worldsizey = 128;
            }
            else
            {
                worldsizex = 512;
                worldsizey = 512;
            }
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
    }
    else
    {
        int actual_len;
        char buff[512];
        sprintf(buff, "ops get_server_name");
        if(SOCKET_ERROR == (send(s, &buff, sizeof(buff), 0)))
        {
            server_error = TRUE;
            sprintf(ser_error_text, "�� ������� ��������� ����");
        }
        for(int iw = 0; iw < 512; iw++)
        {
            buff[iw] = 0;
        }
        sprintf(buff, "ops get_motd");
        if(SOCKET_ERROR == (send(s, &buff, sizeof(buff), 0)))
        {
            server_error = TRUE;
            sprintf(ser_error_text, "�� ������� ��������� ����");
        }
        for(int iw = 0; iw < 512; iw++)
        {
            buff[iw] = 0;
        }
        sprintf(buff, "ops get_worldsize");
        if(SOCKET_ERROR == (send(s, &buff, sizeof(buff), 0)))
        {
            server_error = TRUE;
            sprintf(ser_error_text, "�� ������� ��������� ����");
        }
    }
    if(on_server == FALSE)
    {
        cursorShow = FALSE;
        SetCursorPos((int)rctb.left + scrSize.x/2, (int)rctb.top + scrSize.y/2);
        while (ShowCursor(FALSE) >= 0);
    }
}
void WndResize(int x, int y)
{
    glViewport(0,0,x,y);
    scrKoef = x / (float)y;
    scrSize.x = x;
    scrSize.y = y;
    chat_enter[2] = scrSize.x-10;
    chat_enter[4] = chat_enter[2];
}

void Player_Move()
{
    if(SENSOR_MODE == FALSE)
    {
        if(GetBlockID((int)camera.x, (int)camera.y, (int)camera.z) == 7 || GetBlockID((int)camera.x, (int)camera.y, (int)camera.z) == 8) Camera_MoveDirection(GetKeyState(conW) < 0 ? 1: (GetKeyState(conS) < 0 ? -1 : 0)
                             ,GetKeyState(conD) < 0 ? 1 : (GetKeyState(conA) < 0 ? -1: 0)
                             ,0.02);
        else Camera_MoveDirection(GetKeyState(conW) < 0 ? 1: (GetKeyState(conS) < 0 ? -1 : 0)
            ,GetKeyState(conD) < 0 ? 1 : (GetKeyState(conA) < 0 ? -1: 0)
            ,0.12);
    }
    else
    {
        if(GetBlockID((int)camera.x, (int)camera.y, (int)camera.z) == 7 || GetBlockID((int)camera.x, (int)camera.y, (int)camera.z) == 8) Camera_MoveDirection(GetKeyState(VK_RBUTTON) < 0 ? 1: 0
                             ,0
                             ,0.02);
        else Camera_MoveDirection(GetKeyState(VK_RBUTTON) < 0 ? 1: 0
            ,0
            ,0.12);
    }
    if(afk == FALSE && chat_open == FALSE && open_inventory == FALSE && SENSOR_MODE == FALSE) Camera_AutoMoveByMouse((int)rctb.left + scrSize.x/2, (int)rctb.top + scrSize.y/2, 0.2);
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

    if(GetKeyState('1') < 0 && afk == FALSE && chat_open == FALSE && open_inventory == FALSE) select_inv = 0;
    if(GetKeyState('2') < 0 && afk == FALSE && chat_open == FALSE && open_inventory == FALSE) select_inv = 1;
    if(GetKeyState('3') < 0 && afk == FALSE && chat_open == FALSE && open_inventory == FALSE) select_inv = 2;
    if(GetKeyState('4') < 0 && afk == FALSE && chat_open == FALSE && open_inventory == FALSE) select_inv = 3;
    if(GetKeyState('5') < 0 && afk == FALSE && chat_open == FALSE && open_inventory == FALSE) select_inv = 4;
    if(GetKeyState('6') < 0 && afk == FALSE && chat_open == FALSE && open_inventory == FALSE) select_inv = 5;
    if(GetKeyState('7') < 0 && afk == FALSE && chat_open == FALSE && open_inventory == FALSE) select_inv = 6;
    if(GetKeyState('8') < 0 && afk == FALSE && chat_open == FALSE && open_inventory == FALSE) select_inv = 7;
    if(GetKeyState('9') < 0 && afk == FALSE && chat_open == FALSE && open_inventory == FALSE) select_inv = 8;

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
                            if(!world_visible[chunkx][chunky][x][y][z] && on_server == FALSE) continue;
                            int realx = x + dcx;
                            int realy = y + dcy;
                            if(world[chunkx][chunky][x][y][z] == 17)
                            {
                                for(int zz = z-2; zz <= z+2; zz++)
                                {
                                    for(int yy = realy-2; yy <= realy+2; yy++)
                                    {
                                        for(int xx = realx-2; xx <= realx+2; xx++)
                                        {
                                            if(GetBlockID(xx, yy, zz) == 8) SetBlock(0, xx, yy, zz);
                                        }
                                    }
                                }
                            }
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
                                    if(GetBlockID(x+dcx, y+dcy+1, z) == 0 || GetBlockID(x+dcx, y+dcy+1, z) == 6 || GetBlockID(x+dcx, y+dcy+1, z) == 7 || GetBlockID(x+dcx, y+dcy+1, z) == 8 || GetBlockID(x+dcx, y+dcy+1, z) == 16 || GetBlockID(x+dcx, y+dcy+1, z) == 18 || blocks[GetBlockID(x+dcx, y+dcy+1, z)].type == 3)
                                    {
                                        glTexCoordPointer(2, GL_FLOAT, 0, lava_UV);
                                        glDrawElements(GL_TRIANGLES, entity_man_head_ind_cnt_1, GL_UNSIGNED_INT, entity_man_head_ind_1);
                                    }

                                    if(GetBlockID(x+dcx+1, y+dcy, z) == 0 || GetBlockID(x+dcx+1, y+dcy, z) == 6 || GetBlockID(x+dcx+1, y+dcy, z) == 7 || GetBlockID(x+dcx+1, y+dcy, z) == 8 || GetBlockID(x+dcx+1, y+dcy, z) == 16 || GetBlockID(x+dcx+1, y+dcy, z) == 18 || blocks[GetBlockID(x+dcx+1, y+dcy, z)].type == 3)
                                    {
                                        glTexCoordPointer(2, GL_FLOAT, 0, lava_UV_2);
                                        glDrawElements(GL_TRIANGLES, entity_man_head_ind_cnt_2, GL_UNSIGNED_INT, entity_man_head_ind_2);
                                    }

                                    if(GetBlockID(x+dcx, y+dcy-1, z) == 0 || GetBlockID(x+dcx, y+dcy-1, z) == 6 || GetBlockID(x+dcx, y+dcy-1, z) == 7 || GetBlockID(x+dcx, y+dcy-1, z) == 8 || GetBlockID(x+dcx, y+dcy-1, z) == 16 || GetBlockID(x+dcx, y+dcy-1, z) == 18 || blocks[GetBlockID(x+dcx, y+dcy-1, z)].type == 3)
                                    {
                                        glTexCoordPointer(2, GL_FLOAT, 0, lava_UV_3);
                                        glDrawElements(GL_TRIANGLES, entity_man_head_ind_cnt_3, GL_UNSIGNED_INT, entity_man_head_ind_3);
                                    }

                                    if(GetBlockID(x+dcx-1, y+dcy, z) == 0 || GetBlockID(x+dcx-1, y+dcy, z) == 6 || GetBlockID(x+dcx-1, y+dcy, z) == 7 || GetBlockID(x+dcx-1, y+dcy, z) == 8 || GetBlockID(x+dcx-1, y+dcy, z) == 16 || GetBlockID(x+dcx-1, y+dcy, z) == 18 || blocks[GetBlockID(x+dcx-1, y+dcy, z)].type == 3)
                                    {
                                        glTexCoordPointer(2, GL_FLOAT, 0, lava_UV_4);
                                        glDrawElements(GL_TRIANGLES, entity_man_head_ind_cnt_4, GL_UNSIGNED_INT, entity_man_head_ind_4);
                                    }

                                    if(GetBlockID(x+dcx, y+dcy, z+1) == 0 || GetBlockID(x+dcx, y+dcy, z+1) == 6 || GetBlockID(x+dcx, y+dcy, z+1) == 7 || GetBlockID(x+dcx, y+dcy, z+1) == 8 || GetBlockID(x+dcx, y+dcy, z+1) == 16 || GetBlockID(x+dcx, y+dcy, z+1) == 18 || blocks[GetBlockID(x+dcx, y+dcy, z+1)].type == 3)
                                    {
                                        glTexCoordPointer(2, GL_FLOAT, 0, lava_UV_5);
                                        glDrawElements(GL_TRIANGLES, entity_man_head_up_cnt, GL_UNSIGNED_INT, entity_man_head_ind_up);
                                    }

                                    if(GetBlockID(x+dcx, y+dcy, z-1) == 0 || GetBlockID(x+dcx, y+dcy, z-1) == 6 || GetBlockID(x+dcx, y+dcy, z-1) == 7 || GetBlockID(x+dcx, y+dcy, z-1) == 8 || GetBlockID(x+dcx, y+dcy, z-1) == 16 || GetBlockID(x+dcx, y+dcy, z-1) == 18 || blocks[GetBlockID(x+dcx, y+dcy, z-1)].type == 3)
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
                                    if(GetBlockID(x+dcx, y+dcy+1, z) == 0 || GetBlockID(x+dcx, y+dcy+1, z) == 6 || GetBlockID(x+dcx, y+dcy+1, z) == 7 || GetBlockID(x+dcx, y+dcy+1, z) == 8 || GetBlockID(x+dcx, y+dcy+1, z) == 16 || GetBlockID(x+dcx, y+dcy+1, z) == 18 || blocks[GetBlockID(x+dcx, y+dcy+1, z)].type == 3)
                                    {
                                        glTexCoordPointer(2, GL_FLOAT, 0, lava_UV);
                                        glDrawElements(GL_TRIANGLES, entity_man_head_ind_cnt_1, GL_UNSIGNED_INT, entity_man_head_ind_1);
                                    }

                                    if(GetBlockID(x+dcx+1, y+dcy, z) == 0 || GetBlockID(x+dcx+1, y+dcy, z) == 6 || GetBlockID(x+dcx+1, y+dcy, z) == 7 || GetBlockID(x+dcx+1, y+dcy, z) == 8 || GetBlockID(x+dcx+1, y+dcy, z) == 16 || GetBlockID(x+dcx+1, y+dcy, z) == 18 || blocks[GetBlockID(x+dcx+1, y+dcy, z)].type == 3)
                                    {
                                        glTexCoordPointer(2, GL_FLOAT, 0, lava_UV_2);
                                        glDrawElements(GL_TRIANGLES, entity_man_head_ind_cnt_2, GL_UNSIGNED_INT, entity_man_head_ind_2);
                                    }

                                    if(GetBlockID(x+dcx, y+dcy-1, z) == 0 || GetBlockID(x+dcx, y+dcy-1, z) == 6 || GetBlockID(x+dcx, y+dcy-1, z) == 7 || GetBlockID(x+dcx, y+dcy-1, z) == 8 || GetBlockID(x+dcx, y+dcy-1, z) == 16 || GetBlockID(x+dcx, y+dcy-1, z) == 18 || blocks[GetBlockID(x+dcx, y+dcy-1, z)].type == 3)
                                    {
                                        glTexCoordPointer(2, GL_FLOAT, 0, lava_UV_3);
                                        glDrawElements(GL_TRIANGLES, entity_man_head_ind_cnt_3, GL_UNSIGNED_INT, entity_man_head_ind_3);
                                    }

                                    if(GetBlockID(x+dcx-1, y+dcy, z) == 0 || GetBlockID(x+dcx-1, y+dcy, z) == 6 || GetBlockID(x+dcx-1, y+dcy, z) == 7 || GetBlockID(x+dcx-1, y+dcy, z) == 8 || GetBlockID(x+dcx-1, y+dcy, z) == 16 || GetBlockID(x+dcx-1, y+dcy, z) == 18 || blocks[GetBlockID(x+dcx-1, y+dcy, z)].type == 3)
                                    {
                                        glTexCoordPointer(2, GL_FLOAT, 0, lava_UV_4);
                                        glDrawElements(GL_TRIANGLES, entity_man_head_ind_cnt_4, GL_UNSIGNED_INT, entity_man_head_ind_4);
                                    }

                                    if(GetBlockID(x+dcx, y+dcy, z+1) == 0 || GetBlockID(x+dcx, y+dcy, z+1) == 6 || GetBlockID(x+dcx, y+dcy, z+1) == 7 || GetBlockID(x+dcx, y+dcy, z+1) == 8 || GetBlockID(x+dcx, y+dcy, z+1) == 16 || GetBlockID(x+dcx, y+dcy, z+1) == 18 || blocks[GetBlockID(x+dcx, y+dcy, z+1)].type == 3)
                                    {
                                        glBindTexture(GL_TEXTURE_2D, blocks[world[chunkx][chunky][x][y][z]].texture[1]);
                                        glTexCoordPointer(2, GL_FLOAT, 0, lava_UV_5);
                                        glDrawElements(GL_TRIANGLES, entity_man_head_up_cnt, GL_UNSIGNED_INT, entity_man_head_ind_up);
                                    }

                                    if(GetBlockID(x+dcx, y+dcy, z-1) == 0 || GetBlockID(x+dcx, y+dcy, z-1) == 6 || GetBlockID(x+dcx, y+dcy, z-1) == 7 || GetBlockID(x+dcx, y+dcy, z-1) == 8 || GetBlockID(x+dcx, y+dcy, z-1) == 16 || GetBlockID(x+dcx, y+dcy, z-1) == 18 || blocks[GetBlockID(x+dcx, y+dcy, z-1)].type == 3)
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

                                if(GetBlockID(x+dcx+1, y+dcy, z) == 8) SetBlock(1, x+dcx+1, y+dcy, z);
                                if(GetBlockID(x+dcx-1, y+dcy, z) == 8) SetBlock(1, x+dcx-1, y+dcy, z);
                                if(GetBlockID(x+dcx, y+dcy+1, z) == 8) SetBlock(1, x+dcx, y+dcy+1, z);
                                if(GetBlockID(x+dcx, y+dcy-1, z) == 8) SetBlock(1, x+dcx, y+dcy-1, z);
                                if(GetBlockID(x+dcx, y+dcy, z-1) == 8) SetBlock(1, x+dcx, y+dcy, z-1);
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
                                    if(GetBlockID(x+dcx, y+dcy+1, z) == 0 || GetBlockID(x+dcx, y+dcy+1, z) == 6 || GetBlockID(x+dcx, y+dcy+1, z) == 7 || GetBlockID(x+dcx, y+dcy+1, z) == 8 || GetBlockID(x+dcx, y+dcy+1, z) == 18 || blocks[GetBlockID(x+dcx, y+dcy+1, z)].type == 3)
                                    {
                                        glTexCoordPointer(2, GL_FLOAT, 0, lava_UV);
                                        glDrawElements(GL_TRIANGLES, entity_man_head_ind_cnt_1, GL_UNSIGNED_INT, entity_man_head_ind_1);
                                    }

                                    if(GetBlockID(x+dcx+1, y+dcy, z) == 0 || GetBlockID(x+dcx+1, y+dcy, z) == 6 || GetBlockID(x+dcx+1, y+dcy, z) == 7 || GetBlockID(x+dcx+1, y+dcy, z) == 8 || GetBlockID(x+dcx+1, y+dcy, z) == 18 || blocks[GetBlockID(x+dcx+1, y+dcy, z)].type == 3)
                                    {
                                        glTexCoordPointer(2, GL_FLOAT, 0, lava_UV_2);
                                        glDrawElements(GL_TRIANGLES, entity_man_head_ind_cnt_2, GL_UNSIGNED_INT, entity_man_head_ind_2);
                                    }

                                    if(GetBlockID(x+dcx, y+dcy-1, z) == 0 || GetBlockID(x+dcx, y+dcy-1, z) == 6 || GetBlockID(x+dcx, y+dcy-1, z) == 7 || GetBlockID(x+dcx, y+dcy-1, z) == 8 || GetBlockID(x+dcx, y+dcy-1, z) == 16 || GetBlockID(x+dcx, y+dcy-1, z) == 18 || blocks[GetBlockID(x+dcx, y+dcy-1, z)].type == 3)
                                    {
                                        glTexCoordPointer(2, GL_FLOAT, 0, lava_UV_3);
                                        glDrawElements(GL_TRIANGLES, entity_man_head_ind_cnt_3, GL_UNSIGNED_INT, entity_man_head_ind_3);
                                    }

                                    if(GetBlockID(x+dcx-1, y+dcy, z) == 0 || GetBlockID(x+dcx-1, y+dcy, z) == 6 || GetBlockID(x+dcx-1, y+dcy, z) == 7 || GetBlockID(x+dcx-1, y+dcy, z) == 8 || GetBlockID(x+dcx-1, y+dcy, z) == 16 || GetBlockID(x+dcx-1, y+dcy, z) == 18 || blocks[GetBlockID(x+dcx-1, y+dcy, z)].type == 3)
                                    {
                                        glTexCoordPointer(2, GL_FLOAT, 0, lava_UV_4);
                                        glDrawElements(GL_TRIANGLES, entity_man_head_ind_cnt_4, GL_UNSIGNED_INT, entity_man_head_ind_4);
                                    }

                                    if(GetBlockID(x+dcx, y+dcy, z+1) == 0 || GetBlockID(x+dcx, y+dcy, z+1) == 6 || GetBlockID(x+dcx, y+dcy, z+1) == 7 || GetBlockID(x+dcx, y+dcy, z+1) == 8 || GetBlockID(x+dcx, y+dcy, z+1) == 18 || blocks[GetBlockID(x+dcx, y+dcy, z+1)].type == 3)
                                    {
                                        glTexCoordPointer(2, GL_FLOAT, 0, lava_UV_5);
                                        glDrawElements(GL_TRIANGLES, entity_man_head_up_cnt, GL_UNSIGNED_INT, entity_man_head_ind_up);
                                    }

                                    if(GetBlockID(x+dcx, y+dcy, z-1) == 0 || GetBlockID(x+dcx, y+dcy, z-1) == 6 || GetBlockID(x+dcx, y+dcy, z-1) == 7 || GetBlockID(x+dcx, y+dcy, z-1) == 8 || GetBlockID(x+dcx, y+dcy, z-1) == 16 || GetBlockID(x+dcx, y+dcy, z-1) == 18 || blocks[GetBlockID(x+dcx, y+dcy, z-1)].type == 3)
                                    {
                                        glTranslatef(0.0, 0.0, -1);
                                        glTexCoordPointer(2, GL_FLOAT, 0, lava_UV_updn);
                                        glDrawElements(GL_TRIANGLES, entity_man_head_up_cnt, GL_UNSIGNED_INT, entity_man_head_ind_up);
                                    }
                                glPopMatrix();
                            }
                            if(on_server == FALSE)
                            {
                                int randome = rand();
                                randome %= 500;
                                if(randome == 1 && afk == FALSE)
                                {
                                    if(world[chunkx][chunky][x][y][z] == 3)
                                    {
                                        for(int zz = z+1; zz < 256; zz++)
                                        {
                                            if(world[chunkx][chunky][x][y][zz] != 0 && world[chunkx][chunky][x][y][z+1] != 6 && blocks[world[chunkx][chunky][x][y][z+1]].type != 3) break;
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
                                        if(world[chunkx][chunky][x][y][z+1] != 0 && world[chunkx][chunky][x][y][z+1] != 6 && blocks[world[chunkx][chunky][x][y][z+1]].type != 3)
                                        {
                                            world[chunkx][chunky][x][y][z] = 3;
                                            map_changed = TRUE;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        glDisable(GL_LIGHT0);
        glDisable(GL_LIGHTING);

        glPushMatrix();
            if(Inventory[select_inv].id == 0) glTranslatef(camera.x, camera.y+0.7, camera.z+1.5);
            else glTranslatef(camera.x, camera.y+0.8, camera.z+1.3);
            glRotatef(camera.Xrot, 1,0,0);
            glRotatef(camera.Zrot, 0,0,1);

            glTranslatef(rx, ry, rz);

            if(!is_x && rstep != 4)
            {
                if(rstep == 0) rstep = 1;
                if(rstep == 2) rstep = 3;
            }

            if(is_x && rstep == 4) rstep = 0;

            if(rstep == 0)
            {
                rz -= 0.01;
                rx += 0.01;
                if(rz <= -0.1) rstep = 1;
            }
            if(rstep == 1)
            {
                rz += 0.01;
                rx -= 0.01;
                if(rz >= 0.0)
                {
                    rstep = 2;
                    if(!is_x) rstep = 4;
                }
            }
            if(rstep == 2)
            {
                rz += 0.01;
                rx += 0.01;
                if(rz >= 0.1) rstep = 3;
            }
            if(rstep == 3)
            {
                rz -= 0.01;
                rx -= 0.01;
                if(rz <= 0.0)
                {
                    rstep = 0;
                    rx = 0;
                    rz = 0;
                    if(!is_x) rstep = 4;
                }
            }
            if(Inventory[select_inv].id == 0)
            {
                glRotatef(45, 0,0,1);
                glRotatef(25, 1,0,0);
            }

            glRotatef(-80, 0,1,0);

            if(Inventory[select_inv].id == 0)
            {

                glVertexPointer(3, GL_FLOAT, 0, entity_man_hand);
                glBindTexture(GL_TEXTURE_2D, man_texture);

                glNormalPointer(GL_FLOAT, 0, normal);
                glNormal3f(0,0,1);

                glColor3f(0.7, 0.7, 0.7);

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

            }
            else
            {
                glVertexPointer(3, GL_FLOAT, 0, block_small);
                glBindTexture(GL_TEXTURE_2D, blocks[Inventory[select_inv].id].texture[0]);

                glNormalPointer(GL_FLOAT, 0, normal);
                glNormal3f(0,0,1);

                glColor3f(0.7, 0.7, 0.7);
                glTexCoordPointer(2, GL_FLOAT, 0, lava_UV);
                glDrawElements(GL_TRIANGLES, entity_man_head_ind_cnt_1, GL_UNSIGNED_INT, entity_man_head_ind_1);
                glTexCoordPointer(2, GL_FLOAT, 0, lava_UV_2);
                glDrawElements(GL_TRIANGLES, entity_man_head_ind_cnt_2, GL_UNSIGNED_INT, entity_man_head_ind_2);
                glTexCoordPointer(2, GL_FLOAT, 0, lava_UV_3);
                glDrawElements(GL_TRIANGLES, entity_man_head_ind_cnt_3, GL_UNSIGNED_INT, entity_man_head_ind_3);
                glTexCoordPointer(2, GL_FLOAT, 0, lava_UV_4);
                glDrawElements(GL_TRIANGLES, entity_man_head_ind_cnt_4, GL_UNSIGNED_INT, entity_man_head_ind_4);
                glTexCoordPointer(2, GL_FLOAT, 0, lava_UV_5);
                glDrawElements(GL_TRIANGLES, entity_man_head_up_cnt, GL_UNSIGNED_INT, entity_man_head_ind_up);
                glTranslatef(0.0, 0.0, -0.5);
                glTexCoordPointer(2, GL_FLOAT, 0, lava_UV_updn);
                glDrawElements(GL_TRIANGLES, entity_man_head_up_cnt, GL_UNSIGNED_INT, entity_man_head_ind_up);
            }
        glPopMatrix();

        glEnable(GL_LIGHT0);
        glEnable(GL_LIGHTING);

        for(int j = 0; j < 1500; j++)
        {
            if(camera.x + 9 < Entities[j].x || camera.x - 9 > Entities[j].x || camera.y + 9 < Entities[j].y || camera.y - 9 > Entities[j].y || camera.z + 9 < Entities[j].z || camera.z - 9 > Entities[j].z) continue;
            if(Entities[j].entity_id == 1)
            {
                glColor3f(0.7, 0.7, 0.7);
                glPushMatrix();
                    glTranslatef(Entities[j].x, Entities[j].y, Entities[j].z);

                    if(j >= 499)
                    {
                        glPushMatrix();
                        glTranslatef(0.25, 0, 2.3);
                        glRotatef(camera.Zrot, 0,0,1);
                        int size_nickname = 0;
                        for(int q = 0; q < 512; q++)
                        {
                            if(Entities[j].nickname[q] == '\0') break;
                            size_nickname++;
                        }
                        float ot = 0.15*size_nickname;
                        glTranslatef(-ot, 0, 0);
                        Text_Out3D(Entities[j].nickname);
                    glPopMatrix();
                    }

                    glBindTexture(GL_TEXTURE_2D, man_texture);

                    if(j >= 499)
                    {
                        if(!Entities[j].is_skin_load)
                        {
                            CURL* curl = curl_easy_init();
                            CURLcode response;
                            char skin_path[512];
                            sprintf(skin_path, "skincache\\%s.png", Entities[j].nickname);
                            FILE* file = fopen(skin_path, "wb");
                            if(file == NULL) creat(skin_path, S_IREAD|S_IWRITE);
                            file = fopen(skin_path, "wb");
                            curl_easy_setopt(curl, CURLOPT_URL, Entities[j].skin_url);
                            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
                            curl_easy_setopt(curl, CURLOPT_WRITEDATA, file);
                            curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, TRUE);
                            curl_easy_setopt(curl, CURLOPT_CAINFO, "curl-ca-bundle.crt");
                            response = curl_easy_perform(curl);
                            curl_easy_cleanup(curl);
                            fclose(file);

                            LoadTexture(skin_path, &Entities[j].skin_texture);

                            Entities[j].is_skin_load = TRUE;
                        }
                        glBindTexture(GL_TEXTURE_2D, Entities[j].skin_texture);
                    }

                    glRotatef(Entities[j].Zrot, 0,0,1);

                    glTranslatef(0, Entities[j].leg_left_c, Entities[j].leg_left_z);
                    glRotatef(Entities[j].leg_left, 1,0,0);

                    glVertexPointer(3, GL_FLOAT, 0, entity_man_leg);
                    glNormalPointer(GL_FLOAT, 0, normal);
                    glNormal3f(0,0,1);

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
            float real_lava_UV[] = {0,0+(0.035*anim_lw), 1,0.035+(0.035*anim_lw), 0,0.035+(0.035*anim_lw), 1,0.035+(0.035*anim_lw), 0,0+(0.035*anim_lw), 1,0+(0.035*anim_lw), 0,0+(0.035*anim_lw), 1,0+(0.035*anim_lw)};
            float real_lava_UV_2[] = {0,0+(0.035*anim_lw), 1,0.035+(0.035*anim_lw), 0,0.035+(0.035*anim_lw), 1,0.035+(0.035*anim_lw), 0,0+(0.035*anim_lw), 1,0+(0.035*anim_lw), 0,0+(0.035*anim_lw), 1,0+(0.035*anim_lw)};
            float real_lava_UV_3[] = {0,0.035+(0.035*anim_lw), 1,0.035+(0.035*anim_lw), 0,0.035+(0.035*anim_lw), 1,0.035+(0.035*anim_lw), 0,0+(0.035*anim_lw), 1,0+(0.035*anim_lw), 0,0+(0.035*anim_lw), 1,0+(0.035*anim_lw)};
            float real_lava_UV_4[] = {0,0.035+(0.035*anim_lw), 1,0.035+(0.035*anim_lw), 0,0.035+(0.035*anim_lw), 1,0.035+(0.035*anim_lw), 0,0+(0.035*anim_lw), 1,0+(0.035*anim_lw), 0,0+(0.035*anim_lw), 1,0+(0.035*anim_lw)};
            float real_lava_UV_5[] = {0,0+(0.035*anim_lw), 1,0.035+(0.035*anim_lw), 1,0+(0.035*anim_lw), 0,0+(0.035*anim_lw), 0,0.035+(0.035*anim_lw), 1,0.035+(0.035*anim_lw), 1,0+(0.035*anim_lw), 0,0+(0.035*anim_lw)};
            float real_lava_UV_updn[] = {0,0.035+(0.035*anim_lw), 1,0.035+(0.035*anim_lw), 1,0+(0.035*anim_lw), 0,0+(0.035*anim_lw), 0,0.035+(0.035*anim_lw), 1,0.035+(0.035*anim_lw), 1,0+(0.035*anim_lw), 0,0+(0.035*anim_lw)};
            int x = LavaVisible[w].x;
            int y = LavaVisible[w].y;
            int z = LavaVisible[w].z;
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glVertexPointer(3, GL_FLOAT, 0, block);
            glNormalPointer(GL_FLOAT, 0, normal);
            glNormal3f(0,0,1);
            if(GetBlockID(x, y, z) != 7 && GetBlockID(x, y, z) != 8) continue;
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
                    glTexCoordPointer(2, GL_FLOAT, 0, real_lava_UV);
                    glDrawElements(GL_TRIANGLES, entity_man_head_ind_cnt_1, GL_UNSIGNED_INT, entity_man_head_ind_1);
                }

                if(GetBlockID(x+1, y, z) == 0)
                {
                    glTexCoordPointer(2, GL_FLOAT, 0, real_lava_UV_2);
                    glDrawElements(GL_TRIANGLES, entity_man_head_ind_cnt_2, GL_UNSIGNED_INT, entity_man_head_ind_2);
                }

                if(GetBlockID(x, y-1, z) == 0)
                {
                    glTexCoordPointer(2, GL_FLOAT, 0, real_lava_UV_3);
                    glDrawElements(GL_TRIANGLES, entity_man_head_ind_cnt_3, GL_UNSIGNED_INT, entity_man_head_ind_3);
                }

                if(GetBlockID(x-1, y, z) == 0)
                {
                    glTexCoordPointer(2, GL_FLOAT, 0, real_lava_UV_4);
                    glDrawElements(GL_TRIANGLES, entity_man_head_ind_cnt_4, GL_UNSIGNED_INT, entity_man_head_ind_4);
                }

                if(GetBlockID(x, y, z+1) == 0)
                {
                    glTexCoordPointer(2, GL_FLOAT, 0, real_lava_UV_5);
                    glDrawElements(GL_TRIANGLES, entity_man_head_up_cnt, GL_UNSIGNED_INT, entity_man_head_ind_up);
                }

                if(GetBlockID(x, y, z-1) == 0)
                {
                    glTranslatef(0.0, 0.0, -1);
                    glTexCoordPointer(2, GL_FLOAT, 0, real_lava_UV_updn);
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
                if(world[chunkx][chunky][X][Y][Z] != 7 && world[chunkx][chunky][X][Y][Z] != 8)
                {
                    if(world[chunkx][chunky][X][Y][Z] != 9)
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
                        HSAMPLE sampdes;
                        HCHANNEL chdes;
                        if(world[chunkx][chunky][X][Y][Z] >= 19 && world[chunkx][chunky][X][Y][Z] <= 34 && set_sound)
                        {
                            if(!sampstep || BASS_ChannelIsActive(chstep) == BASS_ACTIVE_STOPPED)
                            {
                                char filename[] = "sounds\\step\\cloth.ogg";
                                sampdes = BASS_SampleLoad(FALSE, filename, 0, 0, 1, BASS_SAMPLE_MONO);
                                chdes = BASS_SampleGetChannel(sampdes, FALSE);
                                BASS_ChannelPlay(chdes, FALSE);
                            }
                        }
                        if(world[chunkx][chunky][X][Y][Z] == 18 && set_sound)
                        {
                            if(!sampstep || BASS_ChannelIsActive(chstep) == BASS_ACTIVE_STOPPED)
                            {
                                char filename[] = "sounds\\step\\glass.ogg";
                                sampdes = BASS_SampleLoad(FALSE, filename, 0, 0, 1, BASS_SAMPLE_MONO);
                                chdes = BASS_SampleGetChannel(sampdes, FALSE);
                                BASS_ChannelPlay(chdes, FALSE);
                            }
                        }
                        if(world[chunkx][chunky][X][Y][Z] == 2 && set_sound)
                        {
                            if(!sampstep || BASS_ChannelIsActive(chstep) == BASS_ACTIVE_STOPPED)
                            {
                                char filename[] = "sounds\\step\\grass.ogg";
                                sampstep = BASS_SampleLoad(FALSE, filename, 0, 0, 1, BASS_SAMPLE_MONO);
                                chstep = BASS_SampleGetChannel(sampstep, FALSE);
                                BASS_ChannelPlay(chstep, FALSE);
                            }
                        }
                        world[chunkx][chunky][X][Y][Z] = 0;
                        map_changed = TRUE;
                    }
                    if(on_server == FALSE)
                    {
                        UpdateChunk(chunkx, chunky);
                        if(X == 0) UpdateChunk(chunkx-1, chunky);
                        if(X == 15) UpdateChunk(chunkx+1, chunky);
                        if(Y == 0) UpdateChunk(chunkx, chunky-1);
                        if(Y == 15) UpdateChunk(chunkx, chunky+1);
                    }
                    else
                    {
                        char buff[512];
                        for(int iw = 0; iw < 512; iw++)
                        {
                            buff[iw] = 0;
                        }
                        sprintf(buff, "ops set_block %d %d %d 0", X+dcx, Y+dcy, Z);
                        if(SOCKET_ERROR == (send(s, &buff, sizeof(buff), 0)))
                        {
                            server_error = TRUE;
                            sprintf(ser_error_text, "����������� ������");
                        }
                    }
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
                if(world[chunkx][chunky][X][Y][Z] == 0 || world[chunkx][chunky][X][Y][Z] == 7 || world[chunkx][chunky][X][Y][Z] == 8)
                {
                    if((int)camera.x == (int)bx && (int)camera.y == (int)by && (int)camera.z == (int)bz) return;
                    if((int)camera.x == (int)bx && (int)camera.y == (int)by && (int)camera.z+1 == (int)bz) return;
                    world[chunkx][chunky][X][Y][Z] = Inventory[select_inv].id;
                    map_changed = TRUE;
                    if(on_server == FALSE)
                    {
                        UpdateChunk(chunkx, chunky);
                        if(X == 0) UpdateChunk(chunkx-1, chunky);
                        if(X == 15) UpdateChunk(chunkx+1, chunky);
                        if(Y == 0) UpdateChunk(chunkx, chunky-1);
                        if(Y == 15) UpdateChunk(chunkx, chunky+1);
                    }
                    else
                    {
                        char buff[512];
                        for(int iw = 0; iw < 512; iw++)
                        {
                            buff[iw] = 0;
                        }
                        sprintf(buff, "ops set_block %d %d %d %d", X+dcx, Y+dcy, Z, world[chunkx][chunky][X][Y][Z]);
                        if(SOCKET_ERROR == (send(s, &buff, sizeof(buff), 0)))
                        {
                            server_error = TRUE;
                            sprintf(ser_error_text, "����������� ������");
                        }
                    }
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
    FILE *file_spawn;
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

    sprintf(path, "%s\\saves\\world\\spawn.dat", buffer);

    file_spawn = fopen(path, "w");

    if(file_spawn == NULL) creat(path, S_IREAD|S_IWRITE);

    file_spawn = fopen(path, "w");

    fwrite(&spawn, 1, sizeof(spawn), file_spawn);
    fclose(file_spawn);


    sprintf(path, "%s\\saves\\world\\world.ocw", buffer);

    file_world = fopen(path, "w");

    if(file_world == NULL) creat(path, S_IREAD|S_IWRITE);

    file_world = fopen(path, "w");

    fwrite(world, sizeof(world), 1, file_world);
    fclose(file_world);


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
    if(Entities[j].entity_id == 0) return 0;
    if(j < 500)
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

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glVertexPointer(2, GL_FLOAT, 0, panel_fast);
    glTexCoordPointer(2, GL_FLOAT, 0, panel_texture);
    glBindTexture(GL_TEXTURE_2D, widgets_texture);
    glPushMatrix();
        glTranslatef(scrSize.x / 2 - 182.5, scrSize.y - 45, 0);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glPopMatrix();

    for(int i = 0; i < 9; i++)
    {
        curx = scrSize.x / 2 - 177 + (35.75*i);
        cury = scrSize.y - 30;

        glBindTexture(GL_TEXTURE_2D, blocks[Inventory[i].id].texture[0]);

        if(blocks[Inventory[i].id].type != 3) glVertexPointer(2, GL_FLOAT, 0, block_inventory);
        else glVertexPointer(2, GL_FLOAT, 0, block_inventory_flat);
        glTexCoordPointer(2, GL_FLOAT, 0, block_inventory_texture);
        glPushMatrix();
            if(blocks[Inventory[i].id].type != 3) glTranslatef(curx, cury, 0);
            else glTranslatef(curx, cury-8, 0);
            glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
        glPopMatrix();

        if(blocks[Inventory[i].id].type != 3)
        {
            glVertexPointer(2, GL_FLOAT, 0, block_inventory_2);
            glTexCoordPointer(2, GL_FLOAT, 0, block_inventory_texture);
            glPushMatrix();
                glTranslatef(curx, cury, 0);
                glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
            glPopMatrix();

            if(blocks[Inventory[i].id].type == 2) glBindTexture(GL_TEXTURE_2D, blocks[Inventory[i].id].texture[1]);

            glVertexPointer(2, GL_FLOAT, 0, block_inventory_3);
            glTexCoordPointer(2, GL_FLOAT, 0, block_inventory_texture);
            glPushMatrix();
                glTranslatef(curx, cury, 0);
                glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
            glPopMatrix();
        }

        glVertexPointer(2, GL_FLOAT, 0, panel_select_fast);
        glTexCoordPointer(2, GL_FLOAT, 0, panel_select_texture);
        glBindTexture(GL_TEXTURE_2D, widgets_texture);
        glPushMatrix();
            glTranslatef(scrSize.x / 2 - 187.5 + (39*select_inv) - 3*select_inv, scrSize.y - 46.5, 0);
            glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
        glPopMatrix();
    }

    glDisable(GL_BLEND);

    for(int i = 0; i < 10; i++)
    {
        if(hp > 2*i)
        {
            glVertexPointer(2, GL_FLOAT, 0, heart);
            glTexCoordPointer(2, GL_FLOAT, 0, heart_texture);
            glBindTexture(GL_TEXTURE_2D, icons_texture);
            glPushMatrix();
                glTranslatef(scrSize.x / 2 - 182.5 + i*16, scrSize.y - 65, 0);
                glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
            glPopMatrix();
        }
        else if(hp == 2*i)
        {
            glVertexPointer(2, GL_FLOAT, 0, heart);
            glTexCoordPointer(2, GL_FLOAT, 0, heart_double_texture);
            glBindTexture(GL_TEXTURE_2D, icons_texture);
            glPushMatrix();
                glTranslatef(scrSize.x / 2 - 182.5 + i*16, scrSize.y - 65, 0);
                glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
            glPopMatrix();
        }
        else
        {
            glVertexPointer(2, GL_FLOAT, 0, heart);
            glTexCoordPointer(2, GL_FLOAT, 0, heart_null_texture);
            glBindTexture(GL_TEXTURE_2D, icons_texture);
            glPushMatrix();
                glTranslatef(scrSize.x / 2 - 182.5 + i*16, scrSize.y - 65, 0);
                glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
            glPopMatrix();
        }
    }

    glPushMatrix();
        glTranslatef(0,0,0);
        Text_Out(OPENCRAFT_VERSION, 0);
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
        sprintf(line, "%d fps, %d ���������� ������", fps, update_chunks);
        if(set_showfps) Text_Out(line, 0);
    glPopMatrix();

    for(int c = 0; c < 10; c++)
    {
        if(chat[c].dVisible == TRUE && chat_open == FALSE) break;
        glPushMatrix();
            glTranslatef(5, scrSize.y-39-(15*(c+1)), 0);
            if(!chat[c].join_message) Text_Out(chat[c].text, 0);
            else Text_Out(chat[c].text, 2);
        glPopMatrix();
    }

    glColor3f(0.7, 0.7, 0.7);

    if(GetKeyState(VK_TAB) < 0 && afk == FALSE && chat_open == FALSE && on_server == TRUE && open_inventory == FALSE)
    {
        glPushMatrix();
            glTranslatef(scrSize.x / 2 - 250, scrSize.y / 2 - 150, 0);
            glDisable(GL_TEXTURE_2D);
            glVertexPointer(2, GL_FLOAT, 0, player_list);
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glColor4f(0, 0, 0, 0.7);
            glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
            glDisable(GL_BLEND);
            glColor3f(0.7, 0.7, 0.7);
            glEnable(GL_TEXTURE_2D);
            glTranslatef(145, 3, 0);
            int cool = 0;
            int columb = 0;
            Text_Out("������������ ������:", 0);
            glTranslatef(-135, 20, 0);
            Text_Out(nickname, 0);
            cool++;
            glTranslatef(0, 15, 0);
            for(int qwe = 499; qwe < 1500; qwe++)
            {
                if(cool >= 18)
                {
                    glTranslatef(160, -270, 0);
                    cool = 0;
                    columb++;
                    if(columb >= 3) break;
                }
                if(strlen(Entities[qwe].nickname) > 0)
                {
                    char nick[32];
                    for(int n = 0; n < 15; n++)
                    {
                        nick[n] = Entities[qwe].nickname[n];
                    }
                    nick[15] = '\0';
                    Text_Out(nick, 0);
                    glTranslatef(0, 15, 0);
                    cool++;
                }
            }
        glPopMatrix();
    }

    if(open_inventory == TRUE)
    {
        glPushMatrix();
            glTranslatef(scrSize.x / 2 - 240, scrSize.y / 2 - 150, 0);
            glDisable(GL_TEXTURE_2D);
            glVertexPointer(2, GL_FLOAT, 0, creative_inventory);
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glColor4f(0, 0, 0, 0.7);
            glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
            glDisable(GL_BLEND);
            glColor3f(0.7, 0.7, 0.7);
            glEnable(GL_TEXTURE_2D);
            glTranslatef(175, 3, 0);
            int cool = 0;
            int columb = 0;
            Text_Out("�������� ����", 0);
            glTranslatef(-165, 30, 0);
            for(int i = 1; i < 256; i++)
            {
                glColor3f(0.7, 0.7, 0.7);
                if(blocks[i].type == 0) break;
                if(i == 7 || i == 8 || i == 9 || i == 2) continue;

                if(selectCreateInventory == i) glColor3f(1, 1, 1);

                createInventoryVertex[i].id = i;
                createInventoryVertex[i].vertex[0] = scrSize.x / 2 - 230 + (60*cool);
                createInventoryVertex[i].vertex[1] = scrSize.y / 2 - 117 + (50*columb) - 8;
                createInventoryVertex[i].vertex[2] = scrSize.x / 2 - 230 + (60*cool) + 30;
                createInventoryVertex[i].vertex[3] = scrSize.y / 2 - 117 + (50*columb) + 30 - 8;

                glBindTexture(GL_TEXTURE_2D, blocks[i].texture[0]);

                if(blocks[i].type != 3) glVertexPointer(2, GL_FLOAT, 0, block_inventory);
                else glVertexPointer(2, GL_FLOAT, 0, block_inventory_flat);
                glTexCoordPointer(2, GL_FLOAT, 0, block_inventory_texture);
                glPushMatrix();
                    if(blocks[i].type == 3) glTranslatef(0, -8, 0);
                    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
                    if(blocks[i].type == 3) glTranslatef(0, 8, 0);
                glPopMatrix();

                if(blocks[i].type != 3)
                {
                    glVertexPointer(2, GL_FLOAT, 0, block_inventory_2);
                    glTexCoordPointer(2, GL_FLOAT, 0, block_inventory_texture);
                    glPushMatrix();
                        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
                    glPopMatrix();

                    if(blocks[i].type == 2) glBindTexture(GL_TEXTURE_2D, blocks[i].texture[1]);

                    glVertexPointer(2, GL_FLOAT, 0, block_inventory_3);
                    glTexCoordPointer(2, GL_FLOAT, 0, block_inventory_texture);
                    glPushMatrix();
                        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
                    glPopMatrix();
                }
                cool++;
                if(cool == 8)
                {
                    glTranslatef(-480, 50, 0);
                    cool = 0;
                    columb++;
                }
                glTranslatef(60, 0, 0);
                glColor3f(0.7, 0.7, 0.7);
            }
        glPopMatrix();
    }

    if(chat_open == TRUE && type_chat != 2)
    {
        glPushMatrix();
            glTranslatef(5, scrSize.y-29, 0);
            glDisable(GL_TEXTURE_2D);
            glVertexPointer(2, GL_FLOAT, 0, chat_enter);
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glColor4f(0, 0, 0, 0.5);
            glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
            glDisable(GL_BLEND);
            glColor3f(0.7, 0.7, 0.7);
            glEnable(GL_TEXTURE_2D);
            glTranslatef(0, 4, 0);
            Text_Out(">", 0);
            glTranslatef(20, 0, 0);
            Text_Out(chat_string, 0);
            if(chat_size != 0) glTranslatef(10*chat_size, 0, 0);
            if(show_chat_cursor == TRUE) Text_Out("_", 0);
        glPopMatrix();
    }

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
                glTranslatef((scrSize.x / 2) - 45, scrY - 95, 0);
                Text_Out("���� ����", 0);
            glPopMatrix();
            if(on_server == TRUE)
            {
                Buttons[0].state = 2;
                Buttons[1].state = 2;
                Buttons[2].state = 2;
            }
            ShowButton("���������", sizeof("���������") - 1, scrX, scrY-65, 8);
            ShowButton("������� ����� ���", sizeof("������� ����� ���") - 1, scrX, scrY, 0);
            ShowButton("��������� ���", sizeof("��������� ���") - 1, scrX, scrY+65, 1);
            ShowButton("��������� ���", sizeof("��������� ���") - 1, scrX, scrY+130, 2);
            ShowButton("��������� � ����", sizeof("��������� � ����") - 1, scrX, scrY+260, 3);

            glPopMatrix();
        }
        else if(menu_page == 1)
        {
            glPushMatrix();
                glTranslatef((scrSize.x / 2) - 85, scrY - 80, 0);
                Text_Out("������� ����� ���", 0);
            glPopMatrix();
            ShowButton("���������", sizeof("���������") - 1, scrX, scrY, 4);
            ShowButton("����������", sizeof("����������") - 1, scrX, scrY+65, 5);
            ShowButton("�������", sizeof("�������") - 1, scrX, scrY+130, 6);
            ShowButton("�����", sizeof("�����") - 1, scrX, scrY+195, 7);

            glPopMatrix();
        }
        else if(menu_page == 2)
        {
            glPushMatrix();
                glTranslatef((scrSize.x / 2) - 45, scrY - 95, 0);
                Text_Out("���������", 0);
            glPopMatrix();
            if(set_music) ShowButton("������: ���.", sizeof("������: ���.") - 1, scrX, scrY-65, 9);
            else ShowButton("������: ����.", sizeof("������: ����.") - 1, scrX, scrY-65, 9);
            if(set_sound) ShowButton("����: ���.", sizeof("����: ���.") - 1, scrX, scrY-15, 10);
            else ShowButton("����: ����.", sizeof("����: ����.") - 1, scrX, scrY-15, 10);
            if(inverted_y) ShowButton("�������������� ����: ���.", sizeof("�������������� ����: ���.") - 1, scrX, scrY+35, 11);
            else ShowButton("�������������� ����: ����.", sizeof("�������������� ����: ����.") - 1, scrX, scrY+35, 11);
            if(set_showfps) ShowButton("���������� FPS: ���.", sizeof("���������� FPS: ���.") - 1, scrX, scrY+85, 12);
            else ShowButton("���������� FPS: ����.", sizeof("���������� FPS: ����.") - 1, scrX, scrY+85, 12);
            if(render_distance == 1) ShowButton("��������� ����������: ����� �������", sizeof("��������� ����������: ����� �������") - 1, scrX, scrY+135, 13);
            else if(render_distance == 2) ShowButton("��������� ����������: �������", sizeof("��������� ����������: �������") - 1, scrX, scrY+135, 13);
            else if(render_distance == 3) ShowButton("��������� ����������: ����������", sizeof("��������� ����������: ����������") - 1, scrX, scrY+135, 13);
            else ShowButton("��������� ����������: �������", sizeof("��������� ����������: �������") - 1, scrX, scrY+135, 13);
            ShowButton("����������", sizeof("����������") - 1, scrX, scrY+200, 14);
            ShowButton("������", sizeof("������") - 1, scrX, scrY+270, 15);

            glPopMatrix();
        }
        else
        {
            glPushMatrix();
                glTranslatef((scrSize.x / 2) - 50, scrY - 95, 0);
                Text_Out("����������", 0);
            glPopMatrix();
            char sw[100];
            BOOL addL = 5;
            if(conW == ' ') sprintf(sw, "�����: ������", conW);
            else if(conW == VK_RETURN) sprintf(sw, "�����: Enter", conW);
            else { sprintf(sw, "�����: %c", conW); addL = 0; }
            ShowButton(sw, 9 + addL, scrX, scrY-65, 16);
            addL = 5;
            if(conA == ' ') sprintf(sw, "�����: ������");
            else if(conA == VK_RETURN) sprintf(sw, "�����: Enter");
            else { sprintf(sw, "�����: %c", conA); addL = 0; }
            ShowButton(sw, 7 + addL, scrX, scrY-15, 17);
            addL = 5;
            if(conD == ' ') sprintf(sw, "������: ������");
            else if(conD == VK_RETURN) sprintf(sw, "������: Enter");
            else { sprintf(sw, "������: %c", conD); addL = 0; }
            ShowButton(sw, 8 + addL, scrX, scrY+35, 18);
            addL = 5;
            if(conS == ' ') sprintf(sw, "�����: ������");
            else if(conS == VK_RETURN) sprintf(sw, "�����: Enter");
            else { sprintf(sw, "�����: %c", conS); addL = 0; }
            ShowButton(sw, 7 + addL, scrX, scrY+85, 19);
            addL = 5;
            if(conSpace == ' ') sprintf(sw, "������: ������");
            else if(conSpace == VK_RETURN) sprintf(sw, "������: Enter");
            else { sprintf(sw, "������: %c", conSpace); addL = 0; }
            ShowButton(sw, 8 + addL, scrX, scrY+135, 20);
            addL = 5;
            if(conR == ' ') sprintf(sw, "����������������� �� �����: ������");
            else if(conR == VK_RETURN) sprintf(sw, "����������������� �� �����: Enter");
            else { sprintf(sw, "����������������� �� �����: %c", conR); addL = 0; }
            ShowButton(sw, 28 + addL, scrX, scrY+185, 21);
            addL = 5;
            if(conEnter == ' ') sprintf(sw, "�������� �����: ������");
            else if(conEnter == VK_RETURN) sprintf(sw, "�������� �����: Enter");
            else { sprintf(sw, "�������� �����: %c", conEnter); addL = 0; }
            ShowButton(sw, 16 + addL, scrX, scrY+235, 22);
            ShowButton("������", sizeof("������") - 1, scrX, scrY+285, 24);
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

void Text_Out(char *text, int type)
{
    glPushMatrix();
        glBindTexture(GL_TEXTURE_2D, ascii_texture);
        glVertexPointer(2, GL_FLOAT, 0, rectCoord);
        glTexCoordPointer(2, GL_FLOAT, 0, rectTex);
        if(type == 0) glColor3f(1, 1, 1);
        else if(type == 1) glColor3f(1, 0, 0);
        else glColor3f(1, 1, 0);

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
                            if(GetBlockID(x+dcx, y+dcy, z) != 16 && GetBlockID(x+dcx+1, y+dcy, z) != 16 && GetBlockID(x+dcx-1, y+dcy, z) != 16 && GetBlockID(x+dcx, y+dcy+1, z) != 16 && GetBlockID(x+dcx, y+dcy-1, z) != 16 && GetBlockID(x+dcx, y+dcy, z+1) != 16 && GetBlockID(x+dcx, y+dcy, z-1) != 16)
                            {
                                if(GetBlockID(x+dcx, y+dcy, z) != 18 && GetBlockID(x+dcx+1, y+dcy, z) != 18 && GetBlockID(x+dcx-1, y+dcy, z) != 18 && GetBlockID(x+dcx, y+dcy+1, z) != 18 && GetBlockID(x+dcx, y+dcy-1, z) != 18 && GetBlockID(x+dcx, y+dcy, z+1) != 18 && GetBlockID(x+dcx, y+dcy, z-1) != 18)
                                {
                                    if(blocks[GetBlockID(x+dcx, y+dcy, z)].type != 3 && blocks[GetBlockID(x+dcx+1, y+dcy, z)].type != 3 && blocks[GetBlockID(x+dcx-1, y+dcy, z)].type != 3 && blocks[GetBlockID(x+dcx, y+dcy+1, z)].type != 3 && blocks[GetBlockID(x+dcx, y+dcy-1, z)].type != 3 && blocks[GetBlockID(x+dcx, y+dcy, z+1)].type != 3 && blocks[GetBlockID(x+dcx, y+dcy, z-1)].type != 3) world_visible[chunkx][chunky][x][y][z] = FALSE;
                                    else world_visible[chunkx][chunky][x][y][z] = TRUE;
                                }
                                else world_visible[chunkx][chunky][x][y][z] = TRUE;
                            }
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

void GenMenu_Show(char secondLine[], int slSize, int type)
{
    glColor3f(1, 1, 1);
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
        if(type == 0)
        {
            curx = (scrSize.x / 2) - ((sizeof("�������� ����") - 1)*10 / 2);
            glTranslatef(curx, (scrSize.y / 2) - 20, 0);
            Text_Out("�������� ����", 0);
        }
        else if(type == 1)
        {
            curx = (scrSize.x / 2) - ((sizeof("�������� ����") - 1)*10 / 2);
            glTranslatef(curx, (scrSize.y / 2) - 20, 0);
            Text_Out("�������� ����", 0);
        }
        else if(type == 2)
        {
            curx = (scrSize.x / 2) - ((sizeof("������ �����������") - 1)*10 / 2);
            glTranslatef(curx, (scrSize.y / 2) - 20, 0);
            Text_Out("������ �����������", 1);
        }
        else if(type == 3)
        {
            int size_name = 0;
            for(int q = 0; q < 512; q++)
            {
                if(server.server_name[q] == '\0') break;
                size_name++;
            }
            curx = (scrSize.x / 2) - (size_name*10 / 2);;
            glTranslatef(curx, (scrSize.y / 2) - 20, 0);
            Text_Out(server.server_name, 0);
        }
    glPopMatrix();
    glPushMatrix();
        curx = (scrSize.x / 2) - ((slSize - 1)*10 / 2);
        glTranslatef(curx, (scrSize.y / 2) + 5, 0);
        Text_Out(secondLine, 0);
    glPopMatrix();

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
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
        else glTexCoordPointer(2, GL_FLOAT, 0, button_coord_closed);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
        int curx = (scrSize.x / 2) - (((tSize)*10) / 2);
        float cury = (45 / 2) - 10;
    glPopMatrix();
        glPushMatrix();
            glTranslatef(curx, y+cury, 0);
            Text_Out(name, 0);
        glPopMatrix();
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

void WaitMessages()
{
    char messages[1000];
    while(bQuit == FALSE)
    {
        int actual_len;
        char buff[512];
        for(int iw = 0; iw < 512; iw++)
        {
            buff[iw] = 0;
        }
        if(server_error == TRUE) continue;
        if(SOCKET_ERROR == (actual_len = recv(s, (char*) &buff, 512, 0)))
        {
            server_error = TRUE;
            sprintf(ser_error_text, "���������� ��������");
            sprintf(ser_error_text_output, "���������� ��������");
        }
        else
        {
            if(buff[4] == 'm' && buff[14] == 's')
            {
                server_error = TRUE;
                sprintf(ser_error_text, "������ ��������!");
            }
            else if(buff[4] == 's' && buff[14] == 'e')
            {
                int cnt = 0;
                for(int ie = 16; ie < 512; ie++)
                {
                    if(buff[ie] == '\0')
                    {
                        server.server_name[cnt] = '\0';
                        break;
                    }
                    server.server_name[cnt] = buff[ie];
                    cnt++;
                }
            }
            else if(buff[4] == 'm' && buff[7] == 'd')
            {
                int cnt = 0;
                for(int ie = 9; ie < 512; ie++)
                {
                    if(buff[ie] == '\0') break;
                    server.motd[cnt] = buff[ie];
                    cnt++;
                }
            }
            else if(buff[4] == 'w' && buff[12] == 'e')
            {
                char worldsize[3];
                for(int ie = 0; ie < 3; ie++)
                {
                    worldsize[ie] = buff[ie+14];
                }
                worldsizex = atoi(worldsize);
                worldsizey = worldsizex;
            }
            else if(buff[4] == 's' && buff[12] == 'k')
            {
                int x_i, y_i, z_i, block_i;
                char x_t[10];
                char y_t[10];
                char z_t[10];
                char block_t[10];
                int now = 0;
                int cnt = 0;
                for(int iw = 0; iw < 10; iw++)
                {
                    x_t[iw] = 0;
                    y_t[iw] = 0;
                    z_t[iw] = 0;
                    block_t[iw] = 0;
                }
                now = 0;
                cnt = 0;
                for(int e = 14; e < 512; e++)
                {
                    if(buff[e] == ' ' && now == 3 || buff[e] == '\0') break;
                    if(buff[e] == ' ')
                    {
                        now++;
                        cnt = 0;
                        continue;
                    }
                    if(now == 0) x_t[cnt] = buff[e];
                    else if(now == 1) y_t[cnt] = buff[e];
                    else if(now == 2) z_t[cnt] = buff[e];
                    else block_t[cnt] = buff[e];
                    cnt++;
                }
                x_i = atoi(x_t);
                y_i = atoi(y_t);
                z_i = atoi(z_t);
                block_i = atoi(block_t);
                int chunkx = x_i / 16;
                int chunky = y_i / 16;
                int dcx = 16*chunkx;
                int dcy = 16*chunky;
                int fx = x_i - dcx;
                int fy = y_i - dcy;
                world[chunkx][chunky][fx][fy][z_i] = block_i;
                if(block_i == 0 && server_loaded == TRUE)
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
                                    Sprites[i].x[a] = (int)x_i + (0.3 * xf);
                                    Sprites[i].y[a] = (int)y_i;
                                    Sprites[i].z[a] = (int)z_i + (0.3 * zf);
                                    a++;
                                }
                            }

                            Sprites[i].block_id = block_i;
                            Sprites[i].step = 0;
                            break;
                        }
                    }
                }
            }
            else if(buff[4] == 'w' && buff[20] == 'd')
            {
                for(int iw = 0; iw < 512; iw++)
                {
                    buff[iw] = 0;
                }
                sprintf(buff, "ops get_spawn");
                if(SOCKET_ERROR == (send(s, &buff, sizeof(buff), 0)))
                {
                    server_error = TRUE;
                    sprintf(ser_error_text, "����������� ������");
                }
            }
            else if(buff[4] == 's' && buff[8] == 'n')
            {
                float x_f, y_f, z_f, Xrot_f, Zrot_f;
                char x_t[10], y_t[10], z_t[10], Xrot_t[10], Zrot_t[10];
                int now = 0;
                int cnt = 0;
                for(int iw = 0; iw < 10; iw++)
                {
                    x_t[iw] = 0;
                    y_t[iw] = 0;
                    z_t[iw] = 0;
                }
                for(int e = 10; e < 512; e++)
                {
                    if(buff[e] == ' ' && now == 4 || buff[e] == '\0') break;
                    if(buff[e] == ' ')
                    {
                        now++;
                        cnt = 0;
                        continue;
                    }
                    if(now == 0) x_t[cnt] = buff[e];
                    else if(now == 1) y_t[cnt] = buff[e];
                    else if(now == 2) z_t[cnt] = buff[e];
                    else if(now == 3) Xrot_t[cnt] = buff[e];
                    else Zrot_t[cnt] = buff[e];
                    cnt++;
                }
                x_f = atof(x_t);
                y_f = atof(y_t);
                z_f = atof(z_t);
                Xrot_f = atof(Xrot_t);
                Zrot_f = atof(Zrot_t);
                camera.x = x_f;
                camera.y = y_f;
                camera.z = z_f;
                camera.Xrot = Xrot_f;
                camera.Zrot = Zrot_f;
                spawn.x = x_f;
                spawn.y = y_f;
                spawn.z = z_f;
                spawn.Xrot = Xrot_f;
                spawn.Zrot = Zrot_f;
                for(int iw = 0; iw < 512; iw++)
                {
                    buff[iw] = 0;
                }
                sprintf(buff, "ops get_mobs");
                if(SOCKET_ERROR == (send(s, &buff, sizeof(buff), 0)))
                {
                    server_error = TRUE;
                    sprintf(ser_error_text, "����������� ������");
                }
            }
            else if(buff[4] == 's' && buff[16] == 'd')
            {
                float x_f, y_f, z_f, Xrot_f, Zrot_f;
                int eid_i, id_i;
                char x_t[10], y_t[10], z_t[10], Xrot_t[10], Zrot_t[10], eid_t[10], id_t[10];
                int now = 0;
                int cnt = 0;
                for(int e = 10; e < 512; e++)
                {
                    if(buff[e] == ' ' && now == 7) break;
                    if(buff[e] == '\0') break;
                    if(buff[e] == ' ')
                    {
                        now++;
                        cnt = 0;
                        continue;
                    }
                    if(now == 0) x_t[cnt] = buff[e];
                    else if(now == 1) y_t[cnt] = buff[e];
                    else if(now == 2) z_t[cnt] = buff[e];
                    else if(now == 3) Xrot_t[cnt] = buff[e];
                    else if(now == 4) Zrot_t[cnt] = buff[e];
                    else if(now == 5) eid_t[cnt] = buff[e];
                    else id_t[cnt] = buff[e];
                    cnt++;
                }
                x_f = atof(x_t);
                y_f = atof(y_t);
                z_f = atof(z_t);
                Xrot_f = atof(Xrot_t);
                Zrot_f = atof(Zrot_t);
                eid_i = atoi(eid_t);
                id_i = atoi(id_t);
                Entities[id_i].x = x_f;
                Entities[id_i].y = y_f;
                Entities[id_i].z = z_f;
                Entities[id_i].Xrot = Xrot_f;
                Entities[id_i].Zrot = Zrot_f;
            }
            else if(buff[4] == 'm' && buff[18] == 'd')
            {
                server_loaded = TRUE;
                cursorShow = FALSE;
                SetCursorPos((int)rctb.left + scrSize.x/2, (int)rctb.top + scrSize.y/2);
                while (ShowCursor(FALSE) >= 0);
            }
            else if(buff[4] == 'g' && buff[14] == 'n')
            {
                for(int iw = 0; iw < 512; iw++)
                {
                    buff[iw] = 0;
                }
                sprintf(buff, "ops version %s", OPENCRAFT_VERSION);
                if(SOCKET_ERROR == (send(s, &buff, sizeof(buff), 0)))
                {
                    server_error = TRUE;
                    sprintf(ser_error_text, "����������� ������");
                }
            }
            else if(buff[4] == 'b' && buff[14] == 'n')
            {
                server_error = TRUE;
                sprintf(ser_error_text, "����������� ������");
            }
            else if(buff[4] == 's' && buff[11] == 'd')
            {
                float x_f, y_f, z_f, Xrot_f, Zrot_f;
                int id_i;
                char x_t[10];
                char y_t[10];
                char z_t[10];
                char Xrot_t[10];
                char Zrot_t[10];
                char id_t[10];
                char nicknamet[512];
                char skint[512];
                int now = 0;
                int cnt = 0;
                for(int iw = 0; iw < 10; iw++)
                {
                    x_t[iw] = 0;
                    y_t[iw] = 0;
                    z_t[iw] = 0;
                    Xrot_t[iw] = 0;
                    Zrot_t[iw] = 0;
                    id_t[iw] = 0;
                    nicknamet[iw] = 0;
                    skint[iw] = 0;
                }
                now = 0;
                cnt = 0;
                for(int e = 13; e < 512; e++)
                {
                    if(buff[e] == ' ' && now == 7 || buff[e] == '\0') break;
                    if(buff[e] == ' ')
                    {
                        now++;
                        cnt = 0;
                        continue;
                    }
                    if(now == 0) x_t[cnt] = buff[e];
                    else if(now == 1) y_t[cnt] = buff[e];
                    else if(now == 2) z_t[cnt] = buff[e];
                    else if(now == 3) Xrot_t[cnt] = buff[e];
                    else if(now == 4) Zrot_t[cnt] = buff[e];
                    else if(now == 5) id_t[cnt] = buff[e];
                    else if(now == 6) nicknamet[cnt] = buff[e];
                    else skint[cnt] = buff[e];
                    cnt++;
                }
                x_f = atof(x_t);
                y_f = atof(y_t);
                z_f = atof(z_t);
                Xrot_f = atof(Xrot_t);
                Zrot_f = atof(Zrot_t);
                id_i = atoi(id_t);
                Entities[499+id_i].x = x_f;
                Entities[499+id_i].y = y_f;
                Entities[499+id_i].z = z_f;
                Entities[499+id_i].Xrot = Xrot_f;
                Entities[499+id_i].Zrot = Zrot_f;
                Entities[499+id_i].entity_id = 1;
                strcpy(Entities[499+id_i].nickname, nicknamet);
                strcpy(Entities[499+id_i].skin_url, skint);
            }
            else if(buff[4] == 'g' && buff[15] == 'e')
            {
                for(int iw = 0; iw < 512; iw++)
                {
                    buff[iw] = 0;
                }
                sprintf(buff, "ops nickname %s", nickname);
                if(SOCKET_ERROR == (send(s, &buff, sizeof(buff), 0)))
                {
                    server_error = TRUE;
                    sprintf(ser_error_text, "����������� ������");
                }
            }
            else if(buff[4] == 'c' && buff[15] == 'e')
            {
                BOOL jm = FALSE;
                char chat_msg[512];
                int cnt = 0;
                for(int ie = 17; ie < 512; ie++)
                {
                    if(buff[ie] == '\0')
                    {
                        chat_msg[cnt] = '\0';
                        break;
                    }
                    if(buff[ie] == '~' && buff[ie+1] == 'y')
                    {
                        jm = TRUE;
                        ie += 2;
                    }
                    chat_msg[cnt] = buff[ie];
                    cnt++;
                }
                ChatUpdate();
                strcpy(chat[0].text, chat_msg);
                if(jm) chat[0].join_message = TRUE;
                chat_max++;
                lastTimeChat = GetTickCount() * 0.001f;
                chat[0].dVisible = FALSE;
                for(int q = chat_max; q >= 0; q--)
                {
                    chat[q].dVisible = FALSE;
                }
            }
            else if(buff[4] == 'b' && buff[9] == 'd')
            {
                server_error = TRUE;
                sprintf(ser_error_text, "�� �������� �� ���� �������");
                sprintf(ser_error_text_output, "�� �������� �� ���� �������");
            }
            else if(buff[4] == 'k' && buff[9] == 'd')
            {
                server_error = TRUE;
                sprintf(ser_error_text, "��� ������ ��������");
                sprintf(ser_error_text_output, "��� ������ ��������");
            }
            else if(buff[4] == 'r' && buff[14] == 'd')
            {
                enter_password = TRUE;
            }
            else if(buff[4] == 'b' && buff[15] == 'd')
            {
                server_error = TRUE;
                sprintf(ser_error_text, "�� ����� �������� ������");
                sprintf(ser_error_text_output, "�� ����� �������� ������");
            }
            else if(buff[4] == 's' && buff[7] == 'm')
            {
                server_error = TRUE;
                sprintf(ser_error_text, "�� ���� ������� �� ����");
                sprintf(ser_error_text_output, "�� ���� ������� �� ����");
            }
            else if(buff[4] == 'g' && buff[11] == 'n')
            {
                for(int iw = 0; iw < 512; iw++)
                {
                    buff[iw] = 0;
                }
                sprintf(buff, "ops skin %s", skin);
                if(SOCKET_ERROR == (send(s, &buff, sizeof(buff), 0)))
                {
                    server_error = TRUE;
                    sprintf(ser_error_text, "����������� ������");
                }
            }
            else if(buff[4] == 'd' && buff[15] == 'd')
            {
                int cnt = 0;
                char idt[512];
                for(int ie = 17; ie < 512; ie++)
                {
                    if(buff[ie] == '\0')
                    {
                        idt[cnt] = '\0';
                        break;
                    }
                    idt[cnt] = buff[ie];
                    cnt++;
                }
                int id = atoi(idt);
                Entities[499+id].entity_id = 0;
                Entities[499+id].is_skin_load = FALSE;
            }
            else if(buff[4] == 't' && buff[5] == 'p')
            {
                char x_t[10];
                char y_t[10];
                char z_t[10];
                int now = 0;
                int cnt = 0;
                for(int iw = 0; iw < 10; iw++)
                {
                    x_t[iw] = 0;
                    y_t[iw] = 0;
                    z_t[iw] = 0;
                }
                now = 0;
                cnt = 0;
                for(int e = 7; e < 512; e++)
                {
                    if(buff[e] == ' ' && now == 2 || buff[e] == '\0') break;
                    if(buff[e] == ' ')
                    {
                        now++;
                        cnt = 0;
                        continue;
                    }
                    if(now == 0) x_t[cnt] = buff[e];
                    else if(now == 1) y_t[cnt] = buff[e];
                    else z_t[cnt] = buff[e];
                    cnt++;
                }
                float x_f = atof(x_t);
                float y_f = atof(y_t);
                float z_f = atof(z_t);
                camera.x = x_f;
                camera.y = y_f;
                camera.z = z_f;
            }
        }
    }
    return 0;
}

void SendCameraPos()
{
    char buff[512];
    for(int iw = 0; iw < 512; iw++)
    {
        buff[iw] = 0;
    }
    sprintf(buff, "ops setcoord %f %f %f %f %f", camera.x, camera.y, camera.z, camera.Xrot, camera.Zrot);
    if(SOCKET_ERROR == (send(s, &buff, sizeof(buff), 0)))
    {
        server_error = TRUE;
        sprintf(ser_error_text, "����������� ������");
    }
}

void ChatUpdate()
{
    for(int i = 0; i < 512; i++)
    {
        chat[9].text[i] = 0;
    }
    chat[9].join_message = FALSE;
    for(int q = 9; q >= 0; q--)
    {
        strcpy(chat[q].text, chat[q-1].text);
        if(chat[q-1].join_message) chat[q].join_message = TRUE;
        chat[q-1].join_message = FALSE;
    }
    for(int i = 0; i < 512; i++)
    {
        chat[0].text[i] = 0;
    }
}

void Text_Out3D(char *text)
{
    glPushMatrix();
        glBindTexture(GL_TEXTURE_2D, ascii_texture);
        glVertexPointer(3, GL_FLOAT, 0, nickname_vertex);
        glTexCoordPointer(2, GL_FLOAT, 0, rectTex);
        glColor3f(1, 1, 1);

        static float charSize = 1/16.0;
        while(*text)
        {
            char c = *text;
            int y = c >> 4;
            int x = c & 0b1111;
            struct {float left, right, top, bottom} rct;
            rct.left = x * charSize;
            rct.right = rct.left + charSize;
            rct.top = y * charSize;
            rct.bottom = rct.top + charSize;

            rectTex[0] = rectTex[6] = rct.left;
            rectTex[2] = rectTex[4] = rct.right;
            rectTex[1] = rectTex[3] = rct.bottom;
            rectTex[5] = rectTex[7] = rct.top;

            glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
            text++;
            glTranslatef(0.3,0,0);
        }
    glPopMatrix();
}

void DirtBackgroundShow()
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

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

void SaveSettings()
{
    FILE *settings;

    settings = fopen("options.txt", "wt");

    if(settings == NULL)
    {
        creat("options.txt", S_IREAD|S_IWRITE);
        settings = fopen("options.txt", "wt");
    }

    fprintf(settings, "music:%s\n", set_music ? "true" : "false");
    fprintf(settings, "sound:%s\n", set_sound ? "true" : "false");
    fprintf(settings, "invertYMouse:%s\n", inverted_y ? "true" : "false");
    fprintf(settings, "showFrameRate:%s\n", set_showfps ? "true" : "false");
    fprintf(settings, "viewDistance:%d\n", render_distance);
    fprintf(settings, "key_Forward:%d\n", conW);
    fprintf(settings, "key_Left:%d\n", conA);
    fprintf(settings, "key_Right:%d\n", conD);
    fprintf(settings, "key_Back:%d\n", conS);
    fprintf(settings, "key_Load location:%d\n", conR);
    fprintf(settings, "key_Save location:%d\n", conEnter);
    fprintf(settings, "key_Chat:%d\n", conT);
    fprintf(settings, "key_Jump:%d", conSpace);

    fclose(settings);
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

    TCHAR buffer[MAX_PATH];
    GetCurrentDirectory(sizeof(buffer), buffer);
    char path[MAX_PATH];
    sprintf(path, "%s\\skincache", buffer);
    if(!DirectoryExists(path)) mkdir(path);

    char argument[1000];
    char value[1000];

    FILE *server_ip;

    char sp[1000];

    server_ip = fopen("server.dat", "rt");

    if(server_ip != NULL)
    {
        while(!feof(server_ip))
        {
            fgets(sp, 1000, server_ip);
            if(sp[0] == '#') continue;
            BOOL exit = FALSE;
            for(int i = 0; i < 1000; i++)
            {
                if(sp[i] == '=')
                {
                    for(int iw = 0; iw < 1000; iw++)
                    {
                        argument[iw] = 0;
                        value[iw] = 0;
                    }
                    for(int iq = 0; iq < i; iq++)
                    {
                        argument[iq] = sp[iq];
                    }
                    int cnt = 0;
                    for(int ie = i+1; ie < 1000; ie++)
                    {
                        if(sp[ie] == '\n') break;
                        value[cnt] = sp[ie];
                        cnt++;
                    }
                    if(strcmp(argument, "server-ip") == 0)
                    {
                        strcpy(server.ip, value);
                    }
                    else if(strcmp(argument, "port") == 0)
                    {
                        server.port = atoi(value);
                    }
                    else if(strcmp(argument, "nickname") == 0)
                    {
                        if(strlen(value) <= 32) strcpy(nickname, value);
                        else sprintf(nickname, "Man");
                    }
                    else if(strcmp(argument, "skin") == 0)
                    {
                        strcpy(skin, value);
                    }
                    exit = TRUE;
                }
                if(exit == TRUE) break;
            }
        }
        fclose(server_ip);
    }

    if(server.port != 0 && strlen(server.ip) != 0)
    {
        on_server = TRUE;
        if(FAILED(WSAStartup(MAKEWORD(1, 1), &ws)))
        {
            on_server = FALSE;
        }
        if(INVALID_SOCKET == (s = socket(AF_INET, SOCK_STREAM, 0)))
        {
            on_server = FALSE;
        }

        SOCKADDR_IN adr;
        ZeroMemory(&adr, sizeof(adr));
        adr.sin_family = AF_INET;
        adr.sin_addr.S_un.S_addr = inet_addr(server.ip);
        adr.sin_port = htons(server.port);

        if(SOCKET_ERROR == (connect(s, (SOCKADDR *) &adr, sizeof(adr))))
        {
            on_server = FALSE;
        }
        else
        {
            pthread_t thread;
            int status;
            int status_addr;

            status = pthread_create(&thread, NULL, WaitMessages, NULL);

            if(status != 0)
            {
                server_error = TRUE;
                sprintf(ser_error_text, "������ �������� ������");
            }
        }
    }


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
                if(on_server == TRUE) closesocket(s);
                BASS_Free();
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

            static float lastTime3 = 0.0f;
            float currentTime3 = GetTickCount() * 0.001f;
            if(currentTime3 - lastTime3 > 0.1f)
            {
                anim_lw++;
                if(anim_lw > 27) anim_lw = 0;
                lastTime3 = currentTime3;
            }

            static float lastTimemm = 0.0f;
            float currentTimemm = GetTickCount() * 0.001f;
            if(currentTimemm - lastTimemm > 300.0f && set_music)
            {
                char filename[] = "music\\calm1.ogg";
                mus = BASS_SampleLoad(FALSE, filename, 0, 0, 1, BASS_SAMPLE_MONO);
                chmus = BASS_SampleGetChannel(mus, FALSE);
                BASS_ChannelPlay(chmus, FALSE);
                lastTimemm = currentTimemm;
            }

            if(on_server == FALSE)
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

                is_x = FALSE;

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
                                if(jump_down == TRUE && world[chunkx][chunky][(int)xjump][(int)yjump][(int)camera.z] != 0 && world[chunkx][chunky][(int)xjump][(int)yjump][(int)camera.z] != 6 && blocks[world[chunkx][chunky][(int)xjump][(int)yjump][(int)camera.z]].type != 3)
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
                        if(GetBlockID((int)camera.x, (int)camera.y, (int)camera.z) == 7 && GetKeyState(conSpace) < 0 && timer_pl <= 0 || GetBlockID((int)camera.x, (int)camera.y, (int)camera.z) == 8 && GetKeyState(conSpace) < 0 && timer_pl <= 0)
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
                            if(!is_jumping && afk == FALSE && !fly)
                            {
                                camera.z -= 0.3;
                                is_fly = TRUE;
                            }
                        }
                        else if(world[chunkx][chunky][(int)xjump][(int)yjump][(int)cameraz] == 0 && !is_jumping || world[chunkx][chunky][(int)xjump][(int)yjump][(int)cameraz] == 6 && !is_jumping || world[chunkx][chunky][(int)xjump][(int)yjump][(int)cameraz] == 7 && !is_jumping || world[chunkx][chunky][(int)xjump][(int)yjump][(int)cameraz] == 8 && !is_jumping || blocks[world[chunkx][chunky][(int)xjump][(int)yjump][(int)cameraz]].type == 3 && !is_jumping)
                        {
                            if(afk == FALSE)
                            {
                                if(GetBlockID((int)camera.x, (int)camera.y, (int)camera.z - 1) != 7 && GetBlockID((int)camera.x, (int)camera.y, (int)camera.z - 1) != 8 && !fly)
                                {
                                    camera.z -= 0.3;
                                    is_fly = TRUE;
                                }
                                else if(GetBlockID((int)camera.x, (int)camera.y, (int)camera.z - 1) == 7 && is_up == FALSE && !fly)
                                {
                                    camera.z -= 0.02;
                                    is_fly = TRUE;
                                }
                                else if(GetBlockID((int)camera.x, (int)camera.y, (int)camera.z - 1) == 8 && is_up == FALSE && !fly)
                                {
                                    camera.z -= 0.02;
                                    is_fly = TRUE;
                                }
                            }
                        }
                        if(chunkx <= cx && chunky <= cy && chunkx >= 0 && chunky >= 0 && afk == FALSE && chat_open == FALSE)
                        {
                            if(GetKeyState(conSpace) < 0 && !is_jumping && world[chunkx][chunky][(int)xjump][(int)yjump][(int)cameraz] != 0 && camera.x > 0 && camera.x < worldsizex && camera.y > 0 && camera.y < worldsizey && camera.z >= 0 && camera.z < 256 && !is_fly && ((int)camera.z - cameraz) == 1 && timer <= 0 && space == FALSE && !fly)
                            {
                                if(GetBlockID((int)camera.x, (int)camera.y, (int)camera.z) != 7 && GetBlockID((int)camera.x, (int)camera.y, (int)camera.z) != 8)
                                {
                                    is_jumping = TRUE;
                                    camera_z_in_jump = camera.z;
                                    space = TRUE;
                                }
                            }
                            else if(GetBlockID((int)camera.x, (int)camera.y, (int)camera.z+2) != 0 && fly && GetKeyState(conSpace) < 0)
                            {
                                is_up = TRUE;
                                float z_new = camera.z + 0.03;
                                camera.z = z_new;
                            }
                        }

                        if(GetBlockID((int)camera.x, (int)camera.y, (int)camera.z - 1) != 0 && GetBlockID((int)camera.x, (int)camera.y, (int)camera.z - 1) != 6 && !is_fly && !is_jumping && !is_up && !fly)
                        {
                            float new_z = camera.z - 0.3;
                            if((int)new_z < (int)camera.z) camera.z = (int)camera.z;
                            else
                            {
                                camera.z -= 0.3;
                            }
                        }

                        if(GetKeyState(conR) < 0 && timer_r <= 0 && afk == FALSE && chat_open == FALSE && open_inventory == FALSE)
                        {
                            camera.x = spawn.x;
                            camera.y = spawn.y;
                            camera.z = spawn.z;
                            camera.Xrot = spawn.Xrot;
                            camera.Zrot = spawn.Zrot;
                        }
                        if(GetKeyState('G') < 0 && timer_g <= 0 && afk == FALSE && chat_open == FALSE && open_inventory == FALSE)
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
                        if(GetKeyState('Y') < 0 && timer_y <= 0 && afk == FALSE && chat_open == FALSE && open_inventory == FALSE)
                        {
                            if(!inverted_y) inverted_y = TRUE;
                            else inverted_y = FALSE;
                            timer_y = 10;
                        }
                        if(GetKeyState('F') < 0 && timer_f <= 0 && afk == FALSE && chat_open == FALSE && open_inventory == FALSE)
                        {
                            if(render_distance < 4) render_distance++;
                            else render_distance = 1;
                            glFogf(GL_FOG_START, 8.0f*render_distance);
                            glFogf(GL_FOG_END, 9.0f*render_distance);
                            timer_f = 10;
                        }
                        if(GetKeyState('N') < 0 && timer_n <= 0 && afk == FALSE && map_changed == TRUE && chat_open == FALSE && open_inventory == FALSE)
                        {
                            SaveWorld();
                            timer_n = 10;
                        }
                        if(GetKeyState(conEnter) < 0 && timer_enter <= 0 && afk == FALSE && chat_open == FALSE && open_inventory == FALSE)
                        {
                            spawn.x = camera.x;
                            spawn.y = camera.y;
                            spawn.z = camera.z;
                            spawn.Xrot = camera.Xrot;
                            spawn.Zrot = camera.Zrot;
                            timer_enter = 10;
                        }
                        if(GetKeyState('B') < 0 && timer_b <= 0 && afk == FALSE && chat_open == FALSE)
                        {
                            open_inventory = TRUE;
                            timer_b = 10;
                            cursorShow = TRUE;
                            while (ShowCursor(TRUE) <= 0);
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
                    timer_enter--;
                    timer_b--;
                    timer_del_chunks--;
                }
                if(GetKeyState(conSpace) > 0) space = FALSE;
                static float lastTimeLM = 0.0f;
                float currentTimeLM = GetTickCount() * 0.001f;
                if(currentTimeLM - lastTimeLM > 0.25f)
                {
                    lastTimeLM = currentTimeLM;
                    lbutton_timer = FALSE;
                }
                if(GetKeyState(VK_LBUTTON) < 0 && lbutton_timer == FALSE)
                {
                    if(afk == FALSE && chat_open == FALSE  && open_inventory == FALSE)
                    {
                        PlayerSetBlock();
                        lbutton_timer = TRUE;
                    }
                }
                if(is_x == FALSE) BASS_ChannelStop(chstep);
            }
            else
            {
                if(server_error == TRUE)
                {
                    cursorShow = TRUE;
                    while (ShowCursor(TRUE) <= 0);
                    int size = 0;
                    for(int q = 0; q < 512; q++)
                    {
                        if(ser_error_text_output[q] == '\0') break;
                        size++;
                    }
                    GenMenu_Show(ser_error_text_output, size+1, 2);
                    SwapBuffers(hDC);
                }
                else if(server_loaded == FALSE)
                {
                    if(!enter_password)
                    {
                        cursorShow = TRUE;
                        while (ShowCursor(TRUE) <= 0);
                        int size_motd = 0;
                        for(int q = 0; q < 512; q++)
                        {
                            if(server.motd[q] == '\0') break;
                            size_motd++;
                        }
                        GenMenu_Show(server.motd, size_motd, 3);
                        SwapBuffers(hDC);
                    }
                    else
                    {
                        static float lastTimec = 0.0f;
                        type_chat = 1;
                        float currentTimec = GetTickCount() * 0.001f;
                        if(currentTimec - lastTimec > 0.5f)
                        {
                            lastTimec = currentTimec;
                            show_chat_cursor = !show_chat_cursor;
                        }
                        cursorShow = TRUE;
                        chat_open = TRUE;
                        while (ShowCursor(TRUE) <= 0);
                        DirtBackgroundShow();
                        glTranslatef(scrSize.x / 2 - 202.5, scrSize.y / 2 - 22.5, 0);
                        glEnableClientState(GL_VERTEX_ARRAY);
                        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
                        glPushMatrix();
                            glDisable(GL_TEXTURE_2D);
                            glVertexPointer(2, GL_FLOAT, 0, button);
                            glColor3f(0, 0, 0);
                            glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
                            glColor3f(1, 1, 1);
                            glDrawArrays(GL_LINE_LOOP, 0, 4);
                            glEnable(GL_TEXTURE_2D);
                            glColor3f(0.7, 0.7, 0.7);
                        glPopMatrix();
                        glTranslatef(0, -30, 0);
                        Text_Out("������� ������:", 0);
                        glTranslatef(5, 45, 0);
                        Text_Out(chat_string, 0);
                        if(chat_size != 0) glTranslatef(10*chat_size, 0, 0);
                        if(show_chat_cursor == TRUE) Text_Out("_", 0);
                        SwapBuffers(hDC);
                        glDisableClientState(GL_VERTEX_ARRAY);
                        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
                    }

                    if(!sendGetWorld)
                    {
                        char buff[512];
                        for(int iw = 0; iw < 512; iw++)
                        {
                            buff[iw] = 0;
                        }
                        sprintf(buff, "ops get_world");
                        if(SOCKET_ERROR == (send(s, &buff, sizeof(buff), 0)))
                        {
                            server_error = TRUE;
                            sprintf(ser_error_text, "����������� ������");
                        }
                        sendGetWorld = TRUE;
                    }
                }
                else
                {
                    glPushMatrix();
                    Game_Show();
                    Menu_Show();

                glPopMatrix();

                SwapBuffers(hDC);

                is_x = FALSE;
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
                            if (GetForegroundWindow() == hwnd && afk == FALSE && chat_open == FALSE && open_inventory == FALSE) Player_Move();
                        glPopMatrix();
                        for(int qwe = 500; qwe < 1500; qwe++) EntityAI(qwe);
                        int chunkx = camera.x / 16;
                        int chunky = camera.y / 16;
                        int cameraz = (int)camera.z - 1;
                        int jdcx = 16*chunkx;
                        int jdcy = 16*chunky;
                        float xjump = camera.x - jdcx;
                        float yjump = camera.y - jdcy;

                        BOOL is_fly = FALSE;
                        BOOL is_up = FALSE;

                        if(is_jumping)
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
                                if(jump_down == TRUE && world[chunkx][chunky][(int)xjump][(int)yjump][(int)camera.z] != 0 && world[chunkx][chunky][(int)xjump][(int)yjump][(int)camera.z] != 6 && blocks[world[chunkx][chunky][(int)xjump][(int)yjump][(int)cameraz]].type != 3)
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
                        if(GetBlockID((int)camera.x, (int)camera.y, (int)camera.z) == 7 && GetKeyState(conSpace) < 0 && timer_pl <= 0 || GetBlockID((int)camera.x, (int)camera.y, (int)camera.z) == 8 && GetKeyState(conSpace) < 0 && timer_pl <= 0)
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
                            if(!is_jumping && !fly)
                            {
                                camera.z -= 0.3;
                                is_fly = TRUE;
                            }
                        }
                        else if(world[chunkx][chunky][(int)xjump][(int)yjump][(int)cameraz] == 0 && !is_jumping || world[chunkx][chunky][(int)xjump][(int)yjump][(int)cameraz] == 6 && !is_jumping || world[chunkx][chunky][(int)xjump][(int)yjump][(int)cameraz] == 7 && !is_jumping || world[chunkx][chunky][(int)xjump][(int)yjump][(int)cameraz] == 8 && !is_jumping || blocks[world[chunkx][chunky][(int)xjump][(int)yjump][(int)cameraz]].type == 3 && !is_jumping)
                        {
                            if(GetBlockID((int)camera.x, (int)camera.y, (int)camera.z - 1) != 7 && GetBlockID((int)camera.x, (int)camera.y, (int)camera.z - 1) != 8 && !fly)
                            {
                                camera.z -= 0.3;
                                is_fly = TRUE;
                            }
                            else if(GetBlockID((int)camera.x, (int)camera.y, (int)camera.z - 1) == 7 && is_up == FALSE && !fly)
                            {
                                camera.z -= 0.02;
                                is_fly = TRUE;
                            }
                            else if(GetBlockID((int)camera.x, (int)camera.y, (int)camera.z - 1) == 8 && is_up == FALSE && !fly)
                            {
                                camera.z -= 0.02;
                                is_fly = TRUE;
                            }
                        }
                        if(chunkx <= cx && chunky <= cy && chunkx >= 0 && chunky >= 0 && afk == FALSE && chat_open == FALSE && open_inventory == FALSE)
                        {
                            if(GetKeyState(conSpace) < 0 && !is_jumping && world[chunkx][chunky][(int)xjump][(int)yjump][(int)cameraz] != 0 && camera.x > 0 && camera.x < worldsizex && camera.y > 0 && camera.y < worldsizey && camera.z >= 0 && camera.z < 256 && !is_fly && ((int)camera.z - cameraz) == 1 && timer <= 0 && space == FALSE)
                            {
                                if(GetBlockID((int)camera.x, (int)camera.y, (int)camera.z) != 7 && GetBlockID((int)camera.x, (int)camera.y, (int)camera.z) != 8)
                                {
                                    is_jumping = TRUE;
                                    camera_z_in_jump = camera.z;
                                    space = TRUE;
                                }
                            }
                        }

                        if(GetBlockID((int)camera.x, (int)camera.y, (int)camera.z - 1) != 0 && GetBlockID((int)camera.x, (int)camera.y, (int)camera.z - 1) != 6 && !is_fly && !is_jumping && !is_up && !fly)
                        {
                            float new_z = camera.z - 0.3;
                            if((int)new_z < (int)camera.z) camera.z = (int)camera.z;
                            else
                            {
                                camera.z -= 0.3;
                            }
                        }

                        if(GetKeyState('R') < 0 && timer_r <= 0 && afk == FALSE && chat_open == FALSE && open_inventory == FALSE)
                        {
                            camera.x = spawn.x;
                            camera.y = spawn.y;
                            camera.z = spawn.z;
                            camera.Xrot = spawn.Xrot;
                            camera.Zrot = spawn.Zrot;
                        }
                        if(GetKeyState('Y') < 0 && timer_y <= 0 && afk == FALSE && chat_open == FALSE && open_inventory == FALSE)
                        {
                            if(!inverted_y) inverted_y = TRUE;
                            else inverted_y = FALSE;
                            timer_y = 10;
                        }
                        if(GetKeyState('F') < 0 && timer_f <= 0 && afk == FALSE && chat_open == FALSE && open_inventory == FALSE)
                        {
                            if(render_distance < 4) render_distance++;
                            else render_distance = 1;
                            glFogf(GL_FOG_START, 8.0f*render_distance);
                            glFogf(GL_FOG_END, 9.0f*render_distance);
                            timer_f = 10;
                        }
                        if(GetKeyState(conT) < 0 && afk == FALSE && chat_open == FALSE && open_inventory == FALSE)
                        {
                            type_chat = 0;
                            chat_open = TRUE;
                            cursorShow = TRUE;
                            for(int wrt = 0; wrt < 256; wrt++)
                            {
                                chat_string[wrt] = 0;
                            }
                            chat_size = 0;
                            while (ShowCursor(TRUE) <= 0);
                        }
                        if(GetKeyState('B') < 0 && timer_b <= 0 && afk == FALSE && chat_open == FALSE)
                        {
                            open_inventory = TRUE;
                            timer_b = 10;
                            cursorShow = TRUE;
                            while (ShowCursor(TRUE) <= 0);
                        }
                        SpritesManage();

                        theta += 0,1080000108;
                    }
                    timer--;
                    timer_r--;
                    timer_g--;
                    timer_y--;
                    timer_f--;
                    timer_n--;
                    timer_pl--;
                    timer_enter--;
                    timer_b--;
                    timer_del_chunks--;
                }
                if(GetKeyState(conSpace) > 0) space = FALSE;
                static float lastTimeLM = 0.0f;
                float currentTimeLM = GetTickCount() * 0.001f;
                if(currentTimeLM - lastTimeLM > 0.25f)
                {
                    lastTimeLM = currentTimeLM;
                    lbutton_timer = FALSE;
                }

                static float lastTimec = 0.0f;
                float currentTimec = GetTickCount() * 0.001f;
                if(currentTimec - lastTimec > 0.5f)
                {
                    lastTimec = currentTimec;
                    show_chat_cursor = !show_chat_cursor;
                }

                float currentTimechat = GetTickCount() * 0.001f;
                if(currentTimechat - lastTimeChat > 10.0f)
                {
                    lastTimeChat = currentTimechat;
                    if(chat_max >= 0)
                    {
                        chat[chat_max].dVisible = TRUE;
                        chat_max--;
                    }
                }

                if(GetKeyState(VK_LBUTTON) < 0 && lbutton_timer == FALSE)
                {
                    if(afk == FALSE && chat_open == FALSE && open_inventory == FALSE)
                    {
                        PlayerSetBlock();
                        lbutton_timer = TRUE;
                    }
                }
                if(camera.x != old_x || camera.y != old_y || camera.z != old_z || camera.Xrot != old_Xrot || camera.Zrot != old_Zrot) SendCameraPos();
                old_x = camera.x;
                old_y = camera.y;
                old_z = camera.z;
                old_Xrot = camera.Xrot;
                old_Zrot = camera.Zrot;
                if(is_x == FALSE) BASS_ChannelStop(chstep);
                }
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
            if(on_server == FALSE) SaveWorld();
            SaveSettings();
            PostQuitMessage(0);
        break;

        case WM_SIZE:
            WndResize(LOWORD(lParam), HIWORD(lParam));
        break;

        case WM_RBUTTONDOWN:
            if(afk == FALSE && chat_open == FALSE && open_inventory == FALSE)
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
                    if(CursorInButton(LOWORD(lParam), HIWORD(lParam), 0) && Buttons[0].state != 2) menu_page = 1;
                    if(CursorInButton(LOWORD(lParam), HIWORD(lParam), 2) && Buttons[2].state != 2)
                    {
                        SaveWorld();
                        afk = FALSE;
                        cursorShow = FALSE;
                        SetCursorPos((int)rctb.left + scrSize.x/2, (int)rctb.top + scrSize.y/2);
                        while (ShowCursor(FALSE) >= 0);
                    }
                    if(CursorInButton(LOWORD(lParam), HIWORD(lParam), 3))
                    {
                        afk = FALSE;
                        cursorShow = FALSE;
                        SetCursorPos((int)rctb.left + scrSize.x/2, (int)rctb.top + scrSize.y/2);
                        while (ShowCursor(FALSE) >= 0);
                    }
                    if(CursorInButton(LOWORD(lParam), HIWORD(lParam), 8) && Buttons[8].state != 2) menu_page = 2;
                }
                else if(menu_page == 1)
                {
                    if(CursorInButton(LOWORD(lParam), HIWORD(lParam), 4) && Buttons[4].state != 2)
                    {
                        worldsizex = 128;
                        worldsizey = 128;
                        menu_page = 0;
                        GenerateNewWorld();
                    }
                    if(CursorInButton(LOWORD(lParam), HIWORD(lParam), 5) && Buttons[5].state != 2)
                    {
                        worldsizex = 256;
                        worldsizey = 256;
                        menu_page = 0;
                        GenerateNewWorld();
                    }
                    if(CursorInButton(LOWORD(lParam), HIWORD(lParam), 6) && Buttons[6].state != 2)
                    {
                        worldsizex = 512;
                        worldsizey = 512;
                        menu_page = 0;
                        GenerateNewWorld();
                    }
                    if(CursorInButton(LOWORD(lParam), HIWORD(lParam), 7) && Buttons[7].state != 2)
                    {
                        menu_page = 0;
                    }
                }
                else if(menu_page == 2)
                {
                    if(CursorInButton(LOWORD(lParam), HIWORD(lParam), 9) && Buttons[9].state != 2)
                    {
                        set_music = !set_music;
                        BASS_ChannelStop(chmus);
                    }
                    if(CursorInButton(LOWORD(lParam), HIWORD(lParam), 10) && Buttons[10].state != 2)
                    {
                        set_sound = !set_sound;
                    }
                    if(CursorInButton(LOWORD(lParam), HIWORD(lParam), 11) && Buttons[11].state != 2)
                    {
                        inverted_y = !inverted_y;
                    }
                    if(CursorInButton(LOWORD(lParam), HIWORD(lParam), 12) && Buttons[12].state != 2)
                    {
                        set_showfps = !set_showfps;
                    }
                    if(CursorInButton(LOWORD(lParam), HIWORD(lParam), 13) && Buttons[13].state != 2)
                    {
                        if(render_distance < 4) render_distance++;
                        else render_distance = 1;
                        glFogf(GL_FOG_START, 8.0f*render_distance);
                        glFogf(GL_FOG_END, 9.0f*render_distance);
                    }
                    if(CursorInButton(LOWORD(lParam), HIWORD(lParam), 15) && Buttons[15].state != 2) menu_page = 0;
                    if(CursorInButton(LOWORD(lParam), HIWORD(lParam), 14) && Buttons[14].state != 2) menu_page = 3;
                }
                else
                {
                    if(CursorInButton(LOWORD(lParam), HIWORD(lParam), 16) && Buttons[16].state != 2)
                    {
                        chat_open = TRUE;
                        type_chat = 2;
                        selCon = 16;
                        conW = 0;
                    }
                    if(CursorInButton(LOWORD(lParam), HIWORD(lParam), 17) && Buttons[17].state != 2)
                    {
                        chat_open = TRUE;
                        type_chat = 2;
                        selCon = 17;
                        conA = 0;
                    }
                    if(CursorInButton(LOWORD(lParam), HIWORD(lParam), 18) && Buttons[18].state != 2)
                    {
                        chat_open = TRUE;
                        type_chat = 2;
                        selCon = 18;
                        conD = 0;
                    }
                    if(CursorInButton(LOWORD(lParam), HIWORD(lParam), 19) && Buttons[19].state != 2)
                    {
                        chat_open = TRUE;
                        type_chat = 2;
                        selCon = 19;
                        conS = 0;
                    }
                    if(CursorInButton(LOWORD(lParam), HIWORD(lParam), 20) && Buttons[20].state != 2)
                    {
                        chat_open = TRUE;
                        type_chat = 2;
                        selCon = 20;
                        conSpace = 0;
                    }
                    if(CursorInButton(LOWORD(lParam), HIWORD(lParam), 21) && Buttons[21].state != 2)
                    {
                        chat_open = TRUE;
                        type_chat = 2;
                        selCon = 21;
                        conR = 0;
                    }
                    if(CursorInButton(LOWORD(lParam), HIWORD(lParam), 22) && Buttons[22].state != 2)
                    {
                        chat_open = TRUE;
                        type_chat = 2;
                        selCon = 22;
                        conEnter = 0;
                    }
                    if(CursorInButton(LOWORD(lParam), HIWORD(lParam), 23) && Buttons[23].state != 2)
                    {
                        chat_open = TRUE;
                        type_chat = 2;
                        selCon = 23;
                        conT = 0;
                    }
                    if(CursorInButton(LOWORD(lParam), HIWORD(lParam), 24) && Buttons[24].state != 2 && chat_open == FALSE) menu_page = 2;
                }
            }
            if(open_inventory == TRUE)
            {
                if(selectCreateInventory != -1)
                {
                    Inventory[select_inv].id = selectCreateInventory;
                    open_inventory = FALSE;
                    cursorShow = FALSE;
                    SetCursorPos((int)rctb.left + scrSize.x/2, (int)rctb.top + scrSize.y/2);
                    while (ShowCursor(FALSE) >= 0);
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
                case VK_SPACE:
                {
                    static float lastTimeLS = 0.0f;
                    if(lastTimeLS == 0.0f) lastTimeLS = GetTickCount() * 0.001f;
                    float currentTimeLS = GetTickCount() * 0.001f;
                    if(currentTimeLS - lastTimeLS > 0.5f)
                    {
                        lastTimeLS = currentTimeLS;
                    }
                    else
                    {
                        fly = !fly;
                    }
                    break;
                }
                case VK_ESCAPE:
                {
                    if(afk == FALSE && chat_open == FALSE && open_inventory == FALSE)
                    {
                        afk = TRUE;
                        cursorShow = TRUE;
                        while (ShowCursor(TRUE) <= 0);
                    }
                    else if(chat_open == TRUE)
                    {
                        chat_open = FALSE;
                        cursorShow = FALSE;
                        SetCursorPos((int)rctb.left + scrSize.x/2, (int)rctb.top + scrSize.y/2);
                        while (ShowCursor(FALSE) >= 0);
                    }
                    else if(open_inventory == TRUE)
                    {
                        open_inventory = FALSE;
                        cursorShow = FALSE;
                        SetCursorPos((int)rctb.left + scrSize.x/2, (int)rctb.top + scrSize.y/2);
                        while (ShowCursor(FALSE) >= 0);
                    }
                    else if(afk == TRUE)
                    {
                        afk = FALSE;
                        cursorShow = FALSE;
                        SetCursorPos((int)rctb.left + scrSize.x/2, (int)rctb.top + scrSize.y/2);
                        while (ShowCursor(FALSE) >= 0);
                    }
                    break;
                }
            }
            if(wParam == VK_F1 || wParam == VK_F2 || wParam == VK_F3 || wParam == VK_F4 || wParam == VK_F5 || wParam == VK_F6 || wParam == VK_F7 || wParam == VK_F8 || wParam == VK_F9 || wParam == VK_F10 || wParam == VK_F11 || wParam == VK_F12) F1_12_pressed = TRUE;
        }

        case WM_CHAR:
        {
            if(wParam == VK_SHIFT || wParam == VK_CAPITAL) return 0;
            if(wParam == VK_UP || wParam == VK_RIGHT || wParam == VK_LEFT || wParam == VK_DOWN) return 0;
            if(wParam == VK_CONTROL) return 0;
            if(wParam == VK_DELETE) return 0;
            if(wParam == VK_INSERT) return 0;
            if(wParam == VK_PRIOR || wParam == VK_NEXT) return 0;
            if(wParam == VK_CANCEL || wParam == VK_TAB || wParam == VK_CLEAR) return 0;
            if(wParam == VK_SHIFT || wParam == VK_MENU || wParam == VK_PAUSE) return 0;
            if(wParam == VK_END || wParam == VK_HOME || wParam == VK_SELECT) return 0;
            if(wParam == VK_SNAPSHOT || wParam == VK_EXECUTE || wParam == VK_HELP) return 0;
            if(wParam == VK_LWIN || wParam == VK_RWIN || wParam == VK_APPS) return 0;
            if(wParam == VK_NUMLOCK || wParam == VK_SCROLL || wParam == VK_LSHIFT || wParam == VK_RSHIFT || wParam == VK_LCONTROL || wParam == VK_RCONTROL) return 0;
            if(chat_open != 2)
            {
                if(wParam == 191)
                {
                    chat_string[chat_size] = '/';
                    chat_size++;
                    return 0;
                }
                if(wParam == 190)
                {
                    chat_string[chat_size] = '.';
                    chat_size++;
                    return 0;
                }
                if(wParam == 254)
                {
                    chat_string[chat_size-1] = '�';
                    return 0;
                }
            }
            if(F1_12_pressed)
            {
                F1_12_pressed = FALSE;
                return 0;
            }
            static BOOL repeat_time = FALSE;
            if(chat_open == TRUE)
            {
                if(repeat_time == TRUE)
                {
                    if(wParam == VK_BACK && chat_open != 2)
                    {
                        if(chat_size > 0)
                        {
                            chat_size--;
                            chat_string[chat_size] = '\0';
                        }
                        repeat_time = FALSE;
                        return 0;
                    }
                    if(wParam == VK_RETURN)
                    {
                        if(type_chat == 2)
                        {
                            if(selCon == 16) conW = VK_RETURN;
                            if(selCon == 17) conA = VK_RETURN;
                            if(selCon == 18) conD = VK_RETURN;
                            if(selCon == 19) conS = VK_RETURN;
                            if(selCon == 20) conSpace = VK_RETURN;
                            if(selCon == 21) conR = VK_RETURN;
                            if(selCon == 22) conEnter = VK_RETURN;
                            if(selCon == 23) conT = VK_RETURN;
                            chat_open = FALSE;
                            repeat_time = FALSE;
                            return 0;
                        }
                        char buff[512];
                        if(chat_size > 0)
                        {
                            if(type_chat == 0)
                            {
                                chat_open = FALSE;
                                cursorShow = FALSE;
                                SetCursorPos((int)rctb.left + scrSize.x/2, (int)rctb.top + scrSize.y/2);
                                while (ShowCursor(FALSE) >= 0);
                            }
                            for(int iw = 0; iw < 512; iw++)
                            {
                                buff[iw] = 0;
                            }
                            if(type_chat == 0) sprintf(buff, "ops chat %s", chat_string);
                            else sprintf(buff, "ops password %s", chat_string);
                            if(SOCKET_ERROR == (send(s, &buff, sizeof(buff), 0)))
                            {
                                server_error = TRUE;
                                sprintf(ser_error_text, "����������� ������");
                            }
                            for(int iw = 0; iw < 512; iw++)
                            {
                                chat_string[iw] = 0;
                            }
                            chat_size = 0;
                            if(type_chat == 1)
                            {
                                enter_password = FALSE;
                                chat_open = FALSE;
                            }
                        }
                        repeat_time = FALSE;
                        return 0;
                    }
                    if(type_chat == 0)
                    {
                        if(chat_size < 70)
                        {
                            chat_string[chat_size] = wParam;
                            chat_size++;
                        }
                    }
                    else if(type_chat == 1)
                    {
                        if(chat_size < 39)
                        {
                            chat_string[chat_size] = wParam;
                            chat_size++;
                        }
                    }
                    else
                    {
                        if(selCon == 16) conW = wParam;
                        if(selCon == 17) conA = wParam;
                        if(selCon == 18) conD = wParam;
                        if(selCon == 19) conS = wParam;
                        if(selCon == 20) conSpace = wParam;
                        if(selCon == 21) conR = wParam;
                        if(selCon == 22) conEnter = wParam;
                        if(selCon == 23) conT = wParam;
                        chat_open = FALSE;
                        repeat_time = FALSE;
                    }
                    repeat_time = FALSE;
                }
                else repeat_time = TRUE;
            }
        }

        case WM_MOUSEMOVE:
        {
            if(afk)
            {
                for(int i = 0; i < 30; i++)
                {
                    if(CursorInButton(LOWORD(lParam), HIWORD(lParam), i) && Buttons[i].state != 2) Buttons[i].state = 1;
                    else if(Buttons[i].state != 2) Buttons[i].state = 0;
                }
            }
            if(open_inventory)
            {
                for(int i = 1; i < 256; i++)
                {
                    if(blocks[i].type == 0) selectCreateInventory = -1;
                    if(LOWORD(lParam) >= createInventoryVertex[i].vertex[0] && LOWORD(lParam) <= createInventoryVertex[i].vertex[2] && HIWORD(lParam) >= createInventoryVertex[i].vertex[1] && HIWORD(lParam) <= createInventoryVertex[i].vertex[3])
                    {
                        selectCreateInventory = i;
                        break;
                    }
                }
            }
        }
        break;

        case WM_MOUSEWHEEL:
            tmp = (int)wParam;
            if(tmp > 0)
            {
                select_inv++;
                if(select_inv > 9) select_inv = 0;
            }
            else
            {
                select_inv--;
                if(select_inv < 0) select_inv = 8;
            }
        break;

        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    return 0;
}
