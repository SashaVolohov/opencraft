#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <time.h>
#include <windows.h>
#include <locale.h>
#include <winuser.h>
#include <io.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <direct.h>
#include <limits.h>
#include <winsock2.h>
#include <pthread.h>
#include <conio.h>
#include <math.h>

#define OPENCRAFT_VERSION "0.0.16a_01"

BOOL bQuit = FALSE;

SOCKET s;
WSADATA ws;
SOCKADDR_IN name;

struct SServer{
    int port;
    int max_players;
    char server_name[1000];
    BOOL public;
    char motd[1000];
} server;

struct SSpawn {
    float x, y, z, Xrot, Zrot;
} spawn;

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

entity Entities[500];

int world[32][32][16][16][256];

int worldsizex, worldsizey = 0;

int cnt_players = 0;

typedef struct {
    SOCKET sock;
    SOCKADDR_IN ca;
    int new_len;
    float x, y, z, Xrot, Zrot;
    BOOL active;
    char nickname[32];
} SPlayer;

SPlayer Player[1001];

typedef struct{
    float x,y,z;
    int block_id;
} SWT;
SWT WorldTranslate[32][32][65536];

int worldTranslatereq[32][32];

float jump_go[10] = {1.10, 1.20, 1.30, 1.40, 1.50, 1.60, 1.70, 1.80, 1.90, 1.95};

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
    printText("Генерация пещер...\n", FALSE);
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
    printText("Генерация воды и лавы...\n", FALSE);
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
    printText("Дополнительная обработка...\n", FALSE);
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
    printText("Генерация деревьев...\n", FALSE);
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
    printText("Генерация руд...\n", FALSE);
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
    printText("Генерация ландшафта...\n", FALSE);
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
    spawn.x = worldsizex / 2 + 0.5;
    spawn.y = worldsizey / 2 + 0.5;
    for(int zx = 255; zx >= 0; zx--)
    {
        if(GetBlockID((int)spawn.x, (int)spawn.y, zx) != 0)
        {
            spawn.z = zx+1;
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
    SaveWorld();
}

char * setTime(struct tm *u, BOOL all)
{
    char s[40];
    char *tmp;
    for(int i = 0; i < 40; i++) s[i] = 0;
    int length = 0;
    if(all == FALSE) length = strftime(s, 40, "%H:%M:%S", u);
    else
    {
        char tmps[40];
        char nday[40];
        strftime(nday, 40, "%A", u);
        if(strcmp(nday, "Monday") == 0) sprintf(nday, "Пон");
        if(strcmp(nday, "Tuesday") == 0) sprintf(nday, "Вто");
        if(strcmp(nday, "Wednesday") == 0) sprintf(nday, "Сре");
        if(strcmp(nday, "Thursday") == 0) sprintf(nday, "Чет");
        if(strcmp(nday, "Friday") == 0) sprintf(nday, "Пят");
        if(strcmp(nday, "Saturday") == 0) sprintf(nday, "Суб");
        if(strcmp(nday, "Sunday") == 0) sprintf(nday, "Вос");

        char month[40];
        strftime(month, 40, "%m", u);
        if(strcmp(month, "01") == 0) sprintf(month, "Янв");
        if(strcmp(month, "02") == 0) sprintf(month, "Фев");
        if(strcmp(month, "03") == 0) sprintf(month, "Мар");
        if(strcmp(month, "04") == 0) sprintf(month, "Апр");
        if(strcmp(month, "05") == 0) sprintf(month, "Май");
        if(strcmp(month, "06") == 0) sprintf(month, "Июн");
        if(strcmp(month, "07") == 0) sprintf(month, "Июл");
        if(strcmp(month, "08") == 0) sprintf(month, "Авг");
        if(strcmp(month, "09") == 0) sprintf(month, "Сен");
        if(strcmp(month, "10") == 0) sprintf(month, "Окт");
        if(strcmp(month, "11") == 0) sprintf(month, "Ноя");
        if(strcmp(month, "12") == 0) sprintf(month, "Дек");

        char day[40];

        strftime(day, 40, "%d %H:%M:%S CEST %Y", u);

        sprintf(tmps, "%s %s %s", nday, month, day);

        length = strftime(s, 40, tmps, u);
    }
    tmp = (char*)malloc(sizeof(s));
    if(tmp != NULL)
    {
        strcpy(tmp, s);
        return(tmp);
    }
}

void printText(char* text, BOOL is_error)
{
    struct tm *u;
    char *f;
    const time_t timer = time(NULL);
    u = localtime(&timer);
    f = setTime(u, FALSE);
    char texte[200];
    if(!is_error) sprintf(texte, "     [%s] %s", f, text);
    else sprintf(texte, "  !  [%s] %s", f, text);

    const DWORD encodedVersion = GetVersion();
    const DWORD majorVersion = LOBYTE(LOWORD(encodedVersion));

    if((int)majorVersion >= 6) OemToChar(texte, texte);
    else CharToOem(texte, texte);

    printf("%s", texte);
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

void UpdateChunk(int chunkx, int chunky)
{
    int cx = worldsizex / 16 - 1;
    int cy = worldsizey / 16 - 1;
    if(chunkx < 0 || chunkx > cx || chunky < 0 || chunky > cy) return;
    worldTranslatereq[chunkx][chunky] = 0;
    int worldTranslatereqlocal = 0;
    for(int z = 0; z < 256; z++)
    {
        for(int y = 0; y < 16; y++)
        {
            for(int x = 0; x < 16; x++)
            {
                int dcx = 16*chunkx;
                int dcy = 16*chunky;
                if(GetBlockID(x+dcx, y+dcy, z) == 0) continue;
                if(GetBlockID(x+dcx, y+dcy, z) != 0 && GetBlockID(x+dcx+1, y+dcy, z) != 0 && GetBlockID(x+dcx-1, y+dcy, z) != 0 && GetBlockID(x+dcx, y+dcy+1, z) != 0 && GetBlockID(x+dcx, y+dcy-1, z) != 0 && GetBlockID(x+dcx, y+dcy, z+1) != 0 && GetBlockID(x+dcx, y+dcy, z-1) != 0 && GetBlockID(x+dcx, y+dcy, z) != 6 && GetBlockID(x+dcx+1, y+dcy, z) != 6 && GetBlockID(x+dcx-1, y+dcy, z) != 6 && GetBlockID(x+dcx, y+dcy+1, z) != 6 && GetBlockID(x+dcx, y+dcy-1, z) != 6 && GetBlockID(x+dcx, y+dcy, z+1) != 6 && GetBlockID(x+dcx, y+dcy, z-1) != 6)
                {
                    if(GetBlockID(x+dcx, y+dcy, z) != 7 && GetBlockID(x+dcx+1, y+dcy, z) != 7 && GetBlockID(x+dcx-1, y+dcy, z) != 7 && GetBlockID(x+dcx, y+dcy+1, z) != 7 && GetBlockID(x+dcx, y+dcy-1, z) != 7 && GetBlockID(x+dcx, y+dcy, z+1) != 7 && GetBlockID(x+dcx, y+dcy, z-1) != 7)
                    {
                        if(GetBlockID(x+dcx, y+dcy, z) != 8 && GetBlockID(x+dcx+1, y+dcy, z) != 8 && GetBlockID(x+dcx-1, y+dcy, z) != 8 && GetBlockID(x+dcx, y+dcy+1, z) != 8 && GetBlockID(x+dcx, y+dcy-1, z) != 8 && GetBlockID(x+dcx, y+dcy, z+1) != 8 && GetBlockID(x+dcx, y+dcy, z-1) != 8)
                        {
                            if(GetBlockID(x+dcx, y+dcy, z) != 16 && GetBlockID(x+dcx+1, y+dcy, z) != 16 && GetBlockID(x+dcx-1, y+dcy, z) != 16 && GetBlockID(x+dcx, y+dcy+1, z) != 16 && GetBlockID(x+dcx, y+dcy-1, z) != 16 && GetBlockID(x+dcx, y+dcy, z+1) != 16 && GetBlockID(x+dcx, y+dcy, z-1) != 16) continue;
                            else
                            {
                                WorldTranslate[chunkx][chunky][worldTranslatereqlocal].x = x+dcx;
                                WorldTranslate[chunkx][chunky][worldTranslatereqlocal].y = y+dcy;
                                WorldTranslate[chunkx][chunky][worldTranslatereqlocal].z = z;
                                WorldTranslate[chunkx][chunky][worldTranslatereqlocal].block_id = GetBlockID(x+dcx, y+dcy, z);
                                worldTranslatereq[chunkx][chunky]++;
                                worldTranslatereqlocal++;
                            }
                        }
                        else
                        {
                            WorldTranslate[chunkx][chunky][worldTranslatereqlocal].x = x+dcx;
                            WorldTranslate[chunkx][chunky][worldTranslatereqlocal].y = y+dcy;
                            WorldTranslate[chunkx][chunky][worldTranslatereqlocal].z = z;
                            WorldTranslate[chunkx][chunky][worldTranslatereqlocal].block_id = GetBlockID(x+dcx, y+dcy, z);
                            worldTranslatereq[chunkx][chunky]++;
                            worldTranslatereqlocal++;
                        }
                    }
                    else
                    {
                        WorldTranslate[chunkx][chunky][worldTranslatereqlocal].x = x+dcx;
                        WorldTranslate[chunkx][chunky][worldTranslatereqlocal].y = y+dcy;
                        WorldTranslate[chunkx][chunky][worldTranslatereqlocal].z = z;
                        WorldTranslate[chunkx][chunky][worldTranslatereqlocal].block_id = GetBlockID(x+dcx, y+dcy, z);
                        worldTranslatereq[chunkx][chunky]++;
                        worldTranslatereqlocal++;
                    }
                }
                else
                {
                    WorldTranslate[chunkx][chunky][worldTranslatereqlocal].x = x+dcx;
                    WorldTranslate[chunkx][chunky][worldTranslatereqlocal].y = y+dcy;
                    WorldTranslate[chunkx][chunky][worldTranslatereqlocal].z = z;
                    WorldTranslate[chunkx][chunky][worldTranslatereqlocal].block_id = GetBlockID(x+dcx, y+dcy, z);
                    worldTranslatereq[chunkx][chunky]++;
                    worldTranslatereqlocal++;
                }
            }
        }
    }
}

void SaveWorld()
{
    TCHAR buffer[MAX_PATH];
    GetCurrentDirectory(sizeof(buffer), buffer);
    char path[MAX_PATH];
    sprintf(path, "%s\\world", buffer);
    if(!DirectoryExists(path)) mkdir(path);
    FILE *file_spawn;
    FILE *file_world;
    FILE *file_version;
    FILE *file_entity;
    FILE *file_borders;

    sprintf(path, "%s\\world\\spawn.dat", buffer);

    file_spawn = fopen(path, "w");

    if(file_spawn == NULL) creat(path, S_IREAD|S_IWRITE);

    file_spawn = fopen(path, "w");

    fwrite(&spawn, 1, sizeof(spawn), file_spawn);
    fclose(file_spawn);


    sprintf(path, "%s\\world\\world.ocw", buffer);

    file_world = fopen(path, "w");

    if(file_world == NULL) creat(path, S_IREAD|S_IWRITE);

    file_world = fopen(path, "w");

    fwrite(world, sizeof(world), 1, file_world);
    fclose(file_world);


    sprintf(path, "%s\\world\\version.dat", buffer);

    file_version = fopen(path, "w");

    if(file_version == NULL) creat(path, S_IREAD|S_IWRITE);

    file_version = fopen(path, "w");

    char version[] = OPENCRAFT_VERSION;

    fwrite(version, 1, sizeof(version), file_version);
    fclose(file_version);

    sprintf(path, "%s\\world\\entity.dat", buffer);

    file_entity = fopen(path, "w");

    if(file_entity == NULL) creat(path, S_IREAD|S_IWRITE);

    file_entity = fopen(path, "wb");

    fwrite(Entities, sizeof(Entities[0]), 500, file_entity);
    fclose(file_entity);

    sprintf(path, "%s\\world\\border.dat", buffer);

    file_borders = fopen(path, "wb");

    if(file_borders == NULL) creat(path, S_IREAD|S_IWRITE);

    file_borders = fopen(path, "wb");

    fwrite(&worldsizex, 1, sizeof(worldsizex), file_borders);
    fclose(file_borders);
}

void WaitSends(int playerid)
{
    char messages[1000];
    while(Player[playerid].active)
    {
        int actual_len;
        char buff[512];
        for(int iw = 0; iw < 512; iw++)
        {
            buff[iw] = 0;
        }
        if(SOCKET_ERROR == (actual_len = recv(Player[playerid].sock, (char*) &buff, 512, 0)))
        {
            kick(playerid);
            return 0;
        }
        else
        {
            if(buff[4] == 'g' && buff[18] == 'e')
            {
                sprintf(buff, "ops server_name %s", server.server_name);
                if(SOCKET_ERROR == (send(Player[playerid].sock, &buff, sizeof(buff), 0))) kick(playerid);
            }
            else if(buff[4] == 'g' && buff[11] == 'd')
            {
                sprintf(buff, "ops motd %s", server.motd);
                if(SOCKET_ERROR == (send(Player[playerid].sock, &buff, sizeof(buff), 0))) kick(playerid);
            }
            else if(buff[4] == 'g' && buff[16] == 'e')
            {
                sprintf(buff, "ops worldsize %d", worldsizex);
                if(SOCKET_ERROR == (send(Player[playerid].sock, &buff, sizeof(buff), 0))) kick(playerid);
            }
            else if(buff[4] == 'g' && buff[12] == 'd')
            {
                int cx = worldsizex / 16 - 1;
                int cy = worldsizey / 16 - 1;

                for(int chunky = 0; chunky <= cy; chunky++)
                {
                    for(int chunkx = 0; chunkx <= cx; chunkx++)
                    {
                        for(int qwe = 0; qwe < worldTranslatereq[chunkx][chunky]; qwe++)
                        {
                            int x = WorldTranslate[chunkx][chunky][qwe].x;
                            int y = WorldTranslate[chunkx][chunky][qwe].y;
                            int z = WorldTranslate[chunkx][chunky][qwe].z;
                            int block_id = WorldTranslate[chunkx][chunky][qwe].block_id;
                            for(int iw = 0; iw < 512; iw++)
                            {
                                buff[iw] = 0;
                            }
                            sprintf(buff, "ops set_block %d %d %d %d", x, y, z, block_id);
                            if(SOCKET_ERROR == (send(Player[playerid].sock, &buff, sizeof(buff), 0))) kick(playerid);
                        }
                    }
                }

                for(int iw = 0; iw < 512; iw++)
                {
                    buff[iw] = 0;
                }
                sprintf(buff, "ops world_transmitted");
                if(SOCKET_ERROR == (send(Player[playerid].sock, &buff, sizeof(buff), 0))) kick(playerid);
            }
            else if(buff[4] == 'g' && buff[12] == 'n')
            {
                for(int iw = 0; iw < 512; iw++)
                {
                    buff[iw] = 0;
                }
                sprintf(buff, "ops spawn %f %f %f %f %f", spawn.x, spawn.y, spawn.z, spawn.Xrot, spawn.Zrot);
                if(SOCKET_ERROR == (send(Player[playerid].sock, &buff, sizeof(buff), 0))) kick(playerid);
            }
            else if(buff[4] == 'g' && buff[11] == 's')
            {
                for(int e = 0; e < 500; e++)
                {
                    if(Entities[e].entity_id == 0) continue;
                    for(int iw = 0; iw < 512; iw++)
                    {
                        buff[iw] = 0;
                    }
                    sprintf(buff, "ops set_mob_coord %f %f %f %f %f %d %d", Entities[e].x, Entities[e].y, Entities[e].z, Entities[e].Xrot, Entities[e].Zrot, Entities[e].entity_id, e);
                    if(SOCKET_ERROR == (send(Player[playerid].sock, &buff, sizeof(buff), 0))) kick(playerid);
                }
                for(int iw = 0; iw < 512; iw++)
                {
                    buff[iw] = 0;
                }
                sprintf(buff, "ops mobs_translated");
                if(SOCKET_ERROR == (send(Player[playerid].sock, &buff, sizeof(buff), 0))) kick(playerid);
            }
            else if(buff[4] == 'v' && buff[10] == 'n')
            {
                int cnt = 0;
                char client_version[512];
                for(int ie = 12; ie < 512; ie++)
                {
                    if(buff[ie] == '\0')
                    {
                        client_version[cnt] = '\0';
                        break;
                    }
                    client_version[cnt] = buff[ie];
                    cnt++;
                }
                if(strcmp(OPENCRAFT_VERSION, client_version) != 0)
                {
                    for(int iw = 0; iw < 512; iw++)
                    {
                        buff[iw] = 0;
                    }
                    sprintf(buff, "ops bad_version");
                    send(Player[playerid].sock, &buff, sizeof(buff), 0);
                    sprintf(buff, "Игрок с ID %d покинул игру из-за несовместимой версии Opencraft\n\0", playerid);
                    printText(buff, FALSE);
                    Player[playerid].active = FALSE;
                    closesocket(Player[playerid].sock);
                    cnt_players--;
                    kick(playerid);
                    return 0;
                }
            }
            else if(buff[4] == 's' && buff[11] == 'd')
            {
                float x_f, y_f, z_f, Xrot_f, Zrot_f;
                char x_t[10];
                char y_t[10];
                char z_t[10];
                char Xrot_t[10];
                char Zrot_t[10];
                int now = 0;
                int cnt = 0;
                for(int iw = 0; iw < 10; iw++)
                {
                    x_t[iw] = 0;
                    y_t[iw] = 0;
                    z_t[iw] = 0;
                    Xrot_t[iw] = 0;
                    Zrot_t[iw] = 0;
                }
                now = 0;
                cnt = 0;
                for(int e = 13; e < 512; e++)
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
                Player[playerid].x = x_f;
                Player[playerid].y = y_f;
                Player[playerid].z = z_f;
                Player[playerid].Xrot = Xrot_f;
                Player[playerid].Zrot = Zrot_f;
                for(int qwe = 0; qwe < server.max_players; qwe++)
                {
                    if(Player[qwe].active == FALSE) continue;
                    if(qwe == playerid) continue;
                    char buff[512];
                    for(int iw = 0; iw < 512; iw++)
                    {
                        buff[iw] = 0;
                    }
                    sprintf(buff, "ops setcoord %f %f %f %f %f %d %s", x_f, y_f, z_f, Xrot_f, Zrot_f, playerid, Player[playerid].nickname);
                    if(SOCKET_ERROR == (send(Player[qwe].sock, &buff, sizeof(buff), 0))) kick(playerid);
                }
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
                for(int y = 0; y < server.max_players; y++)
                {
                    if(Player[y].active == FALSE) continue;
                    for(int iw = 0; iw < 512; iw++)
                    {
                        buff[iw] = 0;
                    }
                    sprintf(buff, "ops set_block %d %d %d %d", x_i, y_i, z_i, block_i);
                    if(SOCKET_ERROR == (send(Player[y].sock, &buff, sizeof(buff), 0))) kick(y);
                }
                UpdateChunk(chunkx, chunky);
                SendChunk(chunkx, chunky);
                if(fx == 0)
                {
                    UpdateChunk(chunkx-1, chunky);
                    SendChunk(chunkx-1, chunky);
                }
                if(fx == 15)
                {
                    UpdateChunk(chunkx+1, chunky);
                    SendChunk(chunkx+1, chunky);
                }
                if(fy == 0)
                {
                    UpdateChunk(chunkx, chunky-1);
                    SendChunk(chunkx, chunky-1);
                }
                if(fy == 15)
                {
                    UpdateChunk(chunkx, chunky+1);
                    SendChunk(chunkx, chunky+1);
                }
            }
            else if(buff[4] == 'n' && buff[11] == 'e')
            {
                int cnt = 0;
                for(int ie = 13; ie < 512; ie++)
                {
                    if(buff[ie] == '\0')
                    {
                        Player[playerid].nickname[cnt] = '\0';
                        break;
                    }
                    Player[playerid].nickname[cnt] = buff[ie];
                    cnt++;
                }
                char buff[512];
                sprintf(buff, "%s присоединился к игре\n\0", Player[playerid].nickname);
                printText(buff, FALSE);
                for(int q = 0; q < server.max_players; q++)
                {
                    if(Player[q].active == FALSE) continue;
                    for(int iw = 0; iw < 512; iw++)
                    {
                        buff[iw] = 0;
                    }
                    sprintf(buff, "ops chat_message ~y%s присоединился к игре", Player[playerid].nickname);
                    if(SOCKET_ERROR == (send(Player[q].sock, &buff, sizeof(buff), 0))) kick(q);
                }
                if(isPlayerBanned(playerid))
                {
                    sprintf(buff, "ops banned");
                    send(Player[playerid].sock, &buff, sizeof(buff), 0);
                    sprintf(buff, "%s покинул игру\n\0", Player[playerid].nickname);
                    printText(buff, FALSE);
                    Player[playerid].active = FALSE;
                    closesocket(Player[playerid].sock);
                    cnt_players--;
                    kick(playerid);
                    return 0;
                }
            }
            else if(buff[4] == 'c' && buff[7] == 't')
            {
                char chat_msg[512];
                int cnt = 0;
                for(int ie = 9; ie < 512; ie++)
                {
                    if(buff[ie] == '\0')
                    {
                        chat_msg[cnt] = '\0';
                        break;
                    }
                    chat_msg[cnt] = buff[ie];
                    cnt++;
                }
                if(chat_msg[0] == '/')
                {
                    int cnt = 0;
                    char cmd[512];
                    char arg[512];
                    for(int ytr = 0; ytr < 512; ytr++)
                    {
                        cmd[ytr] = 0;
                        arg[ytr] = 0;
                    }
                    for(int ie = 0; ie < 512; ie++)
                    {
                        if(chat_msg[ie] == '\0') break;
                        if(chat_msg[ie] == ' ')
                        {
                            cnt = 0;
                            for(int iw = ie+1; iw < 512; iw++)
                            {
                                if(buff[iw] == '\0') break;
                                arg[cnt] = chat_msg[iw];
                                cnt++;
                            }
                            break;
                        }
                        cmd[cnt] = chat_msg[ie];
                        cnt++;
                    }
                    if(strcmp(cmd, "/ban") == 0)
                    {
                        if(!isPlayerOP(playerid))
                        {
                            sprintf(buff, "ops chat_message Извините, но Вы не являетесь администратором этого сервера.");
                            if(SOCKET_ERROR == (send(Player[playerid].sock, &buff, sizeof(buff), 0))) kick(playerid);
                        }
                        else
                        {
                            for(int qwe = 0; qwe < server.max_players; qwe++)
                            {
                                if(strcmp(Player[qwe].nickname, arg) == 0)
                                {
                                    if(qwe == playerid)
                                    {
                                        sprintf(buff, "ops chat_message Вы не можете забанить самого себя.");
                                        if(SOCKET_ERROR == (send(Player[playerid].sock, &buff, sizeof(buff), 0))) kick(playerid);
                                        break;
                                    }
                                    else
                                    {
                                        ban(qwe);
                                        sprintf(buff, "ops chat_message Вы забанили игрока %s", Player[qwe].nickname);
                                        if(SOCKET_ERROR == (send(Player[playerid].sock, &buff, sizeof(buff), 0))) kick(playerid);
                                    }
                                }
                                else if(qwe == server.max_players-1)
                                {
                                    sprintf(buff, "ops chat_message Игрока с таким ником нет в сети", Player[qwe].nickname);
                                    if(SOCKET_ERROR == (send(Player[playerid].sock, &buff, sizeof(buff), 0))) kick(playerid);
                                }
                            }
                        }
                    }
                    if(strcmp(cmd, "/kick") == 0)
                    {
                        if(!isPlayerOP(playerid))
                        {
                            sprintf(buff, "ops chat_message Извините, но Вы не являетесь администратором этого сервера.");
                            if(SOCKET_ERROR == (send(Player[playerid].sock, &buff, sizeof(buff), 0))) kick(playerid);
                        }
                        else
                        {
                            for(int qwe = 0; qwe < server.max_players; qwe++)
                            {
                                if(strcmp(Player[qwe].nickname, arg) == 0)
                                {
                                    if(qwe == playerid)
                                    {
                                        sprintf(buff, "ops chat_message Вы не можете кикнуть самого себя.");
                                        if(SOCKET_ERROR == (send(Player[playerid].sock, &buff, sizeof(buff), 0))) kick(playerid);
                                        break;
                                    }
                                    else
                                    {
                                        sprintf(buff, "ops chat_message Вы кикнули игрока %s", Player[qwe].nickname);
                                        if(SOCKET_ERROR == (send(Player[playerid].sock, &buff, sizeof(buff), 0))) kick(playerid);
                                        sprintf(buff, "ops kicked");
                                        send(Player[qwe].sock, &buff, sizeof(buff), 0);
                                        sprintf(buff, "%s покинул игру\n\0", Player[qwe].nickname);
                                        printText(buff, FALSE);
                                        Player[qwe].active = FALSE;
                                        closesocket(Player[qwe].sock);
                                        kick(playerid);
                                        cnt_players--;
                                    }
                                }
                                else if(qwe == server.max_players-1)
                                {
                                    sprintf(buff, "ops chat_message Игрока с таким ником нет в сети", Player[qwe].nickname);
                                    if(SOCKET_ERROR == (send(Player[playerid].sock, &buff, sizeof(buff), 0))) kick(playerid);
                                }
                            }
                        }
                    }
                    if(strcmp(cmd, "/say") == 0)
                    {
                        if(!isPlayerOP(playerid))
                        {
                            sprintf(buff, "ops chat_message Извините, но Вы не являетесь администратором этого сервера.");
                            if(SOCKET_ERROR == (send(Player[playerid].sock, &buff, sizeof(buff), 0))) kick(playerid);
                        }
                        else
                        {
                            for(int q = 0; q < server.max_players; q++)
                            {
                                if(Player[q].active == FALSE) continue;
                                for(int iw = 0; iw < 512; iw++)
                                {
                                    buff[iw] = 0;
                                }
                                sprintf(buff, "ops chat_message [@] %s", arg);
                                if(SOCKET_ERROR == (send(Player[q].sock, &buff, sizeof(buff), 0))) kick(q);
                            }
                        }
                    }
                }
                else
                {
                    for(int q = 0; q < server.max_players; q++)
                    {
                        if(Player[q].active == FALSE) continue;
                        for(int iw = 0; iw < 512; iw++)
                        {
                            buff[iw] = 0;
                        }
                        sprintf(buff, "ops chat_message %s:%s", Player[playerid].nickname, chat_msg);
                        if(SOCKET_ERROR == (send(Player[q].sock, &buff, sizeof(buff), 0))) kick(q);
                    }
                }
            }
        }
    }
    return 0;
}

void WaitMessages()
{
    char messages[1000];
    while(bQuit == FALSE)
    {
        int playerid = 0;
        for(int i = 0; i < server.max_players; i++)
        {
            if(Player[i].active == FALSE)
            {
                playerid = i;
                break;
            }
        }
        Player[playerid].new_len = sizeof(Player[playerid].ca);
        ZeroMemory(&Player[playerid].ca, sizeof(Player[playerid].ca));
        if(FAILED(Player[playerid].sock = accept(s, (SOCKADDR*) &Player[playerid].ca, &Player[playerid].new_len)))
        {
            int error = WSAGetLastError();
            sprintf(messages, "Ошибка newsock. Код ошибки: %d\n", error);
            printText(messages, TRUE);
        }
        else
        {
            if(cnt_players == server.max_players)
            {
                char buff[512];
                sprintf(buff, "ops max_players");
                if(SOCKET_ERROR == (send(Player[playerid].sock, &buff, sizeof(buff), 0)))
                {
                    int error = WSAGetLastError();
                    sprintf(messages, "Ошибка newsock. Код ошибки: %d\n", error);
                    printText(messages, TRUE);
                }
                closesocket(Player[playerid].sock);
            }
            else
            {
                cnt_players++;
                Player[playerid].active = TRUE;
                pthread_t thread;
                int status;
                int status_addr;

                status = pthread_create(&thread, NULL, WaitSends, playerid);

                if(status != 0)
                {
                    sprintf(messages, "Ошибка инициализации потока. Код ошибки: %d\n", status);
                    printText(messages, TRUE);
                }
                char buff[512];
                for(int iw = 0; iw < 512; iw++)
                {
                    buff[iw] = 0;
                }
                sprintf(buff, "ops get_version");
                if(SOCKET_ERROR == (send(Player[playerid].sock, &buff, sizeof(buff), 0))) kick(playerid);
                for(int iw = 0; iw < 512; iw++)
                {
                    buff[iw] = 0;
                }
                sprintf(buff, "ops get_nickname");
                if(SOCKET_ERROR == (send(Player[playerid].sock, &buff, sizeof(buff), 0))) kick(playerid);
                for(int qwe = 0; qwe < server.max_players; qwe++)
                {
                    if(Player[qwe].active == FALSE) continue;
                    if(qwe == playerid) continue;
                    char buff[512];
                    for(int iw = 0; iw < 512; iw++)
                    {
                        buff[iw] = 0;
                    }
                    sprintf(buff, "ops setcoord %f %f %f %f %f %d %s", Player[qwe].x, Player[qwe].y, Player[qwe].z, Player[qwe].Xrot, Player[qwe].Zrot, qwe, Player[qwe].nickname);
                    if(SOCKET_ERROR == (send(Player[playerid].sock, &buff, sizeof(buff), 0))) kick(playerid);
                }
            }
        }
    }
    return 0;
}

void EntityAI(int j)
{
    if(Entities[j].entity_id == 0) return 0;
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
    char buff[512];
    for(int we = 0; we < server.max_players; we++)
    {
        if(Player[we].active == FALSE) continue;
        for(int iw = 0; iw < 512; iw++)
        {
            buff[iw] = 0;
        }
        sprintf(buff, "ops set_mob_coord %f %f %f %f %f %d %d", Entities[j].x, Entities[j].y, Entities[j].z, Entities[j].Xrot, Entities[j].Zrot, Entities[j].entity_id, j);
        if(SOCKET_ERROR == (send(Player[we].sock, &buff, sizeof(buff), 0))) kick(we);
    }
}

void SendChunk(int chunkx, int chunky)
{
    char buff[512];
    for(int playerid = 0; playerid < server.max_players; playerid++)
    {
        if(Player[playerid].active == FALSE) continue;
        for(int qwe = 0; qwe < worldTranslatereq[chunkx][chunky]; qwe++)
        {
            int x = WorldTranslate[chunkx][chunky][qwe].x;
            int y = WorldTranslate[chunkx][chunky][qwe].y;
            int z = WorldTranslate[chunkx][chunky][qwe].z;
            int block_id = WorldTranslate[chunkx][chunky][qwe].block_id;
            for(int iw = 0; iw < 512; iw++)
            {
                buff[iw] = 0;
            }
            sprintf(buff, "ops set_block %d %d %d %d", x, y, z, block_id);
            if(SOCKET_ERROR == (send(Player[playerid].sock, &buff, sizeof(buff), 0))) kick(playerid);
        }
    }
}

BOOL isPlayerOP(int playerid)
{
    char playername[32];
    strcpy(playername, Player[playerid].nickname);
    FILE *admins;

    char adm[1000];

    admins = fopen("admins.txt", "rt");
    while(!feof(admins))
    {
        for(int wrt = 0; wrt < 1000; wrt++)
        {
            adm[wrt] = 0;
        }
        fgets(adm, 1000, admins);
        if(strcmp(Player[playerid].nickname, adm) == 0) return TRUE;
    }
    fclose(admins);
    return FALSE;
}

void ban(int playerid)
{
    FILE *bans;
    FILE *bans_2;
    char buff[512];

    char banned[1000];

    bans = fopen("banned.txt", "rt");
    bans_2 = fopen("banned.txt.tmp", "wt");

    while(!feof(bans))
    {
        for(int wrt = 0; wrt < 1000; wrt++)
        {
            banned[wrt] = 0;
        }
        fgets(banned, 1000, bans);
        fprintf(bans_2, banned);
    }

    fclose(bans);

    fprintf(bans_2, "%s\n", Player[playerid].nickname);
    fclose(bans_2);

    remove("banned.txt");
    rename("banned.txt.tmp", "banned.txt");

    sprintf(buff, "ops banned");
    send(Player[playerid].sock, &buff, sizeof(buff), 0);
    sprintf(buff, "%s покинул игру\n\0", Player[playerid].nickname);
    printText(buff, FALSE);
    Player[playerid].active = FALSE;
    closesocket(Player[playerid].sock);
    kick(playerid);
    cnt_players--;
}

BOOL isPlayerBanned(int playerid)
{
    char playername[32];
    strcpy(playername, Player[playerid].nickname);
    FILE *bans;

    char banned[1000];

    bans = fopen("banned.txt", "rt");
    while(!feof(bans))
    {
        for(int wrt = 0; wrt < 1000; wrt++)
        {
            banned[wrt] = 0;
        }
        fgets(banned, 1000, bans);
        if(strcmp(Player[playerid].nickname, banned) == 0) return TRUE;
    }
    fclose(bans);
    return FALSE;
}

void kick(int playerid)
{
    if(Player[playerid].active == FALSE) return;
    char buff[512];
    sprintf(buff, "%s покинул игру\n\0", Player[playerid].nickname);
    printText(buff, FALSE);
    Player[playerid].active = FALSE;
    closesocket(Player[playerid].sock);
    cnt_players--;
    for(int q = 0; q < server.max_players; q++)
    {
        if(Player[q].active == FALSE) continue;
        for(int iw = 0; iw < 512; iw++)
        {
            buff[iw] = 0;
        }
        sprintf(buff, "ops chat_message ~w%s покинул игру", Player[playerid].nickname);
        if(SOCKET_ERROR == (send(Player[q].sock, &buff, sizeof(buff), 0))) kick(q);
    }
}

int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    system("title Opencraft Classic Server 1.1");
    MSG msg;

    if(FAILED(WSAStartup(MAKEWORD(1, 1), &ws)))
    {
        int error = WSAGetLastError();
        char errors[100];
        sprintf(errors, "Ошибка инициализации Winsock. Код ошибки: %d\n", error);
        printText(errors, TRUE);
        Sleep(10000);
        return 0;
    }

    if(INVALID_SOCKET == (s = socket(AF_INET, SOCK_STREAM, 0)))
    {
        int error = WSAGetLastError();
        char errors[100];
        sprintf(errors, "Ошибка инициализации сокета. Код ошибки: %d\n", error);
        printText(errors, TRUE);
        Sleep(10000);
        return 0;
    }

    printText("Применение настроек...\n", FALSE);

    char argument[1000];
    char value[1000];

    FILE *server_properties;

    char sp[1000];

    server_properties = fopen("server.properties", "rt");

    if(server_properties != NULL)
    {
        while(!feof(server_properties))
        {
            fgets(sp, 1000, server_properties);
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
                    if(strcmp(argument, "port") == 0)
                    {
                        server.port = atoi(value);
                    }
                    else if(strcmp(argument, "max-players") == 0)
                    {
                        server.max_players = atoi(value);
                        if(server.max_players > 1000)
                        {
                            printText("Некорректное значение аргумента max-players в server.properties\n", TRUE);
                            printText("Значение max-players изменено на 16\n", TRUE);
                            printText("Измените значение max-players в server.properties\n", TRUE);
                            server.max_players = 16;
                        }
                    }
                    else if(strcmp(argument, "server-name") == 0)
                    {
                        strcpy(server.server_name, value);
                    }
                    else if(strcmp(argument, "public") == 0)
                    {
                        if(strcmp("false", value) == 0) server.public = FALSE;
                        else server.public = TRUE;
                    }
                    else if(strcmp(argument, "motd") == 0)
                    {
                        strcpy(server.motd, value);
                    }
                    exit = TRUE;
                }
                if(exit == TRUE) break;
            }
        }
        fclose(server_properties);
    }
    else
    {
        printText("Не обнаружен файл server.properties! Генерация...\n", TRUE);
        creat("server.properties", S_IREAD|S_IWRITE);

        server_properties = fopen("server.properties", "wt");

        fprintf(server_properties, "#Свойства сервера Opencraft\n");

        struct tm *u;
        char *f;
        const time_t timer = time(NULL);
        u = localtime(&timer);
        f = setTime(u, TRUE);

        fprintf(server_properties, "#%s\n", f);
        fprintf(server_properties, "port=25565\n", f);
        fprintf(server_properties, "max-players=16\n", f);
        fprintf(server_properties, "server-name=Сервер Opencraft\n", f);
        fprintf(server_properties, "public=true\n", f);
        fprintf(server_properties, "motd=Добро пожаловать на мой Opencraft сервер!", f);

        server.port = 25565;
        server.max_players = 16;
        sprintf(server.server_name, "Сервер Opencraft");
        server.public = TRUE;
        sprintf(server.motd, "Добро пожаловать на мой Opencraft сервер!");

        fclose(server_properties);
    }

    for(int w = 0; w < server.max_players; w++)
    {
        Player[w].active == FALSE;
    }

    TCHAR buffer[MAX_PATH];
    GetCurrentDirectory(sizeof(buffer), buffer);
    char path[MAX_PATH];
    sprintf(path, "%s\\world", buffer);
    if(DirectoryExists(path))
    {
        printText("Загрузка мира...\n", FALSE);
        FILE *file_spawn;
        FILE *file_world;
        FILE *file_version;
        FILE *file_entity;
        FILE *file_borders;

        sprintf(path, "%s\\world\\border.dat", buffer);

        file_borders = fopen(path, "r");

        if(file_borders == NULL) creat(path, S_IREAD|S_IWRITE);

        file_borders = fopen(path, "r");

        fread(&worldsizex, 1, sizeof(worldsizex), file_borders);
        fclose(file_borders);

        worldsizey = worldsizex;

        sprintf(path, "%s\\world\\spawn.dat", buffer);

        file_spawn = fopen(path, "r");

        if(file_spawn != NULL)
        {
            fread(&spawn, 1, sizeof(spawn), file_spawn);
            fclose(file_spawn);
        }

        sprintf(path, "%s\\world\\world.ocw", buffer);

        file_world = fopen(path, "r");

        if(file_world != NULL)
        {
            fread(world, 1, sizeof(world), file_world);
            fclose(file_world);
        }

        char version[50];

        sprintf(path, "%s\\world\\version.dat", buffer);

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

        sprintf(path, "%s\\world\\entity.dat", buffer);

        file_entity = fopen(path, "rb");

        if(file_entity != NULL)
        {
            fread(Entities, sizeof(Entities[0]), 500, file_entity);
            fclose(file_entity);
        }
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
        printText("Мир не найден. Генерация нового мира...\n", TRUE);
        worldsizex = 512;
        worldsizey = 512;
        GenerateNewWorld();
    }

    SOCKADDR_IN name;
    ZeroMemory(&name, sizeof(name));
    name.sin_family = AF_INET;
    name.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
    name.sin_port = htons(server.port);

    if(SOCKET_ERROR == bind(s, (SOCKADDR_IN*) &name, sizeof(name)))
    {
        return E_FAIL;
    }

    if(FAILED(listen(s, SOMAXCONN)))
    {
        return E_FAIL;
    }

    FILE *admins;

    admins = fopen("admins.txt", "rt");

    if(admins == NULL) creat("admins.txt", S_IREAD|S_IWRITE);
    else fclose(admins);

    FILE *bans;

    bans = fopen("banned.txt", "rt");

    if(bans == NULL) creat("banned.txt", S_IREAD|S_IWRITE);
    else fclose(bans);

    char messages[100];
    sprintf(messages, "Начат приём сообщений на порту %d\n", server.port);

    printText(messages, FALSE);

    pthread_t thread;
    int status;
    int status_addr;

    status = pthread_create(&thread, NULL, WaitMessages, NULL);

    if(status != 0)
    {
        sprintf(messages, "Ошибка инициализации потока. Код ошибки: %d\n", status);
        printText(messages, TRUE);
    }

    while (!bQuit)
    {
        /* check for messages */
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            /* handle or dispatch messages */
            if (msg.message == WM_QUIT)
            {
                printText("Сохранение мира...", FALSE);
                SaveWorld();
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


            static float lastTime = 0.0f;
            float currentTime = GetTickCount() * 0.001f;
            if(currentTime - lastTime > 600.0f)
            {
                lastTime = currentTime;
                printText("Сохранение мира...\n", FALSE);
                SaveWorld();
                printText("Сохранение мира успешно завершено\n", FALSE);
            }
            static float lastTime2 = 0.0f;
            float currentTime2 = GetTickCount() * 0.001f;
            if(currentTime2 - lastTime2 > 10.0f)
            {
                lastTime2 = currentTime2;
            }

            static float lastTime3 = 0.0f;
            float currentTime3 = GetTickCount() * 0.001f;
            if(currentTime3 - lastTime3 > 0.01f)
            {
                float qwe = 0.0;
                if(currentTime - lastTime <= 1) qwe = (currentTime - lastTime) / 0.01f;
                else qwe = 1;
                lastTime = currentTime;
                for(int cnt = 1; cnt <= (int)qwe; cnt++)
                {
                    for(int qwe = 0; qwe < 500; qwe++) EntityAI(qwe);
                }
            }

            int cy = worldsizey / 16 - 1;
            int cx = worldsizex / 16 - 1;


            int chunky = rand() % cy;
            int chunkx = rand() % cx;
            int z = rand() % 255;
            int y = rand() % 15;
            int x = rand() % 15;


            int dcx = 16*chunkx;
            int dcy = 16*chunky;
            int randome = rand();
            randome %= 500;
            if(1 == 1)
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
                                UpdateChunk(chunkx, chunky);
                                SendChunk(chunkx, chunky);
                            }
                        }
                    }
                }
                if(world[chunkx][chunky][x][y][z] == 2)
                {
                    if(world[chunkx][chunky][x][y][z+1] != 0 && world[chunkx][chunky][x][y][z+1] != 6)
                    {
                        world[chunkx][chunky][x][y][z] = 3;
                        UpdateChunk(chunkx, chunky);
                        SendChunk(chunkx, chunky);
                    }
                }
            }
        }
    }
    return 0;
}
