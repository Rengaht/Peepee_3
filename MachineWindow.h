#ifndef MACHINE_WINDOW_H
#define MACHINE_WINDOW_H

#include "ofMain.h"

class MachineWindow{

	float x,y,wid,hei;
	int mhoriz,mvert;
	ofColor fcolor;
	//boolean is_circle=ofRandom(3)<1;
	float distort_x,distort_y;
	/*float dx=ofRandom(.01,.05);
	float dy=ofRandom(.01,.05);*/
	
	float parent_x,parent_wid;

	vector<ofVec2f> light_pos;

	float phi;
public:

	MachineWindow(float x_,float y_,float wid_,float hei_){
		
		x=x_; y=y_; wid=wid_; hei=hei_;
	
		phi=ofRandom(TWO_PI);

		mvert=(int)ofRandom(1,ofMap(wid_,0,200,2,4));
		mhoriz=(int)ofRandom(1,ofMap(wid_,0,200,2,4));
		
		distort_x=ofRandom(.05,.1);
		distort_y=ofRandom(.05,.1);

		fcolor=ofColor(193+ofRandom(-10,10),183,176,220);
		
		int c=(int)ofRandom(0,2);
		
		for(int i=0;i<c;++i){
			light_pos.push_back(ofVec2f(floor(ofRandom(mhoriz)),floor(ofRandom(mvert))));
		}
	}	
	
	void draw(bool draw_fill){
		
		// x+=delta_x;

		ofPushStyle();
		if(draw_fill){
			ofSetColor(fcolor);
		}else{
			ofSetColor(90,120);
			ofNoFill();			
		}

		float swid=wid/(float)mhoriz;
		float shei=hei/(float)mvert;
		ofPushMatrix();


		float tx_=x;//getCurrentX(dx_,x,wid);
		ofTranslate(tx_,y);
		//_path.draw();
		//pg.shearY(PI/4);
			for(int i=0;i<mhoriz;++i)
				for(int j=0;j<mvert;++j){
					
					
					float tmp_wid=swid*(1-distort_x*2);
					float tmp_hei=shei*(1-distort_y*2);
					ofPushMatrix();
					ofTranslate((i+distort_x)*swid,(j+distort_y)*shei);
						

						float dx=sin(phi+i)*.1*tmp_wid;
						float dy=cos(phi+j)*.1*tmp_hei;

						ofBeginShape();
						ofVertex(0,0);
						ofVertex(dx,tmp_hei);
						ofVertex(tmp_wid,tmp_hei-dy);
						ofVertex(tmp_wid+dx,0);
						ofVertex(0,dy);
						ofEndShape();
							

					ofPopMatrix();
				}
		ofPopMatrix();
		
		ofPopStyle();	
	}

	void drawLightWindow(){
		ofPushStyle();
		//if(draw_fill){
			//ofSetColor(fcolor);
		//}else{
			//ofSetColor(90,120);
			//ofNoFill();			
		//}

		float swid=wid/(float)mhoriz;
		float shei=hei/(float)mvert;
		ofPushMatrix();


		float tx_=x;//getCurrentX(dx_,x,wid);
		ofTranslate(tx_,y);
		//ofEnableAlphaBlending();
		//_path.draw();
		//pg.shearY(PI/4);

		float fr=ofGetFrameNum();
		for(auto& pos:light_pos){
			
			int i=pos.x;
			int j=pos.y;
			/*for(int i=0;i<mhoriz;++i)
				for(int j=0;j<mvert;++j){*/			
					ofSetColor(203,255,124+60*abs(sin(j)),85);					
					
					float tmp_wid=swid*(1-distort_x*2);
					float tmp_hei=shei*(1-distort_y*2);
					ofPushMatrix();
					ofTranslate((i+distort_x)*swid,(j+distort_y)*shei);
						

						float dx=sin(phi+i)*.2*tmp_wid;
						float dy=cos(phi+j)*.2*tmp_hei;

						ofBeginShape();
						ofVertex(dx,0);
						ofVertex(dx,tmp_hei);
						ofVertex(tmp_wid-dx,tmp_hei-dy);
						ofVertex(tmp_wid+dx,0);
						ofVertex(0,dy);
						ofEndShape();
							

					ofPopMatrix();
				}
		ofPopMatrix();
		
		ofPopStyle();

	}

};
class FieldWindow{
	float x,y,wid,hei;
	ofColor fcolor;
	
	float distort_x,distort_y;
	
	float parent_x,parent_wid;

	//int mode;
	float phi;
	vector<ofVec2f> grass;
	ofMesh mesh_;

	int vel;
public:
	bool draw_land;

