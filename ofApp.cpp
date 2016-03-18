#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	
	ofHideCursor();

	/*for(float i=0;i<1;i+=.01){
		cout<<ofNoise(i)<<endl;
	}*/

	_distort_fill=DistortGraph(20);
	_distort_fill.transform_mag=2.2;

	ofDisableArbTex();

	_fbo_fill.allocate(ofGetWidth()*_distort_fill.getWeight(),ofGetHeight()*_distort_fill.getWeight(),GL_RGBA);
	_fbo_stroke.allocate(ofGetWidth(),ofGetHeight(),GL_RGBA);

	
	_fbo_fill.begin();
	ofClear(255,255,255,0);
	_fbo_fill.end();

	_fbo_stroke.begin();
	ofClear(255,255,255,0);
	_fbo_stroke.end();

	mid_pos=ofVec2f(ofGetWidth()/2,ofGetHeight()/2);

	M_CONTROL=3;
	M_MODE=2;
	
	mpas=3;
	play_mode=0;
	dest_pmode=0;

	SceneColor[0]=SceneColor[1]=ofColor(255);
	SceneColor[1]=ofColor(97,112,119);
	SceneColor[2]=ofColor(21,28,53);
	SceneColor[3]=ofColor(91,6,43);

	save_frame=false;

	_anim_mode_change=FrameAnimation(50);
	_anim_color_change=FrameAnimation(120);
    _anim_color_change.Restart(1);
    
	color_stage=0;
	ocean_type=false;
	road_sign_num=0;
	
	initControl();
	initSound();

	initTrain();
	initPAnimal();
	initScene();

	changeMode(0);

	ofEnableSmoothing();
	ofSetTextureWrap(GL_REPEAT,GL_REPEAT);

	//ofSetCurveResolution(120);
	ofBackground(255);

	
	

}
void ofApp::initPAnimal(){
	
	pas.clear();

	float tmp_pos=0;
	for(int i=0;i<mpas;++i){
		float tmp_h=ofRandom(0.6,1.2)*64;
		pas.push_back(PAnimal(ofRandom(.1,.9)*ofGetWidth(),ofRandom(.7,.85)*ofGetHeight(),96*ofRandom(.6,1.4)*.7,tmp_h));
		tmp_pos+=tmp_h;
		if(tmp_h>ofGetHeight()) return;
	}
	mpas=pas.size();

}

void ofApp::initTrain(){

	float width=ofGetWidth();
	float height=ofGetHeight();

	windows.clear();	
	for(int i=0;i<8;++i){
		windows.push_back(Window(width/4*i,ofRandom(.3,.35)*height,ofRandom(.15,.28)*width,ofRandom(.15,.25)*height));
	}


	chairs.clear();
	for(int i=0;i<13;++i){
		chairs.push_back(Chair(width*1.8/13*i*ofRandom(.8,1.2),height*.5,width/24,height/6.4));
	}

	handles.clear();
	for(int i=0;i<13;++i){
		handles.push_back(THandle(width*1.8/13*i*ofRandom(.8,1.2),height*.25,width/32,height/12.8));
	}

}
		
		
void ofApp::initControl(){
	
	_control_val[0]=0;
	_control_val[1]=0;
	_control_val[2]=512;
	

//	_serial_ctrl.listDevices();
//
//#ifdef TARGET_WIN32
//	_serial_ctrl.setup(1,9600);
//#endif
//#if defined( TARGET_LINUX )
//		_serial_ctrl.setup("/dev/ttyACM0",9600);
//#endif

}

void ofApp::initSound(){

	_sound_bgm[0].loadSound("sound/pipi_train_bright.ogg");
	//_sound_bgm[1].loadSound("sound/pipi_train_bright2.wav");
	_sound_bgm[2].loadSound("sound/pipi_train_dark.ogg");
	//_sound_bgm[3].loadSound("sound/pipi_train_dark3.wav");

	sound_index=0;//(int)ofRandom(2);
	_sound_vol=1.0;
	_sound_bgm[sound_index].play();
	
}

