#pragma warning(disable:4996)
#include <stdio.h>
#include <stdlib.h>
#include "bitmap.h"
#include <math.h>

typedef uint8_t BYTE;

// 이미지 반전
void InverseImage(BYTE* Img, BYTE* Out, int W, int H) {
    int ImgSize = W * H;
    for (int i = 0; i < ImgSize; i++) {
        Out[i] = 255 - Img[i];
    }
}

// 이진화
void Binarization(BYTE* Img, BYTE* Out, int W, int H, BYTE Threshold) {
    int ImgSize = W * H;
    for (int i = 0; i < ImgSize; i++) {
        Out[i] = (Img[i] < Threshold) ? 0 : 255;
    }
}

// 스택에 좌표 추가
int push(short* stackx, short* stacky, int arr_size, short vx, short vy, int* top) {
    if (*top >= arr_size) return -1;
    (*top)++;
    stackx[*top] = vx;
    stacky[*top] = vy;
    return 1;
}

// 스택에서 좌표 제거
int pop(short* stackx, short* stacky, short* vx, short* vy, int* top) {
    if (*top == 0) return -1;
    *vx = stackx[*top];
    *vy = stacky[*top];
    (*top)--;
    return 1;
}

// GlassFire 알고리즘을 이용한 라벨링
void m_BlobColoring(BYTE* CutImage, int height, int width) {
    int i, j, m, n, top, area, Out_Area, index, BlobArea[1000];
    long k;
    short curColor = 0, r, c;
    Out_Area = 1;

    short* stackx = new short[height * width];
    short* stacky = new short[height * width];
    short* coloring = new short[height * width];

    for (k = 0; k < height * width; k++) coloring[k] = 0;

    for (i = 0; i < height; i++) {
        index = i * width;
        for (j = 0; j < width; j++) {
            if (coloring[index + j] != 0 || CutImage[index + j] != 255) continue;
            r = i; c = j; top = 0; area = 1;
            curColor++;

            while (1) {
                GRASSFIRE:
                for (m = r - 1; m <= r + 1; m++) {
                    index = m * width;
                    for (n = c - 1; n <= c + 1; n++) {
                        if (m < 0 || m >= height || n < 0 || n >= width) continue;

                        if ((int)CutImage[index + n] == 255 && coloring[index + n] == 0) {
                            coloring[index + n] = curColor;
                            if (push(stackx, stacky, height * width, (short)m, (short)n, &top) == -1) continue;
                            r = m; c = n; area++;
                            goto GRASSFIRE;
                        }
                    }
                }
                if (pop(stackx, stacky, &r, &c, &top) == -1) break;
            }
            if (curColor < 1000) BlobArea[curColor] = area;
        }
    }

    float grayGap = 255.0f / curColor;

    for (i = 1; i <= curColor; i++) {
        if (BlobArea[i] >= BlobArea[Out_Area]) Out_Area = i;
    }

    for (k = 0; k < width * height; k++) CutImage[k] = 255;

    for (k = 0; k < width * height; k++) {
        if (coloring[k] == Out_Area) CutImage[k] = 0;
    }

    delete[] coloring;
    delete[] stackx;
    delete[] stacky;
}

// 이진 이미지의 가장자리 검출
void BinaryImageEdgeDetection(BYTE* Bin, BYTE* Out, int W, int H) {
    for (int i = 0; i < H; i++) {
        for (int j = 0; j < W; j++) {
            if (Bin[i * W + j] == 0) {
                if (Bin[(i - 1) * W + j] == 0 && Bin[(i + 1) * W + j] == 0 &&
                    Bin[i * W + j - 1] == 0 && Bin[(i - 1) * W + j + 1] == 0)
                    continue;
                Out[i * W + j] = 0;
            }
        }
    }
}

// 사각형 테두리 그리기
void DrawRectOutline(BYTE* Img, int W, int H, int LU_X, int LU_Y, int RD_X, int RD_Y) {
    for (int i = LU_X; i < RD_X; i++) {
        Img[(LU_Y * W + i * 3)] = 0;
        Img[(LU_Y * W + i * 3) + 1] = 0;
        Img[(LU_Y * W + i * 3) + 2] = 255;
    }
    for (int i = LU_X; i < RD_X; i++) {
        Img[RD_Y * W + i * 3] = 0;
        Img[RD_Y * W + i * 3 + 1] = 0;
        Img[RD_Y * W + i * 3 + 2] = 255;
    }
    for (int i = LU_Y; i < RD_Y; i++) {
        Img[(i * W + LU_X * 3) + 0] = 0;
        Img[(i * W + LU_X * 3) + 1] = 0;
        Img[(i * W + LU_X * 3) + 2] = 255;
    }
    for (int i = LU_Y; i < RD_Y; i++) {
        Img[(i * W + RD_X * 3) + 0] = 0;
        Img[(i * W + RD_X * 3) + 1] = 0;
        Img[(i * W + RD_X * 3) + 2] = 255;
    }
}

