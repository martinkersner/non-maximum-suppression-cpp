// Martin Kersner, m.kersner@gmail.com
// 2016/12/19 

#ifndef UTILS_HPP__
#define UTILS_HPP__ 

#include <vector>
#include <opencv2/opencv.hpp>

const cv::Scalar WHITE_COLOR = cv::Scalar(255, 255, 255);

cv::Rect VecToRect(const std::vector<float> &);

void DrawRectangles(cv::Mat &,
                    const std::vector<std::vector<float>> &);

void DrawRectangles(cv::Mat &,
                    const std::vector<cv::Rect> &);

#endif // UTILS_HPP__ 
