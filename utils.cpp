// Martin Kersner, m.kersner@gmail.com
// 2016/12/19 

#include "utils.hpp"

cv::Rect VecToRect(std::vector<float> vec) {
	return cv::Rect(cv::Point(vec[0], vec[1]), cv::Point(vec[2], vec[3]));
}

void DrawRectangles(cv::Mat& img, std::vector<std::vector<float> > vecVecFloat) {
	for (std::vector<float> vec: vecVecFloat) {
		cv::rectangle(img, VecToRect(vec),  WHITE_COLOR);
	}
}

void DrawRectangles(cv::Mat& img, std::vector<cv::Rect> vecRect) {
	for (cv::Rect rect: vecRect) {
		cv::rectangle(img, rect,  WHITE_COLOR);
	}
}
