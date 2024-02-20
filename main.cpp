#include <opencv2/opencv.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/tracking/tracking_legacy.hpp>
#include <iostream>
#include <string.h>

using namespace cv;
using namespace std;


// ----------------------------- USER DEFINED VARIABLES  -------------------------

int radius              = 50;
int font                = FONT_HERSHEY_SIMPLEX;
cv::Scalar color   = cv::Scalar(0, 255, 0);     //green
int thickness           = 1;
int FH                  = 480 ;     // frame heigth
int FW                  = 640 ;     // frame width
int box_size            = 50;


// ----------------------------- SOME CONSTANTS  -------------------------

String lock_status  = "LOOKING";
String gimble_stat  = "";
Point mouseClick    = Point((FW/2), (FH/2));
Point Cursor        = Point((FW/2), (FH/2));
Point cam           = Point((FW/8),(7*FH/8));
double medianFPS = 0.0;
std::chrono::_V2::system_clock::time_point lastTime;


// ----------------------------- MOUSECLICK CALLBACK  -------------------------

void onMouseClick(int event, int x, int y, int flags, void* userdata) 
{
    if (event == EVENT_LBUTTONDOWN) 
    {
        mouseClick = Point(x, y);
        lock_status = "LOCKED";
    }

    if (event == EVENT_MOUSEMOVE) 
    {
        Cursor = Point(x, y);
    }
   
   // return;
}



// ----------------------------- CAMERA FOV VERBOSE  -------------------------

void camera_fov(cv::Mat frame, int key)
{
    // Max FOV box
    cv::Rect max_cam_box(Point(0, int(3*(FH/4))), Point((FW/4), FH));
    cv::rectangle(frame, max_cam_box , color, 2);


    if ((cam.x > 0+(FW/80)) && (cam.x < ((FW / 4)-(FW/80))) && (cam.y < (FH-(FW/80))) && (cam.y > (((3 * FH) / 4)+FH/80)))
    {
      // Update camera position based on key events
      if (key != -1) 
      {
          if (key == 'w' || key == 'W')       {cam.y = cam.y - 8;} // UP key
          else if (key == 's' || key == 'S')  {cam.y = cam.y + 8;} // DOWN key
          else if (key == 'a' || key == 'A')  {cam.x = cam.x - 8;} // LEFT key
          else if (key == 'd' || key == 'D')  {cam.x = cam.x + 8;} // RIGHT key
      }
        // Draw updated cam box
      cv::Rect cam_box(Point(int((cam.x) - (FW / 40)), int((cam.y) - (FH / 40))),
                       Point(int((cam.x) + (FH / 40)), int((cam.y) + (FH / 40))));
      cv::rectangle(frame, cam_box, color, 1);
      gimble_stat = "";
    } 
    else 
    {
      gimble_stat = "Gimble restricted";
      if(cam.x<(FW/40)  ? cam.x = (cam.x+8) : cam.x = (cam.x-8));
      if(cam.y<(7*FH/8) ? cam.y = (cam.y+8) : cam.y = (cam.y-8));
    }

//    return;
}




// ----------------------------- CLEAR CIRCLE  -------------------------

void clear_screen(cv::Mat frame)
{
   //clear screen
   frame = Mat::zeros(frame.size(), frame.type());
   cv::imshow("Display Window", frame);
   mouseClick = Point((FW/2), (FH/2));
   cam = Point((FW/8),(7*FH/8));
   cout<< "Target out of bounds";
   lock_status = "LOOKING";
   // cal_fps;
   return;
}



// ----------------------------- DRAW CIRCLE  -------------------------

void draw_circle(cv::Mat frame, int radius, cv::Scalar color_circle, int thickness)
{
   // cv::circle(frame, mouseClick, radius, color_circle, thickness);
   cv::Rect rect(Point((int(mouseClick.x) - (box_size/2)), (int(mouseClick.y) - (box_size/2))), Point((int(mouseClick.x) + (box_size/2)), (int(mouseClick.y) + (box_size/2))));
   cv::rectangle(frame, rect ,color_circle, 1);
   return;
}



// ----------------------------- PUT TEXT IN FRAME  -------------------------