//--------------------------------------------------------------
void ofApp::update(){

	ofEnableAlphaBlending();
	//ofDisableAlphaBlending();
	
	road_sign_num+=SceneBlock::scene_speed;
	int fr=ofGetFrameNum();
	if(fr%OCEAN_LENGTH==OCEAN_LENGTH-1){
		ocean_type=!ocean_type;
		cout<<((ocean_type)?"OCEAN":"MOUNT")<<endl;
	}

	float sound_pos=_sound_bgm[sound_index].getPosition();
	
	if(sound_pos>=.5){
		if(color_stage%2==0){
			color_stage=(color_stage+1)%4;
			cout<<"Stage: "<<color_stage<<endl;
            _anim_color_change.Restart();
		}	
		if(sound_pos>=.99){		
			color_stage=(color_stage+1)%4;
			cout<<"Stage: "<<color_stage<<endl;
            _anim_color_change.Restart();

			_sound_bgm[sound_index].stop();
			if(color_stage==2){				
				sound_index=2;//(int)ofRandom(2,4);			
			}else if(color_stage==0){
				sound_index=0;//(int)ofRandom(0,2);
			}
			_sound_bgm[sound_index].play();
			_sound_bgm[sound_index].setVolume(_sound_vol);

			mode_start_time=ofGetElapsedTimeMillis();
		}
	}


	for(auto& pa:pas) pa.update(0);
	_anim_mode_change.Update();
	if(_anim_mode_change.GetPortion()==1) play_mode=dest_pmode;

    _anim_color_change.Update();
    
	for(auto& sb:blocks) sb->update();
	auto it=blocks.begin();
	while(it!=blocks.end()){
		if(((SceneBlock*)*it)->isDead()){
			delete *it;
			it=blocks.erase(it);
		}else ++it;
	}
	for(auto& sb:front_blocks) sb->update();
	auto fit=front_blocks.begin();
	while(fit!=front_blocks.end()){
		if(((SceneBlock*)*fit)->isDead()){
			delete *fit;
			fit=front_blocks.erase(fit);			
		}else ++fit;
	}
	addNewSceneBlock();

	if(play_mode%2==1){
		float speed=2;
		for(auto& window:windows) window.update(speed);
		auto it1=windows.begin();
		while(it1!=windows.end()){
			if(((TrainBase)*it1).isDead()){
				it1=windows.erase(it1);				
			}else ++it1;
		}

		for(auto& chair:chairs) chair.update(speed);
		auto it2=chairs.begin();
		while(it2!=chairs.end()){
			if(((TrainBase)*it2).isDead()){
				it2=chairs.erase(it2);				
			}else ++it2;
		}

		for(auto& hand:handles) hand.update(speed);
		auto it3=handles.begin();
		while(it3!=handles.end()){
			if(((TrainBase)*it3).isDead()){
				it3=handles.erase(it3);				
			}else ++it3;
		}
		addNewTrainScene();
	}
	

	//updateMidPos();
	_distort_fill.update();





	////---- handle serial----
	//if(_serial_ctrl.isInitialized() && _serial_ctrl.available()){
	//	unsigned char bytesReturned[3];
	//	unsigned char bytesReadString[24];
	//	int nRead=0;
	//	int nTimesRead=0;
	//	int nBytesRead=0;

	//	memset(bytesReadString, 0, 24);
	//	memset(bytesReturned, 0, 3);
	//	
	//	while((nRead=_serial_ctrl.readBytes(bytesReturned,3))>0){
	//		unsigned char *endchar=(unsigned char*)memchr(bytesReturned,'!',3);
	//		if(endchar!=NULL){
	//			memcpy(bytesReadString+nBytesRead,bytesReturned,endchar-bytesReturned);
	//			break;
	//		}
	//		memcpy(bytesReadString+nBytesRead,bytesReturned,nRead);
	//		nBytesRead+=nRead;		
	//	};	
	//	//cout<<ofToString(bytesReadString)<<endl;
	//	if(nBytesRead>0){
	//		string read_string=ofToString(bytesReadString);
	//		/*if(read_string.find_last_of('!')==string::npos){
	//			return;
	//		}*/

	//		vector<string> val=split(read_string,'|');
	//		//cout<<ofToString(val)<<endl;
	//		if(val.size()==M_CONTROL){
	//			for(int i=0;i<val.size();++i){
	//				int n=ofToInt(val[i]);
	//				switch(i){						
	//					case 0:
	//						if(n!=_control_val[i]) triggerEvent(0);
	//						_control_val[i]=n;
	//						break;
	//					case 1:
	//						if(n==0) triggerEvent(1);
	//						_control_val[i]=n;
	//						break;
	//					case 2:
	//						if(abs(_control_val[2]-n)<=100){
	//							_sound_vol=ofMap(n,0,1024,0,2);
	//							_sound_bgm[sound_index].setVolume(_sound_vol);
	//							_control_val[i]=n;
	//						}
	//						break;
	//				}
	//									
	//			}
	//			//cout<<ofToString(_control_val)<<endl;
	//		}
	//	}
	//}
	
}
void ofApp::triggerEvent(int ev){
	switch(ev){
		case 0:
			changeMode();
			break;
		case 1:
			resetAll();
			break;
	}
}
void ofApp::resetAll(){
	
	color_stage=0;
	ocean_type=false;
	road_sign_num=0;

	initPAnimal();
	changeMode(play_mode);

	_sound_bgm[sound_index].stop();

	sound_index=0;//(int)ofRandom(2);
	_sound_bgm[sound_index].play();

}
void ofApp::updateMidPos(){
	//vector<ofVec2f> poses;
	tmp_poses.clear();
	for(int i=0;i<pas.size();++i){
		tmp_poses.push_back(ofVec2f(pas[i].x,pas[i].y));
	}


	ofVec2f tmp(0,0);
	for(PAnimal& pa:pas){
		tmp.x+=pa.x;
		tmp.y+=pa.y;
	}
	tmp*=(1.0/pas.size());

	if(mid_pos.x==0 && mid_pos.y==0){
		mid_pos=tmp;
		cout<<"hi";	
	}

	//float mid_constrain=5;
	// mid_pos.x+=constrain(tmp.x-mid_pos.x,0,mid_constrain);
	// mid_pos.y+=constrain(tmp.y-mid_pos.y,-mid_constrain,mid_constrain);

	mid_pos=tmp;

	
	
	for(auto& pa:pas){
		pa.checkSpeed(_control_val[2],mid_pos);
		pa.checkCollide(_control_val[2],tmp_poses);
	}
	
	//return poses;

}
void ofApp::changeMode(){
	changeMode((play_mode+1)%M_MODE);
}
void ofApp::changeMode(int mode){
	//fill_canvas.reset();

	//play_mode=(play_mode+1)%M_MODE;	
	
	
	/*_sound_bgm[play_mode].stop();
	if(_sound_bgm[mode].isLoaded()) _sound_bgm[mode].play();*/

	play_mode=dest_pmode;
	dest_pmode=mode;

	for(auto &pa:pas){
	 	pa.changeMode(dest_pmode%2);
	 	
	}			
	_anim_mode_change.Restart();
}

