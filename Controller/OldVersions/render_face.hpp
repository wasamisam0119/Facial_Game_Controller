/*put this file in dlib/examples
*/
#ifndef BIGVISION_RENDER_FACE_H_
#define BIGVISION_RENDER_FACE_H_

#include <dlib/image_processing/frontal_face_detector.h>
#include <opencv2/highgui/highgui.hpp>

void draw_polyline(cv::Mat &img, const dlib::full_object_detection& d, const int start, const int end, int offset_x, int offset_y, bool isClosed = false)
{
    std::vector <cv::Point> points;
    for (int i = start; i <= end; ++i)
    {

        points.push_back(cv::Point(d.part(i).x()+offset_x, d.part(i).y()+offset_y));
    }
    cv::polylines(img, points, isClosed, cv::Scalar(255,0,0), 2, 16);
    
}

void render_face (cv::Mat &img, const dlib::full_object_detection& d,int offset_x,int offset_y)
{
    DLIB_CASSERT
    (
     d.num_parts() == 68,
     "\t std::vector<image_window::overlay_line> render_face_detections()"
     << "\n\t Invalid inputs were given to this function. "
     << "\n\t d.num_parts():  " << d.num_parts()
     );
    
    draw_polyline(img, d, 0, 16,offset_x,offset_y);           // Jaw line
    draw_polyline(img, d, 17, 21,offset_x,offset_y);          // Left eyebrow
    draw_polyline(img, d, 22, 26,offset_x,offset_y);          // Right eyebrow
    draw_polyline(img, d, 27, 30,offset_x,offset_y,true);          // Nose bridge
    draw_polyline(img, d, 30, 35, offset_x,offset_y,true);    // Lower nose
    draw_polyline(img, d, 36, 41, offset_x,offset_y,true);    // Left eye
    draw_polyline(img, d, 42, 47, offset_x,offset_y,true);    // Right Eye
    draw_polyline(img, d, 48, 59, offset_x,offset_y,true);    // Outer lip
    draw_polyline(img, d, 60, 67, offset_x,offset_y,true);    // Inner lip
    
}

#endif // BIGVISION_RENDER_FACE_H_