void frame_text(cv::Mat frame, double fps, int radius, cv::Scalar color, int thickness)
{
   //locked:status
   cv::putText(frame, lock_status, Point(((FW/2)-60),(((FH))-60)), font, 1, color, (thickness+1));

   //MOUSE XY coordinates
   cv::putText(frame, "Cursor X: " + to_string(Cursor.x) + " Y: " + to_string(Cursor.y), Point((FW - 200), 20), font, 0.5, color, (thickness+0.5));

   //CAM XY coordinates
   cv::putText(frame, "  Cam X: " + to_string(cam.x) + " Y: " + to_string(cam.y), Point((FW - 200), 40), font, 0.5, color, (thickness+0.5));

   // GIMBLE stat
   cv::putText(frame, gimble_stat, Point((FW - 150),(FH-20)), font, 0.5, color, thickness);


   //line
   Point p1(mouseClick.x, 0), p2(FW, mouseClick.y), p3(mouseClick.x, FH), p4(0, mouseClick.y);
   line(frame, p1, p3, Scalar(0, 255, 0), thickness, LINE_4); 
   line(frame, p2, p4, Scalar(0, 255, 0), thickness, LINE_4); 

   //ROI box
   cv::rectangle(frame, cv::Rect(Point((radius), (radius)), Point((FW-(radius)), (FH-(radius))) ), Scalar(0, 255, 0), 1, cv::LINE_8);

   return;
}



// ----------------------------- CALCULATE FPS  -------------------------

double cal_fps(cv::Mat frame, double fps)
{
    static std::vector<double> fpsHistory;
    auto currentTime = chrono::high_resolution_clock::now();
    double elapsedSeconds = std::chrono::duration_cast<chrono::duration<double>>(currentTime - lastTime).count();
   
    // Update FPS using weighted average
    fps = (0.85 / elapsedSeconds) + ((1 - 0.85) * fps);
    lastTime = currentTime;

    fpsHistory.push_back(fps);

    // Keep the history size limited to 10
    if (fpsHistory.size() > 5)
        fpsHistory.erase(fpsHistory.begin());

    // Calculate median FPS
    if (!fpsHistory.empty())
    {
        std::sort(fpsHistory.begin(), fpsHistory.end());
        size_t size = fpsHistory.size();
        if (size % 2 == 0)
            medianFPS = (fpsHistory[size / 2 - 1] + fpsHistory[size / 2]) / 2.0;
        else
            medianFPS = fpsHistory[size / 2];
    }

    //fps
    cv::putText(frame, "FPS: " + to_string(static_cast<int>(medianFPS)), Point(15, 35), font, 1, color, (thickness+1));
   
    return fps;
}




// ----------------------------- MAIN -------------------------

int main() 
{
    cv::VideoCapture cap("/home/ubuntu/LockIn/videos/heplicopter.mp4");
//    cv::VideoCapture cap(0);
   
   if (!cap.isOpened()) 
   {
      std::cerr << "Error opening camera!" << std::endl;
      return -1;
   }

   // FW = static_cast<int>(cap.get(CAP_PROP_FRAME_WIDTH));
   // FH = static_cast<int>(cap.get(CAP_PROP_FRAME_HEIGHT));
    
    cv::Mat img, frame;
    cap >> frame;

    // Select a region to track using a bounding box
    cv::Rect2d roi = cv::selectROI("Select ROI", frame, false, false);
    cv::namedWindow("Display Window");
    setMouseCallback("Display Window", onMouseClick, NULL);
    
    // Initialize the MOSSE tracker
    Ptr<legacy::TrackerMOSSE> tracker = cv::legacy::TrackerMOSSE::create();
    tracker->init(frame, roi);

   double fps = 0;
   
    while (true) 
    {
        cap>>frame;
        lastTime = chrono::high_resolution_clock::now();
        
        int key = cv::waitKey(1); // Change the delay to control frame rate
        if (key == 27) // 'Esc' key
            break;

        frame_text(frame, fps, radius, color, thickness);
        //draw_circle(frame, radius, color, thickness);
        camera_fov(frame, key);
        fps = cal_fps(frame, fps);

        if (!mouseClick.inside(Rect((radius), (radius), (FW - (2 * radius)), (FH - (2 * radius))))) //checks if click in bound
        {clear_screen(frame);}

        bool success = tracker->update(frame, roi);

        if (success)
        {
            cv::rectangle(frame, roi, cv::Scalar(0, 255, 0), 2);
            lock_status = "LOCKED";
        }
        
        else {lock_status = "TRACKING FAILED";}

        cv::imshow("Display Window", frame);
    }

    cout<<"releasing cap";
    
    cap.release();
    return 0;
}