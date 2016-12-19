// Martin Kersner, m.kersner@gmail.com
// 2016/12/18

#include <vector>
#include <opencv2/opencv.hpp>

#include "utils.hpp"
#include "nms.hpp"

int main() {
	// initialization
	cv::Mat imgBefore(cv::Size(600,600), cv::DataType<float>::type);
	cv::Mat imgAfter = imgBefore.clone();
	float threshold	= 0.5;
	std::vector<std::vector<float> > rectangles =
		{
			{300, 300, 400, 400},
			{320, 320, 420, 420},
			{295, 259, 415, 415},
			{100, 100, 150, 150},
			{90,  90,  180, 180},
			{112, 112, 170, 170}
		};

	// before
	DrawRectangles(imgBefore, rectangles);
	cv::imshow("Before", imgBefore);

	// after
	std::vector<cv::Rect> reducedRectangle = nms(rectangles, threshold);
	DrawRectangles(imgAfter, reducedRectangle);
	cv::imshow("After", imgAfter);

	cv::waitKey(0);
}
