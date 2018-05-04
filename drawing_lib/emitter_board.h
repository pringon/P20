#ifndef EMITTER_BOARD_H
#define EMITTER_BOARD_H

#include <iostream>
#include <QApplication>
#include <QWidget>
#include <QMouseEvent>
#include <QCloseEvent>
#include <QPaintEvent>
#include <QPoint>
#include <QColor>
#include <QPen>
#include <QImage>
#include <QPainter>

class EmitterBoard : public QWidget {
  Q_OBJECT

public:
  EmitterBoard(QWidget *parent = 0);

private:
  QPoint start_point;
  QPoint end_point;
  QImage *image;

protected slots:
  virtual void mousePressEvent(QMouseEvent *);
  virtual void mouseMoveEvent(QMouseEvent *);
  virtual void mouseReleaseEvent(QMouseEvent *);
  virtual void closeEvent(QCloseEvent *);
  virtual void paintEvent(QPaintEvent *);
  //virtual void clearEvent();

signals:
  void paintLine(QPoint, QPoint);
};

#endif
