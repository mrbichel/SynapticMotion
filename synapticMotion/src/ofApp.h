#pragma once

#include "ofMain.h"
#include "ofxSyphon.h"
#include "ofxContentScene.h"

#include "BrainWrinkleScene.h"
#include "ConnectingNeuronsScene.h"
#include "MatrixScene.h"
#include "Tracker.h"
#include "TestScene.h"


class ofApp : public ofBaseApp {

	public:
		void setup();
		void update();
		void draw();
        void exit();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
    ofxOscReceiver oscReceiver;
    ofxOscSender * oscSender;
    
    ofxSyphonServer syphonOut;
    ofxSyphonClient * syphonIn;
    
    ofxSyphonServerDirectory directory;
    void serverAnnounced(ofxSyphonServerDirectoryEventArgs &arg);
    void serverUpdated(ofxSyphonServerDirectoryEventArgs &args);
    void serverRetired(ofxSyphonServerDirectoryEventArgs &arg);
    int dirIdx;
    
    ofxXmlSettings XML;
    TTracker * tracker;
    
    // gui
    void setGUI();
    ofxUITabBar *guiTabBar;
    ofxUICanvas *mainGui;
    vector<ofxUICanvas *> guis;
    void guiEvent(ofxUIEventArgs &e);
    
    bool hideGUI;
    bool drawGuide;
    bool drawSyponIn;
    bool drawOutput;
    bool outputCombined;
    bool drawMask;
    
    vector<ofxContentScene*> scenes;
    
    ofColor bg;
    ofFbo fboOut;
		
};
