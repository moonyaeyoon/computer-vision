#pragma warning(disable:4996)
#include <stdio.h>
#include <stdlib.h>
#include "bitmap.h"
#include <math.h>

typedef uint8_t BYTE;


//이미지 반전
void InverseImage(BYTE* Img, BYTE *Out, int W, int H)
{
    int ImgSize = W * H;
    for (int i = 0; i < ImgSize; i++)
    {
        Out[i] = 255 - Img[i];
    }
}

//밝기 조정
void BrightnessAdj(BYTE* Img, BYTE* Out, int W, int H, int Val)
{
    int ImgSize = W * H;
    for (int i = 0; i < ImgSize; i++)
    {
        if (Img[i] + Val > 255)
        {
            Out[i] = 255;
        }
        else if (Img[i] + Val < 0)
        {
            Out[i] = 0;
        }
        else 	Out[i] =Img[i] + Val;
    }
}

//대비 조정
void ContrastAdj(BYTE* Img, BYTE* Out, int W, int H, double Val)
{
    int ImgSize = W * H;
    for (int i = 0; i < ImgSize; i++)
    {
        if (Img[i] * Val > 255.0)
        {
            Out[i] = 255;
        }
        else 	Out[i] = (BYTE)(Img[i] * Val);
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
    BYTE* Image;
    BYTE* Output;
    BYTE * Temp = (BYTE*)malloc(ImgSize);
    // True color
    if (hInfo.biBitCount == 24) {
        Image = (BYTE*)malloc(ImgSize * 3);
        Output = (BYTE*)malloc(ImgSize * 3);
        fread(Image, sizeof(BYTE), ImgSize * 3, fp);
    }
    // Grayscale
    else {
        fread(hRGB, sizeof(RGBQUAD), 256, fp);
        Image = (BYTE*)malloc(ImgSize);
        Output = (BYTE*)malloc(ImgSize);
        fread(Image, sizeof(BYTE), ImgSize, fp);
    }
    fclose(fp);



    // Inverse Image
    InverseImage(Image, Output, W, H);
    SaveBMPFile(hf, hInfo, hRGB, Output, W, H, "InverseImage.bmp");

    // Brightness Adjustment
    BrightnessAdj(Image, Output, W, H, 50);
    SaveBMPFile(hf, hInfo, hRGB, Output, W, H, "BrightnessAdj.bmp");

    // Contrast Adjustment
    ContrastAdj(Image, Output, W, H, 1.5);
    SaveBMPFile(hf, hInfo, hRGB, Output, W, H, "ContrastAdj.bmp");

    free(Image);
    free(Output);
    free(Temp);
    return 0;
}