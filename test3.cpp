#include <opencv2/opencv.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/core/ocl.hpp>
#include <iostream>
#include <string.h>

using namespace cv;
using namespace std;

// Constants
const int radius = 50;
const int font = FONT_HERSHEY_SIMPLEX;
const Scalar text_color(0, 255, 0);      // green
const Scalar circle_color(0, 255, 255);  // yellow
const int thickness = 1;
const int FH = 480;
const int FW = 640;
const int box_size = 50;
const string Tracker_type = "KCF";

// Global variables
String lockStatus = "LOOKING";
Point mouseClick((FW / 2), (FH / 2));
Point cursor((FW / 2), (FH / 2));
Ptr<Tracker> tracker = TrackerKCF::create();
Rect bbox (Point(((FW/2)-5),((FH/2)-5)), Point(((FW/2)+5),((FH/2)+5)));

// Function prototypes
void onMouseClick(int event, int x, int y, int flags, void* userdata);
void track(Mat frame);
void clearScreen(Mat& frame);
void drawCircle(Mat& frame, int radius, const Scalar& colorCircle, int thickness);
void frameText(Mat& frame, double fps);
double calFps(Mat& frame, double fps);

int main() 
{
    VideoCapture cap(-1);

    if (!cap.isOpened()) 
    {
        cerr << "Error opening camera!" << endl;
        return -1;
    }

    Mat frame;
    cap >> frame;

    namedWindow("Display Window");
    setMouseCallback("Display Window", onMouseClick, NULL);

    double fps = 0;
    tracker->init(frame, bbox);

    while (true) 
    {
        fps = calFps(frame, fps);
        frameText(frame, fps);
        drawCircle(frame, radius, circle_color, thickness);
        track(frame);

        if (!mouseClick.inside(Rect(radius, radius, FW - 2 * radius, FH - 2 * radius)))
        {
            clearScreen(frame);
        }

        imshow("Display Window", frame);

        if (waitKey(1) == 27) // 'Esc' key
            break;
    }

    cap.release();
    destroyAllWindows();

    return 0;
}

void onMouseClick(int event, int x, int y, int flags, void* userdata) 
{
    if (event == EVENT_LBUTTONDOWN) 
    {
        mouseClick = Point(x, y);
        bbox = Rect(Point((mouseClick.x - (box_size / 2)), (mouseClick.y - (box_size / 2))),
                    Point((mouseClick.x + (box_size / 2)), (mouseClick.y + (box_size / 2))));
        lockStatus = "LOCKED";
    }

    if (event == EVENT_MOUSEMOVE) 
    {
        cursor = Point(x, y);
    }
}

void track(Mat frame)
{
    bool ok = tracker->update(frame, bbox);
    if (ok)
    {
        rectangle(frame, bbox, Scalar(255, 0, 0), 2, 1);
    }
    else
    {
        putText(frame, "Tracking failure detected", Point(100, 80), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(0, 0, 255), 2);
    }
}

void clearScreen(Mat& frame)
{
    frame = Mat::zeros(frame.size(), frame.type());
    imshow("Display Window", frame);
    mouseClick = Point((FW / 2), (FH / 2));
    cout << "Target out of bounds" << endl;
    lockStatus = "LOOKING";
}

void drawCircle(Mat& frame, int radius, const Scalar& colorCircle, int thickness)
{
   Rect rect(Point((mouseClick.x - (box_size / 2)), (mouseClick.y - (box_size / 2))),
             Point((mouseClick.x + (box_size / 2)), (mouseClick.y + (box_size / 2))));
   rectangle(frame, rect, colorCircle, 1);
   cv::circle(frame, mouseClick, radius, colorCircle, thickness);

}

void frameText(Mat& frame, double fps)
{
    putText(frame, lockStatus, Point(((FW / 2) - 60), ((FH) - 50)), font, 1, text_color, (thickness + 1));
    putText(frame, "X: " + to_string(cursor.x) + " Y: " + to_string(cursor.y), Point((FH - 60), 30), font, 1, text_color, (thickness + 1));

    Point p1(mouseClick.x, 0), p2(FW, mouseClick.y), p3(mouseClick.x, FH), p4(0, mouseClick.y);
    line(frame, p1, p3, Scalar(255, 0, 0), thickness, LINE_4); 
    line(frame, p2, p4, Scalar(255, 0, 0), thickness, LINE_4); 

    rectangle(frame, Rect(Point((radius), (radius)), Point((FW - radius), (FH - radius))), (200, 0, 0), 1);
}

double calFps(Mat& frame, double fps)
{
    static auto lastTime = chrono::high_resolution_clock::now();
    auto currentTime = chrono::high_resolution_clock::now();
    double elapsedSeconds = chrono::duration_cast<chrono::duration<double>>(currentTime - lastTime).count();

    // Update FPS using weighted average
    fps = (0.85 / elapsedSeconds) + ((1 - 0.85) * fps);
    lastTime = currentTime;

    putText(frame, "FPS: " + to_string(static_cast<int>(fps)), Point(10, 30), font, 1, text_color, (thickness + 1));

    return fps;
}
