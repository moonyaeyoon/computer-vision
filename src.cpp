#pragma warning(disable:4996)
#include <stdio.h>
#include <stdlib.h>
#include "bitmap.h"
#include <math.h>

typedef uint8_t BYTE;

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
    fp = fopen("coin.bmp", "rb");
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

    SaveBMPFile(hf, hInfo, hRGB, Output, W, H, "output.bmp");

    free(Image);
    free(Output);
    free(Temp);
    return 0;
}