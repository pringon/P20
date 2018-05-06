#ifndef RECEIVER_BOARD_H
#define RECEIVER_BOARD_H

#include <iostream>
#include <QApplication>
#include <QWidget>
#include <QCloseEvent>
#include <QPaintEvent>
#include <QPoint>
#include <QColor>
#include <QPen>
#include <QImage>
#include <QPainter>
//#include <wiringPi.h>

class ReceiverBoard : public QWidget {
  Q_OBJECT
  friend class ComInterface;

public:
  ReceiverBoard(QWidget *parent = 0);

private:
  QImage *image;

protected:
  void lineReceived(QPoint, QPoint, QColor);
  void clear_screen();

protected slots:
  virtual void closeEvent(QCloseEvent *);
  virtual void paintEvent(QPaintEvent *);
};


#endif