//--------------------------------------------------------------
void ofApp::draw(){
	
	ofBackground(SceneColor[color_stage]);

	_fbo_fill.begin();
		ofEnableSmoothing();
		ofEnableAlphaBlending();

		ofClear(SceneColor[play_mode]);
		ofPushMatrix();
		ofTranslate(_distort_fill.getMargin()*ofGetWidth(),_distort_fill.getMargin()*ofGetHeight());
		ofTranslate(ofGetWidth()/2-mid_pos.x,ofGetHeight()/2-mid_pos.y);
		drawWindow(true);
		for(auto& pa:pas) pa.draw(true);		
		drawWindowLight();
		
		ofPopMatrix();
	_fbo_fill.end();

	_fbo_stroke.begin();
		ofEnableSmoothing();
		ofEnableAlphaBlending();

		ofClear(255,255,255,0);
		ofPushMatrix();
		ofTranslate(ofGetWidth()/2-mid_pos.x,ofGetHeight()/2-mid_pos.y);

		for(auto& pa:pas) pa.draw(false);			
		drawWindow(false);

		ofPopMatrix();
	_fbo_stroke.end();

	ofPushMatrix();
		//ofTranslate(-_distort_fill.getMargin()*ofGetWidth(),-_distort_fill.getMargin()*ofGetHeight());

		_fbo_fill.getTextureReference().bind();
		_distort_fill._mesh.draw();
		_fbo_fill.getTextureReference().unbind();
		//_distort_fill._mesh.drawWireframe();
	ofPopMatrix();
	
	_fbo_stroke.draw(0,0);

	//WindowTexture::GetInstance()->_fbo.draw(0,0);

	
#ifdef DDEBUG
	ofPushStyle();
	ofSetColor(ofColor::red);		
		ofDrawBitmapString(ofToString(ofGetFrameRate()),20,20);
		ofDrawBitmapString(ofToString(play_mode)+"->"+ofToString(dest_pmode)+" c= "+ofToString(color_stage),80,20);
		for(int i=0;i<M_CONTROL;++i) ofDrawBitmapString(ofToString(_control_val[i]),20,30+10*i);
		ofDrawBitmapString(ofToString(sound_index)+" : "+ofToString(_sound_bgm[sound_index].getPosition()),20,80);		
		ofDrawBitmapString(ofToString(_sound_vol),20,90);
	ofPopStyle();
#endif
	if(save_frame) ofSaveFrame(true);

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	switch(key){
		case 's':
			save_frame=!save_frame;
			break;
		case 'd':
			changeMode();
			break;
		
		case 'z':
			resetAll();
			
			break;
		case 'f':
			_sound_vol+=.1;
			break;
		case 'g':
			_sound_vol-=.1;
			break;
	}
}
#pragma region OF_CONTROL
//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
#pragma endregion


