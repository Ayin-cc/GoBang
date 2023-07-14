#include "mainWindow.h"


mainWindow::mainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    chessboard = new Chess(ui.widgetBoard);
    client = new Client(this);   

    QPixmap pixmap("resources/black.png");
    wlabel = new QLabel(ui.widgetInformation);
    wlabel->setPixmap(pixmap);
    wlabel->setGeometry(200, 115, 30, 30);
    wlabel->setStyleSheet("border: 0px;");

    pixmap = QPixmap("resources/white.png");
    blabel = new QLabel(ui.widgetInformation);
    blabel->setPixmap(pixmap);
    blabel->setGeometry(200, 80, 30, 30);
    blabel->setStyleSheet("border: 0px;");

    ui.listWidget->setStyleSheet("QListWidget::item:hover { background-color: transparent; }");

    // ���Ӳ˵������ź���ۺ���
    // Join
    connect(ui.actionPlayer, &QAction::triggered, this, &mainWindow::joinAsPlayer);  
    connect(ui.actionWatcher, &QAction::triggered, this, &mainWindow::joinAsWatcher);  
    // Game
    connect(ui.actionScreenshot, &QAction::triggered, this, &mainWindow::screenshot);
    connect(ui.actionCapitulate, &QAction::triggered, this, &mainWindow::capitulate);
    connect(ui.actionQuit, &QAction::triggered, this, &mainWindow::quitGame);
    // Setting
    connect(ui.actionName, &QAction::triggered, this, &mainWindow::setUserName);   

    // ���ӷ�����Ϣ��ť�Ĳۺ���
    connect(ui.buttonSendMsg, &QPushButton::clicked, this, &mainWindow::sendMsg);

    // ������Ϣ����Ĳۺ���
    connect(client, &Client::playerRecived, this, &mainWindow::setPlayer);
    connect(client, &Client::textRecived, this, &mainWindow::setNewText);
    connect(client, &Client::roundRecived, this, &mainWindow::setRound);
    connect(client, &Client::fightRecived, this, &mainWindow::setFight);
    connect(client, &Client::playerEnough, this, &mainWindow::gameProcess);

    // �����������Ĳۺ���
    connect(chessboard, &Chess::mousePress, this, &mainWindow::drawPiece);

    connect(chessboard, &Chess::win, this, &mainWindow::winDialog);
    connect(chessboard, &Chess::fail, this, &mainWindow::failDialog);
    connect(client, &Client::isCapitulate, this, &mainWindow::recCapitulate);
    connect(client, &Client::gameEnd, this, &mainWindow::gameEnd);

    // ��ʼ����Ϸ
    initWindow();
}

mainWindow::~mainWindow()
{}

QPoint preChess(QPoint point) {
    int x = point.x() - 10;
    int y = point.y() - 30;
    int dx = x % 30;
    int dy = y % 30;
    if (dx > 13 && dx < 17 || dy > 13 && dy < 17) return QPoint();
    else {
        x = x / 30;
        y = y / 30;
        if (dx > 17) x++;
        if (dy > 17) y++;
    }
    return QPoint(x, y);
}

void mainWindow::mousePressEvent(QMouseEvent* event) {
    qDebug() << event->pos();
    if (event->button() & Qt::LeftButton) {
        QPoint site = preChess(event->pos());
        if (!site.isNull()) {
            chessboard->point = site;
            emit chessboard->mousePress();
        }
    }
}
// ��ʼ��
void mainWindow::initWindow() {
    // ���ý�ͼ������ѡ��
    ui.actionScreenshot->setEnabled(false);
    ui.actionCapitulate->setEnabled(false);

    // ������������
    client->connectToServer("43.249.192.204", 27333);

    // ��Ⱦ��Ϣ��
    client->sendToServer(Client::Request, Client::Round);
    ui.labelUserName->setText("Your name: " + client->getUsername());
    client->sendToServer(Client::Request, Client::Player);

    client->chess = chessboard->chess;
    chessboard->show();
    memset(chessboard->board, 0, sizeof(chessboard->board));
}

