#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openframeworks");

	ofBackground(239);
	ofEnableDepthTest();

	for (int i = 0; i < 80; i++) {

		this->noise_seed_list.push_back(glm::vec3(ofRandom(1000), ofRandom(1000), ofRandom(1000)));
		this->noise_step_list.push_back(ofRandom(1000));
	}

	ofColor color;
	vector<int> hex_list = { 0x247BA0, 0x70C1B3, 0xB2DBBF, 0xF3FFBD, 0xFF1654 };
	for (auto hex : hex_list) {

		color.setHex(hex);
		this->color_list.push_back(color);
	}
}

//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(39);
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();

	for (int i = 0; i < this->noise_seed_list.size(); i++) {
		
		ofMesh face, frame;
		frame.setMode(ofPrimitiveMode::OF_PRIMITIVE_LINES);
		glm::vec3 param;
		switch ((int)ofRandom(3)) {
		case 0:
			param = glm::vec3(5, 0, 0);
			break;
		case 1:
			param = glm::vec3(0, 5, 0);
			break;
		case 2:
			param = glm::vec3(0, 0, 5);
			break;
		}

		int n = int(ofRandom(120) + ofGetFrameNum()) % 120;
		int len = n < 60 ? ofMap(n, 0, 60, 1, 35) : ofMap(n, 60, 120, 35, 1);

		for (int k = 0; k < len; k++) {

			auto location = glm::vec3(
				ofMap(ofNoise(this->noise_seed_list[i].x, this->noise_step_list[i] + k * 0.005), 0, 1, -250, 250),
				ofMap(ofNoise(this->noise_seed_list[i].y, this->noise_step_list[i] + k * 0.005), 0, 1, -250, 250),
				ofMap(ofNoise(this->noise_seed_list[i].z, this->noise_step_list[i] + k * 0.005), 0, 1, -250, 250));

			auto side_1 = location + param;
			auto side_2 = location - param;

			face.addVertex(side_1);
			face.addVertex(side_2);
			frame.addVertex(side_1);
			frame.addVertex(side_2);

			if (k > 0) {

				int index = face.getNumVertices() - 1;
				face.addIndex(index - 0); face.addIndex(index - 1); face.addIndex(index - 2);
				face.addIndex(index - 3); face.addIndex(index - 1); face.addIndex(index - 2);
				frame.addIndex(index - 0); frame.addIndex(index - 2);
				frame.addIndex(index - 1); frame.addIndex(index - 3);
			}
		}

		frame.addIndex(0); frame.addIndex(1);
		frame.addIndex(frame.getNumVertices() - 1); frame.addIndex(frame.getNumVertices() - 2);

		ofSetColor(this->color_list[(int)ofRandom(this->color_list.size())]);
		face.draw();
		ofSetColor(39);
		frame.drawWireframe();

		this->noise_step_list[i] += 0.005;
	}

	this->cam.end();
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}