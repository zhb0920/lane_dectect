#ifndef PROCESS_H
#define PROCESS_H
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <vector>
using namespace cv;
using namespace std;

 Vec4i* fitting_Line(Vec4i leftLine ,Vec4i rightLine,Mat frame);




#endif // PROCESS_H
