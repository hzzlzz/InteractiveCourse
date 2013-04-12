#include "icmessagemapper.h"
#include <QSettings>
#include "icconfig.h"

ICMessageMapper::ICMessageMapper() : Mapper()
{
    QSettings settings(CONFIG_FILE, QSettings::IniFormat);

    addMapping(settings.value(APP_CONFIG_TAG,DEFAULT_APP_TAG).toString(), "app");
    addMapping(settings.value(VERSION_CONFIG_TAG,DEFAULT_VERSION_TAG).toString(), "version");
    addMapping(settings.value(COMMAND_CONFIG_TAG,DEFAULT_CMD_TAG).toString(), "cmd");
    addMapping(settings.value(SID_CONFIG_TAG,DEFAULT_SID_TAG).toString(), "sid");
    addMapping(settings.value(UID_CONFIG_TAG,DEFAULT_UID_TAG).toString(), "uid");
    addMapping(settings.value(QUESTIONS_CONFIG_TAG,DEFAULT_QUESTIONS_TAG).toString(), "questions");
    addMapping(settings.value(ANSWERS_CONFIG_TAG,DEFAULT_ANSWERS_TAG).toString(), "answers");
    addMapping(SENDER_KEY, "senderip");
}
