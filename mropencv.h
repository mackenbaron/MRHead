#ifndef _MROPENCV_H_
#define _MROPENCV_H_
#include <stdio.h>
#include <iostream>
#include "opencv2/core/version.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/opencv.hpp"
#ifndef NOT_USE_OPENCV_NAMESPACE
	using namespace cv;
#endif
#ifdef _WIN32
	#include <tchar.h>
	#define CV_VERSION_ID       CVAUX_STR(CV_MAJOR_VERSION) CVAUX_STR(CV_MINOR_VERSION) CVAUX_STR(CV_SUBMINOR_VERSION)
	#include "tickmeter.hpp"
	#define MR_LOG_TIME(Func) \
	TickMeter tm;\
	tm.start();\
	Func;\
	tm.stop();\
	cout<<#Func<<" cost " << tm.getTimeMilli() << " ms" << endl;\

	#ifdef OPENCV_STATIC
		#if _DEBUG
			#pragma comment( lib,"vfw32.lib")
			#pragma comment( lib,"comctl32.lib")
			#pragma comment( lib,"zlibd.lib")
			#pragma comment( lib,"libjasperd.lib")
			#pragma comment( lib,"libjpegd.lib")
			#pragma comment( lib,"libpngd.lib")
			#pragma comment( lib,"libtiffd.lib")
			#pragma comment( lib,"IlmImfd.lib")
		#else
			#pragma comment( lib,"vfw32.lib")
			#pragma comment( lib,"comctl32.lib")
			#pragma comment( lib,"zlib.lib")
			#pragma comment( lib,"libjasper.lib")
			#pragma comment( lib,"libjpeg.lib")
			#pragma comment( lib,"libpng.lib")
			#pragma comment( lib,"libtiff.lib")
			#pragma comment( lib,"IlmImf.lib")
		#endif
	#endif
	#ifdef _DEBUG
		#define cvLIB(name) "opencv_" name CV_VERSION_ID "d"
	#else
		#define cvLIB(name) "opencv_" name CV_VERSION_ID
	#endif
	#if CV_MAJOR_VERSION == 2
		#include <opencv2/nonfree/nonfree.hpp>
		#pragma comment( lib, cvLIB("core"))
		#pragma comment( lib, cvLIB("imgproc"))
		#pragma comment( lib, cvLIB("highgui"))
		#if CV_USE_ALL_MODULES
			#pragma comment( lib, cvLIB("flann"))
			#pragma comment( lib, cvLIB("features2d"))
			#pragma comment( lib, cvLIB("calib3d"))
			#pragma comment( lib, cvLIB("ml"))
			#pragma comment( lib, cvLIB("contrib"))
			#pragma comment( lib, cvLIB("legacy"))
			#pragma comment( lib, cvLIB("objdetect"))
			#pragma comment( lib, cvLIB("video"))
			#pragma comment( lib, cvLIB("nonfree"))
		#endif
	#else
	#if CV_MAJOR_VERSION == 3&&CV_MINOR_VERSION ==0
		#ifdef OPENCV_STATIC
			#pragma comment( lib, cvLIB("core"))
			#pragma comment( lib, cvLIB("imgproc"))
			#pragma comment( lib, cvLIB("highgui"))
			#pragma comment( lib, cvLIB("videoio"))
			#pragma comment( lib, cvLIB("imgproc"))
			#pragma comment( lib, cvLIB("imgcodecs"))
			#pragma comment( lib, cvLIB("hal"))
			#pragma comment( lib, "ippicvmt.lib")
			#pragma comment( lib, "libwebp.lib")
		#else
			#pragma comment( lib, cvLIB("world"))
			#pragma comment( lib, cvLIB("ts"))
		#endif
	#else
		#if CV_MAJOR_VERSION == 3&&CV_MINOR_VERSION ==1
			#pragma comment( lib, cvLIB("core"))
			#pragma comment( lib, cvLIB("videoio"))
			#pragma comment( lib, cvLIB("highgui"))
			#pragma comment( lib, cvLIB("imgproc"))
			#pragma comment( lib, cvLIB("imgcodecs"))
			#if CV_USE_ALL_MODULES		
				#pragma comment( lib, cvLIB("dnn"))
				#pragma comment( lib, cvLIB("flann"))
				#pragma comment( lib, cvLIB("features2d"))
				#pragma comment( lib, cvLIB("calib3d"))
				#pragma comment( lib, cvLIB("ml"))
				#pragma comment( lib, cvLIB("objdetect"))
				#pragma comment( lib, cvLIB("video"))
				#pragma comment( lib, cvLIB("tracking"))
				#pragma comment( lib, cvLIB("dpm"))
			#endif
		#else
			#if CV_MAJOR_VERSION == 3&&CV_MINOR_VERSION ==2&&_MSC_VER==1900
				#pragma comment( lib, cvLIB("world") )
			#else
				#pragma comment( lib, cvLIB("dnn"))
			#endif
		#endif
	#endif
	#endif
#endif
#endif
