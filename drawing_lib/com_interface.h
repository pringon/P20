#ifndef COM_INTERFACE_H
#define COM_INTERFACE_H

#include <stdlib.h>
#include <bitset>
#include <utility>
#include <pthread.h>
#include <mutex>
#include <QObject>
#include "main_window.h"
#include "main_window.h"
#include "receiver_board.h"
#include "queue.h"

class ComInterface : public QObject{
  Q_OBJECT
public:
  ComInterface(QWidget *parent = 0);
  static void *sendHandler_wrapper(void *);
  static void *receiveHandler_wrapper(void *);

private:
  pthread_t send_thread;
  pthread_t receive_thread;
  std::mutex send_queue_mutex;
  queue *send_queue;
  queue *current_element;
  MainWindow send;
  ReceiverBoard receive;
  enum PINS { RX_IN, TX_IN, RX_OUT, TX_OUT, DATA_IN, DATA_OUT, CONNECT_IN, CONNECT_OUT };

  void send_integer(int);
  int  receive_integer();
  void send_line();
  void receive_line();
  void sendHandler(void *);
  void receiveHandler(void *);

protected slots:
  virtual void add_to_queue(QPoint, QPoint, QColor, int);
  virtual void clear_screen();
};

#endif
