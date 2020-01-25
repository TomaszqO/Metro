#include "stdafx.h"
#include"segmentacja.h"

Segmentacja::Segmentacja(cv::Mat image, cv::Mat image1) {
	im_wczytane = image1.clone();
	im_otwarte = image.clone();
	im_przetworz = cv::Mat(image.size(), CV_8UC3);
	image.copyTo(im_przetworz);
}

void Segmentacja::Segmentuj() {
	int n = 1;
	cv::Scalar color;
	for (int i = 0; i < im_przetworz.rows; ++i) {
		for (int j = 0; j < im_przetworz.cols; ++j) {
			if (im_przetworz.at<cv::Vec3b>(i, j)[0] == 255 && im_przetworz.at<cv::Vec3b>(i, j)[1] == 255 && im_przetworz.at<cv::Vec3b>(i, j)[2] == 255) {
				cv::RNG rng(n);
				color = randomColor(rng);
				segments.push_back(Segment(color));
				floodFill(i, j, color);
				n++;
			}
		}
	}
	Usun_obiekt();
	cv::imshow("posegmentowe", im_przetworz);
	cv::waitKey(-1);
}

void Segmentacja::floodFill(int row, int col, cv::Scalar color) {

	std::list<cv::Point2i> Kolejka;//utwórz kolejka
	Kolejka.push_back(cv::Point2i(col, row));// kolejka.wstaw(pozycja);
	cv::Point2i temp, left, right, top, bottom;

	while (!Kolejka.empty()) { //iloœæ_elementów(kolejka)>0
		temp = Kolejka.front();//kolejka.œci¹gnij_element();
		Kolejka.pop_front();

		im_przetworz.at<cv::Vec3b>(temp)[0] = (uchar)color[0];
		im_przetworz.at<cv::Vec3b>(temp)[1] = (uchar)color[1];
		im_przetworz.at<cv::Vec3b>(temp)[2] = (uchar)color[2];
		//dodanie punktu do segmentu
		segments.back().points.push_back(temp);

		left = cv::Point2i(temp.x - 1, temp.y);
		right = cv::Point2i(temp.x + 1, temp.y);
		top = cv::Point2i(temp.x, temp.y - 1);
		bottom = cv::Point2i(temp.x, temp.y + 1);

		// dla danego piksela sprawdz sasiadow lewy, prawy, gora, dol, czy jest bialy. 
		// jesli jest, a nie ma go w kolejce, to taki punkt wstawiaj do kolejki
		//kolejka dziala, az sie oprozni
		if (left.x >= 0 && im_przetworz.at<cv::Vec3b>(left)[0] == 255 &&
			im_przetworz.at<cv::Vec3b>(left)[1] == 255 && im_przetworz.at<cv::Vec3b>(left)[2] == 255 &&
			find(Kolejka.begin(), Kolejka.end(), left) == Kolejka.end()) {
			Kolejka.push_back(left);
		}
		if (right.x < im_przetworz.cols && im_przetworz.at<cv::Vec3b>(right)[0] == 255 && im_przetworz.at<cv::Vec3b>(right)[1] == 255 && im_przetworz.at<cv::Vec3b>(right)[2] == 255 &&
			find(Kolejka.begin(), Kolejka.end(), right) == Kolejka.end()) {
			Kolejka.push_back(right);
		}
		if (top.y >= 0 && im_przetworz.at<cv::Vec3b>(top)[0] == 255 && 
			im_przetworz.at<cv::Vec3b>(top)[1] == 255 && im_przetworz.at<cv::Vec3b>(top)[2] == 255 &&
			find(Kolejka.begin(), Kolejka.end(), top) == Kolejka.end()) {
			Kolejka.push_back(top);
		}
		if (bottom.y < im_przetworz.rows && im_przetworz.at<cv::Vec3b>(bottom)[0] == 255 && 
			im_przetworz.at<cv::Vec3b>(bottom)[1] == 255 && im_przetworz.at<cv::Vec3b>(bottom)[2] == 255 &&
			find(Kolejka.begin(), Kolejka.end(), bottom) == Kolejka.end()) {
			Kolejka.push_back(bottom);
		}
	}
}

