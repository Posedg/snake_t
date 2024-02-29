#include "widget.h"
#include "ui_widget.h"

#include <QPainter>
#include <QPixmap>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
      , ui(new Ui::Widget)
{
  ui->setupUi(this);
  //使用定时器
  timer = new QTimer();
  //定时器有timeout超时信号
  //信号槽(信号和对应关系关联)
  connect(timer, SIGNAL(timeout()), this, SLOT(timeout()));
  resize(600, 600);
  //初始化蛇身
  QRectF rect(300, 180, nodewidth, nodeheight);

  snake.append(rect);
  addtop();
  addtop();
  addtop();

  //初始化奖品
  newreward();
}

Widget::~Widget()
{
  delete ui;
}

void Widget::keyPressEvent(QKeyEvent *event) {
  switch (event->key()) {
    case Qt::Key_Up:
      if (moveFlag != DOWN) {
        moveFlag = UP;
      }
      break;
    case Qt::Key_Down:
      if (moveFlag != UP) {
        moveFlag = DOWN;
      }
      break;
    case Qt::Key_Right:
      if (moveFlag != LEFT) {
        moveFlag = RIGHT;
      }
      break;
    case Qt::Key_Left:
      if (moveFlag != RIGHT) {
        moveFlag = LEFT;
      }
      break;

    case Qt::Key_Space:
      if (gamestart == false) {
        gamestart = true;
        //启动定时器
        timer->start(time);
      } else {
        gamestart = false;
        timer->stop();
      }

      break;
    case Qt::Key_E:
      QApplication *app;
      app->exit(0);
      break;
    default:
      break;
  }
}
void Widget::timeout() {
  //判断重合
  int count = 1;

  if (snake[0].intersects(rewardnode)) {
    count++;
    newreward();
  }
  while (count) {
    switch (moveFlag) {
      case UP:
        // 加一个方块
        addtop();
        break;
      case DOWN:
        adddown();
        break;
      case RIGHT:
        addright();
        break;
      case LEFT:
        addleft();
        break;
      default:
        break;
    }
    count--;
  }

  delelast();
  update();
}

void Widget::addleft() {
  QPointF lefttop;
  QPointF rightbotom;
  if (snake[0].x() - nodewidth < 0) {
    lefttop = QPointF(this->width() - nodewidth, snake[0].y());
    rightbotom = QPointF(this->width(), snake[0].y() + nodeheight);

  } else {
    lefttop = QPointF(snake[0].x() - nodewidth, snake[0].y());
    rightbotom = snake[0].bottomLeft();
  }
  snake.insert(0, QRectF(lefttop, rightbotom));
}

void Widget::addright() {
  QPointF lefttop;
  QPointF rightbotom;
  if (snake[0].x() + nodewidth > this->width()) {
    lefttop = QPointF(0, snake[0].y());
    rightbotom = QPointF(nodewidth, snake[0].y() + nodeheight);
  } else {
    lefttop = snake[0].topRight();
    rightbotom =
        QPointF(snake[0].x() + 2 * nodewidth, snake[0].y() + nodeheight);
  }
  snake.insert(0, QRectF(lefttop, rightbotom));
}

void Widget::adddown() {
  QPointF lefttop;
  QPointF rightbotom;
  if (snake[0].y() + 2 * nodeheight > this->height()) {
    lefttop = QPointF(snake[0].x(), 0);
    rightbotom = QPointF(snake[0].x() + nodewidth, nodeheight);
  } else {
    lefttop = snake[0].bottomLeft();
    rightbotom =
        QPointF(snake[0].x() + nodewidth, snake[0].y() + 2 * nodeheight);
  }
  snake.insert(0, QRectF(lefttop, rightbotom));
}

void Widget::addtop() {
  QPointF lefttop;
  QPointF rightbotom;

  //保持snake循环
  if (snake[0].y() - nodeheight < 0) {
    lefttop = QPoint(snake[0].x(), this->height() - nodeheight);
    rightbotom = QPointF(snake[0].x() + nodewidth, this->height());

  } else {
    lefttop = QPointF(snake[0].x(), snake[0].y() - nodeheight);
    rightbotom = snake[0].topRight();
  }

  // xy坐标系数
  /*y
   * yxxxxxxxxxxxxxxxxxxxx->
   * y
   * y
   * y
   * y
   * y             *
   * y             #
   * y             #
   * y
   * y
   * |
   * v
   *
   * */
  //按照此理解snake[].x()/y()表示右上角的像素点位置

  // QRectF左上角和右下角的位置
  snake.insert(0, QRectF(lefttop, rightbotom));
}

void Widget::paintEvent(QPaintEvent *event) {
  QPainter painter(this);
  QPen pen;
  QBrush brash;

  //背景图片
  QPixmap pix;
  pix.load("D:/images/123.jpg");
  painter.drawPixmap(0, 0, 600, 600, pix);
  QFont font("宋体", 10, QFont::ExtraLight, false);
  painter.setFont(font);
  painter.drawText(10, 20, QString("按“E”退出，“P”暂停"));

  //画蛇
  pen.setColor(Qt::black);
  brash.setColor(Qt::red);
  brash.setStyle(Qt::SolidPattern);
  painter.setPen(pen);
  painter.setBrush(brash);

  for (int i = 0; i < snake.length(); i++) {
    painter.drawRect(snake[i]);
  }

  //画奖品
  pen.setColor(Qt::black);
  brash.setColor(Qt::red);
  brash.setStyle(Qt::SolidPattern);
  painter.setPen(pen);
  painter.setBrush(brash);
  painter.drawRect(rewardnode);

  if (eatself()) {
    timer->stop();
    QFont font("方正舒体", 30, QFont::ExtraLight, false);
    painter.setFont(font);
    painter.drawText(200, 200, QString("GAME OVER!"));
  }
}

void Widget::newreward() {
  int randnum = qrand();

  rewardnode = QRectF(randnum % (this->width() / nodewidth) * nodewidth,
                      randnum % (this->height() / nodeheight) * nodeheight,
                      nodewidth, nodeheight);
}

void Widget::delelast() { snake.removeLast(); }

//自噬
bool Widget::eatself() {
  for (int j = 0; j < snake.length(); j++) {
    for (int i = j + 1; i < snake.length(); i++) {
      if (snake[0] == snake[i]) {
        return true;
      }
    }
  }
  return false;
}
