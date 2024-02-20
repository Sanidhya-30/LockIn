#include <opencv2/opencv.hpp>
#include <opencv2/core/core_c.h>
#include <opencv2/tracking.hpp>
#include <opencv2/tracking/tracking_legacy.hpp>
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/xfeatures2d.hpp"

using namespace cv;
using namespace std;

const int MAX_FEATURES = 500;
const float GOOD_MATCH_PERCENT = 0.15f;

cv::Rect thresholding(cv::Mat frame, cv::Rect2d& roi, int thresh)
{
    // Convert images to grayscale
    cv::Mat roiFrame = frame(roi);
    Mat im1Gray, im2Gray;
    cvtColor(roiFrame, im1Gray, cv::COLOR_BGR2GRAY);
    cvtColor(frame, im2Gray, cv::COLOR_BGR2GRAY);

    // Variables to store keypoints and descriptors
    std::vector<KeyPoint> keypoints1, keypoints2;
    Mat descriptors1, descriptors2;

    // Detect ORB features and compute descriptors within the ROI
    Ptr<ORB> orb = ORB::create(MAX_FEATURES);
    orb->detectAndCompute(im1Gray, Mat(), keypoints1, descriptors1);
    orb->detectAndCompute(im2Gray, Mat(), keypoints2, descriptors2);

    // Match features.
    std::vector<DMatch> matches;
    Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create("BruteForce-Hamming");
    matcher->match(descriptors1, descriptors2, matches, Mat());

    // Sort matches by score
    std::sort(matches.begin(), matches.end());

    // Remove not so good matches(Lowe ratio test)
    const int numGoodMatches = matches.size() * GOOD_MATCH_PERCENT;
    matches.erase(matches.begin() + numGoodMatches, matches.end());

    // Draw top matches
    Mat imMatches;
    drawMatches(roiFrame, keypoints1, frame, keypoints2, matches, imMatches);
    imshow("matches.jpg", imMatches);

    // Extract location of good matches
    std::vector<Point2f> points1, points2;

    for (size_t i = 0; i < matches.size(); i++)
    {
        points1.push_back(keypoints1[matches[i].queryIdx].pt);
        points2.push_back(keypoints2[matches[i].trainIdx].pt);
    }

    std::vector<Point2f> obj_corners(4);
    std::vector<Point2f> scene_corners(4);

    obj_corners[0] = cv::Point(0, 0);
    obj_corners[1] = cv::Point(roi.width, 0);
    obj_corners[2] = Point2f(roi.width, roi.height);
    obj_corners[3] = Point2f(0, roi.height);

    // Find homography
    Mat h = findHomography(points1, points2, RANSAC);

    // Use homography to warp image
    perspectiveTransform(obj_corners, scene_corners, h);

    line(frame, scene_corners[0], scene_corners[1], Scalar(255, 0, 0), 2, LINE_8, 0);
    line(frame, scene_corners[1], scene_corners[2], Scalar(255, 0, 0), 2, LINE_8, 0);
    line(frame, scene_corners[2], scene_corners[3], Scalar(255, 0, 0), 2, LINE_8, 0);
    line(frame, scene_corners[3], scene_corners[0], Scalar(255, 0, 0), 2, LINE_8, 0);

    return roi;
}

int main()
{
    int mode;
    string x;
    cout<<"Select either : \n 1. heplicopter Video\n 2. Helicopter Video\n 3. cat Video\n";
    cin>>mode;
    int i = 0;

    // Open the webcam"
    switch (mode)
    {

    case 1:
        x = "/home/ubuntu/LockIn/videos/helicopter.mp4";
        break;

    case 2:
        x = "/home/ubuntu/LockIn/videos/heplicopter.mp4";
        break;

    case 3:
        x = "/home/ubuntu/LockIn/videos/car.mp4";
        break;
    
    default:
        // cv::VideoCapture cap(0);
        break;
    }

    cv::VideoCapture cap(x);

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
