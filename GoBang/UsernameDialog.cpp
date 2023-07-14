#include "UsernameDialog.h"

UsernameDialog::UsernameDialog(QWidget *parent) {
	dialog = new QDialog(this);
	dialog->setWindowTitle("Username");
	dialog->setFixedSize(200, 150);
	
	lineEdit = new QLineEdit(dialog);
	lineEdit->setPlaceholderText("Input username...");
	lineEdit->setGeometry(25, 10, 120, 20);

	button = new QPushButton(dialog);
	button->setText("OK");
	connect(button, &QPushButton::clicked, this, &UsernameDialog::setUsername);

	QVBoxLayout *layout = new QVBoxLayout(dialog);
	layout->addWidget(lineEdit);
	layout->addWidget(button);

	dialog->setLayout(layout);	
}

UsernameDialog::~UsernameDialog()
{}

void UsernameDialog::showDialog() {
	dialog->exec();
}

void UsernameDialog::setUsername() {
	if (lineEdit->text().isEmpty()) {
		QMessageBox::information(this, "Warning!", "Username cannot be empty!", QMessageBox::Ok);
		return;
	}
	QString username = lineEdit->text();
	dialog->close();
	this->username = username;
}

QString UsernameDialog::getUsername() {
	return this->username;
}