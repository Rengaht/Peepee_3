#ifndef CLOUD_H
#define CLOUD_H

#include "ofMain.h"

class Cloud{
	
	float x,y,wid,hei;
	vector<float> arc_pos;
	int marc;
	float vel;
	float phi;
	float delta_x;
	float vel_y;

	ofColor fcolor;
	//Eye[] aeye;
public:
	Cloud(float x_,float y_,float wid_,float hei_){
		setup(x_,y_,wid_,hei_,ofColor(ofRandom(80,200),ofRandom(120,200)));
	}
	Cloud(float x_,float y_,float wid_,float hei_,ofColor fc_){
		setup(x_,y_,wid_,hei_,fc_);
	}
	void setup(float x_,float y_,float wid_,float hei_,ofColor fc_){
		x=x_; y=y_; wid=wid_; hei=hei_;
		marc=(int)ofRandom(10,24);
		
		vel=ofRandom(50,80);
		phi=ofRandom(TWO_PI);

		float tmp_pos=0;
		for(int i=0;i<marc;++i){
			// arc_pos.append(tmp_pos);
			arc_pos.push_back(ofRandom(-.1,.1)*hei+hei-sin(PI/(float)marc*i)*hei);
			//arc_pos.push_back(hei-sin(PI/(float)marc*i)*hei);
			
			//if(tmp_pos==wid) break;

			tmp_pos+=ofRandom(.8,1.2)*wid/marc;
			if(tmp_pos>wid) tmp_pos=wid;
		}
		marc=arc_pos.size();

		delta_x=wid*ofRandom(.1,.4);
		vel_y=ofRandom(.2,3);

		fcolor=fc_;
		
	}
	void draw(bool draw_fill,float dx_){
		float draw_portion=(float)ofGetFrameNum()/vel+phi;	
		ofPushStyle();
		if(draw_fill){
			ofSetColor(fcolor); 
		}else{			
			ofNoFill(); ofSetColor(120,180);
		}
		ofPushMatrix();
	

		ofTranslate(x,y);
			
		if(draw_fill){
			int mtur=marc;//(int)ofRandom(8,20);
			
			ofVec2f last_tur(0,0);
			ofBeginShape();
				for(int i=0;i<mtur;++i){
					ofVec2f cur_tur(wid/(float)mtur*i,arc_pos[i]);
					if(i%3==0) ofVertex(cur_tur.x,cur_tur.y);
					else ofBezierVertex(ofLerp(cur_tur.x,last_tur.x,-.3),cur_tur.y,
										ofLerp(cur_tur.x,last_tur.x,1.3),last_tur.y,
										cur_tur.x,cur_tur.y);
					last_tur=cur_tur;
				}
				ofVertex(wid,hei);
				ofVertex(0,hei);
			ofEndShape();
		}else{
		//if(draw_fill){
		//	// int meye=aeye.length;
		//	// println(meye+" eye to draw!");
		//	// for(int i=0;i<meye;++i) aeye[i].draw(pg,draw_fill);
		//}else{
			ofNoFill();
			ofSetColor(200,120);
			int mtur_=(int)ofRandom(6,12);
			ofVec2f last_tur_(0,0);
			ofTranslate(delta_x*sin(draw_portion)*ofRandom(.2,1.2),0);
			ofBeginShape();
			for(int i=0;i<mtur_;++i){
				ofVec2f cur_tur(wid/(float)mtur_*i,ofRandom(-.2,.2)*hei);
				if(i%3==0) ofVertex(cur_tur.x,cur_tur.y);
				else ofBezierVertex(ofLerp(cur_tur.x,last_tur_.x,-.3),cur_tur.y,
									ofLerp(cur_tur.x,last_tur_.x,1.3),last_tur_.y,
									cur_tur.x,cur_tur.y);
				last_tur_=cur_tur;
			}
			ofEndShape();
		//}
		}
		ofPopMatrix();

		ofPopStyle();

		update();
	}
	void update(){
		//y+=vel_y*sin((float)ofGetFrameNum()/vel);
		// if(ofRandom(50)<1) y+=vel_y*8;
		// else y+=vel_y;
		// if(y>height+hei) y=-hei;

	}
};

