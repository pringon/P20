#include "receiver_board.h"

ReceiverBoard::ReceiverBoard(QWidget *parent) : QWidget(parent) {
  image = new QImage(this->size(), QImage::Format_RGB32);
  image->fill(QColor(255, 255, 255));

  this->setMaximumSize(300, 400);
  this->setMinimumSize(300,400);
  this->setWindowTitle("Receive");

  /*connect(send, &EmitterBoard::paintLine,
          this,  &ReceiverBoard::lineReceived);*/
}

void ReceiverBoard::lineReceived(QPoint start_point, QPoint end_point) {

  QPainter painter(image);
  QPen pen;
  pen.setWidth(4);
  painter.setPen(pen);
  painter.setBrush(Qt::black);
  painter.drawLine(start_point, end_point);
  painter.end();
  this->update();
}

void ReceiverBoard::closeEvent(QCloseEvent *event) {
  QApplication::quit();
}

void ReceiverBoard::paintEvent(QPaintEvent *event) {

  QPainter painter(this);
  painter.drawImage(0, 0, *image);
  painter.end();
}
