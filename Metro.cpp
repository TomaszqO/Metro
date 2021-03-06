// Metro.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
void Metro(cv::Mat img, int warunek);
cv::Mat Czerwo(cv::Mat& img);
int main()
{
	RECT desktop;
	GetWindowRect(GetDesktopWindow(), &desktop);
	std::cout<<desktop.top << std::endl;
	std::cout << desktop.left << std::endl;
	cv::Mat i_h;
	cv::Mat  img0 = cv::imread("car.bmp");
		cv::Mat img1 = cv::imread("Lena.png");
		cv::cvtColor(img1, i_h, CV_BGR2HSV);
		cv::Mat img2 = cv::imread("M2.jpg");
		cv::Mat img3 = cv::imread("M3.jpg");
		//Gornoprzepustowy(img1);
		//Metro(img1, 1);
		//Metro(img2, 2);
		//Metro(img3, 3);
		return 0;
}

void Metro(cv::Mat img, int warunek) {
	cv::Mat zbinaryzowany = Binaryzacja(img, warunek);
	cv::Mat otwarty = Otwarcie(zbinaryzowany);
	cv::imshow("po_otwarciu", otwarty);
	cv::waitKey(-1);
	Segmentacja seg1(otwarty, img);
	seg1.Segmentuj();
	seg1.Analizuj();
	seg1.Rozpoznanie();
	cv::waitKey(-1);
}
cv::Mat Czerwo(cv::Mat& img) {
	int temp = 0;
	cv::Mat ten(img.size(), CV_8UC3);
	cv::Mat_<cv::Vec3b> I = img;
	cv::Mat_<cv::Vec3b> T = ten;
	for (int i = 0; i < img.rows; i++) {
		for (int j = 0; j < img.cols; j++) {

			if (I(i, j)[2] < 150 || I(i,j)[1]>120) {
				temp = (I(i, j)[0] + I(i, j)[1] + I(i, j)[2]) / 3;
				T(i, j)[0] = temp;
				T(i, j)[1] = temp;
				T(i, j)[2] = temp;
			}
			else {
				T(i, j)[0] = I(i, j)[0];
				T(i, j)[1] = I(i, j)[1];
				T(i, j)[2] = I(i, j)[2];
			}

		
		}
	}
	cv::imshow("czerwo", T);
	cv::waitKey();
	return T;
}