#ifndef PANIMAL_H
#define PANIMAL_H

#include "ofMain.h"
#include "FrameAnimation.h"
#include "Helicopt.h"
class PAnimal{

private:
	float head_wid,head_hei;
	float body_wid,body_hei;
	
	float leg_wid,leg_hei;
	int mleg;
	vector<float> leg_span;

	float tail_wid,tail_hei;
	ofColor fcolor;
	
	float phi;
	float elastic_strength;
	int elastic_stage;

	vector<float> land_poses;
	int land_length;
	float land_vel;
	int land_index;



	ofVec2f _move_vel,_calib_vel;

	int _cur_mode;
	int _dest_mode;

	float wheel_vel;
	float skate_vel;
	float run_vel;

	float v_jump_dest_y;
	float v_jump_vel;

	float s_body_hei_incre;

	ofVec2f first_foot_base;
	ofVec2f last_foot_base;

	FrameAnimation _anim_mode_change;
	FrameAnimation _anim_walk;
	FrameAnimation _anim_fly_leg;


	float _src_ang,_dest_ang;

	
	ofVec2f shear_ang;
	ofVec2f shear_dest;
	float head_ang;
	float ear_ang;
	float head_top;

	
	vector<float> kang,fang;
	vector<float> kang_dest,fang_dest;
	vector<int> leg_direction;

	
	Helicopt head_heli;
	Helicopt tail_heli;

	void drawLegs(float kang,float fang,float lx,float ly,float lw,float lh,int leg_index);
	
	void drawSleepLeg(float lx,float ly,float lw,float lh,int leg_index);
	void drawFlyLeg(float lx,float ly,float lw,float lh,int leg_index);
	void drawWalkLeg(float lx,float ly,float lw,float lh,int leg_index);
	
	

public:
	float x,y,wid,hei;
	
	PAnimal(float x_,float y_,float wid_,float hei_);

	void draw(bool draw_fill);

	void update(float ctrl_);

	void checkSpeed(float ctrl_,ofVec2f mid_);
	void checkCollide(float ctrl_,vector<ofVec2f> pos_);

	void changeMode(int mode_);

	bool operator < (const PAnimal& pa) const;

};



#endif