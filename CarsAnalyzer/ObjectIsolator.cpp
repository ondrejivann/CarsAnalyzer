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
    backgroundSubtractorWhite = cv::createBackgroundSubtractorMOG2();
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
    
    for (int i = 1; i < currentStains.size(); i++) {
        // leva strana
        if (currentStains.at(i).m_center.x < m_windowSize.width * 0.6) {
            // omezeni pro mereni leve strany
            if (currentStains.at(i).m_center.y < m_windowSize.height * 0.8 && currentStains.at(i).m_center.y > m_windowSize.height * 0.27) {
                currentStains.at(i).m_isWhite = isStainWhite(originFrame, imageDraw, currentStains.at(i));
            }
            // prava strana
        } else {
            currentStains.at(i).m_isWhite = isStainWhite(originFrame, imageDraw, currentStains.at(i));
        }
    }
    
    return currentStains;
}

long int ObjectIsolator::getUnixTimestamp()
{
    std::chrono::milliseconds ms = std::chrono::duration_cast< std::chrono::milliseconds > (
        std::chrono::system_clock::now().time_since_epoch()
    );
    return ms.count();
}

bool ObjectIsolator::isStainWhite(cv::Mat originFrame, cv::Mat bgSubtractionFrame, Stain stain) {
    bool isStainWhite = false;
    
    cv::Mat croppedOriginFrame(originFrame, stain.m_boudingBox);
    cv::Mat croppedBgSubFrame(bgSubtractionFrame, stain.m_boudingBox);
    cv::Mat hsvFrame;
    cv::Mat preprocessedImage;

    cv::cvtColor(croppedOriginFrame, hsvFrame, cv::COLOR_BGR2HSV);
    
    auto sensitivity = 25;
    cv::inRange(hsvFrame, cv::Scalar(0,0,255-sensitivity), cv::Scalar(200, sensitivity, 255), preprocessedImage);
    
    cv::erode(preprocessedImage, preprocessedImage, structuringElement3x3);
    cv::dilate(preprocessedImage, preprocessedImage, structuringElement3x3);
    
    cv::dilate(preprocessedImage, preprocessedImage, structuringElement5x5);
    cv::erode(preprocessedImage, preprocessedImage, structuringElement5x5);
    
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    
    cv::findContours(preprocessedImage, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE, cv::Point(0,0));
    
    std::vector<std::vector<cv::Point>> polygons;
    
    std::vector<cv::Point> polygonSum;
    double areaSum = 0.0;
    for (int i = 0; i < contours.size(); i++) {
        double area = cv::contourArea(contours.at(i));
            std::vector<cv::Point> polygon;
            cv::approxPolyDP(contours.at(i), polygon, 5, true);
            polygons.push_back(polygon);
            areaSum += area;
    }
    double windowSizeTotal = croppedBgSubFrame.total();
    double carContourPerc = cv::contourArea(stain.m_polygon) / windowSizeTotal;
    double whiteContourPerc = areaSum / windowSizeTotal;
    
    double pomer;
    
    if (carContourPerc > whiteContourPerc) {
        pomer = whiteContourPerc/carContourPerc;
    } else {
        pomer = carContourPerc/whiteContourPerc;
    }
    
    if (pomer > 0.1) {
        isStainWhite = true;
    }

    return isStainWhite;
}
