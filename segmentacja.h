#pragma once
#include "opencv2/core/core.hpp"//opencv_core249d.lib;opencv_highgui249d.lib;
#include "opencv2/highgui/highgui.hpp"
#include <opencv2/opencv.hpp>
#include"stdafx.h"
#include "segment.h"

class Segmentacja
{
public:
	std::list<Segment> segments;
	Segmentacja(cv::Mat image, cv::Mat image1);
	void Segmentuj();
	void Analizuj();
	void Rozpoznanie();

private:
	cv::Mat im_wczytane;
	cv::Mat im_otwarte;
	cv::Mat im_przetworz;

	void floodFill(int row, int col, cv::Scalar color);
	cv::Scalar randomColor(cv::RNG& random);
	void Usun_obiekt();

	bool czyElement1(const Segment& );
	bool czyElement2(const Segment& );
	bool czyElement3(const Segment& );
};