void ofApp::drawWindow(bool draw_fill){

	if(draw_fill){

		for(auto& sb:blocks){
			sb->draw(true);		
			//sb->drawLight();
		}
		for(auto& sb:front_blocks){
			sb->draw(true);			
			
		}
        
        float color_x=ofGetWidth()*(1-_anim_color_change.GetPortion());
        ofColor pre_color=SceneColor[(color_stage-1+4)%4];
		ofPushStyle();
		ofSetColor(SceneColor[color_stage]);

		int c=windows.size();

		for(int i=0;i<c;++i){				
			Window w=windows[i];
			float tx=w.x;//getCurrentX(dx_,w.x,w.wid);
			
            if(tx<color_x){
                if(tx+w.wid>color_x){
                    ofPushStyle();
                        ofSetColor(pre_color);
                        ofRect(tx,0,color_x-tx,w.y);
                        ofRect(tx,w.y+w.hei,color_x-tx,ofGetHeight()-w.y-w.hei);
                    ofPopStyle();
                
                    ofRect(color_x,0,tx+w.wid-color_x,w.y);
                    ofRect(color_x,w.y+w.hei,tx+w.wid-color_x,ofGetHeight()-w.y-w.hei);
                }else{
                    ofPushStyle();
                        ofSetColor(pre_color);
                        ofRect(tx,0,w.wid,w.y);
                        ofRect(tx,w.y+w.hei,w.wid,ofGetHeight()-w.y-w.hei);
                    ofPopStyle();
                }
                
            }else{
                ofRect(tx,0,w.wid,w.y);
                ofRect(tx,w.y+w.hei,w.wid,ofGetHeight()-w.y-w.hei);
            }
			float wmx=tx+w.wid;
			float mwid=0;
			if(i<c-1) mwid=(windows[i+1].x-wmx);
            
            if(wmx<color_x){
                if(wmx+mwid>color_x){
                    ofPushStyle();
                        ofSetColor(pre_color);
                        ofRect(wmx,0,color_x-wmx,ofGetHeight());
                    ofPopStyle();
                    ofRect(color_x,0,wmx+mwid-color_x,ofGetHeight());
                }else{
                    ofPushStyle();
                    ofSetColor(pre_color);
                        ofRect(wmx,0,mwid,ofGetHeight());
                    ofPopStyle();
                }
			}else{
                ofRect(wmx,0,mwid,ofGetHeight());
            }

		}



		ofPopStyle();

		/*for(auto& sb:blocks){
			sb->drawLight();
		}*/

	}else{
        float color_x=ofGetWidth()*(1-_anim_color_change.GetPortion());
        ofColor pre_color=SceneColor[(color_stage-1+4)%4];
		
		for(auto& win:windows) win.draw(false,0,(win.x<color_x)?pre_color:SceneColor[color_stage]);
		for(auto& chair:chairs){
			chair.draw(true,0,(chair.x<color_x)?pre_color:SceneColor[color_stage]);
			chair.draw(false,0,(chair.x<color_x)?pre_color:SceneColor[color_stage]);
			
		}
		for(auto& handle:handles) handle.draw(false,0,(handle.x<color_x)?pre_color:SceneColor[color_stage]);


	
	}
}

