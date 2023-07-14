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
	static QString playerToJson(QString player1, QString player2 = nullptr);
	static QString roundToJson(QString round);
	static QString endToJson();
	static QString capToJson(QString chess);
};
