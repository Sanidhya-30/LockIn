#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
// #include "FaceDetector.h"
#include "opencv2/objdetect.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "numeric"
using namespace cv;
using namespace std;


// ------------------------------ VARIABLES ---------------------------- //

std::chrono::_V2::system_clock::time_point start;
double medianFPS = 0;
int n =10 ;
int alpha = 0.85;

String face_cascade_name = "/home/ubuntu/LockIn/data/haarcascades/haarcascade_frontalface_alt2.xml";
CascadeClassifier face_cascade;
String window_name = "Capture - Face detection";



// ------------------------------ PROCESS FRAME ---------------------------- //

// void proc(cv::Mat frame) 
// {
//     // Add your frame processing logic here
//     std::vector<Rect> faces;
//     Mat frame_gray;
//     //Conversion of frame to grayscale
//     cvtColor(frame, frame_gray, COLOR_BGR2GRAY); 
//     //Contrast enhance(Spread out intensity distribution)
//     equalizeHist(frame_gray, frame_gray);       
//     //-- Detect faces
//     if (!face_cascade.load("path/to/haarcascade_frontalface_default.xml")) {
//         std::cerr << "Error loading face cascade." << std::endl;
//     face_cascade.detectMultiScale(frame_gray, faces, 1.1, 2, 0 | CASCADE_SCALE_IMAGE, Size(30, 30));
//     cout << faces.size();
//     for (size_t i = 0; i < faces.size(); i++)
//     {
//         Point center(faces[i].x + faces[i].width / 2, faces[i].y + faces[i].height / 2);
//         ellipse(frame, center, Size(faces[i].width / 2, faces[i].height / 2), 0, 0, 360, Scalar(255, 0, 255), 4, 8, 0);
//         Mat faceROI = frame_gray(faces[i]);
//     }   

// }
// }




// ------------------------------ MEDIAN FPS ---------------------------- //

double median_fps(double fps, int n)
{
    static std::vector<double> fpsHistory;
    auto currentTime = chrono::high_resolution_clock::now();
    double elapsedSeconds = std::chrono::duration_cast<chrono::duration<double>>(currentTime - start).count();

    // Update FPS using weighted average
    fps = (0.85 / elapsedSeconds) + ((1 - 0.85) * fps);

    fpsHistory.push_back(fps);

    if (fpsHistory.size() > n)
        fpsHistory.erase(fpsHistory.begin());

    // Calculate median FPS
    if (!fpsHistory.empty()) {
        std::sort(fpsHistory.begin(), fpsHistory.end());
        size_t size = fpsHistory.size();
        if (size % 2 == 0)
            medianFPS = (fpsHistory[size / 2 - 1] + fpsHistory[size / 2]) / 2.0;
        else
            medianFPS = fpsHistory[size / 2];
    }

    return medianFPS;
}




// ------------------------------ MEAN FPS ---------------------------- //

double mean_fps(double fps, int n) 
{
    double total=0, meanFPS=0;
    static std::vector<double> fpsHistory;
    auto currentTime = chrono::high_resolution_clock::now();
    double elapsedSeconds = chrono::duration_cast<chrono::duration<double>>(currentTime - start).count();

    fps = (0.85 / elapsedSeconds) + ((1 - 0.85) * fps);
    fpsHistory.push_back(fps);

    if (fpsHistory.size() > n)
        fpsHistory.erase(fpsHistory.begin());


    if (!fpsHistory.empty()) 
    {
        std::sort(fpsHistory.begin(), fpsHistory.end());
        size_t size = fpsHistory.size();
        total = accumulate(fpsHistory.begin(),fpsHistory.end(),0);
        meanFPS = total/size;
    }
    // cout<<fpsHistory[0]<<endl;

    return meanFPS;
}




// ------------------------------ MOVING AVG. FPS ---------------------------- //

double exponential_moving_average(double fps, double alpha) 
{
    static std::vector<double> fpsHistory;
    auto currentTime = chrono::high_resolution_clock::now();
    double elapsedSeconds = std::chrono::duration_cast<chrono::duration<double>>(currentTime - start).count();
    
    // Update FPS using exponential moving average
    fps = alpha / (elapsedSeconds) + (1 - alpha) * fps;

    fpsHistory.push_back(fps);

    // Keep the history size limited to 10
    if (fpsHistory.size() > 10)
        fpsHistory.erase(fpsHistory.begin());

    return fps;
}




// ------------------------------ LOGGING DATA TO CSV ---------------------------- //

void log_data(ofstream& outputFile, int frameCount, double medianFPS, double meanFPS, double tsliceFPS, double emaFPS, double instFPS) 
    {outputFile << frameCount << ',' << medianFPS << ',' << meanFPS << ',' << tsliceFPS << ',' << emaFPS << ',' << instFPS << '\n';}




int main() 
{    
    ofstream outputFile("fps_data.csv");
    
    if (!outputFile.is_open()) 
    {
        cerr << "Error opening CSV file" << endl;
        return -1;
    }

    outputFile << "Frame,MedianFPS,MeanFPS,TsliceFPS,EMAFPS,InstFPS\n";

    cv::VideoCapture cap(-1);

    if (!cap.isOpened()) 
    {
        cerr << "Error opening camera!" << endl;
        return -1;
    }

    int n = 10;  // Adjust the value based on your requirements
    double alpha = 0.85;
    double fps = 0;
    int frameCount = 0;

    Ptr<Tracker> tracker = TrackerMIL::create();
    if (!tracker)
    {
        cerr << "Error creating TrackerMIL" << endl;
        return -1;
    }

    while(true) 
    {
        start = chrono::high_resolution_clock::now();
        cv::Mat frame;
        cap >> frame;
        int key = cv::waitKey(1); // Change the delay to control frame rate

        Rect bbox(320, 240, 100, 100); 
        tracker->init(frame, bbox);
        rectangle(frame, bbox, Scalar(255, 0, 0), 2, 1);
        bool ok = tracker->update(frame, bbox);
        if (!ok)
        {
            cerr << "Error updating tracker" << endl;
            // Handle the error, for example, by breaking out of the loop or returning from the function.
        }
        
        auto end = chrono::high_resolution_clock::now();
        chrono::duration<double> diffT = end - start;

        medianFPS = median_fps(fps, n);
        double emaFPS = exponential_moving_average(fps, alpha);
        double meanFPS = mean_fps(fps, n);
        double tsliceFPS = 1 / diffT.count();
        double instFPS = 1 / diffT.count();
        frameCount++;
        log_data(outputFile, frameCount, medianFPS, meanFPS, tsliceFPS, emaFPS, instFPS);

        if (key == 27) {break;} // 'Esc' key

        cv::imshow("Display Window", frame);
    }

    outputFile.close();
    return 0;
}
