#include "emitter_board.h"

EmitterBoard::EmitterBoard(QWidget *parent) : QWidget(parent) {
  image = new QImage(this->size(), QImage::Format_RGB32);
  image->fill(QColor(255, 255, 255));

  this->setMaximumSize(300, 400);
  this->setMinimumSize(300,400);
  this->setWindowTitle("Send");
}

void EmitterBoard::mousePressEvent(QMouseEvent *event) {

  if(event->button() == Qt::LeftButton) {
    start_point = event->pos();
    end_point = start_point;
    this->setMouseTracking(true);


    QPainter painter(image);
    QPen pen;
    pen.setWidth(4);
    painter.setPen(pen);
    painter.setBrush(Qt::black);
    painter.drawLine(this->start_point, this->start_point);
    painter.end();
    emit paintLine(start_point, start_point);
    this->update();
  }
}

void EmitterBoard::mouseMoveEvent(QMouseEvent *event) {

  end_point = event->pos();

  QPainter painter(image);
  QPen pen;
  pen.setWidth(4);
  painter.setPen(pen);
  painter.setBrush(Qt::black);
  painter.drawLine(this->start_point, this->end_point);
  painter.end();
  emit paintLine(start_point, end_point);
  this->update();

  start_point = end_point;
}

void EmitterBoard::mouseReleaseEvent(QMouseEvent *event) {

  this->setMouseTracking(false);
}

void EmitterBoard::closeEvent(QCloseEvent *event) {
  digitalWrite(7, HIGH);
  QApplication::quit();
}

void EmitterBoard::paintEvent(QPaintEvent *event) {

  QPainter painter(this);
  painter.drawImage(0, 0, *image);
  painter.end();
}
