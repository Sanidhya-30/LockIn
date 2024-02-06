#include <opencv2/opencv.hpp>
#include <iostream>
#include <string.h>

using namespace cv;
using namespace std;


// ----------------------------- USER DEFINED VARIABLES  -------------------------

int radius = 50;
int font = FONT_HERSHEY_SIMPLEX;
cv::Scalar color_text = cv::Scalar(0, 255, 0);     //green
cv::Scalar color_circle = cv::Scalar(0, 255, 255); //yellow
int thickness = 1;
int FH = 480 ;  // frame heigth
int FW = 640 ;  // frame width
int box_size = 50;

// ----------------------------- SOME CONSTANTS  -------------------------

String lock_status = "LOOKING";
Point mouseClick  = Point((FW/2), (FH/2));
Point Cursor      = Point((FW/2), (FH/2));




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



// ----------------------------- CLEAR CIRCLE  -------------------------

void clear_screen(cv::Mat frame)
{
   //clear screen
   frame = Mat::zeros(frame.size(), frame.type());
   cv::imshow("Display Window", frame);
   mouseClick = Point((FW/2), (FH/2));;
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

void frame_text(cv::Mat frame, double fps, int radius, cv::Scalar color_text, int thickness)
{
   //locked:status
   putText(frame, lock_status, Point(((FW/2)-60),(((FH))-50)), font, 1, color_text, (thickness+1));

   //XY corinates
   putText(frame, "X: " + to_string(Cursor.x) + " Y: " + to_string(Cursor.y), Point((FH - 60), 30), font, 1, color_text, (thickness + 1));


   //line
   Point p1(mouseClick.x, 0), p2(FW, mouseClick.y), p3(mouseClick.x, FH), p4(0, mouseClick.y);
   line(frame, p1, p3, Scalar(255, 0, 0), thickness, LINE_4); 
   line(frame, p2, p4, Scalar(255, 0, 0), thickness, LINE_4); 

   //ROI box
   cv::rectangle(frame, cv::Rect(Point((radius), (radius)), Point((FW-(radius)), (FH-(radius))) ), (200, 0, 0), 1);

   return;
}



// ----------------------------- CALCULATE FPS  -------------------------

double cal_fps(cv::Mat frame, double fps)
{
   static auto lastTime = chrono::high_resolution_clock::now();
   auto currentTime = chrono::high_resolution_clock::now();
   double elapsedSeconds = chrono::duration_cast<chrono::duration<double>>(currentTime - lastTime).count();
   
   // Update FPS using weighted average
   fps = (0.85 / elapsedSeconds) + ((1 - 0.85) * fps);
   lastTime = currentTime;

   //fps
   putText(frame, "FPS: " + to_string(static_cast<int>(fps)), Point(10, 30), font, 1, color_text, (thickness+1));
   
   return fps;
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

   // FW = static_cast<int>(cap.get(CAP_PROP_FRAME_WIDTH));
   // FH = static_cast<int>(cap.get(CAP_PROP_FRAME_HEIGHT));
   cout<<FW<<" "<<FH;

   cv::namedWindow("Display Window");
   setMouseCallback("Display Window", onMouseClick, NULL);

   double fps = 0;
   
   while (true) 
   {
      cv::Mat frame;
      cap >> frame;

      fps = cal_fps(frame, fps);
      frame_text(frame, fps, radius, color_text, thickness);
      draw_circle(frame, radius, color_circle, thickness);


      if (!mouseClick.inside(Rect((radius), (radius), (FW-(2*radius)), (FH-(2*radius))))) //checks if click in bound
      {
         clear_screen(frame);
         
      }

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
