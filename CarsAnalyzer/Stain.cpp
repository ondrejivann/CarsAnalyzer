//
//  Stain.cpp
//  CarsAnalyzer
//
//  Created by Ondřej Ivan on 01/11/2020.
//

#include "Stain.hpp"

Stain::Stain(cv::Rect boudingBox, cv::Point center) {
    m_boudingBox = boudingBox;
    m_center = center;
    m_diagonal = sqrt(pow(boudingBox.width, 2) + pow(boudingBox.height, 2));
    m_stillBeingTracked = true;
}
