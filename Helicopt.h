#ifndef HELICOPT_H
#define HELICOPT_H

#include "ofMain.h"
#include "FrameAnimation.h"


class Helicopt{
	
	float wid,hei;
	int mwing;
	float ang_pos;
	float vel;
	float phi;
	
	float hei_strength;
	float hei_base;
	FrameAnimation _anim_fly;
public:

	float delta_hei;
	Helicopt(){}
	Helicopt(float wid_,float hei_){
		wid=wid_; hei=hei_;
		mwing=(int)ofRandom(3,5);
		ang_pos=ofRandom(TWO_PI);
		vel=TWO_PI/ofRandom(5,25);
		phi=ofRandom(TWO_PI);

		_anim_fly=FrameAnimation(ofRandom(40,60),ofRandom(40));
		_anim_fly.is_loop=true;
		_anim_fly.do_reverse=true;
		_anim_fly.Restart();

		hei_strength=ofRandom(2,5)*hei;
		hei_base=ofRandom(-2,2)*hei;
		
	
	}

	void draw(bool draw_fill,float x,float y){

		_anim_fly.Update();

		//float draw_portion=(float)frameCount/10+phi;
		float draw_portion=_anim_fly.GetPortion();
		
		//delta_hei=hei_strength*sin(HALF_PI*draw_portion+HALF_PI*_anim_fly._loop_count);
		delta_hei=hei_strength*(draw_portion);
		float tail_hei=hei_base+hei_strength;	
		
		ofPushStyle();
		if(!draw_fill){
			ofNoFill();
			ofSetColor(20,35,40);
		}else{
			ofSetColor(184*ofRandom(.8,1.2),180);
			//pg.noStroke();
		}
		ofPushMatrix();
		ofTranslate(x,y);
		if(!draw_fill){
			ofBeginShape();
				ofVertex(0,0);
				ofBezierVertex(wid/25*sin(draw_portion),-tail_hei/4,wid/32*sin(draw_portion),-tail_hei/3*2.5,
								0,-tail_hei);
			ofEndShape();
		
			ofTranslate(0,-tail_hei);
			float eang=TWO_PI/(float)mwing;
			for(int i=0;i<mwing;++i){
				drawWing(eang*i+ang_pos);
			}
		}else{

			ofTranslate(0,-tail_hei);
			
			ofBeginShape();
			float eang=TWO_PI/(float)mwing/2;
			for(int i=0;i<mwing*2;++i){
				float ang_=i*eang+ang_pos;
				float rad_=ofRandom(-.1,1.1);
				ofVec2f pos(wid*cos(ang_),hei*sin(ang_)*abs((float)i/10));
				ofVertex(pos.x,pos.y);
			}
			ofEndShape();
		}
		
		ofPopMatrix();

		ofPopStyle();
		
		ang_pos+=vel;

	}
	void drawWing(float ang){
		// ofVec2f pos=new ofVec2f(rad*cos(ang),rad*sin(ang));
		ofBeginShape();
			ofVertex(0,0);
			int mvert=(int)ofRandom(3,8);
			for(int i=0;i<mvert;++i){
				float ang_=ang+ofRandom(-.05,.05)*TWO_PI/(float)mwing;
				float rad_=ofRandom(-.1,1.1);
				ofVec2f pos(wid*rad_*cos(ang_),hei*rad_*sin(ang_)*abs((float)i/10));
				ofVertex(pos.x,pos.y);
			}
		ofEndShape();
	}

};







#endif