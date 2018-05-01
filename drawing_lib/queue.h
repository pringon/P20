#ifndef QUEUE_H
#define QUEUE_H

#include <QPoint>

typedef struct queue_struct {
  QPoint start;
  QPoint end;
  struct queue_struct *next;
  struct queue_struct *prev;
} queue;

#endif
