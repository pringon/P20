#include "emitter_board.h"

EmitterBoard::EmitterBoard(QWidget *parent) : QWidget(parent) {

  this->setMaximumSize(300, 400);
  this->setMinimumSize(300,400);

  current_color = QColor(0, 0, 0);
  image = new QImage(this->size(), QImage::Format_RGB32);
  image->fill(QColor(255, 255, 255));
  line_width = 4;

  pthread_create(&paint_thread, NULL, *EmitterBoard::painting_handler_wrapper, this);
  line_queue       = (queue*) malloc(sizeof(queue));
  line_queue->next = NULL;
  current_line     = line_queue;
}

void EmitterBoard::append_line() {

  while(!line_queue_mutex_n.try_lock());
  while(!line_queue_mutex_m.try_lock());
  line_queue_mutex_n.unlock();

  line_queue->start = start_point;
  line_queue->end   = end_point;
  line_queue->next = (queue*) malloc(sizeof(queue));
  line_queue->next->prev = line_queue;
  line_queue = line_queue->next;
  line_queue->next = NULL;

  line_queue_mutex_m.unlock();
}

void EmitterBoard::paint_line() {

  int width  = this->width(),
      height = this->height(),
      x1     = current_line->start.rx(),
      y1     = current_line->start.ry(),
      x2     = current_line->end.rx(),
      y2     = current_line->end.ry();

  if(x1 <= width  && x1 >= 0
  && y1 <= height && y1 >= 0
  && x2 <= width  && x2 >= 0
  && y2 <= height && y2 >= 0) {

    QPainter painter(image);
    QPen pen;
    pen.setWidth(line_width);
    pen.setColor(current_color);
    painter.setPen(pen);
    painter.drawLine(this->start_point, this->end_point);
    painter.end();
    emit line_painted(start_point, end_point, current_color, line_width);
    this->update();
  }

  current_line = current_line->next;
  free(current_line->prev);
}

void EmitterBoard::painting_handler(void *thread_args) {

  while(1) {

    while(!line_queue_mutex_l.try_lock());
    while(!line_queue_mutex_n.try_lock());
    while(!line_queue_mutex_m.try_lock());
    line_queue_mutex_n.unlock();

    if(current_line->next != NULL) {
      this->paint_line();
    }

    line_queue_mutex_m.unlock();
    line_queue_mutex_l.unlock();
  }
}

void *EmitterBoard::painting_handler_wrapper(void *object) {
  reinterpret_cast<EmitterBoard*>(object)->painting_handler(object);
}

void EmitterBoard::mousePressEvent(QMouseEvent *event) {

  if(event->button() == Qt::LeftButton) {
    start_point = event->pos();
    end_point = start_point;
    this->setMouseTracking(true);

    this->append_line();
  }
}

void EmitterBoard::mouseMoveEvent(QMouseEvent *event) {

  end_point = event->pos();

  this->append_line();

  start_point = end_point;
}

void EmitterBoard::mouseReleaseEvent(QMouseEvent *event) {

  this->setMouseTracking(false);
}

void EmitterBoard::paintEvent(QPaintEvent *event) {

  QPainter painter(this);
  painter.drawImage(0, 0, *image);
  painter.end();
}

void EmitterBoard::clearEvent() {

    image->fill(QColor(255, 255, 255));

    emit line_painted(QPoint(0, 0), QPoint(0, 0));

    this->update();
}

void EmitterBoard::colorPickEvent() {

  QColor new_color = QColorDialog::getColor();
  if(new_color.isValid()) {
    current_color = new_color;
  }
}

void EmitterBoard::widthChangedEvent() {

  QLineEdit *sender_widget = dynamic_cast<QLineEdit*>(this->sender());

  bool conversion_success;
  int new_width = sender_widget->text().toInt(&conversion_success, 10);

  if(!conversion_success || new_width < 1 || new_width > 20) {

    sender_widget->setText(QString::number(line_width));
    return;
  }

  line_width = new_width;
}
