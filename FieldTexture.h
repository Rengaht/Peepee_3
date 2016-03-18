#ifndef FIELD_TEXTURE_H
#define FIELD_TEXTURE_H

#include "ofMain.h"

class LeafTexture{
	static LeafTexture* _instance;

public:

	ofFbo _fbo;
	float wid,hei;
	ofColor _scolor[5];
	LeafTexture(){}
	static LeafTexture* GetInstance(){
		if(!_instance)
			_instance=new LeafTexture();
		return _instance;
	}

	void setup(float w,float h){
		wid=w; hei=h;
		_fbo.allocate(wid,hei,GL_RGBA);
		_scolor[0]=ofColor(115,188,217);
		_scolor[1]=ofColor(83,127,166);
		_scolor[2]=ofColor(132,177,217);
		_scolor[3]=ofColor(187,216,242);
		_scolor[4]=ofColor(186,203,217);

		draw();
	}
	void draw(){
		_fbo.begin();
		ofClear(255,255,255,0);
		int c=2000;
		for(int i=0;i<c;++i){
			ofVec2f pos(ofRandom(wid),ofRandom(hei));
			ofPushStyle();
			int ic=(int)ofRandom(5);
			ofSetColor(_scolor[ic].r+ofRandom(-10,10),_scolor[ic].g+ofRandom(-10,10),_scolor[ic].b+ofRandom(-10,10),180+ofRandom(70));
			ofBeginShape();
				int mang=(int)(8*(sin(pos.y*2)*.5+1));
				float tmp_ang=0;
		
				for(int k=0;k<mang;++k){
					float tmp_rad=12*(sin(pos.y*2+k)*.3+1);
					ofVec2f tmpv(0,tmp_rad);
					tmpv.rotate(tmp_ang*RAD_TO_DEG);
					ofVertex(pos.x+tmpv.x,pos.y+tmpv.y);

					tmp_ang+=(TWO_PI/mang)*(sin(pos.x)*.4+1);
				}
			ofEndShape(true);
			ofPopStyle();
		}
		_fbo.end();
	}
};
class FieldTexture{
	static FieldTexture* _instance;

public:

	ofFbo _fbo;
	float wid,hei;
	ofColor _scolor[5];

	FieldTexture(){}
	static FieldTexture* GetInstance(){
		if(!_instance)
			_instance=new FieldTexture();
		return _instance;
	}

	void setup(float w,float h){
		wid=w; hei=h;
		_fbo.allocate(wid,hei,GL_RGBA);

		_scolor[0]=ofColor(124,160,118);
		_scolor[1]=ofColor(157,192,169);
		_scolor[2]=ofColor(129,163,144);
		_scolor[3]=ofColor(90,141,100);
		_scolor[4]=ofColor(140,152,139);

		draw();
	}
	void draw(){
		_fbo.begin();
		ofClear(255,255,255,0);
		int c=2000;
		float phi=ofRandom(TWO_PI);
		for(int i=0;i<c;++i){
			ofVec2f pos(ofRandom(wid),ofRandom(hei));
			ofPushStyle();
			ofColor c=_scolor[int(ofRandom(5))];
			ofSetColor(c.r+ofRandom(-5,5),c.g+ofRandom(-5,5),c.b+ofRandom(-5,5),120+ofRandom(60));
			//ofSetColor(ofRandom(83,156),ofRandom(125,148),ofRandom(76,83),120+ofRandom(50));
			ofPushMatrix();
			ofTranslate(pos.x,pos.y);
			ofBeginShape();
				/*int mang=(int)(8*(sin(pos.y*2)*.5+1));
				float tmp_ang=0;
		
				for(int k=0;k<mang;++k){
					float tmp_rad=6*(sin(pos.y*2+k)*.3+1);
					ofVec2f tmpv(0,tmp_rad);
					tmpv.rotate(tmp_ang*RAD_TO_DEG);
					ofVertex(pos.x+tmpv.x,pos.y+tmpv.y);

					tmp_ang+=(TWO_PI/mang)*(sin(pos.x)*.4+1);
				}*/

				float dx2=abs(cos(i+phi));
				float dy2=abs(sin(i+phi));														

				float ele=6;
				float scl=ele*1.6;
				float sr=ofRandom(0.7,1)*ele;

				ofVertex(0,-sr);
				ofBezierVertex(dx2*sr,-dx2*sr,
									dx2*sr,dy2*sr,
									0,sr);
				ofBezierVertex(-dx2*ele*scl,dy2*sr,
									-dy2*ele*scl,dy2*sr,
									0,-sr);
				

			ofEndShape(true);
			ofPopMatrix();

			ofPopStyle();
		}

		ofPushStyle();
		ofSetColor(120,180);
		
		ofNoFill();
		float line_wei=5;
		float line_count=wid/line_wei;
		for(int i=0;i<line_count;++i){
			ofTranslate(line_wei*ofRandom(.8,1.2),0);
			ofSetLineWidth(ofRandom(1,3));
			ofBeginShape();
			int curve_count=ofRandom(30,120);
			for(int k=0;k<curve_count;++k){
				ofVertex(ofRandom(-.2,.2)*line_wei,hei/(float)curve_count*k);
			}
			ofEndShape();
		}
		ofPopStyle();
		
		_fbo.end();
	}
};

