#include <iostream>
#include <limits>

#include "kernel.hpp"

template<int KH, int KW, int IH, int IW, int IC, int OH, int OW, int OC, int M>
void conv(
    int (&in)[IH*IW*IC],
    int (&weight)[OC*KH*KW*IC],
    int (&threshold)[M],
    int (&out)[OH*OW*OC]
) {
    for (int y = 0; y < OH; y++) {
        for (int x = 0; x < OW; x++) {
            for (int oc = 0; oc < OC; oc++) {
                int sum = 0;
                for (int ky = 0; ky < KH; ky++) {
                    for (int kx = 0; kx < KW; kx++) {
                        for (int ic = 0; ic < IC; ic++) {
                            sum += in[((y + ky) * IW + x + kx) * IC + ic] * weight[((oc * KH + ky) * KW + kx) * IC + ic];
                        }
                    }
                }
                int o = 0;
                for (int i = 0; i < M; i++) {
                    if (sum >= threshold[i]) {
                        o = i + 1;
                    }
                }
                out[(y * OW + x) * OC + oc] = o;
            }
        }
    }
}

template<int N, int C>
bool compare(
    int (&x)[N][C],
    int (&y)[N][C]
) {
    bool match = true;
    for (int i = 0; i < N; i++) {
        for (int c = 0; c < C; c++) {
            if (x[i][c] != y[i][c]) {
                printf("mismatch : (%d, %d) %d != %d\n", i, c, x[i][c], y[i][c]);
                match = false;
                break;
            }
        }
    }
    return match;
}

extern uint32_t images[4][28];
extern int conv0_weight[16 * 5 * 5 * 1];
extern int threshold0[3];

int main(int argc, char** argv)
{
    const int N = 4;

    int input[N][28*28*1];

    for (int i = 0; i < N; i++) {
        for (int y = 0; y < 28; y++) {
            auto v = images[i][y];
            for (int x = 0; x < 28; x++) {
                input[i][y * 28 + x] = (v >> (27 - x)) & 1;
            }
        }
    }

    int conv0_ref[N][24*24*16];
    for (int i = 0; i < N; i++) {
        conv<5,5, 28,28,1, 24,24,16>(input[i], conv0_weight, threshold0, conv0_ref[i]);
    }

    int conv0_out[N][24*24*16];
    for (int i = 0; i < N; i++) {
        kernel(input[i], conv0_weight, threshold0, conv0_out[i]);
    }

    if (!compare(conv0_out, conv0_ref))
        return EXIT_FAILURE;
}

uint32_t images[4][28] = {
    { 0b0000000000000000000000000000, 
      0b0000000000000000000000000000, 
      0b0000000000000000000000000000, 
      0b0000000000000000000000000000, 
      0b0000000000000000000000000000, 
      0b0000000000000000000000000000, 
      0b0000000000000000000000000000, 
      0b0000000111000000000000000000, 
      0b0000001111111111111110000000, 
      0b0000000101111111111111000000, 
      0b0000000000000000000111000000, 
      0b0000000000000000000110000000, 
      0b0000000000000000001100000000, 
      0b0000000000000000011100000000, 
      0b0000000000000000011000000000, 
      0b0000000000000000111000000000, 
      0b0000000000000000110000000000, 
      0b0000000000000001110000000000, 
      0b0000000000000001100000000000, 
      0b0000000000000011100000000000, 
      0b0000000000000111000000000000, 
      0b0000000000000110000000000000, 
      0b0000000000001110000000000000, 
      0b0000000000011100000000000000, 
      0b0000000000011100000000000000, 
      0b0000000000111100000000000000, 
      0b0000000000111000000000000000, 
      0b0000000000000000000000000000, 
    },
    { 0b0000000000000000000000000000, 
      0b0000000000000000000000000000, 
      0b0000000000000000000000000000, 
      0b0000000000111111100000000000, 
      0b0000000001111111100000000000, 
      0b0000000011111111110000000000, 
      0b0000000011100001110000000000, 
      0b0000000011000011100000000000, 
      0b0000000000000011100000000000, 
      0b0000000000000111100000000000, 
      0b0000000000000111000000000000, 
      0b0000000000001111000000000000, 
      0b0000000000011110000000000000, 
      0b0000000000011100000000000000, 
      0b0000000000111100000000000000, 
      0b0000000000111000000000000000, 
      0b0000000001111000000000000000, 
      0b0000000011110000000000000000, 
      0b0000000011100000000000000000, 
      0b0000000011100000000000111100, 
      0b0000000011111111111111111110, 
      0b0000000011111111111111111100, 
      0b0000000001111111111100000000, 
      0b0000000000000000000000000000, 
      0b0000000000000000000000000000, 
      0b0000000000000000000000000000, 
      0b0000000000000000000000000000, 
      0b0000000000000000000000000000, 
    },
    { 0b0000000000000000000000000000, 
      0b0000000000000000000000000000, 
      0b0000000000000000000000000000, 
      0b0000000000000000000000000000, 
      0b0000000000000000011000000000, 
      0b0000000000000000110000000000, 
      0b0000000000000000110000000000, 
      0b0000000000000000110000000000, 
      0b0000000000000000100000000000, 
      0b0000000000000001100000000000, 
      0b0000000000000001100000000000, 
      0b0000000000000011100000000000, 
      0b0000000000000011000000000000, 
      0b0000000000000011000000000000, 
      0b0000000000000111000000000000, 
      0b0000000000000110000000000000, 
      0b0000000000000110000000000000, 
      0b0000000000001110000000000000, 
      0b0000000000001110000000000000, 
      0b0000000000001100000000000000, 
      0b0000000000001100000000000000, 
      0b0000000000011100000000000000, 
      0b0000000000011100000000000000, 
      0b0000000000011000000000000000, 
      0b0000000000000000000000000000, 
      0b0000000000000000000000000000, 
      0b0000000000000000000000000000, 
      0b0000000000000000000000000000, 
    },
    { 0b0000000000000000000000000000, 
      0b0000000000000000000000000000, 
      0b0000000000000000000000000000, 
      0b0000000000000000000000000000, 
      0b0000000000000111000000000000, 
      0b0000000000000111100000000000, 
      0b0000000000001111100000000000, 
      0b0000000000111111110000000000, 
      0b0000000000111111111000000000, 
      0b0000000001111111111100000000, 
      0b0000000001111110011110000000, 
      0b0000000011111100001111000000, 
      0b0000000011110000001111000000, 
      0b0000000011100000000111000000, 
      0b0000000011000000000111100000, 
      0b0000000111000000001111000000, 
      0b0000000111000000001111000000, 
      0b0000000111000000111110000000, 
      0b0000000111000001111110000000, 
      0b0000000111001111111100000000, 
      0b0000000111111111111100000000, 
      0b0000000011111111110000000000, 
      0b0000000001111111100000000000, 
      0b0000000000011100000000000000, 
      0b0000000000000000000000000000, 
      0b0000000000000000000000000000, 
      0b0000000000000000000000000000, 
      0b0000000000000000000000000000, 
    },
};

