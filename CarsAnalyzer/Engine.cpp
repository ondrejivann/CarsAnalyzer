//
//  Engine.cpp
//  CarsAnalyzer
//
//  Created by Ondřej Ivan on 28/10/2020.
//

#include "Engine.hpp"

Engine::Engine() {
    videoCap.open(filePath);
}

void Engine::run() {
    for (;;) {
        captureCameraFrame();
        
        cv::imshow("Camera Stream", cameraFrame);
        
        char c = cv::waitKey(33);
        if (c == 27) {
            break;
        }
    }
}

void Engine::captureCameraFrame() {
    videoCap >> cameraFrame;
}

Engine::~Engine() {
    
}
