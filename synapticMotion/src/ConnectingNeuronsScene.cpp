//
//  ConnectingNeuronsScene.cpp
//  synapticMotion
//
//  Created by Johan Bichel Lindegaard on 31/07/14.
//
//

#include "ConnectingNeuronsScene.h"

void ConnectingNeuronsScene::setup(){
    
    name = "ConnectingNeuronsScene";
    oscAddress = "/cn";
    
    for(int i=0; i<MAX_NEURONS; i++) {
        Neuron * n = new Neuron();
                
        n->filter.setQ(0.5);
        n->filter.setFc(0.05);
        
        neurons.push_back(n);
    }
}

void ConnectingNeuronsScene::update(){
    
 
    // if there is a fall trigger midi sound and animation along connections
    ofxCv::RectTracker& t = tracker->contourFinder.getTracker();

    for(int i=0; i<MAX_NEURONS; i++) {
        
        Neuron * n = neurons[i];
    
        if(t.existsCurrent(n->label)) {
            n->filter.update(tracker->getOutPos(n->label));
        } else {
            if(t.getCurrentLabels().size() > i) {
                n->label = t.getCurrentLabels()[i];
            }
        }
        n->pos = n->filter.getValue();
    }
    
}

void ConnectingNeuronsScene::draw(){
    
    ofBackground(0,0,0);
    
    for(int i=0; i<MAX_NEURONS; i++) {
        Neuron * n = neurons[i];

        ofCircle(n->pos,5);
    }

    
}

void ConnectingNeuronsScene::parseOscMessage(ofxOscMessage *m){
}

void ConnectingNeuronsScene::setGui(){
    
    //gui->addSlider("/gravity/x", -1, 1, &gravity.x);
    //gui->addSlider("/gravity/y", -1, 1, &gravity.y);
    
}
