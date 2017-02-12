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

int vaildSpace(int *x,int *y,double *w,double *h,int video_w,int video_h)
{
    int boolean=1;
    if (*x-30<0)
    {
        *x=1;
        *w=0.8*video_w;
        //boolean=0;
    }
    if (*y-30<0)
    {
        *y=1;
        *h=0.8*video_h;
        //boolean=0;
    }

    if(*w+*x>video_w)
        {
            *x=video_w/4;
            *w=video_w/2;
            //boolean=0;
        }
    if (*h+*y>video_h)
        {
            *y=video_h/4;
            *h=video_h/2;
            //boolean=0;
        }
    return boolean;    
    

}

int main()
{
    int count=0;
    int face_detected=0;
    int left_top_x, left_top_y, right_bottom_x, right_bottom_y, x,y;
    int initial_x;
    int initial_y;
    int init=1;
    int *x1=&left_top_x;
    int *y1=&left_top_y;
    double width, height;
    double *w=&width;
    double *h=&height;
    std::vector<int> diff_frames;

    try
    {
        cv::VideoCapture cap(0);
        double video_w=cap.get(CV_CAP_PROP_FRAME_WIDTH);
        double video_h=cap.get(CV_CAP_PROP_FRAME_HEIGHT);
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
            cv_image<bgr_pixel> cimg;
            // Find the pose of each face.
            std::vector<full_object_detection> shapes;
            full_object_detection shape;

            if (face_detected&&vaildSpace(x1,y1,w,h,video_w,video_h))
            {
                
                cv::Rect myROI(left_top_x, left_top_y, width, height);    
                croppedImage= temp(myROI);               
                cv_image<bgr_pixel>cimg_crop(croppedImage);
                cimg= cimg_crop;
            }                                                    
                
                // Detect faces 
            else{
                cv_image<bgr_pixel>cimg_ori(temp);
                cimg=cimg_ori;
                init=1;
            }                                        
                //cv::imshow("hell",croppedImage);
            
            faces = detector(cimg);
            if (faces.size()>0)
                {
                    
                    printf("get Faces!\n");
                    face_detected=1;
                    for (unsigned long i = 0; i < faces.size(); ++i)
                    {
                        shape = pose_model(cimg, faces[i]);                                                        
                        left_top_x = 0.6*shape.part(0).x(); 
                        left_top_y = 0.65*shape.part(18).y();
                        right_bottom_x = shape.part(16).x();
                        right_bottom_y = shape.part(8).y();
                        //diff_frames.push_back(shape.part(0).x());
                        //diff_frames.push_back(shape.part(18).y());

                        if(init)
                        {
                            initial_x=left_top_x;
                            initial_y=left_top_y;
                            printf("init x is %d\n",initial_x );
                            init=0;
                        }
                        else{
                          /*  if (diff_frames.size()>=4)
                            {
                                initial_x+=(diff_frames[diff_frames.size()-2]-diff_frames[diff_frames.size()-4]);
                                initial_y+=(diff_frames[diff_frames.size()-1]-diff_frames[diff_frames.size()-3]);
                            }
                            */
                            left_top_x+=initial_x;
                            left_top_y+=initial_y;
                            right_bottom_x+=initial_x;
                            right_bottom_y+=initial_y;
                            cout<<left_top_x<<" added "<<left_top_y<<endl;                                                                            

                        }                        
                        width = 1.6*(right_bottom_x - left_top_x);
                        height = 1.2*(right_bottom_y - left_top_y);
                        shapes.push_back(shape);
                        cout<<width<<"width and height "<<height<<endl;
                        cout<<left_top_x<<" "<<left_top_y<<endl;
                        //printf("%d %d\n",left_top_x,left_top_y);
                        //printf("%d %d\n",x,y );
                    /*if (count%100==0)
                    {
                        diff_frames.clear();
                    }*/
                    if (count%2==0)
                        {
                            cv::rectangle(
                                    temp,
                                    cv::Point(left_top_x,left_top_y),
                                    cv::Point(left_top_x+width,left_top_y+height),
                                    cv::Scalar(255,255,255)
                                    );                                    
                        }    
                    }

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

