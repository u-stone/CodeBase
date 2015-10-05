// ScreenCapture.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include <Windows.h>
#include <stdio.h>
#include "clrconvert.h"

#define LIB_EXPORT

#include "capture.h"




//每次截取的图片都保存到本地一份
//#define SAVEFILE  //要慎重，开了之后很容易爆盘得！！！

struct Capture 
{
	int                x, y;
	int                nWndWidth;
	int                nWndHeight;
	HWND               hWnd;
	HDC                hWndDC;
	HDC                hMemDC;
	HBITMAP            hCaptureBitmap;
	BOOL               bReAlloc;
	HGLOBAL            hBuffer;
	void*              pData;
	LPBITMAPINFOHEADER pBmpData;
	HGLOBAL            hForeBuffer;
	void*              pForeData;
	LPBITMAPINFOHEADER pForeBmpData;
	BITMAPFILEHEADER   bf;
	BITMAPINFOHEADER   bi;
	int                len;
	BITMAP             bitmap;
};

int abs(int v){
	return v>0 ?v : -v;
}

LIB_API void* init()
{
	Capture* pC = new Capture;
	memset(pC, 0, sizeof(Capture));

	pC->nWndWidth = GetSystemMetrics(SM_CXSCREEN);
	pC->nWndHeight = GetSystemMetrics(SM_CYSCREEN);

	pC->hWnd = GetDesktopWindow();
	pC->hWndDC = GetDC(pC->hWnd);
	pC->hMemDC = CreateCompatibleDC(pC->hWndDC);
	pC->hCaptureBitmap = CreateCompatibleBitmap(pC->hWndDC, pC->nWndWidth, pC->nWndHeight);
	int size = pC->nWndWidth * pC->nWndHeight;
	SelectObject(pC->hMemDC, pC->hCaptureBitmap);

	return pC;
}

LIB_API LONG set(LIB_IN void* pVoid, LIB_IN int x, LIB_IN int y, LIB_IN int cx, LIB_IN int cy)
{
	Capture* pC = (Capture*)pVoid;
	pC->x = x;
	pC->y = y;
	if (cx != pC->nWndWidth || cy != pC->nWndHeight)
		pC->bReAlloc = TRUE;
	pC->nWndWidth = cx;
	pC->nWndHeight = cy;

	if (pC->bReAlloc){
		HBITMAP hBitmap = CreateCompatibleBitmap(pC->hWndDC, cx, cy);
		SelectObject(pC->hMemDC, hBitmap);
		DeleteObject(pC->hCaptureBitmap);
		pC->hCaptureBitmap = hBitmap;

		if (pC->hBuffer){
			GlobalUnlock(pC->hBuffer);
			GlobalFree(pC->hBuffer);
		}
		pC->hBuffer = 0;
		if (pC->hForeBuffer){
			GlobalUnlock(pC->hForeBuffer);
			GlobalFree(pC->hForeBuffer);
		}
		pC->hForeBuffer = 0;
	}
	return 0;
}

BOOL convertData(void* pDst, void* pSrc, int nStepByte, int nDeep){
	char* pBottom = (char*)pSrc + nStepByte * (nDeep - 1), *pTop = (char*)pDst;
	for (int i = 0; i < nDeep; ++i){
		memcpy(pTop, pBottom, nStepByte);
		pTop += nStepByte;
		pBottom -= nStepByte;
	}
// 	int size = nDeep * nStepByte >> 2;
// 	int *pDec = (int*)pDst;
// 	for (int i = 0; i < size; ++i, ++pDec){
// 		*pDec &= 0x00FFFFFF;
// 	}
	return TRUE;
}


