//
//  Engine.hpp
//  CarsAnalyzer
//
//  Created by Ondřej Ivan on 28/10/2020.
//

#ifndef Engine_hpp
#define Engine_hpp

#include <stdio.h>
#include <opencv2/videoio.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <string>

class Engine {
public:
    Engine();
    void run();
    const std::string filePath = "/Users/ondrejivan/Documents/School/LVR/Project_1/CarsAnalyzer/video_pro_analyzu.mp4";
    ~Engine();
    
private:
    cv::VideoCapture videoCap;
    cv::Mat cameraFrame;
    
    void captureCameraFrame();
    
};

#endif /* Engine_hpp */
