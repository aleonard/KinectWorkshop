#include "testApp.h"

const float FovX = 1.0144686707507438;
const float FovY = 0.78980943449644714;
const float XtoZ = tanf(FovX / 2) * 2;
const float YtoZ = tanf(FovY / 2) * 2;
const unsigned int Xres = 640;
const unsigned int Yres = 480;

//--------------------------------------------------------------
ofVec3f ConvertProjectiveToRealWorld(float x, float y, float z) {
	return ofVec3f((x / Xres - .5f) * z * XtoZ,
                   (y / Yres - .5f) * z * YtoZ,
                   z);
}

//--------------------------------------------------------------
ofVec3f getNormal(ofVec3f& a, ofVec3f& b, ofVec3f& c) {
	ofVec3f side1 = a - b;
	ofVec3f side2 = c - b;
	ofVec3f normal = side1.cross(side2);
	normal.normalize();
	return normal;
}


//--------------------------------------------------------------
void testApp::setup() {

    // initialize variables
    bswitchCamera = false;
    bswitchInfrared = false;
    PicColor = WHITE;
    displayType = DISP_BASIC;
    demo.setColorScheme(scheme_bug);
    
	kinect.init();
	kinect.setVerbose(true);
	kinect.open();
	
	// start with the live kinect source
	kinectSource = &kinect;

	// zero the tilt on startup
	angle = 0;
	kinect.setCameraTiltAngle(angle);
	
	// start from the front
	pointCloudRotationY = 180;
    
    // initialize Mesh light
    ofSetVerticalSync(true);
    light.setPosition(0, 0, -200);
	light.setDiffuseColor(ofColor(255));
	light.setAmbientColor(ofColor(0));
   
    // openNI demos
 //   bswitchOpenNI = true;
 //   bopenNIActive = true;
  //  displayType = DISP_SKELETON;
	
    
}

//--------------------------------------------------------------
void testApp::update() {

	ofBackground(0, 0, 0);
    
    if(bswitchOpenNI)
    {
        bopenNIActive = true;
        kinect.close();
        context.setup();
        depth.setup(&context);
        user.setup(&context);
        HandTracker.setup(&context, 4);
        HandTracker.setSmoothing(0.1f);
        HandTracker.setFilterFactors(0.1f);
        
        context.toggleRegisterViewport();
        context.toggleMirror();
        bswitchOpenNI = false;
    }
    if(bswitchCamera)
    {  
        if(bopenNIActive)
        {
            bopenNIActive = false;
            context.shutdown();
        }
        kinect.close();
        kinect.init(bswitchInfrared);
        kinect.open();
        bswitchCamera = false;
    }
	
    if(bopenNIActive)
    {
        context.update();
        depth.update();
        user.update();
        
        if(displayType == DISP_LOGOS)
        {
            int numHands = HandTracker.getMaxNumHands();
            for( int i = 0; i < numHands; i++)
            {
                ofxTrackedHand* hand = HandTracker.getHand(0);
                logos.update(hand->projectPos);
            }
        }
        
    }
    else
        kinectSource->update();
    
	
	// there is a new frame and we are connected
	if(kinectSource->isFrameNew()) {

        if(displayType == DISP_BUG)
            demo.bugMask(kinect.getRawDepthPixels());
        else if (displayType == DISP_MESH)
            computeMesh();
	}
}

//--------------------------------------------------------------
void testApp::computeMesh()
{
    int width = kinect.getWidth();
    int height = kinect.getHeight();
    float* distancePixels = kinect.getDistancePixels(); // distance in centimeters
    mesh.clear();
    mesh.setMode(OF_TRIANGLES_MODE);
    for(int y = 0; y < height - 1; y++) { // don't go to the end
        for(int x = 0; x < width - 1; x++) { // don't go to the end
			
            // get indices for each corner
            int nwi = (y + 0) * width + (x + 0);
            int nei = (y + 0) * width + (x + 1);
            int sei = (y + 1) * width + (x + 1);
            int swi = (y + 1) * width + (x + 0);
            
            // get z values for each corner
            float nwz = distancePixels[nwi];
            float nez = distancePixels[nei];
            float sez = distancePixels[sei];
            float swz = distancePixels[swi];
            
            if(nwz > 0 && nez > 0 && sez > 0 && swz > 0) { // ignore empty depth pixels
                // get real world locations for each corner
                ofVec3f nwv = ConvertProjectiveToRealWorld(x + 0, y + 0, nwz);
                ofVec3f nev = ConvertProjectiveToRealWorld(x + 1, y + 0, nez);
                ofVec3f sev = ConvertProjectiveToRealWorld(x + 1, y + 1, sez);
                ofVec3f swv = ConvertProjectiveToRealWorld(x + 0, y + 1, swz);
                
                // compute normal for the upper left
                ofVec3f normal = getNormal(nwv, nev, swv);
				
                // add the upper left triangle
                mesh.addNormal(normal);
                mesh.addVertex(nwv);
                mesh.addNormal(normal);
                mesh.addVertex(nev);
                mesh.addNormal(normal);
                mesh.addVertex(swv);
                
                // add the bottom right triangle
                mesh.addNormal(normal);
                mesh.addVertex(nev);
                mesh.addNormal(normal);
                mesh.addVertex(sev);
                mesh.addNormal(normal);
                mesh.addVertex(swv);
            }
        }
    }

}

