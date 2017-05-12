// The contents of this file are in the public domain. See LICENSE_FOR_EXAMPLE_PROGRAMS.txt
/*
    This example program shows how to find frontal human faces in an image and
    estimate their pose.  The pose takes the form of 68 landmarks.  These are
    points on the face such as the corners of the mouth, along the eyebrows, on
    the eyes, and so forth.

    This example is essentially just a version of the face_landmark_detection_ex.cpp
    example modified to use OpenCV's VideoCapture object to read from a camera instead
    of files.
    Finally, note that the face detector is fastest when compiled with at least
    SSE2 instructions enabled.  So if you are using a PC with an Intel or AMD
    chip then you should enable at least SSE2 instructions.  If you are using
    cmake to compile this program you can enable them by using one of the
    following commands when you create the build project:
        cmake path_to_dlib_root/examples -DUSE_SSE2_INSTRUCTIONS=ON
        cmake path_to_dlib_root/examples -DUSE_SSE4_INSTRUCTIONS=ON
        cmake path_to_dlib_root/examples -DUSE_AVX_INSTRUCTIONS=ON
    This will set the appropriate compiler options for GCC, clang, Visual
    Studio, or the Intel compiler.  If you are using another compiler then you
    need to consult your compiler's manual to determine how to enable these
	instructions.  Note that AVX is the fastest but requires a CPU from at least
	2011.  SSE4 is the next fastest and is supported by most current machines.
	*/

#include <math.h>
#include <dlib/opencv.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing/render_face_detections.h>
#include <dlib/image_processing.h>
#include <dlib/gui_widgets.h>
#include <vector>
#include "render_face.hpp"
#include <string>
#include <fstream>
#include "Keys.h"

using namespace dlib;
using namespace std;
#define OPENCV_FACE_RENDER
/*actually it is time difference in million second, but the unit is seconds, so that's the name comes from*/
long double getDifferenceInSecond(struct timeval start, struct timeval end)
{
		int iSeconds = end.tv_sec - start.tv_sec;
		int iUSeconds = end.tv_usec - start.tv_usec;
		long double mtime = (iSeconds * 1000 + iUSeconds / 1000.0) / 1000.000;
		return mtime;
}
/*fps calculator*/
inline double FPScal(int count, double fps, long double iDifference, struct timeval *currentTime, struct timeval *startTime)
{
		
		if (count != 50)
		{
				iDifference = getDifferenceInSecond(*startTime, *currentTime);
		}
		printf("%Lf\n", iDifference);
		fps = 50 / iDifference;
		

		return fps;

}

/*this function is used to check if the bounding box(coordinates) excess the video_size,
  which means it is a assertion, if it excessed, set the corrdinates to a proper value*/
int vaildSpace(int *x, int *y, double *w, double *h, int video_w, int video_h)
{
		int boolean = 1;
		if (*x - 10 < 0)
		{
				*x = 1;
				*w = 0.8 * video_w;

		}
		if (*y - 10 < 0)
		{
				*y = 1;
				*h = 0.8 * video_h;

		}

		if(*w + *x > video_w) //the rightest point of the bounding box excess the screen
		{
				*x = video_w / 4;
				*w = video_w / 2;

		}
		if (*h + *y > video_h)
		{
				*y = video_h / 4;
				*h = video_h / 2;

		}
		return boolean;


}

/*This function returns the 3D coordinates of five key points into modelPoints*/
std::vector<cv::Point3d> get_3d_model_points()
{
		std::vector<cv::Point3d> modelPoints;

		modelPoints.push_back(cv::Point3d(0.0f, 0.0f, 0.0f)); //The first must be (0,0,0) while using POSIT
		modelPoints.push_back(cv::Point3d(0.0f, -330.0f, -65.0f));
		modelPoints.push_back(cv::Point3d(-225.0f, 170.0f, -135.0f));
		modelPoints.push_back(cv::Point3d(225.0f, 170.0f, -135.0f));
		modelPoints.push_back(cv::Point3d(-150.0f, -150.0f, -125.0f));
		modelPoints.push_back(cv::Point3d(150.0f, -150.0f, -125.0f));

		return modelPoints;

}

