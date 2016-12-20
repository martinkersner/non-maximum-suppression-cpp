// Martin Kersner, m.kersner@gmail.com
// 2016/12/18

#ifndef NMS_HPP__
#define NMS_HPP__

#include <vector>
#include <numeric>
#include <opencv2/opencv.hpp>

enum PointInRectangle {XMIN, YMIN, XMAX, YMAX};

std::vector<cv::Rect> nms(std::vector<std::vector<float> >, float);

std::vector<float> GetPointFromRect(std::vector<std::vector<float> >, PointInRectangle);

std::vector<float> ComputeArea(std::vector<float>,
															 std::vector<float>,
															 std::vector<float>,
															 std::vector<float>);

template <typename T>
std::vector<int> argsort(const std::vector<T> &v);

std::vector<float> Maximum(float, std::vector<float>);

std::vector<float> Minimum(float, std::vector<float>);

std::vector<float> AccessVectorWithIdx(std::vector<float>, std::vector<int>);

std::vector<int> RemoveLast(std::vector<int>);

std::vector<float> MaxSubtract(std::vector<float>, std::vector<float>, int);

std::vector<float> MultiplyDivide(std::vector<float> vec1,
		                             std::vector<float> vec2,
		                             std::vector<float> vec3);

std::vector<int> WhereLarger(std::vector<float>, float);

std::vector<int> RemoveDuplicates(std::vector<int>, std::vector<int>);

std::vector<cv::Rect> BoxesToRectangles(std::vector<std::vector<float> >);

template <typename T>
std::vector<T> FilterVector(const std::vector<T>, const std::vector<int>);

#endif // NMS_HPP__
