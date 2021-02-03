#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;
using namespace cv::ml;

#define USB_CAMERA 0

int main( int argc, const char** argv )
{
    /// Create a videoreader interface
    VideoCapture cap(USB_CAMERA);
    Mat current_frame;

    /// Set up the pedestrian detector --> let us take the default one
    HOGDescriptor hog;
    hog.setSVMDetector(HOGDescriptor::getDefaultPeopleDetector());

    /// Set up tracking vector (not really needed)
    // vector<Point> track;

    while (1)
    {
        /// Grab a single frame from the video
        cap >> current_frame;

        /// Check if the frame has content
        if(current_frame.empty())
        {
            cerr << "Video has ended or bad frame was read. Quitting." << endl;
            return 0;
        }

        /// run the detector with default parameters. to get a higher hit-rate
        /// (and more false alarms, respectively), decrease the hitThreshold and
        /// groupThreshold (set groupThreshold to 0 to turn off the grouping completely).

        ///image, vector of rectangles, hit threshold, win stride, padding, scale, group th
        Mat img = current_frame.clone();
        resize(img,img,Size(img.cols*2, img.rows*2));

        vector<Rect> found;
        vector<double> weights;

        // detects objects in image
        hog.detectMultiScale(img, found, weights);

        /// draw detections and store location
        for(size_t i = 0; i < found.size(); i++)
        {
            Rect r = found[i];
            rectangle(img, found[i], cv::Scalar(0,0,255), 3);
            stringstream temp;
            temp << weights[i];
            putText(img, temp.str(), Point(found[i].x, found[i].y+50), FONT_HERSHEY_SIMPLEX, 1, Scalar(0,0,255));
            // track.push_back(Point(found[i].x+found[i].width/2,found[i].y+found[i].height/2));
        }

        // /// plot the track so far
        // for(size_t i = 1; i < track.size(); i++){
        //     line(img, track[i-1], track[i], Scalar(255,255,0), 2);
        // }

        if (found.size() == 0)
            putText(img, "No people detected", Point(10, 30), FONT_HERSHEY_PLAIN, 2.0, Scalar(0, 0, 255), 2, LINE_AA);
        // else
        //     putText(img, "Found" << found.size() << " people", Point(10, 30), FONT_HERSHEY_PLAIN, 2.0, Scalar(0, 0, 255), 2, LINE_AA);

        /// show
        imshow("detected person", img);

        // interact with user
        const char key = (char)waitKey(1);
        if (key == 27 || key == 'q') // ESC
        {
            cout << "Exit requested" << endl;
            break;
        }
    }

    return 0;
}