#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetBackgroundColor(ofColor::black);
	ofSetFrameRate(120);
}

//--------------------------------------------------------------
void ofApp::update(){
	g.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
	g.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if (key == 't')
		g.addNew = true;
	if (key == 'r')
		g.restart();
	if (key == OF_KEY_UP)
		g.move(up);
	if (key == OF_KEY_DOWN)
		g.move(down);
	if (key == OF_KEY_LEFT)
		g.move(left);
	if (key == OF_KEY_RIGHT)
		g.move(right);
}

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
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

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
