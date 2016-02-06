#pragma once

#include "ofMain.h"
#include "FieldTexture.h"
#include "StringSplit.h"
#include "DistortGraph.h"
#include "PAnimal.h"
#include "SceneBlock.h"
#include "Window.h"


class ofApp : public ofBaseApp{

	public:
		
		static int OCEAN_LENGTH;
		
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
		float _control_val[3];

		ofSoundPlayer  _sound_bgm[4];
		ofSerial _serial_ctrl;
		
		ofColor SceneColor[5];

		float road_sign_num;
		
		bool ocean_type;
		
		float _sound_vol;

	private:
			
		int M_CONTROL;
		int M_MODE;

		int mpas;
		vector<PAnimal> pas;

		ofVec2f mid_pos;
		float dest_pos;


		bool save_frame;

		//DistortGraphic fill_canvas,stroke_canvas;

		int play_mode,dest_pmode;
		int mode_start_time;
		int sound_index;

		int color_stage;


		float angle_for_mode;

		FrameAnimation _anim_mode_change;

		ofFbo _fbo_fill,_fbo_stroke;
		DistortGraph _distort_fill;
		/*ofImage _dimage;
		ofPixels _dpixels;
		ofShader _dshader;*/

		// things in train
		vector<Chair> chairs;
		vector<THandle> handles;
		vector<Window> windows;

		
		void initTrain();
		
		void initPAnimal();
		void updateMidPos();
		vector<ofVec2f> tmp_poses;

		void changeMode();
		void changeMode(int mode);
		
		
		void initControl();
		void initSound();

		void triggerEvent(int ev);

		void drawWindow(bool draw_fill);
		void drawWindowLight();

		vector<SceneBlock*> blocks;
		vector<SceneBlock*> front_blocks;

		void initScene();
		void addNewSceneBlock();
		void addNewTrainScene();

		void resetAll();
		
};
