#ifndef IMAGEPLANE_H
#define IMAGEPLANE_H

#include <Eigen/Dense>
#include <Eigen/Geometry>

#include "image.h"
//Data types
typedef Eigen::Matrix<float, 3, 1> Vec3;
typedef Eigen::Matrix<float, 2, 1> Vec2;

class ImagePlane
{
public:
	Vec3 llc = Vec3(-1, -1, -1);//lower left corner
	Vec3 urc = Vec3(1, 1, -1);//upper right corner

	void setImage(Image i) {
		image_ = i;
	}

	void setXCol(int x) {
		xCols_ = x;
	}

	void setYRow(int y) {
		yRows_ = y;
	}

	/*
	* Function for generating pixel position  
	*/
	inline Vec3 generatePixelPos(int i, int j, int dx, int dy) {
		float randNum = (float)rand() / RAND_MAX;
		int h_samples = 8;
		int v_samples = 8;
		
		float u = llc(0) + (urc(0) - llc(0)) * (j + ((float)(dx + randNum) / h_samples))/xCols_;
		float v = llc(1) + (urc(1) - llc(1)) * (i + ((float)(dy + randNum) / v_samples))/yRows_;
		return Vec3(u, v, -1);
	}

	inline Vec3 generatePixelPos(int i, int j, int dx, int dy, int rotVal) {
		float randNum = (float)rand() / RAND_MAX;
		int h_samples = 8;
		int v_samples = 8;

		float u = llc(0) + (urc(0) - llc(0)) * (j + ((float)(dx + randNum) / h_samples)) / xCols_;
		float v = llc(1) +  rotVal - (urc(1) - llc(1)) * (i + ((float)(dy + randNum) / v_samples)) / yRows_;
		return Vec3(u, v, -1);
	}

	inline Vec3 generatePixelPos(int i, int j) {
		float u = llc(0) + (urc(0) - llc(0)) * (j + 0.5) / xCols_;
		float v = llc(1) + (urc(1) - llc(1)) * (i + 0.5) / yRows_;
		return Vec3(u, v, -1);
	}
private:
	Image image_;
	int xCols_;
	int yRows_;
};
#endif