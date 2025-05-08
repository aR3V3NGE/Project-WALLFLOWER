import cv2
import numpy as np
import subprocess
import os

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
        if 35 < area:
            x, y, w, h = cv2.boundingRect(contour)
            line = ""
            line += (str)(x)
            line += " " + (str)(y)
            line += " " + (str)(w)
            line += " " + (str)(h)
            line += " " + (str)(area)
            holds.append(line)
            
            #draws box on the image
            #cv2.rectangle(image, (x, y), (x + w, y + h), (255, 0, 0), 2)
    '''
    #output
    cv2.imshow("Detected Holds", image)
    cv2.waitKey(0)
    cv2.destroyAllWindows()
    '''
    return holds

#averages color and makes sure it is within the normal color ranges of climbing holds
#def color_check(holds, image_path):

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
            #cv2.rectangle(image, (x, y), (x + w, y + h), (255, 0, 0), 2)
    '''
    #output
    cv2.imshow("Detected Holds", image)
    cv2.waitKey(0)
    cv2.destroyAllWindows()
    '''
    return holds

#write holds to file for route maker file
def write(holds):
    with open("holds_list", "w") as file:
        for hold in holds:
            file.write(str(hold) + "\n")

#dikjstrater
def dikjstrater(holds):
    process = subprocess.Popen(['holds.exe', 'holds_list'], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    stdout, stderr = process.communicate()
    return list(map(int, stdout.decode().strip().split()))

#output
def output(image_path, numbers):
    image = cv2.imread(image_path)

    #draw start
    x, y, w, h = numbers[0:4]
    top_left = (x, y)
    bottom_right = (x + w, y + h)
    cv2.rectangle(image, top_left, bottom_right, color=(0, 255, 0), thickness=2)

    #draw finish
    x, y, w, h = numbers[len(numbers) - 4:len(numbers)]
    top_left = (x, y)
    bottom_right = (x + w, y + h)
    cv2.rectangle(image, top_left, bottom_right, color=(0, 0, 255), thickness=2)

    #draw middle holds
    for i in range(4, len(numbers) - 4, 4):
        x, y, w, h = numbers[i:i+4]
        top_left = (x, y)
        bottom_right = (x + w, y + h)
        cv2.rectangle(image, top_left, bottom_right, color=(255, 0, 0), thickness=2)

    output_path = os.path.join("static/uploads", "holds_output.jpg")
    cv2.imwrite(output_path, image)

    '''
    cv2.imshow("Image with Boxes", image)
    cv2.waitKey(0)
    cv2.destroyAllWindows()
    '''

def main_function(image_path, difficulty):

    holds = detect_holds(image_path)

    #holds = detect_saturated_holds(image_path)

    #color_check(holds, image_path)
    #holds = user_input(holds)

    #writes to file
    write(holds)

    #call route maker
    numbers = dikjstrater(holds)

    output(image_path, numbers)
