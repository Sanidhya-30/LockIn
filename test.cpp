#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

Point mouseClick;

void onMouseClick(int event, int x, int y, int flags, void* userdata) {
    if (event == EVENT_LBUTTONDOWN) {
        mouseClick = Point(x, y);
    }
}

int main() {
    cv::VideoCapture cap(0);
   
    if (!cap.isOpened()) {
        std::cerr << "Error opening camera!" << std::endl;
        return -1;
    }

    cv::namedWindow("Display Window");
    setMouseCallback("Display Window", onMouseClick, NULL);
   
    while (true) {
        cv::Mat frame;
        cap >> frame;

        // Draw a circle on the frame at the mouse-clicked location
        if (!mouseClick.inside(Rect(0, 0, frame.cols, frame.rows))) {
            circle(frame, mouseClick, 20, Scalar(0, 255, 0), 2);
        }

        // Display the frame
        cv::imshow("Display Window", frame);

        // Check for user input to exit the loop
        if (cv::waitKey(1) == 27) // 'Esc' key
            break;
    }

    // Release the camera and close the window after exiting the loop
    cap.release();
    cv::destroyAllWindows();

    return 0;
}
