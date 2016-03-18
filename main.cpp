

#include "ofMain.h"
#include "ofApp.h"

//========================================================================


FieldTexture *FieldTexture::_instance=0;
OceanTexture *OceanTexture::_instance=0;
RoofTexture *RoofTexture::_instance=0;
LeafTexture *LeafTexture::_instance=0;
WindowTexture *WindowTexture::_instance=0;

ofColor FactoryPart::pcolor[5]={ofColor(38,40,46),ofColor(173,81,56),ofColor(247,247,247),ofColor(221,218,224),ofColor(133,148,174)};
ofColor FactorySmoke::pcolor[5]={ofColor(85,96,158),ofColor(87,90,133),ofColor(136,144,148),ofColor(165,161,163),ofColor(117,114,115)};


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
