//
//  ContentScene.cpp
//  Gloria
//
//  Created by Johan Bichel Lindegaard on 27/06/14.
//
//

#include "ofxContentScene.h"

void ofxContentScene::init() {
}
void ofxContentScene::setup(){
}
void ofxContentScene::update(){
}
void ofxContentScene::draw() {
}
void ofxContentScene::setGui() {
}

void ofxContentScene::parseOscMessage(ofxOscMessage *m) {
}

void ofxContentScene::setupScene(int _width, int _height, int _i) {
    index = _i;
    name = "Scene" + ofToString(_i);
    
    ofFbo::Settings settings;
    
    height = _height;
    width = _width;
    
    settings.height = _height;
    settings.width = _width;
    settings.numSamples = numSamples;
    settings.useDepth = false;
    //settings.colorFormats = GL_RGBA;
    
    fbo.allocate(settings);
    
    fbo.begin();
    ofClear(0,0,0,0);
    fbo.end();
    
    setup();
    syphonOut.setName(name);
}


void ofxContentScene::setSceneGui(){
    
    gui = new ofxUICanvas();
    
    string i = "["+ ofToString(index) + "] ";
    gui->setName(name);
    
    gui->addWidgetDown(new ofxUILabel(name, OFX_UI_FONT_SMALL));
    gui->addWidgetDown(new ofxUILabel("OSC Address: " + oscAddress, OFX_UI_FONT_SMALL));
    
    gui->addSpacer(GUIWIDTH, 1);
    gui->addSlider("/opacity/x", 0., 1., &opacity);
    
    gui->addSpacer(GUIWIDTH, 1);
    //gui->addSlider(i+"speed", minSpeed, maxSpeed, &speed);
    gui->addToggle("/enabled/x", &enabled);
    gui->addToggle("/syphonout/x", &solo);
    
    setGui();
    gui->autoSizeToFitWidgets();

    ofAddListener(gui->newGUIEvent,this,&ofxContentScene::guiEvent);
}

void ofxContentScene::guiEvent(ofxUIEventArgs &e)
{
    string name = e.widget->getName();
    int kind = e.widget->getKind();
    string canvasParent = e.widget->getCanvasParent()->getName();
    
    //cout << canvasParent << endl;
}

void ofxContentScene::parseSceneOscMessage(ofxOscMessage * m){

    // you can change values of widgets in gui with osc just send to its name
    
    // supports either /sceneadress/paramtername or /sceneadress_parametername
    // everything is case insensitive
    
    bool isScene = false;
    string loweraddr = ofToLower(m->getAddress());
    
	vector<string> adrSplit = ofSplitString(loweraddr, "/");
	string rest = ofSplitString(loweraddr, "/"+adrSplit[1])[1];
	
    //cout<<adrSplit[1]<<"   "<<rest<<endl;
    
    if(adrSplit[1] == " scene"+ofToString(index) || "/"+adrSplit[1] == oscAddress) {
        
        isScene = true;
        
    } else {
        
        // check the underscore formatting
        
        adrSplit = ofSplitString(loweraddr, "_");
        if(adrSplit.size() > 1) {
            
        rest = adrSplit[1];
        
        if (adrSplit[0] == "scene"+ofToString(index) || adrSplit[0] == oscAddress) {
            
            // have it match the original / formatting
            rest = "/" + rest;
            
            isScene = true;
            
        }
            
        }
    }
    
    
    if(isScene) {
        
        //cout<<"    scene:"<<rest<<endl;
        
        for(int i=0; i< gui->getWidgets().size(); i++ ) {
            
            ofxUIWidget * widget = gui->getWidgets()[i];
            
            if(ofToLower(widget->getName()) == rest) {
                
                //cout<<widget->getKind()<<endl;

                if(widget->getKind() == OFX_UI_WIDGET_SLIDER_H || widget->getKind() == OFX_UI_WIDGET_SLIDER_V) {
                    
                    ofxUISlider *slider = (ofxUISlider *) widget;
                    
                    slider->setValue(ofMap(m->getArgAsFloat(0), 0, 1, slider->getMin(), slider->getMax()));
                    
                } else if(widget->getKind() == OFX_UI_WIDGET_INTSLIDER_H || widget->getKind() == OFX_UI_WIDGET_INTSLIDER_V) {
                    
                    ofxUISlider *slider = (ofxUISlider *) widget;
                    slider->setValue(ofMap(m->getArgAsInt32(0), 0, 1, slider->getMin(), slider->getMax()));
                    
                } else if(widget->getKind() == OFX_UI_WIDGET_TOGGLE) {
                    
                    ofxUIToggle *toggle = (ofxUIToggle *) widget;
                    toggle->setValue(m->getArgAsFloat(0));
                    
                } else if(widget->getKind() == OFX_UI_WIDGET_BUTTON) {
                    
                    ofxUIToggle *toggle = (ofxUIToggle *) widget;
                    toggle->setValue(m->getArgAsFloat(0));
                    
                }
                
                
                for(int o=0; o<oscClients.size(); o++) {
                    
                    oscClients[o]->sendMessage(*(m));
                    
                }
                
            }
            
        }
    }
}


void ofxContentScene::updateScene() {
    
    if(enabled) {
        update();
    }
}

void ofxContentScene::drawScene() {
    if(enabled) {
        ofPushStyle();
        ofPushMatrix();
        fbo.begin();
        draw();
        fbo.end();
        ofPopMatrix();
        ofPopStyle();
    }
}

void ofxContentScene::publishSyphonTexture(bool _force) {
    if ( (solo && enabled) || _force ) {
        syphonOut.publishTexture(&fbo.getTextureReference());
    }
}


void ofxContentScene::exit() {
    delete gui;
}