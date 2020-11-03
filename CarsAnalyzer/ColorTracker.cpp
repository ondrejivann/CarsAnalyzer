//
//  ColorTracker.cpp
//  CarsAnalyzer
//
//  Created by Ondřej Ivan on 31/10/2020.
//

#include "ColorTracker.hpp"

ColorTracker::ColorTracker() {
    
}

cv::Mat ColorTracker::preprocessImage(cv::Mat image) {
    cv::Mat preprocessedImage;
    cv::Mat hsvImage;
    // změna barevného prostoru z BGR do HSV (Hue, Saturation, Value)
    cv::cvtColor(image, hsvImage, cv::COLOR_BGR2HSV);
    // metoda pro izolati konkrétní barvy v obraze. Používáte se ve spojení s barevným modelem HSV
    auto sensitivity = 60;
    cv::inRange(hsvImage, cv::Scalar(0,0,255-sensitivity), cv::Scalar(200, sensitivity, 255), preprocessedImage);
    
    // Erosion - Snaží se odstranit objekty na pozadí - scvrknout stávající objekt na menší bez šumu v okolí. Odřízne pixely šumu
    // Dilation - Opak erosion, snaží se objekt vyhladit tím, že ho rozšíří. Tam kde je šum přidá pixely objektu
    // Kombinací těchto dvou operací vznikají další morphologické operace - otevření a zavření
    
    // otevření (erode-dialte)
    // Odstraní na pozadí drobné objekty, které nesplní určitý trashold
    cv::erode(preprocessedImage, preprocessedImage, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(3,3)));
    cv::dilate(preprocessedImage, preprocessedImage, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(3,3)));
    
    // zavření (dilate-erode)
    // Zavření dír v objektu
    cv::dilate(preprocessedImage, preprocessedImage, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(7,7)));
    cv::erode(preprocessedImage, preprocessedImage, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(7,7)));
    
    return preprocessedImage;
}

std::vector<cv::Point> ColorTracker::getColorPosition(cv::Mat image) {
    std::vector<cv::Point> positions;
    cv::Mat preprocessedImage = preprocessImage(image);
    
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    
    cv::findContours(preprocessedImage, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE, cv::Point(0,0));
    
    for (int i = 0; i < contours.size(); i++) {
        double area = cv::contourArea(contours.at(i));
        if (area > 100) {
            std::vector<cv::Point> polygon;
            cv::approxPolyDP(contours.at(i), polygon, 5, true);
            cv::Rect boundingBox = cv::boundingRect(polygon);
            cv::Point position = cv::Point(boundingBox.x + boundingBox.width/2, boundingBox.y + boundingBox.height/2);
            positions.push_back(position);
        }
    }
    
    return positions;
}

ColorTracker::~ColorTracker() {
    
}
