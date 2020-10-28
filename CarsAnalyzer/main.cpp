//
//  main.cpp
//  CarsAnalyzer
//
//  Created by Ondřej Ivan on 28/10/2020.
//

#include <iostream>
#include <opencv2/opencv.hpp>
#include "Engine.hpp"

int main(int argc, const char * argv[]) {
    
    Engine engine = Engine();
    engine.run();
    
    return 0;
}
