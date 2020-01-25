#pragma once

struct piksel {
	int red;
	int green;
	int blue;
};

cv::Mat Filtr_RankingowyRGB(cv::Mat &Img);

cv::Mat Binaryzacja(cv::Mat im, int warunek);

cv::Mat Erozja(cv::Mat Img);

cv::Mat Dylatacja(cv::Mat Img);

cv::Mat Otwarcie(cv::Mat Img);

void Gornoprzepustowy(cv::Mat &im);