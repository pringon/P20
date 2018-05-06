#include "com_interface.h"

ComInterface::ComInterface(QWidget *parent) {
  std::cout<<"Ajunge aici"<<std::endl;

  /*wiringPiSetup();
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
*/

  /*connect(this,  &ComInterface::clear_screen,
          &send, &EmitterBoard::clearEvent);*/
  connect(send.drawing_board, &EmitterBoard::line_painted,
          this,  &ComInterface::add_to_queue);
  connect(send.drawing_board, &EmitterBoard::board_cleared,
          this, &ComInterface::clear_screen);
  std::cout<<'2'<<std::endl;
  send_queue = (queue*) malloc(sizeof(queue));
  send_queue->next = NULL;
  current_element = send_queue;
  std::cout<<'3'<<std::endl;
  pthread_create(&send_thread, NULL, &ComInterface::sendHandler_wrapper, this);
  //pthread_create(&receive_thread, NULL, &ComInterface::receiveHandler_wrapper, this);
  std::cout<<'4'<<std::endl;
  send.show();
  receive.show();
  std::cout<<'5'<<std::endl;
}

void ComInterface::add_to_queue(QPoint start_point, QPoint end_point, QColor used_color, int line_width) {

  while(!send_queue_mutex.try_lock());
  send_queue->start = start_point;
  send_queue->end   = end_point;
  send_queue->color = used_color;
  send_queue->width = line_width;
  send_queue->next = (queue*) malloc(sizeof(queue));
  send_queue->next->prev = send_queue;
  send_queue = send_queue->next;
  send_queue->next = NULL;
  send_queue_mutex.unlock();
}

/*void ComInterface::send_integer(int int_to_send) {

  std::bitset<32> bits_to_send(int_to_send);
  for(int i = 0; i < 32; i++) {

    while(!digitalRead(RX_IN));

    digitalWrite(DATA_OUT, bits_to_send[i]);
    digitalWrite(TX_OUT, LOW);

    while(digitalRead(RX_IN));
    digitalWrite(TX_OUT, HIGH);
  }
  std::cout<<" Data sent!"<<'\n';
}*/

/*int ComInterface::receive_integer() {

  std::bitset<32> bits_to_receive;
  std::cout<<"Primesc ";
  for(int i = 0; i < 32; i++) {

    while(digitalRead(TX_IN));

    bits_to_receive[i] = digitalRead(DATA_IN);
    digitalWrite(RX_OUT, LOW);

    while(!digitalRead(TX_IN));
    digitalWrite(RX_OUT, HIGH);
  }

  int int_to_receive = (int)(bits_to_receive.to_ulong());
  std::cout<<int_to_receive<<'\n';
  return int_to_receive;
}*/

void ComInterface::send_line() {

  if(current_element->start.rx() != 0
  && (current_element->start.rx() != current_element->next->start.ry()
  || current_element->start.ry() != current_element->next->start.ry())) {

    receive.lineReceived(current_element->start, current_element->end, current_element->color, current_element->width);
    /*send_integer(current_element->start.rx());
    send_integer(current_element->start.ry());
    send_integer(current_element->end.rx());
    send_integer(current_element->end.ry());*/
  }

  current_element = current_element->next;
  free(current_element->prev);
}

void ComInterface::clear_screen() {

  receive.clear_screen();
}

/*void ComInterface::receive_line() {

  int x, y;

  x = receive_integer();
  y = receive_integer();
  QPoint start(x, y);

  x = receive_integer();
  y = receive_integer();
  QPoint end(x, y);

  if(start.rx() == start.ry()
    && start.ry() == end.rx()
    && end.rx()   == end.ry()
    && end.ry()   == 0) {

    emit clear_screen();
  } else {
    receive.lineReceived(start, end);
  }
}*/

void ComInterface::sendHandler(void *thread_args) {

  while(1) {

    while(!send_queue_mutex.try_lock());
    if(current_element->next != NULL) {
      send_line();
    }
    send_queue_mutex.unlock();
  }
}

/*void ComInterface::receiveHandler(void *thread_args) {

  while(1) {

    while(digitalRead(CONNECT_IN));

    receive_line();
  }
}*/

void *ComInterface::sendHandler_wrapper(void *object) {
  reinterpret_cast<ComInterface*>(object)->sendHandler(object);
}

//void *ComInterface::receiveHandler_wrapper(void *object) {
//  reinterpret_cast<ComInterface*>(object)->receiveHandler(object);
//}
