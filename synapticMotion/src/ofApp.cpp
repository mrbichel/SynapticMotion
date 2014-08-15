#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    oscReceiver.setup(OSCRECEIVEPORT);
    
    oscSender = new ofxOscSender();
    oscSender->setup(OSCCLIENT, OSCSENDPORT);
    
    ofSetFrameRate(TARGET_FRAMERATE);
    ofSetVerticalSync(true);
    
    glEnable(GL_LINES);
    ofEnableSmoothing();
    ofEnableAlphaBlending();
    ofEnableDepthTest();
    
    ofSetWindowTitle("Synaptic Motion | Capacitor | 2014");
    
    syphonOut.setName("Gloria Main");
    fboOut.allocate(OUTWIDTH, OUTHEIGHT);
    
    ofShowCursor();
    syphonIn = new ofxSyphonClient();
    
    syphonIn->setApplicationName("Millumin");
    syphonIn->setServerName("");
    syphonIn->setup();
    
    directory.setup();
    
    //register for our directory's callbacks
    ofAddListener(directory.events.serverAnnounced, this, &ofApp::serverAnnounced);
    ofAddListener(directory.events.serverUpdated,   this, &ofApp::serverUpdated);
    ofAddListener(directory.events.serverRetired,   this, &ofApp::serverRetired);
    dirIdx = -1;
    
    tracker = new TTracker();
    tracker->setup();
    
    scenes.push_back(new BrainWrinkleScene());
    scenes.push_back(new MatrixScene());
    scenes.push_back(new ConnectingNeuronsScene());
    scenes.push_back(new TestScene());
    
    ofFbo::Settings fboSettings;
    fboSettings.height = OUTHEIGHT;
    fboSettings.width = OUTWIDTH;
    fboSettings.numSamples = 4;
    fboSettings.useDepth = false;
    
    fboOut.allocate(fboSettings);
    
    fboOut.begin();
    ofBackground(0,0,0,255);
    fboOut.end();
    
    
    for(int i=0; i<scenes.size(); i++) {
        //scenes[i]->mapping = mapping; tracker
        scenes[i]->tracker = tracker;
        scenes[i]->syphonIn = syphonIn;
        scenes[i]->oscClients.push_back(oscSender);
        scenes[i]->setupScene(OUTWIDTH, OUTHEIGHT, i);
    }
    
    setGUI();
    

}


void ofApp::serverAnnounced(ofxSyphonServerDirectoryEventArgs &arg)
{
    for( auto& dir : arg.servers ){
        ofLogNotice("ofxSyphonServerDirectory Server Announced")<<" Server Name: "<<dir.serverName <<" | App Name: "<<dir.appName;
    }
    dirIdx = 0;
}

void ofApp::serverUpdated(ofxSyphonServerDirectoryEventArgs &arg)
{
    for( auto& dir : arg.servers ){
        ofLogNotice("ofxSyphonServerDirectory Server Updated")<<" Server Name: "<<dir.serverName <<" | App Name: "<<dir.appName;
    }
    dirIdx = 0;
}

void ofApp::serverRetired(ofxSyphonServerDirectoryEventArgs &arg)
{
    for( auto& dir : arg.servers ){
        ofLogNotice("ofxSyphonServerDirectory Server Retired")<<" Server Name: "<<dir.serverName <<" | App Name: "<<dir.appName;
    }
    dirIdx = 0;
}

