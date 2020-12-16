#include "stdafx.h"
#include<math.h>
#include<complex>
using namespace std;

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
	int w= lpBitsInfo->bmiHeader. biWidth;
	int h = lpBitsInfo->bmiHeader.biHeight ;
	int LineBytes = (w * lpBitsInfo->bmiHeader. biBitCount + 31)/32* 4;
	BYTE* lpBits = (BYTE*)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];
	
	int LineBytes_gray = (w* 8 + 31)/32* 4;
	LONG size = 40 +1024 + LineBytes_gray * h;
	
	LPBITMAPINFO lpBitsInfo_gray = (LPBITMAPINFO) malloc(size);
	
	memcpy(lpBitsInfo_gray,lpBitsInfo,40);
	lpBitsInfo_gray->bmiHeader. biBitCount = 8;
	lpBitsInfo_gray->bmiHeader.biClrUsed = 256;
	
	int i, j;
	for (i = 0 ; i <256; i ++)
	{
		lpBitsInfo_gray->bmiColors[i].rgbRed = i ;
		lpBitsInfo_gray->bmiColors[i].rgbGreen = i ;
		lpBitsInfo_gray->bmiColors[i].rgbBlue = i ;
		lpBitsInfo_gray->bmiColors[i].rgbReserved = 0;
	}

	BYTE* lpBits_gray = (BYTE*)&lpBitsInfo_gray->bmiColors[256];
	BYTE *R,*G,*B,avg,*pixel;
	for (i = 0; i < h; i ++)
	{
		for (j = 0; j < w; j ++)
		{
			B = lpBits + LineBytes * (h - 1 - i) + j * 3;
			G = B + 1;
			R = G + 1;

			avg = (*R + *G + *B)/3;
			
			pixel = lpBits_gray + LineBytes_gray * (h - 1 - i) + j;
			*pixel = avg;

		}
	}
	free (lpBitsInfo);
	lpBitsInfo = lpBitsInfo_gray ;

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

//histogram equalization
void Equalize()
{
	int w = lpBitsInfo->bmiHeader.biWidth;
	int h = lpBitsInfo->bmiHeader.biHeight;
	int LineBytes = 
		(w * lpBitsInfo->bmiHeader.biBitCount + 31)/32* 4;
	BYTE* lpBits = (BYTE*)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];
	
	int i ,j;
	BYTE* pixel;
	BYTE Map[256];
	
	Histogram();
	DWORD temp;
	for (i = 0; i < 256; i++)
	{
		temp = 0;
		for (j = 0; j <= i; j ++)
		{
			temp += H[j];
		}
		Map[i] = (BYTE)(temp * 255/ (w * h) + 0.5);
	}
	
	for (i = 0; i < h; i ++)
	{
		for (j = 0; j < w;j ++)
		{
			pixel  = lpBits + LineBytes * (h - 1 - i) + j ;
			*pixel = Map[*pixel];
		}
	}
}

#define PI 3.1415926535
void FT(complex<double>* TD, complex<double>* FD, int m)
{
	int u,x;
	double angle;
	for (u = 0; u< m; u ++)
	{
		FD[u] = 0;
		for (x = 0; x < m; x ++)
		{
			angle = -2*PI * u* x/m;
			FD[u] += 
				TD[x]* complex<double>(cos(angle), sin(angle)) ;
		}
		FD[u]/= m;
	}
}

void IFT(complex<double>* FD, complex<double>* TD, int m)
{
	int u,x;
	double angle;
	for (x = 0; x < m; x ++)
	{
		TD[x] = 0;
		for (u = 0; u< m; u ++)
		{
			angle = 2*PI*u*x /m;
			TD[x] +=
				FD[u] * complex<double>(cos(angle), sin(angle)) ;
		}
	}
}

complex<double>* gFD = NULL;
void Fourier ()
{
	int w =lpBitsInfo->bmiHeader. biWidth;
	int h = lpBitsInfo->bmiHeader.biHeight ;
	int LineBytes = 
		(w * lpBitsInfo->bmiHeader.biBitCount + 31)/32* 4;
	BYTE* lpBits = 
		(BYTE*)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];

	complex<double>*TD = new complex<double>[w * h];
	complex<double>* FD = new complex<double>[w * h];

	int i,j;
	BYTE* pixel;
	for (i = 0; i < h; i ++)
	{
		for (j = 0; j < w; j ++)
		{
			pixel = lpBits + LineBytes * (h- 1 - i) + j ;
			TD[w * i + j] = complex<double>(*pixel * pow (-1,i+j), 0);
		}
	}
	
	for (i = 0; i < h; i ++)
		FT(&TD[w * i],&FD[w * i], w);

	for (i = 0; i < h; i ++)
		for (j = 0; j < w; j ++)
			TD[h*j +i]=FD[w * i + j];

	for ( j = 0; j < w; j ++)
		FT(&TD[j * h], &FD[j * h], h);

	LONG size = 40 +1024 + LineBytes * h;
	LPBITMAPINFO lpDIB_FT =(LPBITMAPINFO) malloc(size);
	memcpy (lpDIB_FT,lpBitsInfo, size);

	lpBits = (BYTE*)&lpDIB_FT->bmiColors[256];
	
	double temp;
	for (i= 0; i < h; i ++)
	{
		for (j = 0; j < w; j ++)
		{
			pixel = lpBits + LineBytes * (h - 1 - i) + j;
			temp = 
				sqrt(FD[j * h + i].real() * FD[ j * h + i].real() + FD[j * h + i].imag() * FD[j *h + i].imag()) * 2000;
			if (temp > 255)
				temp = 255;
			*pixel = (BYTE) (temp);
		}
	}
	delete TD;
//	delete FD;

	gFD = FD;

	free (lpBitsInfo) ;
	lpBitsInfo = lpDIB_FT;

}

BOOL gFD_isValid()
{
	return (gFD != NULL);
}

void IFourier()
{
	int w = lpBitsInfo->bmiHeader.biWidth;
	int h = lpBitsInfo->bmiHeader.biHeight ;
	int LineBytes = 
		(w * lpBitsInfo->bmiHeader.biBitCount + 31)/32* 4;
	BYTE* lpBits = 
		(BYTE*)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];

	complex<double>* TD = new complex<double>[w * h];
	complex<double>* FD = new complex<double>[w * h];

	int i ,j;
	BYTE* pixel ;
	for (i=0; i < h; i ++)
		for (j= 0; j < w; j ++)
			FD[w*i+j]=gFD[h * j + i];

	for (i = 0; i < h; i ++)
		IFT (&FD[w * i],&TD[w * i], w) ;
	
	for (i = 0; i < h; i ++)
		for (j = 0; j < w; j ++)
			FD[h* j +i] =TD[w * i + j];

	for (j = 0; j < w; j ++)
		IFT(&FD[j * h],&TD[j * h], h);

	LONG size = 40 + 1024 + LineBytes * h;

	LPBITMAPINFO lpDIB_IFT =(LPBITMAPINFO) malloc(size);
	memcpy (lpDIB_IFT, lpBitsInfo, size);

	lpBits = (BYTE*)&lpDIB_IFT->bmiColors[256];
	
	for (i = 0; i < h;i ++)
	{
		for (j = 0; j < w; j ++)
		{
			pixel = lpBits + LineBytes * (h -1- i) + j;
			*pixel = 
				(BYTE)(TD[j * h + i].real ()/pow (-1,i+j) ) ;
		}
	}

	delete TD;
	delete FD;
	//delete gFD;
	//gFD = NULL;

	free (lpBitsInfo) ;
	lpBitsInfo = lpDIB_IFT;
}