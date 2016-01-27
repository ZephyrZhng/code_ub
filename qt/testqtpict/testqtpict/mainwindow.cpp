#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QPainter>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->label->setScaledContents(true);

    img = new QPixmap("./ardrone1.png");
    int width = ui->label->width();
    int height = ui->label->height();
    int x = ui->label->x();
    int y = ui->label->y();
    rect = new QRect(x, y, width, height);

    angle = 0;

    rotate(60);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    img->load("./ardrone1.png");
    int width = ui->label->width();
    int height = ui->label->height();
    int x = ui->label->x();
    int y = ui->label->y();
    painter.translate((x + width) / 2,(y + height) / 2);                //使图片的中心作为旋转的中心
    painter.rotate(angle);                //顺时针旋转90°
    painter.translate(- (x + width) / 2, - (y + height) / 2);        //将原点复位
    painter.drawPixmap(*rect,*img);
}

void MainWindow::rotate(int _angle)
{
    angle = _angle;

    update();
}
