#include "JsonManager.h"

QString JsonManager::Player = QString("player");
QString JsonManager::Watcher = QString("watcher");
QString JsonManager::Fight = QString("fight");
QString JsonManager::Text = QString("text");
QString JsonManager::Round = QString("round");
QString JsonManager::Request = QString("req");

QString JsonManager::parseJson(QString data) {
	qDebug() << "ParseJson: " + data;
	QJsonDocument doc = QJsonDocument::fromJson(data.toUtf8());
	QJsonObject rootObj = doc.object();
	if (rootObj.contains("type")) {
		QString type = rootObj["type"].toString();
		if (type == "player") {
			// 新玩家请求加入对局
			return "pla:" + rootObj["username"].toString();
		}
		else if (type == "watcher") {
			// 新观众
			return "wat:" + rootObj["username"].toString();
		}
		else if (type == "fight") {
			// 对局数据
			return "fig:" + data;
		}
		else if (type == "text") {
			// 文本信息
			return "tex:" + data;
		}
		else if (type == "req") {
			// 客户端请求数据
			return "req:" + rootObj["content"].toString();
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

QString JsonManager::playerToJson(QString player1, QString player2) {
	QJsonArray players;
	QJsonDocument doc;
	QJsonObject obj;
	players.append(player1);
	if (!player2.isEmpty()) {
		players.append(player2);
	}
	obj.insert("type", "player");
	obj.insert("content", players);
	doc.setObject(obj);
	return doc.toJson(QJsonDocument::Indented);
}

QString JsonManager::roundToJson(QString round) {
	QJsonDocument doc;
	QJsonObject obj;
	obj.insert("type", "round");
	obj.insert("content", round);
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

QString JsonManager::capToJson(QString chess) {
	QJsonDocument doc;
	QJsonObject obj;
	obj.insert("type", "capitulate");
	obj.insert("content", chess);
	doc.setObject(obj);
	return doc.toJson(QJsonDocument::Indented);
}