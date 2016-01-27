#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void paintEvent(QPaintEvent *);
    void rotate(int _angle);
    
private:
    Ui::MainWindow *ui;

    QPixmap *img;
    QRect *rect;
    double angle;
};

#endif // MAINWINDOW_H
