#include <iostream>
#include <string>

#include "opencv2/opencv.hpp"

// function prototypes
static void clickCallback(int event, int x, int y, int flags, void *param);
void paintBucketFunction(int x, int y);

// Global Varaibles
bool eyedropper_mode = true;
bool crop_mode = false;
bool pencil_mode = false;
bool pencilOn = false;
bool paint_bucket_mode = false;
int rightbutton_counter = 0;
cv::Vec3b color_value;
cv::Vec3b current_color;
cv::Point start_point, end_point;
cv::Mat imageOut;
cv::Mat imageIn;


void paintBucketFunction(int x, int y)
{   
    imageOut.at<cv::Vec3b>(y, x) = color_value;
    if (imageOut.at<cv::Vec3b>(y, x-1) == current_color)
    {
        imageOut.at<cv::Vec3b>(y, x-1) = color_value;
        paintBucketFunction(y, x-1);
    }
    if (imageOut.at<cv::Vec3b>(y, x+1) == current_color)
    {
        imageOut.at<cv::Vec3b>(y, x+1) = color_value;
        paintBucketFunction(y, x+1);
    }
    if (imageOut.at<cv::Vec3b>(y-1, x) == current_color)
    {
        imageOut.at<cv::Vec3b>(y-1, x) = color_value;
        paintBucketFunction(y-1, x);
    }
     if (imageOut.at<cv::Vec3b>(y+1, x) == current_color)
    {
        imageOut.at<cv::Vec3b>(y+1, x) = color_value;
        paintBucketFunction(y+1, x);
    }

}

static void clickCallback(int event, int x, int y, int flags, void *param)
{
    // cast userdata to a cv::Mat
    imageOut = *(cv::Mat *)param;

    // Right Button
    if (event == cv::EVENT_RBUTTONDOWN)
    {
        rightbutton_counter = rightbutton_counter + 1;

        if (rightbutton_counter == 0 || rightbutton_counter == 4)
        {
            eyedropper_mode = true;
            paint_bucket_mode = false;
            std::cout << "Current Mode: eyedropper mode " << std::endl;
            std::cout << "Next Mode : crop mode " << std::endl;

            //To reset rightbutton_counter incase its 4
            rightbutton_counter = 0;
        }
        else if (rightbutton_counter == 1)
        {
            eyedropper_mode = false;
            crop_mode = true;
            std::cout << "Current Mode: crop mode " << std::endl;
            std::cout << " Next Mode : pencil mode " << std::endl;
        }
        else if (rightbutton_counter == 2)
        {
            crop_mode = false;
            pencil_mode = true;
            std::cout << "Current Mode: pencil mode " << std::endl;
            std::cout << "Next Mode : paint bucket mode " << std::endl;
        }
        else if (rightbutton_counter == 3)
        {
            pencil_mode = false;
            paint_bucket_mode = true;
            std::cout << "Current Mode: paint bucket mode " << std::endl;
            std::cout << "Next Mode : eyedropper mode " << std::endl;
        }

        // std::cout << rightbutton_counter << std::endl;
        std::cout << "------------------------------" << std::endl;
    }

    // Left Button
    if (event == cv::EVENT_LBUTTONDOWN)
    {

        if (eyedropper_mode)
        {
            color_value = imageOut.at<cv::Vec3b>(y, x);
            std::cout << "Eyedropper value: " << color_value << std::endl;
        }
        else if (crop_mode)
        {
            start_point.x = x;
            start_point.y = y;
            std::cout << "Croping from (" << x << ", " << y << ")" << std::endl;
        }
        else if (pencil_mode)
        {
            pencilOn = true;
            imageOut.at<cv::Vec3b>(y, x) = color_value;
            cv::imshow("Image", imageOut);
            cv::waitKey();
        }
        else if (paint_bucket_mode)
        {
            current_color = imageOut.at<cv::Vec3b>(y, x);
            paintBucketFunction(x,y);
            cv::imshow("Image", imageOut);
            cv::waitKey();
        }
    }

    if (event == cv::EVENT_MOUSEMOVE && pencilOn)
    {
        imageOut.at<cv::Vec3b>(y, x) = color_value;
        cv::imshow("Image", imageOut);
        cv::waitKey();
    }

    // left button up
    if (event == cv::EVENT_LBUTTONUP)
    {
        if (crop_mode)
        {
            end_point.x = x;
            end_point.y = y;

            std::cout << "Croping to (" << x << ", " << y << ")" << std::endl;

            cv::Rect region(start_point, end_point);
            // extract the ROI into its own image and display
            imageOut = imageOut(region);

            cv::imshow("Image", imageOut);
            cv::waitKey();
        }
        else if (pencil_mode)
        {
            pencilOn = false;
        }
    }

    //reset
    if (event == cv::EVENT_LBUTTONDBLCLK)
    {
        //clone image
        imageOut = imageIn.clone();
        cv::imshow("Image", imageOut);
        cv::waitKey();
    }
}

int main(int argc, char **argv)
{

    if (argc != 2)
    {
        std::printf("USAGE: %s <image_path> \n", argv[0]);
        return 0;
    }

    // open the input image
    std::string inputFileName = argv[1];

    imageIn = cv::imread(inputFileName, cv::IMREAD_COLOR);

    // check for file error
    if (!imageIn.data)
    {
        std::cout << "Error while opening file " << inputFileName << std::endl;
        return 0;
    }

    //clone image
    imageOut = imageIn.clone();

    // display the input image
    cv::imshow("Image", imageOut);
    cv::setMouseCallback("Image", clickCallback, &imageOut);
    cv::waitKey();
}