#ifndef BLOCKS_H_INCLUDED
#define BLOCKS_H_INCLUDED

typedef struct {
    int type;
    int texture[8];
} SBlock;

SBlock blocks[256];

#endif // BLOCKS_H_INCLUDED
