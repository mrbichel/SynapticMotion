//
//  TestScene.cpp
//  synapticMotion
//
//  Created by Johan Bichel Lindegaard on 31/07/14.
//
//

#include "TestScene.h"

using namespace ofxCv;

void TestScene::setup(){
    
    name = "TestScene";
    oscAddress = "/bw";
}

void TestScene::update(){
    
    
}

void TestScene::draw(){;
    
    ofBackground(0);
    
    ofSetColor(255);
    
    RectTracker& t = tracker->contourFinder.getTracker();
    
    for(int i = 0; i < t.getCurrentLabels().size(); i++) {
		int label = t.getCurrentLabels()[i];
        
        
        ofVec2f center =  tracker->getOutPos(label);
        ofPushMatrix();
        
        ofTranslate(center.x, center.y);
        ofCircle(0, 0, 10);
        ofPopMatrix();
        
        
    }
    
    
    
}

void TestScene::parseOscMessage(ofxOscMessage *m){
}

void TestScene::setGui(){
    
    //gui->addSlider("/gravity/x", -1, 1, &gravity.x);
    //gui->addSlider("/gravity/y", -1, 1, &gravity.y);
    
}
