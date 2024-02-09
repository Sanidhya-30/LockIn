#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>

using namespace cv;
using namespace std;

// Global variables for the cascade files. Ensure these paths are correct.
const String face_cascade_name = "/home/ubuntu/LockIn/data/haarcascades/haarcascade_frontalface_alt2.xml";
// const String eyes_cascade_name = "haarcascade_eye_tree_eyeglasses.xml";

CascadeClassifier face_cascade;
// CascadeClassifier eyes_cascade;

// Function to load cascades and return success status
bool loadCascades() {
    if (!face_cascade.load(face_cascade_name) ) {
        cerr << "Error loading cascade files." << endl;
        return false;
    }
    return true;
}

// Simplified main function focusing on optimizations and corrections
int main() {
    if (!loadCascades()) {
        return -1; // Exit if cascades cannot be loaded
    }

    VideoCapture cap(0); // Open the default camera
    if (!cap.isOpened()) {
        cerr << "Error opening video capture." << endl;
        return -1;
    }

    Ptr<Tracker> tracker = TrackerMIL::create(); // Create a tracker
    if (!tracker) {
        cerr << "Error creating TrackerMIL." << endl;
        return -1;
    }

    // Main loop
    while (true) {
        Mat frame;
        if (!cap.read(frame)) { // Check if we succeeded
            cerr << "No captured frame -- Break!" << endl;
            break;
        }

        // Process frame (e.g., face detection, tracking)
        // This is where you'd integrate your processing logic

        // Example: Tracking initialization
        // Note: You should initialize the tracker with a valid bounding box (bbox) from detected objects
        // This example code does not include object detection logic
        Rect bbox(320, 240, 100, 100); 
        tracker->init(frame, bbox);
        rectangle(frame, bbox, Scalar(255, 0, 0), 2, 1);// Placeholder bounding box
        tracker->update(frame, bbox);
        // Display the resulting frame
        imshow("Frame", frame);

        if (waitKey(1) == 27) break; // Exit on ESC key
    }

    // When everything done, release the video capture and close windows
    cap.release();
    destroyAllWindows();
    return 0;
}
