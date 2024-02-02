#include <opencv2/opencv.hpp>
#include <iostream>
#include<string.h>

using namespace cv;
using namespace std;


// ----------------------------- SOME CONSTANTS  -------------------------

Point mouseClick;
int radius = 2;
int font = FONT_HERSHEY_SIMPLEX;
cv::Scalar color_text = cv::Scalar(0, 255, 0); //green
cv::Scalar color_circle = cv::Scalar(0, 255, 255); //yellow
int thickness = 3;
String lock_status = "LOCKED";
int frame_heigth = 480;
int frame_width = 640;

// ----------------------------- MOUSECLICK CALLBACK  -------------------------

void onMouseClick(int event, int x, int y, int flags, void* userdata) 
{
    if (event == EVENT_LBUTTONDOWN) 
    {
        mouseClick = Point(x, y);
      //   lock_status = "LOCKED";
    }
   
   return;
}



// ----------------------------- DRAW CIRCLE  -------------------------

void draw_circle(cv::Mat frame, int radius, cv::Scalar color_circle, int thickness)
{
   cv::circle(frame, mouseClick, radius, color_circle, thickness);
   return;
}



// ----------------------------- PUT TEXT IN FRAME  -------------------------

void frame_text(cv::Mat frame, double fps, int radius, cv::Scalar color_text, int thickness)
{
   //locked
   putText(frame, lock_status, Point((frame_width/2), (frame_heigth/2)), font, 1, color_text, thickness);

   //line
   Point p1(mouseClick.x, 0), p2(frame_width, mouseClick.y), p3(mouseClick.x, frame_heigth), p4(0, mouseClick.y);
   line(frame, p1, p3, Scalar(255, 0, 0), thickness, LINE_4); 
   line(frame, p2, p4, Scalar(255, 0, 0), thickness, LINE_4); 
   return;
}



// ----------------------------- CALCULATE FPS  -------------------------

double cal_fps(cv::Mat frame, double fps)
{
   static auto lastTime = chrono::high_resolution_clock::now();
   auto currentTime = chrono::high_resolution_clock::now();
   double elapsedSeconds = chrono::duration_cast<chrono::duration<double>>(currentTime - lastTime).count();
   
   // Update FPS using weighted average
   double currentFPS = (0.85 / elapsedSeconds) + ((1 - 0.85) * currentFPS);
   lastTime = currentTime;

   //fps
   putText(frame, "FPS: " + to_string(static_cast<int>(fps)), Point(10, 30), font, 1, color_text, thickness);
   
   return currentFPS;
}




// ----------------------------- MAIN -------------------------

int main() 
{
   cv::VideoCapture cap(-1);
   
   if (!cap.isOpened()) 
   {
      std::cerr << "Error opening camera!" << std::endl;
      return -1;
   }

   int frame_width = static_cast<int>(cap.get(CAP_PROP_FRAME_WIDTH));
   int frame_height = static_cast<int>(cap.get(CAP_PROP_FRAME_HEIGHT));

   cv::namedWindow("Display Window");
   setMouseCallback("Display Window", onMouseClick, NULL);

   double fps = 0;
   
   while (true) 
   {
      cv::Mat frame;
      cap >> frame;

      fps = cal_fps(frame, fps);

      cv::imshow("Display Window", frame);

        if (!mouseClick.inside(Rect(0, 0, (frame.cols-20), (frame.rows-20)))) //checks if click in bound
        {
            frame_text(frame, fps, radius, color_text, thickness);
            draw_circle(frame, radius, color_circle, thickness);
        }

      // Check for user input to exit the loop
      if (cv::waitKey(1) == 27) // 'Esc' key
         break;
   }

   // Release the camera and close the window after exiting the loop
   cap.release();
   cv::destroyAllWindows();

   return 0;
}