LIB_API LONG getScreen(LIB_IN void* pVoid, LIB_OUT void** pScreenData, LIB_OUT int* pLen, LIB_OUT int width)
{
#ifdef SAVEFILE
 	static int count = 0;
 	count++;
 	TCHAR filename[40];
 	memset(filename, 0, sizeof(filename));
 	swprintf_s(filename, sizeof(filename)/sizeof(TCHAR), L"E:\\tmp\\bmp%05d.bmp", count);
#endif

	Capture* pC = (Capture*)pVoid;
	BitBlt(pC->hMemDC, 0, 0, pC->nWndWidth, pC->nWndHeight, pC->hWndDC, pC->x, pC->y, SRCCOPY);
	
	HDC hDC = CreateDC(L"DISPLAY", 0, 0, 0);
	int iBits = GetDeviceCaps(hDC, BITSPIXEL) * GetDeviceCaps(hDC, PLANES);
	DeleteDC(hDC);
	WORD wBitCount = 0;
	if      (iBits <= 1)     wBitCount = 1;
	else if (iBits <= 4 )    wBitCount = 4;
	else if (iBits <= 8 )    wBitCount = 8;
	else if (iBits <= 24)    wBitCount = 24;
	else                     wBitCount = 32;

	DWORD dwPaletteSize = 0;
	if (wBitCount <= 8)
		dwPaletteSize = (1 << wBitCount) * sizeof(RGBQUAD);
	
	GetObject(pC->hCaptureBitmap, sizeof(BITMAP), &(pC->bitmap));
	
	pC->bi.biSize          = sizeof(BITMAPINFOHEADER);
	pC->bi.biWidth         = pC->bitmap.bmWidth;
	pC->bi.biHeight        = pC->bitmap.bmHeight;
	pC->bi.biPlanes        = 1;
	pC->bi.biBitCount      = wBitCount;
	pC->bi.biCompression   = BI_RGB;
	pC->bi.biSizeImage     = 0;
	pC->bi.biXPelsPerMeter = 0;
	pC->bi.biYPelsPerMeter = 0;
	pC->bi.biClrUsed       = 0;
	pC->bi.biClrImportant  = 0;

	int nStepByte = (((pC->bitmap.bmWidth * wBitCount + 31) & ~31) >> 3);
	pC->len = nStepByte * pC->bitmap.bmHeight;

	if (pC->hBuffer == 0){
		pC->hBuffer = ::GlobalAlloc(GHND, sizeof(BITMAPFILEHEADER) + dwPaletteSize + pC->len);
		pC->pBmpData = (LPBITMAPINFOHEADER)GlobalLock(pC->hBuffer);
		*(pC->pBmpData) = pC->bi;
	} 
	if (pC->hForeBuffer == 0){
		pC->hForeBuffer = ::GlobalAlloc(GHND, sizeof(BITMAPFILEHEADER) + dwPaletteSize + pC->len);
		pC->pForeBmpData = (LPBITMAPINFOHEADER)GlobalLock(pC->hForeBuffer);
	}
	
	HPALETTE hOldPal, hPal = (HPALETTE)GetStockObject(DEFAULT_PALETTE);
	HDC hDesktopDC = 0;
	if (hPal){
		hDesktopDC = ::GetDC(NULL);
		hOldPal = SelectPalette(hDesktopDC, hPal, FALSE);
		RealizePalette(hDesktopDC);
	}

	int ret = GetDIBits(hDesktopDC, pC->hCaptureBitmap, 0, (UINT)pC->bitmap.bmHeight, 
		     (LPSTR)(pC->pBmpData) + sizeof(BITMAPINFOHEADER) + dwPaletteSize, 
			 (BITMAPINFO*)pC->pBmpData, DIB_RGB_COLORS);
	if (hOldPal){
		SelectPalette(hDesktopDC, hOldPal, TRUE);
		RealizePalette(hDesktopDC);
		::ReleaseDC(NULL, hDesktopDC);
	}
	pC->pData = (LPSTR)(pC->pBmpData) + sizeof(BITMAPINFOHEADER) + dwPaletteSize;
	pC->pForeData = (LPSTR)(pC->pForeBmpData) + sizeof(BITMAPINFOHEADER) + dwPaletteSize;

	if (convertData(pC->pForeData, pC->pData, nStepByte, pC->nWndHeight))
		*pScreenData = pC->pForeData;
	else
		return FALSE;


#ifdef SAVEFILE
 	HANDLE hf = CreateFile(filename, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
 	if (hf == INVALID_HANDLE_VALUE){
 		return FALSE;
 	}
 	pC->bf.bfType = 0x4D42; //"BM"
 	int dwDIBSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) * dwPaletteSize + pC->len;
 	pC->bf.bfSize = dwDIBSize;
 	pC->bf.bfReserved1 = 0;
 	pC->bf.bfReserved2 = 0;
 	pC->bf.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER) + dwPaletteSize;
 	DWORD dwWritten = 0;
 	WriteFile(hf, &pC->bf, sizeof(BITMAPFILEHEADER), &dwWritten, NULL);
 	WriteFile(hf, pC->pBmpData, sizeof(BITMAPINFOHEADER) + dwPaletteSize, &dwWritten, NULL);
	WriteFile(hf, pC->pForeBmpData, pC->len, &dwWritten, NULL);
 	CloseHandle(hf);
#endif

	*pLen = pC->len;
	return TRUE;
}

LIB_API LONG uninit(LIB_IN void* pVoid)
{
	Capture* pC = (Capture*)pVoid;
	if (pC->hBuffer){
		GlobalUnlock(pC->hBuffer);
		GlobalFree(pC->hBuffer);
	}
	ReleaseDC(pC->hWnd, pC->hWndDC);
	DeleteDC(pC->hMemDC);
	DeleteObject(pC->hCaptureBitmap);
	delete pC;
	return 0;
}


