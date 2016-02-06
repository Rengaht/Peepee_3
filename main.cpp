#include "ofMain.h"
#include "ofApp.h"

//========================================================================


FieldTexture *FieldTexture::_instance=0;
OceanTexture *OceanTexture::_instance=0;
RoofTexture *RoofTexture::_instance=0;
LeafTexture *LeafTexture::_instance=0;
WindowTexture *WindowTexture::_instance=0;

float SceneBlock::scene_speed=5;
ofColor SceneBlock::scene_color[4]={ofColor(255),ofColor(97,112,119),ofColor(21,28,52),ofColor(77,31,42)};

int ofApp::OCEAN_LENGTH=720;


int main( ){
	ofSetupOpenGL(800,480,OF_WINDOW);			// <-------- setup the GL context

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new ofApp());

}
