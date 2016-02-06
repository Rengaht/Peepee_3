#ifndef SCENE_BLOCK_H
#define SCENE_BLOCK_H


#include "MachineWindow.h"
#include "Cloud.h"

class SceneBlock{
public:
	float x,y,wid,hei;
	int scene_type;
	int color_type;
	
	static float scene_speed;
	static ofColor scene_color[4];

	SceneBlock(float x_,float y_,float wid_,float hei_,int color_type_){
		x=x_; y=y_; wid=wid_; hei=hei_;
		// scene_type=scene_type_;
		color_type=color_type_;
		scene_speed=5;
	}

	virtual void draw(bool draw_fill){
		ofPushStyle();
		ofSetColor(scene_color[color_type]);
		//ofNoFill();
		ofRect(x,y,wid,hei);
		ofPopStyle();
	}
	
	virtual void update(){
		x-=scene_speed;	
		// if(x<0)	 x=width;
		// //println(x);
	}

	bool isDead(){
		return x+wid*1.2<0;
	}
	float getXBorder(){
		return x+wid;
	}
	ofColor getLightColor(){
		return scene_color[color_type];
	}
	void drawLight(){
		ofPushStyle();
		ofSetColor(scene_color[color_type],120);
			ofBeginShape();
				ofVertex(x,hei+50);
				ofVertex(x+wid,hei+50);
				ofVertex(x+wid+wid/5,ofGetHeight());
				ofVertex(x-wid/5,ofGetHeight());
			ofEndShape(true);
		ofPopStyle();
	}


};

class CityBlock:public SceneBlock{
public:
	vector<MachineWindow> windows;
	//vector<RoadLamp> lamps;
	vector<CityRoof> roofs;

	int mhoriz;
	int mvert;
	
	ofFbo _fbo;
	
	CityBlock(float x_,float y_,float wid_,float hei_,int color_type_):SceneBlock(x_,y_,wid_,hei_,color_type_){
		mhoriz=floor(ofRandom(3,ofMap(wid_,0,800,4,9)));
		mvert=floor(ofRandom(3,ofMap(wid_,0,800,4,9)));	
		scene_type=1;
		initWindow(0);

		_fbo.allocate(wid,hei,GL_RGBA);
		drawFbo();
	}

	void initWindow(int mode){
		float tmpx=0;
		float tmpy=0;

		int i=0;
		//for(int i=0;i<mhoriz;++i){
		while(tmpx<wid){
			float tmpw=ofRandom(.2,1)*wid/(float)mhoriz;
			//if(tmpx+tmpw>wid) break;

			tmpy=ofRandom(.5)*hei;
			//if(ofRandom(3)<1) lamps.push_back(RoadLamp(tmpx,ofRandom(.5,.7)*hei,hei*.1*ofRandom(.8,1.2),hei*.2*ofRandom(.8,1.2)));
			
			//for(int j=0;j<mvert;++j){
			
			if(ofRandom(3)<1) roofs.push_back(CityRoof(tmpx,tmpy,tmpw*ofRandom(.8,1.5),ofRandom(.1,.3)*hei));

			while(tmpy<hei){
				float tmph=ofRandom(.2,3)*hei/(float)mvert;
				//if(tmpy+tmph>hei) break;
				float tx=ofClamp(tmpx*ofRandom(.9,1.1),0,wid-tmpw);
				windows.push_back(MachineWindow(tx,tmpy*ofRandom(.8,1.2),tmpw*ofRandom(.8,1.2),tmph*ofRandom(.8,1.2)));
				tmpy+=tmph;

				
			}
			tmpx+=tmpw*ofRandom(.8,1.2);

			//if(ofRandom(3)<1) 
		}
	/*	int rfcount=floor(ofRandom(5,10));
		for(int i=0;i<rfcount;++i){
			roofs.push_back(CityRoof(ofRandom(wid),ofRandom(0,.5)*hei,ofRandom(.1,.2)*wid,ofRandom(.1,.2)*hei));
		}*/

	}
	void drawFbo(){
		_fbo.begin();
			ofClear(0);
			//ofBackground(255);
			for(auto& w: windows) w.draw(true);
			
			//for(auto& w: windows) w.draw(false);
		_fbo.end();
	}
	void draw(bool draw_fill){
		SceneBlock::draw(draw_fill);

		float draw_portion=(sin((float)ofGetFrameNum()/(20)));
		ofPushStyle();
		ofPushMatrix();
		ofTranslate(x,y);
		// pg.shearX(PI/8);
		//ofEnableBlendMode(OF_BLENDMODE_SCREEN);
		//ofEnableAlphaBlending();
		/*if(draw_fill)ofSetColor(70,120,255,180);
		else{ ofNoFill(); ofSetColor(0); }*/
		ofSetColor(255);
		_fbo.draw(0,0);
		//ofDisableBlendMode();
		for(auto& roof:roofs) roof.draw(true);
		for(auto& w: windows) w.drawLightWindow();
		//for(auto& w: windows) w.draw(true);
				
		//ofEnableBlendMode(OF_BLENDMODE_ALPHA);
		//for(auto& lamp:lamps) lamp.draw();
		//for(auto& w: windows) w.draw(draw_fill);

		ofPopMatrix();
		ofPopStyle();
	}
	
};

