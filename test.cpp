#include <opencv2/opencv.hpp>
#include <opencv2/core/core_c.h>
#include <opencv2/tracking.hpp>
#include <opencv2/tracking/tracking_legacy.hpp>
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>

using namespace cv;
using namespace std;


cv::Rect thresholding(cv::Mat frame, cv::Rect2d& roi, int thresh)
{
    static int previousArea;
    // Crop the region of interest (ROI) from the frame
    cv::Mat roiFrame = frame(roi);

    // Convert the cropped frame to grayscale
    cv::Mat grayFrame;
    cv::cvtColor(roiFrame, grayFrame, cv::COLOR_BGR2GRAY);

    // Blur the gray frame
    cv::Mat blurFrame;
    cv::GaussianBlur(grayFrame, blurFrame, cv::Size(11, 11), 10, 10);

    // Apply thresholding to segment objects
    cv::Mat binaryFrame;
    cv::threshold(blurFrame, binaryFrame, thresh, 255, cv::THRESH_BINARY + cv::THRESH_OTSU);

    cv::Mat kernel;
    cv::Mat openFrame;
    kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(10, 10));

    cv::morphologyEx(binaryFrame, openFrame, cv::MORPH_OPEN, kernel);
    cv::bitwise_not(openFrame, openFrame);

    cv::Mat canny_output;
    Canny(openFrame, canny_output, thresh, thresh * 2);

    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;  // Add hierarchy vector

    cv::findContours(canny_output, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    std::vector<cv::Point> max_contour;
    for (size_t i = 0; i < contours.size(); i++)
    {
        if (contours[i].size() > max_contour.size())
        {
            max_contour = contours[i];
        }
    }

    // Check if max_contour is not empty before calculating boundingRect
    if (!max_contour.empty())
    {
        cv::Rect boundRect = boundingRect(max_contour);

        // Initialize previousArea with the area of the blob from the first frame
        if (previousArea == 0) {
            previousArea = boundRect.area();
        }

        // Store the area of the blob from the current frame
        int currentArea = boundRect.area();

        // Adjust dimensions based on the change in area
        double areaChangeFactor = sqrt(static_cast<double>(currentArea) / previousArea);
        int padding = 20;
        int adjustedWidth = static_cast<int>(boundRect.width * areaChangeFactor);
        int adjustedHeight = static_cast<int>(boundRect.height * areaChangeFactor);

        // Update the previousArea for the next frame
        previousArea = currentArea;

        // Create and draw the adjusted bounding box
        cv::Rect bbox(boundRect.x - padding, boundRect.y - padding, adjustedWidth + 2 * padding, adjustedHeight + 2 * padding);
        cv::rectangle(openFrame, bbox, Scalar(0, 255, 0));
        cout << bbox.area() << "\n";

        cv::imshow("blob", openFrame);
        return bbox;
    }
    else
    {
        // Return an invalid Rect if no contours are found
        cout << "no contour" << "\n";
        return cv::Rect();
    }

    cv::Rect bbox(roi.x - 1, roi.y - 1, roi.x + 2 , roi.y + 2 );
    return bbox;
}


int main()
{
    int mode;
    string x;
    cout<<"Seelct either : \n 1. heplicopter Video\n 2. Helicopter Video\n 3. cat Video\n";
    cin>>mode;
    int i = 0;

    // // Open the webcam"
    // switch (mode)
    // {

    // case 1:
    //     x = "/home/ubuntu/LockIn/videos/helicopter.mp4";
    //     break;

    // case 2:
    //     x = "/home/ubuntu/LockIn/videos/heplicopter.mp4";
    //     break;

    // case 3:
    //     x = "/home/ubuntu/LockIn/videos/car.mp4";
    //     break;
    
    // default:
    //     cv::VideoCapture cap(0);
    //     break;
    // }

    cv::VideoCapture cap(0);

    if (!cap.isOpened()) {
        std::cerr << "Error: Couldn't open the webcam." << std::endl;
        return -1;
    }

    // Read the first frame
    cv::Mat frame;
    cap >> frame;

    // Select a region to track using a bounding box
    waitKey(1000);
    cv::Rect2d roi = cv::selectROI("Select ROI", frame, false, false);

    while (true) 
    {
        // Initialize the MOSSE tracker
        Ptr<legacy::TrackerMOSSE> tracker = cv::legacy::TrackerMOSSE::create();
        tracker->init(frame, roi);
        
        // Read a new frame
        cap >> frame;
        if (frame.empty())
            break;

        // Update the tracker with the new frame
        bool success = tracker->update(frame, roi);

        // Draw the bounding box on the frame
        if (success)
            cv::rectangle(frame, roi, cv::Scalar(0, 255, 0), 2);
        else
            cv::putText(frame, "Tracking failed", cv::Point(100, 80), cv::FONT_HERSHEY_SIMPLEX, 0.75, cv::Scalar(0, 0, 255), 2);

        int padding = 10;
        cv::Rect2d uproi(roi.x - padding, roi.y - padding, roi.width + 2 * padding, roi.height + 2 * padding);
        cv::Rect2d newroi = thresholding(frame, uproi, 150);

        // Display the frame
        cv::imshow("MOSSE Tracker", frame);
        cout<<roi.area()<<"\n";

        // Exit the loop if the user presses 'Esc'
        char key = cv::waitKey(33);
        if (key == 27) // ASCII code for 'Esc'
            break;

        // i = i+10;
        tracker.release();
    }

    // Release the video capture object
    cap.release();

    return 0;
}
