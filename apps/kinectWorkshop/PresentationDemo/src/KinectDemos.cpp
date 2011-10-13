//
//  KinectDemos.cpp
//  ofxKinectExample
//
//  Created by Alison Leonard on 10/2/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include "KinectDemos.h"

KinectDemos::KinectDemos()
{
    displayTex.allocate(IMG_WIDTH, IMG_HEIGHT, GL_RGB);
    
    nearClipping = rawToCentimeters(0);
    farClipping = 400;
}

KinectDemos::~KinectDemos()
{
    displayTex.clear();
}

void KinectDemos::draw(float _x, float _y, float _w, float _h){
		displayTex.draw(_x, _y, _w, _h);
}
void KinectDemos::colorMask(unsigned short * depth)
{
    lookupPix(depth);
    displayTex.loadData(colorPix, IMG_WIDTH, IMG_HEIGHT, GL_RGB);
    
}
void KinectDemos::bugMask(unsigned short * depth)
{
    lookupPix(depth);
    int RowIdx, HalfWidth = (IMG_WIDTH / 2) *3;
    for (int i = 0; i < IMG_HEIGHT; i++)
    {
        RowIdx = i*IMG_WIDTH*3;
        for(int j = 0; j < HalfWidth; j+=3)
        {
            colorPix[RowIdx + HalfWidth + j] = colorPix[RowIdx + HalfWidth-j-3];
            colorPix[RowIdx + HalfWidth + j+1] = colorPix[RowIdx + HalfWidth-j-2];
            colorPix[RowIdx + HalfWidth + j+2] = colorPix[RowIdx + HalfWidth-j-1];
        }
    }
    displayTex.loadData(colorPix, IMG_WIDTH, IMG_HEIGHT, GL_RGB);
}

void KinectDemos::updateTex(unsigned char * newPix)
{
    displayTex.loadData(newPix, IMG_WIDTH, IMG_HEIGHT, GL_RGB);
}
void KinectDemos::lookupPix(unsigned short * depth){

  for(int i = 0; i < IMG_SIZE; i++)
  {
    colorPix[i*3] = depthPixelsLookupRed[depth[i]];
    colorPix[i*3 + 1] = depthPixelsLookupGreen[depth[i]];
    colorPix[i*3 + 2] = depthPixelsLookupBlue[depth[i]];
  }
           
}

// these are for converting centimeters to/from raw values
// using equation from http://openkinect.org/wiki/Imaging_Information
const float
k1 = 0.1236,
k2 = 2842.5,
k3 = 1.1863,
k4 = 0.0370;

float KinectDemos::rawToCentimeters(unsigned short raw) {
	return 100 * (k1 * tanf((raw / k2) + k3) - k4);
}

void KinectDemos::setColorScheme(int scheme)
{
    int sp = 256;
    
    // get grayscale mapped pixels
    for(int i = 0; i < 2048; i++){
        distanceDepth[i] = rawToCentimeters(i);
        depthPixelsLookupNearWhite[i] = 255 - ofMap(distanceDepth[i], nearClipping, farClipping, 0, 255, true);
    }
        
    switch(scheme)
    {
        case scheme_red:
            for(int i = 0; i < 2048; i++){
                // generate colormap
                depthPixelsLookupRed[i] = depthPixelsLookupNearWhite[i];
                depthPixelsLookupGreen[i] = 0;
                depthPixelsLookupBlue[i] = 0;
            }
            
            break;
        case scheme_bug:
            
            for(int i = 0; i < 3*sp; i++){
                // generate colormap
                depthPixelsLookupRed[i] = depthPixelsLookupNearWhite[i*2];
                depthPixelsLookupGreen[i] = depthPixelsLookupNearWhite[i*2];
                depthPixelsLookupBlue[i] = 0;
            }
            for(int i = 0; i < sp; i++){
                // generate colormap
                depthPixelsLookupRed[sp*3+i] = depthPixelsLookupNearWhite[i*8];
                depthPixelsLookupGreen[sp*3+i] = 100;
                depthPixelsLookupBlue[sp*3+i] = 255 - depthPixelsLookupNearWhite[i*8];
            }
            for(int i = 0; i < 3*sp; i++){
                // generate colormap
                depthPixelsLookupRed[sp*4+i] = 0;
                depthPixelsLookupGreen[sp*4+i] = 0;
                depthPixelsLookupBlue[sp*4+i] = depthPixelsLookupNearWhite[i];
            }
            for(int i = 0; i < sp; i++){
                // generate colormap
                depthPixelsLookupRed[sp*7+i] = 50;
                depthPixelsLookupGreen[sp*7+i] = 10;
                depthPixelsLookupBlue[sp*7+i] = 122;
            }
            break;
        default:
            // add error checking for schemes
            break;
            
    }
    
}