class FieldBlock:public SceneBlock{
	vector<FieldWindow> windows;
	int mhoriz;
	int mvert;
public:
	FieldBlock(float x_,float y_,float wid_,float hei_,int color_type_):SceneBlock(x_,y_,wid_,hei_,color_type_){
		mhoriz=floor(ofRandom(3,6));
		mvert=floor(ofRandom(3,6));	
		scene_type=2;

		
		initWindow(0);
		
	}
	void initWindow(int mode){
		float tmpx=0;
		float tmpy=0;

		int i=0;
		//for(int i=0;i<mhoriz;++i){
		while(tmpx<wid){
			float tmpw=ofRandom(.2,1)*wid/(float)mhoriz;
			//if(tmpx+tmpw>wid) break;

			tmpy=ofRandom(.5)*hei;
			//for(int j=0;j<mvert;++j){
			while(tmpy<hei){
				float tmph=ofRandom(.2,3)*hei/(float)mvert;
				//if(tmpy+tmph>hei) break;
				windows.push_back(FieldWindow(tmpx,tmpy,tmpw*ofRandom(.8,1.2),tmph*ofRandom(.8,1.2)));
				tmpy+=tmph;
			}
			tmpx+=tmpw*ofRandom(.8,1.2);
		}
	}
	
	void draw(bool draw_fill){
		SceneBlock::draw(draw_fill);

		float draw_portion=(sin((float)ofGetFrameNum()/(20)));
		
		ofPushMatrix();
		ofTranslate(x,y);
	
		
		/*if(draw_fill)ofSetColor(70,120,255,180);
		else{ ofNoFill(); ofSetColor(0); }*/
		for(auto& w:windows) w.draw(draw_fill);
		

		ofPopMatrix();
	}
};


class SkyBlock:public SceneBlock{
	vector<Cloud> acloud;
	vector<Eye> aeye;
	ofFbo _fbo;

public:
	SkyBlock(float x_,float y_,float wid_,float hei_,int color_type_):SceneBlock(x_,y_,wid_,hei_,color_type_){
		
		scene_type=0;
		
		int mcloud=(int)ofRandom(6,12);
		for(int i=0;i<mcloud;++i){
			float x=ofRandom(1)*wid;
			acloud.push_back(Cloud(x,ofRandom(.2,.75)*hei,min(ofRandom(.2,.8)*wid,wid*1.1f-x),ofRandom(.3,.6)*hei));
		}

		int meye=(int)ofRandom(3,6);
		for(int i=0;i<meye;++i){
			float ewid=ofRandom(8,25);
			aeye.push_back(Eye(ofRandom(.05,1.5)*wid,ofRandom(.2,.75)*hei,
							 ewid,ewid*ofRandom(.2,.4),ofRandom(50,80),ofRandom(TWO_PI),(int)ofRandom(3,9)));
		}

		_fbo.allocate(wid,hei,GL_RGBA);
		drawFbo();
	
	}
	void drawFbo(){
		_fbo.begin();
			//ofClear(0);
			//ofBackground(255);
			
			for(auto& c:acloud) c.draw(true,0);
			
		_fbo.end();
	}
	void draw(bool draw_fill){	
		SceneBlock::draw(draw_fill);
		
		ofPushMatrix();
		ofTranslate(x,y);
		
		/*ofPushStyle();
		ofSetColor(145,168,209);
			ofRect(0,0,wid,hei);
		ofPopStyle();*/
		_fbo.draw(0,0);
		
		for(auto& c:acloud) c.draw(false,0);
		//for(auto& e:aeye) e.draw(true,0);				
	
		ofPopMatrix();
	}
	
	
};



