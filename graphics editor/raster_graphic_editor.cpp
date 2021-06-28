#include <iostream>
#include <string>
#include "opencv2/opencv.hpp"

// function prototypes
static void clickCallback(int event, int x, int y, int flags, void* param);

static void clickCallback(int event, int x, int y, int flags, void *param)
{
    // bool eyedropper_mode = true;
    // bool crop_mode = false;
    // bool pencil_mode = false;
    // bool paint_bucket_mode = false;
    // int rightbutton_counter = 0;

    // cast userdata to a cv::Mat
    cv::Mat imageIn = *(cv::Mat *)param;

    // For Right Button Press to switch between mode
    // if (event == cv::EVENT_RBUTTONDOWN)
    // {
    //     if (rightbutton_counter == 0 || rightbutton_counter == 4)
    //     {
    //         eyedropper_mode = true;
    //         paint_bucket_mode = false;
    //         std::cout << "Current Mode: eyedropper mode " << std::endl;
    //         std::cout << "Next Mode : crop mode " << std::endl;

    //         //To reset rightbutton_counter incase its 4
    //         rightbutton_counter = 0;
    //     }
    //     else if (rightbutton_counter == 1)
    //     {
    //         eyedropper_mode = false;
    //         crop_mode = true;
    //         std::cout << "Current Mode: crop mode " << std::endl;
    //         std::cout << " Next Mode : pencil mode " << std::endl;
    //     }
    //     else if (rightbutton_counter == 2)
    //     {
    //         crop_mode = false;
    //         pencil_mode = true;
    //         std::cout << "Current Mode: pencil mode " << std::endl;
    //         std::cout << "Next Mode : paint bucket mode " << std::endl;
    //     }
    //     else if (rightbutton_counter == 3)
    //     {
    //         pencil_mode = flags;
    //         paint_bucket_mode = false;
    //         std::cout << "Current Mode: paint bucket mode " << std::endl;
    //         std::cout << "Next Mode : eyedropper mode " << std::endl;
    //     }

    //     rightbutton_counter = rightbutton_counter++;
    // }

}

int main(int argc, char **argv)
{
    // validate and parse the command line arguments
    if (argc != 2)
    {
        std::printf("USAGE: %s <image_path> \n", argv[0]);
        return 0;
    }

    cv::Mat imageIn;
    imageIn = cv::imread(argv[1], cv::IMREAD_COLOR);

    // check for file error
    if (!imageIn.data)
    {
        std::cout << "Error while opening file " << argv[1] << std::endl;
        return 0;
    }

    cv::imshow("imageIn", imageIn);
    cv::setMouseCallback("Original image", clickCallback, &imageIn);
    cv::waitKey();
}