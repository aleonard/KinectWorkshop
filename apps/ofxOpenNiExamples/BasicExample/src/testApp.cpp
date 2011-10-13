#include "testApp.h"

void testApp::setup() {
	context.setup();
	//context.setupUsingXMLFile();
	depth.setup(&context);
	image.setup(&context);
	user.setup(&context);
	
	context.toggleRegisterViewport();
	context.toggleMirror();
}

void testApp::update(){
	// update all nodes
	context.update();
	depth.update();
	image.update();
	user.update();
}

void testApp::draw(){
	ofBackground(0);
	
	ofSetColor(255);
	ofScale(.75, .75, .75);
	depth.draw(0, 0, 640, 480);
	image.draw(640, 0, 640, 480);
	user.draw();
}