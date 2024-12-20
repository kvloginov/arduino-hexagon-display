#include "matrix.h"

// static const uint8_t xyToLed[MX_XY_H][MX_XY_W] = {
//     {NM, NM, NM, 0, NM, 1, NM, 2, NM, 3, NM, 4, NM, 5, NM, 6, NM, 7, NM, 8, NM, 9, NM, 10, NM, 11, NM, 12, NM, 13, NM, 14, NM, 15, NM, 16, NM, NM, NM},
//     {NM, NM, 34, NM, 33, NM, 32, NM, 31, NM, 30, NM, 29, NM, 28, NM, 27, NM, 26, NM, 25, NM, 24, NM, 23, NM, 22, NM, 21, NM, 20, NM, 19, NM, 18, NM, 17, NM, NM},
//     {NM, 35, NM, 36, NM, 37, NM, 38, NM, 39, NM, 40, NM, 41, NM, 42, NM, 43, NM, 44, NM, 45, NM, 46, NM, 47, NM, 48, NM, 49, NM, 50, NM, 51, NM, 52, NM, 53, NM},
//     {73, NM, 72, NM, 71, NM, 70, NM, 69, NM, 68, NM, 67, NM, 66, NM, 65, NM, 64, NM, 63, NM, 62, NM, 61, NM, 60, NM, 59, NM, 58, NM, 57, NM, 56, NM, 55, NM, 54},
//     {NM, 74, NM, 75, NM, 76, NM, 77, NM, 78, NM, 79, NM, 80, NM, 81, NM, 82, NM, 83, NM, 84, NM, 85, NM, 86, NM, 87, NM, 88, NM, 89, NM, 90, NM, 91, NM, 92, NM},
//     {NM, NM, 110, NM, 109, NM, 108, NM, 107, NM, 106, NM, 105, NM, 104, NM, 103, NM, 102, NM, 101, NM, 100, NM, 99, NM, 98, NM, 97, NM, 96, NM, 95, NM, 94, NM, 93, NM, NM},
//     {NM, NM, NM, 111, NM, 112, NM, 113, NM, 114, NM, 115, NM, 116, NM, 117, NM, 118, NM, 119, NM, 120, NM, 121, NM, 122, NM, 123, NM, 124, NM, 125, NM, 126, NM, 127, NM, NM, NM}};

static const uint8_t xyToLed[MX_REAL_H][MX_XY_VIRT_W] = {
    {NM, NM, NM, 0, NM, 1, NM, 2, NM, 3, NM, NM, NM},
    {NM, NM, 8, NM, 7, NM, 6, NM, 5, NM, 4, NM, NM},
    {NM, 9, NM, 10, NM, 11, NM, 12, NM, 13, NM, 14, NM},
    {21, NM, 20, NM, 19, NM, 18, NM, 17, NM, 16, NM, 15},
    {NM,22, NM, 23, NM, 24, NM, 25, NM, 26, NM, 27, NM},
    {NM, NM, 32, NM, 31, NM, 30, NM, 29, NM, 28, NM, NM},
    {NM, NM, NM, 33, NM, 34, NM, 35, NM, 36, NM, NM, NM},
};

int BallMatrix::ledXY(int x, int y)
{
    // we define only odd rows, so here we should map y's and skip every even rows
    if (x < 0 || y < 0 || x >= MX_XY_VIRT_W || y >= MX_XY_VIRT_H)
        return NM;
    return ((y & 1) ? NM : xyToLed[y >> 1][x]);
}

BallMatrix matrix;