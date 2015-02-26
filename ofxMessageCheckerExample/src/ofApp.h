#pragma once

#include "ofMain.h"
#include "ofxMessageChecker.h"

class ofApp : public ofBaseApp{
public:
    void setup() {
        ofAddListener(checker.gotMessage, this, &ofApp::gotMessageListener);
        // ex:
        //     checker.setup("2bit");
        // i.e. /Users/[Your User Name]/
        checker.setup("Your User Name Here");
    }
    void gotMessageListener(ofxMessageChecker::Message &m) {
        ofLogNotice() << m.messageID << ":" << m.message << " from " << m.userAccount;
    }
    
private:
    ofxMessageChecker checker;
};