class IslandBlock:public SceneBlock{
	vector<Island> islands;
	ofColor fcolor;
	float phi;

	ofVec2f corner_1,corner_2;
	ofVec2f ctrl_1,ctrl_2;

	ofVec2f cent1,cent2;
	vector<ofVec2f> corner;
	vector<ofVec2f> corner2;

	vector<ofMesh> mesh_island;
	ofMesh mesh_back;
	ofVec2f vel_1,vel_2;

	vector<Cloud> left_mountain,right_mountain;

	float left_ocean,right_ocean;
	

public:

	bool has_left,has_right;
	float ocean_height;
	IslandBlock(float x_,float y_,float wid_,float hei_,int color_type_,bool left,bool right,float ocean_hei_):SceneBlock(x_,y_,wid_,hei_,color_type_){
		
		setup(left,right,ocean_hei_);
	}
	IslandBlock(float x_,float y_,float wid_,float hei_,int color_type_):SceneBlock(x_,y_,wid_,hei_,color_type_){
			
		setup(true,true,hei*ofRandom(.3,.6));
	}
	void setup(bool left,bool right,float ochei_){
		scene_type=4;
		int ic=(int)ofRandom(5);
		ofColor mcolor=OceanTexture::GetInstance()->_scolor[ic];
		fcolor=ofColor(mcolor.r,mcolor.g+ofRandom(-10,10),mcolor.b+ofRandom(-10,10),180);
		
		phi=ofRandom(TWO_PI);

		vel_1=ofVec2f(ofRandom(20,80),ofRandom(20,80));
		vel_2=ofVec2f(ofRandom(20,80),ofRandom(20,80));

		ocean_height=ochei_;
		has_left=left;
		has_right=right;
		left_ocean=(has_left)?ofRandom(.05,.25)*wid:0;
		right_ocean=(has_right)?ofRandom(.85,.95)*wid:wid;


		if(has_left){
			int c=floor(ofRandom(3,6));
			float ty=ocean_height-hei*.1;
			while(ty<hei){
				float w=ofRandom(.2,.4)*wid;
				float h=ofRandom(.3,.6)*hei;
				float t=ofRandom(.9,1.1);
				
				left_mountain.push_back(Cloud(left_ocean-w*ofRandom(.5,.8),ty,w,h,ofColor(fcolor.b*t,fcolor.b*t,fcolor.b*t,180)));
				ty+=h/2;
			}
		}
		if(has_right){
			int c2=floor(ofRandom(3,6));
			float ty=ocean_height-hei*.1;
			while(ty<hei){
				float w=ofRandom(.2,.4)*wid;
				float h=ofRandom(.3,.6)*hei;
				float t=ofRandom(.9,1.1);
				right_mountain.push_back(Cloud(right_ocean-w*ofRandom(.1,.4),ty,w,h,ofColor(fcolor.b*t,fcolor.b*t,fcolor.b*t,180)));
				ty+=h/2;
			}
		}
		createMesh();
		
	}
	void createMesh(){
		cent1=ofVec2f(ofRandom(.4,.6)*wid,ocean_height+ofRandom(-.1,.2)*hei);
		float W=ofGetWidth();
		float H=ofGetHeight()*.33;

		int icount=floor(ofRandom(1,4));
		for(int i=0;i<icount;++i){
			ofVec2f cent=ofVec2f(ofRandom(left_ocean,right_ocean),ocean_height+ofRandom(0,.2)*hei);
			float radx=ofRandom(.2,.5)*min(cent.x,wid-cent.x);
			float rady=ofRandom(.4,.8)*min(cent.y,hei-cent.y);
		

			int c=floor(ofRandom(4,12));
			float a=0;
		
			ofVec2f r(radx,rady);
			r*=(ofRandom(.9,1.5));
			ofMesh mesh;
			while(a<TWO_PI){
				ofVec2f tr(radx,rady);
				tr*=(ofRandom(.9,1.4));
				//r.rotate(a*RAD_TO_DEG);
				//corner.push_back(ofVec2f(r.x*sin(a),r.y*cos(a)));
				float ta=a+TWO_PI/(float)c;
				//if(ta>TWO_PI) ta=TWO_PI;

				mesh.addVertex(ofPoint(cent.x+r.x*sin(a),cent.y+r.y*cos(a)));
				mesh.addVertex(ofPoint(cent.x+0,cent.y+0));
				mesh.addVertex(ofPoint(cent.x+tr.x*sin(ta),cent.y+tr.y*cos(ta)));
			

				mesh.addTexCoord(ofPoint((cent.x+r.x*sin(a))/W,(cent.y+r.y*cos(a))/H));
				mesh.addTexCoord(ofPoint((cent.x)/W,(cent.y)/H));
				mesh.addTexCoord(ofPoint((cent.x+tr.x*sin(ta))/W,(cent.y+tr.y*cos(ta))/H));

				a=ta;
				r=tr;
				//if(a>TWO_PI) break;
			}
			mesh_island.push_back(mesh);
		}
		
		cent2=ofVec2f(ofRandom(wid,W-wid),hei/2);

		mesh_back.addVertex(ofPoint(-wid/2+left_ocean,-hei/2+ocean_height));
		mesh_back.addVertex(ofPoint(-wid/2+right_ocean,-hei/2+ocean_height));
		mesh_back.addVertex(ofPoint(-wid/2+right_ocean,-hei/2+hei));

		mesh_back.addVertex(ofPoint(-wid/2+right_ocean,-hei/2+hei));
		mesh_back.addVertex(ofPoint(-wid/2+left_ocean,-hei/2+ocean_height));
		mesh_back.addVertex(ofPoint(-wid/2+left_ocean,-hei/2+hei));
		
		mesh_back.addTexCoord(ofPoint((cent2.x-wid/2)/W,(cent2.y-hei/2+ocean_height)/H));
		mesh_back.addTexCoord(ofPoint((cent2.x+wid/2)/W,(cent2.y-hei/2+ocean_height)/H));
		mesh_back.addTexCoord(ofPoint((cent2.x+wid/2)/W,(cent2.y+hei/2)/H));

		mesh_back.addTexCoord(ofPoint((cent2.x+wid/2)/W,(cent2.y+hei/2)/H));
		mesh_back.addTexCoord(ofPoint((cent2.x-wid/2)/W,(cent2.y-hei/2+ocean_height)/H));
		mesh_back.addTexCoord(ofPoint((cent2.x-wid/2)/W,(cent2.y+hei/2)/H));

		
		cent2=ofVec2f(wid/2,hei/2);

		//float misland=floor(ofRandom(1,3));
		//for(int i=0;i<misland;++i){
		//	float tw=ofRandom(.1,.4)*radx;
		//	
		//	ofVec2f r(radx,rady);
		//	r*=(ofRandom(.2,.5));
		//	//r.rotate(ofRandom(360));	
		//	float ang=ofRandom(360);
		//	islands.push_back(Island(r.x*sin(ang),r.y*cos(ang),tw,tw*ofRandom(.2,.4)));
		//}
	}
	void updateMesh(){
		/*vector<ofVec2f> tc=mesh_1.getTexCoords();
		float dx=ofRandom(-1,1)*.01;
		float dy=ofRandom(-1,1)*.002;
		for(int i=0;i<tc.size();++i){
			mesh_1.setTexCoord(i,ofVec2f(tc[i].x+dx,tc[i].y+dy));
		}*/

		vector<ofVec2f> tc2=mesh_back.getTexCoords();
		float dx=ofRandom(-1,1)*.005;
		float dy=ofRandom(-1,1)*.005;
		for(int i=0;i<tc2.size();++i){
			mesh_back.setTexCoord(i,ofVec2f(tc2[i].x+dx,tc2[i].y+dy));
		}
	}
	
