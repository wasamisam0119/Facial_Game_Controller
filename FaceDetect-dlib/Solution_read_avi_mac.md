#Solution of Mac OS read .avi video

## 1. Problem of environment

#### Reinstall the opencv with ffmpeg can solve it
Just do it in the terminal 

	brew install ffmpeg
	brew tap homebrew/science
	brew install --HEAD opencv --with-ffmpeg


If it reminds unlink some package in the homebrew, just do according to reminder.


See more: 

1. <https://github.com/Homebrew/homebrew-science/issues/2862>

2. <https://github.com/opencv/opencv/issues/7474>


## 2. Problem of video position
### put the video file in

	dlib/examples/video/vid.avi
	//if no file video, just mkdir video