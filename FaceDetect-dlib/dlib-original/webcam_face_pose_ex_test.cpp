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

#include <dlib/opencv.h>
#include <opencv2/highgui/highgui.hpp>
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing/render_face_detections.h>
#include <dlib/image_processing.h>
#include <dlib/gui_widgets.h>
#include <vector>
using namespace dlib;
using namespace std;

int main()
{
    int count=0;
    int flag=0;
    int left_top_x, left_top_y, right_bottom_x, right_bottom_y, width, height, new_left_top_x, new_left_top_y;
    try
    {
        cv::VideoCapture cap(0);
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
            // Turn OpenCV's Mat into something dlib can deal with.  Note that this just
            // wraps the Mat object, it doesn't copy anything.  So cimg is only valid as
            // long as temp is valid.  Also don't do anything to temp that would cause it
            // to reallocate the memory which stores the image as that will make cimg
            // contain dangling pointers.  This basically means you shouldn't modify temp
            // while using cimg.
            
            cv_image<bgr_pixel> im_display(temp);              
                                                    
                
        
            
            cv_image<bgr_pixel> cimg(temp);

            
            if (count>5)
            {
                cv::Rect myROI(new_left_top_x,new_left_top_y, width, height);    
                croppedImage= temp(myROI); 
                cv_image<bgr_pixel> cimg_small(croppedImage);
                faces = detector(cimg_small);
                cimg=cimg_small;
             
                cv::rectangle(
                        temp,
                        cv::Point(new_left_top_x, new_left_top_y),
                        cv::Point(new_left_top_x + width , new_left_top_y + height),
                        cv::Scalar(255,0 ,0)
                   );
                       
            }
            if (count <6){
                faces = detector(cimg);    
                
                cv::rectangle(
                        temp,
                        cv::Point(left_top_x-40,left_top_y-40),
                        cv::Point(right_bottom_x + 40 , right_bottom_y+40),
                        cv::Scalar(255,255,255)
                        );
                if (count == 5)
                {
                    new_left_top_x = left_top_x-40;
                    new_left_top_y = left_top_y-40;
                    width = right_bottom_x - left_top_x + 80;
                    height = right_bottom_y - left_top_y + 80;
 
                }
                count++;
            }
            
            std::vector<full_object_detection> shapes;   
            for (unsigned long i = 0; i < faces.size(); ++i)
            {
                
                full_object_detection shape = pose_model(cimg, faces[i]);
              
                    //cout << "Number of faces detected: " << shape.size() << endl;
                    // Print the coordinate  there are 68 points on face
                    
                    cout << "pixel position of point 181: " << shape.part(18).x() << endl;
                    cout << "pixel position of point 182: " << shape.part(18).y() << endl;

                    cout << "pixel position of point 16x: " << shape.part(8).x() << endl;
                    cout << "pixel position of point 16y: " << shape.part(8).y() << endl;
                 
                    //dlib::point p;
                                        
                    left_top_x = shape.part(0).x(); 
                    left_top_y = shape.part(18).y();
                    right_bottom_x = shape.part(16).x();
                    right_bottom_y = shape.part(8).y();

                    
                    //cv::imshow("test",temp);
                    
                   
                    

                shapes.push_back(shape);
            }
            // Display it all on the screen
            win.clear_overlay();
            win.set_image(im_display);
            win.add_overlay(render_face_detections(shapes));
           
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