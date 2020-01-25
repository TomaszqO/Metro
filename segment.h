#pragma once
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <list>

class Segment {
public:
	std::list<cv::Point2i> points;
	cv::Scalar color;

	//momenty zwykle rzedu (i,j)
	double m00;
	double m01;
	double m10;
	double m02;
	double m20;
	double m03;
	double m30;
	double m11;
	double m21;
	double m12;

	double center_i;
	double center_j;

	//momenty centralne
	double M01;
	double M10;
	double M02;
	double M20;
	double M03;
	double M30;
	double M11;
	double M21;
	double M12;

	double M1;
	double M2;
	double M3;
	double M4;
	double M7;

	Segment();
	Segment(cv::Scalar kolor);
	bool operator==(const Segment& q);
	void Licz_niezmienniki();

private:
	double Oblicz_moment(int p, int q);
};