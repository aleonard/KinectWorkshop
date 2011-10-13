#pragma once

#include "ofMain.h"
#include "ofxOpenNI.h"
#include "ofxOsc.h"

class testApp : public ofBaseApp{
	
public:
	void setup();
	void update();
	void draw();
	
	ofxOpenNIContext context;
	ofxDepthGenerator depth;
	ofxImageGenerator image;	
	ofxUserGenerator user;
	
	float distance;
	
	ofxOscSender osc;
};
