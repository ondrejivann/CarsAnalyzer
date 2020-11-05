//
//  ObjectIsolator.hpp
//  CarsAnalyzer
//
//  Created by Ondřej Ivan on 31/10/2020.
//

#ifndef ObjectIsolator_hpp
#define ObjectIsolator_hpp

#include <stdio.h>
#include <opencv2/opencv.hpp>
#include "ObjectTracker.hpp"
#include "Stain.hpp"

class ObjectIsolator {
private:
    cv::Ptr<cv::BackgroundSubtractor> backgroundSubtractor;
    cv::Size m_windowSize;
    
    const cv::Mat structuringElement3x3 = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(3, 3));
    const cv::Mat structuringElement5x5 = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5));
    const cv::Mat structuringElement7x7 = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(7, 7));
    const cv::Mat structuringElement9x9 = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(9, 9));
    const cv::Mat structuringElement11x11 = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(11, 11));
    
public:
    ObjectIsolator();
    std::vector<Stain> getObjects(cv::Mat image, int minContourArea);
};

#endif /* ObjectIsolator_hpp */
