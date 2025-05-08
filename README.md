# Project-WALLFLOWER

Usage for building:
1. Make a folder called "templates" and put both HTML files in it. (login & display_image).
2. Make a folder called "static" and put both image files into it. (UploadImageButton & FindRouteButton)
3. Then, run testSite.py. (MAKE SURE FLASK AND PYTHON ARE INSTALLED ON YOUR MACHINE.)
4. If you've done the previous steps correctly, it should open the website on a local address.
5. Click upload image to upload image of a rock wall, pick a difficulty, then click find a route to run the program.
6. Viola! You have a route picked out for you based on your image and difficulty!
7. If you would like to retry, please just hit the back button on your browser.

To run website:
1. Download website.zip file
2. Uncompress
3. Run testSite.py

Run with clear image such as the included kilter.jpg for best results

Known bugs: 
  Takes very long to run for higher grades. This is due to issues with pathfinder.cpp. It has memory issues, likely caused by the removal of holds but we were unable to pinpoint and fix these issues.
  Our workaround for this was to run it until there was no error.

Team dynamics:
  Things discussed in team meetings included talking about progress on individual tasks, deciding on tasks, etc. We did not too much for this project. 
  Drew Berard worked on the python code that read in an image and output a list of holds. He also contributed to the creation of testsite.py.
  Grantham Jennings built the skeleton for the website and got the buttons and such working. He also wrote the file input and file deletion.
  Jubin Vijayakumar wrote the pathfinder.cpp file that reads in holds and, through Dijkstra's algorithm, determines routes.
  Justin Wiseman had cosmetic changes to the HTML file.

Our time log:
https://docs.google.com/spreadsheets/d/1CKQqe18hkLn9kmRKhLO-F_sopTWbIG8bWeyxEAbQiQk/edit?usp=sharing
