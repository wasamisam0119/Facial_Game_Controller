# G52GRP Project Group 27

## Extra project resources
* Trello Board : https://trello.com/gameface5
* Google Drive document storage: https://drive.google.com/drive/folders/0B5RShekiv6-4czBlVEpybmN0XzQ?usp=sharing


## Project Overview  
Create a controller that uses facial tracking in use for different video games.

The controller will be made to interact with DosBox in order to play retro games such as pacman, mortal kombat etc.

## Requiements  

* C++ [Boost Library][1]  version 1.6.2     
* Cmake at least 2.8.4     
* C++ [Dlib][2] version 19.2   
* Xquartz
* Python3.4 or higher
* xdotool 3.20141006.1 or higher

## Installation
### 1. Face Landmark Detection
##### Compile Dlib
-Download a copy from github

    git clone https://github.com/davisking/dlib.git
    
    
-Build examples ( macOS / Linux )

    cd dlib/examples;   cd dlib/examples;   mkdir build;   cd build;

    cmake ..

    cmake --build . --config Release

##### Run Dlib’s facial landmark detector

    cd examples/build/

-Download the face landmark model  from http://dlib.net/files/shape_predictor_68_face_landmarks.dat.bz2
-Uncompress model to build directory

-Run webcam file

    ./webcam_face_pose_ex

##### Run our facial landmark detector
-Change the dlib/examples/webcam_face_pose_ex  to our G52GRP_TEAM27_2016_DANGEROUS_DUCKS/FaceDetect-dlib/webcam_face_pose_ex.cpp

-Rebuild the examples and run

    cmake --build . --config Release
    ./webcam_face_pose_ex

Now we can run a speed-up real time face landmark, get the coordinates of 68 points and the frames per second (fps) .


### 2. Game Emulator
* Download and install DosBox for your machine: http://www.dosbox.com/download.php?main=1

- After installing Dosbox, you need to mount the DosBox folder, via opening up the DosBox exe and run the command mount c c:\dosgames where dosgames where your MSDOS games are stored

- Then go into the mounted folder by typing c: into the DosBox terminal and type dir to see what folders are present

- After choosing a game which is present in the directory (e.g. pacman) you can go into that folder by doing cd pacman

- To run the gane then just type the name of the executable in order to run the game

- For more information on how to run DosBox go to http://www.pcworld.com/article/239399/how_to_use_dosbox_to_play_classic_games.html

- You can download different MSDOS games from http://www.myabandonware.com/. Once downloaded, just move the games into your mounted folder

[1]:	https://sourceforge.net/projects/boost/files/boost/1.62.0/
[2]:	http://dlib.net/

### 3. xdotool
* Our controller will work by obtaining output from the controller and then use xdotools to simulate the keyboard commands onto the DosBox emulator

* In order to use the xdotools script, you will need to be using a linux machine

* To install xdotools, it depends on the type of linux OS that your machine is running

> For Debian and Ubuntu, you can use apt-get install xdotool
> FreeBSD has xdotools in x11/xdotool
> Fedora can use yum install xdotool
> MAC OS can use sudo port install xdotool
> OpenSUSE can use zypper install xdotool

* To run our xdotools script, you need to open it with chmod 777