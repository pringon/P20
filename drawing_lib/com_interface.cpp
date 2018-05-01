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
  pullUpDnControl(0, PUD_UP);
  pullUpDnControl(1, PUD_UP);
  pullUpDnControl(4, PUD_UP);
  pullUpDnControl(6, PUD_UP);
  pullUpDnControl(2, PUD_UP);
  pullUpDnControl(3, PUD_UP);
  pullUpDnControl(5, PUD_UP);
  pullUpDnControl(7, PUD_UP);
  digitalWrite(TX_OUT, HIGH);
  digitalWrite(RX_OUT, HIGH);

  digitalWrite(CONNECT_OUT, LOW);
  while(digitalRead(CONNECT_IN));

  connect(&send, &EmitterBoard::paintLine,
          this,  &ComInterface::add_to_queue);

  send_queue = (queue*) malloc(sizeof(queue));
  send_queue->next = NULL;
  current_element = send_queue;

  pthread_create(&send_thread, NULL, &ComInterface::sendHandler_wrapper, this);
  pthread_create(&receive_thread, NULL, &ComInterface::receiveHandler_wrapper, this);

  send.show();
  receive.show();
}

void ComInterface::add_to_queue(QPoint start_point, QPoint end_point) {

  send_queue->start = start_point;
  send_queue->end   = end_point;
  send_queue->next = (queue*) malloc(sizeof(queue));
  send_queue->next->prev = send_queue;
  send_queue = send_queue->next;
  send_queue->next = NULL;
}

void ComInterface::send_integer(int int_to_send) {

  std::bitset<32> bits_to_send(int_to_send);
  for(int i = 0; i < 32; i++) {

    digitalWrite(DATA_OUT, bits_to_send[i]);
    delay(5);
    digitalWrite(TX_OUT, LOW);

    while(digitalRead(RX_IN));
    digitalWrite(TX_OUT, HIGH);
  }
  std::cout<<"Data sent!";
}

int ComInterface::receive_integer() {

  std::bitset<32> bits_to_receive;
  std::cout<<"Primesc";
  for(int i = 0; i < 32; i++) {

    while(digitalRead(TX_IN));

    bits_to_receive[i] = digitalRead(DATA_IN);
    digitalWrite(RX_OUT, LOW);
    delay(5);
    digitalWrite(RX_OUT, HIGH);
  }

  int int_to_receive = (int)(bits_to_receive.to_ulong());
  std::cout<<int_to_receive;
  return int_to_receive;
}

void ComInterface::send_line() {

  std::cout<<'1';

  if(current_element->start.rx() != 0) {
    send_integer(current_element->start.rx());
    send_integer(current_element->start.ry());
    send_integer(current_element->end.rx());
    send_integer(current_element->end.ry());
  }
  std::cout<<'1';
  current_element = current_element->next;
  std::cout<<'2';
  free(current_element->prev);
  std::cout<<"S-A TRIMIS!";
}

void ComInterface::receive_line() {

  int x, y;

  x = receive_integer();
  y = receive_integer();
  QPoint start(x, y);
  std::cout<<std::endl;

  x = receive_integer();
  y = receive_integer();
  QPoint end(x, y);
  std::cout<<std::endl;

  receive.lineReceived(start, end);
}

void ComInterface::sendHandler(void *thread_args) {

  while(1) {

    while(current_element->next == NULL) {
      delay(1);
    }

    send_line();
  }
}

void ComInterface::receiveHandler(void *thread_args) {

  while(1) {

    receive_line();
  }
}

void *ComInterface::sendHandler_wrapper(void *object) {
  reinterpret_cast<ComInterface*>(object)->sendHandler(object);
}

void *ComInterface::receiveHandler_wrapper(void *object) {
  reinterpret_cast<ComInterface*>(object)->receiveHandler(object);
}
