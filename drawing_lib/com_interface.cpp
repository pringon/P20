#include "com_interface.h"

ComInterface::ComInterface(QWidget *parent) {

  wiringPiSetup();
  pinMode(0, OUTPUT);
  pinMode(1, OUTPUT);
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  pinMode(4, OUTPUT);
  /*pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);*/
  pinMode(12, INPUT);
	/*pinMode(13, INPUT);
	pinMode(14, INPUT);
	pinMode(15, INPUT);
	pinMode(16, INPUT);
	pinMode(21, INPUT);
	pinMode(22, INPUT);
	pinMode(23, INPUT);*/
  digitalWrite(0, HIGH);
  digitalWrite(1, HIGH);

  connect(&send, &EmitterBoard::paintLine,
          this, &ComInterface::postLine);
  /*QObject::connect(this, &ComInterface::lineReceived,
          receive, &ReceiverBoard::lineReceived);*/

  pthread_create(&send_thread, NULL, &ComInterface::sendHandler_wrapper, this);

  send.show();
  receive.show();
}

void ComInterface::receiveHandler(void *thread_args) {

  while(1) {

    while(digitalRead(2));

    ComInterface::receiveLine();
  }
}

void ComInterface::sendHandler(void *thread_args) {

  while(1) {

    while(post_queue.size() == 0);

    ComInterface::sendLine();
  }
}

void receiveLine() {

  int x, y;

  x = receiveInteger();
  y = receiveInteger();
  QPoint start(x, y);
  x = receiveInteger();
  y = receiveInteger();
  QPoint end(x, y);

  receiver.lineReceived(start, end);
}

void ComInterface::sendLine() {

  std::pair<QPoint, QPoint> line_to_send = post_queue.back();
  post_queue.pop_back();

  if(line_to_send->first.rx() == 0) {
    return;
  }

  //std::bitset<32> b(line_to_send->first.rx());

  digitalWrite(0, LOW);
  delay(1);
  digitalWrite(0, HIGH);

  sendInteger(line_to_send->first.rx());
  sendInteger(line_to_send->first.ry());
  sendInteger(line_to_send->second.rx());
  sendInteger(line_to_send->second.ry());
  /*for(int i = 0; i < 4; i++) {
    digitalWrite(4, b[i]);
    digitalWrite(5, b[i+1]);
    digitalWrite(6, b[i+2]);
    digitalWrite(7, b[i+3]);
    digitalWrite(8, b[i+4]);
    digitalWrite(9, b[i+5]);
    digitalWrite(10, b[i+6]);
    digitalWrite(11, b[i+7]);

    delay(1);
    digitalWrite(0, LOW);
    delay(1);

    while(digitalRead(3));
    digitalWrite(0, HIGH);
  }
  digitalWrite(0, HIGH);

  b = std::bitset<32>(line_to_send->first.ry());
  for(int i = 0; i < 4; i++) {
    digitalWrite(4, b[i]);
    digitalWrite(5, b[i+1]);
    digitalWrite(6, b[i+2]);
    digitalWrite(7, b[i+3]);
    digitalWrite(8, b[i+4]);
    digitalWrite(9, b[i+5]);
    digitalWrite(10, b[i+6]);
    digitalWrite(11, b[i+7]);

    delay(1);
    digitalWrite(0, LOW);
    delay(1);

    while(digitalRead(3));
    digitalWrite(0, HIGH);
  }
  digitalWrite(0, HIGH);

  b = std::bitset<32>(line_to_send->second.rx());
  for(int i = 0; i < 4; i++) {
    digitalWrite(4, b[i]);
    digitalWrite(5, b[i+1]);
    digitalWrite(6, b[i+2]);
    digitalWrite(7, b[i+3]);
    digitalWrite(8, b[i+4]);
    digitalWrite(9, b[i+5]);
    digitalWrite(10, b[i+6]);
    digitalWrite(11, b[i+7]);

    delay(1);
    digitalWrite(0, LOW);
    delay(1);

    while(digitalRead(3));
    digitalWrite(0, HIGH);
  }
  digitalWrite(0, HIGH);

  b = std::bitset<32>(line_to_send->second.ry());
  for(int i = 0; i < 4; i++) {
    digitalWrite(4, b[i]);
    digitalWrite(5, b[i+1]);
    digitalWrite(6, b[i+2]);
    digitalWrite(7, b[i+3]);
    digitalWrite(8, b[i+4]);
    digitalWrite(9, b[i+5]);
    digitalWrite(10, b[i+6]);
    digitalWrite(11, b[i+7]);

    delay(1);
    digitalWrite(0, LOW);
    delay(1);

    while(digitalRead(3));
    digitalWrite(0, HIGH);
  }
  digitalWrite(0, HIGH);
  */
  //emit ComInterface::lineReceived(line_to_send->first, line_to_send->second);

  //post_queue.pop_back();
}

