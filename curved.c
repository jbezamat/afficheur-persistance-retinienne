#include "curved.h"
#include "global.h"
#include "SPI.h"

int angle[33][33] = 
{{22,23,23,23,24,24,25,25,26,26,27,27,28,28,29,29,30,31,31,32,32,33,33,34,34,35,35,36,36,37,37,37,38},
{22,22,23,23,24,24,24,25,25,26,26,27,28,28,29,29,30,31,31,32,32,33,34,34,35,35,36,36,36,37,37,38,38},
{22,22,22,23,23,24,24,25,25,26,26,27,27,28,29,29,30,31,31,32,33,33,34,34,35,35,36,36,37,37,38,38,38},
{22,22,22,22,23,23,24,24,25,25,26,26,27,28,29,29,30,31,31,32,33,34,34,35,35,36,36,37,37,38,38,38,38},
{21,21,22,22,22,23,23,24,24,25,26,26,27,28,28,29,30,31,32,32,33,34,34,35,36,36,37,37,38,38,38,39,39},
{21,21,21,22,22,22,23,23,24,25,25,26,27,27,28,29,30,31,32,33,33,34,35,35,36,37,37,38,38,38,39,39,39},
{20,21,21,21,22,22,22,23,24,24,25,26,26,27,28,29,30,31,32,33,34,34,35,36,36,37,38,38,38,39,39,39,40},
{20,20,20,21,21,22,22,22,23,24,24,25,26,27,28,29,30,31,32,33,34,35,36,36,37,38,38,38,39,39,40,40,40},
{19,20,20,20,21,21,21,22,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,38,39,39,39,40,40,40,41},
{19,19,19,20,20,20,21,21,22,22,23,24,25,26,27,29,30,31,33,34,35,36,37,38,38,39,39,40,40,40,41,41,41},
{18,19,19,19,19,20,20,21,21,22,22,23,24,26,27,28,30,32,33,34,36,37,38,38,39,39,40,40,41,41,41,41,42},
{18,18,18,19,19,19,19,20,20,21,22,22,24,25,26,28,30,32,34,35,36,38,38,39,40,40,41,41,41,41,42,42,42},
{17,17,18,18,18,18,19,19,19,20,21,21,22,24,26,28,30,32,34,36,38,39,39,40,41,41,41,42,42,42,42,43,43},
{17,17,17,17,17,18,18,18,18,19,19,20,21,22,24,27,30,33,36,38,39,40,41,41,42,42,42,42,43,43,43,43,43},
{16,16,16,16,17,17,17,17,17,18,18,19,19,21,22,26,30,34,38,39,41,41,42,42,43,43,43,43,43,44,44,44,44},
{16,16,16,16,16,16,16,16,16,16,17,17,17,18,19,22,30,38,41,42,43,43,43,44,44,44,44,44,44,44,44,44,44},
{15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,0,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45},
{14,14,14,14,14,14,14,14,14,14,13,13,13,12,11,8,0,52,49,48,47,47,47,46,46,46,46,46,46,46,46,46,46},
{14,14,14,14,13,13,13,13,13,12,12,11,11,9,8,4,0,56,52,51,49,49,48,48,47,47,47,47,47,46,46,46,46},
{13,13,13,13,13,12,12,12,12,11,11,10,9,8,6,3,0,57,54,52,51,50,49,49,48,48,48,48,47,47,47,47,47},
{13,13,12,12,12,12,11,11,11,10,9,9,8,6,4,2,0,58,56,54,52,51,51,50,49,49,49,48,48,48,48,47,47},
{12,12,12,11,11,11,11,10,10,9,8,8,6,5,4,2,0,58,56,55,54,52,52,51,50,50,49,49,49,49,48,48,48},
{12,11,11,11,11,10,10,9,9,8,8,7,6,4,3,2,0,58,57,56,54,53,52,52,51,51,50,50,49,49,49,49,48},
{11,11,11,10,10,10,9,9,8,8,7,6,5,4,3,1,0,59,57,56,55,54,53,52,52,51,51,50,50,50,49,49,49},
{11,10,10,10,9,9,9,8,8,7,6,5,4,3,2,1,0,59,58,57,56,55,54,53,52,52,51,51,51,50,50,50,49},
{10,10,10,9,9,8,8,8,7,6,6,5,4,3,2,1,0,59,58,57,56,55,54,54,53,52,52,52,51,51,50,50,50},
{10,9,9,9,8,8,8,7,6,6,5,4,4,3,2,1,0,59,58,57,56,56,55,54,54,53,52,52,52,51,51,51,50},
{9,9,9,8,8,8,7,7,6,5,5,4,3,3,2,1,0,59,58,57,57,56,55,55,54,53,53,52,52,52,51,51,51},
{9,9,8,8,8,7,7,6,6,5,4,4,3,2,2,1,0,59,58,58,57,56,56,55,54,54,53,53,52,52,52,51,51},
{8,8,8,8,7,7,6,6,5,5,4,4,3,2,1,1,0,59,59,58,57,56,56,55,55,54,54,53,53,52,52,52,52},
{8,8,8,7,7,6,6,5,5,4,4,3,3,2,1,1,0,59,59,58,57,57,56,56,55,55,54,54,53,53,52,52,52},
{8,8,7,7,6,6,6,5,5,4,4,3,2,2,1,1,0,59,59,58,58,57,56,56,55,55,54,54,54,53,53,52,52},
{8,7,7,7,6,6,5,5,4,4,3,3,2,2,1,1,0,59,59,58,58,57,57,56,56,55,55,54,54,53,53,53,52}
};

