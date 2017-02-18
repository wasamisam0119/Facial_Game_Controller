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

//This version only print the coordinates of points
#include <math.h> 
#include <dlib/opencv.h>
#include <opencv2/highgui/highgui.hpp>
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing/render_face_detections.h>
#include <dlib/image_processing.h>
#include <dlib/gui_widgets.h>
#include <vector>
#include "render_face.hpp"

using namespace dlib;
using namespace std;
#define OPENCV_FACE_RENDER

/*this function is used to check if the bounding box(coordinates) excess the video_size, 
which means it is a assertion, if it excessed, set the corrdinates to a proper value*/
int vaildSpace(int *x,int *y,double *w,double *h,int video_w,int video_h)
{
    int boolean=1;
    if (*x-30<0)
    {
        *x=1;
        *w=0.8*video_w;
        
    }
    if (*y-30<0)
    {
        *y=1;
        *h=0.8*video_h;
        
    }

    if(*w+*x>video_w)   //the rightest point of the bounding box excess the screen
        {
            *x=video_w/4;
            *w=video_w/2;
         
        }
    if (*h+*y>video_h)
        {
            *y=video_h/4;
            *h=video_h/2;
            
        }
    return boolean;    
    

}

int main()
{
    int count=0;//count the frame
    int face_detected=0;//check if the detector detecte the face
    int left_top_x, left_top_y, right_bottom_x, right_bottom_y, x,y,offset_x,offset_y,point_zero_x,point_zero_y;
    int last_point;
    int initial_y_zero; //initial point of the zero landmark
    int initial_x;
    int initial_y;
    int init=1;     
    int *x1=&left_top_x;
    int *y1=&left_top_y;
    double width, height;
    double *w=&width;
    double *h=&height;
    int second_time=0;

    try
    {
        cv::VideoCapture cap("../test_video/vid.avi"); //put the test video in file dlib/examples/video/vid.avi
        double video_w=cap.get(CV_CAP_PROP_FRAME_WIDTH);//get the pixel size of the camera 
        double video_h=cap.get(CV_CAP_PROP_FRAME_HEIGHT);//macOS is 1280*720
        cout<<video_w<<" "<<video_h<<endl;
        if (!cap.isOpened())
        {
            cerr << "Unable to connect to camera" << endl;
            return 1;
        }
        image_window win;

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

            /*
            if it is second time and the coordinates is in the picture size, 
            crop the image and the detector will only detecte the face within the cropped image
            */
            if (face_detected&&vaildSpace(x1,y1,w,h,video_w,video_h))   
            {
                cv::Rect myROI(left_top_x, left_top_y, width, height);    
                croppedImage= temp(myROI);               
                cv_image<bgr_pixel>cimg_crop(croppedImage);
                cimg= cimg_crop;
            }
            /*otherwise scan the whole image*/                                                    
            else{
                cv_image<bgr_pixel>cimg_ori(temp);
                cimg=cimg_ori;
                init=1;
                second_time=0;
            }
                                                        
                                                        
            faces = detector(cimg);
            if (faces.size()>0)
                {            
                    face_detected=1;
                    for (unsigned long i = 0; i < faces.size(); ++i)
                    {
                        shape = pose_model(cimg, faces[i]);                                                         
                        left_top_x = 0.6*shape.part(0).x(); //this point is used to draw the bounding box,0.6 is make the box bigger
                        left_top_y = 0.65*shape.part(18).y();//same as previous
                        point_zero_x=left_top_x/0.6;    //this is used to calculate the offset for the next frame
                        point_zero_y=shape.part(0).y(); // same as previous                         
                        right_bottom_x = shape.part(16).x();    
                        right_bottom_y = shape.part(8).y();

                        if(init)
                        {
                            initial_x=left_top_x/0.6;   //get the initial x which is used to calculate offset
                            initial_y=left_top_y;       
                            initial_y_zero=shape.part(0).y();//same as previous
                            printf("init x is %d\n",initial_x );
                            init=0;
                            #ifdef OPENCV_FACE_RENDER
                                render_face(temp, shape,0,0);   //draw the face landmark
                            #endif
                            second_time++;
                        }
                        else{                                                    
                            offset_x=initial_x-point_zero_x;    //calculate offset
                            offset_y=initial_y_zero-point_zero_y;                                                    
                            left_top_x+=0.85*offset_x;
                            left_top_y+=0.9*offset_y;
                            right_bottom_x+=offset_x;
                            right_bottom_y+=offset_y;
                            
                            cout<<left_top_x<<" added "<<left_top_y<<endl;  //print the left top point of the bounding box 
                            #ifdef OPENCV_FACE_RENDER   
                                render_face(temp, shape,0.99*offset_x,0.99*offset_y);
                            #endif
                            if (second_time>0)
                            {
                                initial_x=point_zero_x+offset_x;
                                initial_y=point_zero_y+offset_y;
                            }
                            second_time++;

                        }                        
                        width = 1.4*(right_bottom_x - left_top_x);
                        height = 1.2*(right_bottom_y - left_top_y);                          
                        shapes.push_back(shape);
                        cout<<width<<"width and height "<<height<<endl;
                        
                        //printf("%d %d\n",left_top_x,left_top_y);
                        //printf("%d %d\n",x,y );                                        
                        cv::rectangle(
                                temp,
                                cv::Point(left_top_x,left_top_y),
                                cv::Point(left_top_x+width,left_top_y+height),
                                cv::Scalar(255,255,255)
                                );                                    
                
                    }
                    last_point=left_top_x;
                }
                else
                {
                    face_detected=0;
                }
            count++;
            // Display it all on the screen
            win.clear_overlay();
            win.set_image(im_display);                      
            //win.add_overlay(render_face_detections(shapes));
        }
    }
    catch(serialization_error& e)
    {
        cout << "You need dlib's default face landmarking model file to run this example." << endl;
        cout << "You can get it from the following URL: " << endl;
        cout << "   http://dlib.net/files/shape_predictor_68_face_landmarks.dat.bz2" << endl;
        cout << endl << e.what() << endl;
    }
    catch(exception& e)
    {
        cout << e.what() << endl;
    }


}

