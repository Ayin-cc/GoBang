#pragma once

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QTime>
#include <string>

class JsonManager {
public:
	static QString Player;
	static QString Watcher;
	static QString Fight;
	static QString Text;
	static QString Round;
	static QString Request;

	static QString parseJson(QString data);
	static QString playerToJson(QString player);
	static QString watcherToJson(QString watcher);
	static QString fightToJson(QString chess, int x, int y);
	static QString textToJson(QString username, QString content);
	static QString getTime();
	static QString reqToJson(QString req);
	static QString capitulateToJson(QString name);
	static QString endToJson();
};