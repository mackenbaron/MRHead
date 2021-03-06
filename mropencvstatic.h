//自动识别OpenCV版本并添加依赖库
//把opencv安装路径C or D:\opencv\build\include加入系统path
//然后把本文件拷贝至C or D:\opencv\build\include就可以了
//使用时只需#include "mropencv.h"
#ifndef _MROPENCV_H_
#define _MROPENCV_H_
#include <stdio.h>
#include <tchar.h>
#include <iostream>
#include "opencv2/core/version.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/opencv.hpp"
#include <opencv2/nonfree/nonfree.hpp>
using namespace cv;
using namespace std;
#define CV_VERSION_ID       CVAUX_STR(CV_MAJOR_VERSION) CVAUX_STR(CV_MINOR_VERSION) CVAUX_STR(CV_SUBMINOR_VERSION)
 
#ifdef _DEBUG
#define cvLIB(name) "opencv_" name CV_VERSION_ID "d"
#pragma comment( lib, "vfw32.lib")
#pragma comment( lib, "comctl32.lib")
#pragma comment( lib, "libjasperd.lib")
#pragma comment( lib, "libjpegd.lib")
#pragma comment( lib, "libpngd.lib")
#pragma comment( lib, "libtiffd.lib")
#pragma comment( lib, "IlmImfd.lib")
#pragma comment( lib, "zlibd.lib")
#else
#define cvLIB(name) "opencv_" name CV_VERSION_ID
#pragma comment( lib, "vfw32.lib")
#pragma comment( lib, "comctl32.lib")
#pragma comment( lib, "libjasper.lib")
#pragma comment( lib, "libjpeg.lib")
#pragma comment( lib, "libpng.lib")
#pragma comment( lib, "libtiff.lib")
#pragma comment( lib, "IlmImf.lib")
#pragma comment( lib, "zlib.lib")
#endif
#pragma comment( lib, cvLIB("core") )
#pragma comment( lib, cvLIB("imgproc") )
#pragma comment( lib, cvLIB("highgui") )
#pragma comment( lib, cvLIB("flann") )
#pragma comment( lib, cvLIB("features2d") )
#pragma comment( lib, cvLIB("calib3d") )
#pragma comment( lib, cvLIB("ml") )
#pragma comment( lib, cvLIB("contrib") )
#pragma comment( lib, cvLIB("legacy") )
#pragma comment( lib, cvLIB("objdetect") )
#pragma comment( lib, cvLIB("video") )
#pragma comment( lib, cvLIB("nonfree") )
#endif

// 自动识别OpenCV版本并添加依赖库
// 这里给大家介绍一种在头文件中自动判断识别OpenCV的版本号，并据此添加对应的依赖库（.lib文件）的方法。
// 这里首先是通过OpenCV的version.hpp文件获取版本号，然后将CV_MAJOR_VERSION、CV_MINOR_VERSION、CV_SUBMINOR_VERSION三个版本号数字串接成一个字符串CV_VERSION_ID。然后，根据OpenCV的lib文件的命名规则和当前的编译环境（debug或release）来定义一个命名lib文件的宏 cvLIB。最后，通过 #pragma comment ( lib, * ) 指令来添加所需的依赖库。
//  
// 上面这种方法的便捷性在于，我们只需要指定依赖库的名称，通过所定义的宏就可以自动生成和添加与你所安装的OpenCV版本相对应的依赖库；并且将来升级OpenCV时，除非新版本的依赖库命名规则发生改变，否则就不需要再改动代码，程序会自动判断OpenCV的版本并找到对应的依赖库。
//  
// 这种方法是参考自 wxWidgets 的，更多的技巧可以参见 wxWidgets\include\msvc\wx\setup.h 和 wxWidgets\include\wx\version.h 。
//  
// P.S.：另外，建议在系统环境变量中设置一个专门的项来存储OpenCV的安装路径，比如命名为 OPENCVDIR，然后在Visual Studio的项目属性VC++目录中设置include目录和library目录为 $(OPENCVDIR)/include和$(OPENCVDIR)/lib。这样以后升级OpenCV时，只需在系统环境变量中修改OPENCVDIR的路径，就不需要修改项目代码和编译器设置了。

