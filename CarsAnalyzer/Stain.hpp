//
//  Stain.hpp
//  CarsAnalyzer
//
//  Created by Ondřej Ivan on 01/11/2020.
//

#ifndef Stain_hpp
#define Stain_hpp

#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <ctime>

class Stain{
public:
    Stain(std::vector<cv::Point> polygon);
    cv::Rect m_boudingBox;
    cv::Point m_center;
    double m_diagonal;
    bool m_stillBeingTracked;
    std::vector<cv::Point> m_polygon;
    std::vector<cv::Point> m_center_history;
    long int m_startTime;
    long int m_endTime;
    double m_speed;
};

#endif /* Stain_hpp */
