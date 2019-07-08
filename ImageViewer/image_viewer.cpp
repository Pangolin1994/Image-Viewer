#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "image_viewer.hpp"

const int NUMBER_OF_FORMATS = 3, HIST_SIZE = 256;
float range[] = {0, HIST_SIZE};
array<const char*, NUMBER_OF_FORMATS> im_formats = { "*.jpeg", "*.jpg", "*.png" };

vector<String> list_images(const String& path_to_dir) {
	vector<String> images, temp;
	for (auto f : im_formats) {
		glob(path_to_dir + f, temp);
		images.insert(images.begin(), temp.begin(), temp.end());
	}
	return images;
}

Mat make_gistogram_image(const Mat& image) {
	vector<Mat> rgb_planes;
	split(image, rgb_planes);
	const float* hist_range = {range};
	const auto uniform = true, accumulate = false;
	auto hists = new Mat[3];
	for (auto i = 0; i < 3; ++i) {
		calcHist(&rgb_planes[i], 1, nullptr, Mat(), hists[i], 1, &HIST_SIZE, &hist_range, uniform, accumulate);
	}
	const int height = 256, width = 256;
	const int bin_width = cvRound(static_cast<double>(height) / HIST_SIZE);
	Mat hist_image(height, width, CV_8UC3, Scalar(0, 0, 0));
	for (auto i = 0; i < 3; ++i) {
		normalize(hists[i], hists[i], 0, hist_image.rows, NORM_MINMAX, -1, Mat());
	}
	for (auto i = 1; i < HIST_SIZE; ++i) {
		line(hist_image, Point(bin_width*(i - 1), height - cvRound(hists[0].at<float>(i - 1))),
			Point(bin_width*(i), height - cvRound(hists[0].at<float>(i))),
			Scalar(255, 0, 0), 2, 8, 0);
		line(hist_image, Point(bin_width*(i - 1), height - cvRound(hists[1].at<float>(i - 1))),
			Point(bin_width*(i), height - cvRound(hists[1].at<float>(i))),
			Scalar(0, 255, 0), 2, 8, 0);
		line(hist_image, Point(bin_width*(i - 1), height - cvRound(hists[2].at<float>(i - 1))),
			Point(bin_width*(i), height - cvRound(hists[2].at<float>(i))),
			Scalar(0, 0, 255), 2, 8, 0);
	}
	delete[] hists;
	return hist_image;
}