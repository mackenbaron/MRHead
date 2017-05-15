#ifndef _MROPENCVUTIL_H_
#define _MROPENCVUTIL_H_
#include "opencv2/opencv.hpp"
#include "atlimage.h"
static int Mat2CImage(cv::Mat *mat, CImage &img) {
	if (!mat || mat->empty())
		return -1;
	int nBPP = mat->channels() * 8;
	img.Create(mat->cols, mat->rows, nBPP);
	if (nBPP == 8)
	{
		static RGBQUAD pRGB[256];
		for (int i = 0; i < 256; i++)
			pRGB[i].rgbBlue = pRGB[i].rgbGreen = pRGB[i].rgbRed = i;
		img.SetColorTable(0, 256, pRGB);
	}
	uchar* psrc = mat->data;
	uchar* pdst = (uchar*)img.GetBits();
	int imgPitch = img.GetPitch();
	for (int y = 0; y < mat->rows; y++)
	{
		memcpy(pdst, psrc, mat->cols*mat->channels());//mat->step is incorrect for those images created by roi (sub-images!)  
		psrc += mat->step;
		pdst += imgPitch;
	}

	return 0;
}

static void DrawMat2Wnd(const HWND &hWnd, cv::Mat &img, CRect *Roi = NULL)
{
	CWnd*pWnd=CWnd::FromHandle(hWnd);
	CImage imgDst;
	Mat2CImage(&img, imgDst);
	CRect drect;
	if (Roi == NULL)
		pWnd->GetClientRect(drect);
	else
		drect = *Roi;
	imgDst.Draw(pWnd->GetDC()->GetSafeHdc(), drect);
}

//功能：把Mat绘制到pWnd所代表的窗体上，使用方法如下所示:
//DrawMatToWnd(GetDlgItem(IDC_PIC), img);
static void DrawMat2Wnd(CWnd* pWnd, cv::Mat &img, CRect *Roi = NULL)
{
	CImage imgDst;
	Mat2CImage(&img,imgDst);
	CRect drect;
	if (Roi == NULL)
		pWnd->GetClientRect(drect);
	else
		drect = *Roi;
	imgDst.Draw(pWnd->GetDC()->GetSafeHdc(), drect);
}

static void BitMat2Wnd(CWnd* wnd, cv::Mat &img, CRect *Roi=NULL)
{
	if (img.empty())
		return;
	CDC *cdc = wnd->GetDC();
	CDC MemDC;//首先定义一个显示设备对象
	CBitmap MemBitmap;//定义一个位图对象
	CRect rect, drect;

	wnd->GetClientRect(rect);
	Gdiplus::Bitmap bitmap(img.cols, img.rows, img.cols * img.channels(), PixelFormat24bppRGB, (BYTE*)img.data);//根据Mat矩阵创建一个GDI+中的Bitmap位图

	if (Roi == NULL)
		drect = rect;
	else
		drect = *Roi;
	//随后建立与屏幕显示兼容的内存显示设备
	MemDC.CreateCompatibleDC(cdc);
	//下面建立一个与屏幕显示兼容的位图，至于位图的大小，可以用窗口的大小
	MemBitmap.CreateCompatibleBitmap(cdc, rect.Width(), rect.Height());

	//将位图选入到内存显示设备中
	//只有选入了位图的内存显示设备才有地方绘图，画到指定的位图上
	CBitmap *pOldBit = MemDC.SelectObject(&MemBitmap);

	//先用背景色将位图清除干净，可以用自己应该用的颜色
	MemDC.FillSolidRect(0, 0, rect.Width(), rect.Height(), RGB(255, 255, 255));

	//绘图
	Gdiplus::Graphics g(MemDC.m_hDC);
	Gdiplus::Image *ii = &bitmap;
	g.DrawImage(ii, Gdiplus::Rect(0, 0, drect.Width(), drect.Height()));
	g.ReleaseHDC(MemDC.m_hDC);

	//将内存中的图拷贝到屏幕上进行显示
	cdc->BitBlt(0, 0, drect.Width(), drect.Height(), &MemDC, 0, 0, SRCCOPY);
	//绘图完成后的清理
	MemBitmap.DeleteObject();
	MemDC.DeleteDC();
	//ReleaseDC(cdc);  这里需要做如下修改
	wnd->ReleaseDC(cdc);
}

static void DrawMat2DuiWnd(CWnd* pWnd, cv::Mat &img, CRect *Roi = NULL)
{
	if (img.empty())
		return;
	CRect drect;
	pWnd->GetClientRect(drect);
	CClientDC dc(pWnd);
	HDC hDC = dc.GetSafeHdc();
	BYTE *bitBuffer = NULL;
	BITMAPINFO *bitMapinfo = NULL;
	int ichannels = img.channels();
	if (ichannels == 1)
	{
		bitBuffer = new BYTE[40 + 4 * 256];
	}
	else if (ichannels == 3)
	{
		bitBuffer = new BYTE[sizeof(BITMAPINFO)];
	}
	else
	{
		return;
	}
	if (bitBuffer == NULL)
	{
		return;
	}
	bitMapinfo = (BITMAPINFO *)bitBuffer;
	bitMapinfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bitMapinfo->bmiHeader.biHeight = -img.rows; //如果高度为正的，位图的起始位置在左下角。如果高度为负，起始位置在左上角。 
	bitMapinfo->bmiHeader.biWidth = img.cols;
	bitMapinfo->bmiHeader.biPlanes = 1; // 目标设备的级别，必须为1 
	bitMapinfo->bmiHeader.biBitCount = ichannels * 8; // 每个像素所需的位数，必须是1(双色), 4(16色)，8(256色)或24(真彩色)之一 
	bitMapinfo->bmiHeader.biCompression = BI_RGB; //位图压缩类型，必须是 0(不压缩), 1(BI_RLE8压缩类型)或2(BI_RLE4压缩类型)之一 
	bitMapinfo->bmiHeader.biSizeImage = 0; // 位图的大小，以字节为单位 
	bitMapinfo->bmiHeader.biXPelsPerMeter = 0; // 位图水平分辨率，每米像素数 
	bitMapinfo->bmiHeader.biYPelsPerMeter = 0; // 位图垂直分辨率，每米像素数 
	bitMapinfo->bmiHeader.biClrUsed = 0; // 位图实际使用的颜色表中的颜色数 
	bitMapinfo->bmiHeader.biClrImportant = 0; // 位图显示过程中重要的颜色数
	if (ichannels == 1)
	{
		for (int i = 0; i < 256; i++)
		{ //颜色的取值范围 (0-255) 
			bitMapinfo->bmiColors[i].rgbBlue = bitMapinfo->bmiColors[i].rgbGreen = bitMapinfo->bmiColors[i].rgbRed = (BYTE)i;
		}

		bitMapinfo->bmiHeader.biClrUsed = 256; // 位图实际使用的颜色表中的颜色数 
	}
	SetStretchBltMode(hDC, COLORONCOLOR);
	StretchDIBits(hDC,
		0,
		0,
		drect.right, //显示窗口宽度 
		drect.bottom, //显示窗口高度 
		0,
		0,
		img.cols, //图像宽度 
		img.rows, //图像高度 
		img.data,
		bitMapinfo,
		DIB_RGB_COLORS,
		SRCCOPY
		);
	delete[]bitBuffer;
}
#endif