#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include "test.h"

using namespace cv;
using namespace std;

int main( int argc, const char** argv )
{
     test();
     //read the image data in the file "testpic.jpg" and store it in 'img'
     Mat img = imread("testpic.jpg", CV_LOAD_IMAGE_UNCHANGED); 

     if (img.empty()) //check whether the image is loaded or not
     {
          cout << "Error : Image cannot be loaded..!!" << endl;
          //system("pause"); //wait for a key press
          return -1;
     }

     namedWindow("MyWindow", CV_WINDOW_AUTOSIZE); //create a window with the name "MyWindow"
     imshow("MyWindow", img); //display the image which is stored in the 'img' in the "MyWindow" window

     waitKey(0); //wait infinite time for a keypress

     destroyWindow("MyWindow"); //destroy the window with the name, "MyWindow"

     return 0;
}