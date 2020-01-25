#include "stdafx.h"

bool pair_less(const std::pair<int, piksel> lhs, const std::pair<int, piksel> rhs) {
	return lhs.first < rhs.first;
}

cv::Mat Filtr_RankingowyRGB(cv::Mat &Img) {
	std::cout << "podaj promien maski...\n";
	int r = 0;
	int index = 0;
	while (!(std::cin >> r)) {
		std::cin.clear();
		while (std::cin.get() != '\n')
			continue;
		std::cout << "podaj poprawny promien maski...\n";
	}
	if (r % 2 == 0)
		r = r + 1;
	std::cout << "Wybrany promien to: " << r << ".Wybierz indeks od 0 do " << r * r - 1 << std::endl;
	std::cin >> index;
	index < 0 ? index = 0 : index>r*r - 1 ? index = r * r - 1 : index = index;


	cv::Mat Temp(Img.rows, Img.cols, CV_8UC3);
	cv::Mat_<cv::Vec3b> I = Img;
	cv::Mat_<cv::Vec3b> T = Temp;

	piksel p;
	for (int i = r - 1; i < Img.rows - r + 1; i++) {
		for (int j = r - 1; j < Img.cols - r + 1; j++) {
			std::vector<std::pair<int, piksel>>ramka;
			std::vector<int>ramka_red;
			std::vector<int>ramka_blue;
			std::vector<int>ramka_green;
			for (int i1 = 0; i1 < 2 * r - 1; i1++) {
				for (int j1 = 0; j1 < 2 * r - 1; j1++) {
					//ramka.push_back(I[i1][j1] = (I(i + i1 - r - 1, j + j1 - r - 1)[1] + I(i + i1 - r - 1, j + j1 - r - 1)[2] + I(i + i1 - r - 1, j + j1 - r - 1)[3])) / 3;
					p.red = I(i + i1 - r + 1, j + j1 - r + 1)[2];
					p.green = I(i + i1 - r + 1, j + j1 - r + 1)[1];
					p.blue = I(i + i1 - r + 1, j + j1 - r + 1)[0];
					ramka.push_back(std::make_pair((p.red + p.green + p.blue) / 3, p));
					ramka_red.push_back(I(i + i1 - r + 1, j + j1 - r + 1)[2]);
					ramka_green.push_back(I(i + i1 - r + 1, j + j1 - r + 1)[1]);
					ramka_blue.push_back(I(i + i1 - r + 1, j + j1 - r + 1)[0]);
				}
			}
			std::sort(ramka.begin(), ramka.end(), pair_less);
			T(i, j)[1] = ramka[index].second.green;
			T(i, j)[0] = ramka[index].second.blue;
			T(i, j)[2] = ramka[index].second.red;
		}
	}
	Img = T;
	cv::imshow("po_rankingowym", Img);
	cv::waitKey(-1);
	return Img;
}

cv::Mat Binaryzacja(cv::Mat im, int warunek) {
	cv::Mat im1 = im.clone();
	cv::Mat_<cv::Vec3b> I = im1;
	if (warunek == 1) {
		for (int i = 0; i < I.rows; i++) {
			for (int j = 0; j < I.cols; j++) {
				if (I(i, j)[1] < 97 && I(i, j)[1]>38 && I(i, j)[2] > 150 && I(i, j)[2] < 210) {
					I(i, j)[0] = 255;
					I(i, j)[1] = 255;
					I(i, j)[2] = 255;
				}
				else {
					I(i, j)[0] = 0;
					I(i, j)[1] = 0;
					I(i, j)[2] = 0;
				}
				}
			}
		}
	
	else if (warunek ==2){
		for (int i = 0; i < I.rows; i++) {
			for (int j = 0; j < I.cols; j++) {
				if (I(i, j)[1] > 30 && I(i, j)[1] < 85 && I(i, j)[2] > 55 && I(i, j)[2] < 145 && I(i, j)[0]>22 && I(i, j)[0] < 85) { 
						I(i, j)[0] = 255;
						I(i, j)[1] = 255;
						I(i, j)[2] = 255;
					}
				else {
					I(i, j)[0] = 0;
					I(i, j)[1] = 0;
					I(i, j)[2] = 0;
						}
				}
			}
		}
	 else if (warunek == 3) {
		 for (int i = 0; i < I.rows; i++) {
			 for (int j = 0; j < I.cols; j++) {
				if (I(i, j)[1]<190 && I(i, j)[2]>175 && (I(i, j)[0]<100 || I(i, j)[0]>180)){
						I(i, j)[0] = 255;
						I(i, j)[1] = 255;
						I(i, j)[2] = 255;
					}
				else {
					I(i, j)[0] = 0;
					I(i, j)[1] = 0;
					I(i, j)[2] = 0;
				}
			 }
		 }
	 }
	return im1;
}

