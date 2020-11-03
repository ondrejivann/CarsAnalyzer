//
//  ObjectTracker.hpp
//  CarsAnalyzer
//
//  Created by Ondřej Ivan on 01/11/2020.
//

#ifndef ObjectTracker_hpp
#define ObjectTracker_hpp

#include <stdio.h>
#include <opencv2/opencv.hpp>

class ObjectTracker {
    
public:
    ObjectTracker();
    std::vector<cv::Point2f> getFeatures(cv::Mat image);
    std::vector<cv::Point2f> trackObject(cv::Mat oldImage, cv::Mat currentImage, std::vector<cv::Point2f> oldPoints);
    void drawPoints(cv::Mat image, std::vector<cv::Point2f> points, cv::Scalar color);
    
};

#endif /* ObjectTracker_hpp */