int r[33][33] = 
{{23,22,21,21,20,19,19,18,18,17,17,17,16,16,16,16,16,16,16,16,16,17,17,17,18,18,19,19,20,21,21,22,23},
{22,21,21,20,19,19,18,17,17,17,16,16,16,15,15,15,15,15,15,15,16,16,16,17,17,17,18,19,19,20,21,21,22},
{21,21,20,19,18,18,17,17,16,16,15,15,15,14,14,14,14,14,14,14,15,15,15,16,16,17,17,18,18,19,20,21,21},
{21,20,19,18,18,17,16,16,15,15,14,14,14,13,13,13,13,13,13,13,14,14,14,15,15,16,16,17,18,18,19,20,21},
{20,19,18,18,17,16,16,15,14,14,13,13,13,12,12,12,12,12,12,12,13,13,13,14,14,15,16,16,17,18,18,19,20},
{19,19,18,17,16,16,15,14,14,13,13,12,12,11,11,11,11,11,11,11,12,12,13,13,14,14,15,16,16,17,18,19,19},
{19,18,17,16,16,15,14,13,13,12,12,11,11,10,10,10,10,10,10,10,11,11,12,12,13,13,14,15,16,16,17,18,19},
{18,17,17,16,15,14,13,13,12,11,11,10,10,9,9,9,9,9,9,9,10,10,11,11,12,13,13,14,15,16,17,17,18},
{18,17,16,15,14,14,13,12,11,11,10,9,9,9,8,8,8,8,8,9,9,9,10,11,11,12,13,14,14,15,16,17,18},
{17,17,16,15,14,13,12,11,11,10,9,9,8,8,7,7,7,7,7,8,8,9,9,10,11,11,12,13,14,15,16,17,17},
{17,16,15,14,13,13,12,11,10,9,8,8,7,7,6,6,6,6,6,7,7,8,8,9,10,11,12,13,13,14,15,16,17},
{17,16,15,14,13,12,11,10,9,9,8,7,6,6,5,5,5,5,5,6,6,7,8,9,9,10,11,12,13,14,15,16,17},
{16,16,15,14,13,12,11,10,9,8,7,6,6,5,4,4,4,4,4,5,6,6,7,8,9,10,11,12,13,14,15,16,16},
{16,15,14,13,12,11,10,9,9,8,7,6,5,4,4,3,3,3,4,4,5,6,7,8,9,9,10,11,12,13,14,15,16},
{16,15,14,13,12,11,10,9,8,7,6,5,4,4,3,2,2,2,3,4,4,5,6,7,8,9,10,11,12,13,14,15,16},
{16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,1,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16},
{16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16},
{16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,1,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16},
{16,15,14,13,12,11,10,9,8,7,6,5,4,4,3,2,2,2,3,4,4,5,6,7,8,9,10,11,12,13,14,15,16},
{16,15,14,13,12,11,10,9,9,8,7,6,5,4,4,3,3,3,4,4,5,6,7,8,9,9,10,11,12,13,14,15,16},
{16,16,15,14,13,12,11,10,9,8,7,6,6,5,4,4,4,4,4,5,6,6,7,8,9,10,11,12,13,14,15,16,16},
{17,16,15,14,13,12,11,10,9,9,8,7,6,6,5,5,5,5,5,6,6,7,8,9,9,10,11,12,13,14,15,16,17},
{17,16,15,14,13,13,12,11,10,9,8,8,7,7,6,6,6,6,6,7,7,8,8,9,10,11,12,13,13,14,15,16,17},
{17,17,16,15,14,13,12,11,11,10,9,9,8,8,7,7,7,7,7,8,8,9,9,10,11,11,12,13,14,15,16,17,17},
{18,17,16,15,14,14,13,12,11,11,10,9,9,9,8,8,8,8,8,9,9,9,10,11,11,12,13,14,14,15,16,17,18},
{18,17,17,16,15,14,13,13,12,11,11,10,10,9,9,9,9,9,9,9,10,10,11,11,12,13,13,14,15,16,17,17,18},
{19,18,17,16,16,15,14,13,13,12,12,11,11,10,10,10,10,10,10,10,11,11,12,12,13,13,14,15,16,16,17,18,19},
{19,19,18,17,16,16,15,14,14,13,13,12,12,11,11,11,11,11,11,11,12,12,13,13,14,14,15,16,16,17,18,19,19},
{20,19,18,18,17,16,16,15,14,14,13,13,13,12,12,12,12,12,12,12,13,13,13,14,14,15,16,16,17,18,18,19,20},
{21,20,19,18,18,17,16,16,15,15,14,14,14,13,13,13,13,13,13,13,14,14,14,15,15,16,16,17,18,18,19,20,21},
{21,21,20,19,18,18,17,17,16,16,15,15,15,14,14,14,14,14,14,14,15,15,15,16,16,17,17,18,18,19,20,21,21},
{22,21,21,20,19,19,18,17,17,17,16,16,16,15,15,15,15,15,15,15,16,16,16,17,17,17,18,19,19,20,21,21,22},
{23,22,21,21,20,19,19,18,18,17,17,17,16,16,16,16,16,16,16,16,16,17,17,17,18,18,19,19,20,21,21,22,23}
};