void ofApp::drawWindowLight(){
		
		int iscene=0;
		int c=windows.size();

		SceneBlock *scene=blocks[iscene];
		ofColor pre_color=scene->getLightColor();
		ofColor cur_color=pre_color;

		while(scene->x<0 && iscene<blocks.size()-1){
			iscene++;
			scene=blocks[iscene];

			pre_color=cur_color;
			cur_color=scene->getLightColor();
		}

		
		float sx=scene->x;
		
		ofPushStyle();
		ofSetColor(pre_color);
		ofFill();


		float shadow_y=ofGetHeight()*.61;
		float shadow_hei=1.1;
		float mx=0;
		for(int i=0;i<c;++i){
		
			Window w=windows[i];
			float tx=w.x;//getCurrentX(dx_,w.x,w.wid);
			

			while(sx<tx){
				
				if(sx>tx+w.wid) break;
				
				iscene++;
				if(iscene>=blocks.size()){
					pre_color=cur_color;
					break;
				}
				if(iscene<blocks.size()){
					scene=blocks[iscene];
					pre_color=cur_color;
					cur_color=scene->getLightColor();	
					sx=scene->x;

				}				
			}

			if(sx>=tx && sx<=tx+w.wid){
				if(pre_color!=ofColor::white){
					ofSetColor(pre_color,120);
					ofRect(tx,shadow_y*2-w.hei-w.y,sx-tx,w.hei*shadow_hei);
				}
				//right
				if(cur_color!=ofColor::white){
					ofSetColor(cur_color,120);
					ofRect(sx,shadow_y*2-w.hei-w.y,tx+w.wid-sx,w.hei*shadow_hei);
				}
			}else{
				if(pre_color!=ofColor::white){
					ofSetColor(pre_color,120);
					ofRect(tx,shadow_y*2-w.hei-w.y,w.wid,w.hei*shadow_hei);
				}
			}

			
		}
		ofPopStyle();
    
    float hei=ofGetHeight();
    for(auto& lb:front_blocks){
        if(lb->scene_type==7){
            ofSetColor(((LightBlock*)lb)->getColor(),60);
            ofVec2f pos=((LightBlock*)lb)->getLightPos();
            ofVec2f sze=((LightBlock*)lb)->getLightSize();
            ofRect(pos.x,shadow_y*2-sze.y-pos.y,sze.x,sze.y*shadow_hei*4);
        }
    }
}