int conv0_weight[16 * 5 * 5 * 1] = 
{  0, -1, -1, -1, -1,  0,  0, -1, -1,  0,  0,  1,  0,  0,  1,  1,  1,  1,  0,  1,  1,  0,  0,  0,  0, 
   1,  1,  0,  0, -1,  1,  1,  0, -1, -1,  1,  0, -1, -1, -1,  1,  0, -1, -1, -1,  0,  0, -1, -1, -1, 
  -1,  0,  0,  1,  1,  0,  0,  1,  1,  0,  1,  0,  0,  1,  0, -1,  0,  0,  0,  0,  0,  0,  0, -1,  0, 
   0, -1,  0,  0,  0, -1,  0,  0,  1,  1, -1,  0,  0,  1,  0, -1, -1,  1,  1,  0,  0,  0,  1,  0,  0, 
   0,  0,  0,  0, -1,  0, -1,  0,  0,  0,  0,  1,  1,  1, -1,  1,  1,  1,  0,  0,  1,  0,  1,  1,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  0,  1,  1,  0,  0,  0,  1,  0,  0,  0,  0, 
   1,  0,  0,  1,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  1,  0, -1, -1,  0,  0,  0, -1, -1, -1, -1, 
   0,  0,  0, -1,  0,  0, -1,  0,  0,  0, -1, -1, -1,  0,  0,  1,  0,  0,  1,  0,  0,  1,  0,  1,  1, 
   1,  0,  1,  0,  0,  1,  1,  0,  0,  0,  1,  0,  1,  0,  0, -1,  0, -1,  0,  0, -1,  0,  0,  0,  1, 
   0,  0,  0,  1,  0,  0,  0,  0,  1,  0,  1,  0,  0,  1,  1,  0,  0,  1,  0,  1,  0,  0,  0,  0, -1, 
   0, -1,  0,  0,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1, -1,  0,  1,  0,  1,  0, -1,  0,  0,  1, 
   0,  1,  0,  0,  0,  0,  0,  0,  1,  0,  1,  0,  0,  1,  1,  0,  0,  0,  0,  0, -1,  0,  0,  0,  0, 
   0,  0,  1,  1,  0,  1,  0,  1,  0,  0,  0,  0,  0,  0,  0, -1,  0, -1, -1,  0, -1, -1, -1, -1,  0, 
   1,  0,  1,  0,  1,  0,  0,  1,  0,  0,  0,  0,  1,  0,  0, -1, -1,  0,  0,  1, -1, -1, -1, -1, -1, 
   1,  0,  0,  0,  0,  1,  0,  0, -1,  0,  1,  0,  0, -1, -1,  0,  1,  0, -1, -1,  1,  1,  0,  0,  0, 
  -1, -1,  0, -1,  0, -1, -1,  0, -1, -1,  0,  0,  0,  0, -1,  1,  0,  0,  0,  0,  0,  1,  0,  1,  1, };

int threshold0[3] = { 1, 3, 4, };
