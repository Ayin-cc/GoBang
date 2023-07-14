#pragma once

#include <QString>
#include <QTcpSocket>
#include <QMainWindow>

#include "JsonManager.h"

class Client : public QTcpSocket{
	Q_OBJECT

private:
	QString username;
	QTcpSocket* mySocket;		
	
public:
	Client(QObject* parent);
	~Client();

	static QString Player;
	static QString Watcher;
	static QString Fight;
	static QString Text;
	static QString Round;
	static QString Request;
	static QString End;

	bool isPlayerEnough = false;
	QString player1;
	QString player2;
	int round;
	QString newText;
	QString fight;
	QString capitulate;
	bool isPlayer;
	QString chess;

	bool isConnected();
	void setUsername(QString username);
	QString getUsername();
	void connectToServer(const QString& host, quint16 port);
	void disconnectFromServer();
	void sendToServer(QString type, QString data);
	void sendToServer(QString type, QString chess, int x, int y);


public slots:
	void acceptFromServer();

signals:
	void onConnected();
	void onDisconnected();
	void playerRecived();
	void playerEnough();
	void fightRecived();
	void textRecived();
	void roundRecived();
	void isCapitulate();
	void gameEnd();
};