class OceanTexture{
	static OceanTexture* _instance;

public:

	ofFbo _fbo;
	float wid,hei;
	ofColor _scolor[5];

	OceanTexture(){}
	static OceanTexture* GetInstance(){
		if(!_instance)
			_instance=new OceanTexture();
		return _instance;
	}

	void setup(float w,float h){
		wid=w; hei=h;
		_fbo.allocate(wid,hei,GL_RGBA);

		_scolor[0]=ofColor(0,33,69);
		_scolor[1]=ofColor(5,62,103);
		_scolor[2]=ofColor(24,74,104);
		_scolor[3]=ofColor(66,124,132);
		_scolor[4]=ofColor(132,188,171);

		draw();


	}
	void draw(){
		_fbo.begin();
		ofBackground(255,0);
		float mline=80;
		for(int j=0;j<mline;++j){
			
			float twid=ofRandom(.3,.8)*wid;
			ofVec2f pos(ofRandom(0,wid-twid),ofRandom(.8,1.2)*hei/mline*j);
			
			ofPushMatrix();
			ofTranslate(pos.x,pos.y);

			ofPushStyle();
			if(ofRandom(30)<1) ofSetColor(242,180,65,ofRandom(180,255));
			else ofSetColor(62,68,89,ofRandom(180,255));
			ofNoFill();
			ofBeginShape();
			
			int mtur_=floor(ofRandom(150,300));
			
			
			ofVec2f last_tur_(0,0);
			for(int i=0;i<mtur_;++i){
				ofVec2f cur_tur(twid/(float)mtur_*i,ofRandom(-1,1)*hei/mline);
				if(i%3==0) ofVertex(cur_tur.x,cur_tur.y);
				else ofBezierVertex(ofLerp(cur_tur.x,last_tur_.x,-.3),cur_tur.y,
									ofLerp(cur_tur.x,last_tur_.x,1.3),last_tur_.y,
									cur_tur.x,cur_tur.y);
				last_tur_=cur_tur;
			}
			ofEndShape();
			ofPopStyle();

			ofPopMatrix();
		}
		_fbo.end();
	}

};


class RoofTexture{
	static RoofTexture* _instance;

public:

	ofFbo _fbo;
	float wid,hei;

	RoofTexture(){}
	static RoofTexture* GetInstance(){
		if(!_instance)
			_instance=new RoofTexture();
		return _instance;
	}

