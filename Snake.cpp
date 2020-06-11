#include <QPainter>
#include <QTime>
#include <QDebug>
#include <QtCore/qmath.h>
#include <QMediaPlayer>
#include "Snake.h"

Bullet::Bullet(int x,int y){
    this->x = x;
    this->y = y;
    bulletImg.load(":/images/images/bullet.png");
}
void Bullet::move(){
    //没使用方向，请自行修改。
    x = x - (int) (qCos(direction) * 20);
    y = y - (int) (qSin(direction) * 20);
}

Snake::Snake(QWidget *parent) : QWidget(parent) {
    setStyleSheet("background-color:blue;");
    leftDirection = false;
    rightDirection = true;
    upDirection = false;
    downDirection = false;
    inGame = true;

    bcount =0;

    resize(B_WIDTH, B_HEIGHT);
    loadImages();
    initGame();
}

void Snake::loadImages() {

    dot.load(":/images/images/dot.png");
    head.load(":/images/images/head.png");
    apple.load(":/images/images/apple.png");
    button.load(":/images/images/button.png");
    buttonCover.load(":/images/images/buttonCover.png");
}

void Snake::initGame() {

    dots = 3;

    for (int z = 0; z < dots; z++) {
        x[z] = 50 - z * 10;
        y[z] = 50;
    }

    locateApple();
    locateButton();
    timerId = startTimer(DELAY);

    QMediaPlayer * player = new QMediaPlayer;
    player->setMedia(QUrl("qrc:/sounds/sounds/hdl.mp3"));
    player->setVolume(30);
    player->play();

}

void Snake::locateButton(){
    mvButton_x = 390;
    mvButton_y = 390;

    contrlButton_x =350;
    contrlButton_y =350;

}


void Snake::paintEvent(QPaintEvent *e) {

    Q_UNUSED(e);

    doDrawing();
}

void Snake::doDrawing() {

    QPainter qp(this);

    if (inGame) {
        qp.drawImage(apple_x, apple_y, apple);
        qp.drawImage(mvButton_x,mvButton_y, apple);
        qp.drawImage(contrlButton_x, contrlButton_y, button);

        for(int i=0; i<100;i++)
            qp.drawImage(bt[i].x, bt[i].y, bt[i].bulletImg);

        if(buttonCoverFlag==true)
            qp.drawImage(contrlButton_x, contrlButton_y, buttonCover);


        for (int z = 0; z < dots; z++) {
            if (z == 0) {
                qp.drawImage(x[z], y[z], head);
            } else {
                qp.drawImage(x[z], y[z], dot);
            }
        }

    } else {

        gameOver(qp);
    }
}

void Snake::gameOver(QPainter &qp) {

    QString message = "Game over";
    QFont font("Courier", 15, QFont::DemiBold);
    QFontMetrics fm(font);
    int textWidth = fm.width(message);

    qp.setFont(font);
    int h = height();
    int w = width();

    qp.translate(QPoint(w/2, h/2));
    qp.drawText(-textWidth/2, 0, message);
}

void Snake::checkApple() {

    if ((x[0] == apple_x) && (y[0] == apple_y)) {

        dots++;
        locateApple();
    }
}

void Snake::move() {

    for (int z = dots; z > 0; z--) {
        x[z] = x[(z - 1)];
        y[z] = y[(z - 1)];
    }

    if (leftDirection) {
        x[0] -= DOT_SIZE;
    }

    if (rightDirection) {
        x[0] += DOT_SIZE;
    }

    if (upDirection) {
        y[0] -= DOT_SIZE;
    }

    if (downDirection) {
        y[0] += DOT_SIZE;
    }
}

void Snake::checkCollision() {

    for (int z = dots; z > 0; z--) {

        if ((z > 4) && (x[0] == x[z]) && (y[0] == y[z])) {
            inGame = false;
        }
    }

    if (y[0] >= B_HEIGHT) {
        inGame = false;
    }

    if (y[0] < 0) {
        inGame = false;
    }

    if (x[0] >= B_WIDTH) {
        inGame = false;
    }

    if (x[0] < 0) {
        inGame = false;
    }

    if(!inGame) {
        killTimer(timerId);
        killTimer(timerId2);
    }
}

