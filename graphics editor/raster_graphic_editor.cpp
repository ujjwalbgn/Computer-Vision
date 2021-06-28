#include <iostream>
#include <string>
#include "opencv2/opencv.hpp"

// function prototypes
static void clickCallback(int event, int x, int y, int flags, void *param);

// Global Varaibles for tools
bool eyedropper_mode = true;
bool crop_mode = false;
bool pencil_mode = false;
bool paint_bucket_mode = false;
int rightbutton_counter = 0;
cv::Vec3b color_value;
cv::Point start_point, end_point;
cv::Mat imageOut;

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
            pencil_mode = flags;
            paint_bucket_mode = true;
            std::cout << "Current Mode: paint bucket mode " << std::endl;
            std::cout << "Next Mode : eyedropper mode " << std::endl;
        }

        // std::cout << rightbutton_counter << std::endl;
        std::cout << "------------------------------" << std::endl;
    }

    if (event == cv::EVENT_LBUTTONDOWN)
    {
        if (eyedropper_mode)
        {
            cv::Vec3b color_value = imageOut.at<cv::Vec3b>(y, x);
            std::cout << color_value << std::endl;
        }
        else if (crop_mode)
        {
            start_point.x = x;
            start_point.y = y;
            std::cout << "Croping from (" << x << ", " << y << ")" << std::endl;
        }
    }
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
            
            cv::imshow("croped image", imageOut);
            cv::waitKey();
        }
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

    cv::Mat imageIn;
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