#include <iostream>
#include <depthai/depthai.hpp>
#include <opencv2/opencv.hpp>

#define R_W 1366
#define R_H 768

using namespace std;

int main(){

    dai::Pipeline pipeline;
    auto colorCam = pipeline.create<dai::node::ColorCamera>();
    auto xlinkOut = pipeline.create<dai::node::XLinkOut>();
    xlinkOut->setStreamName("preview");
    colorCam->setInterleaved(true);
    colorCam->setPreviewSize(R_W, R_H);
    colorCam->preview.link(xlinkOut->input);

    try {
        dai::Device device(pipeline);

        auto preview = device.getOutputQueue("preview");

        cv::Mat frame;
        while (true) {

            auto imgFrame = preview->get<dai::ImgFrame>();

            cv::Mat input(imgFrame->getHeight(), imgFrame->getWidth(), CV_8UC3, imgFrame->getData().data());

            cv::UMat output;
            cvtColor( input, output, cv::COLOR_BGR2GRAY );

            cv::imshow("preview", output);

            if (cv::waitKey(1) == 'q') return 0;

        }
    } catch (const std::runtime_error& err) {
        std::cout << err.what() << std::endl;
    }

    return 0;
}