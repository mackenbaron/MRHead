#ifndef _MRDLIB_H_
#define _MRDLIB_H_

#include <iostream>
using namespace std;

#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing/render_face_detections.h>
#include <dlib/image_processing.h>
#include <dlib/gui_widgets.h>
#include <dlib/image_io.h>
#include <dlib/opencv.h>

#ifndef NOT_USE_DLIB_NAMESPACE
using namespace dlib;
#endif

#ifdef _WIN32
	#if _DEBUG
		#pragma comment(lib,"dlibd.lib")
	#else
		#pragma comment(lib,"dlib.lib")
	#endif
#endif

#ifndef NOTUSE_SINGLETON_DLIBCLASS

#define DLIB_LANDMARK 1
#define DLIB_LOG_TIME 0

class CDlibDetector
{
public:
	void init(const string shapemodelpath = "shape_predictor_68_face_landmarks.dat")
	{
		detector = dlib::get_frontal_face_detector();
		dlib::deserialize(shapemodelpath) >> sp;
		inited = true;
	}
	static CDlibDetector* getInstance()
	{
		static CDlibDetector instance;
		return &instance;
	}
	int detectandmarkFace(cv::Mat &frame)
	{
		if (!inited)
		{
			init();
		}
		dlib::cv_image<dlib::bgr_pixel> img(frame);
#if DLIB_LOG_TIME
		cv::TickMeter tm;
		tm.start();
#endif
		std::vector<dlib::rectangle> dets = detector(img);
#if DLIB_LOG_TIME
		tm.stop();
#endif
		std::vector<dlib::full_object_detection> shapes;
		for (unsigned long j = 0; j < dets.size(); ++j)
		{
			cv::rectangle(frame, cv::Rect(dets[j].left(), dets[j].top(), dets[j].width(), dets[j].height()), cv::Scalar(0, 255, 0));
#if DLIB_LANDMARK		
#if DLIB_LOG_TIME
			tm.start();
#endif
			dlib::full_object_detection shape = sp(img, dets[j]);
#if DLIB_LOG_TIME
			tm.stop();
#endif
			shapes.push_back(shape);
			for (int i = 0; i < shape.num_parts(); i++)
			{
				cv::Point pt = cv::Point(shape.part(i).x(), shape.part(i).y());
				cv::circle(frame, cv::Point(shape.part(i).x(), shape.part(i).y()), 1, cv::Scalar(0, 0, 255));
			}
#endif
		}
#if DLIB_LOG_TIME
		cout << "cost" << tm.getTimeMilli() << "ms" << endl;
#endif
		cout << "Number of faces detected: " << dets.size() << endl;
		cv::imshow("result", frame);
		cv::waitKey(1);
		return 0;
	}
private:
	CDlibDetector(){
	}
	bool inited = false;
	dlib::shape_predictor sp;
	dlib::frontal_face_detector detector;
};
#endif

#endif