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

	//# if the bounding boxes integers, convert them to floats --
	//# this is important since we'll be doing a bunch of divisions
	//if boxes.dtype.kind == "i":
		//boxes = boxes.astype("float")

	//# initialize the list of picked indexes	
	//pick = []

	//# grab the coordinates of the bounding boxes
	//x1 = boxes[:,0]
	//y1 = boxes[:,1]
	//x2 = boxes[:,2]
	//y2 = boxes[:,3]
	
	std::vector<float> x1 = GetPointFromRect(boxes, XMIN);
	std::vector<float> y1 = GetPointFromRect(boxes, YMIN);
	std::vector<float> x2 = GetPointFromRect(boxes, XMAX);
	std::vector<float> y2 = GetPointFromRect(boxes, YMAX);

	//# compute the area of the bounding boxes and sort the bounding
	//# boxes by the bottom-right y-coordinate of the bounding box
	//area = (x2 - x1 + 1) * (y2 - y1 + 1)
	//idxs = np.argsort(y2)
	std::vector<float> area = ComputeArea(x1, y1, x2, y2);
	std::vector<int> idxs = argsort(y2);

	int last, i;
	std::vector<int> pick;

	//# keep looping while some indexes still remain in the indexes
	//# list
	//while len(idxs) > 0:
	while (idxs.size() > 0) {
		//# grab the last index in the indexes list and add the
		//# index value to the list of picked indexes
		//last = len(idxs) - 1
		//i = idxs[last]
		//pick.append(i)
		last = idxs.size() - 1;	
		i = idxs[last];
		pick.insert(pick.end(), i);

		//# find the largest (x, y) coordinates for the start of
		//# the bounding box and the smallest (x, y) coordinates
		//# for the end of the bounding box
		//xx1 = np.maximum(x1[i], x1[idxs[:last]])
		//yy1 = np.maximum(y1[i], y1[idxs[:last]])
		//xx2 = np.minimum(x2[i], x2[idxs[:last]])
		//yy2 = np.minimum(y2[i], y2[idxs[:last]])
		std::vector<int> idxWoLast = RemoveLast(idxs);
		std::vector<float> xx1 = Maximum(x1[i], AccessVectorWithIdx(x1, idxWoLast));
		std::vector<float> yy1 = Maximum(y1[i], AccessVectorWithIdx(y1, idxWoLast));
		std::vector<float> xx2 = Minimum(x2[i], AccessVectorWithIdx(x2, idxWoLast));
		std::vector<float> yy2 = Maximum(y2[i], AccessVectorWithIdx(y2, idxWoLast));

		//# compute the width and height of the bounding box
		//w = np.maximum(0, xx2 - xx1 + 1)
		//h = np.maximum(0, yy2 - yy1 + 1)
		std::vector<float> w = MaxSubtract(xx2, xx1, 0);
		std::vector<float> h = MaxSubtract(yy2, yy1, 0);
		
		//# compute the ratio of overlap
		//overlap = (w * h) / area[idxs[:last]]
		std::vector<float> overlap = MultiplyDivide(w, h, AccessVectorWithIdx(area, idxWoLast));

		//# delete all indexes from the index list that have
		//idxs = np.delete(idxs, np.concatenate(([last], np.where(overlap > overlapThresh)[0])))
		std::vector<int> deleteIdxs = WhereLarger(overlap, threshold);
		deleteIdxs.push_back(last);
		idxs = RemoveDuplicates(idxs, deleteIdxs);
	}

	return MakeRects(boxes, pick);
}

std::vector<float> GetPointFromRect(std::vector<std::vector<float> > rect, PointInRectangle pos) {
	std::vector<float> points;

	for (std::vector<float> p: rect)  {
		points.insert(points.end(), p[pos]);
	}

	return points;
}

//area = (x2 - x1 + 1) * (y2 - y1 + 1)
std::vector<float> ComputeArea(std::vector<float> x1,
															 std::vector<float> y1,
															 std::vector<float> x2,
															 std::vector<float> y2) {

	std::vector<float> area;

	for (size_t idx = 0; idx < x1.size(); ++idx) {
		float tmp_area = (x2[idx] - x1[idx] + 1) * (y2[idx] - y1[idx] + 1);
		area.insert(area.end(), tmp_area);
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
	std::vector<float> max_vec= vec;

	for (std::size_t idx = 0; idx < vec.size(); ++idx) {
		if (vec[idx] < num) {
			max_vec[idx] = num;
		}
	}

	return max_vec;
}

std::vector<float> Minimum(float num, std::vector<float> vec) {
	std::vector<float> min_vec= vec;

	for (std::size_t idx = 0; idx < vec.size(); ++idx) {
		if (vec[idx] > num) {
			min_vec[idx] = num;
		}
	}

	return min_vec;
}

std::vector<float> AccessVectorWithIdx(std::vector<float> vec, std::vector<int> idxs) {
	std::vector<float> final_vec;

	for (int idx : idxs) {
		final_vec.insert(final_vec.end(), vec[idx]);
}

	return final_vec;
}

std::vector<int> RemoveLast(std::vector<int> vec) {
	vec.erase(vec.end()-1);
	return vec;
}

//w = np.maximum(0, xx2 - xx1 + 1)
std::vector<float> MaxSubtract(std::vector<float> vec1, std::vector<float> vec2, int minValue) {
	std::vector<float> sizeVec;

	float tmpSize;

	for (size_t idx = 0; idx < vec1.size(); ++idx) {
		tmpSize = vec1[idx] - vec2[idx] + 1;
		if (tmpSize < minValue) { tmpSize = minValue; }
		sizeVec.insert(sizeVec.end(), tmpSize);
	}

	return sizeVec;
}

std::vector<float> MultiplyDivide(std::vector<float> vec1,
		                              std::vector<float> vec2,
		                              std::vector<float> vec3) {

	std::vector<float> resultVec;
	for (size_t idx = 0; idx < vec1.size(); ++idx) {
		resultVec.insert(resultVec.end(), vec1[idx] * vec2[idx] / vec3[idx]);
	}

	return resultVec;
}

std::vector<int> WhereLarger(std::vector<float> vec, float threshold) {
	std::vector<int> resultVec;

	for (size_t idx = 0; idx < vec.size(); ++idx) {
		if (vec[idx] > threshold) {
			resultVec.insert(resultVec.end(), idx);
		}
	}

	return resultVec;
}

std::vector<int> RemoveDuplicates(std::vector<int> vec, std::vector<int> idxs) {
	std::vector<int> resultVec = vec;
	int offset = 0;

	for (int tmpIdx : idxs) {
		resultVec.erase(resultVec.begin() + tmpIdx + offset);
		offset -= 1;
	}

	return resultVec;
}

std::vector<cv::Rect> MakeRects(std::vector<std::vector<float> > boxes, std::vector<int> pick) {
	std::vector<cv::Rect> rects;
	std::vector<float> tmpFloatRect;

	for (int tmpPick : pick) {
		tmpFloatRect = boxes[tmpPick];
		rects.insert(rects.end(), cv::Rect(cv::Point(tmpFloatRect[0], tmpFloatRect[1]), 
																			 cv::Point(tmpFloatRect[2], tmpFloatRect[3])));
	}

	return rects;
}

cv::Rect VecToRect(std::vector<float> vec) {
	return cv::Rect(cv::Point(vec[0], vec[1]), cv::Point(vec[2], vec[3]));
}
