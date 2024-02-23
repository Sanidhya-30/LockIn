import cv2
import numpy as np

#Define object specific variables  
dist = 0
focal = 450
pixels = 30
width = 4

def getdatD(rect, frame):

    #find no of pixels covered
    pixels = rect[1][0]
    print(pixels)
    #calculate distance
    dist = (width*focal)/pixels

    return dist

# Main function
def main():
    # vs = cv2.VideoCapture(0)  # Webcam stream
    vs = cv2.VideoCapture("/home/ubuntu/LockIn/videos/car.mp4")

    # Ensure the webcam is initialized
    if not vs.isOpened():
        print("webcam kharaab")
        return

    (grabbed,frame) = vs.read()

    # Select ROI
    bbox = cv2.selectROI(frame)

    # Initialize trackers with different algorithms
    tracker = cv2.TrackerMedianFlow_create()
    ok = tracker.init(frame, bbox)

    while True:

        timer = cv2.getTickCount()

        frame = vs.read()
        
        if frame is None:
            print("Webcam nava chakko")
            break

        ok, bbox = tracker.update(frame)

        if ok:
            # Tracking success
            p1 = (int(bbox[0]), int(bbox[1]))
            p2 = (int(bbox[0] + bbox[2]), int(bbox[1] + bbox[3]))
            cv2.rectangle(frame, p1, p2, (255,0,0), 2, 1)
        else :
            # Tracking failure
            cv2.putText(frame, "Tracking failure detected", (100,80), cv2.FONT_HERSHEY_SIMPLEX, 0.75,(0,0,255),2)
     
        # Display result
        D = getdatD(bbox, frame)
        cv2.putText(frame, str(float(D)) + " m", (100,20), cv2.FONT_HERSHEY_SIMPLEX, 0.75, (50,170,50),2)
      
        # Exit if ESC pressed
        k = cv2.waitKey(1) & 0xff
        if k == 27 : break

        fps = cv2.getTickFrequency() / (cv2.getTickCount() - timer)
        # Display FPS on frame
        cv2.putText(frame, "FPS : " + str(int(fps)), (100,50), cv2.FONT_HERSHEY_SIMPLEX, 0.75, (50,170,50), 2)
        cv2.imshow("Mossi D", frame)

    cv2.destroyAllWindows()
    vs.stop()

if __name__ == "__main__":
    main()
