// include necessary dependencies
#include <iostream>
#include <cstdio>
#include "opencv2/opencv.hpp"
#include <opencv2/dnn.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

// configuration parameters
#define NUM_COMNMAND_LINE_ARGUMENTS 1
#define DISPLAY_WINDOW_NAME "Video Frame"

// declare function prototypes
bool processFrame(const cv::Mat &imageIn, cv::Mat &imageOut);


bool processFrame(const cv::Mat &imageIn, cv::Mat &imageOut, cv::dnn::Net &network)
{
    // copy the input image frame to the ouput image (deep copy)
    imageOut = imageIn.clone();

    // create the input DNN blob from the image frame
    static cv::Mat blobFromImg;
    const double scaleFactor = 1.0;
    const cv::Size size = cv::Size(416, 416);
    const cv::Scalar mean = cv::Scalar();
    const bool swapRB = false;
    const bool crop = false;
    const int ddepth = CV_32F;
    cv::dnn::blobFromImage(imageIn, blobFromImg, scaleFactor, size, mean, swapRB, crop);

    // set the blob as input to the network
    const float blob_scale = 1.0 / 255.0;
    const cv::Scalar blob_mean = 0;
    network.setInput(blobFromImg, "", blob_scale, blob_mean);

    // feed forward the inputs through the network
    cv::Mat outMat;
    network.forward(outMat);

    // reach row represents one result per detected object
    int numRows = outMat.rows;

    // column arrangement:
    // [x, y, w, h, class_1_score, class_2_score, ..., ... class_N_score]
    int numCols = outMat.cols;
    for (int j = 0; j < numRows; ++j)
    {
        // get scores for each possible class (starting at element 5)
        cv::Mat scores = outMat.row(j).colRange(5, numCols);

        // find indexes of min and max confidence and related index of element.
        cv::Point maxPos;
        double confidence;
        cv::minMaxLoc(scores, 0, &confidence, 0, &maxPos);

        // consider only scores above a minimum threshold
        const double minConfidence = 0.0001;
        if (confidence > minConfidence)
        {
            // parse the coordinates and parameters of this result
            int center_x = (int)(outMat.at<float>(j, 0) * imageOut.cols);
            int center_y = (int)(outMat.at<float>(j, 1) * imageOut.rows);
            int width = (int)(outMat.at<float>(j, 2) * imageOut.cols + 20);
            int height = (int)(outMat.at<float>(j, 3) * imageOut.rows + 100);

            // calculate top left
            int left = center_x - width / 2;
            int top = center_y - height / 2;

            // annotate the image
            std::stringstream ss;
            ss << maxPos.x;
            std::string clas = ss.str();
            int color = maxPos.x * 10;
            cv::putText(imageOut, clas, cv::Point(left, top), 1, 2, cv::Scalar(color, 255, 255), 2, false);
            std::stringstream ss2;
            ss << confidence;
            std::string conf = ss.str();
            cv::rectangle(imageOut, cv::Rect(left, top, width, height), cv::Scalar(color, 0, 0), 2, 8, 0);
        }
    }

    // return true on success
    return true;
}

// void drawInference(int classID, float confidence, c)

/*******************************************************************************************************************/ /**
 * @brief program entry point
 * @param[in] argc number of command line arguments
 * @param[in] argv string array of command line arguments
 * @return return code (0 for normal termination)
 * @author Christoper D. McMurrough
 **********************************************************************************************************************/
int main(int argc, char **argv)
{
    // store video capture parameters
     std:strin videoFileName = 0;

    validate and parse the command line arguments
    if (argc != NUM_COMNMAND_LINE_ARGUMENTS + 1)
    {
        std::printf("USAGE: %s <file_path> \n", argv[0]);
        return 0;
    }
    else
    {
        // videoFileName = argv[1];
        std::printf("Unable to acquire image frame! \n");
    }

    // open the video file
    cv::VideoCapture capture(videoFileName);
    if (!capture.isOpened())
    {
        std::printf("Unable to open video source, terminating program! \n");
        return 0;
    }

    // get the video source parameters
    int captureWidth = static_cast<int>(capture.get(cv::CAP_PROP_FRAME_WIDTH));
    int captureHeight = static_cast<int>(capture.get(cv::CAP_PROP_FRAME_HEIGHT));
    int captureFPS = static_cast<int>(capture.get(cv::CAP_PROP_FPS));
    std::cout << "Video source opened successfully (width=" << captureWidth << " height=" << captureHeight << " fps=" << captureFPS << ")!" << std::endl;

    // create image window
    cv::namedWindow(DISPLAY_WINDOW_NAME, cv::WINDOW_AUTOSIZE);

    // initialize YOLO
    std::string model = "yolov3.weights";
    std::string config = "yolov3.cfg";
    cv::dnn::Net network = cv::dnn::readNet(model, config, "Darknet");
    network.setPreferableBackend(cv::dnn::DNN_BACKEND_DEFAULT);
    network.setPreferableTarget(cv::dnn::DNN_TARGET_OPENCL);

    // process data until program termination
    bool doCapture = true;
    int frameCount = 0;
    while (doCapture)
    {
        // get the start time
        double startTicks = static_cast<double>(cv::getTickCount());

        // attempt to acquire and process an image frame
        cv::Mat captureFrame;
        cv::Mat processedFrame;
        bool captureSuccess = capture.read(captureFrame);
        if (captureSuccess)
        {
            // process the image frame
            processFrame(captureFrame, processedFrame, network);

            // increment the frame counter
            frameCount++;
        }
        else
        {
            std::printf("Unable to acquire image frame! \n");
        }

        // update the GUI window if necessary
        if (captureSuccess)
        {
            cv::imshow(DISPLAY_WINDOW_NAME, processedFrame);

            // get the number of milliseconds per frame
            int delayMs = (1.0 / captureFPS) * 1000;

            // check for program termination
            if (((char)cv::waitKey(1)) == 'q')
            {
                doCapture = false;
            }
        }

        // compute the frame processing time
        double endTicks = static_cast<double>(cv::getTickCount());
        double elapsedTime = (endTicks - startTicks) / cv::getTickFrequency();
        std::cout << "Frame processing time: " << elapsedTime << std::endl;
    }

    // release program resources before returning
    capture.release();
    cv::destroyAllWindows();
}