void Snake::locateApple() {

    QTime time = QTime::currentTime();
    qsrand((uint) time.msec());

    int r = qrand() % RAND_POS;
    apple_x = (r * DOT_SIZE);

    r = qrand() % RAND_POS;
    apple_y = (r * DOT_SIZE);

    printf("%d %d", apple_x, apple_y);
}

void Snake::timerEvent(QTimerEvent *e) {

    Q_UNUSED(e);
    int id = e->timerId();
    if (inGame) {
        if(id == timerId){
        checkApple();
        checkCollision();
        move();

        }
        else if(id==timerId2){
            if(buttonMove==false){
               //对子弹的初始和设定，并更新移动
               bt[bcount].x = mvButton_x;
               bt[bcount].y = mvButton_y;
               bt[bcount].direction=qAtan((double)(mvButton_y-y[0])/(mvButton_x-x[0]));
               bcount++;
               if(bcount>=20) bcount = 0;
            }
           for(int i=0; i<10; i++)
                bt[i].move();

           repaint();
        }

    }

    repaint();
}

void Snake::keyPressEvent(QKeyEvent *e) {

    int key = e->key();

    if ((key == Qt::Key_Left) && (!rightDirection)) {
        leftDirection = true;
        upDirection = false;
        downDirection = false;
    }

    if ((key == Qt::Key_Right) && (!leftDirection)) {
        rightDirection = true;
        upDirection = false;
        downDirection = false;
    }

    if ((key == Qt::Key_Up) && (!downDirection)) {
        upDirection = true;
        rightDirection = false;
        leftDirection = false;
    }

    if ((key == Qt::Key_Down) && (!upDirection)) {
        downDirection = true;
        rightDirection = false;
        leftDirection = false;
    }

    QWidget::keyPressEvent(e);
}

void Snake::mousePressEvent(QMouseEvent *event)
{
    int mx = event->x();
    int my = event->y();


    //设置按钮变化，开启第二个计时器，
    if ((mx<400) && (mx >=350) && (my<400) &&(my>=350)){
        buttonMove = true;
        mvButton_x = mx;
        mvButton_y = my;
        timerId2 = startTimer(10); //启动第二个时间控制器
        buttonCoverFlag = true; //按钮覆盖的标示
    }
    else{
        buttonMove = false;
    }

    if ((mx < x[0]) && (!rightDirection)) {
        leftDirection = true;
        upDirection = false;
        downDirection = false;
    }

    if ((mx > x[0]) && (!leftDirection)) {
        rightDirection = true;
        upDirection = false;
        downDirection = false;
    }

    if ((my < y[0]) && (!downDirection)) {
        upDirection = true;
        rightDirection = false;
        leftDirection = false;
    }

    if ((my > y[0]) && (!upDirection)) {
        downDirection = true;
        rightDirection = false;
        leftDirection = false;
    }

    QWidget::mousePressEvent(event);


}

void Snake::mouseMoveEvent(QMouseEvent *event){
    int mx = event->x();
    int my = event->y();

    if(buttonMove==true){
        mvButton_x = mx-5;
        mvButton_y = my-5;
        qDebug()<<mx<<my;
    }
    QWidget::mouseMoveEvent(event);
}

void Snake::mouseReleaseEvent(QMouseEvent *event){
    if(buttonMove==true){
        buttonMove =false;
    }
    int mx = event->x();
    int my = event->y();

//    //此处可以更改优化成类的封装方法
//    bt[bcount].x = mx;
//    bt[bcount].y = my;
//    bt[bcount].direction=qAtan((double)(my-y[0])/(mx-x[0]));
//    qDebug()<<bt[bcount].direction;
//    bcount++;
//    if(bcount>=10) bcount = 0;

    buttonCoverFlag = false;

    QWidget::mouseReleaseEvent(event);
}

