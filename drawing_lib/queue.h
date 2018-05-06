#ifndef QUEUE_H
#define QUEUE_H

#include <QPoint>

typedef struct queue_struct {
  QPoint start;
  QPoint end;
  QColor color;
  int width;
  struct queue_struct *next;
  struct queue_struct *prev;
} queue;

#endif
