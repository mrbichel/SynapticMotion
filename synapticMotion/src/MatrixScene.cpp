//
//  MatrixScene.cpp
//  synapticMotion
//
//  Created by Johan Bichel Lindegaard on 31/07/14.
//
//

#include "MatrixScene.h"


void MatrixScene::setup(){
    
    name = "MatrixScene";
    oscAddress = "/ma";
    
    plane = ofPlanePrimitive(OUTWIDTH, OUTHEIGHT, 10*10,6*10);
    plane.setPosition(OUTWIDTH/2, OUTHEIGHT/2, 0);
    
    
    filter.setQ(0.5);
    filter.setFc(0.05);
}

void MatrixScene::update(){
    
    ofxCv::RectTracker& t = tracker->contourFinder.getTracker();
    
    if(t.existsCurrent(centerLabel)) {
        filter.update(tracker->getOutPos(centerLabel));
    } else {
        if(t.getCurrentLabels().size() > 0) {
            centerLabel = t.getCurrentLabels()[0];
        }
    }
    center = filter.value();
}

void MatrixScene::draw(){
    
    ofBackground(0,0,0);
    
    // http://stackoverflow.com/questions/5915753/generate-a-plane-with-triangle-strips
    
    // deform a plane
    
    //ofDrawPlane(OUTWIDTH/2, OUTHEIGHT/2, 0, OUTWIDTH, OUTHEIGHT);
    
    ofNoFill();
    //plane.draw();
    
    ofVec2f outCenter(OUTWIDTH/2, OUTHEIGHT/2);
    
    ofPushMatrix();
    ofTranslate(outCenter);
    
    glBegin(GL_TRIANGLES);
    ofSetColor(linecolor);
    for(int i=0;i<plane.getMesh().getVertices().size(); i+=2) {
        ofSetColor(linecolor);
        
        ofVec2f v = plane.getMesh().getVertices()[i];
        
        ofVec2f c(center.x-OUTWIDTH/2,center.y-OUTHEIGHT/2);
        
        glVertex2f(c.x,c.y);
        glVertex2f(v.x, v.y);
        //glVertex2f(vR.x, vR.y);
        
        //v.perpendicular();
        //glVertex2f(v.x, v.y);
        
    }
    glEnd();
    
    /*glBegin(GL_LINES);
    ofSetColor(ofFloatColor(0,0,0,0.2));
    for(int i=0;i<plane.getMesh().getVertices().size(); i++) {
        ofVec3f v = plane.getMesh().getVertices()[i];
        
        ofVec2f c(center.x-OUTWIDTH/2,center.y-OUTHEIGHT/2);
        if(c.distance(v) < centerSize*OUTWIDTH/2) {
            glVertex2f(v.x, v.y);
            glVertex2f(c.x,c.y);
        }
    }
    glEnd();*/
    
    
    /*glBegin(GL_POINTS);
    ofSetColor(linecolor.getLerped(ofFloatColor(0,0,0,1), 0.8));
    for(int i=0;i<plane.getMesh().getVertices().size(); i++) {
        ofVec3f v = plane.getMesh().getVertices()[i];
        
        ofVec2f c(center.x-OUTWIDTH/2,center.y-OUTHEIGHT/2);
        if(c.distance(v) < centerSize*OUTWIDTH/2) {
            glVertex2f(v.x, v.y);
            glVertex2f(c.x,c.y);
        }
    }
    glEnd();*/
    
    
    glBegin(GL_POINTS);
    ofSetColor(linecolor.getLerped(ofFloatColor(0,0,0,1), 0.8));
    for(int i=0;i<plane.getMesh().getVertices().size(); i++) {
        ofVec3f v = plane.getMesh().getVertices()[i];
        
        ofVec2f c(center.x-OUTWIDTH/2,center.y-OUTHEIGHT/2);
        if(c.distance(v) < centerSize*OUTWIDTH/2) {
            glVertex2f(v.x, v.y);
            glVertex2f(c.x,c.y);
        }
    }
    glEnd();
    
    /*glBegin(GL_POINTS);
    ofSetColor(0, 0, 0);
    for(int i=0;i<plane.getMesh().getVertices().size(); i++) {
        
        ofVec3f v = plane.getMesh().getVertices()[i];
        glVertex2f(v.x,v.y);
        
    }
    glEnd();
    
    ofPopMatrix();*/
    
    // for vertices displace plane.getMesh().getVertices().size();
    // if close to center - draw a new connecting lines at 90 degree angles
    
    // animate light on the connecting lines
    
    //ofFill();
    
    ofSetColor(0, 0, 0);
    //ofCircle(center, 20);
    
    
    
}

void MatrixScene::parseOscMessage(ofxOscMessage *m){
}

void MatrixScene::setGui(){
    
    gui->addSlider("/linesalpha/x", 0, 1, &linecolor.a);
    gui->addSlider("/linesr/x", 0, 1, &linecolor.r);
    gui->addSlider("/linesg/x", 0, 1, &linecolor.g);
    gui->addSlider("/linesb/x", 0, 1, &linecolor.b);
    
    gui->addSlider("/centersize/x", 0, 1, &centerSize);
    //gui->addSlider("/gravity/y", -1, 1, &gravity.y);
    
}
