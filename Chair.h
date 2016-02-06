#ifndef CHAIR_H
#define CHAIR_H

#include "ofMain.h"

class TrainBase{
public:
	float x,y,wid,hei;
	TrainBase(){}	
	void update(float speed){
		x-=speed;	
	}

	bool isDead(){
		return x+wid*1.5<0;
	}
};

class Chair:public TrainBase{
	float wid1,wid2,hei1,hei2;
	ofColor fcolor;
	int mstroke;
	float phi;
	
public:
	Chair(float x_,float y_,float w_,float h_){
		x=x_; y=y_; 
		wid1=w_*ofRandom(.8,1); 
		wid2=w_*ofRandom(1,1.5);
		hei1=h_*ofRandom(.6,.8);
		hei2=h_-hei1;

		wid=max(wid1,wid2);

		mstroke=floor(ofRandom(1,4));
		phi=ofRandom(TWO_PI);

		fcolor=ofColor(255);

		//if(ofRandom(2)<1) human=new Human(0,h_*.3,w_,h_*.7);
	}
	void draw(bool draw_fill,float dx_,ofColor scene_color_){
		
		ofPushStyle();
		if(draw_fill){
			ofSetColor(scene_color_);				
			//pg.noStroke();
		}else{
			ofSetColor(scene_color_!=ofColor::white?ofColor::white:ofColor::black);
			ofNoFill();
			//ofSetColor(fofColor);
		}

		ofPushMatrix();
		ofTranslate(x,y);

		ofBeginShape();
			int c=(draw_fill)?1:mstroke;			
			for(int i=0;i<c;++i){
				if(draw_fill) i+=mstroke+1;
				float dx=(draw_fill)?0:cos(i+phi)*.05*wid1;
				float dy=(draw_fill)?0:sin(i+phi)*.05*hei1;

				ofVertex(0,0);
				ofBezierVertex(-dx,0,
								-dx,hei1,
								dx,hei1+dy);
				ofBezierVertex(0,hei1+2*dy,
								wid1,hei1+dy,
								wid1-dx,hei1-dy);
				ofBezierVertex(wid1,hei1,
								wid1-dx*2,dy,
								wid1-dx,-dy);
				ofBezierVertex(wid1,0,
								0,-dy,
						  		dx,dy);
			}
		ofEndShape();
		ofPushMatrix();
		ofTranslate(0,hei1);
		ofBeginShape();
			for(int i=0;i<c;++i){
				if(draw_fill) i+=mstroke+1;

				float dx=(draw_fill)?0:cos(i+phi)*.06*wid2;
				float dy=(draw_fill)?0:sin(i+phi)*.03*hei2;

				ofVertex(0,0);
				ofBezierVertex(-dx,0,
								-dx,hei2,
								dx,hei2+dy);
				ofBezierVertex(0,hei2+2*dy,
								wid2,hei2+dy,
								wid2+dx,hei2-dy);
				ofBezierVertex(wid2,hei2,
								wid1-dx*2,dy,
								wid1-dx,-dy);
				ofBezierVertex(wid1,0,
								0,-dy,
						  		dx,dy);
			}
		ofEndShape();
		ofPopMatrix();

			
		ofPopMatrix();

		ofPopStyle();
	}

};



#endif