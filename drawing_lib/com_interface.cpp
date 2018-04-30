#include "com_interface.h"

ComInterface::ComInterface(QWidget *parent) {

  wiringPiSetup();
  pinMode(0, INPUT);
  pinMode(1, INPUT);
  pinMode(4, INPUT);
  pinMode(6, INPUT);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(7, OUTPUT);
  pullUpDnControl(2, PUD_UP);
  pullUpDnControl(3, PUD_UP);
  pullUpDnControl(5, PUD_UP);
  pullUpDnControl(7, PUD_UP);

  digitalWrite(CONNECT_OUT, HIGH);
  while(digitalRead(CONNECT_IN));

  connect(&send, &EmitterBoard::paintLine,
          this,  &ComInterface::add_to_queue);

  pthread_create(&send_thread, NULL, &ComInterface::sendHandler_wrapper, this);
  pthread_create(&receive_thread, NULL, &ComInterface::receiveHandler_wrapper, this);

  send_queue = (queue*) malloc(sizeof(queue));
  send_queue->next = 0;
  current_element->next = send_queue;

  send.show();
  receive.show();
}

void ComInterface::add_to_queue(QPoint start_point, QPoint end_point) {

  send_queue->start = start_point;
  send_queue->end   = end_point;
  send_queue->next = (queue*) malloc(sizeof(queue));
  send_queue->prev = send_queue;
  send_queue = send_queue->next;
  send_queue->next = 0;
}

void ComInterface::send_integer(int int_to_send) {

  std::bitset<32> bits_to_send(int_to_send);
  for(int i = 0; i < 32; i++) {

    digitalWrite(DATA_OUT, bits_to_send[i]);
    digitalWrite(TX_OUT, LOW);
    delay(100);
    while(digitalRead(RX_IN));
    digitalWrite(TX_OUT, HIGH);
  }
}

int ComInterface::receive_integer() {

  std::bitset<32> bits_to_receive;
  for(int i = 0; i < 32; i++) {

    bits_to_receive[i] = digitalRead(DATA_IN);
    digitalWrite(RX_OUT, LOW);
    delay(100);
    while(digitalRead(TX_IN));
    digitalWrite(RX_OUT, HIGH);
  }

  int int_to_send = (int)(bits_to_send.to_ulong());
  std::cout<<int_to_send;
  return int_to_send;
}

void ComInterface::send_line() {

  send_integer(current_element->start.rx());
  send_integer(current_element->start.ry());
  send_integer(current_element->end.rx());
  send_integer(current_element->end.ry());

  current_element = current_element->next;
  free(current_element->prev);
}

void ComInterface::receive_line() {

  int x, y;

  x = receive_integer();
  y = receive_integer();
  QPoint start(x, y);

  x = receive_integer();
  y = receive_integer();
  QPoint end(x, y);

  receive.lineReceived(start, end);
}

void ComInterface::sendHandler(void *thread_args) {

  while(1) {

    while(send_queue->next == 0);

    send_line();
  }
}

void ComInterface::receiveHandler(void *thread_args) {

  while(1) {

    while(digitalRead(TX_IN));

    receive_line();
  }
}

void *ComInterface::sendHandler_wrapper(void *object) {
  reinterpret_cast<ComInterface*>(object)->sendHandler(object);
}

void *ComInterface::receiveHandler_wrapper(void *object) {
  reinterpret_cast<ComInterface*>(object)->receiveHandler(object);
}
