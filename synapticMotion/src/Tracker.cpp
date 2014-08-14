//
//  Tracker.cpp
//  synapticMotion
//
//  Created by Johan Bichel Lindegaard on 05/08/14.
//
//

#include "Tracker.h"

using namespace ofxCv;
using namespace cv;

void TTracker::setup() {
    
#ifdef _USE_LIVE_VIDEO
    vidGrabber.setVerbose(true);
    vidGrabber.initGrabber(320,240);
#else
    movie.loadMovie("video.mov");
	movie.play();
#endif
    
    inputWidth = 320;
    inputHeight = 240;
    
    colorImg.allocate(inputWidth,inputHeight);
	grayImg.allocate(inputWidth,inputHeight);
    
	showLabels = false;
}

void TTracker::update(){
    
    contourFinder.setMinAreaRadius(minArea);
	contourFinder.setMaxAreaRadius(maxArea);
	contourFinder.setThreshold(threshold);
	// wait for half a frame before forgetting something
	contourFinder.getTracker().setPersistence(persistence);
	// an object can move up to 32 pixels per frame
	contourFinder.getTracker().setMaximumDistance(maxDistance);
    
    
    bool bNewFrame = false;
    
#ifdef _USE_LIVE_VIDEO
    vidGrabber.update();
    bNewFrame = vidGrabber.isFrameNew();
    if(bNewFrame) blur(vidGrabber, 10);
#else
    movie.update();
    bNewFrame = movie.isFrameNew();
    if(bNewFrame) blur(movie, 10);
#endif
    
	if (bNewFrame){
        
#ifdef _USE_LIVE_VIDEO
        colorImg.setFromPixels(vidGrabber.getPixels(), inputWidth,inputHeight);
#else
        colorImg.setFromPixels(movie.getPixels(), inputWidth,inputHeight);
#endif
        
       
        
        grayImg = colorImg;
    
        contourFinder.findContours(grayImg);
        
        //blobTracker.update(grayImage, threshold, minArea, maxArea, nConsidered, hullPress, bFindHoles, bUseApproximation);
        
        //dancerBlobs.push_back()
        
        for(int i = 0; i < contourFinder.size(); i++) {
            
			//ofPoint center = toOf(contourFinder.getCenter(i));

			//ofVec2f velocity = toOf(contourFinder.getVelocity(i));
		}
        
        
	}
}





ofVec2f TTracker::convert(ofVec2f _in) {
    return _in / ofVec2f(inputWidth, inputHeight) * ofVec2f(OUTWIDTH, OUTHEIGHT);
}


ofVec2f TTracker::getOutPreviousPos(unsigned int _label) {
    
    RectTracker& tracker = contourFinder.getTracker();
    
    if(tracker.existsPrevious(_label)) {
        const cv::Rect& previous = tracker.getPrevious(_label);
        ofVec2f pos(previous.x + previous.width / 2, previous.y + previous.height / 2);
        
        return convert(pos);
        
    }
    
    return ofVec2f(0,0);
    
    
}


ofVec2f TTracker::getOutPos(unsigned int _label) {
    
    RectTracker& tracker = contourFinder.getTracker();
    
    const cv::Rect& current = tracker.getCurrent(_label);
    ofVec2f pos(current.x + current.width / 2, current.y + current.height / 2);
    
    return convert(pos);
}


//--------------------------------------------------------------
void TTracker::draw(){
    
    ofDisableDepthTest();
    ofSetLineWidth(2);
    ofSetBackgroundAuto(showLabels);
    
    RectTracker& tracker = contourFinder.getTracker();
    
    ofPushMatrix();
    ofTranslate(20, ofGetHeight()-inputHeight);
        
        ofSetColor(255);
		movie.draw(0, 0);
		contourFinder.draw();
		for(int i = 0; i < contourFinder.size(); i++) {
			ofPoint center = toOf(contourFinder.getCenter(i));
			ofPushMatrix();
			ofTranslate(center.x, center.y);
			int label = contourFinder.getLabel(i);
			string msg = ofToString(label) + ":" + ofToString(tracker.getAge(label));
			ofDrawBitmapString(msg, 0, 0);
			ofVec2f velocity = toOf(contourFinder.getVelocity(i));
			ofScale(5, 5);
			ofLine(0, 0, velocity.x, velocity.y);
			ofPopMatrix();
		}
        
		for(int i = 0; i < contourFinder.size(); i++) {
			unsigned int label = contourFinder.getLabel(i);
			// only draw a line if this is not a new label
            
            const cv::Rect& current = tracker.getCurrent(label);
            ofVec2f currentPosition(current.x + current.width / 2, current.y + current.height / 2);
            
            ofCircle(currentPosition, 5);
            
			if(tracker.existsPrevious(label)) {
                
				// use the label to pick a random color
				ofSeedRandom(label << 24);
				ofSetColor(ofColor::fromHsb(ofRandom(255), 255, 255));
				// get the tracked object (cv::Rect) at current and previous position
				const cv::Rect& previous = tracker.getPrevious(label);
                
				// get the centers of the rectangles
				ofVec2f previousPosition(previous.x + previous.width / 2, previous.y + previous.height / 2);

				ofLine(previousPosition, currentPosition);
                
                // todo add dancerblob with correct width and height calibrated to the output
 
                
			}
		}
	
	// this chunk of code visualizes the creation and destruction of labels
	const vector<unsigned int>& currentLabels = tracker.getCurrentLabels();
	const vector<unsigned int>& previousLabels = tracker.getPreviousLabels();
	const vector<unsigned int>& newLabels = tracker.getNewLabels();
	const vector<unsigned int>& deadLabels = tracker.getDeadLabels();
    
	ofSetColor(cyanPrint);
	for(int i = 0; i < currentLabels.size(); i++) {
		int j = currentLabels[i];
		ofLine(j, 0, j, 4);
	}
	ofSetColor(magentaPrint);
	for(int i = 0; i < previousLabels.size(); i++) {
		int j = previousLabels[i];
		ofLine(j, 4, j, 8);
	}
	ofSetColor(yellowPrint);
	for(int i = 0; i < newLabels.size(); i++) {
		int j = newLabels[i];
		ofLine(j, 8, j, 12);
	}
	ofSetColor(ofColor::white);
	for(int i = 0; i < deadLabels.size(); i++) {
		int j = deadLabels[i];
		ofLine(j, 12, j, 16);
	}

    
    ofPopMatrix();
    
	// draw the incoming, the grayscale, the bg and the thresholded difference
	/*ofSetHexColor(0xffffff);
	colorImg.draw(20,ofGetHeight()-20-colorImg.height);
	grayImg.draw(360,ofGetHeight()-20-grayImage.height);
    */
    
    
    
    /*
	// then draw the contours:
    
	blobTracker.draw(360,ofGetHeight()-20-grayImage.height,grayImage.width,grayImage.height);
	// or, instead we can draw each blob individually,
	// this is how to get access to them:
    
    for (int i = 0; i < blobTracker.size(); i++){
        ofFill();
        ofSetColor(255,0,0);
        ofCircle(blobTracker[i].centroid.x * ofGetWidth(),
                 blobTracker[i].centroid.y * ofGetHeight(),
                 10);
        ofSetColor(255);
        ofDrawBitmapString(ofToString( blobTracker[i].id ),
                           blobTracker[i].centroid.x * ofGetWidth(),
                           blobTracker[i].centroid.y * ofGetHeight());
    }*/
    
}

