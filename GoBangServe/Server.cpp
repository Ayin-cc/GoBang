#include "Server.h"

Server::Server(QObject *parent)
	: QTcpServer(parent){
	this->myServer = new QTcpServer(this);

	connect(myServer, &QTcpServer::newConnection, this, &Server::newConnection);
	myServer->listen(QHostAddress::Any, 28875);
}

Server::~Server()
{}

void Server::newConnection() {
	// 连接新的客户端
	thisSocket = myServer->nextPendingConnection();
	sockets.append(thisSocket);

	// 为新客户端连接槽函数
	connect(thisSocket, &QTcpSocket::readyRead, this, &Server::receive);
	connect(thisSocket, &QTcpSocket::disconnected, this, &Server::socketDisconnect);

	emit change();
}

void Server::socketDisconnect() {
	// 获取断开连接的socket
	QTcpSocket* socket = qobject_cast<QTcpSocket*>(sender());
	if (!socket) return;

	if (p1Socket == socket) {
		player1 = nullptr;
		p1Socket = nullptr;
		if (!player2.isEmpty()) {
			QString json = JsonManager::capToJson("white");
			sendToAll(json);
			player1 = player2;
			p1Socket = p2Socket;
			player2 = nullptr;
			p2Socket = nullptr;
		}
		
	}
	else if (p2Socket == socket) {
		QString json = JsonManager::capToJson("black");
		sendToAll(json);
		player2 = nullptr;
		p2Socket = nullptr;
		
	}
	else if (watchers.contains(socket)) {
		watchers.removeOne(socket);
	}
	else if (sockets.contains(socket)) {
		sockets.removeOne(socket);
	}

	// 客户端刷新玩家列表
	QString json;
	if (player1.isEmpty()) {
		json = JsonManager::playerToJson(nullptr);
	}
	else {
		json = JsonManager::playerToJson(player1, player2);
	}
	sendToAll(json);

	emit change();
}

void Server::sendToOne(QTcpSocket* socket, QString data) {
	qDebug() << "sendToOne: " + data;
	data += "&";
	socket->write(data.toUtf8());
}

void Server::sendToAll(QString data) {
	qDebug() << "sendToAll: " + data;
	data += "&";

	// 对战中的玩家
	if (!player1.isEmpty()) p1Socket->write(data.toUtf8());
	if (!player2.isEmpty()) p2Socket->write(data.toUtf8());

	// 观众
	for (int i = 0; i < watchers.size(); i++) {
		watchers[i]->write(data.toUtf8());
	}

	// 待加入的玩家
	for (int i = 0; i < sockets.size(); i++) {
		sockets[i]->write(data.toUtf8());
	}
}

void Server::receive() {
	// 获取消息来源的socket
	QTcpSocket* socket = qobject_cast<QTcpSocket*>(sender());
	thisSocket = socket;
	if (!socket) return;

	QString jsons = socket->readAll();
	QStringList jsonList = jsons.split("&");
	for (int i = 0; i < jsonList.size() - 1; i++) {
		QString json = jsonList[i];
		QString buffer = JsonManager::parseJson(json);
		QString prefix = buffer.left(3);
		QString data;
		if(!buffer.mid(4).isEmpty()) {
			data = buffer.mid(4);
		}
		else {
			data = nullptr;
		}

		if (prefix == "pla") {
			// 添加新玩家
			if (p1Socket == socket || p2Socket == socket)	return;
			if (player1.isEmpty()) {
				player1 = data;
				p1Socket = socket;
			}
			else {
				player2 = data;
				p2Socket = socket;
				isGameGoing = true;
			}
			sockets.removeOne(socket);
			QString json;
			if (player1.isEmpty()) {
				json = JsonManager::playerToJson(nullptr);
			}
			else {
				if (player2.isEmpty()) {
					json = JsonManager::playerToJson(player1);
				}
				else {
					json = JsonManager::playerToJson(player1, player2);
				}
			}
			sendToAll(json);

			// 如果之前是观众，则从观众中去除
			if (watchers.contains(socket)) {
				watchers.removeOne(socket);
				watchersName.removeOne(data);
			}
			emit change();
		}
		else if (prefix == "wat") {
			// 添加新观众
			if (watchers.contains(socket)) return;
			watchersName.append(data);
			watchers.append(socket);
			sockets.removeOne(socket);

			// 如果之前是玩家，则从玩家中去除
			if (p1Socket == socket) {
				if (p2Socket) {
					p1Socket = p2Socket;
					player1 = player2;
					p2Socket = nullptr;
					player2 = nullptr;
				}
				else {
					p1Socket = nullptr;
					player1 = nullptr;
				}
			}
			if (p2Socket == socket) {
				p2Socket = nullptr;
				player2 = nullptr;
			}

			emit change();
		}
		else if (prefix == "fig") {
			// 对局数据
			sendToAll(data);
		}
		else if (prefix == "tex") {
			// 文本数据
			sendToAll(data);
		}
		else if (prefix == "req") {
			// 客户端请求
			sendResponse(socket, data);
		}
		else if (prefix == "cap") {
			// 对方认输
			QString json = JsonManager::capToJson(data);
			sendToAll(json);
		}
		else if (prefix == "end") {
			QString json = JsonManager::endToJson();
			sendToAll(json);
		}
	}
}

void Server::sendResponse(QTcpSocket* socket, QString req) {
	if (req == "player") {
		// 用户请求获取对局玩家信息
		QString json;
		if (player1.isEmpty()) {
			json = JsonManager::playerToJson(nullptr);
		}
		else {
			if (player2.isEmpty()) {
				json = JsonManager::playerToJson(player1);
			}
			else {
				json = JsonManager::playerToJson(player1, player2);
			}
		}
		sendToAll(json);
	}

	else if (req == "round") {
		// 用户请求获取回合数
		QString json = JsonManager::roundToJson(QString::number(this->round));
		sendToAll(json);
	}
}
