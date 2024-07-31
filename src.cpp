#pragma warning(disable:4996)
#include <stdio.h>
#include <stdlib.h>
#include "bitmap.h"
#include <math.h>

typedef uint8_t BYTE;

// 평균값 필터
void AverageConv(BYTE* Img, BYTE* Out, int W, int H) {
    double Kernel[3][3] = {
        {0.11111, 0.11111, 0.11111},
        {0.11111, 0.11111, 0.11111},
        {0.11111, 0.11111, 0.11111}
    };
    double SumProduct = 0.0;

    for (int i = 1; i < H - 1; i++) {
        for (int j = 1; j < W - 1; j++) {
            for (int m = -1; m <= 1; m++) {
                for (int n = -1; n <= 1; n++) {
                    SumProduct += Img[(i + m) * W + (j + n)] * Kernel[m + 1][n + 1];
                }
            }
            Out[i * W + j] = (BYTE)SumProduct;
            SumProduct = 0.0;
        }
    }
}

// 가우시안 평균값 필터
void GaussAvrConv(BYTE* Img, BYTE* Out, int W, int H) {
    double Kernel[3][3] = {
        {0.0625, 0.125, 0.0625},
        {0.125, 0.25, 0.125},
        {0.0625, 0.125, 0.0625}
    };
    double SumProduct = 0.0;

    for (int i = 1; i < H - 1; i++) {
        for (int j = 1; j < W - 1; j++) {
            for (int m = -1; m <= 1; m++) {
                for (int n = -1; n <= 1; n++) {
                    SumProduct += Img[(i + m) * W + (j + n)] * Kernel[m + 1][n + 1];
                }
            }
            Out[i * W + j] = (BYTE)SumProduct;
            SumProduct = 0.0;
        }
    }
}

// Prewitt 필터 (X 방향)
void Prewitt_X_Conv(BYTE* Img, BYTE* Out, int W, int H) {
    double Kernel[3][3] = {
        {-1.0, 0.0, 1.0},
        {-1.0, 0.0, 1.0},
        {-1.0, 0.0, 1.0}
    };
    double SumProduct = 0.0;

    for (int i = 1; i < H - 1; i++) {
        for (int j = 1; j < W - 1; j++) {
            for (int m = -1; m <= 1; m++) {
                for (int n = -1; n <= 1; n++) {
                    SumProduct += Img[(i + m) * W + (j + n)] * Kernel[m + 1][n + 1];
                }
            }
            Out[i * W + j] = abs((long)SumProduct) / 3;
            SumProduct = 0.0;
        }
    }
}

// Prewitt 필터 (Y 방향)
void Prewitt_Y_Conv(BYTE* Img, BYTE* Out, int W, int H) {
    double Kernel[3][3] = {
        {-1.0, -1.0, -1.0},
        {0.0, 0.0, 0.0},
        {1.0, 1.0, 1.0}
    };
    double SumProduct = 0.0;

    for (int i = 1; i < H - 1; i++) {
        for (int j = 1; j < W - 1; j++) {
            for (int m = -1; m <= 1; m++) {
                for (int n = -1; n <= 1; n++) {
                    SumProduct += Img[(i + m) * W + (j + n)] * Kernel[m + 1][n + 1];
                }
            }
            Out[i * W + j] = abs((long)SumProduct) / 3;
            SumProduct = 0.0;
        }
    }
}

// Sobel 필터 (X 방향)
void Sobel_X_Conv(BYTE* Img, BYTE* Out, int W, int H) {
    double Kernel[3][3] = {
        {-1.0, 0.0, 1.0},
        {-2.0, 0.0, 2.0},
        {-1.0, 0.0, 1.0}
    };
    double SumProduct = 0.0;

    for (int i = 1; i < H - 1; i++) {
        for (int j = 1; j < W - 1; j++) {
            for (int m = -1; m <= 1; m++) {
                for (int n = -1; n <= 1; n++) {
                    SumProduct += Img[(i + m) * W + (j + n)] * Kernel[m + 1][n + 1];
                }
            }
            Out[i * W + j] = abs((long)SumProduct) / 4;
            SumProduct = 0.0;
        }
    }
}

// Sobel 필터 (Y 방향)
void Sobel_Y_Conv(BYTE* Img, BYTE* Out, int W, int H) {
    double Kernel[3][3] = {
        {-1.0, -2.0, -1.0},
        {0.0, 0.0, 0.0},
        {1.0, 2.0, 1.0}
    };
    double SumProduct = 0.0;

    for (int i = 1; i < H - 1; i++) {
        for (int j = 1; j < W - 1; j++) {
            for (int m = -1; m <= 1; m++) {
                for (int n = -1; n <= 1; n++) {
                    SumProduct += Img[(i + m) * W + (j + n)] * Kernel[m + 1][n + 1];
                }
            }
            Out[i * W + j] = abs((long)SumProduct) / 4;
            SumProduct = 0.0;
        }
    }
}

