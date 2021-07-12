// include necessary dependencies
#include <iostream>
#include <string>
#include "opencv2/opencv.hpp"
#include "opencv2/imgproc.hpp"

// configuration parameters
#define NUM_COMNMAND_LINE_ARGUMENTS 1

//Globalk Variables
int quarter;
int dime;
int nickel;
int penny;
double total;

int main(int argc, char **argv)
{
    cv::Mat imageIn;
    cv::Mat imageResize;

    // validate and parse the command line arguments
    if (argc != NUM_COMNMAND_LINE_ARGUMENTS + 1)
    {
        std::printf("USAGE: %s <image_path> \n", argv[0]);
        return 0;
    }
    else
    {
        imageIn = cv::imread(argv[1], cv::IMREAD_COLOR);

        // check for file error
        if (!imageIn.data)
        {
            std::cout << "Error while opening file " << argv[1] << std::endl;
            return 0;
        }
    }

    //Resize Image to fit screen
    cv::resize(imageIn, imageResize, cv::Size(), 0.25, 0.25);

    // convert the image to grayscale
    cv::Mat imageGray;
    cv::cvtColor(imageResize, imageGray, cv::COLOR_BGR2GRAY);

    // find the image edges
    cv::Mat imageEdges;
    const double cannyThreshold1 = 100;
    const double cannyThreshold2 = 200;
    const int cannyAperture = 3;
    cv::Canny(imageGray, imageEdges, cannyThreshold1, cannyThreshold2, cannyAperture);

    // erode and dilate the edges to remove noise
    int morphologySize = 1;
    cv::Mat edgesDilated;
    cv::dilate(imageEdges, edgesDilated, cv::Mat(), cv::Point(-1, -1), morphologySize);
    cv::Mat edgesEroded;
    cv::erode(edgesDilated, edgesEroded, cv::Mat(), cv::Point(-1, -1), morphologySize);

    // locate the image contours (after applying a threshold or canny)
    std::vector<std::vector<cv::Point>> contours;
    //std::vector<int> hierarchy;
    cv::findContours(edgesEroded, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE, cv::Point(0, 0));

    // draw the contours
    cv::Mat imageContours = cv::Mat::zeros(imageEdges.size(), CV_8UC3);
    cv::RNG rand(12345);
    for (int i = 0; i < contours.size(); i++)
    {
        cv::Scalar color = cv::Scalar(255, 255, 255);
        cv::drawContours(imageContours, contours, i, color);
    }

    // fit ellipses to contours containing sufficient inliers
    std::vector<cv::RotatedRect> fittedEllipses(contours.size());
    for (int i = 0; i < contours.size(); i++)
    {
        // compute an ellipse only if the contour has more than 40 points (the minimum for ellipse fitting)
        if (contours.at(i).size() > 40)
        {
            fittedEllipses[i] = cv::fitEllipse(contours[i]);
        }
    }

    // draw the ellipses
    const int minEllipseInliers = 40;
    for (int i = 0; i < contours.size(); i++)
    {
        // draw any ellipse with sufficient inliers
        if (contours.at(i).size() > minEllipseInliers)
        {

            if (contours.at(i).size() >= 96)
            {
                quarter = quarter + 1;
                total = total + .25;
                cv::Scalar color = cv::Scalar(0, 255, 0); // Green Color
                cv::ellipse(imageResize, fittedEllipses[i], color, 2);
            }
            if (contours.at(i).size() >= 74 && contours.at(i).size() < 96)
            {
                nickel = nickel + 1;
                cv::Scalar color = cv::Scalar(0, 255, 255); // Yellow Color
                cv::ellipse(imageResize, fittedEllipses[i], color, 2);
                total = total + .05;
            }
            if(contours.at(i).size() > 60 && contours.at(i).size() < 74)
            {
                penny = penny + 1;
                cv::Scalar color = cv::Scalar(0,0,255); // Red Color
                cv::ellipse(imageResize, fittedEllipses[i], color, 2);
                total = total + .01;
            }
            if(contours.at(i).size() <= 60)
            {
                dime = dime + 1;
                cv::Scalar color = cv::Scalar(255,0,0); // Blue Color
                cv::ellipse(imageResize, fittedEllipses[i], color, 2);
                total = total + .1;
            }
        }
    }

    std::cout << "penny count: " << penny << std::endl;
    std::cout << "dime count: " << dime << std::endl;
    std::cout << "nickel count: " << nickel << std::endl;
    std::cout << "quarter count: " << quarter << std::endl;
    std::cout << "Total : $" << total << std::endl;

    // display the images
    cv::imshow("imageResize", imageResize);

    cv::waitKey();
}
