//
//  ColorTracker.hpp
//  CarsAnalyzer
//
//  Created by Ondřej Ivan on 31/10/2020.
//

#ifndef ColorTracker_hpp
#define ColorTracker_hpp

#include <stdio.h>
#include <opencv2/opencv.hpp>

class ColorTracker{
//private:
    
    
public:
    ColorTracker();
    cv::Mat preprocessImage(cv::Mat image);
    std::vector<cv::Point> getColorPosition(cv::Mat image);
    ~ColorTracker();
};

#endif /* ColorTracker_hpp */
