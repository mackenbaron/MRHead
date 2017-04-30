#include "mropencv.h"
#include "mrdir.h"
#include "mrutil.h"
#include "fstream"
using namespace std;
int main()
{
	cv::Mat img = cv::imread("../opencv.png");
	cv::namedWindow("img", 0);
	cv::imshow("img", img);
	cv::waitKey();
	return 0;
}