bool digits[10][5][3] = {
    {
        {1, 1, 1},
        {1, 0, 1},
        {1, 0, 1},
        {1, 0, 1},
        {1, 1, 1},
    },
    {
        {0, 1, 0},
        {1, 1, 0},
        {0, 1, 0},
        {0, 1, 0},
        {1, 1, 1},
    },
    {
        {1, 1, 1},
        {0, 0, 1},
        {1, 1, 1},
        {1, 0, 0},
        {1, 1, 1},
    },
    {
        {1, 1, 1},
        {0, 0, 1},
        {1, 1, 1},
        {0, 0, 1},
        {1, 1, 1},
    },
    {
        {1, 0, 1},
        {1, 0, 1},
        {1, 1, 1},
        {0, 0, 1},
        {0, 0, 1},
    },
    {
        {1, 1, 1},
        {1, 0, 0},
        {1, 1, 1},
        {0, 0, 1},
        {1, 1, 1},
    },
    {
        {1, 1, 1},
        {1, 0, 0},
        {1, 1, 1},
        {1, 0, 1},
        {1, 1, 1},
    },
    {
        {1, 1, 1},
        {0, 0, 1},
        {0, 1, 0},
        {0, 1, 0},
        {0, 1, 0},
    },
    {
        {1, 1, 1},
        {1, 0, 1},
        {1, 1, 1},
        {1, 0, 1},
        {1, 1, 1},
    },
    {
        {1, 1, 1},
        {1, 0, 1},
        {1, 1, 1},
        {0, 0, 1},
        {1, 1, 1},
    }
};

