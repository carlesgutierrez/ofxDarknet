#include "ofApp.h"

void ofApp::setup() 
{
	std::string cfgfile = ofToDataPath( "cfg/yolo9000.cfg" );
	std::string weightfile = ofToDataPath( "yolo9000.weights" );
	std::string namesfile = ofToDataPath( "cfg/9k.names" );
    
	darknet.init( cfgfile, weightfile, namesfile );

	video.setDeviceID( 0 );
	video.setDesiredFrameRate( 30 );
	video.initGrabber( 640, 480 );
}

void ofApp::update()
{
	video.update();
}

void ofApp::draw()
{
	float thresh = ofMap( ofGetMouseX(), 0, ofGetWidth(), 0, 1 );
	
	ofSetColor( 255 );
	video.draw( 0, 0 );

	if( video.isFrameNew() ) {
		std::vector< detected_object > detections = darknet.yolo( video.getPixels(), thresh );

		ofNoFill();	
		for( detected_object d : detections )
		{
			ofSetColor( d.color );
			glLineWidth( ofMap( d.probability, 0, 1, 0, 8 ) );
			ofNoFill();
			ofDrawRectangle( d.rect );
			ofDrawBitmapStringHighlight( d.label + ": " + ofToString(d.probability), d.rect.x, d.rect.y + 20 );
		}
	}
}