//
//  ofxMessageChecker.h
//
//  Created by ISHII 2bit on 2015/02/26.
//
//

#pragma once

#include "ofMain.h"
#include "SQLiteC++.h"

#define QUERY(...) #__VA_ARGS__

static const char *OFX_MESSAGE_CHECKER_QUERY_FORMAT = QUERY(
    SELECT
        message.ROWID,
        message.text,
        message.date,
        handle.id,
        attachment.filename
    FROM message
    LEFT OUTER JOIN handle ON
        message.handle_id = handle.ROWID
    LEFT OUTER JOIN message_attachment_join ON
        message.ROWID = message_attachment_join.message_id
    LEFT OUTER JOIN attachment ON
        message_attachment_join.attachment_id = attachment.ROWID
    WHERE
        %d < message.ROWID;
);

class ofxMessageChecker {
public:
    struct Message {
        int messageID; // message.ROWID
        string message;
        string userAccount; // message.text
        int timestamp; // message.date
        bool hasAttachment; // if attachmentPath != ""
        string attachmentPath; // attachment.filename
    };
    ofEvent<Message> gotMessage;
    
    ~ofxMessageChecker() {
        disableAutoUpdate();
    }
    
    void setup(const string &computerUserName, int latestID = 0, bool bAutoUpdate = true, float updateInterval = 3.0f) {
        userName = computerUserName;
        databasePath = string("/Users/") + userName + "/Library/Messages/chat.db";
        this->latestID = latestID;
        this->updateInterval = updateInterval;
        latestUpdateTime = 0.0f;
        
        setAutoUpdate(bAutoUpdate);
    }
    
    void update() {
        updateImpl();
    }
    
    int getLatestMessageID() const {
        return latestID;
    }
    
    float getLatestUpdateTime() const {
        return latestUpdateTime;
    }
    
    void setAutoUpdate(bool bAutoUpdate) {
        if(bAutoUpdate) {
            enableAutoUpdate();
        } else {
            disableAutoUpdate();
        }
    }
    
    bool isAutoUpdate() const {
        return bAutoUpdate;
    }
    
    void setUpdateInterval(float updateInterval) {
        this->updateInterval = updateInterval;
    }
    
    float getUpdateInterval() const {
        return updateInterval;
    }
    
    void enableAutoUpdate() {
        if(!isAutoUpdate()) {
            bAutoUpdate = true;
            ofAddListener(ofEvents().update, this, &ofxMessageChecker::updateImpl, OF_EVENT_ORDER_BEFORE_APP);
        }
    }
    
    void disableAutoUpdate() {
        if(isAutoUpdate()) {
            bAutoUpdate = false;
            ofRemoveListener(ofEvents().update, this, &ofxMessageChecker::updateImpl, OF_EVENT_ORDER_BEFORE_APP);
        }
    }
    
private:
    string userName;
    string databasePath;
    bool bAutoUpdate;
    float updateInterval;
    float latestUpdateTime;
    
    void updateImpl(ofEventArgs & args) {
        if(ofGetElapsedTimef() < latestUpdateTime + updateInterval) {
            updateImpl();
        }
    }
    
    void updateImpl() {
        SQLite::Database sqlite(databasePath);
        string queryString = ofVAArgsToString(OFX_MESSAGE_CHECKER_QUERY_FORMAT, latestID);
        try {
            SQLite::Statement query(sqlite, queryString);
            while(query.executeStep()) {
                Message m;
                m.messageID = query.getColumn(0);
                m.message = query.getColumn(1).getText();
                m.timestamp = query.getColumn(2);
                if(!query.getColumn(3).isNull()) {
                    m.userAccount = query.getColumn(3).getText();
                }
                if(!query.getColumn(4).isNull()) {
                    m.attachmentPath = query.getColumn(4).getText();
                }
                m.hasAttachment = (m.attachmentPath != "");
                
                latestID = max(latestID, m.messageID);
                
                ofNotifyEvent(gotMessage, m, this);
            }
        } catch (std::exception &e) {
            ofLogError() << "SQLite exception: " << e.what();
        }
        latestUpdateTime = ofGetElapsedTimef();
    }
    
    int latestID;
};