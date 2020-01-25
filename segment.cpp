#include "stdafx.h"
#include "segment.h"

Segment::Segment()
{
	color = cv::Scalar(255, 255, 255);

	m00 = 0;
	m01 = 0;
	m10 = 0;
	m02 = 0;
	m20 = 0;
	m03 = 0;
	m30 = 0;
	m11 = 0;
	m21 = 0;
	m12 = 0;

	center_i = 0;
	center_j = 0;

	M01 = 0;
	M10 = 0;
	M02 = 0;
	M20 = 0;
	M03 = 0;
	M30 = 0;
	M11 = 0;
	M21 = 0;
	M12 = 0;

	M1 = 0;
	M2 = 0;
	M3 = 0;
	M4 = 0;
	M7 = 0;
}

Segment::Segment(cv::Scalar kolor)
{
	color = kolor;
	m00 = 0; // pole
	m01 = 0;
	m10 = 0;
	m02 = 0;
	m20 = 0;
	m03 = 0;
	m30 = 0;
	m11 = 0;
	m21 = 0;
	m12 = 0;

	center_i = 0;
	center_j = 0;

	M01 = 0;
	M10 = 0;
	M02 = 0;
	M20 = 0;
	M03 = 0;
	M30 = 0;
	M11 = 0;
	M21 = 0;
	M12 = 0;

	M1 = 0;
	M2 = 0;
	M3 = 0;
	M4 = 0;
	M7 = 0;
}

bool Segment::operator==(const Segment& s)
{
	return (color.val[0] == s.color.val[0] && color.val[1] == s.color.val[1] && color.val[2] == s.color.val[2]);
}

double Segment::Oblicz_moment(int p, int q) {
	double moment = 0;
	for (std::list<cv::Point2i>::iterator it = points.begin(); it != points.end(); ++it) {
		moment += pow(it->y + 1, p) * pow(it->x + 1, q);
	}
	return moment;
}

void Segment::Licz_niezmienniki() {
	m01 = Oblicz_moment(0, 1);
	m10 = Oblicz_moment(1, 0);
	m02 = Oblicz_moment(0, 2);
	m20 = Oblicz_moment(2, 0);
	m03 = Oblicz_moment(0, 3);
	m30 = Oblicz_moment(3, 0);
	m11 = Oblicz_moment(1, 1);
	m21 = Oblicz_moment(2, 1);
	m12 = Oblicz_moment(1, 2);

	center_i = m10 / m00;
	center_j = m01 / m00;

	M01 = m01 - (m01 / m00) * m00;
	M10 = m10 - (m10 / m00) * m00;
	M11 = m11 - m10 * m01 / m00;
	M20 = m20 - m10 * m10 / m00;
	M02 = m02 - m01 * m01 / m00;
	M21 = m21 - 2 * m11*center_i - m20 * center_j + 2 * m01*center_i*center_i;
	M12 = m12 - 2 * m11*center_j - m02 * center_i + 2 * m10*center_j*center_j;
	M30 = m30 - 3 * m20*center_i + 2 * m10*center_i*center_i;
	M03 = m03 - 3 * m02*center_j + 2 * m01*center_j*center_j;

	M1 = (M20 + M02) / pow(m00, 2);
	M2 = ((M20 - M02)*(M20 - M02) + 4 * pow(M11,2)) / pow(m00, 4);
	M3 = ((M30 - 3 * M12)*(M30 - 3 * M12) + (3 * M21 - M03)*(3 * M21 - M03)) / pow(m00, 5);
	M4 = (pow((M30 + M12), 2) + pow((M21 + M03), 2)) / pow(m00, 5);
	M7 = (M20*M02 - M11 * M11) / pow(m00, 4);
}
