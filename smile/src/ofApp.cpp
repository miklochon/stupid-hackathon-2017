#include "ofApp.h"
#include <stdlib.h>

using namespace ofxCv;
using namespace cv;

bool isSmiling;
bool showingNotHappy;
bool showingHappy;
bool detectedUnhappy;
vector<float> recentMeasurements;

void ofApp::setup() {
    detectedUnhappy = false;
    isSmiling = true;
    showingHappy = false;
    showingNotHappy = false;
    ofResetElapsedTimeCounter();
    ofSetVerticalSync(true);
    ofSetFrameRate(60);
    cam.setup(1280, 720);
    smile.setup();
}

void ofApp::update() {
    cam.update();
    if(cam.isFrameNew()) {
        smile.update(cam);
        if(smile.getFaceFound()) {
            float cur = smile.getSmileAmount();
            graph.add(cur);
            //ofLog() << cur;
            //ofLog() << graph.getNormalized(cur);
            recentMeasurements.push_back(cur);
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
            ofLog() << avg;
            if(avg > 120) {
                if(ofGetElapsedTimeMillis() > 5000 && !showingHappy && detectedUnhappy) {
                    //system("osascript -e 'quit app \"Safari\"'");
                    //ofSleepMillis(500);
                    system("open http://localhost:8000/congrats.html");
                    system("python /Users/carl/Documents/Dev/stupid-hackathon-2017/twilio/happy.py");
                    ofResetElapsedTimeCounter();
                    showingHappy = true;
                    showingNotHappy = false;
                    detectedUnhappy = false;
                }
                isSmiling = true;
            } else {
                if(ofGetElapsedTimeMillis() > 5000 && !showingNotHappy && !detectedUnhappy) {
                    system("open http://localhost:8000");
                    system("python /Users/carl/Documents/Dev/stupid-hackathon-2017/twilio/not-happy.py");
                    ofResetElapsedTimeCounter();
                    showingHappy = false;
                    showingNotHappy = true;
                    detectedUnhappy = true;
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
    
    ofTranslate(970, 610);
    ofSetColor(0);
    ofDrawRectangle(0, 0, 300, 100);
    ofSetColor(255);
    graph.draw(300, 100);
}
