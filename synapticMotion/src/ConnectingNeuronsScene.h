//
//  ConnectingNeuronsScene.h
//  synapticMotion
//
//  Created by Johan Bichel Lindegaard on 31/07/14.
//
//

#pragma once

#include "ofMain.h"
#include "ofxContentScene.h"
#include "ofxBiquadFilter.h"
#include "ofxCv.h"

#define MAX_NEURONS 6

// nurbs curves between neurons

class Neuron {
public:
    ofVec2f pos;
    float activity; // a fall causes big activity
    
    ofxBiquadFilter2f filter;
    
    unsigned int label; // direct hooked up to rectTracker
    
};

class Connection {
public:
    // a connection grows between two Neurons
};

class ConnectingNeuronsScene : public ofxContentScene {
    
public:
    
    void setup();
    void draw();
    void debugDraw();
    void update();
    
    void parseOscMessage(ofxOscMessage * m);
    void setGui();
    
    vector<Neuron*> neurons;
    
};
