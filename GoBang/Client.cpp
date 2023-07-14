#include "Client.h"

QString Client::Player = QString("player");
QString Client::Watcher = QString("watcher");
QString Client::Fight = QString("fight");
QString Client::Text = QString("text");
QString Client::Round = QString("round");
QString Client::Request = QString("req");
QString Client::End = QString("end");

Client::Client(QObject* parent) : QTcpSocket(parent){
	mySocket = new QTcpSocket(this);
	round = 0;
	isPlayer = false;
}

Client::~Client() {

}

void Client::setUsername(QString username) {
	this->username = username;
}

QString Client::getUsername() {
	return this->username;
}

// 连接至服务器
void Client::connectToServer(const QString& host, quint16 port) {
	mySocket->connectToHost(host, port);
	connect(mySocket, &QTcpSocket::readyRead, this, &Client::acceptFromServer);
}

// 断开连接
void Client::disconnectFromServer() {
	if (isPlayer) {
		if (player1 == username) {
			player1 = player2;
			player2 = nullptr;
		}
		else {
			player2 = nullptr;
		}
	}
	//sendToServer("capitulate", chess);
	mySocket->disconnectFromHost();
}

bool Client::isConnected() {
	return mySocket->state() == QAbstractSocket::ConnectedState;
}

// 发送
void Client::sendToServer(QString type, QString data) {
	if (!isConnected()) return;
	QString newJson = nullptr;
	if (type == "player") {
		newJson = JsonManager::playerToJson(data);
	}
	else if (type == "watcher") {
		newJson = JsonManager::watcherToJson(data);
	}
	else if (type == "text") {
		newJson = JsonManager::textToJson(this->username, data);
	}
	else if (type == "req") {
		newJson = JsonManager::reqToJson(data);
	}
	else if (type == "capitulate") {
		newJson = JsonManager::capitulateToJson(data);
	}
	else if (type == "end") {
		newJson = JsonManager::endToJson();
	}
	mySocket->write(newJson.toUtf8() + "&");
}

void Client::sendToServer(QString type, QString chess, int x, int y) {
	QString json;
	if (type == "fight") {
		json = JsonManager::fightToJson(chess, x, y);
	}
	mySocket->write(json.toUtf8() + "&");
}

// 接收
void Client::acceptFromServer() {
	QString buffer = mySocket->readAll();
	QStringList buffers = buffer.split("&");

	for (int i = 0; i < buffers.size() - 1; i++) {
		QString json = buffers[i];
		QString data = JsonManager::parseJson(json);

		// 处理消息内容
		QString prefix = data.left(3);
		data = data.mid(4);

		if (prefix == "pla") {
			// 对局中玩家信息
			if (data.contains(";")) {
				QList players = data.split(";");
				player1 = players[0];
				player2 = players[1];
				isPlayerEnough = true;
				emit playerRecived();
				emit playerEnough();
			}
			else {
				player1 = data;
				player2 = nullptr;
				isPlayerEnough = false;
				emit playerRecived();
			}
		}
		else if (prefix == "fig") {
			fight = data;

			// 对局信息
			emit fightRecived();
		}
		else if (prefix == "tex") {
			newText = data;

			// 文字信息
			emit textRecived();
		}
		else if (prefix == "cap") {
			capitulate = data;			
			// 认输
			emit isCapitulate();
		}
		else if (prefix == "end") {
			emit gameEnd();
		}
	}
}

