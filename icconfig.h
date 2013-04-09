#ifndef ICCONFIG_H
#define ICCONFIG_H

#include <QString>

// currently not used
const QString CONFIG_FILE = "configs/config.ini";

const QString APP_CONFIG_TAG = "protocol/application/tag";
const QString APP_CONFIG_VALUE = "protocol/application/value";
const QString VERSION_CONFIG_TAG = "protocol/version/tag";
const QString VERSION_CONFIG_VALUE = "protocol/version/value";
const QString COMMAND_CONFIG_TAG = "protocol/command/tag";
const QString SID_CONFIG_TAG = "protocol/sid/tag";
const QString UID_CONFIG_TAG = "protocol/uid/tag";
const QString QUESTIONS_CONFIG_TAG = "protocol/questions/tag";
const QString ANSWERS_CONFIG_TAG = "protocol/answers/tag";

// communication format
const int CONNECTION_OFFER = 1;
const int QUESTION_DELIVER = 4;
const int NOT_QUESTIONING = 5;
const int SERVER_DISCOVERY = 101;
const int QUESTION_REQUEST = 104;
const int ANSWER = 105;

// Following settings will take effect if not specified in config file
const QString DEFAULT_APP_VALUE = "IC2013";
const QString DEFAULT_VERSION_VALUE = "1";

// Connection settings
const int CLIENT_PORT = 8026;
const int SERVER_PORT = 8027;

// json tag name
const QString DEFAULT_ANSWERS_TAG = "answers";
const QString DEFAULT_QUESTIONS_TAG = "questions";

const QString DEFAULT_APP_TAG = "app";
const QString DEFAULT_VERSION_TAG = "version";
const QString DEFAULT_CMD_TAG = "cmd";
const QString DEFAULT_SID_TAG = "sid";
const QString DEFAULT_UID_TAG = "uid";

const QString SENDER_KEY = ".senderipaddress";

// question type
const int TOF = 1;
const int SINGLE_CHOICE = 2;
const int MULTI_CHOICE = 3;
const int UNDERSTAND = 4;

const int SERVER_SID_TEST_DURATION = 1000;

#endif // ICCONFIG_H