	FieldWindow(float x_,float y_,float wid_,float hei_,bool draw_land_){
		setup(x_,y_,wid_,hei_,draw_land_);
	}
	FieldWindow(float x_,float y_,float wid_,float hei_){
		setup(x_,y_,wid_,hei_,true);
	}
	void setup(float x_,float y_,float wid_,float hei_,bool draw_land_){
		x=x_; y=y_; wid=wid_; hei=hei_;
		//mode=mode_;
		phi=ofRandom(TWO_PI);

		distort_x=.01;
		distort_y=.01;

		vel=floor(ofRandom(20,30));

		//if(ofRandom(10)<1) fcolor=ofColor(146,168,209,120);
		
		if(!draw_land) fcolor=ofColor(255,ofRandom(100,180));
		else{
			ofColor sc=FieldTexture::GetInstance()->_scolor[int(ofRandom(5))];
			fcolor=ofColor(sc.r+ofRandom(-5,5),sc.g+ofRandom(-5,5),sc.b+ofRandom(-5,5),120+ofRandom(60));		
		}
		int c=floor(ofRandom(5,15));
		for(int i=0;i<c;++i) grass.push_back(ofVec2f(ofRandom(1)*wid,ofRandom(1)*hei));
		
		draw_land=draw_land_;

		createMesh();
	}

	void createMesh(){
		float tmp_wid=wid*(1-distort_x*2);
		float tmp_hei=hei*(1-distort_y*2);											
		float dx=(!draw_land)?0:cos(phi)*.2*tmp_wid;
		float dy=(!draw_land)?0:sin(phi)*.2*tmp_hei;

		mesh_.addVertex(ofPoint(0,0));
		mesh_.addVertex(ofPoint(dx,tmp_hei));
		mesh_.addVertex(ofPoint(tmp_wid,tmp_hei-dy));
								
		mesh_.addVertex(ofPoint(tmp_wid,tmp_hei-dy));
		mesh_.addVertex(ofPoint(tmp_wid+dx,0));
		mesh_.addVertex(ofPoint(0,0));
								
		//mesh_.addVertex(ofPoint(0,dy));
								
		float W=ofGetWidth();
		float H=ofGetHeight()*.33;

		mesh_.addTexCoord(ofPoint(x/W,y/H));
		mesh_.addTexCoord(ofPoint((x+dx)/W,(y+tmp_hei)/H));
		mesh_.addTexCoord(ofPoint((x+tmp_wid)/W,(y+tmp_hei-dy)/H));
								
		mesh_.addTexCoord(ofPoint((x+tmp_wid)/W,(y+tmp_hei-dy)/H));
		mesh_.addTexCoord(ofPoint((x+tmp_wid+dx)/W,y/H));
		mesh_.addTexCoord(ofPoint(x/W,y/H));

		
	}	
	void updateMesh(){

		float frameCount=(float)ofGetFrameNum()/100+phi;

		float tmp_wid=wid*(1-distort_x*2);
		float tmp_hei=hei*(1-distort_y*2);											

		
		float dx=cos(phi)*(!draw_land?.5:.2)*tmp_wid;
		float dy=sin(phi)*.2*tmp_hei;
		
		float tx=x+tmp_wid*(!draw_land?.5:.3)*sin(frameCount);
		float ty=y+tmp_hei*.3*cos(frameCount);

		float W=ofGetWidth();
		float H=ofGetHeight()*.33;

		mesh_.setTexCoord(0,ofPoint(tx/W,ty/H));
		mesh_.setTexCoord(1,ofPoint((tx+dx)/W,(ty+tmp_hei)/H));
		mesh_.setTexCoord(2,ofPoint((tx+tmp_wid)/W,(ty+tmp_hei-dy)/H));
								
		mesh_.setTexCoord(3,ofPoint((tx+tmp_wid)/W,(ty+tmp_hei-dy)/H));
		mesh_.setTexCoord(4,ofPoint((tx+tmp_wid+dx)/W,ty/H));
		mesh_.setTexCoord(5,ofPoint(tx/W,ty/H));
	}
	
