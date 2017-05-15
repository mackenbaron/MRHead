#ifndef MRTESSERACT_H
#define MRTESSERACT_H
#include "allheaders.h"
#include "baseapi.h"
#include "string"
#ifdef WIN32
#pragma comment(lib,"libtesseract304.lib")
#pragma comment(lib,"liblept171.lib")
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
	const char* langname = "passport";
#endif
#endif

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
	std::string recog(cv::Mat img)
	{
		if (!img.data)
		{
			std::cout << "cannot load image" <<std::endl;
			return "";
		}
		api.SetImage((uchar*)img.data, img.cols, img.rows, img.channels(), img.step);
		return api.GetUTF8Text();
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