bool colon[5][2] = {
    {0, 0},
    {1, 1},
    {0, 0},
    {1, 1},
    {0, 0},
};

bool digits2[10][14][7] = {
    {
        {1, 1, 1, 1, 1, 1, 1},
        {1, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 1},
        {1, 1, 1, 1, 1, 1, 1},
    },
    {
        {0, 0, 0, 1, 0, 0, 0},
        {0, 0, 1, 1, 0, 0, 0},
        {0, 1, 0, 1, 0, 0, 0},
        {1, 0, 0, 1, 0, 0, 0},
        {0, 0, 0, 1, 0, 0, 0},
        {0, 0, 0, 1, 0, 0, 0},
        {0, 0, 0, 1, 0, 0, 0},
        {0, 0, 0, 1, 0, 0, 0},
        {0, 0, 0, 1, 0, 0, 0},
        {0, 0, 0, 1, 0, 0, 0},
        {0, 0, 0, 1, 0, 0, 0},
        {0, 0, 0, 1, 0, 0, 0},
        {0, 0, 0, 1, 0, 0, 0},
        {1, 1, 1, 1, 1, 1, 1},
    },
    {
        {1, 1, 1, 1, 1, 1, 1},
        {0, 0, 0, 0, 0, 0, 1},
        {0, 0, 0, 0, 0, 0, 1},
        {0, 0, 0, 0, 0, 0, 1},
        {0, 0, 0, 0, 0, 0, 1},
        {0, 0, 0, 0, 0, 0, 1},
        {0, 0, 0, 0, 0, 0, 1},
        {1, 1, 1, 1, 1, 1, 1},
        {1, 0, 0, 0, 0, 0, 0},
        {1, 0, 0, 0, 0, 0, 0},
        {1, 0, 0, 0, 0, 0, 0},
        {1, 0, 0, 0, 0, 0, 0},
        {1, 0, 0, 0, 0, 0, 0},
        {1, 1, 1, 1, 1, 1, 1},
    },
    {
        {1, 1, 1, 1, 1, 1, 1},
        {0, 0, 0, 0, 0, 0, 1},
        {0, 0, 0, 0, 0, 0, 1},
        {0, 0, 0, 0, 0, 0, 1},
        {0, 0, 0, 0, 0, 0, 1},
        {0, 0, 0, 0, 0, 0, 1},
        {0, 0, 0, 0, 0, 0, 1},
        {1, 1, 1, 1, 1, 1, 1},
        {0, 0, 0, 0, 0, 0, 1},
        {0, 0, 0, 0, 0, 0, 1},
        {0, 0, 0, 0, 0, 0, 1},
        {0, 0, 0, 0, 0, 0, 1},
        {0, 0, 0, 0, 0, 0, 1},
        {1, 1, 1, 1, 1, 1, 1},
    },
    {
        {1, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 1},
        {1, 1, 1, 1, 1, 1, 1},
        {0, 0, 0, 0, 0, 0, 1},
        {0, 0, 0, 0, 0, 0, 1},
        {0, 0, 0, 0, 0, 0, 1},
        {0, 0, 0, 0, 0, 0, 1},
        {0, 0, 0, 0, 0, 0, 1},
        {0, 0, 0, 0, 0, 0, 1},
    },
    {
        {1, 1, 1, 1, 1, 1, 1},
        {1, 0, 0, 0, 0, 0, 0},
        {1, 0, 0, 0, 0, 0, 0},
        {1, 0, 0, 0, 0, 0, 0},
        {1, 0, 0, 0, 0, 0, 0},
        {1, 0, 0, 0, 0, 0, 0},
        {1, 0, 0, 0, 0, 0, 0},
        {1, 1, 1, 1, 1, 1, 1},
        {0, 0, 0, 0, 0, 0, 1},
        {0, 0, 0, 0, 0, 0, 1},
        {0, 0, 0, 0, 0, 0, 1},
        {0, 0, 0, 0, 0, 0, 1},
        {0, 0, 0, 0, 0, 0, 1},
        {1, 1, 1, 1, 1, 1, 1},
    },
    {
        {1, 1, 1, 1, 1, 1, 1},
        {1, 0, 0, 0, 0, 0, 0},
        {1, 0, 0, 0, 0, 0, 0},
        {1, 0, 0, 0, 0, 0, 0},
        {1, 0, 0, 0, 0, 0, 0},
        {1, 0, 0, 0, 0, 0, 0},
        {1, 0, 0, 0, 0, 0, 0},
        {1, 1, 1, 1, 1, 1, 1},
        {1, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 1},
        {1, 1, 1, 1, 1, 1, 1},
    },
    {
        {1, 1, 1, 1, 1, 1, 1},
        {0, 0, 0, 0, 0, 0, 1},
        {0, 0, 0, 0, 0, 1, 0},
        {0, 0, 0, 0, 0, 1, 0},
        {0, 0, 0, 0, 1, 0, 0},
        {0, 0, 0, 0, 1, 0, 0},
        {0, 0, 0, 1, 0, 0, 0},
        {0, 0, 0, 1, 0, 0, 0},
        {0, 0, 1, 0, 0, 0, 0},
        {0, 0, 1, 0, 0, 0, 0},
        {0, 1, 0, 0, 0, 0, 0},
        {0, 1, 0, 0, 0, 0, 0},
        {1, 0, 0, 0, 0, 0, 0},
        {1, 0, 0, 0, 0, 0, 0},
    },
    {
        {1, 1, 1, 1, 1, 1, 1},
        {1, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 1},
        {1, 1, 1, 1, 1, 1, 1},
        {1, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 1},
        {1, 1, 1, 1, 1, 1, 1},
    },
    {
        {1, 1, 1, 1, 1, 1, 1},
        {1, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 1},
        {1, 1, 1, 1, 1, 1, 1},
        {0, 0, 0, 0, 0, 0, 1},
        {0, 0, 0, 0, 0, 0, 1},
        {0, 0, 0, 0, 0, 0, 1},
        {0, 0, 0, 0, 0, 0, 1},
        {0, 0, 0, 0, 0, 0, 1},
        {1, 1, 1, 1, 1, 1, 1},
    }
};

