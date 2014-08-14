//
//  BrainWrinkleScene.cpp
//  synapticMotion
//
//  Created by Johan Bichel Lindegaard on 31/07/14.
//
//

#include "BrainWrinkleScene.h"

void BrainWrinkleScene::setup(){
    
    name = "BrainWrinkleScene";
    oscAddress = "/bw";
    
    filter.setQ(0.5);
    filter.setFc(0.05);
    
    brainModel.loadModel("brain.3ds", 1);
}

void BrainWrinkleScene::update(){
    
    ofxCv::RectTracker& t = tracker->contourFinder.getTracker();
    
    if(t.existsCurrent(centerLabel)) {
        filter.update(tracker->getOutPos(centerLabel));
    } else {
        if(t.getCurrentLabels().size() > 0) {
            centerLabel = t.getCurrentLabels()[0];
        }
    }
    center = filter.getValue();
    
}

void BrainWrinkleScene::draw(){
    
    
    

    
    ofBackground(0,0,0);
    ofSetColor(255,255,255);
    
    brainModel.setPosition(0, 0, 0);
    
    
    brainModel.draw();
    
    ofSetColor(255,0,0);
    ofCircle(center, 10);
    
    
    
    
}

void BrainWrinkleScene::parseOscMessage(ofxOscMessage *m){
}

void BrainWrinkleScene::setGui(){
    
    //gui->addSlider("/gravity/x", -1, 1, &gravity.x);
    //gui->addSlider("/gravity/y", -1, 1, &gravity.y);
    
}
