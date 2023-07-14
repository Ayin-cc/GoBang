#pragma once

#include <QtWidgets/QMainWindow>
#include <string>
#include <QScreen>
#include <QListWidgetItem>
#include <QThread>

#include "ui_mainWindow.h"
#include "Client.h"
#include "UsernameDialog.h"
#include "Chess.h"

class mainWindow : public QMainWindow
{
    Q_OBJECT

public:
    mainWindow(QWidget *parent = nullptr);
    ~mainWindow();
    void initWindow();
    void refresh();
    void setPlayer();
    void setNewText();
    void setRound();
    void setFight();
    void drawPiece();
    void gameProcess();
    void hint(QString chess);
    QLabel* wlabel;
    QLabel* blabel;
    QList<QLabel*> pieces;
    void  mousePressEvent(QMouseEvent* event);
public slots:
    void joinAsPlayer();
    void joinAsWatcher();
    void screenshot();
    void capitulate();
    void quitGame();
    void setUserName();
    void sendMsg();
    void winDialog();
    void failDialog();
    void recCapitulate();
    void gameEnd();

private:
    Ui::mainWindowClass ui;
    Client* client;
    Chess* chessboard;
};
