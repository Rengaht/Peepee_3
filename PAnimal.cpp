#include "PAnimal.h"

PAnimal::PAnimal(float x_,float y_,float wid_,float hei_){
		x=x_; y=y_; wid=wid_; hei=hei_;
		

		float hhead_portion=ofRandom(.3,.5);
		head_wid=hhead_portion*wid;
		body_wid=wid-head_wid;
		
		body_hei=(hei)*ofRandom(.3,.5);
		float vhead_portion=ofRandom(.4,.8);
		head_hei=vhead_portion*body_hei;
		
		leg_hei=hei-body_hei;

		mleg=(int)ofRandom(3,6);
		leg_wid=body_wid/mleg*ofRandom(.2,.4);
		
		for(int i=0;i<mleg;++i){
			leg_span.push_back(ofRandom(0.05,0.5));
			fang.push_back(0);
			kang.push_back(0);
			fang_dest.push_back(0);
			kang_dest.push_back(0);
			leg_direction.push_back(ofRandom(2)<1?1:0);
		}


		fcolor=ofColor(ofRandom(100,255),ofRandom(20,80)+100,ofRandom(20,85));
		
		phi=ofRandom(HALF_PI);
		
		
		_cur_mode=0;
		_dest_mode=0;
		elastic_stage=0;
		
		wheel_vel=27;
		skate_vel=60;
		run_vel=50;

		v_jump_vel=ofRandom(10,24);
		s_body_hei_incre=0;

		_src_ang=0;
		_dest_ang=0;

		
		
		land_vel=ofRandom(4,6);
		_move_vel=ofVec2f(land_vel/10.0,0);


		elastic_strength=(float)land_vel/10*ofRandom(1,3);


		head_top=0;
		head_ang=PI/8;
		ear_ang=PI/3;
		/*ear_ang=-PI/4;
		head_ang=-PI/8;*/

		_anim_mode_change=FrameAnimation(ofRandom(20,40),ofRandom(0,10));
		_anim_walk=FrameAnimation(ofRandom(10,40),ofRandom(10));
		_anim_walk.is_loop=true;
		//_anim_walk.do_reverse=true;
		_anim_walk.Restart();


		_anim_fly_leg=FrameAnimation(ofRandom(40,60));
		_anim_fly_leg.is_loop=true;


		_calib_vel=ofVec2f(0,0);

		
		
		head_heli=Helicopt(wid/8,wid/8*ofRandom(.2,.5));
		tail_heli=Helicopt(wid/8,wid/8*ofRandom(.2,.5));

		shear_ang=ofVec2f(ofRandom(-PI/6,PI/6),ofRandom(-PI/6,PI/12));
	}	

