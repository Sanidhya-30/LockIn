import numpy as np
import cv2
import yaml

# termination criteria
criteria = (cv2.TERM_CRITERIA_EPS + cv2.TERM_CRITERIA_MAX_ITER, 30, 0.001)
# Arrays to store object points and image points from all the images.
objpoints = [] # 3d point in real world space
imgpoints = [] # 2d points in image plane.


# prepare object points, like (0,0,0), (1,0,0), (2,0,0) ....,(6,5,0)
objp = np.zeros((6*7,3), np.float32)
objp[:,:2] = np.mgrid[0:7,0:6].T.reshape(-1,2)


def capture():
    cap = cv2.VideoCapture(0)
    found = 0
    while(found < 15):  # Here, 10 can be changed to whatever number you like to choose
        ret, img = cap.read() # Capture frame-by-frame
        gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

        # Find the chess board corners
        ret, corners = cv2.findChessboardCorners(gray, (7,6),None)

        # If found, add object points, image points (after refining them)
        if ret == True:
            objpoints.append(objp)   # Certainly, every loop objp is the same, in 3D.
            corners2 = cv2.cornerSubPix(gray,corners,(11,11),(-1,-1),criteria)
            imgpoints.append(corners2)

            # Draw and display the corners
            img = cv2.drawChessboardCorners(img, (7,6), corners2, ret)
            found += 1
            print(found)

        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

        cv2.imshow('img', img)
        cv2.waitKey(10)

    # When everything done, release the capture
    cap.release()
    cv2.destroyAllWindows()


def undistort(img, mtx, dist):
    h, w = img.shape[:2]
    newcameramtx, roi = cv2.getOptimalNewCameraMatrix(mtx, dist, (w, h), 1, (w, h))

    # undistort
    dst = cv2.undistort(img, mtx, dist, None, newcameramtx)
    # crop the image
    x, y, w, h = roi
    dst = dst[y:y + h, x:x + w]
    return dst


def store(mtx, dist):
    # It's very important to transform the matrix to list.

    data = {'camera_matrix': np.asarray(mtx).tolist(), 'dist_coeff': np.asarray(dist).tolist()}

    with open("calibration.yaml", "w") as f:
        yaml.dump(data, f)


def load():
    with open('calibration.yaml') as f:
        loadeddict = yaml.load(f)

    mtxloaded = loadeddict.get('camera_matrix')
    distloaded = loadeddict.get('dist_coeff')
    return mtxloaded, distloaded


def main():
    capture()
    ret, mtx, dist, rvecs, tvecs = cv2.calibrateCamera(objpoints, imgpoints, (640,480), None, None)
    store(mtx, dist)

    cap = cv2.VideoCapture(0)

    while True:
        ret, img = cap.read()
        undistorted_img = undistort(img, mtx, dist)
        cv2.imshow('Undistorted Image', undistorted_img)

        key = cv2.waitKey(1)
        if key & 0xFF == ord('q'):
            break

    cap.release()  # Release the video capture
    cv2.destroyAllWindows()

    mean_error = 0
    for i in range(len(objpoints)):
        imgpoints2, _ = cv2.projectPoints(objpoints[i], rvecs[i], tvecs[i], mtx, dist)
        error = cv2.norm(imgpoints[i], imgpoints2, cv2.NORM_L2) / len(imgpoints2)
        mean_error += error
    print("Total error: {}".format(mean_error / len(objpoints)))

if __name__ == "__main__":
    main()