//
//  Engine.cpp
//  CarsAnalyzer
//
//  Created by Ondřej Ivan on 28/10/2020.
//

#include "Engine.hpp"

Engine::Engine() {
    videoCap.open(filePath);
    windowSize = cv::Size(1000,750);
    m_numOfLesnickaDirectionCar = 0;
    m_numOfPionyrskaDirectionCar = 0;
}

void Engine::run() {
    
    for (;;) {
        std::vector<std::string> reports;
        int horizontalLine = windowSize.height * 0.8;
        int verticalLine = windowSize.width *  0.63;
        captureCameraFrame();
        
        /*
        cv::Mat whiteColorImage = colorTracker.preprocessImage(cameraFrame);
        
        cv::imshow("White color", whiteColorImage);
        
        std::vector<Stain> whiteStains = objectIsolator.getObjects(whiteColorImage, 2500);
        
        for (auto car: whiteStains){
            cv::rectangle(cameraFrame, car.m_boudingBox, cv::Scalar(0,255,0));
        }
        */
        
        std::vector<Stain> currentStains = objectIsolator.getObjects(cameraFrame, 3000);
        
        for (auto car: currentStains){
            cv::rectangle(cameraFrame, car.m_boudingBox, cv::Scalar(0,255,0));
        }
        
        currentStainsToExistingStains(currentStains, m_existingStain);
        bool stainCrossedTheLine = stainsCrossedTheLine(m_existingStain, horizontalLine, verticalLine, m_numOfLesnickaDirectionCar, m_numOfPionyrskaDirectionCar);
        
        if (stainCrossedTheLine) {
            cv::line(cameraFrame, cv::Point(0, horizontalLine), cv::Point(windowSize.width, horizontalLine), cv::Scalar(0,0,200), 2);
        } else {
            cv::line(cameraFrame, cv::Point(0, horizontalLine), cv::Point(windowSize.width, horizontalLine), cv::Scalar(0,200,0), 2);
        }
        
        reports.push_back("Smer Lesnicka: " + std::to_string(m_numOfLesnickaDirectionCar));
        reports.push_back("Smer Pionyrska: " + std::to_string(m_numOfPionyrskaDirectionCar));
         
         
        showReports(cameraFrame, reports);
        
        cv::imshow("Car counter", cameraFrame);

        char c = cv::waitKey(33);
        if (c == 27) {
            break;
        }
    }
}

void Engine::captureCameraFrame() {
    videoCap >> cameraFrame;
    cv::resize(cameraFrame, cameraFrame, windowSize);
}

bool Engine::stainsCrossedTheLine(std::vector<Stain> &stains, int horizontalLinePosition, int verticalLinePosition, int &numOfLesnickaDirectionCar, int &numOfPionyrskaDirectionCar) {
    bool carCrossedTheLine = false;
    
    for (int i = 0; i < stains.size(); i++) {
        if (stains.at(i).m_stillBeingTracked == true) {
            // Bude počítat auta směrem na Pionyrskou (levá část)
            if (stains.at(i).m_center.x < verticalLinePosition && stains.at(i).m_center.y > horizontalLinePosition) {
                numOfPionyrskaDirectionCar++;
                stains.at(i).m_stillBeingTracked = false;
                carCrossedTheLine = true;
            // Bude počítat auta směrem na Lesnickou (pravá část)
            } else if (stains.at(i).m_center.x > verticalLinePosition && stains.at(i).m_center.y < horizontalLinePosition) {
                numOfLesnickaDirectionCar++;
                stains.at(i).m_stillBeingTracked = false;
                carCrossedTheLine = true;
            }
        }
    }
    return carCrossedTheLine;
}

void Engine::currentStainsToExistingStains(std::vector<Stain> currentStains, std::vector<Stain> &existingStains) {
    for (auto currentStain: currentStains) {
        int intIndexOfLeastDistance = 0;
        double dblLeastDistance = 100000.0;
        
        for (unsigned int i = 0; i < existingStains.size(); i++) {
            double dblDistance = distanceBetweenPoints(currentStain.m_center, existingStains[i].m_center);
            if (dblDistance < dblLeastDistance) {
                dblLeastDistance = dblDistance;
                intIndexOfLeastDistance = i;
            }
        }
        if (dblLeastDistance < 75.0) {
            existingStains.at(intIndexOfLeastDistance).m_boudingBox = currentStain.m_boudingBox;
            existingStains.at(intIndexOfLeastDistance).m_center = currentStain.m_center;
            existingStains.at(intIndexOfLeastDistance).m_diagonal = sqrt(pow(currentStain.m_boudingBox.width, 2) + pow(currentStain.m_boudingBox.height, 2));
        } else {
            existingStains.push_back(currentStain);
        }
    }
}

double Engine::distanceBetweenPoints(cv::Point point1, cv::Point point2){
    int intX = abs(point1.x - point2.x);
    int intY = abs(point1.y - point2.y);

    return(sqrt(pow(intX, 2) + pow(intY, 2)));
}

void Engine::showReports(cv::Mat &frame, std::vector<std::string> &reports){
    cv::Mat overlay;
    frame.copyTo(overlay);
    cv::rectangle(overlay, cv::Point(730,20), cv::Point(980, 250), cv::Scalar(0,0,0), -1);
    cv::addWeighted(overlay, 0.5, frame, 1 - 0.5, 0, frame);
    
    int firstReportHeight = 45;
    for (auto report: reports) {
        cv::putText(frame, report, cv::Point(750, firstReportHeight),
                    cv::FONT_HERSHEY_SIMPLEX,
                    0.5,
                    CV_RGB(255, 255, 255),
                    1);
        firstReportHeight += 25;
    }
}

Engine::~Engine() {
    
}
