#ifndef IMAGE_VIEWER_HPP
#define IMAGE_VIEWER_HPP
#include <vector>
#include <opencv2/core.hpp>

using namespace cv;
using namespace std;

vector<String> list_images(const String&);

Mat make_gistogram_image(const Mat&);
#endif