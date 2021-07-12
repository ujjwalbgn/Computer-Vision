#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace cv;
using namespace std;


int main()
{

    Mat src; Mat src_gray;
    int thresh = 100;
    RNG rng(12345);
    /// Load source image and convert it to gray
    src = imread( "IMG-2.jpg", 1 );
    Mat original = src.clone();
    /// Convert image to gray and blur it
    cvtColor( src, src_gray, CV_BGR2GRAY );

    Mat threshold_output;
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;

    /// Detect edges using Threshold
    threshold( src_gray, threshold_output, thresh, 255, THRESH_BINARY );
    /// Find contours
    findContours( threshold_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

    /// Find the rotated rectangles for each contour
    vector<RotatedRect> minRect( contours.size() );

    for( int i = 0; i < contours.size(); i++ )
        minRect[i] = minAreaRect( Mat(contours[i]) );

    /// Draw contours + rotated rects
    Mat drawing = Mat::zeros( threshold_output.size(), CV_8UC3 );
    Mat result_zero = Mat::zeros( threshold_output.size(), CV_8UC3 );

    for( int i = 0; i< contours.size(); i++ )
    {
        Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
        // detect contours
        drawContours( drawing, contours, i, color, 1, 8, vector<Vec4i>(), 0, Point() );
        // detect rectangle for each contour
        Point2f rect_points[4]; minRect[i].points( rect_points );

        double length_1 = cv::norm(cv::Mat(rect_points[0]),cv::Mat(rect_points[1]));
        double length_2 = cv::norm(cv::Mat(rect_points[1]),cv::Mat(rect_points[2]));


        for( int j = 0; j < 4; j++ )
        {
            int temp1 = (int)length_1;
            int temp2 = (int)length_2;

            if(length_1>length_2)
                putText(original,to_string(temp1),rect_points[0],FONT_HERSHEY_SIMPLEX,1.0,Scalar(0,255,255),2);
            else
                putText(original,to_string(temp2),rect_points[0],FONT_HERSHEY_SIMPLEX,1.0,Scalar(0,255,255),2);

            line( result_zero, rect_points[j], rect_points[(j+1)%4], color, 1, 8 );
        }

    }

    /// Show in windows
    imshow("First",original);
    imshow( "Contours", drawing );
    waitKey(0);
    return(0);
}