// 교차선 그리기
void DrawCrossLine(BYTE* Img, int W, int H, int Cx, int Cy) {
    for (int i = 0; i < W - 1; i++) {
        Img[Cy * W + i] = 255;
    }
    for (int i = 0; i < H - 1; i++) {
        Img[i * W + Cx] = 255;
    }
}

// 2D 무게 중심 구하기
void Obtain2DCenter(BYTE* Image, int W, int H, int* Cx, int* Cy) {
    int SumX = 0, SumY = 0, cnt = 0;
    for (int i = 0; i < H; i++) {
        for (int j = 0; j < W; j++) {
            if (Image[i * W + j] == 0) {
                SumX += j;
                SumY += i;
                cnt++;
            }
        }
    }
    if (cnt == 0) cnt = 1;
    *Cx = SumX / cnt;
    *Cy = SumY / cnt;
    printf("%d %d\n", *Cx, *Cy);
}

// 2D 바운딩 박스 구하기
void Obtain2DBoundingBox(BYTE* Image, int W, int H, int* LUX, int* LUY, int* RDX, int* RDY) {
    int flag = 0;
    for (int i = 0; i < H; i++) {
        for (int j = 0; j < W; j++) {
            if (Image[i * W * 3 + j * 3] != 255 ||
                Image[i * W * 3 + j * 3 + 1] != 255 ||
                Image[i * W * 3 + j * 3 + 2] != 255) {
                *LUY = i;
                flag = 1;
                break;
            }
        }
        if (flag == 1) break;
    }
    flag = 0;
    for (int i = H - 1; i >= 0; i--) {
        for (int j = 0; j < W; j++) {
            if (Image[i * W * 3 + j * 3] != 255 ||
                Image[i * W * 3 + j * 3 + 1] != 255 ||
                Image[i * W * 3 + j * 3 + 2] != 255) {
                *RDY = i;
                flag = 1;
                break;
            }
        }
        if (flag == 1) break;
    }
    flag = 0;
    for (int j = 0; j < W; j++) {
        for (int i = 0; i < H; i++) {
            if (Image[i * W * 3 + j * 3] != 255 ||
                Image[i * W * 3 + j * 3 + 1] != 255 ||
                Image[i * W * 3 + j * 3 + 2] != 255) {
                *LUX = j;
                flag = 1;
                break;
            }
        }
        if (flag == 1) break;
    }
    flag = 0;
    for (int j = W - 1; j >= 0; j--) {
        for (int i = 0; i < H; i++) {
            if (Image[i * W * 3 + j * 3] != 255 ||
                Image[i * W * 3 + j * 3 + 1] != 255 ||
                Image[i * W * 3 + j * 3 + 2] != 255) {
                *RDX = j;
                flag = 1;
                break;
            }
        }
        if (flag == 1) break;
    }
    printf("%d %d %d %d\n", *LUX, *LUY, *RDX, *RDY);
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
    fp = fopen("pupil1.bmp", "rb");
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

    //경계검출
    Binarization(Image,Temp,hInfo.biWidth,hInfo.biHeight,100);
    m_BlobColoring(Temp,hInfo.biHeight,hInfo.biWidth);
    for(int i=0;i<ImgSize;i++){
        Output[i]=Image[i];
    }
    BinaryImageEdgeDetection(Temp,Output,hInfo.biWidth,hInfo.biHeight);

    //동공 경계 검출
    Binarization(Image,Output,hInfo.biWidth,hInfo.biHeight,50);
    InverseImage(Output,Output,hInfo.biWidth,hInfo.biHeight);
    m_BlobColoring(Output,hInfo.biHeight,hInfo.biWidth);
    int Cx, Cy;
    Obtain2DCenter(Output,W,H,&Cx,&Cy ); //이진 영상의 무게 중심
    int LUX,LUY,RDX,RDY;
    Obtain2DBoundingBox(Output,W,H,&LUX,&LUY,&RDX,&RDY);
    DrawRectOutline(Image,W,H,LUX,LUY,RDX,RDY);//이진영상의 외접직사각형 좌표 추출
    DrawCrossLine(Image,W,H,Cx,Cy);

    BinaryImageEdgeDetection(Temp,Output,hInfo.biWidth,hInfo.biHeight);
    SaveBMPFile(hf, hInfo, hRGB,Output, hInfo.biWidth, hInfo.biHeight, "output_line.bmp");


    free(Image);
    free(Output);
    free(Temp);
    return 0;
}