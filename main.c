#include <windows.h>
#include <gl/gl.h>

#include "camera.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// Текстуры

int cobblestone_texture;
int grass_top_texture;
int grass_side_texture;
int dirt_texture;

float camera_z_in_jump;

int timer = 0;

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
GLuint blockInd[] = {0,1,5, 5,4,0, 1,2,6, 6,5,1, 2,3,7, 7,6,2, 3,0,4, 4,7,3};
float block_texture[] = {0,1, 1,1, 0,1, 1,1, 0,0, 1,0, 0,0, 1,0};
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

void GenerateNewChunk(chunkx, chunky)
{
    for(int z = 0; z <= 255; z++)
    {
        for(int y = 0; y <= 15; y++)
        {
            for(int x = 0; x <= 15; x++)
            {
                if(z <= 43) world[chunkx][chunky][x][y][z] = 1;
                else if(z == 44) world[chunkx][chunky][x][y][z] = 2;
                else world[chunkx][chunky][x][y][z] = 0;
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
    LoadTexture("textures/cobblestone.png", &cobblestone_texture);
    LoadTexture("textures/grass_top.png", &grass_top_texture);
    LoadTexture("textures/grass_side.png", &grass_side_texture);
    LoadTexture("textures/dirt.png", &dirt_texture);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_NORMALIZE);
    GenerateNewWorld();
}
void WndResize(int x, int y)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0,0,x,y);
    float k = x / (float)y;
    float sz = 0.1;
    glFrustum(-k*sz,k*sz, -sz, sz, sz*2, 100);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void Player_Move()
{
    Camera_MoveDirection(GetKeyState('W') < 0 ? 1: (GetKeyState('S') < 0 ? -1 : 0)
                         ,GetKeyState('D') < 0 ? 1 : (GetKeyState('A') < 0 ? -1: 0)
                         ,0.2);
    Camera_AutoMoveByMouse(400, 300, 0.2);
}

void Game_Show()
{
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
        printf("%f\n", ste);
        camera.z = (camera_z_in_jump - 2)+(ste);
        printf("%f\n", camera.z);
        //Sleep(1000);
        if(camera.x >= 0 && camera.x < 256 && camera.y >= 0 && camera.y < 256 && camera.z >= 0 && camera.z < 256)
        {
            if(jump_down == TRUE && world[chunkx][chunky][(int)xjump][(int)yjump][(int)cameraz] != 0)
            {
                is_jumping = FALSE;
                jump_down = FALSE;
                timer = 3;
                jump_tmp = 0;
            }
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
        }
        else
        {
            is_jumping = FALSE;
            jump_down = FALSE;
            timer = 3;
            jump_tmp = 0;
        }
    }
    if(chunkx > 15 || chunky > 15 || camera.x < 0 || camera.y < 0 || cameraz <= 0 || chunkx < 0 || chunky < 0)
    {
        if(!is_jumping)
        {
            camera.z -= 0.3;
            is_fly = TRUE;
        }
    }
    else if(world[chunkx][chunky][(int)xjump][(int)yjump][(int)cameraz] == 0 && !is_jumping)
    {
        camera.z -= 0.3;
        is_fly = TRUE;
    }
    if(chunkx <= 15 && chunky <= 15 && chunkx >= 0 && chunky >= 0)
    {
        if(GetKeyState(' ') < 0 && !is_jumping && world[chunkx][chunky][(int)xjump][(int)yjump][(int)cameraz] != 0 && camera.x > 0 && camera.x < 256 && camera.y > 0 && camera.y < 256 && cameraz > 0 && cameraz < 256 && !is_fly && ((int)camera.z - cameraz) == 1 && timer <= 0)
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
    glEnable(GL_FOG);
    glFogi(GL_FOG_MODE, fogMode[fogfilter]);
    glFogfv(GL_FOG_COLOR, fogColor);
    glFogf(GL_FOG_DENSITY, 0.35f);
    glHint(GL_FOG_HINT, GL_DONT_CARE);
    glFogf(GL_FOG_START, 7.0f);
    glFogf(GL_FOG_END, 8.0f);

    int dcnt = 0;

    glEnable(GL_TEXTURE_2D);
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
                            if(camera.x + 9 < x + dcx || camera.x - 9 > x + dcx || camera.y + 9 < y + dcy || camera.y - 9 > y + dcy) continue;
                            if(world[chunkx][chunky][x][y][z] != 0 && world[chunkx][chunky][x+1][y][z] != 0 && world[chunkx][chunky][x-1][y][z] != 0 && world[chunkx][chunky][x][y+1][z] != 0 && world[chunkx][chunky][x][y-1][z] != 0 && world[chunkx][chunky][x][y][z+1] != 0 && world[chunkx][chunky][x][y][z-1] != 0 && x != 0 && y != 0 && x != 15 && y != 15 && z != 0 && z != 255) continue;
                            if((x == 15 && chunkx != 15) && world[chunkx][chunky][x][y][z] != 0 && world[chunkx+1][chunky][0][y][z] != 0 && world[chunkx][chunky][x-1][y][z] != 0 && world[chunkx][chunky][x][y+1][z] != 0 && world[chunkx][chunky][x][y-1][z] != 0 && world[chunkx][chunky][x][y][z+1] != 0 && world[chunkx][chunky][x][y][z-1] != 0 && z != 0 && z != 255)
                            {
                                if(chunky == 0 && y == 0 || chunky == 15 || chunkx == 15) {}
                                else continue;
                            }
                            if((x == 0 && chunkx != 0) && world[chunkx][chunky][x][y][z] != 0 && world[chunkx-1][chunky][15][y][z] != 0 && world[chunkx][chunky][x+1][y][z] != 0 && world[chunkx][chunky][x][y+1][z] != 0 && world[chunkx][chunky][x][y-1][z] != 0 && world[chunkx][chunky][x][y][z+1] != 0 && world[chunkx][chunky][x][y][z-1] != 0 && z != 0 && z != 255)
                            {
                                if(chunky == 0 && y == 0 || chunky == 15 || chunkx == 15) {}
                                else continue;
                            }
                            if((y == 15 && chunky != 15) && world[chunkx][chunky][x][y][z] != 0 && world[chunkx][chunky+1][x][0][z] != 0 && world[chunkx][chunky][x][y-1][z] != 0 && world[chunkx][chunky][x+1][y][z] != 0 && world[chunkx][chunky][x-1][y][z] != 0 && world[chunkx][chunky][x][y][z+1] != 0 && world[chunkx][chunky][x][y][z-1] != 0 && z != 0 && z != 255)
                            {
                                if(chunkx == 0 && x == 0 || chunkx == 15 || chunky == 15) {}
                                else continue;
                            }
                            if((y == 0 && chunky != 0) && world[chunkx][chunky][x][y][z] != 0 && world[chunkx][chunky-1][x][15][z] != 0 && world[chunkx][chunky][x][y+1][z] != 0 && world[chunkx][chunky][x+1][y][z] != 0 && world[chunkx][chunky][x-1][y][z] != 0 && world[chunkx][chunky][x][y][z+1] != 0 && world[chunkx][chunky][x][y][z-1] != 0 && z != 0 && z != 255){
                                if(chunkx == 0 && x == 0 || chunkx == 15 || chunky == 15) {}
                                else continue;
                            }
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
                            if(world[chunkx][chunky][x][y][z] != 0) dcnt++;
                        }
                    }
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

    hwnd = CreateWindowEx(0,
                          "Opencraft",
                          "Opencraft rd-132211",
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

            glPopMatrix();

            SwapBuffers(hDC);

            //ShowDebugInfo();

            printf("%f %f %f\n", camera.Xrot, camera.Yrot, camera.Zrot);

            theta += 0,1080000108;

            timer--;
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
            PostQuitMessage(0);
        break;

        case WM_SIZE:
            WndResize(LOWORD(lParam), HIWORD(lParam));
        break;

        case WM_SETCURSOR:
            ShowCursor(FALSE);
        break;

        case WM_RBUTTONDOWN:
            if(camera.x < 0 || camera.x >= 256 || camera.y < 0 || camera.y >= 256 || camera.z < 0 || camera.z >= 256) break;
            float x = camera.x;
            float y = camera.y;
            float z = camera.z + 1.7;

            int X, Y, Z, oldX, oldY, oldZ;
            int dist = 0;
            while(dist < 4)
            {
                dist++;

                x += -sin(forblocksX/180*M_PI);
                z += tan(forblocksX/180*M_PI);
                y += -cos(forblocksY/180*M_PI);

                int chunkx = x / 16;
                int chunky = y / 16;
                int dcx = 16*chunkx;
                int dcy = 16*chunky;
                X = x - dcx;
                Y = y - dcy;
                Z = z;

                if(world[chunkx][chunky][X][Y][Z] != 0)
                {
                    world[chunkx][chunky][X][Y][Z] = 0;
                    break;
                }
                oldX=X; oldY = Y; oldZ = Z;
            }
        break;

        case WM_DESTROY:
            return 0;

        case WM_KEYDOWN:
        {
            switch (wParam)
            {
                case VK_ESCAPE:
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

