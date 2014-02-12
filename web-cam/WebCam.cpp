#include <stdio.h>
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main(int argc, const char** argv)
{
    VideoCapture cap(0);
    if(!cap.isOpened())
    {
        cout << "Failed to acquire camera" << endl;
        return -1;
    }

    namedWindow("result", WINDOW_AUTOSIZE);
    for(;;)
    {
        Mat frame;
        cap >> frame;
        imshow("result", frame);
        if(waitKey(30) >= 0) break;
    }

    return 0;
}