//--------------------------------------------------------------
void testApp::draw() {

    if(PicColor == ORANGE)
        ofSetColor(255,100,30);
    else if (PicColor == WHITE)
        ofSetColor(255, 255, 255);

    int winWidth = ofGetWidth();
    int winHeight= ofGetHeight() - 40;
    int buf = 10, div = 8;
	
	switch(displayType){
        case DISP_POINT_CLOUD:
            ofPushMatrix();
            ofTranslate(420, 320);
            // we need a proper camera class
            drawPointCloud();
            ofPopMatrix();
            break;
        case DISP_BASIC:
            kinect.drawDepth(buf, buf, winWidth/2 - 2*buf, winHeight- 2*buf);
            kinect.draw(2*buf+ winWidth/2, buf, winWidth/2 -3*buf, winHeight - 2*buf);
            break;
        case DISP_BUG:
            demo.draw(buf, buf, winWidth - 2*buf, winHeight- 2*buf);
            break;
        case DISP_MESH:
            ofPushMatrix();
          //  ofTranslate(420, 320);
            drawMesh();
            ofPopMatrix();
            break;
        case DISP_SKELETON:
            ofPushMatrix();
            ofSetColor(255);
            ofScale(1.7, 1.7, 1.7);
            //ofScale(1.75, 1.75, 1.75);
            depth.draw(0, 0, 640, 480);
            user.draw();
            ofPopMatrix();
            break;
        case DISP_HAND_TRACK:
            ofPushMatrix();
            ofSetColor(255);
            ofScale(1.7, 1.7, 1.7);
            //ofScale(1.75, 1.75, 1.75);
            depth.draw(0, 0, 640, 480);
            HandTracker.drawHands();
            ofPopMatrix();
            break;
        case DISP_LOGOS:
            ofPushMatrix();
            ofScale(1.75, 1.75, 1.75);
            logos.draw();
            ofPopMatrix();
            break;
    }
	
    // draw instructions
	ofSetColor(255, 100, 30);
	stringstream reportStream;
	reportStream << "1:Basic 2:Infrared 3:Orange 4:PointCloud 5:Mesh 6:Bug 7:Skeleton 8:HandTracking 9:HandDots" << endl
                << "built on source from http://futuretheater.net/wiki/Kinect_Workshop";
	ofDrawBitmapString(reportStream.str(),20,winHeight + 10);
}

//--------------------------------------------------------------
void testApp::drawPointCloud() {
	ofScale(400, 400, 400);
	int w = 640;
	int h = 480;
	ofRotateY(pointCloudRotationY);
	float* distancePixels = kinect.getDistancePixels();
	glBegin(GL_POINTS);
	int step = 2;
	for(int y = 0; y < h; y += step) {
		for(int x = 0; x < w; x += step) {
			ofPoint cur = kinect.getWorldCoordinateFor(x, y);
			ofColor color = kinect.getCalibratedColorAt(x,y);
			glColor3ub((unsigned char)color.r,(unsigned char)color.g,(unsigned char)color.b);
			glVertex3f(cur.x, cur.y, cur.z);
		}
	}
	glEnd();
}

//--------------------------------------------------------------
void testApp::drawMesh() {
    ofBackground(0);
	
	easyCam.begin();
	ofScale(1, -1, -1); // orient the point cloud properly
	ofTranslate(0, 0, -650); // rotate about z = 150 cm
	glEnable(GL_DEPTH_TEST);
	ofSetColor(255, 100, 30);
    ofEnableLighting();
    mesh.drawFaces();
    easyCam.end();
	
	ofDisableLighting();
}


//--------------------------------------------------------------
void testApp::exit() {
	kinect.setCameraTiltAngle(0); // zero the tilt on exit
	kinect.close();
}

//--------------------------------------------------------------
void testApp::keyPressed (int key) {
	switch (key) {
        case OF_KEY_UP:
			angle++;
			if(angle>30) angle=30;
			kinect.setCameraTiltAngle(angle);
			break;

		case OF_KEY_DOWN:
			angle--;
			if(angle<-30) angle=-30;
			kinect.setCameraTiltAngle(angle);
			break;
        case '1':
            PicColor = WHITE;
            displayType = DISP_BASIC;
            bswitchCamera = true;
            bswitchInfrared = false;
            break;
        case '2':
            PicColor = WHITE;
            displayType = DISP_BASIC;
            bswitchCamera = true;
            bswitchInfrared = true;
            break;
        case '3':
            PicColor = ORANGE;
            break;
        case '4':
            bswitchCamera = true;
            bswitchInfrared = false;
            displayType = DISP_POINT_CLOUD;
            light.disable();
            break;
        case '5':
            displayType = DISP_MESH;
            light.enable();
            break;
        case'6':
            PicColor = WHITE;
            displayType = DISP_BUG;
            break;
        case '7':
            displayType = DISP_SKELETON;
            if(!bopenNIActive)
                bswitchOpenNI = true;
            break;
        case '8':
            displayType = DISP_HAND_TRACK;
            if(!bopenNIActive)
                bswitchOpenNI = true;
            break;
        case '9':
            displayType = DISP_LOGOS;
            if(!bopenNIActive)
                bswitchOpenNI = true;
            break;
        case ' ':
            logos.clear();
            break;
	}
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y) {
	pointCloudRotationY = x;
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button)
{}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button)
{}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button)
{}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h)
{}
