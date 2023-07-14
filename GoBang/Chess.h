#pragma once

#include <QWidget>
#include <QMainWindow>
#include <QDir>
#include <QImage>
#include <QLabel>
#include <QMouseEvent>

class Chess  : public QWidget
{
	Q_OBJECT

public:
	Chess(QWidget *parent = nullptr);
	~Chess();

	int round;
	QString chess;
	int x;
	int y;
	QPoint point;
	bool isNext;
	int board[16][16];

	//QPoint preChess(QPoint point) ;
	QWidget* parentWidget;
	QPoint getMouseSite();
	void judge(int x, int y, int chess);

	//void mousePressEvent(QMouseEvent* event);
	void sendSignals(int chess);

signals:
	void mousePress();
	void win();
	void fail();
};