	void draw(bool draw_fill){	
		SceneBlock::draw(draw_fill);
		
		updateMesh();

		ofPushMatrix();
		ofTranslate(x,y);
		
		

		float dx=abs(sin(phi))*wid*.1;
		float dy=abs(sin((float)ofGetFrameNum()/220.0+phi))*ocean_height*.2;
		ofPushStyle();
		ofSetColor(fcolor);
		
		ofPushMatrix();
		ofTranslate(cent2.x,cent2.y);
			mesh_back.draw();


			//ofSetColor(180+20*abs(sin((float)ofGetFrameNum()/220.0+phi)));
			OceanTexture::GetInstance()->_fbo.getTextureReference().bind();
			ofSetTextureWrap(GL_REPEAT,GL_REPEAT);
			mesh_back.draw();
			ofPopMatrix();
			

			
			for(auto& m:mesh_island) m.draw();			
			OceanTexture::GetInstance()->_fbo.getTextureReference().unbind();
			
			for(auto& mount:left_mountain) mount.draw(true,0);
			for(auto& mount:right_mountain) mount.draw(true,0);

		ofPopStyle();
		/*FieldTexture::GetInstance()->_fbo.getTextureReference().bind();
		for(auto& island:islands) island.draw(draw_fill);
		FieldTexture::GetInstance()->_fbo.getTextureReference().unbind();*/

		ofPopMatrix();
	}
	


};