	void draw(bool draw_fill){
		
		// x+=delta_x;
		if(ofGetFrameNum()%vel==0) updateMesh();

		ofPushStyle();
		//if(draw_fill){
			ofSetColor(fcolor);
		/*}else{
			ofSetColor(90,120);
			ofNoFill();			
		}*/

		
		ofPushMatrix();


		//float tx_=x;//getCurrentX(dx_,x,wid);
		ofTranslate(x,y);
		/*ofMatrix4x4 mat; 
		mat.set(1,-tan(PI/8),0,0,
				0,1,0,0,
				0,0,1,0,
				0,0,0,1);
		ofMultMatrix(mat);*/

		
			if(draw_land){ 
				mesh_.draw();

				ofSetColor(ofMap(fcolor.g,0,255,180,255));
				ofTranslate(.2*wid*sin(phi),.2*hei*cos(phi));
				FieldTexture::GetInstance()->_fbo.getTextureReference().bind();
				//ofSetTextureWrap(GL_REPEAT,GL_REPEAT);
					mesh_.draw();						
				FieldTexture::GetInstance()->_fbo.getTextureReference().unbind();
			}else{
				LeafTexture::GetInstance()->_fbo.getTextureReference().bind();
				//ofSetTextureWrap(GL_REPEAT,GL_REPEAT);
					mesh_.draw();						
				LeafTexture::GetInstance()->_fbo.getTextureReference().unbind();
			}
					
		ofPopMatrix();
		
		ofPopStyle();	
	}

};
class CityRoof{
	float x,y,wid,hei;
	ofColor fcolor;
	
	float distort_x,distort_y;
	
	float phi;
	ofMesh mesh_;

	int vel;
public:

	CityRoof(float x_,float y_,float wid_,float hei_){
		
		x=x_; y=y_; wid=wid_; hei=hei_;
		//mode=mode_;
		phi=ofRandom(-PI/12,PI/12);

		distort_x=.01;
		distort_y=.01;

		vel=floor(ofRandom(30,80));

		fcolor=ofColor(255);//182,186+ofRandom(20),173+ofRandom(20),120+ofRandom(50));
		
		createMesh();
	}

	void createMesh(){
		float tmp_wid=wid*(1-distort_x*2);
		float tmp_hei=hei*(1-distort_y*2);											
		float dx=cos(phi)*.1*tmp_wid;
		float dy=sin(phi)*.2*tmp_hei;

		mesh_.addVertex(ofPoint(0,0));
		mesh_.addVertex(ofPoint(dx,tmp_hei));
		mesh_.addVertex(ofPoint(tmp_wid,tmp_hei-dy));
								
		mesh_.addVertex(ofPoint(tmp_wid,tmp_hei-dy));
		mesh_.addVertex(ofPoint(tmp_wid-dx,0));
		mesh_.addVertex(ofPoint(0,0));
								
		//mesh_.addVertex(ofPoint(0,dy));
								
		float W=ofGetWidth();
		float H=ofGetHeight()*.33;

		mesh_.addTexCoord(ofPoint(x/W,y/H));
		mesh_.addTexCoord(ofPoint((x+dx)/W,(y+tmp_hei)/H));
		mesh_.addTexCoord(ofPoint((x+tmp_wid)/W,(y+tmp_hei-dy)/H));
								
		mesh_.addTexCoord(ofPoint((x+tmp_wid)/W,(y+tmp_hei-dy)/H));
		mesh_.addTexCoord(ofPoint((x+tmp_wid+dx)/W,y/H));
		mesh_.addTexCoord(ofPoint(x/W,y/H));

		
	}	
	void updateMesh(){

		float frameCount=(float)ofGetFrameNum()/100+phi;

		float tmp_wid=wid*(1-distort_x*2);
		float tmp_hei=hei*(1-distort_y*2);											

		
		float dx=cos(phi)*.2*tmp_wid;
		float dy=sin(phi)*.2*tmp_hei;
		
		float tx=x+tmp_wid*.3*sin(frameCount);
		float ty=y;//+tmp_hei*.3*cos(frameCount);

		float W=ofGetWidth();
		float H=ofGetHeight()*.33;

		mesh_.setTexCoord(0,ofPoint(tx/W,ty/H));
		mesh_.setTexCoord(1,ofPoint((tx+dx)/W,(ty+tmp_hei)/H));
		mesh_.setTexCoord(2,ofPoint((tx+tmp_wid)/W,(ty+tmp_hei-dy)/H));
								
		mesh_.setTexCoord(3,ofPoint((tx+tmp_wid)/W,(ty+tmp_hei-dy)/H));
		mesh_.setTexCoord(4,ofPoint((tx+tmp_wid+dx)/W,ty/H));
		mesh_.setTexCoord(5,ofPoint(tx/W,ty/H));
	}
	