void PAnimal::draw(bool draw_fill){
		
		
		ofPushStyle();
		
		if(draw_fill){
		 	
		 	ofSetColor(fcolor);
		 	
		}else{
			ofNoFill();
			ofSetColor(ofColor::black);			
		}
		
		float draw_portion=1.5*abs(sin((float)ofGetFrameNum()/(120/land_vel)));



		float elastic_portion=.6+elastic_strength*abs(sin((float)ofGetFrameNum()/(180/land_vel)+phi));
		
		int new_elastic_stage=(int)(fmod((float)ofGetFrameNum()/(180/land_vel)+phi,PI)/(PI/2));

		float cur_body_wid=body_wid;


		
		int jump_stage=0;//sship.stage;//(int)((jump_portion)/(PI/2));
		float jump_pos_y=0;
		float jump_pos_x=0;
		
		s_body_hei_incre=0;

		float cur_head_pos=(_cur_mode==2)?head_heli.delta_hei:0;
		float cur_tail_pos=(_cur_mode==2)?tail_heli.delta_hei:0;

		

		ofPushMatrix();

		ofTranslate(x,y);
		
		//TODO: shear!!!
		ofMatrix4x4 mat; 
		mat.set(1,tan(shear_ang.x),0,0,
				tan(shear_ang.y),1,0,0,
				0,0,1,0,
				0,0,0,1);
		ofMultMatrix(mat);
		

		
		ofBeginShape();
		ofVertex(0,0+head_top+cur_tail_pos);
		
		
		ofBezierVertex(cur_body_wid/3,head_top-body_hei*.3*draw_portion+ofLerp(cur_tail_pos,cur_head_pos,.33),
						 cur_body_wid/3*2,head_top-body_hei*.3*draw_portion+ofLerp(cur_tail_pos,cur_head_pos,.66),
						 cur_body_wid,head_top+cur_head_pos);


		ofVec2f ear_base1(head_hei,0);
		ear_base1.rotate(((-PI/6)*draw_portion-PI/6+ear_ang)*RAD_TO_DEG);
		ofVec2f ear_base2(head_hei,0);
		ear_base2.rotate((-PI/6*draw_portion+ear_ang)*RAD_TO_DEG);
		
		ofBezierVertex(cur_body_wid+ear_base1.x,ear_base1.y+cur_head_pos,
					  cur_body_wid+ear_base2.x,ear_base2.y+cur_head_pos,
					  cur_body_wid,head_top+cur_head_pos);
		
		ofVec2f h1(head_wid*.5,0);
		h1.rotate(head_ang*RAD_TO_DEG);

		ofVec2f h2(head_wid,body_hei);
		h2.rotate(head_ang*RAD_TO_DEG);

		ofBezierVertex(cur_body_wid+h1.x,-body_hei*.1*draw_portion+h1.y+cur_head_pos,
					 cur_body_wid+h2.x,body_hei*.4*draw_portion+h2.y+cur_head_pos,
					 cur_body_wid,head_top+head_hei*.7+cur_head_pos);
		// vertex(cur_body_wid,body_hei);
		// bezierVertex(cur_body_wid*.95,body_hei,cur_body_wid,body_hei,
		// 			
	
		ofVertex(cur_body_wid*(1-(float)(0+leg_span[0])/mleg),body_hei+cur_head_pos);

		for(int i=0;i<mleg;++i){
			
			float cur_tmp_pos=ofLerp(cur_head_pos,cur_tail_pos,((float)(i+leg_span[i])/mleg));
			
			if(i>0) ofVertex(cur_body_wid*(1-(float)(i+leg_span[i])/mleg),body_hei+cur_tmp_pos);

			if(_cur_mode==_dest_mode){
				if(_cur_mode==0) drawSleepLeg(cur_body_wid*(1-(float)(i+leg_span[i])/mleg),body_hei,leg_wid,leg_hei,i);
				else if(_cur_mode==1) drawWalkLeg(cur_body_wid*(1-(float)(i+leg_span[i])/mleg),body_hei,leg_wid,leg_hei,i);
				else if(_cur_mode==2) drawFlyLeg(cur_body_wid*(1-(float)(i+leg_span[i])/mleg),body_hei+cur_tmp_pos,leg_wid,leg_hei,i);
			}else{
				if(_dest_mode==2) drawWalkLeg(cur_body_wid*(1-(float)(i+leg_span[i])/mleg),body_hei,leg_wid,leg_hei,i);
				else if(_dest_mode==0) drawSleepLeg(cur_body_wid*(1-(float)(i+leg_span[i])/mleg),body_hei,leg_wid,leg_hei,i);
				else if(_dest_mode==1) drawWalkLeg(cur_body_wid*(1-(float)(i+leg_span[i])/mleg),body_hei,leg_wid,leg_hei,i);
				
			}
			drawLegs(kang[i],fang[i],cur_body_wid*(1-(float)(i+leg_span[i])/mleg),body_hei+(_cur_mode==0?leg_hei*.1:0)+cur_tmp_pos,leg_wid,leg_hei,i);
		}
		
		ofBezierVertex(-cur_body_wid*.1*draw_portion,body_hei+cur_tail_pos,
					 -cur_body_wid*.1*draw_portion,body_hei-head_top+cur_tail_pos,
					 0,head_top+cur_tail_pos);


		ofEndShape();
		if(!draw_fill){
			ofSetColor((fcolor.r)/1.1,(fcolor.g)/1.1,(fcolor.b)/1.3);
			ofNoFill();
			ofBeginShape();
			for(int i=0;i<50;++i){
					// pg.strokeWeight(random(2));
					float n=ofNoise((i+ofGetFrameNum()/10+land_vel));
					float n2=ofNoise(i*land_vel);

					float cur_tmp_pos=ofLerp(cur_tail_pos,cur_head_pos,n);

					ofVertex(n*cur_body_wid,
							   	cur_tmp_pos+ofLerp(0,.5,n2)*body_hei);
			
			}
			ofEndShape();
		}

		if(_dest_mode!=_cur_mode){
			float t=_anim_mode_change.GetPortion();
			switch(_cur_mode){
				
			}
		}else{
			switch(_cur_mode){
				case 2:
					head_heli.draw(draw_fill,cur_body_wid,cur_head_pos);
					tail_heli.draw(draw_fill,0,cur_tail_pos);
					break;
			}
		}

		ofDrawBitmapString(ofToString(_cur_mode)+" "+ofToString(_dest_mode),0,0);

		ofPopMatrix();
		

		ofPopStyle();

		
		//update();
	}

