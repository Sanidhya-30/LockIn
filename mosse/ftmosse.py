import cv2
from imutils.video import VideoStream
import imutils
import numpy as np

def select_roi(frame):
    # Display the frame and ask the user to select a region of interest (ROI)
    rect = cv2.selectROI(frame, fromCenter=False, showCrosshair=True)
    cv2.destroyAllWindows()  # Close the window after ROI selection
    return rect

def initialize_tracker(frame, init_bbox, tracker_type):
    # Initialize the tracker based on the provided type
    if tracker_type == 'MOSSE':
        tracker = cv2.TrackerMOSSE_create()
    elif tracker_type == 'SIFT':
        sift = cv2.SIFT_create()
        kp, des = sift.detectAndCompute(frame, None)
        tracker = cv2.Tracker_create("SIFT")
        tracker.init(frame, (int(init_bbox[0]), int(init_bbox[1]), int(init_bbox[2]), int(init_bbox[3])))
    elif tracker_type == 'SURF':
        surf = cv2.xfeatures2d.SURF_create()
        kp, des = surf.detectAndCompute(frame, None)
        tracker = cv2.Tracker_create("SURF")
        tracker.init(frame, (int(init_bbox[0]), int(init_bbox[1]), int(init_bbox[2]), int(init_bbox[3])))
    elif tracker_type == 'ORB':
        orb = cv2.ORB_create()
        kp, des = orb.detectAndCompute(frame, None)
        tracker = cv2.Tracker_create("ORB")
        tracker.init(frame, (int(init_bbox[0]), int(init_bbox[1]), int(init_bbox[2]), int(init_bbox[3])))
    elif tracker_type == 'AKAZE':
        akaze = cv2.AKAZE_create()
        kp, des = akaze.detectAndCompute(frame, None)
        tracker = cv2.Tracker_create("KCF")
        tracker.init(frame, (int(init_bbox[0]), int(init_bbox[1]), int(init_bbox[2]), int(init_bbox[3])))
    else:
        raise ValueError("Invalid tracker type")

    return tracker

def update_tracker(frame, tracker):
    # Update the tracker using the new frame
    success, bbox = tracker.update(frame)
    return success, bbox

def blob_detection(frame):
    # Perform blob detection using SimpleBlobDetector
    params = cv2.SimpleBlobDetector_Params()

    # Set parameters (you can adjust these based on your requirements)
    params.minThreshold = 10
    params.maxThreshold = 200
    params.filterByArea = True
    params.minArea = 100
    params.filterByCircularity = True
    params.minCircularity = 0.8

    detector = cv2.SimpleBlobDetector_create(params)
    keypoints = detector.detect(frame)

    # Draw blobs on the frame
    frame_with_blobs = cv2.drawKeypoints(frame, keypoints, np.array([]), (0, 0, 255),
                                         cv2.DRAW_MATCHES_FLAGS_DRAW_RICH_KEYPOINTS)

    return frame_with_blobs

# Main function
def main():
    vs = cv2.VideoCapture(0)  # Webcam stream

    # Ensure the webcam is initialized
    if not vs.isOpened():
        print("Error: Unable to access the webcam.")
        return

    (grabbed,frame) = vs.read()
    if frame is None:
        print("Error: Unable to read the first frame from the webcam.")
        return

    # Select ROI
    init_bbox = select_roi(frame)

    # Initialize trackers with different algorithms
    tracker_mosse = initialize_tracker(frame, init_bbox, 'MOSSE')
    tracker_sift = initialize_tracker(frame, init_bbox, 'SIFT')
    tracker_surf = initialize_tracker(frame, init_bbox, 'SURF')
    tracker_orb = initialize_tracker(frame, init_bbox, 'ORB')
    tracker_akaze = initialize_tracker(frame, init_bbox, 'AKAZE')

    while True:
        frame = vs.read()
        if frame is None:
            print("Error: Unable to read a frame from the webcam.")
            break

        frame = imutils.resize(frame, width=800)

        # Update trackers
        success_mosse, bbox_mosse = update_tracker(frame, tracker_mosse)
        success_sift, bbox_sift = update_tracker(frame, tracker_sift)
        success_surf, bbox_surf = update_tracker(frame, tracker_surf)
        success_orb, bbox_orb = update_tracker(frame, tracker_orb)
        success_akaze, bbox_akaze = update_tracker(frame, tracker_akaze)

        # Display the output side by side
        frame_output = np.hstack((frame, frame))

        if success_mosse:
            (x, y, w, h) = [int(i) for i in bbox_mosse]
            cv2.rectangle(frame_output, (x, y), (x + w, y + h), (0, 255, 0), 2)

        if success_sift:
            (x, y, w, h) = [int(i) for i in bbox_sift]
            cv2.rectangle(frame_output, (x + frame.shape[1], y), (x + w + frame.shape[1], y + h), (0, 0, 255), 2)

        if success_surf:
            (x, y, w, h) = [int(i) for i in bbox_surf]
            cv2.rectangle(frame_output, (x, y), (x + w, y + h), (255, 0, 0), 2)

        if success_orb:
            (x, y, w, h) = [int(i) for i in bbox_orb]
            cv2.rectangle(frame_output, (x + frame.shape[1], y), (x + w + frame.shape[1], y + h), (255, 255, 0), 2)

        if success_akaze:
            (x, y, w, h) = [int(i) for i in bbox_akaze]
            cv2.rectangle(frame_output, (x, y), (x + w, y + h), (0, 255, 255), 2)

        cv2.imshow("Object Tracking with Different Algorithms", frame_output)

        key = cv2.waitKey(1) & 0xFF
        if key == ord("q"):
            break

    cv2.destroyAllWindows()
    vs.stop()

if __name__ == "__main__":
    main()