bool colon2[14][2] = {
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {1, 1},
    {1, 1},
    {0, 0},
    {0, 0},
    {1, 1},
    {1, 1},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
};
//int hash = 60;

bool hourHand[4][3] = {
    {0, 1, 0},
    {0, 1, 0},
    {1, 1, 1},
    {0, 1, 0},
};

bool minuteHand[8][3] = {
    {0, 1, 0},
    {0, 1, 0},
    {0, 1, 0},
    {0, 1, 0},
    {0, 1, 0},
    {0, 1, 0},
    {1, 1, 1},
    {0, 1, 0},
};

uint32_t Calc_deg(int time_now)
{
    uint32_t deg=time_now;
    deg = (deg + turnTime/2)%turnTime;
    return deg;
}

void reinitArrayBool2() 
{
    volatile int i = 0;
    volatile int li = sizeof(ledStates[0])/sizeof(ledStates[0][0]);
    while(i < li) {
        volatile int j = 0;
        volatile int lj = sizeof(ledStates)/sizeof(ledStates[0][0]);
        while(j < lj) {
            ledStates[i][j] = 0;
                j++;
        }
    i++;
    }
}

void hourHandToLedStates(int hour) {
    volatile int li = 3;
    volatile int i = -1*li/2;
    volatile int lj = 4 - 1;
    volatile int j = 0;
    int k = 0;
    while(i <= li/2) {
        k = (hour%12)*60/12+i;
        if(k < 0)
            k = 60 + k;
        else if(k >= 60)
            k = k - 60;
        
        j = lj;
        while(j >= 0) {
            ledStates[k][j] |= hourHand[j][i+li/2];
            j--;
        }
        i++;
    }
}

