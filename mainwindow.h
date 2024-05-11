
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QRubberBand>
#include <QPixmap>
#include <QThread>
#include <QGraphicsDropShadowEffect>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow

{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    //virtual void mousePressEvent(QMouseEvent*) override;
    //virtual void mouseMoveEvent(QMouseEvent*) override;
    //virtual void mouseReleaseEvent(QMouseEvent*) override;
private slots:
    void on_actionOpen_triggered();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_actionSave_triggered();


    bool eventFilter(QObject *o, QEvent *e);

    void on_PaletteButton_clicked();

    void on_Back_origButton_clicked();

public slots:
    void SliderM_valueChanged();
    void Slider_red_Changed();
    void Slider_green_Changed();
    void Slider_blue_Changed();
    //void show();


private:

    QMap<int, int> colorMap;
    QRubberBand* rubberBand;
    //QGraphicsDropShadowEffect *shadow;
    int im_wid;
    int lw;
    int num;
    int im_height;
    QColor color_before;
    QColor color_after;

    QString Line1;
    QString Line2;
    QString Line3;
    QString Line4;
    QPoint origin;
    Ui::MainWindow *ui;
    QString filename;
    QPixmap image;
    QPixmap original;
    QPixmap crop_orig;
    double factor;
};

#endif // MAINWINDOW_H
