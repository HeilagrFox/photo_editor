
#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QFileDialog>
#include <QColorDialog>
#include <iostream>
#include <QMouseEvent>
#include <QRubberBand>
#include <QPixmap>
#include <QMessageBox>
#include <QDebug>
#include <QSet>
#include <QMap>
#include <algorithm>
#include <QColor>
#include <QDebug>
#include <QGraphicsDropShadowEffect>

using namespace std;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    std::srand(std::time(nullptr));
    ui->setupUi(this);
    ui->MonSlider->setRange(0,255);
    ui->Blue_Slider->setRange(-255,255);
     ui->Blue_Slider->setValue(0);
    ui->Red_Slider->setRange(-255,255);
     ui->Red_Slider->setValue(0);
    ui->Green_Slider->setRange(-255,255);
     ui->Green_Slider->setValue(0);
    ui->MonSlider->hide();
    qDebug()<<ui->MonSlider->value();
    connect(ui->MonSlider,SIGNAL(valueChanged(int)),this,SLOT(SliderM_valueChanged()));
    connect(ui->Red_Slider,SIGNAL(valueChanged(int)),this,SLOT(Slider_red_Changed()));
    connect(ui->Green_Slider,SIGNAL(valueChanged(int)),this,SLOT(Slider_green_Changed()));
    connect(ui->Blue_Slider,SIGNAL(valueChanged(int)),this,SLOT(Slider_blue_Changed()));
    ui->originButton->installEventFilter(this);
    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
    shadow->setOffset(0, 0);
    QString str="#818181";
    shadow->setColor(QColor(str));
    shadow->setBlurRadius(30);
    QGraphicsDropShadowEffect *shadow_2=new QGraphicsDropShadowEffect(this);
    shadow_2->setOffset(0, 0);
    shadow_2->setColor(QColor(str));
    shadow_2->setBlurRadius(30);
    QGraphicsDropShadowEffect *shadow_3=new QGraphicsDropShadowEffect(this);
    shadow_3->setOffset(0, 0);
    shadow_3->setColor(QColor(str));
    shadow_3->setBlurRadius(30);

    ui->frame->setGraphicsEffect(shadow);
     ui->frame_2->setGraphicsEffect(shadow_2);
    ui->label->setGraphicsEffect(shadow_3);


}


int getRandomNumber(int min, int max)
{
    return min + (std::rand() % static_cast<int>(max - min + 1));
}

MainWindow::~MainWindow()
{
    delete ui;
}


bool MainWindow::eventFilter(QObject *o, QEvent *e){
    if(o ==ui-> originButton){

        if(e->type() == QEvent::HoverEnter) {
            ui->originButton->setStyleSheet("background:rgb(208, 197, 184);font: 700 9pt 'Segoe Script';color:black;border: 1px  solid white;border-radius: 10px 10px 10px 10px;padding: 2px;");
            ui->label->setPixmap(original);
            return true;
        }
        if(e->type() == QEvent::HoverLeave) {
            ui->originButton->setStyleSheet("background:rgb(255, 242, 225);font: 700 9pt 'Segoe Script';color:black;border: 1px  solid white;border-radius: 10px 10px 10px 10px;padding: 2px;");
            ui->label->setPixmap(crop_orig);
            return true;
        }


    }

    return QMainWindow::eventFilter(o, e);
}


void MainWindow::on_actionOpen_triggered()
{
    filename = QFileDialog::getOpenFileName(this,tr("Choose"),"","*.png *.jpg *.jpeg ;; *.png ;; *.jpg ;; *.jpeg");
    if (QString::compare(filename,QString())!=0){


        bool valid = image.load(filename);
        if (valid){
            ui->label->setPixmap(image);
            //ui->label->setStyleSheet("border-radius: 10px 10px 10px 10px;border: 1px  solid black;");
            original=image;
             crop_orig=original;
            im_wid=image.width();
            QString v_w=QString::number(im_wid);
            ui->l_width->setText(v_w);

            im_height=image.height();
            QString v_h=QString::number(im_height);
            ui->l_height->setText(v_h);

            lw=ui->label->width();
            double factor=double(lw)/(im_wid);
            ui->label->setFixedWidth(factor*ui->label->pixmap().width());
            ui->label->setFixedHeight(factor*ui->label->pixmap().height());
            ui->MonSlider->hide();

        }
        else{

        }
    }
}

void MainWindow::on_actionSave_triggered()
{
    if (crop_orig.isNull()){
        QMessageBox::warning(this,tr("Image Editor"),tr("No image is loaded. Please open an image before saving."));
    }
    else{
        filename = QFileDialog::getSaveFileName(this,tr("Save image"),"","*.png;; *.jpg ;;*.jpeg");
        if (!filename.isEmpty()){
            crop_orig.save(filename,0,100);
        }

    }
}

