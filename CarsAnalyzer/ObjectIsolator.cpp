//
//  ObjectIsolator.cpp
//  CarsAnalyzer
//
//  Created by Ondřej Ivan on 31/10/2020.
//

#include "ObjectIsolator.hpp"

ObjectIsolator::ObjectIsolator() {
    m_windowSize = cv::Size(1000, 750);
    backgroundSubtractor = cv::createBackgroundSubtractorMOG2(100, 250.0, true);
}

std::vector<Stain> ObjectIsolator::getObjects(cv::Mat image, int minContourArea) {
    std::vector<Stain> currentStains;
    
    cv::Mat originFrame;
    image.copyTo(originFrame);

    backgroundSubtractor->apply(image, image);
    cv::threshold(image, image, 170, 255.0, cv::THRESH_BINARY);
    cv::imshow("back", image);
    for (unsigned int i = 0; i < 2; i++) {
        cv::dilate(image, image, structuringElement3x3);
        cv::dilate(image, image, structuringElement9x9);
        cv::erode(image, image, structuringElement9x9);
    }
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::findContours(image, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
    
    std::vector<std::vector<cv::Point>> contoursToPush;
    
    for (int i = 0; i < contours.size(); i++) {
        double area = cv::contourArea(contours.at(i));
        if (area > minContourArea) {
            std::vector<cv::Point> polygon;
            cv::approxPolyDP(contours.at(i), polygon, 5, true);
            //std::vector<cv::Point> polygonConvexHull;
            //cv::convexHull(polygon, polygonConvexHull);
            //contoursToPush.push_back(polygonConvexHull);
            Stain stain = Stain(polygon);
            stain.m_timeStampCurrent = getUnixTimestamp();
            currentStains.push_back(stain);
        }
    }
    
    cv::Mat imageDraw(m_windowSize, CV_8UC3, cv::Scalar(0,0,0));
    for (int i  = 0; i < contours.size(); i++) {
        if (cv::contourArea(contours.at(i)) > minContourArea) {
            cv::drawContours(imageDraw, contours, i, cv::Scalar(255,255,255));
        }
    }
    
    cv::imshow("Contour", imageDraw);
    
    return currentStains;
}

long int ObjectIsolator::getUnixTimestamp()
{
    std::chrono::milliseconds ms = std::chrono::duration_cast< std::chrono::milliseconds > (
        std::chrono::system_clock::now().time_since_epoch()
    );
    return ms.count();
    //time_t t = std::time(0);
    //long int now = static_cast<long int> (t);
    //return now;
}

