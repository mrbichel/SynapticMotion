//
//  Tracker.h
//  synapticMotion
//
//  Created by Johan Bichel Lindegaard on 05/08/14.
//
//


#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxOpenCv.h"
#include "Defines.h"

//#define _USE_LIVE_VIDEO		// uncomment this to use a live camera
// otherwise, we'll use a movie file

class TTracker {
    
public:
    
    void setup();
    void draw();
    void debugDraw();
    void update();

    ofCamera cam;
    
#ifdef _USE_LIVE_VIDEO
    ofVideoGrabber 		vidGrabber;
#else
    ofVideoPlayer movie;
#endif
    
    ofVec2f convert(ofVec2f _in);
    ofVec2f getOutPos(unsigned int label);
    ofVec2f getOutPreviousPos(unsigned int label);
    
    int inputWidth;
    int inputHeight;
    
    int threshold;
    float minArea;
    float maxArea;
    int persistence;
    int nConsidered;
    int maxDistance;
    
	ofxCv::ContourFinder contourFinder;
	bool showLabels = false;
    
    ofxCvColorImage colorImg;
    ofxCvGrayscaleImage grayImg;
    
    // todo: quadwarp to calibrate positions to output
    
    // todo - coordinate calibration
    
    // a quadwarp to fix position to a rectangle that matches the floor
    
};
