//
//  Logos.cpp
//  DefaultExample
//
//  Created by Alison Leonard on 10/13/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include "KinectDemos.h"

Logos::Logos()
{
    numLogos = 0;
    skipCount = 0;
}
void Logos::draw()
{
    if(numLogos == MAX_LOGOS)
        clear();
    ofSetColor(10,20,255);
    for(int i = 0; i < numLogos; i++)
    {
        ofCircle(Places[i].x, Places[i].y, (2048 - Places[i].z) / 40 );
    }
}
void Logos::update(ofPoint place)
{
    skipCount++;
    if(skipCount == SKIP_HANDS)
    {
        Places[numLogos] = place;
        //Sizes[numLogos] = size;
        numLogos++;
        skipCount = 0;
    }
    
    
}
void Logos::clear()
{
    numLogos = 0;
}
