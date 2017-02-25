#include "ofApp.h"
#include <stdlib.h>

using namespace ofxCv;
using namespace cv;

bool isSmiling;
bool showingNotHappy;
bool showingHappy;
vector<float> recentMeasurements;

void ofApp::setup() {
    isSmiling = true;
    showingHappy = false;
    showingNotHappy = false;
    ofResetElapsedTimeCounter();
    ofSetVerticalSync(true);
    ofSetFrameRate(120);
    cam.setup(640, 480);
    smile.setup();
}

void ofApp::update() {
    cam.update();
    if(cam.isFrameNew()) {
        smile.update(cam);
        if(smile.getFaceFound()) {
            float cur = smile.getSmileAmount();
            graph.add(cur);
            ofLog() << graph.getNormalized(cur);
            recentMeasurements.push_back(graph.getNormalized(cur));
            if(recentMeasurements.size() > 120) {
                recentMeasurements.erase(recentMeasurements.begin());
            }
            float avg = 0;
            vector<float>::iterator it = recentMeasurements.begin();
            for(; it != recentMeasurements.end(); ++it){
                avg += *it;
            }
            for(int i = 0; i < recentMeasurements.size(); i++) {
                avg += recentMeasurements[i];
            }
            avg = avg / recentMeasurements.size();
            if(avg > 0.5) {
                if(ofGetElapsedTimeMillis() > 5000 && !showingHappy) {
                    //system("osascript -e 'quit app \"Safari\"'");
                    //ofSleepMillis(500);
                    system("open http://localhost:8000/congrats.html");
                    ofResetElapsedTimeCounter();
                    showingHappy = true;
                    showingNotHappy = false;
                }
                isSmiling = true;
            } else {
                if(ofGetElapsedTimeMillis() > 5000 && !showingNotHappy) {
                    system("open http://localhost:8000");
                    ofResetElapsedTimeCounter();
                    showingHappy = false;
                    showingNotHappy = true;
                }
                isSmiling = false;
            }
        }
    }
}

void ofApp::draw() {
    ofSetColor(255);
    cam.draw(0, 0);
    smile.draw();
    
    ofTranslate(10, 10);
    ofSetColor(0);
    ofDrawRectangle(0, 0, 300, 100);
    ofSetColor(255);
    graph.draw(300, 100);
}