class BeachBlock:public SceneBlock{
	ofColor fcolor;
	float phi;

	ofVec2f cent2;

	vector<ofMesh> mesh_island;
	ofMesh mesh_back;
	ofVec2f vel_1,vel_2;

	vector<Cloud> left_mountain,right_mountain;

	float left_ocean,right_ocean;
	

public:

	bool has_left,has_right;
	float ocean_height;
	BeachBlock(float x_,float y_,float wid_,float hei_,int color_type_,bool left,bool right,float ocean_hei_):SceneBlock(x_,y_,wid_,hei_,color_type_){
		
		setup(left,right,ocean_hei_);
	}
	BeachBlock(float x_,float y_,float wid_,float hei_,int color_type_):SceneBlock(x_,y_,wid_,hei_,color_type_){
			
		setup(true,true,hei*ofRandom(.3,.6));
	}
	void setup(bool left,bool right,float ochei_){
		scene_type=3;
		int ic=(int)ofRandom(5);
		ofColor mcolor=LeafTexture::GetInstance()->_scolor[ic];
		fcolor=ofColor(mcolor.r,mcolor.g+ofRandom(-10,10),mcolor.b+ofRandom(-10,10),180);
		
		phi=ofRandom(TWO_PI);

		vel_1=ofVec2f(ofRandom(20,80),ofRandom(20,80));
		vel_2=ofVec2f(ofRandom(20,80),ofRandom(20,80));

		ocean_height=ochei_;
		has_left=left;
		has_right=right;
		left_ocean=(has_left)?ofRandom(.05,.25)*wid:0;
		right_ocean=(has_right)?ofRandom(.75,.95)*wid:wid;


		if(has_left){
			int c=floor(ofRandom(3,6));
			float ty=ocean_height-hei*.1;
			while(ty<hei){
				float w=ofRandom(.2,.4)*wid;
				float h=ofRandom(.3,.6)*hei;
				float t=ofRandom(.9,1.1);
				
				left_mountain.push_back(Cloud(left_ocean-w*ofRandom(.3,.6),ty,w,h,ofColor(fcolor.r*t,fcolor.r*t,fcolor.r*t,180)));
				ty+=h/2;
			}
		}
		if(has_right){
			int c2=floor(ofRandom(3,6));
			float ty=ocean_height-hei*.1;
			while(ty<hei){
				float w=ofRandom(.2,.4)*wid;
				float h=ofRandom(.3,.6)*hei;
				float t=ofRandom(.9,1.1);
				right_mountain.push_back(Cloud(right_ocean-w*ofRandom(.5,.8),ty,w,h,ofColor(fcolor.r*t,fcolor.r*t,fcolor.r*t,180)));
				ty+=h/2;
			}
		}
		createMesh();
		
	}
	void createMesh(){
		float W=ofGetWidth();
		float H=ofGetHeight()*.33;

		int icount=max(ofRandom(1,5),floor(wid/200));
		float margin=(right_ocean-left_ocean)/(float)(icount+1);
		for(int i=0;i<icount;++i){
			ofVec2f cent=ofVec2f(margin*(i+1+ofRandom(-.5,.5)),ofRandom(.4,.7)*hei);
			float radx=ofRandom(.6,.9)*margin*.5;
			float rady=ofRandom(.2,.5)*radx;
		

			int c=floor(ofRandom(5,12));
			float a=0;
		
			ofVec2f r(radx,rady);
			r*=(ofRandom(.9,1.5));
			ofMesh mesh;
			while(a<TWO_PI){
				ofVec2f tr(radx,rady);
				tr*=(ofRandom(.9,1.4));
				//r.rotate(a*RAD_TO_DEG);
				//corner.push_back(ofVec2f(r.x*sin(a),r.y*cos(a)));
				float ta=a+TWO_PI/(float)c;
				if(ta>TWO_PI) ta=TWO_PI;

				mesh.addVertex(ofPoint(cent.x+r.x*sin(a),cent.y+r.y*cos(a)));
				mesh.addVertex(ofPoint(cent.x+0,cent.y+0));
				mesh.addVertex(ofPoint(cent.x+tr.x*sin(ta),cent.y+tr.y*cos(ta)));
			

				mesh.addTexCoord(ofPoint((cent.x+r.x*sin(a))/W,(cent.y+r.y*cos(a))/H));
				mesh.addTexCoord(ofPoint((cent.x)/W,(cent.y)/H));
				mesh.addTexCoord(ofPoint((cent.x+tr.x*sin(ta))/W,(cent.y+tr.y*cos(ta))/H));

				a=ta;
				r=tr;
				//if(a>TWO_PI) break;
			}
			
			mesh_island.push_back(mesh);
		}
		
		cent2=ofVec2f(ofRandom(wid,W-wid),hei/2);

		mesh_back.addVertex(ofPoint(-wid/2+left_ocean,-hei/2+ocean_height));
		mesh_back.addVertex(ofPoint(-wid/2+right_ocean,-hei/2+ocean_height));
		mesh_back.addVertex(ofPoint(-wid/2+right_ocean,-hei/2+hei));

		mesh_back.addVertex(ofPoint(-wid/2+right_ocean,-hei/2+hei));
		mesh_back.addVertex(ofPoint(-wid/2+left_ocean,-hei/2+ocean_height));
		mesh_back.addVertex(ofPoint(-wid/2+left_ocean,-hei/2+hei));
		
		mesh_back.addTexCoord(ofPoint((cent2.x-wid/2)/W,(cent2.y-hei/2+ocean_height)/H));
		mesh_back.addTexCoord(ofPoint((cent2.x+wid/2)/W,(cent2.y-hei/2+ocean_height)/H));
		mesh_back.addTexCoord(ofPoint((cent2.x+wid/2)/W,(cent2.y+hei/2)/H));

		mesh_back.addTexCoord(ofPoint((cent2.x+wid/2)/W,(cent2.y+hei/2)/H));
		mesh_back.addTexCoord(ofPoint((cent2.x-wid/2)/W,(cent2.y-hei/2+ocean_height)/H));
		mesh_back.addTexCoord(ofPoint((cent2.x-wid/2)/W,(cent2.y+hei/2)/H));

		
		cent2=ofVec2f(wid/2,hei/2);

	}
	void updateMesh(){
		/*vector<ofVec2f> tc=mesh_1.getTexCoords();
		float dx=ofRandom(-1,1)*.01;
		float dy=ofRandom(-1,1)*.002;
		for(int i=0;i<tc.size();++i){
			mesh_1.setTexCoord(i,ofVec2f(tc[i].x+dx,tc[i].y+dy));
		}*/
		for(auto& island:mesh_island){
			vector<ofVec2f> tc2=island.getTexCoords();
			float dx=ofRandom(-1,1)*.02;
			float dy=ofRandom(-1,1)*.01;
			for(int i=0;i<tc2.size();++i){
				island.setTexCoord(i,ofVec2f(tc2[i].x+dx,tc2[i].y+dy));
			}
		}
	}
	
