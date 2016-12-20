// Martin Kersner, m.kersner@gmail.com
// 2016/12/18
// C++ version of http://www.pyimagesearch.com/2015/02/16/faster-non-maximum-suppression-python/

#include "nms.hpp"

template <typename T>
void DV(const std::vector<T> vec) {

	for (T tmp : vec) {
		std::cout << tmp << std::endl;
	}
}

std::vector<cv::Rect> nms(std::vector<std::vector<float> > boxes, float threshold) {
	if (boxes.empty()) { 
		return std::vector<cv::Rect>();
	}

	// grab the coordinates of the bounding boxes
	std::vector<float> x1 = GetPointFromRect(boxes, XMIN);
	std::vector<float> y1 = GetPointFromRect(boxes, YMIN);
	std::vector<float> x2 = GetPointFromRect(boxes, XMAX);
	std::vector<float> y2 = GetPointFromRect(boxes, YMAX);

	// compute the area of the bounding boxes and sort the bounding
	// boxes by the bottom-right y-coordinate of the bounding box
	std::vector<float> area = ComputeArea(x1, y1, x2, y2);
	std::vector<int> idxs   = argsort(y2);

	int last, i;
	std::vector<int> pick;

	// keep looping while some indexes still remain in the indexes list
	while (idxs.size() > 0) {
		// grab the last index in the indexes list and add the
		// index value to the list of picked indexes
		last = idxs.size() - 1;	
		i    = idxs[last];
		pick.push_back(i);

		// find the largest (x, y) coordinates for the start of
		// the bounding box and the smallest (x, y) coordinates
		// for the end of the bounding box
		std::vector<int> idxsWoLast = RemoveLast(idxs);
		std::vector<float> xx1 = Maximum(x1[i], AccessVectorWithIdx(x1, idxsWoLast));
		std::vector<float> yy1 = Maximum(y1[i], AccessVectorWithIdx(y1, idxsWoLast));
		std::vector<float> xx2 = Minimum(x2[i], AccessVectorWithIdx(x2, idxsWoLast));
		std::vector<float> yy2 = Maximum(y2[i], AccessVectorWithIdx(y2, idxsWoLast));

		// compute the width and height of the bounding box
		std::vector<float> w = MaxSubtract(xx2, xx1, 0);
		std::vector<float> h = MaxSubtract(yy2, yy1, 0);
		
		// compute the ratio of overlap
		std::vector<float> overlap = MultiplyDivide(w, h, AccessVectorWithIdx(area, idxsWoLast));

		// delete all indexes from the index list that have
		std::vector<int> deleteIdxs = WhereLarger(overlap, threshold);
		deleteIdxs.push_back(last);
		idxs = RemoveByIndexes(idxs, deleteIdxs);
	}

	return BoxesToRectangles(FilterVector(boxes, pick));
}

std::vector<float> GetPointFromRect(std::vector<std::vector<float> > rect, PointInRectangle pos) {
	std::vector<float> points;

	for (std::vector<float> p: rect)  {
		points.push_back(p[pos]);
	}

	return points;
}

//area = (x2 - x1 + 1) * (y2 - y1 + 1)
std::vector<float> ComputeArea(std::vector<float> x1,
															 std::vector<float> y1,
															 std::vector<float> x2,
															 std::vector<float> y2) {

	std::vector<float> area;
	int len = x1.size();

	for (size_t idx = 0; idx < len; ++idx) {
		float tmpArea = (x2[idx] - x1[idx] + 1) * (y2[idx] - y1[idx] + 1);
		area.push_back(tmpArea);
	}
	
	return area;
}

template <typename T>
std::vector<int> argsort(const std::vector<T> &v) {

  // initialize original index locations
  std::vector<int> idx(v.size());
  std::iota(idx.begin(), idx.end(), 0);

  // sort indexes based on comparing values in v
  sort(idx.begin(), idx.end(),
       [&v](int i1, int i2) {return v[i1] < v[i2];});

  return idx;
}

//xx1 = np.maximum(x1[i], x1[idxs[:last]])
std::vector<float> Maximum(float num, std::vector<float> vec) {
	std::vector<float> maxVec= vec;
	int len = vec.size();

	for (std::size_t idx = 0; idx < len; ++idx) {
		if (vec[idx] < num) {
			maxVec[idx] = num;
		}
	}

	return maxVec;
}

std::vector<float> Minimum(float num, std::vector<float> vec) {
	std::vector<float> minVec= vec;
	int len = vec.size();

	for (std::size_t idx = 0; idx < len; ++idx) {
		if (vec[idx] > num) {
			minVec[idx] = num;
		}
	}

	return minVec;
}

std::vector<float> AccessVectorWithIdx(std::vector<float> vec, std::vector<int> idxs) {
	std::vector<float> resultVec;

	for (int idx : idxs) {
		resultVec.push_back(vec[idx]);
}

	return resultVec;
}

std::vector<int> RemoveLast(std::vector<int> vec) {
	vec.erase(vec.end()-1);
	return vec;
}

//w = np.maximum(0, xx2 - xx1 + 1)
std::vector<float> MaxSubtract(std::vector<float> vec1, std::vector<float> vec2, int minValue) {
	std::vector<float> sizeVec;

	float tmpSize;
	int len = vec1.size();

	for (size_t idx = 0; idx < len; ++idx) {
		tmpSize = vec1[idx] - vec2[idx] + 1;
		if (tmpSize < minValue) { tmpSize = minValue; }
		sizeVec.push_back(tmpSize);
	}

	return sizeVec;
}

std::vector<float> MultiplyDivide(std::vector<float> vec1,
		                              std::vector<float> vec2,
		                              std::vector<float> vec3) {

	std::vector<float> resultVec;
	int len = vec1.size();

	for (size_t idx = 0; idx < len; ++idx) {
		resultVec.push_back(vec1[idx] * vec2[idx] / vec3[idx]);
	}

	return resultVec;
}

std::vector<int> WhereLarger(std::vector<float> vec, float threshold) {
	std::vector<int> resultVec;
	int len = vec.size();

	for (size_t idx = 0; idx < len; ++idx) {
		if (vec[idx] > threshold) {
			resultVec.push_back(idx);
		}
	}

	return resultVec;
}

std::vector<int> RemoveByIndexes(std::vector<int> vec, std::vector<int> idxs) {
	std::vector<int> resultVec = vec;
	int offset = 0;

	for (int idx : idxs) {
		resultVec.erase(resultVec.begin() + idx + offset);
		offset -= 1;
	}

	return resultVec;
}

std::vector<cv::Rect> BoxesToRectangles(std::vector<std::vector<float> > boxes) {
	std::vector<cv::Rect> rectangles;
	std::vector<float> box;

	for (std::vector<float> box: boxes) {
		rectangles.push_back(cv::Rect(cv::Point(box[0], box[1]), cv::Point(box[2], box[3])));
	}

	return rectangles;
}

template <typename T>
std::vector<T> FilterVector(const std::vector<T> vec, const std::vector<int> idxs) {
	std::vector<T> resultVec;

	for (int idx: idxs) {
		resultVec.push_back(vec[idx]);
	}

	return resultVec;
}
