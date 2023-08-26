#include <iostream>
#include <opencv2/opencv.hpp>
#include <unistd.h>

int main()
{

    std::cout << "Built with OpenCV " << CV_VERSION << std::endl;

	  // 3种打开方式，3选一
	  // 只有方式1能够正确打开摄像头
     
    // 打开方式1：
    cv::VideoCapture capture=cv::VideoCapture(0);
    // capture.open(0);
    
    // 打开方式2：
    // cv::VideoCapture capture(0);
    // capture.open("/dev/video0", cv::CAP_V4L2);
    
    // 打开方式3：
    // cv::VideoCapture capture("/dev/video0", cv::CAP_V4L2);
    // capture.open("/dev/video0", cv::CAP_V4L2);


    // capture.open("/home/topeet/Documents/TOB/video/down.avi"); // 打开视频文件

    

    bool isOpenCamera = capture.isOpened();
    if (!isOpenCamera)
    {
        std::cout << "the 'open' funtion fails to open the OV5695" << std::endl;
        return -1;
    }
    // capture.set(cv::CAP_PROP_FOURCC, cv::VideoWriter::fourcc('M', 'J', 'P', 'G'));
    capture.set(cv::CAP_PROP_FOURCC, cv::VideoWriter::fourcc('N', 'V', '1', '2'));
    // capture.set(cv::CAP_PROP_FOURCC, cv::VideoWriter::fourcc('M', 'J', 'P', 'G'));
    capture.set(cv::CAP_PROP_FRAME_WIDTH, 800);
    capture.set(cv::CAP_PROP_FRAME_HEIGHT, 1280);
    capture.set(cv::CAP_PROP_FPS, 30);

    cv::Mat frame;
    while (1)
    {
        capture >> frame;
        // capture.read(frame); // 读取视频帧
        if (!frame.empty())
        {
            cv::imshow("frame", frame);
        }
        else
        {
            std::cout << "fail to open the OV5695! The frame is empty!!" << std::endl;
            break;
        }
        if (cv::waitKey(1) == 27)
            break;
        sleep(1);
    }

    return 0;
}