//--------------------------------------------------------------
void ofApp::update(){
    tracker->update();
    
    while(oscReceiver.hasWaitingMessages()){
        
		// get the next message
		ofxOscMessage m;
		oscReceiver.getNextMessage(&m);
        
        //cout<<m.getAddress()<<endl;
        for(int i=0; i<scenes.size();i++) {
            scenes[i]->parseSceneOscMessage(&m);
        }
    }
    
    // Scenes
    for(int i=0; i<scenes.size(); i++) {
        scenes[i]->updateScene();
    }

    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    
    // Draw scene fbo's
    ofPushStyle();
    ofNoFill();
    for(int i=0; i<scenes.size(); i++) {
        ofSetColor(255);
        ofFill();
        
        ofPushStyle();
        ofPushMatrix();
        scenes[i]->drawScene();
        ofPopMatrix();
        ofPopStyle();
    }
    ofPopStyle();
    
    /* ofPushStyle();{
     fboOut.begin();{
     ofEnableAlphaBlending();
     ofClear(0, 0);
     
     for(int i=0; i<scenes.size(); i++) {
     ofSetColor(255,255,255,scenes[i]->opacity*255);
     
     if(scenes[i]->enabled) {
     scenes[i]->fbo.draw(0,0);
     }
     }
     
     //glBlendFunc(GL_FUNC_SUBTRACT​);
     
     if(drawMask) mapping->drawMask();
     
     }fboOut.end();
     } ofPopStyle();
     */
    ofDisableDepthTest();
    ofBackground(0, 0, 0);
    ofSetColor(255,255,255,255);
    
    float scale = 0.08;
    ofPushMatrix();{
        ofTranslate(ofGetWidth()-scale*fboOut.getWidth()-40, 40);
        
        //ofScale(0.08, 0.08);
        
        ofSetColor(255,255,255,255);
        ofNoFill();
        ofSetLineWidth(1);
        
        for(int i=0; i<scenes.size(); i++) {
            if(scenes[i]->enabled){
                ofSetColor(255);
            } else {
                ofSetColor(255,0,0,100);
            }
            
            ofRect(-1, -1, scale*fboOut.getWidth()+2, scale*fboOut.getHeight()+2);
            // fboOut.draw(0, 0);
            ofSetColor(255,255,255,scenes[i]->opacity*255);
            
            if(scenes[i]->enabled) {
                scenes[i]->fbo.draw(0,0, scenes[i]->fbo.getWidth()*scale, scenes[i]->fbo.getHeight()*scale);
            }
            
            ofSetColor(255);
            
            ofDrawBitmapString(scenes[i]->name + "    ("+ofToString(scenes[i]->opacity*100.,0)+"%)", ofPoint(0,-3));
            
            if(drawGuide) {
                //ofSetColor(255,255,255,96);
                ofPushMatrix();
                ofScale(scale, scale);
                //drawGrid();
                //debugDraw();
                ofPopMatrix();
            }
            
            ofTranslate(0, scale*fboOut.getHeight()+30);
        }
        
        
        ofTranslate(0, 30);
        //Syphon
        ofPushMatrix();
        
        ofSetColor(0,0,255);
        ofSetLineWidth(1);
        ofRect(-1, -1, scale*syphonIn->getWidth()+2, scale*syphonIn->getHeight()+2);
        
        ofSetColor(255);
        ofDrawBitmapString("Syphon input - (Press 'i' to change)",  ofPoint(0,-18));
        ofDrawBitmapString(syphonIn->getApplicationName()+" "+syphonIn->getServerName(),  ofPoint(0,-3));
        
        syphonIn->draw(0, 0, scale*syphonIn->getWidth(), scale*syphonIn->getHeight());
        
        

        
        ofPopMatrix();
        
    }ofPopMatrix();
    
    
    tracker->draw();
    
    ofSetColor(255);
    ofDrawBitmapString("FPS: " + ofToString(ofGetFrameRate()), ofGetWidth()-200, 20);
    
    
    for(int i=0; i<scenes.size(); i++) {
        if(scenes[i]->enabled) {
            scenes[i]->publishSyphonTexture();
        }
    }
    
    syphonOut.publishTexture(&fboOut.getTextureReference());
    
}

void ofApp::setGUI()
{
    
    float dim = 16;
    float xInit = OFX_UI_GLOBAL_WIDGET_SPACING;
    float width = 255-xInit;
    hideGUI = false;
    
    guiTabBar = new ofxUITabBar();
    mainGui = new ofxUICanvas();
    
    mainGui->setFont("GUI/Arial.ttf");
    mainGui->setWidgetFontSize(OFX_UI_FONT_SMALL);
    
    guiTabBar->setFont("GUI/Arial.ttf");
    guiTabBar->setWidgetFontSize(OFX_UI_FONT_SMALL);
    
    mainGui->addLabel("Synaptic Motion");
    
    mainGui->addLabel("OSC info");
    mainGui->addLabel("In: " + ofToString(OSCRECEIVEPORT));
    mainGui->addLabel("Out: " + string(OSCCLIENT) + ":" + ofToString(OSCSENDPORT));
    
    mainGui->addLabel("Tracker");
    mainGui->addIntSlider("Threshold", 0, 255, &tracker->threshold);
    
    mainGui->addIntSlider("Persistence", 0, 30, &tracker->persistence);

    mainGui->addIntSlider("maxDistance", 0, 30, &tracker->maxDistance);

    //mainGui->addButton("Learn background", &tracker->bUpdateBackground);
    
    mainGui->addRangeSlider("Blobsize range", 0, tracker->grayImg.width, &tracker->minArea, &tracker->maxArea);
    
    //mainGui->addIntSlider("nConsidered", 0, 20, &tracker->nConsidered);

    //mainGui->addDoubleSlider("hullPress", 0, 20, &tracker->hullPress);
    
    //mainGui->addToggle("bFindHoles",  &tracker->bFindHoles);
    
    mainGui->addToggle("showLabels",  &tracker->showLabels);
    
    mainGui->autoSizeToFitWidgets();
    ofAddListener(mainGui->newGUIEvent,this,&ofApp::guiEvent);
    
    
    for(int i=0; i<scenes.size(); i++) {
        scenes[i]->setSceneGui();
        guiTabBar->addCanvas(scenes[i]->gui);
        scenes[i]->gui->setColorBack(ofColor(0,100 + 20*i,0,255));
        guis.push_back(scenes[i]->gui);
    }
    
    guiTabBar->autoSizeToFitWidgets();
    ofAddListener(guiTabBar->newGUIEvent,this,&ofApp::guiEvent);
    
    guiTabBar->setPosition(0, mainGui->getRect()->height+10);
    //guiTabBar->setScrollAreaToScreenHeight();
    
    mainGui->setColorBack(ofColor(0,150,200,255));
    guiTabBar->setColorBack(ofColor(0,100,0,255));
    
    mainGui->loadSettings("GUI/guiMainSettings.xml");
    guiTabBar->loadSettings("GUI/guiSettings.xml", "ui-");
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    
}

void ofApp::exit() {
    mainGui->saveSettings("GUI/guiMainSettings.xml");
    guiTabBar->saveSettings("GUI/guiSettings.xml", "ui-");
    
    delete guiTabBar;
}

void ofApp::guiEvent(ofxUIEventArgs &e)
{
}