	void draw(bool draw_fill){	
		SceneBlock::draw(draw_fill);
		
		updateMesh();

		ofPushMatrix();
		ofTranslate(x,y);
		
		

		float dx=abs(sin(phi))*wid*.1;
		float dy=abs(sin((float)ofGetFrameNum()/220.0+phi))*ocean_height*.2;
		ofPushStyle();
		ofSetColor(fcolor);
		
		ofPushMatrix();
		ofTranslate(cent2.x,cent2.y);
			//mesh_back.draw();


			ofSetColor(fcolor.r,fcolor.g+10*dy,fcolor.b+20*dy,fcolor.a);
			LeafTexture::GetInstance()->_fbo.getTextureReference().bind();
			ofSetTextureWrap(GL_REPEAT,GL_REPEAT);
			//mesh_back.draw();
			ofPopMatrix();
			

			
			for(auto& m:mesh_island) m.draw();			
			LeafTexture::GetInstance()->_fbo.getTextureReference().unbind();
			
			for(auto& mount:left_mountain) mount.draw(true,0);
			for(auto& mount:right_mountain) mount.draw(true,0);

		ofPopStyle();
		/*FieldTexture::GetInstance()->_fbo.getTextureReference().bind();
		for(auto& island:islands) island.draw(draw_fill);
		FieldTexture::GetInstance()->_fbo.getTextureReference().unbind();*/

		ofPopMatrix();
	}
	
};