	void setup(float w,float h){
		wid=w; hei=h;
		_fbo.allocate(wid,hei,GL_RGBA);
		draw();
	}
	void draw(){
		_fbo.begin();
		
		ofBackground(139,143,128,200);

		ofPushStyle();
		int rect_count=30;
		for(int i=0;i<rect_count;++i){
			ofSetColor(114+ofRandom(20),132,113+ofRandom(20),120);
			ofFill();
			ofPushStyle();
			ofPushMatrix();
			ofTranslate(ofRandom(wid),ofRandom(hei));
			ofRotate(ofRandom(360));				
				ofRect(0,0,ofRandom(.3,1.2)*wid,ofRandom(.3,1.2)*hei);
			
			ofPopMatrix();
			ofPopStyle();
		}

		ofPushStyle();
		ofSetColor(60,180);
		ofNoFill();
		float line_wei=3;
		float line_count=wid/line_wei;
		for(int i=0;i<line_count;++i){
			ofTranslate(line_wei*ofRandom(.8,1.2),0);
			ofBeginShape();
			int curve_count=ofRandom(30,120);
			for(int k=0;k<curve_count;++k){
				ofVertex(ofRandom(-.1,.1)*line_wei,hei/(float)curve_count*k);
			}
			ofEndShape();
		}
		ofPopStyle();
		

		ofPopStyle();
		
		_fbo.end();
	}

};

class WindowTexture{
	static WindowTexture* _instance;

public:

	ofFbo _fbo;
	float wid,hei;

	WindowTexture(){}
	static WindowTexture* GetInstance(){
		if(!_instance)
			_instance=new WindowTexture();
		return _instance;
	}

	void setup(float w,float h){
		wid=w; hei=h;
		_fbo.allocate(wid,hei,GL_RGBA);
		draw();
	}
	void draw(){
		_fbo.begin();
		
		
		ofPushStyle();
		ofClear(255,255,255,0);
	
		
		//		for(int i=0;i<140;++i){
        //			ofVec2f pos(ofRandom(wid),ofRandom(hei));
        //			ofPushStyle();
        //			int ic=(int)ofRandom(5);
        //			ofSetColor(245+ofRandom(-10,10),200+ofRandom(50));
        //			ofBeginShape();
        //				int mang=(int)(5*(sin(pos.y*2)*.5+1));
        //				float tmp_ang=0;
        //
        //				for(int k=0;k<mang;++k){
        //					float tmp_rad=12*(sin(pos.y*2+k)*.3+1);
        //					ofVec2f tmpv(0,tmp_rad);
        //					tmpv.rotate(tmp_ang*RAD_TO_DEG);
        //					ofVertex(pos.x+tmpv.x,pos.y+tmpv.y);
        //
        //					tmp_ang+=(TWO_PI/mang)*(sin(pos.x)*.4+1);
        //				}
        //			ofEndShape(true);
        //			ofPopStyle();
        //		}
		
		ofPushMatrix();
		float line_wei=2;
		float line_count=wid/line_wei;
		for(int i=0;i<line_count;++i){
			
			ofSetColor(245+ofRandom(-10,10),180+ofRandom(70));
            
			ofTranslate(line_wei*ofRandom(.8,1.2),0);
			
			//			ofSetLineWidth(ofRandom(3,8));
            //			ofBeginShape();
			int curve_count=ofRandom(5,20);
			for(int k=0;k<curve_count;++k){
				//ofVertex(wid/(float)curve_count*(float)k,ofRandom(-2,2)*line_wei);
                ofVec2f pos(ofRandom(-.2,.2)*line_wei,hei/(float)curve_count*(float)k);
                ofBeginShape();
                int mang=(int)ofRandom(3,5);
                float tmp_ang=0;
                float tmp_rad=line_wei*(sin(pos.y*2)*.3+1);
                
                for(int k=0;k<mang;++k){
                    ofVec2f tmpv(0,tmp_rad);
                    tmpv.rotate(tmp_ang*RAD_TO_DEG);
                    ofVertex(pos.x+tmpv.x,pos.y+tmpv.y);
                    
                    tmp_ang+=(TWO_PI/mang)*(sin(pos.x)*.4+1);
                }
                ofEndShape(true);
                
			}
            //			ofEndShape();
			//ofLine(ofRandom(.5)*wid,0,ofRandom(.5,1)*wid,0);
		}		ofPopMatrix();

		ofPopStyle();
		
		_fbo.end();
	}

};


#endif