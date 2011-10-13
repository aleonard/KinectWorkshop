#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxKinect.h"
#include "KinectDemos.h"
#include "ofxOpenNI.h"

class testApp : public ofBaseApp {
	public:

		void setup();
		void update();
		void draw();
		void exit();
	
		void drawPointCloud();
        void drawMesh();
        void computeMesh();

		void keyPressed  (int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		

		ofxKinect 			kinect;
		
		/// used to switch between the live kinect and the recording player
		ofxBase3DVideo* 	kinectSource;

		ofxCvColorImage		colorImg;
    
        // dipslay type and colors
        int                 PicColor;
        int                 displayType;
        KinectDemos         demo;
    
        // mesh demo
        ofEasyCam easyCam;
        ofMesh mesh; // for even more speed, use ofVboMesh
        ofLight light;
    
        // tracker demos
        ofxOpenNIContext context;
        ofxDepthGenerator depth;
        ofxUserGenerator user;
        ofxHandGenerator HandTracker;
        Logos           logos;

		int					angle;
		
		int 				pointCloudRotationY;
		
		bool 				bRecord;
		bool 				bPlayback;
    
        bool                bswitchCamera;
        bool                bswitchInfrared;
    
        bool                bswitchOpenNI;
        bool                bopenNIActive;
    
        const static int    WHITE = 1;
        const static int    ORANGE = 2;
        const static int    DISP_BASIC = 1;
        const static int    DISP_POINT_CLOUD = 2;
        const static int    DISP_BUG = 3;
        const static int    DISP_MESH = 4;
        const static int    DISP_SKELETON = 5;
        const static int    DISP_HAND_TRACK = 6;
        const static int    DISP_LOGOS = 7;
};