cv::Mat Erozja(cv::Mat Img) {
	cv::Mat ten(Img.size(), CV_8UC3);
	cv::Mat_<cv::Vec3b> I = Img;
	cv::Mat_<cv::Vec3b> T = ten;
	//image.at<cv::Vec3b>(row, col)[channel] = 255;
	for (int i = 1; i < I.rows - 1; i++) {
		for (int j = 1; j < I.cols - 1; j++) {

			if (I(i - 1, j - 1)[0] == 0 || I(i - 1, j)[0] == 0 || I(i - 1, j + 1)[0] == 0 ||
				I(i, j - 1)[0] == 0 || I(i, j)[0] == 0 || I(i, j + 1)[0] == 0 ||
				I(i + 1, j - 1)[0] == 0 || I(i + 1, j)[0] == 0 || I(i + 1, j + 1)[0] == 0) {
				T(i, j)[0] = 0;
				T(i, j)[1] = 0;
				T(i, j)[2] = 0;
			}
			else {
				T(i, j)[0] = 255;
				T(i, j)[1] = 255;
				T(i, j)[2] = 255;
			}

		}
	}
	return T;
}

cv::Mat Dylatacja(cv::Mat Img) {
	cv::Mat ten(Img.size(), CV_8UC3);
	cv::Mat_<cv::Vec3b> I = Img;
	cv::Mat_<cv::Vec3b> T = ten;
	//image.at<cv::Vec3b>(row, col)[channel] = 255;
	for (int i = 1; i < I.rows - 1; i++) {
		for (int j = 1; j < I.cols - 1; j++) {

			if (I(i - 1, j - 1)[0] == 255 || I(i - 1, j)[0] == 255 || I(i - 1, j + 1)[0] == 255 ||
				I(i, j - 1)[0] == 255 || I(i, j)[0] == 255 || I(i, j + 1)[0] == 255 ||
				I(i + 1, j - 1)[0] == 255 || I(i + 1, j)[0] == 255 || I(i + 1, j + 1)[0] == 255) {
				T(i, j)[0] = 255;
				T(i, j)[1] = 255;
				T(i, j)[2] = 255;
			}
			else {
				T(i, j)[0] = 0;
				T(i, j)[1] = 0;
				T(i, j)[2] = 0;
			}

		}
	}
	return T;
}

cv::Mat Otwarcie(cv::Mat Img) {
	return Dylatacja(Erozja(Img));
}

void Gornoprzepustowy(cv::Mat &im) {
	cv::Mat Temp(im.rows, im.cols, CV_8UC3);
	cv::Mat_<cv::Vec3b> I = im;
	cv::Mat_<cv::Vec3b> T = Temp;
	int maska[3][3] = { 0,-1,0,-1,5,-1, 0,-1,0 };
	for (int i = 1; i < im.rows - 1; i++) {
		for (int j = 1; j < im.cols - 1; j++) {
			T(i, j)[0] = I(i - 1, j - 1)[0] * maska[0][0] + I(i - 1, j)[0] * maska[0][1] + I(i + 1, j)[0] * maska[0][2] +
				I(i, j - 1)[0] * maska[1][0] + I(i, j)[0] * maska[1][1] + I(i, j + 1)[0] * maska[1][2] +
				I(i + 1, j - 1)[0] * maska[2][0] + I(i + 1, j)[0] * maska[2][1] + I(i + 1, j + 1)[0] * maska[2][2];

			T(i, j)[1] = I(i - 1, j - 1)[1] * maska[0][0] + I(i - 1, j)[1] * maska[0][1] + I(i + 1, j)[1] * maska[0][2] +
				I(i, j - 1)[1] * maska[1][0] + I(i, j)[1] * maska[1][1] + I(i, j + 1)[1] * maska[1][2] +
				I(i + 1, j - 1)[1] * maska[2][0] + I(i + 1, j)[1] * maska[2][1] + I(i + 1, j + 1)[1] * maska[2][2];

			T(i, j)[2] = I(i - 1, j - 1)[2] * maska[0][0] + I(i - 1, j)[2] * maska[0][1] + I(i + 1, j)[2] * maska[0][2] +
				I(i, j - 1)[2] * maska[1][0] + I(i, j)[2] * maska[1][1] + I(i, j + 1)[2] * maska[1][2] +
				I(i + 1, j - 1)[2] * maska[2][0] + I(i + 1, j)[2] * maska[2][1] + I(i + 1, j + 1)[2] * maska[2][2];

			if (T(i, j)[0] > 255)
				T(i, j)[0] = 255;
			else if (T(i, j)[0] < 0)
				T(i, j)[0] = 0;
			if (T(i, j)[1] > 255)
				T(i, j)[1] = 255;
			else if (T(i, j)[1] < 0)
				T(i, j)[1] = 0;
			if (T(i, j)[2] > 255)
				T(i, j)[2] = 255;
			else if (T(i, j)[2] < 0)
				T(i, j)[2] = 0;
		}
	}

	cv::imshow("gownoprzepustowy", im);
	cv::waitKey();
	cv::imshow("gownoprzepustowy", T);
	cv::waitKey();
}