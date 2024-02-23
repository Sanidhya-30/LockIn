import cv2
import numpy as np

# Initialize webcam
cap = cv2.VideoCapture("/home/ubuntu/LockIn/videos/helicopter.mp4")  # Use 0 for the default webcam, adjust as needed

# Initialize feature detector and descriptor
orb = cv2.ORB_create()

# Initialize brute-force matcher
bf = cv2.BFMatcher()

# Capture the first frame for reference
ret, first_frame = cap.read()
gray_first_frame = cv2.cvtColor(first_frame, cv2.COLOR_BGR2GRAY)
kp1, des1 = orb.detectAndCompute(gray_first_frame, None)

while True:
    # Capture frame-by-frame
    ret, frame = cap.read()
    
    # Convert the frame to grayscale
    gray_frame = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)

    # Find keypoints and descriptors
    kp, des = orb.detectAndCompute(gray_frame, None)

    # Match descriptors with the reference descriptors
    matches = bf.knnMatch(des1, des, k=2)

    # Apply ratio test to get good matches
    good_matches = []
    for m, n in matches:
        if m.distance < 0.75 * n.distance:
            good_matches.append(m)

    # Draw matches
    img_matches = cv2.drawMatches(first_frame, kp1, frame, kp, good_matches, None, flags=cv2.DrawMatchesFlags_NOT_DRAW_SINGLE_POINTS)

    if len(good_matches) > 4:
        # Extract matched keypoints
        src_pts = np.float32([kp1[m.queryIdx].pt for m in good_matches]).reshape(-1, 1, 2)
        dst_pts = np.float32([kp[m.trainIdx].pt for m in good_matches]).reshape(-1, 1, 2)

        # Use RANSAC to estimate homography
        H, _ = cv2.findHomography(src_pts, dst_pts, cv2.RANSAC, 5.0)

        # Warp frame into the perspective of the reference image
        warped_frame = cv2.warpPerspective(frame, H, (first_frame.shape[1], first_frame.shape[0]))

        # Display results
        cv2.imshow('Matches', img_matches)
        cv2.imshow('Warped Frame', warped_frame)
    else:
        cv2.imshow('Matches', img_matches)

    # Break the loop if 'q' is pressed
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

# Release the webcam and close windows
cap.release()
cv2.destroyAllWindows()
