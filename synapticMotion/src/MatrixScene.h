//
//  MatrixScene.h
//  synapticMotion
//
//  Created by Johan Bichel Lindegaard on 31/07/14.
//
//

#pragma once

#include "ofMain.h"
#include "ofxContentScene.h"
#include "ofxCv.h"
#include "ofxBiquadFilter.h"


class gridwalkers {
public:
    ofVec2f posOne;
    ofVec2f posTwo;
};

class MatrixScene : public ofxContentScene {
    
public:
    
    void setup();
    void draw();
    void debugDraw();
    void update();
    
    void parseOscMessage(ofxOscMessage * m);
    void setGui();
    
    ofVec2f center;
    
    ofPlanePrimitive plane;
    
    unsigned int centerLabel;
    ofxBiquadFilter2f filter;
    
    ofFloatColor linecolor;
    ofFloatColor closecolor;
    
    float lineAlpha;
    float centerSize;
    
};

