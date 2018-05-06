#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QWidget>
#include <QApplication>
#include <QPushButton>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSizePolicy>
#include <QCloseEvent>
#include "emitter_board.h"

class ComInterface;

class MainWindow : public QWidget {
  Q_OBJECT
  friend class ComInterface;

public:
  MainWindow(QWidget *parent = 0);

private:
  EmitterBoard *drawing_board;
  QGroupBox *drawing_board_group;
  QGroupBox *tools_group;

protected slots:
  virtual void closeEvent(QCloseEvent *);
};

#endif
