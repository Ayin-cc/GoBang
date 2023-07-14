#include "GoBangServe.h"

GoBangServe::GoBangServe(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    myServer = new Server(this);

    connect(myServer, &Server::change, this, &GoBangServe::refresh);
}

GoBangServe::~GoBangServe()
{}

void GoBangServe::refresh() {
    ui.textBrowser->clear();
    ui.textBrowser_2->clear();
    ui.textBrowser_3->clear();
    // 对战中
    if (!myServer->player1.isEmpty()) {
        ui.textBrowser->append(myServer->player1);
        ui.textBrowser->append(myServer->p1Socket->peerAddress().toString() + QString::number(myServer->p1Socket->peerPort()) + "\n");
    }
    if (!myServer->player2.isEmpty()) {
        ui.textBrowser->append(myServer->player2);
        ui.textBrowser->append(myServer->p2Socket->peerAddress().toString() + QString::number(myServer->p2Socket->peerPort()) + "\n");
    }
    // 观众
    for (int i = 0; i < myServer->watchers.size(); i++) {
        ui.textBrowser_2->append(myServer->watchersName[i]);
        ui.textBrowser_2->append(myServer->watchers[i]->peerAddress().toString() + QString::number(myServer->watchers[i]->peerPort()) + "\n");
    }
    // 待加入
    for (int i = 0; i < myServer->sockets.size(); i++) {
        ui.textBrowser_3->append(myServer->sockets[i]->peerAddress().toString() + ": " + QString::number(myServer->sockets[i]->peerPort()) + "\n");
    }
}
