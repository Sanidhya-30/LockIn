import cv2
import numpy as np

#Variables
dist = 0
focal = 900
pixels = 30
width = 16.5
kernel = np.ones((11,11),'uint8')
font = cv2.FONT_HERSHEY_SIMPLEX 
org = (10,50)  
fontScale = 0.6 
color = (0, 255, 0) 
thickness = 0.5
lower = np.empty(3)
upper = np.empty(3)
dist_memory = []


# pata nhi kya zarurat thi iski - 1
def empty(a):
    pass


def getdatD(rectangle, frame):
    global dist_memory

    pixels = rectangle[1][0]
    dist = round((width * focal) / pixels, 2)

    # Add the current distance to the memory
    dist_memory.append(dist)

    # Keep only the last 10 readings in memory
    if len(dist_memory) > 20:
        dist_memory = dist_memory[-20:]

    # Calculate the average of the last 10 readings
    avg_dist = round(sum(dist_memory) / len(dist_memory), 1)

    frame = cv2.putText(frame, f'Distance: {avg_dist} (cm)', org, font, 1, color, 2, cv2.LINE_AA)
    return frame


# pata nhi kya zarurat thi iski - 2
def resize_final_img(x,y,*argv):
    images  = cv2.resize(argv[0], (x, y))
    for i in argv[1:]:
        resize = cv2.resize(i, (x, y))
        images = np.concatenate((images,resize),axis = 1)
    return images


# HSV alag karne ke liye
def process(img):
    hsv_img = cv2.cvtColor(img,cv2.COLOR_BGR2HSV)

    #predefined mask for green colour detection
    lower = np.array([85, 27, 157])
    upper = np.array([120, 255, 255])
    mask = cv2.inRange(hsv_img, lower, upper)

    #Remove Extra garbage from image
    d_img = cv2.morphologyEx(mask, cv2.MORPH_OPEN, kernel,iterations = 5)

    #find the histogram
    cont,hei = cv2.findContours(d_img,cv2.RETR_EXTERNAL,cv2.CHAIN_APPROX_SIMPLE)
    cont = sorted(cont, key = cv2.contourArea, reverse = True)[:1]
    
    for cnt in cont:
        #check for contour area
        if (cv2.contourArea(cnt)>100 and cv2.contourArea(cnt)<306000):
            #Draw a rectange on the contour
            rect = cv2.minAreaRect(cnt)
            box = cv2.boxPoints(rect) 
            box = np.int0(box)
            cv2.drawContours(img,[box], -1,(255,0,0),3)
            img = getdatD(rect,img)
    

# color nikaalne ke liye
def seperate_color():
    cap = cv2.VideoCapture(0)
    cv2.namedWindow("HSV")
    cv2.resizeWindow("HSV", 300, 300)
    cv2.createTrackbar("HUE Min", "HSV", 0, 179, empty)
    cv2.createTrackbar("HUE Max", "HSV", 179, 179, empty)
    cv2.createTrackbar("SAT Min", "HSV", 0, 255, empty)
    cv2.createTrackbar("SAT Max", "HSV", 255, 255, empty)
    cv2.createTrackbar("VALUE Min", "HSV", 0, 255, empty)
    cv2.createTrackbar("VALUE Max", "HSV", 255, 255, empty)

    while True:
        ret, img = cap.read()
        h_min = cv2.getTrackbarPos("HUE Min", "HSV")
        h_max = cv2.getTrackbarPos("HUE Max", "HSV")
        s_min = cv2.getTrackbarPos("SAT Min", "HSV")
        s_max = cv2.getTrackbarPos("SAT Max", "HSV")
        v_min = cv2.getTrackbarPos("VALUE Min", "HSV")
        v_max = cv2.getTrackbarPos("VALUE Max", "HSV")

        hsv_img = cv2.cvtColor(img,cv2.COLOR_BGR2HSV)

        lower = np.array([h_min, s_min, v_min])
        upper = np.array([h_max, s_max, v_max])
        mask = cv2.inRange(hsv_img, lower, upper)
        kernel = np.ones((3,3),'uint8')

        d_img = cv2.morphologyEx(mask, cv2.MORPH_OPEN, kernel,iterations = 5)

        final_img = resize_final_img(300,300, mask, d_img)
        # final_img = np.concatenate((mask,d_img,e_img),axis =1)
        cv2.imshow('F',final_img)
        if cv2.waitKey(1) & 0xFF == ord('q'):
            lower = np.array([h_min, s_min, v_min])
            upper = np.array([h_max, s_max, v_max])
            break

    cv2.destroyAllWindows()


# Main function
def main():
    # seperate_color()
    print (lower)
    print (upper)

    cap = cv2.VideoCapture(0)

    while True:
        ret, img = cap.read()
        process(img)
        cv2.imshow('frame ', img)

        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

    cv2.destroyAllWindows()


if __name__ == "__main__":
    main()
