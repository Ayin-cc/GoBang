#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_GoBangServe.h"
#include "Server.h"

class GoBangServe : public QMainWindow
{
    Q_OBJECT

public:
    GoBangServe(QWidget *parent = nullptr);
    ~GoBangServe();
       
public slots:
    void refresh();

private:
    Ui::GoBangServeClass ui;
    Server* myServer;
};
