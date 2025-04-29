import cv2
import numpy as np

#finds the holds in the given image
def detect_holds(image_path):

    #load image
    image = cv2.imread(image_path)
    gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
    
    #applies blur (adjust for bigger or smaller for better detection)
    blurred = cv2.GaussianBlur(gray, (7, 7), 0)
    
    #finds countours using edges found using Canny Edge Detection
    edges = cv2.Canny(blurred, 15, 150)
    contours, _ = cv2.findContours(edges, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
    
    holds = []
    for contour in contours:
        area = cv2.contourArea(contour)
        if 40 < area:
            x, y, w, h = cv2.boundingRect(contour)
            holds.append({"x": x, "y": y, "w": w, "h": h, "area": area})
            
            #draws box on the image
            cv2.rectangle(image, (x, y), (x + w, y + h), (255, 0, 0), 2)
    
    #output
    cv2.imshow("Detected Holds", image)
    cv2.waitKey(0)
    cv2.destroyAllWindows()
    
    return holds

#averages color and makes sure it is within the normal color ranges of climbing holds
def color_check(holds, image_path):
    
    #load image and blur it
    image = cv2.imread(image_path)
    hsv_image = cv2.cvtColor(image, cv2.COLOR_BGR2HSV)

    saturation_factor = 2  # Adjust this value to control the saturation increase
    hsv_image[..., 1] = np.clip(hsv_image[..., 1] * saturation_factor, 0, 255)

    saturated_image = cv2.cvtColor(hsv_image, cv2.COLOR_HSV2BGR)
    
    '''
    for n in range(len(holds)):

        blurred = cv2.GaussianBlur(image, (11, 11), 0)

        x = holds[n]["x"]
        y = holds[n]["y"]
        w = holds[n]["w"]
        h = holds[n]["h"]

        cv2.rectangle(blurred, (x, y), (x + w, y + h), (255, 0, 0), 2)

        average_cpr = cv2.mean(blurred[y:y+h, x:x+w])
        print("Average BGR: " + (str)(average_cpr))

        cv2.imshow("Blurred", blurred)
        cv2.waitKey(0)
        cv2.destroyAllWindows()

    return holds
'''

    blurred = cv2.GaussianBlur(saturated_image, (11, 11), 0)

    n = 3

    x = holds[n]["x"]
    y = holds[n]["y"]
    w = holds[n]["w"]
    h = holds[n]["h"]

    cv2.rectangle(blurred, (x, y), (x + w, y + h), (255, 0, 0), 2)

    average_cpr = cv2.mean(blurred[y:y+h, x:x+w])
    print("Average BGR: " + (str)(average_cpr))

    cv2.imshow("Blurred", blurred)
    cv2.waitKey(0)
    cv2.destroyAllWindows()

    return holds
    
#allows user to add and remove holds given the output of detect_holds
def user_input(holds):
    return holds


#test to see if saturating the image gives better output
def detect_saturated_holds(image_path):
    
    #load image
    image = cv2.imread(image_path)
    hsv_image = cv2.cvtColor(image, cv2.COLOR_BGR2HSV)

    saturation_factor = 3  # Adjust this value to control the saturation increase
    hsv_image[..., 1] = np.clip(hsv_image[..., 1] * saturation_factor, 0, 255)

    saturated_image = cv2.cvtColor(hsv_image, cv2.COLOR_HSV2BGR)
    cv2.imshow("Saturated Image", saturated_image)
    cv2.waitKey(0)
    cv2.destroyAllWindows()

    gray = cv2.cvtColor(saturated_image, cv2.COLOR_BGR2GRAY)
    cv2.imshow("Graysacle Saturation", gray)
    cv2.waitKey(0)
    cv2.destroyAllWindows()
    
    #applies blur (adjust for bigger or smaller for better detection)
    blurred = cv2.GaussianBlur(gray, (7, 7), 0)
    cv2.imshow("Blurred Image", blurred)
    cv2.waitKey(0)
    cv2.destroyAllWindows()
    
    #finds countours using edges found using Canny Edge Detection
    edges = cv2.Canny(blurred, 15, 150)
    contours, _ = cv2.findContours(edges, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
    
    holds = []
    for contour in contours:
        area = cv2.contourArea(contour)
        if 40 < area < 100:
            x, y, w, h = cv2.boundingRect(contour)
            line = ""
            line += (str)(x)
            line += " " + (str)(y)
            line += " " + (str)(w)
            line += " " + (str)(h)
            line += " " + (str)(area)
            holds.append(line)
            
            #draws box on the image
            cv2.rectangle(image, (x, y), (x + w, y + h), (255, 0, 0), 2)
    
    #output
    cv2.imshow("Detected Holds", image)
    cv2.waitKey(0)
    cv2.destroyAllWindows()
    
    return holds

image_path = r"C:\Users\fived\Downloads\Kilterboard-7x10-full-ride.jpg"

holds = detect_holds(image_path)
#testing if saturated holds is better
#holds = detect_saturated_holds(image_path)

#color_check(holds, image_path)
#holds = user_input(holds)

with open("holds_list", "w") as file:
    for hold in holds:
        file.write(str(hold) + "\n")