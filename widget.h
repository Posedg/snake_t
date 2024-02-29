#ifndef WIDGET_H
#define WIDGET_H

#include <QKeyEvent>
#include <QTimer>
#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

typedef enum Director { LEFT, RIGHT, UP, DOWN } dir;

//蛇的表示

//按键处理
//使用定时器
//   关联信号槽
//   启动定时器
//实现对应的超时处理
//渲染
//
class Widget : public QWidget
{
  Q_OBJECT

 public:
  Widget(QWidget *parent = nullptr);
  ~Widget();

 protected:
  //渲染
  void paintEvent(QPaintEvent *event);

  //按键处理

  void keyPressEvent(QKeyEvent *event);
  //减去小方块

  //添加奖品
  void newreward();

  //贪吃蛇移动函数
  void addtop();
  void adddown();
  void addright();
  void addleft();
  void delelast();
  //自噬
  bool eatself();

 private:
  Ui::Widget *ui;
  int moveFlag = UP;
  bool gamestart = false;

  //蛇d的表示,链表，有数组的特点
  // QRectF表示一块区域

  QList<QRectF> snake;

  //奖品
  QRectF rewardnode;
  //小方块宽和高
  int nodewidth = 20;
  int nodeheight = 20;

  //定时器
  QTimer *timer;
  int time = 100;  //超时时间间隔

 protected slots:
  void timeout();
};
#endif // WIDGET_H
