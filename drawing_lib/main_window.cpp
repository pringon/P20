#include "main_window.h"

MainWindow::MainWindow(QWidget *parent) : QWidget(parent) {

  this->setMaximumSize(300, 500);
  this->setMinimumSize(300, 500);
  this->setWindowTitle("Send");

  QVBoxLayout *windowLayout = new QVBoxLayout(this);
  windowLayout->setContentsMargins(0, 0, 0, 0);
  windowLayout->setSpacing(0);

  // Drawing board group placement
  drawing_board_group = new QGroupBox();
  QHBoxLayout *drawing_board_box = new QHBoxLayout(this);
  drawing_board = new EmitterBoard(this);
  QSizePolicy drawing_board_group_height(QSizePolicy::Preferred,
                                         QSizePolicy::Preferred);
  drawing_board_group_height.setVerticalStretch(3);
  drawing_board_group->setSizePolicy(drawing_board_group_height);
  drawing_board_box->addWidget(drawing_board, 1);
  drawing_board_group->setLayout(drawing_board_box);

  // Tools group placement
  tools_group = new QGroupBox();
  QHBoxLayout *horizontal_tools_box = new QHBoxLayout(this);
  QSizePolicy tools_group_height(QSizePolicy::Preferred, QSizePolicy::Preferred);
  tools_group_height.setVerticalStretch(1);
  tools_group->setSizePolicy(tools_group_height);
  QPushButton *clear_board = new QPushButton(tr("Clear"), this);
  connect(clear_board,   &QPushButton::released,
          drawing_board, &EmitterBoard::clearEvent);
  QPushButton *pick_color = new QPushButton(tr("Pick a color"), this);
  connect(pick_color,    &QPushButton::released,
          drawing_board, &EmitterBoard::colorPickEvent);
  line_width_form = new QLineEdit(tr("4"), this);
  connect(line_width_form, &QLineEdit::editingFinished,
          drawing_board,   &EmitterBoard::widthChangedEvent);

  horizontal_tools_box->addWidget(clear_board);
  horizontal_tools_box->addWidget(pick_color);
  horizontal_tools_box->addWidget(line_width_form);
  tools_group->setLayout(horizontal_tools_box);

  // Set window layout
  windowLayout->addWidget(drawing_board_group);
  windowLayout->addWidget(tools_group);
  this->setLayout(windowLayout);
}

void MainWindow::closeEvent(QCloseEvent *event) {
  //digitalWrite(7, HIGH);
  QApplication::quit();
}
