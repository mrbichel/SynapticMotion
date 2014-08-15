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
        n->uid = i;
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
            n->area = t.getCurrent(n->label).area();
            
        } else {
            
            n->area = 0;
            
            if(t.getCurrentLabels().size() > i) {
                n->label = t.getCurrentLabels()[i];
            }
        }
        n->pos = n->filter.value();
        
        
        if(n->connected.size() == 0 && n->area > fallSizeThreshold) {
            connect(n);
            connect(n);
            connect(n);
            connect(n);
            connect(n);
            connect(n);
            connect(n);
        }
        
        if(n->connected.size() < 7) {
            //add more ocnnections over time
        
        }

    }
    
}

void ConnectingNeuronsScene::draw(){
    
    ofBackground(0,0,0);
    
    for(int i=0; i<MAX_NEURONS; i++) {
        
        Neuron * n = neurons[i];
        ofCircle(n->pos,5);
        
    }
    
    
    ofSetColor(255);
    
    ofNoFill();
    
    for(int i=0; i<neurons.size(); i++) {
        
        Neuron * n = neurons[i];
        ofCircle(n->pos,20);
        
        
        for(int c=0;c<n->connected.size();c++) {
            Neuron * co = n->connected[c];
            
            //ofLine(n->pos, co->pos);
            
            float a = sin((ofGetElapsedTimeMillis()+((i+c)*200)) / 20000.0 + ((i+c)*200)) * TWO_PI;
            
            float radius = ofSignedNoise((i*c)/ofGetElapsedTimeMillis()) + co->pos.distance(n->pos)/4;
            
           // n->pos.getInterpolated
            
            ofVec2f c1 =  n->pos.getInterpolated(co->pos, 0.2) + ofVec2f(sin(a), cos(a)) * radius;
            ofVec2f c2 =  co->pos.getInterpolated(n->pos, 0.2) + ofVec2f(sin(a), cos(a)) * radius;
            
            ofSetColor(255,255,255,255);
            ofCircle(c1,10);
            ofCircle(c2,10);
            
            ofSetLineWidth(2);
            
            ofSetColor(20,20,255);
            ofBezier(n->pos.x, n->pos.y,
                     c1.x, c1.y,
                     c2.x, c2.y,
                     co->pos.x, co->pos.y);
        }
        
    }
    
    
    

    
}

void ConnectingNeuronsScene::parseOscMessage(ofxOscMessage *m){
}

void ConnectingNeuronsScene::setGui(){
    
     gui->addSlider("/fallSizeThreshold/x", 0, 200, &fallSizeThreshold);
    //gui->addSlider("/gravity/y", -1, 1, &gravity.y);
    
}
