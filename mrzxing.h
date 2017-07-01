#ifndef MRZXING_H
#define MRZXING_H
#include "opencv2/opencv.hpp"
#include "zxing/ZXing.h"
#include <zxing/ReaderException.h>
#include <zxing/common/GlobalHistogramBinarizer.h>
#include <zxing/qrcode/QRCodeReader.h>
#include <zxing/MatSource.h>
#include <zxing/ChecksumException.h>
using namespace zxing;
using namespace zxing::qrcode;
#if _WIN32
	#if _DEBUG
		#pragma comment(lib,"libzxingd.lib")
	#else
		#pragma comment(lib,"libzxing.lib")
	#endif
#endif

class MRzxing
{
public:
	cv::Point toCvPoint(Ref<ResultPoint> resultPoint) {
		return cv::Point(resultPoint->getX(), resultPoint->getY());
	}
	static MRzxing*getInstance()
	{
		MRzxing instance;
		return &instance;
	}
	int recog(const cv::Mat&image, std::string &strQR,const int blocksize = 15, const int C = 2);
	cv::Mat debug(const cv::Mat&image);
};

int MRzxing::recog(const cv::Mat&image, std::string &strQR, const int blocksize, const int C)
{
	if (!image.data)
		return -1;
	cv::Mat grey;
	cv::cvtColor(image, grey, CV_BGR2GRAY);
	cv::Mat bin;
	cv::adaptiveThreshold(grey, bin, 255, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY, blocksize, C);
	try {
		Ref<LuminanceSource> source = MatSource::create(bin);
		Ref<Reader> reader;
		reader.reset(new QRCodeReader);
		Ref<Binarizer> binarizer(new GlobalHistogramBinarizer(source));
		Ref<BinaryBitmap> bitmap(new BinaryBitmap(binarizer));
		Ref<Result> result(reader->decode(bitmap, DecodeHints(DecodeHints::TRYHARDER_HINT)));
		strQR= result->getText()->getText();
		return 0;
	}
	catch (const ChecksumException &e)
	{
		strQR= "bad location";
		return -2;
	}
	catch (const std::exception& e) {
		strQR= e.what();
		return -3;
	}
	return -1;
}
cv::Mat MRzxing::debug(const cv::Mat&image)
{
	cv::Mat grey;
	cv::cvtColor(image, grey, CV_BGR2GRAY);
	try {

		// Create luminance  source
		Ref<LuminanceSource> source = MatSource::create(grey);

		// Search for QR code
		Ref<Reader> reader;
		reader.reset(new QRCodeReader);
		Ref<Binarizer> binarizer(new GlobalHistogramBinarizer(source));
		Ref<BinaryBitmap> bitmap(new BinaryBitmap(binarizer));
		Ref<Result> result(reader->decode(bitmap, DecodeHints(DecodeHints::TRYHARDER_HINT)));
		cv::Mat r = image.clone();
		int resultPointCount = result->getResultPoints()->size();

		for (int j = 0; j < resultPointCount; j++) {
			cv::circle(r, toCvPoint(result->getResultPoints()[j]), 10, cv::Scalar(110, 220, 0), 2);
		}
		if (resultPointCount > 1) {

			for (int j = 0; j < resultPointCount; j++) {
				Ref<ResultPoint> previousResultPoint = (j > 0) ? result->getResultPoints()[j - 1] : result->getResultPoints()[resultPointCount - 1];
				line(r, toCvPoint(previousResultPoint), toCvPoint(result->getResultPoints()[j]), cv::Scalar(110, 220, 0), 2, 8);
				previousResultPoint = result->getResultPoints()[j];
			}
		}

		if (resultPointCount > 0) {
			putText(r, result->getText()->getText(), toCvPoint(result->getResultPoints()[0]), cv::FONT_HERSHEY_PLAIN, 1, cv::Scalar(110, 220, 0));
		}
		return r;
	}
	catch (const ReaderException& e) {
		std::cerr << e.what() << " (ignoring)" << std::endl;
	}
	catch (const zxing::IllegalArgumentException& e) {
		std::cerr << e.what() << " (ignoring)" << std::endl;
	}
	catch (const zxing::Exception& e) {
		std::cerr << e.what() << " (ignoring)" << std::endl;
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << " (ignoring)" << std::endl;
	}
}
#endif