/*This function saves the real-time 2D coordinates of five key points into image_points*/
std::vector<cv::Point2d> get_2d_image_points(full_object_detection &d, int offset_x, int offset_y)
{
		std::vector<cv::Point2d> image_points;
		image_points.push_back( cv::Point2d( d.part(30).x() + offset_x, d.part(30).y() + offset_y ) ); // Nose tip
		image_points.push_back( cv::Point2d( d.part(8).x() + offset_x, d.part(8).y() + offset_y ) );  // Chin
		image_points.push_back( cv::Point2d( d.part(36).x() + offset_x, d.part(36).y() + offset_y ) ); // Left eye left corner
		image_points.push_back( cv::Point2d( d.part(45).x() + offset_x, d.part(45).y() + offset_y ) ); // Right eye right corner
		image_points.push_back( cv::Point2d( d.part(48).x() + offset_x, d.part(48).y() + offset_y ) ); // Left Mouth corner
		image_points.push_back( cv::Point2d( d.part(54).x() + offset_x, d.part(54).y() + offset_y ) ); // Right mouth corner
		return image_points;

}

/*Set a generic parameter of camera*/
cv::Mat get_camera_matrix(float focal_length, cv::Point2d center)
{
		cv::Mat camera_matrix = (cv::Mat_<double>(3, 3) << focal_length, 0, center.x, 0 , focal_length, center.y, 0, 0, 1);
		return camera_matrix;
}

/*Get the euler angles from processed rotation matrix*/
void getEulerAngles(cv::Mat &rotCamerMatrix,cv::Vec3d &eulerAngles){

		cv::Mat cameraMatrix,rotMatrix,transVect,rotMatrixX,rotMatrixY,rotMatrixZ;
		double* _r = rotCamerMatrix.ptr<double>();
		double projMatrix[12] = {_r[0],_r[1],_r[2],0,
				_r[3],_r[4],_r[5],0,
				_r[6],_r[7],_r[8],0};

		decomposeProjectionMatrix( cv::Mat(3,4,CV_64FC1,projMatrix),
						cameraMatrix,
						rotMatrix,
						transVect,
						rotMatrixX,
						rotMatrixY,
						rotMatrixZ,
						eulerAngles);
}

/*
This function converts the specfic angel to head movements
Interface of Keyboard function
*/
int quadrant (cv::Vec3d eulerAngles)
{
    float yaw = eulerAngles[1];
    float pitch = eulerAngles[0];
    float roll = eulerAngles[2];
        if (yaw < -27)
        {
			left();
            cout << "left" << endl;     //mirror reflection
            return 2;
        }
        else if (yaw > 27)
        {
			right();
            cout << "right" << endl;
            return 3;
        }
        else if (pitch<0 && pitch>-168)
        {
			down();
            cout << "down" << endl;
            return 0;

        }
        else if (pitch<168 && pitch>0)
        {
			up();
            cout << "up" << endl;
            return 1;
        }
        else if (roll<-25)
		{
			mouseRight();
			cout <<"roll right"<< endl;
			//cout <<roll<<endl;
			return 4;
		}
		else if(roll>25)
		{
			mouseLeft();
			cout <<"roll left"<< endl;
			//cout <<roll<<endl;
			return 5;
		}
        else
        {
			release();
        	cout << "still" << endl;
        	return 6;
        }
}

