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

#include <dlib/opencv.h>
#include <opencv2/highgui/highgui.hpp>
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing/render_face_detections.h>
#include <dlib/image_processing.h>
#include <dlib/gui_widgets.h>
#include <opencv2/imgproc/imgproc.hpp>

#define BIGVISION_RENDER_FACE_H_
 
#include <dlib/image_processing/frontal_face_detector.h>


#define FACE_DOWNSAMPLE_RATIO 2
#define SKIP_FRAMES 1

using namespace dlib;
using namespace std;

int main()
{
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
        // Number of frames to capture
        double fps = cap.get(CV_CAP_PROP_FPS);
        int num_frames = 120;
     
        // Start and end times
        time_t start, end;
        int countt = 0;
        std::vector<rectangle> faces;
        int count=0;
        // Grab and process frames until the main window is closed by the user.
        while(!win.is_closed())
        {
            // Grab a frame
            cv::Mat temp;

            //yehan add 1/12/2016
            cv::Mat temp_small, temp_display;
            time(&start);
            for (int i = 0; i < 120; ++i)
            {
                
                cap >> temp; 
                // Turn OpenCV's Mat into something dlib can deal with.  Note that this just
                // wraps the Mat object, it doesn't copy anything.  So cimg is only valid as
                // long as temp is valid.  Also don't do anything to temp that would cause it
                // to reallocate the memory which stores the image as that will make cimg
                // contain dangling pointers.  This basically means you shouldn't modify temp
                // while using cimg.

                //Resize image for face detection -yehan
                cv::resize(temp, temp_small, cv::Size(), 1.0/FACE_DOWNSAMPLE_RATIO, 1.0/FACE_DOWNSAMPLE_RATIO);
                // Change to dlib's image format. No memory is copied.  -yehan
                cv_image<bgr_pixel> cimg_small(temp_small);

                
                cv_image<bgr_pixel> cimg(temp);

                // Detect faces 
                if ( count % SKIP_FRAMES == 0 )
                {
                    faces = detector(cimg_small);
                }
                //std::vector<rectangle> faces = detector(cimg_small);
                //try1 print number of faces
                if (count<1)
                {
                cout << "Number of faces detected: " << faces.size() << endl;
                ++count;
                }
                // Find the pose of each face.
                std::vector<full_object_detection> shapes;
                //for (unsigned long i = 0; i < faces.size(); ++i)
                //  shapes.push_back(pose_model(cimg, faces[i]));

                //try2 print the coordinate
                for (unsigned long j = 0; j < faces.size(); ++j)
                {
                    rectangle r(
                    (long)(faces[j].left() * FACE_DOWNSAMPLE_RATIO),
                    (long)(faces[j].top() * FACE_DOWNSAMPLE_RATIO),
                    (long)(faces[j].right() * FACE_DOWNSAMPLE_RATIO),
                    (long)(faces[j].bottom() * FACE_DOWNSAMPLE_RATIO)
                );

                    full_object_detection shape = pose_model(cimg, r);
                    /*
                    cout << "number of parts: "<< shape.num_parts() << endl;
                    cout << "pixel position of point 0: " << shape.part(0) << endl;
                    cout << "pixel position of point 1: " << shape.part(1) << endl;

                    cout << "pixel position of point 27: " << shape.part(27) << endl;
                    cout << "pixel position of point 30: " << shape.part(30) << endl;

                    cout << "pixel position of point 16: " << shape.part(16) << endl;
                    cout << "pixel position of point 15: " << shape.part(15) << endl;
                    */
                    // Here we just store them in shapes so we can
                    // put them on the screen.
                    shapes.push_back(pose_model(cimg, r));
                }

                // Display it all on the screen
                win.clear_overlay();
                win.set_image(cimg);
                win.add_overlay(render_face_detections(shapes));
            }
            time(&end);
            double seconds = difftime (end, start);
            cout << "Time taken : " << seconds << " seconds" << endl;
            // Calculate frames per second
            fps  = num_frames / seconds;
            cout << "Estimated frames per second : " << fps << endl;
            // Display it all on the screen
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