// ˢ�½���
void mainWindow::refresh() {
    // �غ���
    setRound();

    // ��ȡ�û���
    QString label = "Your name: " + client->getUsername();
    ui.labelUserName->setText(label);

    // ����Ծ��е������Ϣ
    client->sendToServer(Client::Request, Client::Player);   
}

void mainWindow::setPlayer() {
    QString player1 = client->player1;
    QString player2 = client->player2;

    // ˢ����Ϣ��
    QString label1 = "PLAYER1: " + client->player1;
    QString label2 = "PLAYER2: " + client->player2;
    ui.labelP1Name->setText(label1);
    ui.labelP2Name->setText(label2);
}

void mainWindow::setNewText() {
    QString text = client->newText;
    QListWidgetItem* item = new QListWidgetItem();

    item->setText(text);
    item->setSizeHint(QSize(ui.listWidget->width() - 5, 20));
    item->setFlags(item->flags() & ~Qt::ItemIsSelectable);
    
    ui.listWidget->addItem(item);
}

void mainWindow::setFight() {
    QString fight = client->fight;
    QStringList list = fight.split(";");

    if (list[0] == chessboard->chess) return;

    QPixmap pixmap("resources/" + list[0] + ".png");
    int x = list[1].toInt();
    int y = list[2].toInt();
    qDebug() << x << y;
    QLabel* label = new QLabel(chessboard);
    pieces.append(label);
    label->setPixmap(pixmap);
    label->setGeometry(x * 30 - 10, y * 30 - 10, 21, 21);
    label->setStyleSheet("border: 0px;");
    label->show();

    chessboard->isNext = true;
    chessboard->board[x][y] = -1;
    chessboard->judge(x, y, -1);
    client->round++;
    setRound();

    hint(list[0]);
}

void mainWindow::setRound() {
    QString round = QString::number(client->round);

    // ˢ����Ϣ��
    QString label = "Round: " + round;
    ui.label->setText(label);    
}

// ������ֵĲۺ���
void mainWindow::joinAsPlayer() {
    refresh();
    // �û����Ƿ����
    if (client->getUsername().isEmpty()) {
        setUserName();
    }
    if (client->getUsername().isEmpty())return;

    // �������Ƿ�����
    if (client->isPlayerEnough) {
        QMessageBox::information(this, "Failed", "Players are full! Now you will be the audience.");
        joinAsWatcher();
        return;
    }

    // �˵���ť���������
    ui.actionPlayer->setEnabled(false);
    if (!ui.actionWatcher->isEnabled()) ui.actionWatcher->setEnabled(true);
    ui.actionScreenshot->setEnabled(true);
    ui.actionCapitulate->setEnabled(true);

    // ѡ��p1��p2
    if (client->player1.isEmpty()) {
        chessboard->chess = "white";
        client->chess = chessboard->chess;
        chessboard->isNext = false;
    }
    else {
        chessboard->chess = "black";
        client->chess = chessboard->chess;
        chessboard->isNext = false;
    }
    
    client->isPlayer = true;
    client->sendToServer(Client::Player, client->getUsername());
    refresh();
}

// ��ս�Ĳۺ���
void mainWindow::joinAsWatcher() {
    // �û����Ƿ����
    if (client->getUsername().isEmpty()) {
        setUserName();
    }
    if (client->getUsername().isEmpty())return;

    ui.actionCapitulate->setEnabled(false);
    ui.actionWatcher->setEnabled(false);
    if (!ui.actionPlayer->isEnabled()) ui.actionPlayer->setEnabled(true);
    ui.actionScreenshot->setEnabled(true);

    client->isPlayer = false;
    ui.widgetBoard->setAttribute(Qt::WA_TransparentForMouseEvents);
    client->sendToServer(Client::Watcher, client->getUsername());
    refresh();
}