class Eye{
	float x,y,wid,hei;
	float vel,phi;
	int mhair;
	float vel_y;
public:
	Eye(float x_,float y_,float wid_,float hei_,float vel_,float phi_,int mhair_){
		x=x_; y=y_; wid=wid_; hei=hei_;
		vel=vel_;
		phi=phi_;
		mhair=mhair_;
		vel_y=ofRandom(.2,3);

		vel=ofRandom(50,80);
		phi=ofRandom(TWO_PI);
	}
	void draw(bool draw_fill,float dx_){
		drawPart(true);
		drawPart(false);
	}
	void drawPart(bool draw_fill){
		// if(draw_fill) return;

		float draw_portion=(float)ofGetFrameNum()/vel+phi;
		ofPushStyle();
		//ofSetColor(145,168,209);
		if(draw_fill){
			ofSetColor(180);
		}else{
			ofNoFill();
			ofSetColor(120);	
		}
		// pg.fill(0);

		ofPushMatrix();

		ofTranslate(x-wid/2,y);
		// pg.rect(0,0,wid,hei);
		

		ofBeginShape();
			ofVertex(0,0);
			ofBezierVertex(0,hei,wid,hei,
							wid,0);
			// ofBezierVertex(wid,hei*sin(draw_portion),0,hei*sin(draw_portion),
			//                 0,0);
		ofEndShape();
		
		if(draw_fill){
			ofPushStyle();
		
				ofPoint eyep=ofBezierPoint(ofPoint(0,0),ofPoint(-hei,hei),ofPoint(-hei,hei),ofPoint(0,0),.5);
				float eye_top=eyep.x;//bezierPoint(0,-hei,-hei,0,.5);//bezierPoint(0,hei*sin(draw_portion),hei*sin(draw_portion),0,.5);
				float eye_bottom=eyep.y;//bezierPoint(0,hei,hei,0,.5);
				float eye_rad=(eye_bottom-eye_top)*.8;
			
				ofSetColor(120);
				ofFill();
					ofEllipse(wid/2,eye_top+eye_rad/2,eye_rad,eye_rad);
			ofPopStyle();

		//pg.stroke(0,0);
		
			ofBeginShape();
				ofVertex(0,0);
				ofVertex(0,-hei);
				ofVertex(wid,-hei);
				ofVertex(wid,0);
				ofBezierVertex(wid,hei*sin(draw_portion),0,hei*sin(draw_portion),
								0,0);
			ofEndShape();
		}else{

			ofSetColor(80);
			ofNoFill();
			ofBeginShape();
				ofVertex(wid,0);
				ofBezierVertex(wid,hei*sin(draw_portion),0,hei*sin(draw_portion),
								0,0);
			ofEndShape();

			ofSetColor(80);
			ofNoFill();
			float eang=1/(float)mhair;
			for(int i=0;i<mhair;++i){

				ofPoint tmp=ofBezierPoint(ofPoint(0,0),ofPoint(0,hei),ofPoint(wid,hei),ofPoint(wid,0),eang*i);

				float tmp_x=tmp.x;//bezierPoint(0,0,wid,wid,eang*i);
				float tmp_y=tmp.y;//bezierPoint(0,hei,hei,0,eang*i);

				ofPoint tanp=ofBezierTangent(ofPoint(0,0),ofPoint(hei,0),ofPoint(hei,wid),ofPoint(0,wid),eang*i);

				float tan_x=-tanp.x/9;
				float tan_y=tanp.y/9;
			
				ofLine(tmp_x,tmp_y,tmp_x+tan_x,tmp_y+tan_y);
			}
		}
		
		ofPopMatrix();
		ofPopStyle();

		update();

	}
	void update(){
		y+=vel_y*sin((float)ofGetFrameNum()/vel);
		
	}
};

#endif