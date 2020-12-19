// CoordinateConverter.cpp
#include "CoordinateConverter.h"
#include <opencv2/core/mat.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/calib3d.hpp>
#include <iostream>

using namespace coordinates;

cv::Mat toCVMat(double_mat& fltMat) {
	cv::Mat cvMat(fltMat.size(), fltMat.at(0).size(), CV_64F);
	for (int i = 0; i < cvMat.rows; ++i)
		for (int j = 0; j < cvMat.cols; ++j)
			cvMat.at<double>(i, j) = fltMat.at(i).at(j);
	return cvMat;
}

double_mat toDoubleMat(cv::Mat cvMat) {
	double_mat mat(cvMat.rows);
	for (int i = 0; i < cvMat.rows; ++i)
		for (int j = 0; j < cvMat.cols; ++j)
			mat.at(i).push_back(cvMat.at<double>(i, j));
	return mat;
}

Converter::Converter() {}

double_mat Converter::getTransform(double_mat& src, double_mat& dest) {
	cv::Mat srcMat = toCVMat(src);
	cv::Mat destMat = toCVMat(dest);
	cv::Mat transform(3,4,CV_64F);
	std::vector<uchar> inliers;

	if (srcMat.rows != destMat.rows) {
		throw ConversionException("Source and destination must have the same number of points (i.e., rows)");
	}

	if (srcMat.cols != 3 || destMat.cols != 3) {
		throw ConversionException("Both source and destination ust have 3 columns (the x, y, and z coordinates)");
	}

	int success = cv::estimateAffine3D(srcMat, destMat, transform, inliers);
	if (success != 1) {
		std::cout << "Could not get affine transformation for point sets" << std::endl;
	}

	return toDoubleMat(transform);
}
