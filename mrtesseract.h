#ifndef MRTESSERACT_H
#define MRTESSERACT_H
#include "allheaders.h"
#include "baseapi.h"
#include "string"
#ifdef _WIN32
#if _DEBUG
#pragma comment(lib,"libtesseract304d.lib")
#pragma comment(lib,"liblept171d.lib")
#else
#pragma comment(lib,"libtesseract304.lib")
#pragma comment(lib,"liblept171.lib")
#endif
#endif
#define TESS_WITH_OPENCV 1
#if TESS_WITH_OPENCV
#include "opencv2/opencv.hpp"
#endif
#ifdef USE_ENG_DATA
	const char* langname = "eng";
#else
	#define TESSERACT_PASSPORT 1
	#if TESSERACT_PASSPORT
		const char* langname = "chi_sim";
	#endif
#endif

class MRTessMNumRecognier
{
public:
	static MRTessMNumRecognier *getInstance()
	{
		static MRTessMNumRecognier instance;
		return &instance;
	}
#if TESS_WITH_OPENCV
	std::string recog(const cv::Mat &img)
	{
		if (!img.data)
		{
			std::cout << "cannot load image" << std::endl;
			return "";
		}
		api.SetImage((uchar*)img.data, img.cols, img.rows, img.channels(), img.step);
		char*ocrresult = api.GetUTF8Text();
		std::string str = ocrresult;
		delete[]ocrresult;
		return str;
	}
#endif
	tesseract::TessBaseAPI  api;
private:
	MRTessMNumRecognier(){
		api.SetPageSegMode(tesseract::PSM_SINGLE_CHAR);
		api.Init("", "eng");
		api.SetVariable("tessedit_char_whitelist", "0123456789");
	}
	~MRTessMNumRecognier()
	{
		api.Clear();
		api.End();
	}
};

class MRTessChineseRecognier{
public:
	static MRTessChineseRecognier *getInstance()
	{
		static MRTessChineseRecognier instance;
		return &instance;
	}
#if TESS_WITH_OPENCV
	std::string recog(const cv::Mat &img)
	{
		if (!img.data)
		{
			std::cout << "cannot load image" << std::endl;
			return "";
		}
		api.SetImage((uchar*)img.data, img.cols, img.rows, img.channels(), img.step);
		char*ocrresult = api.GetUTF8Text();
		std::string str = ocrresult;
		delete[]ocrresult;
		return str;
	}
#endif
	tesseract::TessBaseAPI  api;
private:
	MRTessChineseRecognier(){
		api.SetPageSegMode(tesseract::PSM_SINGLE_CHAR);
		api.Init("", "chi_sim");
	}
	~MRTessChineseRecognier()
	{
		api.Clear();
		api.End();
	}
};

class MRTesseract
{
public:
	static MRTesseract *getInstance()
	{
		static MRTesseract instance;
		return &instance;
	}
	std::string recog(std::string filepath)
	{
		Pix*  pixs = pixRead(filepath.c_str());
		if (pixs == NULL)
		{
			std::cout << "cannot load " << filepath << std::endl;
			return "";
		}
		api.SetImage(pixs);
		std::string ret = api.GetUTF8Text();
		pixDestroy(&pixs);
		return ret;
	}
#if TESS_WITH_OPENCV
	std::string recog(const cv::Mat &img)
	{
		if (!img.data)
		{
			std::cout << "cannot load image" <<std::endl;
			return "";
		}
		api.SetImage((uchar*)img.data, img.cols, img.rows, img.channels(), img.step);
		char*ocrresult=api.GetUTF8Text();
		std::string str = ocrresult;
		delete[]ocrresult;
		return str;
	}
#endif
private:
	MRTesseract()
	{
#ifdef USE_ENG_DATA
		api.Init("", "eng");
		api.SetVariable("tessedit_char_whitelist", "0123456789");
#else
#if TESSERACT_PASSPORT
		api.Init("", langname);
#else	
		api.SetVariable("tessedit_char_whitelist", "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ<");
#endif
		api.SetPageSegMode(tesseract::PSM_SINGLE_CHAR);
#endif
	}
	tesseract::TessBaseAPI  api;
	~MRTesseract()
	{
		api.Clear();
		api.End();
	}
};
#endif
