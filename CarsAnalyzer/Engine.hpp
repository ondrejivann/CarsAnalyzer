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
#include "ColorTracker.hpp"
#include "ObjectIsolator.hpp"
#include "Stain.hpp"

class Engine {
public:
    Engine();
    void run();
    const std::string filePath = "/Users/ondrejivan/Documents/School/LVR/Project_1/CarsAnalyzer/video_pro_analyzu.mp4";
    ~Engine();
    
private:
    cv::VideoCapture videoCap;
    cv::Mat cameraFrame;
    cv::Size windowSize;
    int m_numOfLesnickaDirectionCar;
    int m_numOfPionyrskaDirectionCar;
    std::vector<Stain> m_existingStain;
    bool firstFrame;
    void captureCameraFrame();
    void showReports(cv::Mat &frame, std::vector<std::string> &reports);
    double distanceBetweenPoints(cv::Point point1, cv::Point point2);
    void currentStainsToExistingStains(std::vector<Stain> currentStains, std::vector<Stain> &existingStains);
    bool stainsCrossedTheLine(std::vector<Stain> &stains, int horizontalLinePosition, int verticalLinePosition, int &numOfLesnickaDirectionCar, int &numOfPionyrskaDirectionCar);
    
    /*
     Izolace pohybujiciho se objektu
     */
    ObjectIsolator objectIsolator;
    
    ColorTracker colorTracker;
    
};

#endif /* Engine_hpp */
