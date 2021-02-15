// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#include <opencv2/objdetect.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/core/utility.hpp>
#include <iostream>
#include <iomanip>

using namespace cv;
using namespace std;

#define USB_CAMERA 0

class Detector
{
    enum Mode { Default, Daimler } m;
    HOGDescriptor hog, hog_d;

public:    
    Detector() : m(Default), hog(), hog_d(Size(48, 96), Size(16, 16), Size(8, 8), Size(8, 8), 9)
    {
        hog.setSVMDetector(HOGDescriptor::getDefaultPeopleDetector());
        hog_d.setSVMDetector(HOGDescriptor::getDaimlerPeopleDetector());
    }
    
    void toggleMode() { m = (m == Default ? Daimler : Default); }
    
    string modeName() const { return (m == Default ? "Default" : "Daimler"); }
    
    void detect(InputArray img, vector<Rect>& found, vector<double>& weights)
    {
        // Run the detector with default parameters. to get a higher hit-rate
        // (and more false alarms, respectively), decrease the hitThreshold and
        // groupThreshold (set groupThreshold to 0 to turn off the grouping completely).
        if (m == Default)
            hog.detectMultiScale(img, found, weights, .25, Size(8,8), Size(), 1.05, 2, false);
        else if (m == Daimler)
            hog_d.detectMultiScale(img, found, weights, .1, Size(8,8), Size(), 1.05, 2, true);
    }

    void adjustRect(Rect & r) const
    {
        // The HOG detector returns slightly larger rectangles than the real objects,
        // so we slightly shrink the rectangles to get a nicer output.
        r.x += cvRound(r.width*0.1);
        r.width = cvRound(r.width*0.8);
        r.y += cvRound(r.height*0.07);
        r.height = cvRound(r.height*0.8);
    }
};

int main(int argc, char** argv)
{
    // open camera
    VideoCapture cap(USB_CAMERA);

    if (!cap.isOpened())
    {
        cout << "Can't open video" << endl;
        return 2;
    }

    cout << "Press 'q' or <ESC> to quit." << endl;
    cout << "Press <space> to toggle between Default and Daimler detector" << endl;
    
    Detector detector;
    Mat frame;

    for (;;)
    {
        cap >> frame;
        if (frame.empty())
        {
            cout << "Finished reading: empty frame" << endl;
            break;
        }

        vector<Rect> found;
        vector<double> weights;
        int64 t = getTickCount();
        detector.detect(frame, found, weights);
        t = getTickCount() - t;

        // show the window
        {
            ostringstream buf;
            buf << "Mode: " << detector.modeName();
                // << "FPS: " << fixed << setprecision(1) << (getTickFrequency() / (double)t);
            putText(frame, buf.str(), Point(10, 30), FONT_HERSHEY_PLAIN, 2.0, Scalar(0, 0, 255), 2, LINE_AA);
        }


        for (vector<Rect>::iterator i = found.begin(); i != found.end(); ++i)
        {
            // draw box
            Rect &r = *i;
            detector.adjustRect(r);
            rectangle(frame, r.tl(), r.br(), cv::Scalar(0, 255, 0), 2);

            // label confidence
            // ostringstream buf;
            // buf << weights[i - found.begin()];
            // putText(frame, buf.str(), Point(r.x, r.y+50), FONT_HERSHEY_SIMPLEX, 1, Scalar(0,0,255));
        }

        // Show image
        imshow("People detector", frame);

        // interact with user
        const char key = (char)waitKey(1);
        if (key == 27 || key == 'q') // ESC
        {
            cout << "Exit requested" << endl;
            break;
        }
        else if (key == ' ')
        {
            detector.toggleMode();
        }
    }
    return 0;
}
