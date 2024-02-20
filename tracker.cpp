#include <opencv2/opencv.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/tracking/tracking_legacy.hpp>

using namespace cv;

int main() {
    // Open the webcam
    cv::VideoCapture cap(0);
    if (!cap.isOpened()) {
        std::cerr << "Error: Couldn't open the webcam." << std::endl;
        return -1;
    }

    // Read the first frame
    cv::Mat frame;
    cap >> frame;

    // Select a region to track using a bounding box
    cv::Rect2d roi = cv::selectROI("Select ROI", frame, false, false);

    // Initialize the MOSSE tracker
    Ptr<legacy::TrackerMOSSE> tracker = cv::legacy::TrackerMOSSE::create();
    tracker->init(frame, roi);

    while (true) {
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

        // Display the frame
        cv::imshow("MOSSE Tracker", frame);

        // Exit the loop if the user presses 'Esc'
        char key = cv::waitKey(1);
        if (key == 27) // ASCII code for 'Esc'
            break;
    }

    // Release the video capture object
    cap.release();

    return 0;
}
