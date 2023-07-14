#pragma once

#include <QTcpServer>
#include <QTcpSocket>

#include "JsonManager.h"

class Server  : public QTcpServer
{
	Q_OBJECT

public:
	Server(QObject *parent = nullptr);
	~Server();

	QTcpServer* myServer;
	QTcpSocket* thisSocket;
	QList<QTcpSocket*> sockets;
	QList<QTcpSocket*> watchers;
	QList<QString> watchersName;
	QString player1;
	QString player2;
	QTcpSocket* p1Socket;
	QTcpSocket* p2Socket;
	bool isGameGoing;
	int round = 0;

	void sendToAll(QString data);
	void sendToOne(QTcpSocket* socket, QString data);
	void receive();
	void socketDisconnect();

signals:		
	void change();
	void receivedText(QString text);
	void receivedFight(QString fight);
	void receivedReq(QString req);
	

public slots:
	void newConnection();
	void sendResponse(QTcpSocket* socket, QString req);

private:
	QString host = "127.0.0.1";
	int port = 8888;	
};