class TreeBlock:public SceneBlock{
	vector<FieldWindow> windows;
	
	
public:
	TreeBlock(float x_,float y_,float wid_,float hei_,int color_type_):SceneBlock(x_,y_,wid_,hei_,color_type_){
		scene_type=5;
		initWindow(0);
		//scene_speed*=4;
	}
	void initWindow(int mode){
		float tmpx=0;
		float tmpy=0;

		int c=floor(ofRandom(2,5));
		for(int i=0;i<c;++i){
			float tx=ofRandom(0,.2)*wid;
			windows.push_back(FieldWindow(tx,ofRandom(-.1,.1)*hei,(wid-tx)*ofRandom(1,1.2),hei*ofRandom(1,1.2),false));			
		}
	}
	
	void draw(bool draw_fill){
		SceneBlock::draw(draw_fill);

		float draw_portion=(sin((float)ofGetFrameNum()/(20)));
		
		ofPushMatrix();
		ofTranslate(x,y);
	
		
		/*if(draw_fill)ofSetColor(70,120,255,180);
		else{ ofNoFill(); ofSetColor(0); }*/
		for(auto& w:windows) w.draw(draw_fill);
		

		ofPopMatrix();
	}
	void update(){
		x-=scene_speed*3;		
	}
};

class SignBlock:public SceneBlock{
	//vector<RoadLamp> lamps;
	int sign_num;
	float dx;
	float phi;
public:
	SignBlock(float x_,float y_,float wid_,float hei_,int color_type_,int num_):SceneBlock(x_,y_,wid_,hei_,color_type_){
		scene_type=6;
		//scene_speed*=4;

		sign_num=num_;
		dx=20.0*ofRandom(-.1,.1);

		phi=ofRandom(-PI/12,PI/12);
		/*int c=floor(ofRandom(8,20));
		for(int i=0;i<c;++i){
			lamps.push_back(RoadLamp(wid/(float)c*i,.5*hei,hei*.2,hei));
		}*/
	}
	
	
	void draw(bool draw_fill){
		//SceneBlock::draw(draw_fill);

		//float draw_portion=(sin((float)ofGetFrameNum()/(20)));
		
		ofPushMatrix();
		ofTranslate(x,y);
		
		ofTranslate(0,hei/2);
		ofRotate(phi*RAD_TO_DEG);
		//ofTranslate(0,-hei/2);
		
		ofPushStyle();
		ofSetColor(200+dx,180);
		ofBeginShape();
			ofVertex(wid/2-10+dx,-hei/cos(phi));
			ofVertex(wid/2-10,hei/cos(phi));
			ofVertex(wid/2+10+dx,hei/cos(phi));
			ofVertex(wid/2+10+dx,-hei/cos(phi));
		ofEndShape(true);
		
		ofSetColor(51,75,115);
		ofSetDrawBitmapMode(OF_BITMAPMODE_MODEL);
		
		ofPushMatrix();
		ofTranslate(wid/2+5,0);
		ofRotate(270);			
		ofScale(.75,.75);
			ofDrawBitmapString(ofToString(sign_num),0,0);
		ofPopMatrix();

		/*if(draw_fill)ofSetColor(70,120,255,180);
		else{ ofNoFill(); ofSetColor(0); }*/
		ofPopStyle();

		ofPopMatrix();
	}
	void update(){
		x-=scene_speed*3;		
	}
};


#endif