void minuteHandToLedStates(int minute) {
    volatile int li = 3;
    volatile int i = -1*li/2;
    volatile int lj = 8 - 1;
    volatile int j = 0;
    int k = 0;
    while(i <= li/2) {
        k = minute+i;
        if(k < 0)
            k = 60 + k;
        else if(k >= 60)
            k = k - 60;
        
        j = lj;
        while(j >= 0) {
            ledStates[k][j] |= minuteHand[j][i+li/2];
            j--;
        }
        i++;
    }
}

int digitToLedStates(int digit, int a, int li, int lj, bool display) {
    volatile int i = 0;
    volatile int j = lj;
    if(display) {
        while(i < li) {
            j = lj-1;
            while(j >= 0) {
                if(mode == 2){
                    ledStates[a+i][lj-j+(16-lj)/2] = digits[digit][j][i];
                }
                else if(mode == 3){
                    ledStates[a+i][lj-j+(16-lj)/2] = digits2[digit][j][i];
                }
                j--;
            }
            i++;
        }
    }

    return((a+li+1)%60);
}

int digitToLedStatesRect(int digit, int a, int li, int lj, bool display){
    volatile int i = 0;
    volatile int j = 0;
    volatile int offsetx = 0;
    volatile int offsety = 0;

    if(a == 1){
        offsetx = 2;
        offsety = 2;
    }
    else if(a == 2){
        offsetx = 20;
        offsety = 2;
    }
    else if(a == 2){
        offsetx = 2;
        offsety = 20;
    }
    else if(a == 2){
        offsetx = 20;
        offsety = 20;
    }
    while(i < li){
        while(j < lj){
            ledStates[angle[offsetx+i][offsety+j]][r[offsetx+i][offsety+j]] = digits[digit][j][i];
            j++;
        }
        i++;
    }
    return 0;
}

int colonToLedStates(int a, int li, int lj, bool display) {
    volatile int i = 0;
    volatile int j = 0;
    if(display) {
        while(i < li) {
            j = 0;
            while(j < lj) {
                int k = j+(16-lj)/2+((16-lj)/2)%2;
                if(mode == 2){
                    ledStates[a+i][k] = colon[j][i];
                }
                else if(mode == 3){
                    ledStates[a+i][k] = colon2[j][i];
                }
                j++;
            }
            i++;
        }
    }

    return((a+li+1)%60);
}

int lastH1 = -1;
int lastH2 = -1;
int lastM1 = -1;
int lastM2 = -1;

