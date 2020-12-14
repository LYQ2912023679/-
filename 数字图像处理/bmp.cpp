#include "stdafx.h"

BITMAPINFO* lpBitsInfo = NULL;

BOOL LoadBmpFile(char* BmpFileName)
{
	FILE* fp;
	if(NULL == (fp = fopen(BmpFileName,"rb")))
		return FALSE;
	
	BITMAPFILEHEADER bf;	//文件头
	BITMAPINFOHEADER bi;	//信息头

	//将文件头BITMAPFILEHEADER结构从文件中读出，填写到bf中
	fread(&bf,14,1,fp);

	//将文件头BITMAPINFOHEADER结构从文件中读出，填写到bf中
	fread(&bi,40,1,fp);

	//实际用到的颜色数，即调色板数组中的颜色个数
	DWORD NumColors;
	if(bi.biClrUsed != 0)
		NumColors = bi.biClrUsed;
	else {
		switch(bi.biBitCount)
		{
		case 1:
			NumColors = 2;
			break;
		case 2:
			NumColors = 24;
			break;
		case 4:
			NumColors = 16;
			break;
		case 8:
			NumColors = 256;
			break;
		case 24:
			NumColors = 0;
			break;
		}
	}

	DWORD LineBytes = (bi.biWidth * bi.biBitCount + 31)/32 * 4;

	DWORD ImgSize = LineBytes * bi.biHeight;

	DWORD size = 40 + NumColors * 4 + ImgSize;
	
	if(NULL== (lpBitsInfo = (BITMAPINFO*)malloc(size) ))
		return NULL;

	//文件指针重新定位到BITMAPINFOHEADER开始处
	fseek(fp, sizeof (BITMAPFILEHEADER),SEEK_SET);
	
	//将文件内容读入lpBitsInfo
	fread( (char*) lpBitsInfo,size, 1, fp);
	fclose(fp);
	lpBitsInfo->bmiHeader.biClrUsed = NumColors;
	
	return TRUE;
}

void gray()
{
	if(NULL == lpBitsInfo)
		return;
	int w = lpBitsInfo->bmiHeader.biWidth;
	int h = lpBitsInfo->bmiHeader.biHeight;
	int LineBytes = (w * lpBitsInfo->bmiHeader.biBitCount + 31)/32 * 4;
	BYTE* lpBits = 
	(BYTE*)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];

	int i, j;
	BYTE *R,*G,*B,avg;
	for (i = 0; i < h; i ++)
	{
		for (j = 0; j < w; j ++)
		{
			B = lpBits + LineBytes * i + j * 3;
			G = B + 1;
			R = G + 1;
			avg = (*R + *G + *B)/3;
			*R = *G = *B = avg;

		}
	}

}

DWORD H[256];
void Histogram()
{
	int w= lpBitsInfo->bmiHeader. biWidth;
	int h = lpBitsInfo->bmiHeader.biHeight;
	int LineBytes = (w * lpBitsInfo->bmiHeader. biBitCount + 31)/32* 4;
	BYTE* lpBits = (BYTE*)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];
	
	int i ,j;
	BYTE* pixel;
	
	for(i = 0; i < 256; i ++)
		H[i] =0;
	for(i = 0; i < h; i ++)
	{
		for (j = 0; j < w; j ++)
		{
			pixel =lpBits + LineBytes * (h - 1 - i) + j ;
			H[*pixel] ++;
		}
	}
}

BOOL IsGray()
{
	int r,g,b;
	if (lpBitsInfo->bmiHeader. biBitCount == 8)
	{
		r = lpBitsInfo->bmiColors[128].rgbRed;
		g = lpBitsInfo->bmiColors[128].rgbGreen;
		b = lpBitsInfo->bmiColors[128].rgbBlue;
		if (r == b && r == g)
			return TRUE;
	}
	return FALSE;
}