void ComInterface::postLine(QPoint start, QPoint end) {
  post_queue.insert(post_queue.begin(),
                  std::make_pair(start, end));

  //cand e this sender cheama functia de aici, altfel seteaza sa fie chemata aia static(adica fara obiect, uitete in documentatie pentru qconnect).
  //mersi
}

void sendInteger(int int_to_send) {

  std::bitset<32> bits_to_send(int_to_send);
  for(int i = 0; i < 32; i++) {

    digitalWrite(4, bits_to_send[i]);
    std::cout<<bits_to_send[i]);
    /*digitalWrite(5, bits_to_send[i+1]);
    std::cout<<bits_to_send[i+1]);
    digitalWrite(6, bits_to_send[i+2]);
    std::cout<<bits_to_send[i+2]);
    digitalWrite(7, bits_to_send[i+3]);
    std::cout<<bits_to_send[i+3]);
    digitalWrite(8, bits_to_send[i+4]);
    std::cout<<bits_to_send[i+4]);
    digitalWrite(9, bits_to_send[i+5]);
    std::cout<<bits_to_send[i+5]);
    digitalWrite(10, bits_to_send[i+6]);
    std::cout<<bits_to_send[i+6]);
    digitalWrite(11, bits_to_send[i+7]);
    std::cout<<bits_to_send[i+7])<<std::endl;*/

    delay(1);
    digitalWrite(0, LOW);
    delay(1);
    while(digitalRead(3));
    digitalWrite(0, HIGH);
  }
}

int receiveInteger() {

  std::bitset<32> bits_to_receive;
  for(int i = 0; i < 32; i++) {

    bits_to_receive[i] = digitalRead(12);
    std::cout<<bits_to_send[i];
    /*bits_to_receive[i+1] = digitalRead(13);
    std::cout<<bits_to_send[i+1];
    bits_to_receive[i+2] = digitalRead(14);
    std::cout<<bits_to_send[i+2];
    bits_to_receive[i+3] = digitalRead(15);
    std::cout<<bits_to_send[i+3];
    bits_to_receive[i+4] = digitalRead(16);
    std::cout<<bits_to_send[i+4];
    bits_to_receive[i+5] = digitalRead(21);
    std::cout<<bits_to_send[i+5];
    bits_to_receive[i+6] = digitalRead(22);
    std::cout<<bits_to_send[i+6];
    bits_to_receive[i+7] = digitalRead(23);
    std::cout<<bits_to_send[i+7]<<std::endl;*/

    delay(1);
    digitalWrite(1, LOW);
    delay(1);
    while(digitalRead(2));
    digitalWrite(1, HIGH);
  }

  return (int)(b.to_ulong());
}

void *ComInterface::sendHandler_wrapper(void *object) {
  reinterpret_cast<ComInterface*>(object)->sendHandler(object);
}

void *ComInterface::receiveHandler_wrapper(void *object) {
  reinterpret_cast<ComInterface*>(object)->receiveHandler(object);
}
