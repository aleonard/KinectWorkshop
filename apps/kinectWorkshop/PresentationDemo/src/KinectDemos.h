//
//  KinectDemos.h
//  ofxKinectExample
//
//  Created by Alison Leonard on 10/2/11.
//  
//

#pragma once

#include "ofTexture.h"
#include "ofxKinectCalibration.h"
#include "ofMain.h"

#define NUM_DEPTH_PIXELS    2048
#define IMG_WIDTH   640
#define IMG_HEIGHT  480
#define IMG_SIZE    307200

typedef enum { 
    scheme_red = 1,
    scheme_bug = 2
}color_schemes;


class KinectDemos{
    
    
public:
    KinectDemos();
    ~KinectDemos();
    
    
    void colorMask(unsigned short * depth);
    void bugMask(unsigned short * depth);
    void draw(float _x, float _y, float _w, float _h);
    void updateTex(unsigned char * newPix); 
    void lookupPix(unsigned short * depth);
    void setColorScheme(int scheme);
    float rawToCentimeters(unsigned short raw);
    
    ofVbo               vbo;
    ofTexture			displayTex;			// the depth texture
    
    float distanceDepth[NUM_DEPTH_PIXELS];
    unsigned char depthPixelsLookupNearWhite[NUM_DEPTH_PIXELS];
    unsigned char depthPixelsLookupRed[NUM_DEPTH_PIXELS];
    unsigned char depthPixelsLookupGreen[NUM_DEPTH_PIXELS];
    unsigned char depthPixelsLookupBlue[NUM_DEPTH_PIXELS];
    unsigned char depthPixelsLookupAlpha[NUM_DEPTH_PIXELS];
    
    unsigned char colorPix[IMG_SIZE*3];
    
private:
    int nearClipping, farClipping;
};

class Logos{
    
public:
    Logos();
    
    void draw();
    void update(ofPoint place);
    void clear();
    
    int numLogos;
    ofPoint Places[1500];
    int Sizes[1500];
    int skipCount;
    
    const static int MAX_LOGOS = 1500;
    const static int SKIP_HANDS = 5;
    
};