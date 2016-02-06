#ifndef WINDOW_H
#define WINDOW_H
#include "ofMain.h"
#include "Chair.h"
#include "Handle.h"

class Window:public TrainBase{
	

	int mstroke;
	float phi;
	bool use_filter;
	ofMesh _mesh;
	int mesh_shake;
public:
	//float x,y,wid,hei;

	Window(float x_,float y_,float wid_,float hei_){
		x=x_; y=y_; wid=wid_; hei=hei_;
		mstroke=(int)ofRandom(2,4);
		phi=ofRandom(TWO_PI);

		use_filter=ofRandom(5)<1;

		mesh_shake=(int)ofRandom(5,20);

		if(use_filter){

			float W=ofGetWidth();
			float H=ofGetHeight()*.33;

			_mesh.addVertex(ofPoint(0,0));
			_mesh.addVertex(ofPoint(0,hei));
			_mesh.addVertex(ofPoint(wid,hei));
			
			_mesh.addVertex(ofPoint(wid,hei));
			_mesh.addVertex(ofPoint(wid,0));
			_mesh.addVertex(ofPoint(0,0));
			
			float tposx=ofRandom(0,1-wid/W);
			float tposy=ofRandom(0,1-hei/H);

			_mesh.addTexCoord(ofPoint(tposx+0,tposy+0));
			_mesh.addTexCoord(ofPoint(tposx+0,tposy+hei/H));
			_mesh.addTexCoord(ofPoint(tposx+wid/W,tposy+hei/H));
			
			_mesh.addTexCoord(ofPoint(tposx+wid/W,tposy+hei/H));
			_mesh.addTexCoord(ofPoint(tposx+wid/W,tposy+0));
			_mesh.addTexCoord(ofPoint(tposx+0,tposy+0));
			
		}
	}
	void updateMesh(){
		vector<ofVec2f> tc2=_mesh.getTexCoords();
		float dx=ofRandom(-1,1)*.005;
		float dy=ofRandom(-1,1)*.005;
		for(int i=0;i<tc2.size();++i){
			_mesh.setTexCoord(i,ofVec2f(tc2[i].x+dx,tc2[i].y+dy));
		}
	}

	void draw(bool draw_fill,float dx_,ofColor scolor){


		if(ofGetFrameNum()%mesh_shake==0) updateMesh();

		ofPushStyle();
		if(draw_fill){
			if(phi>PI) ofSetColor(247,202,201);
			else ofSetColor(145,168,209);
			
			
		}else{
			ofNoFill();
			ofSetColor(scolor!=ofColor::white?ofColor::white:ofColor::black);
		}

		ofPushMatrix();

		

		ofTranslate(x,y);
		if(use_filter){
			ofPushStyle();
			ofSetColor(scolor,120);
			ofFill();

			WindowTexture::GetInstance()->_fbo.getTextureReference().bind();
			_mesh.draw();
			WindowTexture::GetInstance()->_fbo.getTextureReference().unbind();

			ofPopStyle();
		}

		ofBeginShape();
			int c=(draw_fill)?1:mstroke;			
			for(int i=0;i<c;++i){

				
				float dx=(draw_fill)?0:cos(i+phi)*.05*wid;
				float dy=(draw_fill)?0:sin(i+phi)*.05*hei;

				ofVertex(0,dy);
				ofBezierVertex(dx,-dy,
								-dx,hei,
								dx,hei+dy);
				ofBezierVertex(0,hei+2*dy,
								wid,hei+dy,
								wid-dx,hei-dy);
				ofBezierVertex(wid,hei,
								wid-dx*2,dy,
								wid-dx,-dy);
				ofBezierVertex(wid,0,
								0,-dy,
						  		dx,dy);

				
			}
		ofEndShape();
		
		ofPopMatrix();


		ofPopStyle();
	}

	
};


#endif