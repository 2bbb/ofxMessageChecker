# ofxMessageChecker (OSX only)

read message data of Message.app

## Dependencies

* [ofxSQLiteCpp](https://github.com/bakercp/ofxSQLiteCpp) by bakercp

## API

### ofxMessageChecker

#### void setup(const string &computerUserName, int latestID = 0, bool bAutoUpdate = true, 

float updateInterval = 3.0f)
	* _computerUseName_ : your account name i.e. /User/**YourComputerUserName**/
	* _latestID_ : will read from this id
	* _bAutoUpdate_ : if set false, you must call *ofxMessageChecker::update()* manually.
	* _updateInterval_ : if _bAutoUpdate_ is **true** then *update()* is called every _updateInterval_ sec.

#### void update()

if you set disable auto update then you must call this method manually.

#### void enableAutoUpdate()
#### void disableAutoUpdate()

change auto update mode.

### Notify Event

ofxMessageChecker will push event with ofxMessageChecker::gotMessage.

you can receive event, like this:

```
    ofAddListener(checker.gotMessage, this, &ofApp::gotMessageListener);
```

here, *checker* is instance of _ofxMessageChecker_, and _ofApp::gotMessageListener_ is define as

```
void ofApp::gotMEssageListener(ofxMessageChecker::Message &m) {
	// some code
}
```

### struct ofxMessageChecker::Mesage

* int _messageID_ : internal id
* string _message_ : message body
* string _userAccount_ : tel no. or mail address of message's sender
* int _timestamp_ : received date
* bool _hasAttachment_ : if this value is **true** then message has attachment
* string _attachmentPath_ : if _hasAttachment_ is **true** then this contain path of attachment file


## Update history

### 2015/02/26 ver 0.01 release

## License

MIT License.

## Author

* ISHII 2bit [bufferRenaiss co., ltd.]
* ishii[at]buffer-renaiss.com

## At the last

Please create new issue, if there is a problem.
And please throw pull request, if you have a cool idea!!