void hourToCurveLed(int hour, int minute) {

    int h1 = hour/10;
    int h2 = hour - h1*10;
    int m1 = minute/10;
    int m2 = minute - m1*10;
    int i = 0;
    if(mode == 1) {
        reinitArrayBool2();
        hourHandToLedStates(hour);
        minuteHandToLedStates(minute);
    }
    else if(mode == 2) {
        if(changedMode)
            reinitArrayBool2();

        //i = 0;
        i = 51;

        // if(lastH1 != h1 || changedMode) {
        //     digitToLedStates(h1, i, 3, 5);
        // }
        i = digitToLedStates(h1, i, 3, 5, lastH1 != h1 || changedMode);
        // i += 4;
        // i = i%60;

        // if(lastH2 != h2 || changedMode) {
        //     digitToLedStates(h2, i, 3, 5);
        // }
        // i += 4;
        // i = i%60;
        i = digitToLedStates(h2, i, 3, 5, lastH2 != h2 || changedMode);

        // if(changedMode)
        //     colonToLedStates(i, 2, 5);
        // i += 3;
        // i = i%60;
        i = colonToLedStates(i, 2, 5, changedMode);

        // if(lastM1 != m1 || changedMode) {
        //     digitToLedStates(m1, i, 3, 5);
        // }
        // i += 4;
        // i = i%60;
        i = digitToLedStates(m1, i, 3, 5, lastM1 != m1 || changedMode);

        // if(lastM2 != m2 || changedMode) {
        //     digitToLedStates(m2, i, 3, 5);
        // }
        // i += 4;
        // i = i%60;
        i = digitToLedStates(m2, i, 3, 5, lastM2 != m2 || changedMode);
    }
    else if(mode == 3) {
        if(changedMode)
            reinitArrayBool2();

        i = 43;
        i = i%60;

        // if(lastH1 != h1 || changedMode) {
        //     digitToLedStates(h1, i, 7, 14);
        // }
        // i += 8;
        // i = i%60;
        i = digitToLedStates(h1, i, 7, 14, lastH1 != h1 || changedMode);

        // if(lastH2 != h2 || changedMode) {
        //     digitToLedStates(h2, i, 7, 14);
        // }
        // i += 8;
        // i = i%60;
        i = digitToLedStates(h2, i, 7, 14, lastH2 != h2 || changedMode);

        // if(changedMode)
        //     colonToLedStates(i, 2, 14);
        // i += 3;
        // i = i%60;
        i = colonToLedStates(i, 2, 14, changedMode);

        // if(lastM1 != m1 || changedMode) {
        //     digitToLedStates(m1, i, 7, 14);
        // }
        // i += 8;
        // i = i%60;
        i = digitToLedStates(m1, i, 7, 14, lastM1 != m1 || changedMode);

        // if(lastM2 != m2 || changedMode) {
        //     digitToLedStates(m2, i, 7, 14);
        // }
        // i += 8;
        // i = i%60;
        i = digitToLedStates(m2, i, 7, 14, lastM2 != m2 || changedMode);

        // i = digitToLedStates(h1, i, 7, 14);
        // i = digitToLedStates(h2, i, 7, 14);
        // i = colonToLedStates(i, 2, 14);
        // i = digitToLedStates(m1, i, 7, 14);
        // i = digitToLedStates(m2, i, 7, 14);
    }
    else if(mode == 4) {
        if(changedMode)
            reinitArrayBool2();

        i = digitToLedStatesRect(h1, 1, 3, 5, lastH1 != h1 || changedMode);

        i = digitToLedStatesRect(h2, 2, 3, 5, lastH2 != h2 || changedMode);

        i = digitToLedStatesRect(m1, 3, 3, 5, lastM1 != m1 || changedMode);

        i = digitToLedStatesRect(m2, 4, 3, 5, lastM2 != m2 || changedMode);
    }

    lastH1 = h1;
    lastH2 = h2;
    lastM1 = m1;
    lastM2 = m2;
    changedMode = false;
}

void displayCurveTime(int hour, int minute) {
    if(lastMinute != minute || lastHour != hour) {
        hourToCurveLed(hour, minute);
        lastHour = hour;
        lastMinute = minute;
    }

    if(TCNT3 >= (turnTime/60)*(tour+1)) {
        tour += 1;

        char1 = 0x0000;
        char2 = 0x0000;

        volatile int j = 0;
        while(j < 16) {
            if(j < 8 && ledStates[(tour+30)%60][j])
                char1 |= 1 << (j%8);
            else if(j >= 8 && ledStates[(tour+30)%60][j])
                char2 |= 1 << (j%8);
            j++;
        }
    }
    int deg = Calc_deg(TCNT3);
    if(deg < minute*10*(turnTime/600)){
        char2 = char2 | 0x8000;
    }

    SPI_MasterTransmit(char2, char1);
}

