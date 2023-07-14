#pragma once

#include <QWidget>
#include <QDialog>
#include <QPushButton>
#include <QLineEdit>
#include <QLayout>
#include <QMessageBox>
#include <QPainter>

class UsernameDialog  : public QDialog
{
	Q_OBJECT

public:
	UsernameDialog(QWidget *parent);
	~UsernameDialog();
	void showDialog();
	QString getUsername();

public slots:
	void setUsername();

private:
	QDialog* dialog;
	QString username;
	QLineEdit* lineEdit;
	QPushButton* button;
};
