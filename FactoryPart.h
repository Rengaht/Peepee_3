//
//  FactoryPart.h
//  Peepeee_3_1
//
//  Created by reng on 2016/2/10.
//
//

#ifndef Peepeee_3_1_FactoryPart_h
#define Peepeee_3_1_FactoryPart_h

class FactoryPart{
    float x,y,wid,hei;
    float phi;
    ofColor fcolor;
    int mstroke;
    bool is_round;
public:
    static ofColor pcolor[5];
    
    FactoryPart(float x_,float y_,float wid_,float hei_){
        x=x_; y=y_; wid=wid_; hei=hei_;
        phi=ofRandom(TWO_PI);
        fcolor=pcolor[int(ofRandom(5))];
        fcolor.r+=ofRandom(-5,5);
        fcolor.g+=ofRandom(-5,5);
        fcolor.b+=ofRandom(-5,5);
        
        mstroke=floor(ofRandom(1,3));
        is_round=ofRandom(2)<1;
        
    }
    void draw(bool draw_fill){
        ofPushStyle();
		if(draw_fill){
			ofSetColor(fcolor);
		}else{
			ofNoFill(); ofSetColor(80,180);
		}
		ofPushMatrix();
        ofTranslate(x+wid/2,y);
        int c=(draw_fill)?1:mstroke;
        for(int i=0;i<c;++i){
            if(draw_fill) i+=mstroke+1;
            float dx=(draw_fill)?0:cos(i+phi)*.05*wid;
            float dy=(draw_fill)?0:sin(i+phi)*.05*hei;
            
            if(is_round){
                ofBeginShape();
                    ofVertex(-wid/2,hei);
                    ofVertex(wid/2+dx,hei+dy);
                    ofBezierVertex(wid+dx*2,hei-dy,wid+dx,0,
                                   wid/2,dy);
                    ofVertex(-wid/2,-dy);
                    ofBezierVertex(-wid-dx*2,dy,-wid-dx*3,hei,-wid/2-dx,hei+dy);
                ofEndShape(true);

            }else{
                ofBeginShape();
                    ofVertex(-wid/2,hei);
                    ofVertex(wid/2+dx,hei+dy);
                    ofVertex(wid/4-dx,-dy);
                    ofVertex(-wid/4+dx,dy);
                ofEndShape(true);
            }
        }
        
        ofPopMatrix();
        
        ofPopStyle();
    }
};
class FactoryLadder{
    float x,y,wid,hei;
    float phi;
    int mstroke;
    float mlad;
public:
    
    FactoryLadder(float x_,float y_,float wid_,float hei_){
        x=x_; y=y_; wid=wid_; hei=hei_;
        phi=ofRandom(TWO_PI);
        
        mstroke=floor(ofRandom(1,2));
        mlad=ofRandom(5,20);
    }
    void draw(bool draw_fill){
        ofPushStyle();
		ofNoFill(); ofSetColor(20*abs(sin(phi)),120);
		ofSetLineWidth(.5);
        ofPushMatrix();
        ofTranslate(x+wid/2,y);
        int c=(draw_fill)?1:mstroke;
        
        float elad=hei/mlad;
        
        for(int i=0;i<c;++i){
            if(draw_fill) i+=mstroke+1;
            float dx=(draw_fill)?0:cos(i+phi)*.05*wid;
            float dy=(draw_fill)?0:sin(i+phi)*.05*hei;
            
            ofLine(wid/12,0,wid/2-dx,hei+dy);
            ofLine(-wid/12+dx,dy,-wid/2-dx,hei+dy);
            
            ofBeginShape();
            for(int i=0;i<mlad;++i) ofVertex(ofLerp(wid/12+dx,wid/2-dx,(float)i/mlad)*(i%2==0?-1:1),(elad+dy*sin(i))*i);
            
            ofEndShape(true);
            
        }
        
        ofPopMatrix();
        
        ofPopStyle();
    }

};

class FactorySmoke{
    float x,y,wid,hei;
    float phi;
    ofColor fcolor;
    int mstroke;
    float vel;
    
    ofMesh _mesh;
    
public:
    static ofColor pcolor[5];
    
    FactorySmoke(float x_,float y_,float wid_,float hei_){
        x=x_; y=y_; wid=wid_; hei=hei_;
        phi=ofRandom(TWO_PI);
        fcolor=pcolor[int(ofRandom(5))];
        fcolor.r+=ofRandom(-5,5);
        fcolor.g+=ofRandom(-5,5);
        fcolor.b+=ofRandom(-5,5);
        fcolor.a=ofRandom(80,150);
        
        mstroke=floor(ofRandom(1,3));
        
        vel=ofRandom(120,250);
        
        createMesh();
    }
    void createMesh(){
        
        float W=ofGetWidth();
		float H=ofGetHeight()*.33;

        ofVec2f radi(wid,0);
        float ang=ofRandom(2,10);
        
        //top
        _mesh.addVertex(ofPoint(-wid/2,0));
        _mesh.addTexCoord(ofPoint((x-wid/2)/W,y/H));
        
        radi.rotate(-ang);
        _mesh.addVertex(ofPoint(-wid/2+radi.x,0+radi.y));
        _mesh.addTexCoord(ofPoint((x-wid/2+radi.x)/W,(y+0+radi.y))/H);
        
        _mesh.addVertex(ofPoint(wid/2,0));
        _mesh.addTexCoord(ofPoint((x+wid/2)/W,(y+0)/H));
        
        //bottom
        _mesh.addVertex(ofPoint(-wid/2,0));
        _mesh.addTexCoord(ofPoint((x-wid/2)/W,y/H));
        
        radi.rotate(ang*2);
        _mesh.addVertex(ofPoint(-wid/2+radi.x,0+radi.y));
        _mesh.addTexCoord(ofPoint((x-wid/2+radi.x)/W,(y+0+radi.y)/H));
        
        _mesh.addVertex(ofPoint(wid/2,0));
        _mesh.addTexCoord(ofPoint((x+wid/2)/W,(y+0)/H));
        
        
    }
    void draw(bool draw_fill){
        
        x+=wid/30.0*sin((float)ofGetFrameNum()/vel+phi);
        
        ofPushStyle();
		if(draw_fill){
			ofSetColor(fcolor);
		}else{
			ofNoFill(); ofSetColor(80,180);
		}
		ofPushMatrix();
        ofTranslate(x+wid/2,y+hei/2);
        ofRotate(180);
//        int c=(draw_fill)?1:mstroke;
//        for(int i=0;i<c;++i){
//            if(draw_fill) i+=mstroke+1;
//            float dx=(draw_fill)?0:cos(i+phi)*.05*wid;
//            float dy=(draw_fill)?0:sin(i+phi)*.05*hei;
//            
//                ofBeginShape();
//                ofVertex(-wid/2,0);
//                ofBezierVertex(wid/2,-hei/2,wid/2,-hei/2,
//                               wid/2,0);
//                ofBezierVertex(wid/2,hei/2,wid/2,hei/2,
//                           -wid/2,0);
//                ofEndShape(true);
//            
//        }
        //_mesh.draw();
        WindowTexture::GetInstance()->_fbo.getTextureReference().bind();
        ofSetTextureWrap(GL_REPEAT,GL_REPEAT);
        _mesh.draw();
        WindowTexture::GetInstance()->_fbo.getTextureReference().unbind();
        
        
        
        ofPopMatrix();
        
        ofPopStyle();
    }

    
};

#endif
