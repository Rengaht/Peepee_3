#ifndef HANDLE_H
#define HANDLE_H
#include "ofMain.h"
#include "Chair.h"


class THandle:public TrainBase{
	
	//float x,y,wid,hei;
	ofColor fofColor;
	int mstroke;
	float phi;
public:
	THandle(float x_,float y_,float w_,float h_){
		x=x_; y=y_; 
		wid=w_; 
		hei=h_;
		mstroke=floor(ofRandom(1,4));
		phi=ofRandom(TWO_PI);

		fofColor=ofColor(155,159,150);
	}
	void draw(bool draw_fill,float dx_){
		
		ofPushStyle();
		if(draw_fill){
			ofSetColor(fofColor);
			
		}else{
			ofSetColor(0);
			ofNoFill();
			//ofSetColor(fofColor);
		}

		ofPushMatrix();
		ofTranslate(x+wid/2,y);

		ofBeginShape();
			int c=(draw_fill)?1:mstroke;			
			for(int i=0;i<c;++i){
				if(draw_fill) i+=mstroke+1;
				float dx=(draw_fill)?0:cos(i+phi)*.05*wid;
				float dy=(draw_fill)?0:sin(i+phi)*.05*hei;

					ofVertex(-dx,0);
					ofVertex(0,hei-wid);
					ofBezierVertex(-wid/2+dx,hei-dy,
									-wid/2-dx,hei,
									0,hei+dy);
					ofBezierVertex(wid/2-2*dx,hei+dy,
									wid/2+dx,hei,
									dx,hei-wid);
					//ofVertex(dx,hei-wid);
				
			}
		ofEndShape();
		
		
		ofPopMatrix();

		ofPopStyle();
	}


};








#endif