// ��ͼ�Ĳۺ���
void mainWindow::screenshot() {
    QScreen* screen = QGuiApplication::screenAt(QCursor::pos());
    QPixmap pixmap = screen->grabWindow(this->winId());
    int n = 1;
    while(true) {
        QFileInfo info("screenshots/screenshot" + QString::number(n) + ".png");
        if (!info.exists()) {
            pixmap.save(QString("screenshots/screenshot" + QString::number(n) + ".png"));
            break;
        }
        n++;
    }
}

// ����Ĳۺ���
void mainWindow::capitulate() {
    client->sendToServer("capitulate", client->getUsername());
}

void mainWindow::recCapitulate() {
    if (!client->isPlayer) {
        gameEnd();
    }
    QString name = client->capitulate;
    qDebug() << name;
    if (name == client->getUsername()) {
        failDialog();
    }
    else if(name == client->player2 || name == client->player1) {
        winDialog();
    }
    else {
        gameEnd();
    }
}

// �˳��Ĳۺ���
void mainWindow::quitGame() {
    emit client->onDisconnected();
    this->close();
}

// �����û����Ĳۺ���
void mainWindow::setUserName() {
    UsernameDialog dialog(this);
    dialog.showDialog();
    client->setUsername(dialog.getUsername());
    ui.labelUserName->setText("Your name: " + client->getUsername());
}

// ������Ϣ�Ĳۺ���
void mainWindow::sendMsg() {
    QString text = ui.textEdit->toPlainText();
    client->sendToServer(Client::Text, text);
    ui.textEdit->clear();
}

void mainWindow::drawPiece() {
    QPoint point = chessboard->point;
    if (point.x() == 0 || point.y() == 0 || point.x() == 16 || point.y() == 16) return;
    if (!chessboard->isNext) return;
    if (!client->isPlayer) return;
    int x = point.x();
    int y = point.y();

    if (chessboard->board[x][y] == -1) return;
    
    qDebug() << x << y;
    
    // ����ͼƬ
    QPixmap pixmap("resources/" + chessboard->chess + ".png");
    QLabel* label = new QLabel(chessboard);
    pieces.append(label);
    label->setPixmap(pixmap);
    label->setGeometry(x * 30 - 10, y * 30 - 10, 21, 21);
    label->setStyleSheet("border: 0px;");
    label->show();

    hint(chessboard->chess);

    client->sendToServer(Client::Fight, chessboard->chess, x, y);
    chessboard->isNext = false;
    chessboard->board[x][y] = 1;
    chessboard->judge(x, y, 1);
    client->round++;
    setRound();
}

void mainWindow::hint(QString chess) {
    if (chess == "white") {
        blabel->hide();
        wlabel->show();
    }
    else {
        wlabel->hide();       
        blabel->show();
    }
}

void mainWindow::gameProcess() {
    if (chessboard->chess == "white") chessboard->isNext = true;

    client->round = 0;
    setRound();
    
    wlabel->hide();
    blabel->show();
}

void mainWindow::winDialog() {
    if (!client->isPlayer) {
        gameEnd();
        return;
    }
    QMessageBox::information(this, "Win", "Congratulate! You Win!");
    joinAsWatcher();
    gameEnd();
}

void mainWindow::failDialog() {
    if (!client->isPlayer) {
        gameEnd();
        return;
    }
    QMessageBox::information(this, QString("Fail"), QString("Oh! You Fail!"));
    joinAsWatcher();
    gameEnd();
}


void mainWindow::gameEnd() {
    wlabel->show();
    blabel->show();

    // ɾ�������ϵ�����
    for (QLabel* label : pieces) {
        label->setParent(nullptr);
        delete label;
    }
    pieces.clear();

    memset(chessboard->board, 0, sizeof(chessboard->board));
    chessboard->isNext = false;
    client->isPlayerEnough = false;
    client->isPlayer = false;
    client->player1 = nullptr;
    client->player2 = nullptr;
    client->round = 0;   
    this->setMouseTracking(true);

    refresh();
}