// 라플라스 필터
void Laplace_Conv(BYTE* Img, BYTE* Out, int W, int H) {
    double Kernel[3][3] = {
        {-1.0, -1.0, -1.0},
        {-1.0, 8.0, -1.0},
        {-1.0, -1.0, -1.0}
    };
    double SumProduct = 0.0;

    for (int i = 1; i < H - 1; i++) {
        for (int j = 1; j < W - 1; j++) {
            for (int m = -1; m <= 1; m++) {
                for (int n = -1; n <= 1; n++) {
                    SumProduct += Img[(i + m) * W + (j + n)] * Kernel[m + 1][n + 1];
                }
            }
            Out[i * W + j] = abs((long)SumProduct) / 8;
            SumProduct = 0.0;
        }
    }
}

// 라플라스 필터 (동적 범위 조정)
void Laplace_Conv_DC(BYTE* Img, BYTE* Out, int W, int H) {
    double Kernel[3][3] = {
        {-1.0, -1.0, -1.0},
        {-1.0, 9.0, -1.0},
        {-1.0, -1.0, -1.0}
    };
    double SumProduct = 0.0;

    for (int i = 1; i < H - 1; i++) {
        for (int j = 1; j < W - 1; j++) {
            for (int m = -1; m <= 1; m++) {
                for (int n = -1; n <= 1; n++) {
                    SumProduct += Img[(i + m) * W + (j + n)] * Kernel[m + 1][n + 1];
                }
            }
            Out[i * W + j] = (SumProduct > 255.0) ? 255 : (SumProduct < 0.0) ? 0 : (BYTE)SumProduct;
            SumProduct = 0.0;
        }
    }
}

void Binarization(BYTE * Img, BYTE * Out, int W, int H, BYTE Threshold)
{
    int ImgSize = W * H;
    for (int i = 0; i < ImgSize; i++) {
        if (Img[i] < Threshold) Out[i] = 0;
        else Out[i] = 255;
    }
}


// SaveBMPFile
void SaveBMPFile(BITMAPFILEHEADER hf, BITMAPINFOHEADER hInfo,
                 RGBQUAD* hRGB, BYTE* Output, int W, int H, const char* FileName)
{
    FILE * fp = fopen(FileName, "wb");
    if(hInfo.biBitCount==24){
        fwrite(&hf, sizeof(BYTE), sizeof(BITMAPFILEHEADER), fp);
        fwrite(&hInfo, sizeof(BYTE), sizeof(BITMAPINFOHEADER), fp);
        fwrite(Output, sizeof(BYTE), W*H*3, fp);
    }
    else{
        fwrite(&hf, sizeof(BYTE), sizeof(BITMAPFILEHEADER), fp);
        fwrite(&hInfo, sizeof(BYTE), sizeof(BITMAPINFOHEADER), fp);
        fwrite(hRGB, sizeof(RGBQUAD), 256, fp);
        fwrite(Output, sizeof(BYTE), W*H, fp);
    }
    fclose(fp);
}

int main()
{
    BITMAPFILEHEADER hf; // 14바이트
    BITMAPINFOHEADER hInfo; // 40바이트
    RGBQUAD hRGB[256]; // 1024바이트
    FILE* fp;
    fp = fopen("LENNA.bmp", "rb");
    if (fp == NULL) {
        printf("File not found!\n");
        return -1;
    }
    fread(&hf, sizeof(BITMAPFILEHEADER), 1, fp);
    fread(&hInfo, sizeof(BITMAPINFOHEADER), 1, fp);
    fread(hRGB, sizeof(RGBQUAD), 256, fp);
    int H = hInfo.biHeight, W = hInfo.biWidth;
    int ImgSize = W*H;
    BYTE * Image = (BYTE *)malloc(ImgSize);
    BYTE * Temp = (BYTE*)malloc(ImgSize); // 임시배열
    BYTE* Output = (BYTE*)malloc(ImgSize);
    fread(Image, sizeof(BYTE), ImgSize, fp);
    fclose(fp);

    // Convolution Operations
    AverageConv(Image, Output, hInfo.biWidth, hInfo.biHeight);
    GaussAvrConv(Image, Output, W, H);
    Prewitt_X_Conv(Image, Output, W, H);
    Prewitt_Y_Conv(Image, Output, W, H);
    Sobel_X_Conv(Image, Output, W, H);
    Sobel_Y_Conv(Image, Output, hInfo.biWidth, hInfo.biHeight);
    Laplace_Conv(Image, Output, W, H);


    Laplace_Conv_DC(Image, Output, W, H);
    SaveBMPFile(hf, hInfo, hRGB, Output, W, H, "output.bmp");

    free(Image);
    free(Output);
    free(Temp);
    return 0;
}