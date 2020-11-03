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

class Stain{
public:
    Stain(cv::Rect boudingBox, cv::Point center);
    cv::Rect m_boudingBox;
    cv::Point m_center;
    double m_diagonal;
    bool m_stillBeingTracked;
};

#endif /* Stain_hpp */
