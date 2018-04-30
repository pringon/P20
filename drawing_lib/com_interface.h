#ifndef COM_INTERFACE_H
#define COM_INTERFACE_H

#include <bitset>
#include <utility>
#include <vector>
#include <pthread.h>
#include <wiringPi.h>
#include <QObject>
#include "emitter_board.h"
#include "receiver_board.h"

class ComInterface : public QObject{
  Q_OBJECT
public:
  ComInterface(QWidget *parent = 0);
  static void* sendHandler_wrapper(void *);
  static void* receiveHandler_wrapper(void *);

private:
  std::vector<std::pair<QPoint, QPoint>> post_queue;
  pthread_t send_thread;
  EmitterBoard send;
  ReceiverBoard receive;

  void sendInteger(int);
  int receiveInteger();
  void sendLine();
  void receiveLine();
  void sendHandler(void *);
  void receiveHandler(void *);

protected slots:
  void postLine(QPoint, QPoint);
};

#endif
