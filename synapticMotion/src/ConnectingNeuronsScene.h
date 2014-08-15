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

#define MAX_NEURONS 7

// nurbs curves between neurons

class Neuron {
public:
    ofVec2f pos;
    int uid;
    float activity; // a fall causes big activity
    
    ofxBiquadFilter2f filter;
    
    unsigned int label; // direct hooked up to rectTracker
    float area; // dont add any connections before over a certain size
    
    vector<Neuron*> connected;
    
};

class ConnectingNeuronsScene : public ofxContentScene {
    
public:
    
    void setup();
    void draw();
    void debugDraw();
    void update();
    
    void parseOscMessage(ofxOscMessage * m);
    void setGui();
    
    void connect(Neuron * one) {
        
        // connect to the closest other that we are not already connected to
        float dist = 10000;
        Neuron * connector;
        
        for(int i=0; i<neurons.size(); i++) {
            int alreadyConnected = false;
            
            for(int c=0;c<one->connected.size();c++) {
                if(neurons[i]->uid == one->connected[c]->uid) {
                    alreadyConnected = true;
                    break;
                }
            }
            
            if(!alreadyConnected && neurons[i]->uid != one->uid) {
                float d = neurons[i]->pos.distance(one->pos);
                if(d<dist && d > 2) connector = neurons[i];
            }
        }
        
        one->connected.push_back(connector);
        
    }
    vector<Neuron*> neurons;

    
    float fallSizeThreshold;
    
};