void PAnimal::update(float ctrl_){

	_anim_mode_change.Update();
	_anim_fly_leg.Update();
	_anim_walk.Update();

	if(_dest_mode!=_cur_mode){
		float t=_anim_mode_change.GetPortion();
		if(t==1) _cur_mode=_dest_mode;		
		//shear_ang=ofVec2f.ofLerp(shear_dest,shear_ang,t);

		shear_ang.x+=(shear_dest.x-shear_ang.x)*t;
		shear_ang.y+=(shear_dest.y-shear_ang.y)*t;

		/*x+=(walk_dest.x-x)*t;
		y+=(walk_dest.y-y)*t;
		mode_scale+=(dest_scale-mode_scale)*t;*/

		for(int i=0;i<mleg;++i){
			kang[i]+=(kang_dest[i]-kang[i])*t;
			fang[i]+=(fang_dest[i]-fang[i])*t;
		}
			
		switch(_dest_mode){
			case 0:
			case 2:
				ear_ang+=(PI/3-ear_ang)*t;
				head_ang+=(PI/8-head_ang)*t;
				head_top+=(body_hei*.3-head_top)*t;

				break;
			case 1:
				ear_ang+=(-PI/4-ear_ang)*t;
				head_ang+=(-PI/8-head_ang)*t;
				head_top+=(body_hei*.3-head_top)*t;

				break;
		}			
	}else{
		for(int i=0;i<mleg;++i){
			kang[i]=(kang_dest[i]);
			fang[i]=(fang_dest[i]);
		}
		switch(_cur_mode){
			case 2:
				y+=land_vel/2*cos((_anim_walk.GetPortion()+_anim_walk._loop_count)*PI/4);
				break;
		}
		
	}

}
void PAnimal::checkSpeed(float ctrl_,ofVec2f mid_){
		
		_calib_vel.scale(0);

		ofVec2f c(mid_.x-x,mid_.y-y);
		ofVec2f dir(land_vel,0);
		if(_cur_mode==1) dir.rotate(PI/4*RAD_TO_DEG);
		else if(_cur_mode==2) dir.rotate(-PI/8*RAD_TO_DEG);

		float sp_=ofMap(ctrl_,0,1024,5,.5);
		//dir.setMag(constrain(c.mag(),0,1.5));
		dir.scale(sp_);

		float dot=c.dot(dir);
		
		if(dot>ofGetWidth()/4) _calib_vel+=dir;
		if(dot<-ofGetWidth()/4) _calib_vel-=dir;
		 

		// if(_cur_mode!=0) c.setMag(constrain(c.mag(),0,1));
		// else c.setMag(0);
		
		//_calib_vel=c.get();
		
	}
void PAnimal::checkCollide(float ctrl_,vector<ofVec2f> pos_){
		
		for(ofVec2f p:pos_){
			//println(dist(p.x,p.y,x,y));
			if(ofDist(p.x,p.y,x,y)<60){
				ofVec2f dir(x-p.x,y-p.y);
				dir.scale(ofMap(ctrl_,0,1024,0.5,5));
				_calib_vel+=dir;
			}
		}

	}

void PAnimal::changeMode(int mode_){
		_move_vel=ofVec2f(land_vel,0);

		_src_ang=ofLerp(_src_ang,_dest_ang,_anim_mode_change.GetPortion());
		_anim_mode_change.Restart();

		switch(mode_){
			case 1:
				shear_dest=ofVec2f(0,0);
				_dest_ang=0;
				break;
			case 0:
				shear_dest=ofVec2f(ofRandom(-PI/6,PI/6),ofRandom(-PI/6,PI/12));
				_move_vel.rotate(PI/4*RAD_TO_DEG);
				_dest_ang=PI/4;
				break;
			case 2:
				shear_dest=ofVec2f(0,0);
				_move_vel.rotate(-PI/8*RAD_TO_DEG);
				_dest_ang=-PI/8;
				break;
		}

		_dest_mode=mode_;

	}


