#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include "image_viewer.hpp"

using namespace std;
using namespace cv;

const int NUMBER_OF_ARGUMENTS_ERROR = -1,
		  INPUT_ARGUMENT_ERROR = -2;

void callback(int val, void* data) {
	auto paths = *static_cast<vector<String>*>(data);
	const auto image = imread(paths.at(val), IMREAD_COLOR);
	if (!image.data) {
		cout << "Image can't be read";
		return;
	}
	const auto hist = make_gistogram_image(image);
	imshow("Image_Window", image);
	imshow("Hist_Window", hist);
}

int main(int argc, char** argv)
{
	if (argc != 2) {
		cout << "Two arguments will have to be passed exactly";
		return NUMBER_OF_ARGUMENTS_ERROR;
	}
	auto image_paths = list_images(String(argv[1]));
	if (image_paths.empty()) {
		cout << "Input directory string has incorrect format";
		return INPUT_ARGUMENT_ERROR;
	}
	int val = 0;
	namedWindow("Window", WINDOW_AUTOSIZE);
	namedWindow("Hist_Window", WINDOW_AUTOSIZE);
	createTrackbar("TrackBar", "Window", &val, image_paths.size() - 1, callback, &image_paths);
	waitKey(0);
	destroyAllWindows();
	return 0;
}