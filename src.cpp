#pragma warning(disable:4996)
#include <stdio.h>
#include <stdlib.h>
#include "bitmap.h"
#include <math.h>

typedef uint8_t BYTE;

// 두 값을 교환하는 함수
void swap(BYTE* a, BYTE* b) {
    BYTE temp = *a;
    *a = *b;
    *b = temp;
}

// 수직 반전
void VerticalFlip(BYTE* Img, int W, int H) {
    for (int i = 0; i < H / 2; i++) {
        for (int j = 0; j < W; j++) {
            swap(&Img[i * W + j], &Img[(H - 1 - i) * W + j]);
        }
    }
}

// 수평 반전
void HorizontalFlip(BYTE* Img, int W, int H) {
    for (int i = 0; i < W / 2; i++) {
        for (int j = 0; j < H; j++) {
            swap(&Img[j * W + i], &Img[j * W + (W - 1 - i)]);
        }
    }
}

// 평행 이동
void Translation(BYTE* Image, BYTE* Output, int W, int H, int Tx, int Ty) {
    Ty *= -1;
    for (int i = 0; i < H; i++) {
        for (int j = 0; j < W; j++) {
            if ((i + Ty < H && i + Ty >= 0) && (j + Tx < W && j + Tx >= 0))
                Output[(i + Ty) * W + (j + Tx)] = Image[i * W + j];
        }
    }
}

// 스케일링
void Scaling(BYTE* Image, BYTE* Output, int W, int H, double SF_X, double SF_Y) {
    int tmpX, tmpY;
    for (int i = 0; i < H; i++) {
        for (int j = 0; j < W; j++) {
            tmpX = (int)(j / SF_X);
            tmpY = (int)(i / SF_Y);
            if (tmpY < H && tmpX < W)
                Output[i * W + j] = Image[tmpY * W + tmpX];
        }
    }
}

// 회전
void Rotation(BYTE* Image, BYTE* Output, int W, int H, int Angle) {
    int tmpX, tmpY;
    double Radian = Angle * 3.141592 / 180.0;
    for (int i = 0; i < H; i++) {
        for (int j = 0; j < W; j++) {
            tmpX = (int)(cos(Radian) * j + sin(Radian) * i);
            tmpY = (int)(-sin(Radian) * j + cos(Radian) * i);
            if ((tmpY < H && tmpY >= 0) && (tmpX < W && tmpX >= 0))
                Output[i * W + j] = Image[tmpY * W + tmpX];
        }
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
    int ImgSize = hInfo.biWidth * hInfo.biHeight;
    int H = hInfo.biHeight;
    int W = hInfo.biWidth;
    BYTE * Image = (BYTE *)malloc(ImgSize);
    BYTE * Temp = (BYTE*)malloc(ImgSize);
    BYTE* Output = (BYTE*)malloc(ImgSize);
    fread(Image, sizeof(BYTE), ImgSize, fp);
    fclose(fp);
    fclose(fp);
    VerticalFlip(Image, W, H);
    HorizontalFlip(Image, W, H);
    Translation(Image, Output, W, H, 50, 30);
    Scaling(Image, Output, W, H, 1.5, 1.5);
    Rotation(Image, Output, W, H, 45);

    SaveBMPFile(hf, hInfo, hRGB, Output, W, H, "output.bmp");

    free(Image);
    free(Output);
    free(Temp);
    return 0;
}