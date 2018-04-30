#ifndef QUEUE_H
#define QUEUE_H

#include <QPoint>

typedef struct {
  QPoint start;
  QPoint end;
  queue *next;
  queue *prev;
} queue;

#endif
