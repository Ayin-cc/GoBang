#include "Chess.h"
Chess::Chess(QWidget* parent)
	: QWidget(parent) {
	parentWidget = parent;
	this->setGeometry(0, 0, 481, 481);
	this->setMouseTracking(true);
	isNext = false;
	memset(board, 0, sizeof(board));
}

Chess::~Chess()
{}

//// 监听鼠标左键是否按下
//void Chess::mousePressEvent(QMouseEvent* event) {
//    qDebug() << "12312321312321321";
//        
//	if (event->button() & Qt::LeftButton) {
//		QPoint site = preChess(event->pos());
//		if (!site.isNull()) {
//			point = site;
//			emit mousePress();			
//		}
//	}
//}
//
//// 棋子准备落下
//QPoint Chess::preChess(QPoint point) {
//	int x = point.x();
//	int y = point.y();
//	int dx = x % 30;
//	int dy = y % 30;
//	if (dx > 13 && dx < 17 || dy > 13 && dy < 17) return QPoint();
//	else {
//		x = x / 30;
//		y = y / 30;
//		if (dx > 17) x++;
//		if (dy > 17) y++;
//	}
//
//	return QPoint(x, y);
//}

QPoint Chess::getMouseSite() {
	QPoint point = this->mapFromGlobal(QCursor::pos());
	return point;
}

bool isValid(int x, int y) {
    if (x < 1 || y < 1 || x > 15 || y > 15) return false;
    else return true;
}

void Chess::sendSignals(int chess) {
    if (chess == 1) emit win();
    if (chess == -1) emit fail();
}

void Chess::judge(int x, int y, int chess) {
    // 检查横向方向
    int count = 0;
    for (int i = x - 4; i < x + 5; ++i) {
        if (!isValid(i, y)) continue;
        if (board[i][y] == chess) {
            ++count;
        }
        else {
            count = 0;
        }
        if (count >= 5) {
            sendSignals(chess);
            return;
        }
    }

    // 检查纵向方向
    count = 0;
    for (int j = y - 4; j < y + 5; ++j) {
        if (!isValid(x, j)) continue;
        if (board[x][j] == chess) {
            ++count;
        }
        else {
            count = 0;
        }
        if (count >= 5) {
            sendSignals(chess);
            return;
        }
    }

    // 检查左上到右下的对角线方向
    count = 0;
    for (int k = -4; k < 5; ++k) {
        int x = x + k;
        int y = y + k;
        if (!isValid(x, y)) continue;
        if (board[x][y]) {
            ++count;
        }
        else {
            count = 0;
        }
        if (count >= 5) {
            sendSignals(chess);
            return;
        }
    }


    // 检查左下到右上的对角线方向
    count = 0;
    for (int k = -4; k < 5; ++k) {
        int x = x + k;
        int y = y - k;
        if (!isValid(x, y)) continue;
        if (board[x][y] == chess) {
            ++count;
        }
        else {
            count = 0;
        }
        if (count >= 5) {
            sendSignals(chess);
            return;
        }
    }
}


