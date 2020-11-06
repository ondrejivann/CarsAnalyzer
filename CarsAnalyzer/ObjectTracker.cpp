//
//  ObjectTracker.cpp
//  CarsAnalyzer
//
//  Created by Ondřej Ivan on 01/11/2020.
//

#include "ObjectTracker.hpp"

ObjectTracker::ObjectTracker() {
    
}

std::vector<cv::Point2f> ObjectTracker::getFeatures(cv::Mat image) {
    std::vector<cv::Point2f> points;
    cv::cvtColor(image, image, cv::COLOR_BGR2GRAY);
    cv::goodFeaturesToTrack(image, points, 1000, 0.01, 25);
    
    return points;
}

std::vector<cv::Point2f> ObjectTracker::trackObject(cv::Mat oldImage, cv::Mat currentImage, std::vector<cv::Point2f> oldPoints) {
    std::vector<cv::Point2f> newPoints;
    if (oldPoints.size() > 0) {
        cv::cvtColor(oldImage, oldImage, cv::COLOR_BGR2GRAY);
        cv::cvtColor(currentImage, currentImage, cv::COLOR_BGR2GRAY);
        std::vector<uchar> status;
        std::vector<float> error;
        cv::TermCriteria criteria = cv::TermCriteria((cv::TermCriteria::COUNT) + (cv::TermCriteria::EPS), 10, 0.03);
        cv::calcOpticalFlowPyrLK(oldImage, currentImage, oldPoints, newPoints, status, error);
    }
    
    return newPoints;
}

void ObjectTracker::drawPoints(cv::Mat image, std::vector<cv::Point2f> points, cv::Scalar color) {
    for (int i = 0; i < points.size(); i++) {
        cv::circle(image, points.at(i), 4, color, -1);
    }
}
