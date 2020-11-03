//
//  ObjectIsolator.cpp
//  CarsAnalyzer
//
//  Created by Ondřej Ivan on 31/10/2020.
//

#include "ObjectIsolator.hpp"

ObjectIsolator::ObjectIsolator() {
    backgroundSubtractor = cv::createBackgroundSubtractorMOG2(100, 250.0, true);
}

std::vector<Stain> ObjectIsolator::getObjects(cv::Mat image, int minContourArea) {
    std::vector<Stain> currentStains;
    
    cv::Mat originFrame;
    image.copyTo(originFrame);

    backgroundSubtractor->apply(image, image);
    cv::threshold(image, image, 170, 255.0, cv::THRESH_BINARY);
    for (unsigned int i = 0; i < 2; i++) {
        cv::dilate(image, image, structuringElement3x3);
        cv::dilate(image, image, structuringElement9x9);
        cv::erode(image, image, structuringElement9x9);
    }
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::findContours(image, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
    
    std::vector<std::vector<cv::Point> > convexHulls(contours.size());
    for (unsigned int i = 0; i < contours.size(); i++) {
        cv::convexHull(contours[i], convexHulls[i]);
    }
    
    cv::Mat imageDraw(cv::Size(1000,750), CV_8UC3, cv::Scalar(0,0,0));
    for (int i=0; i<contours.size(); i++) {
        if (cv::contourArea(contours.at(i)) > minContourArea) {
            cv::drawContours(imageDraw, contours, i, cv::Scalar(255,255,255));
        }
    }
    
    cv::imshow("Contour", imageDraw);
    
    for (int i = 0; i < contours.size(); i++) {
        double area = cv::contourArea(contours.at(i));
        if (area > minContourArea) {
            std::vector<cv::Point> polygon;
            cv::approxPolyDP(contours.at(i), polygon, 5, true);
            cv::Rect boundingBox = cv::boundingRect(polygon);
            cv::Point position = cv::Point(boundingBox.x + boundingBox.width/2, boundingBox.y + boundingBox.height/2);
            Stain stain = Stain(boundingBox, position);
            currentStains.push_back(stain);
        }
    }
    
    return currentStains;
}

