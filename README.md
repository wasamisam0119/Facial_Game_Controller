# G52GRP Project Group 27

## Extra project resources
* Trello Board : https://trello.com/gameface5
* Google Drive document storage: https://drive.google.com/drive/folders/0B5RShekiv6-4czBlVEpybmN0XzQ?usp=sharing


## Project Overview  
Create a controller that uses facial tracking in use for different video games.
The controller will be made to interact with DosBox in order to play retro games such as pacman, mortal kombat etc.

## Requiements  

* C++ [Boost Library](https://sourceforge.net/projects/boost/files/boost/1.62.0/)  version 1.6.2     
* Cmake at least 2.8.4     
* C++ [Dlib](http://dlib.net/) version 19.2   
* Xquartz
* Python3.4 or higher
* xdotool 3.20141006.1 or higher

## Installation
### 1.Face Landmark Detection
#### Compile Dlib
* Download a copy from github

> git clone https://github.com/davisking/dlib.git

* Build examples ( macOS / Linux )

> cd dlib/examples

> mkdir build

> cd build

> cmake .. 

> cmake --build . --config Release

#### Run Dlib’s facial landmark detector
> cd examples/build/

> //Download the face landmark model  from http://dlib.net/files/shape_predictor_68_face_landmarks.dat.bz2

> ./webcam_face_pose_ex

#### Run our facial landmark detector
> change the dlib/examples/webcam_face_pose_ex  to our G52GRP_TEAM27_2016_DANGEROUS_DUCKS/FaceDetect-dlib/webcam_face_pose_ex.cpp

> //Rebuild the examples

> cmake --build . --config Release

Now we can run a speed-up real time face landmark, get the coordinates of 68 points and get the frames per second (fps) .

### 2. Game Emulator