cv::Scalar Segmentacja::randomColor(cv::RNG& rng) {
	int icolor = (unsigned)rng;
	return cv::Scalar(icolor & 255, (icolor >> 8) & 255, (icolor >> 16) & 255);
}

void Segmentacja::Usun_obiekt() {
	for (std::list<Segment>::iterator it = segments.begin(); it != segments.end(); ++it) {
		it->m00 = it->points.size(); // m00 = pole segmentu
		if (it->m00 < 125) { //jesli pole za male, 125 dla strzalki w obrazie z dwoma M
			it->color = cv::Scalar(0, 0, 0);  // czernienie obiektu
			for (std::list<cv::Point2i>::iterator i = it->points.begin(); i != it->points.end(); ++i) {
				im_przetworz.at<cv::Vec3b>(*i)[0] = 0;
				im_przetworz.at<cv::Vec3b>(*i)[1] = 0;
				im_przetworz.at<cv::Vec3b>(*i)[2] = 0;
			}
		}
	}
	segments.remove(Segment(cv::Scalar(0, 0, 0))); // usuniecie segmentow czarnych z Segment
}

void Segmentacja::Analizuj() {
	std::cout.setf(std::ios::fixed);
	std::cout << std::endl;
	for (std::list<Segment>::iterator it = segments.begin(); it != segments.end(); ++it)
		it->Licz_niezmienniki();
}

