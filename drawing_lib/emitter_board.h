#ifndef EMITTER_BOARD_H
#define EMITTER_BOARD_H

#include <iostream>
#include <stdlib.h>
#include <pthread.h>
#include <mutex>
#include <QString>
#include <QLineEdit>
#include <QWidget>
#include <QSizePolicy>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QColorDialog>
#include <QPoint>
#include <QColor>
#include <QPen>
#include <QImage>
#include <QPainter>
#include "queue.h"

class MainWindow;

class EmitterBoard : public QWidget {
  Q_OBJECT
  friend class MainWindow;

public:
  EmitterBoard(QWidget *parent = 0);
  static void *painting_handler_wrapper(void *);

private:
  pthread_t paint_thread;
  std::mutex line_queue_mutex_m;
  std::mutex line_queue_mutex_n;
  std::mutex line_queue_mutex_l;
  queue *line_queue;
  queue *current_line;
  QPoint start_point;
  QPoint end_point;
  QImage *image;
  QColor current_color;
  int line_width;
  void append_line();
  void paint_line();
  void painting_handler(void *);

protected slots:
  virtual void mousePressEvent(QMouseEvent *);
  virtual void mouseMoveEvent(QMouseEvent *);
  virtual void mouseReleaseEvent(QMouseEvent *);
  virtual void paintEvent(QPaintEvent *);
  virtual void clearEvent();
  virtual void colorPickEvent();
  virtual void widthChangedEvent();

signals:
  void line_painted(QPoint, QPoint, QColor, int);
  void board_cleared();
};

#endif
