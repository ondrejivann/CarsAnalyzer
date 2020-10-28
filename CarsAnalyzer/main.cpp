//
//  main.cpp
//  CarsAnalyzer
//
//  Created by Ondřej Ivan on 28/10/2020.
//

#include <iostream>
#include <opencv2/opencv.hpp>

int main(int argc, const char * argv[]) {
    
    cv::VideoCapture videoCapture(0);
    if (!videoCapture.isOpened()) {
        return -1;
    }
    
    for (;;) {
        cv::Mat frame;
        videoCapture >> frame;
        cv::imshow("Camera Stream", frame);
        if (cv::waitKey(30) >= 0) break;
    }
    return 0;
}