void Segmentacja::Rozpoznanie() {
	std::list<Segment> El1, El2, El3;

	for (std::list<Segment>::iterator it = segments.begin(); it != segments.end(); ++it) {
		if (czyElement1(*it))
			El1.push_back(*it);
		else if (czyElement2(*it))
			El2.push_back(*it);
		else if (czyElement3(*it))
			El3.push_back(*it);
		else {
			for (std::list<cv::Point2i>::iterator i = it->points.begin(); i != it->points.end(); ++i) {
				im_przetworz.at<cv::Vec3b>(*i)[0] = 0;
				im_przetworz.at<cv::Vec3b>(*i)[1] = 0;
				im_przetworz.at<cv::Vec3b>(*i)[2] = 0;
			}
		}
	}

	std::vector<cv::Point2i> minEl1, minEl2, minEl3;
	std::vector<cv::Point2i> maxEl1, maxEl2, maxEl3;
	int xMin, yMin, xMax, yMax;

	if (!El1.empty() && !El3.empty() && !El2.empty())
	{
		
		for (std::list<Segment>::iterator it = El1.begin(); it != El1.end(); ++it) { //kazdy segment
			yMax = 0; xMax = 0;
			xMin = im_wczytane.cols;
			yMin = im_wczytane.rows;
			for (std::list<cv::Point2i>::iterator i = it->points.begin(); i != it->points.end(); ++i) { //punkt po punkcie
				if (xMin > i->x)
					xMin = i->x;
				if (yMin > i->y)
					yMin = i->y;
				if (xMax < i->x)
					xMax = i->x;
				if (yMax < i->y)
					yMax = i->y;
			}
			minEl1.push_back(cv::Point2i(xMin, yMin));
			maxEl1.push_back(cv::Point2i(xMax, yMax));
		}
		for (std::list<Segment>::iterator it = El2.begin(); it != El2.end(); ++it) {//kazdy segment
			yMax = 0; xMax = 0;
			xMin = im_wczytane.cols;
			yMin = im_wczytane.rows;
			for (std::list<cv::Point2i>::iterator i = it->points.begin(); i != it->points.end(); ++i) {//punkt po punkcie
				if (xMin > i->x)
					xMin = i->x;
				if (yMin > i->y)
					yMin = i->y;
				if (xMax < i->x)
					xMax = i->x;
				if (yMax < i->y)
					yMax = i->y;
			}
			minEl2.push_back(cv::Point2i(xMin, yMin));
			maxEl2.push_back(cv::Point2i(xMax, yMax));
		}
		for (std::list<Segment>::iterator it = El3.begin(); it != El3.end(); ++it) {
			yMax = xMax = 0;
			xMin = im_wczytane.cols;
			yMin = im_wczytane.rows;
			for (std::list<cv::Point2i>::iterator i = it->points.begin(); i != it->points.end(); ++i) {
				if (xMin > i->x)
					xMin = i->x;
				if (yMin > i->y)
					yMin = i->y;
				if (xMax < i->x)
					xMax = i->x;
				if (yMax < i->y)
					yMax = i->y;
			}
			minEl3.push_back(cv::Point2i(xMin, yMin));
			maxEl3.push_back(cv::Point2i(xMax, yMax));
		}
		for (int i = 0; i<El1.size(); ++i) {
			//xmin
			if (minEl1[i].x<=minEl2[i].x && minEl1[i].x<=minEl3[i].x)
				xMin = minEl1[i].x;
			else if (minEl2[i].x<minEl1[i].x && minEl2[i].x<=minEl3[i].x)
				xMin = minEl2[i].x;
			else if (minEl3[i].x<minEl1[i].x && minEl3[i].x<minEl2[i].x)
				xMin = minEl3[i].x;
			//ymin
			if (minEl1[i].y <= minEl2[i].y && minEl1[i].y <= minEl3[i].y)
				yMin = minEl1[i].y;
			else if (minEl2[i].y<minEl1[i].y && minEl2[i].y <= minEl3[i].y)
				yMin = minEl2[i].y;
			else if (minEl3[i].y<minEl1[i].y && minEl3[i].y<minEl2[i].y)
				yMin = minEl3[i].y;
			//xmax
			if (maxEl1[i].x >= maxEl2[i].x && maxEl1[i].x >= maxEl3[i].x)
				xMax = maxEl1[i].x;
			else if (maxEl2[i].x>maxEl1[i].x && maxEl2[i].x >= maxEl3[i].x)
				xMax = maxEl2[i].x;
			else if (maxEl3[i].x>maxEl1[i].x && maxEl3[i].x>maxEl2[i].x)
				xMax = maxEl3[i].x;
			// ymax
			if (maxEl1[i].y >= maxEl2[i].y && maxEl1[i].y >= maxEl3[i].y)
				yMax = maxEl1[i].y;
			else if (maxEl2[i].y>maxEl1[i].y && maxEl2[i].y >= maxEl3[i].y)
				yMax = maxEl2[i].y;
			else if (maxEl3[i].y>maxEl1[i].y && maxEl3[i].y>maxEl2[i].y)
				yMax = maxEl3[i].y;

			rectangle(im_wczytane, cv::Point2i(xMin - 2, yMin - 2), cv::Point2i(xMax + 2, yMax + 2), cv::Scalar(0, 200, 150), +2, 4);
		}
	}

	cv::imshow("znalezione_logo", im_wczytane);
	cv::waitKey(-1);
}

// dobrane "recznie" wartosci wspolczynnikow
bool Segmentacja::czyElement1(const Segment& seg) {
	return (seg.M1 >= 0.32 && seg.M1 <= 0.365 && seg.M2 >= 0.07 && seg.M2 <= 0.11
		&& seg.M7 >= 0.0067 && seg.M7 <= 0.0071) ==1;
}

bool Segmentacja::czyElement2(const Segment& seg) {

	return (seg.M1 >= 0.269 && seg.M1 < 0.304 && seg.M2 >= 0.041 && seg.M2 < 0.063 &&
		 seg.M7 >= 0.0074 && seg.M7 < 0.009 ) ==1;
}

bool Segmentacja::czyElement3(const Segment& seg) {
	return (seg.M1 >= 0.21 && seg.M1 < 0.25 && seg.M2 >= 0.0014 && seg.M2 < 0.087 &&
		 seg.M7 >= 0.009 && seg.M7 < 0.012 && seg.M4>0.00023) ==1;
}
