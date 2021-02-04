#include "blocks.h"

void registerBlock(int id, int type, char* texture, char* texture1, char* texture2)
{
    blocks[id].type = type;
    if(type != 2)
    {
        LoadTexture(texture, &blocks[id].texture[0]);
    }
    else
    {
        LoadTexture(texture, &blocks[id].texture[0]);
        LoadTexture(texture1, &blocks[id].texture[1]);
        LoadTexture(texture2, &blocks[id].texture[2]);
    }
}
