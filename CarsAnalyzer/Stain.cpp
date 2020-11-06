//
//  Stain.cpp
//  CarsAnalyzer
//
//  Created by Ondřej Ivan on 01/11/2020.
//

#include "Stain.hpp"

Stain::Stain(std::vector<cv::Point> polygon) {
    m_polygon = polygon;
    m_boudingBox = cv::boundingRect(polygon);
    m_center = cv::Point(m_boudingBox.x + m_boudingBox.width/2, m_boudingBox.y + m_boudingBox.height/2);
    m_diagonal = sqrt(pow(m_boudingBox.width, 2) + pow(m_boudingBox.height, 2));
    m_stillBeingTracked = true;
    m_speedStillBeingTracked = true;
    m_ignore = false;
    m_timeStampStart = -1;
    m_timeStampEnd = -1;
    m_speed = -1.0;
}