void MainWindow::SliderM_valueChanged()
{
    QImage myImage;
    myImage = image.toImage();
    QImage newImage=myImage;
    int clr=(ui->MonSlider->value());
    QColor oldColor;
    for(int x = 0; x<newImage.width(); x++){
        for(int y = 0; y<newImage.height(); y++){
            oldColor = newImage.pixelColor(x,y);
            qreal ns=oldColor.hslSaturation()-clr;
            if (ns<0) ns=0;
            if (ns<-255) ns=-255;
            oldColor.setHsl(oldColor.hslHue(),ns, oldColor.lightness());
            newImage.setPixelColor(x,y,oldColor);
        }
    }
    QPixmap ans =QPixmap::fromImage(newImage);
    crop_orig=ans;

     ui->label->setPixmap(ans);

}

void MainWindow::Slider_red_Changed()
{
  QImage myImage;
  myImage = crop_orig.toImage();
  QImage newImage=myImage;
  int clr=(ui->Red_Slider->value());
  QColor oldColor;
  for(int x = 0; x<newImage.width(); x++){
        for(int y = 0; y<newImage.height(); y++){
            oldColor=newImage.pixelColor(x,y);
            int color_after = oldColor.red()+clr;

            if (color_after<=255 && color_after>=-255 ){
                newImage.setPixelColor(x,y,qRgb(color_after, oldColor.green(),oldColor.blue()));
            }

        }
  }
  QPixmap ans =QPixmap::fromImage(newImage);

  crop_orig=ans;
  ui->label->setPixmap(ans);

}

void MainWindow::Slider_green_Changed()
{
  QImage myImage;
  myImage = crop_orig.toImage();
  QImage newImage=myImage;
  int clr=(ui->Green_Slider->value());
  QColor oldColor;
  for(int x = 0; x<newImage.width(); x++){
        for(int y = 0; y<newImage.height(); y++){
            oldColor=newImage.pixelColor(x,y);
            int color_after = oldColor.green()+clr;
            if (color_after<=255 && color_after>=-255 ){
                newImage.setPixelColor(x,y,qRgb(oldColor.red(), color_after,oldColor.blue()));
            }

        }
  }
  QPixmap ans =QPixmap::fromImage(newImage);

  crop_orig=ans;
  ui->label->setPixmap(ans);
   //ui->Green_Slider->setValue(0);
}

void MainWindow::Slider_blue_Changed()
{
  QImage myImage;
  myImage = crop_orig.toImage();
  QImage newImage=myImage;
  int clr=(ui->Blue_Slider->value());
  QColor oldColor;
  for(int x = 0; x<newImage.width(); x++){
        for(int y = 0; y<newImage.height(); y++){
            oldColor=newImage.pixelColor(x,y);
            int color_after = oldColor.blue()+clr;
            if (color_after<=255 && color_after>=-255 ){
                newImage.setPixelColor(x,y,qRgb(oldColor.red(), oldColor.green(),color_after));
            }

        }
  }
  QPixmap ans =QPixmap::fromImage(newImage);

  crop_orig=ans;
  ui->label->setPixmap(ans);
  // ui->Blue_Slider->setValue(0);
}

void MainWindow::on_pushButton_clicked()
{
    bool ok;
    Line1= ui->lineEdit->text();
    Line2 = ui->lineEdit_2->text();
    Line3 = ui->lineEdit_3->text();
    Line4 = ui->lineEdit_4->text();
    int val1=Line1.toInt(&ok);
    int val2=Line2.toInt(&ok);
    int val3=Line3.toInt(&ok);
    int val4=Line4.toInt(&ok);

    QPixmap Copy;
    QRect Rect(val1, val2, val3, val4);
    Copy=crop_orig.copy(Rect);
    image=Copy;
    im_wid=image.width();
    QString v_w=QString::number(im_wid);
    ui->l_width->setText(v_w);

    im_height=image.height();
    QString v_h=QString::number(im_height);
    ui->l_height->setText(v_h);
    crop_orig=image;
    ui->label->setPixmap((crop_orig));

}


