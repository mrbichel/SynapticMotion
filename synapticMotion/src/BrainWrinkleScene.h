//
//  BrainWrinkleScene.h
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
#include "ofx3DModelLoader.h"
#include "Defines.h"

class BrainWrinkleScene : public ofxContentScene {
    
public:
    
    void setup();
    void draw();
    void debugDraw();
    void update();
    
    void parseOscMessage(ofxOscMessage * m);
    void setGui();
    
    ofVec2f center;
    unsigned int centerLabel;
    ofxBiquadFilter2f filter;
    
    ofx3DModelLoader brainModel;
    
};
