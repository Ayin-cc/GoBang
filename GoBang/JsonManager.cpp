#include "JsonManager.h"

QString JsonManager::Player = QString("player");
QString JsonManager::Watcher = QString("watcher");
QString JsonManager::Fight = QString("fight");
QString JsonManager::Text = QString("text");
QString JsonManager::Round = QString("round");
QString JsonManager::Request = QString("req");

QString JsonManager::parseJson(QString data) {
	QJsonDocument doc = QJsonDocument::fromJson(data.toUtf8());
	QJsonObject rootObj = doc.object();
	if (rootObj.contains("type")) {
		QString type = rootObj["type"].toString();
		if (type == "player") {
			// 对局中的玩家
			QJsonArray array = rootObj["content"].toArray();
			if (array.size() == 0) {
				return "pla:";
			}
			else if (array.size() == 1) {
				return "pla:" + array[0].toString();
			}
			else if (array.size() == 2) {
				return "pla:" + array[0].toString() + ";" + array[1].toString();
			}
		}

		else if (type == "fight") {
			// 对局数据
			return "fig:" + rootObj["chess"].toString() + ";" + rootObj["x"].toString() + ";" + rootObj["y"].toString();
		}

		else if (type == "text") {
			// 文本信息
			return "tex:" + rootObj["content"].toString();
		}

		else if (type == "round") {
			// 回合数
			return "rod:" + rootObj["content"].toString();
		}
		else if (type == "capitulate") {
			// 对方认输
			return "cap:" + rootObj["content"].toString();
		}
		else if (type == "end") {
			return "end:";
		}
	}
}

QString JsonManager::playerToJson(QString player) {
	QJsonDocument doc;
	QJsonObject obj;
	obj.insert("type", "player");
	obj.insert("username", player);
	doc.setObject(obj);
	return doc.toJson(QJsonDocument::Indented);
}

QString JsonManager::watcherToJson(QString watcher) {
	QJsonDocument doc;
	QJsonObject obj;
	obj.insert("type", "watcher");
	obj.insert("username", watcher);
	doc.setObject(obj);
	return doc.toJson(QJsonDocument::Indented);
}

QString JsonManager::fightToJson(QString chess, int x, int y) {
	QJsonDocument doc;
	QJsonObject obj;
	obj.insert("type", "fight");
	obj.insert("chess", chess);
	obj.insert("x", QString::number(x));
	obj.insert("y", QString::number(y));
	doc.setObject(obj);
	return doc.toJson(QJsonDocument::Indented);
}

QString JsonManager::textToJson(QString username, QString content) {
	QString time = getTime();
	QJsonDocument doc;
	QJsonObject obj;
	obj.insert("type", "text");
	// 组合文本
	QString str = "[" + username + "]" + "[" + time + "]" + " " + content;
	obj.insert("content", str);
	doc.setObject(obj);
	return doc.toJson(QJsonDocument::Indented);
}

QString JsonManager::getTime() {
	QTime time = QTime::currentTime();
	return time.toString("HH:mm");
}

QString JsonManager::reqToJson(QString req) {
	QJsonDocument doc;
	QJsonObject obj;
	obj.insert("type", "req");
	obj.insert("content", req);
	doc.setObject(obj);
	return doc.toJson(QJsonDocument::Indented);
}

QString JsonManager::capitulateToJson(QString name) {
	QJsonDocument doc;
	QJsonObject obj;
	obj.insert("type", "capitulate");
	obj.insert("content", name);
	doc.setObject(obj);
	return doc.toJson(QJsonDocument::Indented);
}

QString JsonManager::endToJson() {
	QJsonDocument doc;
	QJsonObject obj;
	obj.insert("type", "end");
	doc.setObject(obj);
	return doc.toJson(QJsonDocument::Indented);
}