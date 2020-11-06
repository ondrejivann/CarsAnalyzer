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
    m_numOfWhiteCar = 0;
    m_horizontalLine = windowSize.height * 0.75;
    m_verticalLine = windowSize.width *  0.63;
}

void Engine::run() {
    for (;;) {
        std::vector<std::string> reports;
        captureCameraFrame();
        
        std::vector<Stain> currentStains = m_objectIsolator.getObjects(cameraFrame, 1000);
        
        for (auto car: currentStains){
            cv::rectangle(cameraFrame, car.m_boudingBox, cv::Scalar(200,10,10), 2);
        }
        /*
        std::vector<Stain> stainsHistory;
        for (auto car: m_existingStain){
            stainsHistory.push_back(car);
        }
         */
        
        currentStainsToExistingStains(currentStains, m_existingStain);
        bool stainCrossedTheLine = stainsCrossedTheLine(m_existingStain, m_horizontalLine, m_verticalLine, m_numOfLesnickaDirectionCar, m_numOfPionyrskaDirectionCar);
        
        if (stainCrossedTheLine) {
            cv::line(cameraFrame, cv::Point(0, m_horizontalLine), cv::Point(windowSize.width, m_horizontalLine), cv::Scalar(0,0,200), 2);
        } else {
            cv::line(cameraFrame, cv::Point(0, m_horizontalLine), cv::Point(windowSize.width, m_horizontalLine), cv::Scalar(0,200,0), 2);
        }
        
        cv::line(cameraFrame, cv::Point(0, windowSize.height*0.4), cv::Point(windowSize.width, windowSize.height*0.4), cv::Scalar(0,200,0), 2);
        
        reports.push_back("Smer Lesnicka: " + std::to_string(m_numOfLesnickaDirectionCar));
        reports.push_back("Smer Pionyrska: " + std::to_string(m_numOfPionyrskaDirectionCar));
        
        for (auto car: m_existingStain){
            //cv::rectangle(cameraFrame, car.m_boudingBox, cv::Scalar(10,10,200), 2);
            
            for (auto carHistory: stainsHistory) {
                if (car.m_center.x == carHistory.m_center.x && carHistory.m_center.y == car.m_center.y) {
                    car.m_center.x = 0;
                    car.m_center.y = 0;
                    /*
                    cv::putText(cameraFrame, std::to_string(car.m_center.y) + " / " + std::to_string(car.m_center_history.y), cv::Point(car.m_center.x, car.m_center.y-20),
                             cv::FONT_HERSHEY_SIMPLEX,
                             0.85,
                             CV_RGB(0, 200, 150),
                             2);
                     */
                }
            }
             
            
             
            if ( car.m_speed != -1 && car.m_center.x > windowSize.width * 0.5) {
                cv::putText(cameraFrame, std::to_string((int)(car.m_speed)) + " km/h", cv::Point(car.m_center.x, car.m_center.y),
                         cv::FONT_HERSHEY_SIMPLEX,
                         0.85,
                         CV_RGB(0, 200, 150),
                         2);
                
            }
        }
    
        
        if (m_speeds.size() > 0) {
            reports.push_back("Prumerna rychlost: " + std::to_string(std::accumulate(m_speeds.begin(), m_speeds.end(), 0)/m_speeds.size()) + " km/h");
        }
        
        
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
                // pocatecni bod a cas pro mereni rychlosti
                stains.at(i).m_timeStampStart = getUnixTimestamp();
                stains.at(i).m_pointStart = stains.at(i).m_center;
            }
        }
 
        if (stains.at(i).m_speedStillBeingTracked == true) {
            if (stains.at(i).m_center.x > windowSize.width * 0.5
                && stains.at(i).m_center.y <= windowSize.height * 0.4
                && stains.at(i).m_speed == -1
                ) {
                int v = getCarSpeed(stains.at(i).m_pointStart.y,
                                    stains.at(i).m_center.y,
                                    stains.at(i).m_timeStampStart,
                                    stains.at(i).m_timeStampCurrent
                                    );
                
                stains.at(i).m_speed = v;
                if (v > 15.0) {
                    m_speeds.push_back(v);
                }
                
                stains.at(i).m_speedStillBeingTracked = false;
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
        //if (existingStains.at(intIndexOfLeastDistance).m_center.x != 0 && existingStains.at(intIndexOfLeastDistance).m_center.y !=0) {
            if (dblLeastDistance < 100) {
                existingStains.at(intIndexOfLeastDistance).m_boudingBox = currentStain.m_boudingBox;
                existingStains.at(intIndexOfLeastDistance).m_center_history = existingStains.at(intIndexOfLeastDistance).m_center;
                existingStains.at(intIndexOfLeastDistance).m_center = currentStain.m_center;
                existingStains.at(intIndexOfLeastDistance).m_timeStampCurrent = currentStain.m_timeStampCurrent;
                existingStains.at(intIndexOfLeastDistance).m_diagonal = sqrt(pow(currentStain.m_boudingBox.width, 2) + pow(currentStain.m_boudingBox.height, 2));
            } else {
                if (currentStain.m_center.y > windowSize.height * 0.35
                    && currentStain.m_center.y <= windowSize.height * 0.75
                    && currentStain.m_center.x > windowSize.width * 0.55
                    ) {
                    currentStain.m_timeStampStart = getUnixTimestamp();
                    currentStain.m_pointStart = currentStain.m_center;
                }
                existingStains.push_back(currentStain);
            }
        //}
        
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

long int Engine::getUnixTimestamp()
{
    std::chrono::milliseconds ms = std::chrono::duration_cast< std::chrono::milliseconds > (
        std::chrono::system_clock::now().time_since_epoch()
    );
    return ms.count();
}

int Engine::getCarSpeed(int startY, int currentY, long int timeStampStart, long int timeStampCurrent) {
    double k1 = (0.75 - (startY / (double) windowSize.height))/(0.75 - 0.35);
    double s1 = 35 - k1 * 35;
    
    double k2 = ((startY - currentY) / (double)windowSize.height) / ((startY / (double)windowSize.height) - 0.35);
    double s2 = k2 * s1;
    
    long int t = timeStampCurrent - timeStampStart;
    double v = (s2/(t/(double)1000)) * 3.6;
    
    return (int)v;
}

Engine::~Engine() {
    
}
