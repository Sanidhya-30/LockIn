import cv2
import numpy as np

class DistanceTracker:
    def __init__(self, focal_length=1000, width=16.5, max_memory_length=20):
        self.focal_length = focal_length
        self.width = width
        self.max_memory_length = max_memory_length
        self.dist_memory = []
        self.frame_counter = 0

    def calculate_distance(self, rectangle):
        pixels = rectangle[1][0]
        dist = round((self.width * self.focal_length) / pixels, 2)
        return dist

    def update_distance_memory(self, dist):
        if len(self.dist_memory) > 0:
            # Only update if the change is greater than 5cm
            if abs(dist - self.dist_memory[-1]) > 5:
                self.dist_memory.append(dist)
        else:
            self.dist_memory.append(dist)

        if len(self.dist_memory) > self.max_memory_length:
            self.dist_memory = self.dist_memory[-self.max_memory_length:]

    def get_average_distance(self):
        if self.dist_memory:
            return round(sum(self.dist_memory) / len(self.dist_memory), 1)
        else:
            return 0

def main():
    # seperate_color()
    distance_tracker = DistanceTracker()

    cap = cv2.VideoCapture(0)

    while True:
        ret, img = cap.read()
        process(img, distance_tracker)
        cv2.imshow('frame ', img)

        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

    cv2.destroyAllWindows()


## PROCESS IS THE
def process(img, distance_tracker):
    hsv_img = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)
    lower = np.array([50, 141, 175])
    upper = np.array([108, 255, 255])
    mask = cv2.inRange(hsv_img, lower, upper)
    kernel = np.ones((11, 11), 'uint8')
    d_img = cv2.morphologyEx(mask, cv2.MORPH_OPEN, kernel, iterations=5)

    cont, hei = cv2.findContours(d_img, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
    cont = sorted(cont, key=cv2.contourArea, reverse=True)[:1]

    for cnt in cont:
        if 100 < cv2.contourArea(cnt) < 306000:
            rect = cv2.minAreaRect(cnt)
            box = np.int0(cv2.boxPoints(rect))
            cv2.drawContours(img, [box], -1, (255, 0, 0), 3)

            distance = distance_tracker.calculate_distance(rect)

            # Calculate distance every 5 frames
            if distance_tracker.frame_counter % 5 == 0:
                distance_tracker.update_distance_memory(distance)

            avg_distance = distance_tracker.get_average_distance()
            img = cv2.putText(img, f'Distance: {avg_distance} (cm)', (10, 50),
                              cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 255, 0), 2, cv2.LINE_AA)

    distance_tracker.frame_counter += 1
    cv2.imshow('morphed', d_img)


## HSV Masking is the
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



# pata nhi kya zarurat thi iski - 1
def empty(a):
    pass

# pata nhi kya zarurat thi iski - 2
def resize_final_img(x,y,*argv):
    images  = cv2.resize(argv[0], (x, y))
    for i in argv[1:]:
        resize = cv2.resize(i, (x, y))
        images = np.concatenate((images,resize),axis = 1)
    return images

if __name__ == "__main__":
    main()
