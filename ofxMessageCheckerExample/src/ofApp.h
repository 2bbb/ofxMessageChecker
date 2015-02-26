#pragma once

#include "ofMain.h"
#include "ofxMessageChecker.h"

class ofApp : public ofBaseApp{
public:
    void setup() {
        ofAddListener(checker.gotMessage, this, &ofApp::gotMessageListener);
        checker.setup("Your User Name Here");
        // ex:
        // checker.setup("2bit");
    }
    void gotMessageListener(ofxMessageChecker::Message &m) {
        ofLogNotice() << m.messageID << ":" << m.message << " from " << m.userAccount;
    }
    
private:
    ofxMessageChecker checker;
};