void MainWindow::on_pushButton_2_clicked()
{

    QImage myImage;
    myImage = crop_orig.toImage();

    QImage newImage = QImage(myImage.width(), myImage.height(), QImage::Format_ARGB32);

    QColor oldColor;

    for(int x = 0; x<newImage.width(); x++){
        for(int y = 0; y<newImage.height(); y++){
            oldColor = QColor(myImage.pixel(x,y));
            int average = (oldColor.red()+oldColor.green()+oldColor.blue())/3;
            newImage.setPixel(x,y,qRgb(average,average,average));
        }
    }
    crop_orig=QPixmap::fromImage(newImage);
    ui->label->setPixmap( crop_orig);
    ui->MonSlider->show();
    ui->MonSlider->setValue(255);

}

void MainWindow::on_PaletteButton_clicked()
{
    ui->Colorlbl_1->setStyleSheet("background-color: white; border: 1px  solid black; border-radius: 20px 10px 20px 10px;");
    ui->Colorlbl_2->setStyleSheet("background-color: white; border: 1px  solid black; border-radius: 20px 10px 20px 10px;");
    ui->Colorlbl_3->setStyleSheet("background-color: white; border: 1px  solid black; border-radius: 20px 10px 20px 10px;");
    ui->Colorlbl_4->setStyleSheet("background-color: white; border: 1px  solid black; border-radius: 20px 10px 20px 10px;");
    ui->Colorlbl_5->setStyleSheet("background-color: white; border: 1px  solid black; border-radius: 20px 10px 20px 10px;");
    ui->Colorlbl_6->setStyleSheet("background-color: white; border: 1px  solid black; border-radius: 20px 10px 20px 10px;");
    QImage myImage;
    myImage = crop_orig.toImage();
    int numColors = 6; // количество основных цветов, которые мы хотим определить
    int saturationTolerance = getRandomNumber(20,40);; // порог насыщенности для сравнения
    std::vector<QColor> colors;
    QImage newImage = QImage(myImage.width(), myImage.height(), QImage::Format_RGB32);
    for (int y = 0; y < newImage.height(); ++y) {
        for (int x = 0; x < newImage.width(); ++x) {
            QColor pixelColor = myImage.pixelColor(x, y); // получаем цвет пикселя
            bool found = false;
            for(int i = 0; i < colors.size(); i++) {
                if(qAbs(pixelColor.hsvSaturation() - colors[i].hsvSaturation()) <= saturationTolerance) {
                    // насыщенность цветов примерно одинакова, добавляем пиксель в кластер
                    colors[i] = QColor((colors[i].red() + pixelColor.red())/2,
                                       (colors[i].green() + pixelColor.green())/2,
                                       (colors[i].blue() + pixelColor.blue())/2);
                    found = true;
                    break;
                }
            }
            if(!found) {
                // не нашли подходящий кластер, создаем новый
                colors.push_back(pixelColor);
            }
        }
    }
    std::sort(colors.begin(), colors.end(), [](const QColor& a, const QColor& b) -> bool {
        return a.value() > b.value();
    });
    int i=0;
    for (const QColor& color : colors) {
        qDebug()<<color.value();

        if (i==0){
            ui->Colorlbl_1->setStyleSheet("background-color: " + QColor(color).name() + ";"+" border: 1px  solid black; border-radius: 20px 10px 20px 10px;");
        }
        if (i==1){
            ui->Colorlbl_2->setStyleSheet("background-color: " + QColor(color).name() + ";"+" border: 1px  solid black; border-radius: 20px 10px 20px 10px;");
        }
        if (i==2){
            ui->Colorlbl_3->setStyleSheet("background-color: " + QColor(color).name() + ";"+" border: 1px  solid black; border-radius: 20px 10px 20px 10px;");
        }
        if (i==3){
            ui->Colorlbl_4->setStyleSheet("background-color: " + QColor(color).name() + ";"+" border: 1px  solid black; border-radius: 20px 10px 20px 10px;");
        }
        if (i==4){
            ui->Colorlbl_5->setStyleSheet("background-color: " + QColor(color).name() + ";"+" border: 1px  solid black; border-radius: 20px 10px 20px 10px;");
        }
        if (i==5){
            ui->Colorlbl_6->setStyleSheet("background-color: " + QColor(color).name() + ";"+" border: 1px  solid black; border-radius: 20px 10px 20px 10px;");
        }
        i++;
    }
}

void MainWindow::on_Back_origButton_clicked()
{
    ui->label->setPixmap(original);
    image=original;
    crop_orig=original;
    im_wid=original.width();
    QString v_w=QString::number(im_wid);
    ui->l_width->setText(v_w);

    im_height=original.height();
    QString v_h=QString::number(im_height);
    ui->l_height->setText(v_h);
    ui->Blue_Slider->setValue(0);
    ui->Red_Slider->setValue(0);
    ui->Green_Slider->setValue(0);

}











