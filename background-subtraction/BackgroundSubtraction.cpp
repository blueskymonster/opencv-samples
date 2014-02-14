#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/background_segm.hpp>

#include <stdio.h>

#include <iostream>
#include <sstream>

using namespace cv;
using namespace std;

//global variables
Mat frame; //current frame
Mat fgMaskMOG; //fg mask generated by MOG method
Mat fgMaskMOG2; //fg mask generated by MOG2 method
Ptr<BackgroundSubtractor> pMOG; //MOG Background subtractor
Ptr<BackgroundSubtractor> pMOG2; //MOG2 Background subtractor
int keyboard;

//function declarations
void help();
void processVideo(char* videoFilename);

void help()
{
  cout
  << "----------------------------------------------------------------------------" << endl
  << "This program shows how to use background subtraction methods provided by "    << endl
  << "OpenCV."                                                                      << endl 
                                                                                    << endl
  << "Usage:"                                                                       << endl
  << "./BackgroundSubtraction <video filename>"                                     << endl
  << "for example: ./BackgroundExample video.avi"                                   << endl
  << endl;
}

int main(int argc, char* argv[])
{
  if(argc != 2)
  {
    help();
    return EXIT_FAILURE;
  }

  //create GUI windows
  namedWindow("Frame");
  namedWindow("FG Mask MOG");
  namedWindow("FG Mask MOG2");

  //create Background Subtractor objects
  pMOG = createBackgroundSubtractorMOG();
  pMOG2 = createBackgroundSubtractorMOG2();

  processVideo(argv[1]);

  destroyAllWindows();

  return EXIT_SUCCESS;
}

void processVideo(char* videoFilename)
{
  //create the capture object
  VideoCapture capture(videoFilename);
  if(!capture.isOpened())
  {
    cerr << "Unable to open video file: " << videoFilename << endl;
    exit(EXIT_FAILURE);
  }
  //read input data. ESC or 'q' for quitting
  while ((char)keyboard != 'q' && (char)keyboard != 27)
  {
    //read the current frame
    if(!capture.read(frame))
    {
      cerr << "Unable to read next frame." << endl;
      cerr << "Exiting..." << endl;
      exit(EXIT_FAILURE);
    }
    //update the background model
    pMOG->apply(frame, fgMaskMOG);
    pMOG2->apply(frame, fgMaskMOG2);  
    //get the frame number and write it on the current frame
    stringstream ss;
    rectangle(frame, cv::Point(10, 2), cv::Point(100, 20),
              cv::Scalar(255, 255, 255), -1);
    ss << capture.get(CAP_PROP_POS_FRAMES);
    string frameNumberString = ss.str();
    putText(frame, frameNumberString.c_str(), cv::Point(15, 15),
            FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0,0,0));
    //show the current frame and the fg masks
    imshow("Frame", frame);
    imshow("FG Mask MOG", fgMaskMOG);
    imshow("FG Mask MOG2", fgMaskMOG2);
    //get the input from the keyboard
    keyboard = waitKey(30);
  }
  //delete capture object
  capture.release();
}