void ofApp::initScene(){

	blocks.clear();

	FieldTexture::GetInstance()->setup(ofGetWidth(),ofGetHeight()*.3);
	OceanTexture::GetInstance()->setup(ofGetWidth(),ofGetHeight()*.3);
	RoofTexture::GetInstance()->setup(ofGetWidth(),ofGetHeight()*.3);
	LeafTexture::GetInstance()->setup(ofGetWidth(),ofGetHeight()*.3);
	WindowTexture::GetInstance()->setup(ofGetWidth(),ofGetHeight()*.3);

	addNewSceneBlock();
}
void ofApp::addNewSceneBlock(){	

	float x=0;
	int a=blocks.size();
	if(a>0) x=blocks[a-1]->getXBorder();

	float width=ofGetWidth();
	float height=ofGetHeight();

	while(x<width*1.1){

		float w=0;
		int color_type=0;
		switch(color_stage){
				case 0:
					w=ofRandom(.5,.8)*width;
					if(ofRandom(6)<1) color_type=1;
					break;
				/*case 1:
					w=ofRandom(.5,.8)*width;
					if(ofRandom(4)<1) color_type=(int)(ofRandom(1,3));
					break;*/
				case 1:
					w=ofRandom(.3,.6)*width;
					if(ofRandom(4)<1) color_type=(int)(ofRandom(1,3));
					break;
				case 2:
					w=ofRandom(.2,.6)*width;
					color_type=(int)(ofRandom(1,4));
					break;
				case 3:
					w=ofRandom(.2,.5)*width;
					color_type=3;
					break;
			}
		

		if(a>0 && blocks[a-1]->scene_type==4 && !((IslandBlock*)blocks[a-1])->has_right){

			w=width*ofRandom(.6,1.2);				
			blocks.push_back(new IslandBlock(x,height*.3,w,height*.3,color_type,false,(ocean_type?(ofRandom(2)<1):true),((IslandBlock*)blocks[a-1])->ocean_height));
		}else{
			int type=ocean_type?(ofRandom(5)<1?0:(ofRandom(2)<1?4:5)):floor(ofRandom(5));
					
			cout<<"create block: "<<x<<" "<<w<<" type: "<<type<<endl;

	
			switch(type){
				case 0:
					blocks.push_back(new SkyBlock(x,height*.3,w,height*.3,color_type));
					break;
				case 1:
					blocks.push_back(new CityBlock(x,height*.3,w,height*.3,color_type));
					break;
				case 2:
					blocks.push_back(new FieldBlock(x,height*.3,w,height*.3,color_type));
					break;
                case 3:
                    blocks.push_back(new FactoryBlock(x,height*.3,w,height*.3,color_type));
                    break;
                    
				case 4:
					blocks.push_back(new BeachBlock(x,height*.3,w,height*.3,color_type,true,true,height*.3*ofRandom(.3,.6)));
					break;
				case 5:
					w=width*ofRandom(.8,1.2);				
					blocks.push_back(new IslandBlock(x,height*.3,w,height*.3,color_type,true,ofRandom(2)<1,height*.3*ofRandom(.3,.6)));
					break;
                    
                
				
			}
			
		}
		x+=w;
		//front scene
	/*	if(ofRandom(5)<1){
			int type=floor(ofRandom(2));
			w=width;							
			switch(type){
				case 0:
					blocks.push_back(new TreeBlock(x,height*.3,w,height*.3,0));
					break;				
			}
			
		}*/
		

	}
    
    
//    if(color_stage>0 && ofGetFrameNum()%120==(int)abs(120*sin(ofGetFrameNum()))){
//        int count=(int)ofRandom(1,4);
//        float tx=width;
//        for(int i=0;i<count;++i){
//            float tw=ofRandom(.05,.2)*width;
//            front_blocks.push_back(new LightBlock(tx,height*.3,tw,height*.3,0));
//            tx+=tw;
//        }
//    }
    
	if(fmod(road_sign_num,width)==0){
			front_blocks.push_back(new SignBlock(width,height*.3,width/3,height*.3,0,road_sign_num/100));					
	}
}

void ofApp::addNewTrainScene(){	

	float x=0;
	int a=windows.size();
	if(a>0) x=windows[a-1].x+windows[a-1].wid;

	float width=ofGetWidth();
	float height=ofGetHeight();

	while(x<width){

		float w=ofRandom(.4,.8)*width;
		windows.push_back(Window(x+ofRandom(.1)*width,ofRandom(.3,.35)*height,ofRandom(.15,.28)*width,ofRandom(.15,.25)*height));
		
		int cc=floor(ofRandom(1,3));
		for(int i=0;i<cc;++i){
			chairs.push_back(Chair(x+width*1.8/13*i*ofRandom(.8,1.2),height*.5,width/24,height/6.4));
		}
		int hc=floor(ofRandom(1,3));
		for(int i=0;i<hc;++i){
			handles.push_back(THandle(x+width*1.8/13*i*ofRandom(.8,1.2),height*.25,width/32,height/12.8));
		}
		x+=w;
	}
}