int main()
{
		int left_top_x, left_top_y, right_bottom_x, right_bottom_y, offset_x, offset_y;
		int initial_x;
		int initial_y;
		int *x1 = &initial_x;
		int *y1 = &initial_y;
		double width, height;
		double *w = &width;
		double *h = &height;
		string number;
		fstream file;
		double fps = 0;
		char direction[20]="";
		setup();
		mouseSetup();
		try
		{
						int second_time = 0;
						int init = 1;
						int count = 0; //count the frame
						int face_detected = 0; //check if the detector detecte the face
						/* ===========code for video test==============
						cv::VideoCapture cap("../test_video/video.avi");
						*/
						cv::VideoCapture cap(0);
						//cv::VideoCapture cap(0);
						//		int frameCnt = cap.get(CV_CAP_PROP_FRAME_COUNT);
						double video_w = cap.get(CV_CAP_PROP_FRAME_WIDTH);//get the pixel size of the camera
						double video_h = cap.get(CV_CAP_PROP_FRAME_HEIGHT);//macOS is 1280*720
						cout << video_w << " " << video_h << endl;
						if (!cap.isOpened())
						{
								cerr << "Unable to connect to camera" << endl;
								return 1;
						}
						image_window win;
						cv::Mat im;
						// Get first frame and allocate memory.
						cap >> im;
						cv::Size size = im.size();
						// Load face detection and pose estimation models.
						frontal_face_detector detector = get_frontal_face_detector();
						shape_predictor pose_model;
						deserialize("shape_predictor_68_face_landmarks.dat") >> pose_model;
						std::vector<rectangle> faces;
						// Grab and process frames until the main window is closed by the user.
						while(!win.is_closed())
						{
								cv::Mat croppedImage;
								// Grab a frame
								cv::Mat temp;
								cap >> temp;
								cv_image<bgr_pixel> im_display(temp);
								cv_image<bgr_pixel> cimg;
								// Find the pose of each face.
								std::vector<full_object_detection> shapes;
								full_object_detection shape;

								// Pose estimation variables
								std::vector<cv::Point3d> model_points;
								std::vector<cv::Point2d> image_points;
								cv::Mat rotation_vector;
								cv::Mat rotation_matrix;
								cv::Vec3d eulerAngles;
								cv::Mat translation_vector;
								std::vector<cv::Point3d> nose_end_point3D;
								std::vector<cv::Point2d> nose_end_point2D;
								double focal_length = temp.cols;
								model_points = get_3d_model_points();
								cv::Mat camera_matrix = get_camera_matrix(focal_length, cv::Point2d(temp.cols / 2, temp.rows / 2));;
								cv::Mat dist_coeffs = cv::Mat::zeros(4, 1, cv::DataType<double>::type);

								/*
								   if it is second time and the coordinates is in the picture size,
								   crop the image and the detector will only detecte the face within the cropped image
								   */

								if (face_detected && vaildSpace(x1, y1, w, h, video_w, video_h))
								{
										cv::Rect myROI(initial_x, initial_y, width, height);
										croppedImage = temp(myROI);
										cv_image<bgr_pixel>cimg_crop(croppedImage);
										cimg = cimg_crop;
								}
								/*otherwise scan the whole image*/
								else
								{
										cv_image<bgr_pixel>cimg_ori(temp);
										cimg = cimg_ori;
										init = 1;
										second_time = 0;
								}
								faces = detector(cimg);					
								

								if (faces.size() > 0)
								{
										face_detected = 1;
										for (unsigned long i = 0; i < faces.size(); ++i)
										{
												shape = pose_model(cimg, faces[i]);
												right_bottom_x = shape.part(16).x();
												right_bottom_y = shape.part(8).y();
												if(init)
												{
														left_top_x = 0.6 * shape.part(0).x(); //this point is used to draw the bounding box,0.6 is make the box bigger
														left_top_y = 0.65 * shape.part(18).y(); //same as previous
														initial_x = left_top_x; //get the initial x which is used to calculate offset
														initial_y = left_top_y;
														init = 0;
														/*
#ifdef OPENCV_FACE_RENDER
render_face(temp, shape, 0, 0); //draw the face landmark

image_points = get_2d_image_points(shape, 0, 0);

cv::solvePnP(model_points, image_points, camera_matrix, dist_coeffs, rotation_vector, translation_vector);
nose_end_point3D.push_back(cv::Point3d(0, 0, 1000.0));

cv::projectPoints(nose_end_point3D, rotation_vector, translation_vector, camera_matrix, dist_coeffs, nose_end_point2D);
cv::line(temp, image_points[0], nose_end_point2D[0], cv::Scalar(255, 255, 0), 2);

														//cout << "Rotation Vector " << endl << rotation_vector << endl;
														//cout << "Translation Vector" << endl << translation_vector << endl;
														quadrant (image_points, nose_end_point2D );

														//cout << "nose pose:"<< nose_end_point2D[0] << endl;
														//---------------------
#endif
*/
														second_time++;
												}
												else
												{
														if (second_time == 1)
														{
																//calculate offset
																offset_x = initial_x;
																offset_y = initial_y;
														}
														left_top_x = shape.part(0).x() + offset_x;
														left_top_y = shape.part(18).y() + offset_y;
														right_bottom_x += offset_x;
														right_bottom_y += offset_y;
														if (second_time > 0)
														{
																left_top_x *= 0.6;
																left_top_y *= 0.65;
														}
														second_time++;
												}
												// the factor of 1.4 and 1.2 is based on several user experience, 
												// and we find this can be a suitable value for enlarge the bounding box
												width = 1.4 * (right_bottom_x - left_top_x);
												height = 1.2 * (right_bottom_y - left_top_y);
												shapes.push_back(shape);
												if (second_time<=1)
												{	
														render_face(temp, shape, 0, 0); //draw the face landmark
														image_points = get_2d_image_points(shape, 0, 0);
												}
												else{
														render_face(temp, shape, offset_x, offset_y);
														image_points = get_2d_image_points(shape, offset_x, offset_y);

												}
												//get the rotation function through the correspondences between 3D and 2D points
												cv::solvePnP(model_points, image_points, camera_matrix, dist_coeffs, rotation_vector, translation_vector);

												/*Draw a line from nose to 3D point to illustrate the rotation*/
												nose_end_point3D.push_back(cv::Point3d(0, 0, 1000.0));
												cv::projectPoints(nose_end_point3D, rotation_vector, translation_vector, camera_matrix, dist_coeffs, nose_end_point2D);
												cv::line(temp, image_points[0], nose_end_point2D[0], cv::Scalar(255, 255, 0), 2);

												Rodrigues(rotation_vector,rotation_matrix);
												getEulerAngles(rotation_matrix,eulerAngles);

												/*
												   Print the real time result on screen
												   */
												switch (quadrant (eulerAngles))
												{
														case 0:
																strcpy(direction,"Down");
																break;
														case 1:
																strcpy(direction,"Up");
																break;
														case 2:
																strcpy(direction,"Left");
																break;
														case 3:
																strcpy(direction,"Right");
																break;
														case 4:
																strcpy(direction,"Roll right");
																break;
														case 5:
																strcpy(direction,"Roll left");
																break;
														case 6:
																strcpy(direction,"Still");
																break;
												}
												cv::putText(temp, cv::format("Direction: %s",direction), cv::Point(50, size.height - 25), cv::FONT_HERSHEY_COMPLEX, 1, cv::Scalar(0, 0, 255), 2);

										}
										if (vaildSpace(x1, y1, w, h, video_w, video_h))
										{
												cv::rectangle(
																temp,
																cv::Point(left_top_x, left_top_y),
																cv::Point(left_top_x + width, left_top_y + height),
																cv::Scalar(255, 255, 255)
															 );
										}
								}
								else
								{
										face_detected = 0;
								}
								count++;
								// Display the fps dashboard on the screen
								win.clear_overlay();
								win.set_image(im_display);
						}
		}
		catch(serialization_error &e)
		{
				cout << "You need dlib's default face landmarking model file to run this example." << endl;
				cout << "You can get it from the following URL: " << endl;
				cout << "   http://dlib.net/files/shape_predictor_68_face_landmarks.dat.bz2" << endl;
				cout << endl << e.what() << endl;
		}
		catch(exception &e)
		{
				cout << e.what() << endl;
		}

}

