#ifndef COM_INTERFACE_H
#define COM_INTERFACE_H

#include <stdlib.h>
#include <bitset>
#include <utility>
#include <vector>
#include <pthread.h>
#include <wiringPi.h>
#include <QObject>
#include "emitter_board.h"
#include "receiver_board.h"
#include "queue.h"

class ComInterface : public QObject{
  Q_OBJECT
public:
  ComInterface(QWidget *parent = 0);
  static void* sendHandler_wrapper(void *);
  static void* receiveHandler_wrapper(void *);

private:
  pthread_t send_thread;
  pthread_t receive_thread;
  queue *send_queue;
  queue *current_element;
  EmitterBoard send;
  ReceiverBoard receive;
  enum PINS { RX_IN, TX_IN, RX_OUT, TX_OUT, DATA_IN, DATA_OUT, CONNECT_IN, CONNECT_OUT };

  void send_integer(int);
  int  receive_integer();
  void send_line();
  void receive_line();
  void sendHandler(void *);
  void receiveHandler(void *);

protected slots:
  void add_to_queue(QPoint, QPoint);
};

#endif