void PAnimal::drawLegs(float kang,float fang,float lx,float ly,float lw,float lh,int leg_index){	
	ofVec2f knee_base(0,lh/2);
	knee_base.rotate(kang*RAD_TO_DEG);

	ofVec2f foot_base(0,lh/2);
	foot_base.rotate(fang*RAD_TO_DEG);
		

	ofVec2f foot_base2(-lw,lh/2);
	foot_base2.rotate(fang*RAD_TO_DEG);
		
		

	foot_base+=knee_base;
	foot_base2+=knee_base;
		
	ofBezierVertex(lx+knee_base.x,ly+knee_base.y,
					lx+foot_base.x,ly+foot_base.y,
					lx+foot_base.x,ly+foot_base.y);
	// vertex(lx+knee_base.x,ly+knee_base.y);
	// vertex(lx+foot_base.x,ly+foot_base.y);
		
	ofVertex(lx+foot_base2.x,ly+foot_base2.y);

	// vertex(lx-lw+knee_base.x,ly+knee_base.y);
	// vertex(lx-lw,ly);

	ofBezierVertex(lx-lw+knee_base.x,ly+knee_base.y,
					lx-lw+knee_base.x,ly+knee_base.y,
					lx-lw,ly);

	switch(_cur_mode){
		case 1:
			if(leg_index==0) first_foot_base=ofVec2f(lx-lw+foot_base.x,ly+foot_base.y);
			else if(leg_index==mleg-1) last_foot_base=ofVec2f(lx+lw+foot_base.x,ly+foot_base.y);
			break;
		case 2:
			if(leg_index==0) first_foot_base=ofVec2f(lx-lw+foot_base.x,ly+lh);
			else if(leg_index==mleg-1) last_foot_base=ofVec2f(lx+lw+foot_base.x,ly+lh);
			break;
		
	}
}

void PAnimal::drawSleepLeg(float lx,float ly,float lw,float lh,int leg_index){


	float frame_portion=(float)ofGetFrameNum()/(80/land_vel)+phi;
	int stage=(int)(fmod(frame_portion,TWO_PI)/(PI/2));
		
	kang_dest[leg_index]=-PI/2.5+PI*.05*sin(frame_portion);
	fang_dest[leg_index]=kang_dest[leg_index]+PI*.1*abs(sin(frame_portion));

	if(ofNoise(leg_index*land_vel)<0.5||leg_index==0){
		kang_dest[leg_index]=-PI/2.7;
		fang_dest[leg_index]=kang_dest[leg_index]+PI*(.8+.01*abs(sin(frame_portion)));
			 	
	}
	// kang*=_anim_mode_change.GetPortion();
	// fang*=_anim_mode_change.GetPortion();
		
	// drawLegs(kang,fang,lx,ly+lh*.1,lw,lh,leg_index,pg);
			
}
void PAnimal::drawWalkLeg(float lx,float ly,float lw,float lh,int leg_index){

	float fp=_anim_walk.GetPortion();
	int stage=(_anim_walk._loop_count+leg_index%2)%2;
	// if(stage%2==1) fp=1-fp;
	//if(leg_index%2==0) fp=(1-fp);

	switch(stage){
		case 0:
			kang_dest[leg_index]=PI/3-PI/3*2*fp;
			fang_dest[leg_index]=kang_dest[leg_index]+PI/4;
			break;						
		case 1:
			kang_dest[leg_index]=-PI/3+PI/3*2*fp;
			fang_dest[leg_index]=kang_dest[leg_index]+PI/4*(1-sin(fp*PI));
			break;
		case 2:
			kang_dest[leg_index]=-PI/3;
			fang_dest[leg_index]=kang_dest[leg_index];
			break;
		case 3:
			kang_dest[leg_index]=-PI/3*fp;
			fang_dest[leg_index]=kang_dest[leg_index];				
			break;
	}
		

	// drawLegs(kang,fang,lx,ly+lh*.1,lw,lh,leg_index,pg);	
}
void PAnimal::drawFlyLeg(float lx,float ly,float lw,float lh,int leg_index){

	float frame_portion=_anim_fly_leg.GetPortion();
	int stage=_anim_fly_leg._loop_count%4;
		
	kang_dest[leg_index]=-PI/4*(sin(frame_portion*HALF_PI+HALF_PI*stage));
		
	if(leg_direction[leg_index]==1) kang_dest[leg_index]=-PI/4*(sin(frame_portion*HALF_PI+HALF_PI*stage+HALF_PI*sin(leg_index)));
		
	switch(stage){
		case 0:
			fang_dest[leg_index]=kang_dest[leg_index]+PI/3;//*(sin(frame_portion));
			break;
		case 1:
			fang_dest[leg_index]=kang_dest[leg_index]+PI/3*(1-frame_portion);
			break;
		case 2:
			fang_dest[leg_index]=kang_dest[leg_index];
			break;
			
		case 3:
			fang_dest[leg_index]=kang_dest[leg_index]+PI/3*(frame_portion);
			break;
				
	}

}



bool PAnimal::operator < (const PAnimal& pa) const{
	return y<pa.y;
}
