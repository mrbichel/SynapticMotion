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
    
    brainModel.loadModel("brain.obj"/*"brain.obj"*/);
    
    
    brainMesh = brainModel.getMesh(0);
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
    
    
    
    //light.setPosition(lightpos*ofVec3f(OUTWIDTH,OUTHEIGHT,OUTHEIGHT));
    
    
    light.setPosition(center);
    
    
    //brainModel.update();
    brainModel.setScale(4, 4, 1);
    
}

void BrainWrinkleScene::draw(){
    
    ofBackground(0,0,0);
    
    ofEnableLighting();
    light.enable();
    
    ofPushMatrix();
    
    ofTranslate(OUTWIDTH/2, OUTHEIGHT/2);
    
    //ofRotateX(ofGetElapsedTimef());
    //ofRotateY(ofGetElapsedTimef());
    ofRotateZ(ofGetElapsedTimef());
    
    //brainModel.setPosition(OUTWIDTH/2, OUTHEIGHT/2, 0);
    brainModel.drawFaces();
    
    ofPopMatrix();
    
    light.disable();
    
    ofDisableLighting();

    
    /*ofPushStyle();
    glPushMatrix();
    ofEnableLighting();
    light.enable();
    
    
    
    ofSetColor(255,255,255);
    
    
    ofPushMatrix();
    
    ofRotateX(ofGetElapsedTimef());
    ofRotateY(ofGetElapsedTimef());
    ofRotateZ(ofGetElapsedTimef());

    
    //brainModel.setScale(10, 10, 10);
    //brainModel.setPosition(OUTWIDTH/2, OUTHEIGHT/2, 0);
    //brainModel.drawFaces();
    
    
    ofPopMatrix();
    
    ofSetColor(100,100,100);
    
    //brainModel.drawWireframe();
    
    ofSetColor(255,0,0);
    ofCircle(center, 10);
    
    light.draw();
    
    light.disable();
    
    ofDisableLighting();
    
    glPopMatrix();
    ofPopStyle();*/
    
}

void BrainWrinkleScene::parseOscMessage(ofxOscMessage *m){
}

void BrainWrinkleScene::setGui(){
    
    gui->addSlider("/lightPos/x", 0, 1, &lightpos.x);
    gui->addSlider("/lightPos/y", 0, 1, &lightpos.y);
    gui->addSlider("/lightPos/z", 0, 1, &lightpos.z);
    
}
