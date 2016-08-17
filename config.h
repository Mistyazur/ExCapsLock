#ifndef CONFIG_H
#define CONFIG_H

#include <QCoreApplication>

#define USER_SETTINGS	QCoreApplication::applicationDirPath() + "/settings.user.json"
#define KEY_APP_PATH	"run.app.path"

#endif // CONFIG_H
