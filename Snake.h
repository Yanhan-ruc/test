#ifndef SNAKE_H
#define SNAKE_H

#include <QWidget>
#include <QKeyEvent>
#include <QMouseEvent>

class Bullet{
   public:
    Bullet(int x=-100,int y=-100);

    //请自行封装
    int x,y;
    double direction;
    QImage bulletImg;

    void move();
};

class Snake : public QWidget {
  public:
      Snake(QWidget *parent = 0);

  protected:
      void paintEvent(QPaintEvent *);
      void timerEvent(QTimerEvent *);
      void keyPressEvent(QKeyEvent *);
      void mousePressEvent(QMouseEvent *event);
      void mouseMoveEvent(QMouseEvent *event);
      void mouseReleaseEvent(QMouseEvent *event);

  private:
      QImage dot;
      QImage head;
      QImage apple;
      QImage button;
      QImage buttonCover;

      static const int B_WIDTH = 410;
      static const int B_HEIGHT = 410;
      static const int DOT_SIZE = 10;
      static const int ALL_DOTS = 900;
      static const int RAND_POS = 29;
      static const int DELAY = 140;

      int timerId;

      int timerId2; //第二个时间控制器，可以控制页面的响应操作

      //后续可以更改
      Bullet bt[100];
      int bcount;

      int dots;
      int apple_x;
      int apple_y;

      int contrlButton_x;
      int contrlButton_y;

      int mvButton_x;
      int mvButton_y;

      int x[ALL_DOTS];
      int y[ALL_DOTS];

      bool leftDirection;
      bool rightDirection;
      bool upDirection;
      bool downDirection;
      bool inGame;

      bool buttonMove;
      bool buttonCoverFlag;

      void loadImages();
      void initGame();
      void locateApple();
      void locateButton();
      void checkApple();
      void checkCollision();
      void move();
      void doDrawing();
      void gameOver(QPainter &);
};




#endif // SNAKE_H