	void draw(bool draw_fill){
		
		// x+=delta_x;
		if(ofGetFrameNum()%vel==0) updateMesh();

		ofPushStyle();
		if(draw_fill){
			ofSetColor(fcolor);
		}else{
			ofSetColor(90,120);
			ofNoFill();			
		}

		
		ofPushMatrix();


		//float tx_=x;//getCurrentX(dx_,x,wid);
		ofTranslate(x,y);
		ofMatrix4x4 mat; 
		mat.set(1,-tan(phi),0,0,
				0,1,0,0,
				0,0,1,0,
				0,0,0,1);
		ofMultMatrix(mat);
			//mesh_.draw();
			//ofTranslate(.2*wid*sin(phi),.2*hei*cos(phi));
			RoofTexture::GetInstance()->_fbo.getTextureReference().bind();
			mesh_.draw();						
			RoofTexture::GetInstance()->_fbo.getTextureReference().unbind();							
					
		ofPopMatrix();
		
		ofPopStyle();	
	}

};
class Island{

	float x,y,wid,hei;

	ofColor fcolor;
	float distort_x,distort_y;
	float phi;

	vector<float> arc_pos;
	ofMesh mesh_;

public:

	Island(float x_,float y_,float wid_,float hei_){
		
		x=x_; y=y_; wid=wid_; hei=hei_;
		phi=ofRandom(TWO_PI);


		distort_x=ofRandom(.05,.1);
		distort_y=ofRandom(.05,.1);
		
		fcolor=ofColor(ofRandom(193,200),ofRandom(177,191),ofRandom(155,186));
		
		int marc=ofRandom(10,25);
		float tmp_pos=0;
		for(int i=0;i<marc;++i){
			// arc_pos.append(tmp_pos);
			arc_pos.push_back(ofRandom(-.2,.2)*hei+hei/2-sin(PI/marc*i)*hei/2);			

			tmp_pos+=ofRandom(.1,2.5)*wid/marc;
			if(tmp_pos>=wid) break;
		}

		createMesh();
	}	
	void createMesh(){


	}
	
	void draw(bool draw_fill){
		
		// x+=delta_x;

		ofPushStyle();
		if(draw_fill){
			ofSetColor(fcolor);
		}else{
			ofSetColor(90,120);
			ofNoFill();			
		}

		ofPushMatrix();

		ofTranslate(x,y);
			ofBeginShape();
				int mtur=arc_pos.size();
			
				for(int i=0;i<mtur;++i){
					ofVec2f cur_tur(wid/(float)mtur*i,arc_pos[i]);
					ofVertex(cur_tur.x,cur_tur.y);
				}
				
				ofVertex(wid,hei);
				ofBezierVertex(wid/3*2,hei*(1+abs(cos(phi))),
								wid/3,hei*(1+abs(sin(phi))),
								0,hei);

			ofEndShape();
		ofPopMatrix();
		ofPopStyle();
	}

};

class RoadLamp{

	float x,y,wid,hei;

	ofColor fcolor;
	float phi;
	ofMesh mesh_;
	int mstroke;
public:

	RoadLamp(float x_,float y_,float wid_,float hei_){
		
		x=x_; y=y_; wid=wid_; hei=hei_;
		phi=ofRandom(TWO_PI);
		mstroke=floor(ofRandom(1,3));
	}
	void draw(){
		ofPushStyle();
		ofSetColor(80);
		ofNoFill();

		ofPushMatrix();
		ofTranslate(x,y);
		
		//ofRect(0,0,wid,hei);
		float lamp_rad=wid*.4;
		
		drawLight(lamp_rad,0,0,true);
					
			for(int i=0;i<mstroke;++i){
				float dx=cos(i+phi)*.02*wid;
				float dy=sin(i+phi)*.02*hei;
					
					drawLight(lamp_rad,dx,dy,false);
					

					ofBeginShape();
						ofVertex(wid/2+dx/2,lamp_rad/2+dy);
						ofBezierVertex(wid/2+dx,hei*.7-dy,
										wid/2-dx,hei,
										wid/2-dx*10,hei+dy);
						ofVertex(wid/2+dx*10,hei);
						ofBezierVertex(wid/2+dx,hei,
										wid/2-dx,hei*.8+dy,
										wid/2-dx/2,lamp_rad/2);
					ofEndShape();
			}
		


		ofPopMatrix();

		ofPopStyle();

	}

	void drawLight(float lamp_rad,float dx,float dy,bool light){

		ofPushStyle();
		if(light){
			ofSetColor(236,160*(1+.2*sin(phi)),55*(1+.2*sin(phi)));
			ofFill();
		}
		ofBeginShape();
			ofVertex(wid/2,0);
			ofBezierVertex(wid/2+lamp_rad*.55,0,
							wid/2+lamp_rad*.55+dx,lamp_rad+dy,
							wid/2+dx,lamp_rad);
			ofBezierVertex(wid/2-lamp_rad*.55,lamp_rad,
							wid/2-lamp_rad*.55,dy,
							wid/2,0);
		ofEndShape();

		ofPopStyle();
	}
};

#endif