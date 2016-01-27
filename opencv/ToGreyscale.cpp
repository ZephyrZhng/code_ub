#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main( int argc, const char** argv )
{
	Mat src;
	Mat dst;
	string filename;
	string src_window_title;
	string dst_window_title;
	int i = 1;

	for(int i = 1; i <= 7; ++i)
	{
		filename = to_string(i) + ".jpg";
		src = imread(filename, CV_LOAD_IMAGE_UNCHANGED);
		if (src.empty())
		{
			 cout << "Error : Image cannot be loaded!" << endl;
			 return -1;
		}

		cvtColor(src, dst, COLOR_BGR2GRAY);
		imwrite("grey" + filename, dst);

		src_window_title = filename + " show";
		dst_window_title = "grey" + filename + " show";
		
		namedWindow(src_window_title);
		imshow(src_window_title, src);
		waitKey(0);

		namedWindow(dst_window_title, CV_WINDOW_AUTOSIZE);
		imshow(dst_window_title, dst);
		waitKey(0);
	}

	return 0;
}