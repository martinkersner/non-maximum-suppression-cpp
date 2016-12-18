// Martin Kersner, m.kersner@gmail.com
// 2016/12/18

#include <opencv2/opencv.hpp>
#include <vector>

#include "nms.hpp"

int main() {
	cv::Mat imgBefore(cv::Size(600,600), cv::DataType<float>::type);
	cv::Mat imgAfter = imgBefore.clone();

	// initialization
	std::vector<float> rect1 = {300, 300, 400, 400};
	std::vector<float> rect2 = {320, 320, 420, 420};
	std::vector<float> rect3 = {295, 259, 415, 415};

	std::vector<float> rect4 = {100, 100, 150, 150};
	std::vector<float> rect5 = {90, 90, 180, 180};
	std::vector<float> rect6 = {112, 112, 170, 170};
	float threshold	= 0.5;

	// before
	std::vector<cv::Rect> rectAll;
	rectAll.push_back(VecToRect(rect1));
	rectAll.push_back(VecToRect(rect2));
	rectAll.push_back(VecToRect(rect3));
	rectAll.push_back(VecToRect(rect4));
	rectAll.push_back(VecToRect(rect5));
	rectAll.push_back(VecToRect(rect6));

	for (cv::Rect tmpRect : rectAll) {
		cv::rectangle(imgBefore, tmpRect, cv::Scalar(255, 255, 255));
	}
	cv::imshow("Before", imgBefore);

	// after
	std::vector<std::vector<float> > rectForNms;
	rectForNms.push_back(rect1);
	rectForNms.push_back(rect2);
	rectForNms.push_back(rect3);
	rectForNms.push_back(rect4);
	rectForNms.push_back(rect5);
	rectForNms.push_back(rect6);

	std::vector<cv::Rect> rectNms = nms(rectForNms, threshold);

	for (cv::Rect tmpRect : rectNms) {
		cv::rectangle(imgAfter, tmpRect, cv::Scalar(255, 255, 255));
	}
	cv::imshow("After", imgAfter);

	cv::waitKey(0);
}
