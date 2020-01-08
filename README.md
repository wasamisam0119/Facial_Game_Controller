# G52GRP Project

## Project Overview  
Create a controller that uses facial tracking in use for different video games.

The controller will be made to interact with DosBox in order to play retro games such as pacman, mortal kombat etc.

### Requirements
- Windows
- C++ Boost Library version 1.6.2 or later
  - Available [here](http://www.boost.org)
  - Or at `www.boost.org`
- CMake version 2.8.4 or later
  - Available [here](http://www.cmake.org)
  - Or at `www.cmake.org`
- C++ Dlib version 19.2 or later
  - Available [here](http://www.dlib.net)
  - Or at `www.dlib.net`
- A C++11 compiler (Visual Studio is recommended)
  - Available [here](http://www.visualstudio.com/free-developer-offers)
  - Or at `www.visualstudio.com/free-developer-offers`
- OpenCV
  - Available [here](http://www.opencv.org)
  - Or at `www.opencv.org`

### Installation
  1. Install CMake.
  2. Download the OpenCV self-extracting archive and place it in a location you can access.
  3. Run the CMake GUI and build the OpenCV library.
  4. Select the source code of OpenCV, which is in the `OpenCV/sources/` folder.
  5. Build the binaries in the `OpenCV/build` folder
  6. Press 'Configure', and wait for it to complete
  7. Press 'Generate' to finish the installation
  8. Once OpenCV is built, download the provided `dlib` folder
  9. Build the folder using CMake
  10. For the source code, select the `dlib/Face` folder
  11. Build the binaries in that folder
  12. Press 'Configure' and wait for completion  
       If you get an error saying that OpenCV is not found, find the `OpenCV_DIR` variable and change the path to
       point to your OpenCV build folder
  13. Generate the code
  14. Once the code is generated, press 'Open Project'
  15. Run the Visual Studio project file from the \verb|dlib/build| folder and build the project inside Visual Studio
- Change mode to 'Release' using the build menu

### Use
  To use GameFace, simply double-click the `webcam_face_pose_ex.exe` file and you're good to go!
  Currently the controls are as follow:
- Head up: w
- Head down: s
    - Look left: a
    - Look right: d
    - Roll left: left-click (mouse 0)
    - Roll right: right-click (mouse 1)

### Facial Recgnization Speedup Testing Result

Original Speed(fps) using dlib is usually between 5~8, now it is increse to more than 20 when playing video.

![example1](https://github.com/wasamisam0119/Facial_Game_Controller/blob/master/test_capture.png) 

### Demo

![demo](src = "https://github.com/wasamisam0119/Facial_Game_Controller/blob/master/demo.jpg" width="640" height="480" align=center) 