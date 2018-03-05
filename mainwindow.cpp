#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QDebug>
#include <QApplication>
#include <QDebug>
#include <cstdlib>
#include <ctime>
#include <iostream>
QString selectedFileName;
int couner = 0;
bool on_off = true;
QString fileName;
int zoomPort=0;
int xx;
int yy;
int angle = 0;
int offsetAngle=0;
double sx, sy;
#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT void JNICALL
Java_com_imageEditor_android_PhotoArt_fileSelected(JNIEnv */*env*/,
                                                             jobject /*obj*/,
                                                             jstring results)
{
    selectedFileName = QAndroidJniObject(results).toString();
}

#ifdef __cplusplus
}
#endif
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);

}

MainWindow::~MainWindow()
{
    delete ui;
}

//return to main
void MainWindow::on_Backbutton_clicked()
{
    QMessageBox::StandardButton reply;

    reply = QMessageBox::question(this, "Save", "Do you want to save your changes?",
        QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);

    if (reply == QMessageBox::Save) {
        MainWindow::Save();
        ui->stackedWidget->setCurrentIndex(0);
    }
    if (reply == QMessageBox::Discard)
    {
        ui->stackedWidget->setCurrentIndex(0);
    }

}

//start program
void MainWindow::on_GalleryButton_clicked()
{
     MainWindow::Open();
     ui->stackedWidget->setCurrentIndex(1);
}



void MainWindow::on_ExiButton_clicked()
{
    QApplication::quit();
}

void MainWindow::on_SaveButton_clicked()
{
  MainWindow::Save();
}

void MainWindow::on_OpenButton_clicked()
{
     MainWindow::Open();
}

void MainWindow::on_RedoButton_clicked()
{
     MainWindow::Redo();
}

void MainWindow::on_UndoButton_clicked()
{
     MainWindow::Undo();
}

void MainWindow::on_ZoomoutButton_clicked()
{
     MainWindow::ZoomOut();
}

void MainWindow::on_ZoomInButton_clicked()
{
    MainWindow::ZoomIn();
}

void MainWindow::on_ResetButton_clicked()
{
    MainWindow::Reset();
}

void MainWindow::on_SelectButton_clicked()
{
    MainWindow::SelectImage();
}

void MainWindow::on_CropButton_clicked()
{
    MainWindow::Crop();
}

void MainWindow::on_horizontalSlider_valueChanged(int angle)
{
   rotate(angle);
}
void MainWindow::Open() {
    if(select==1){
       rubberBand->hide();
    }
   undoStack = new QStack <QImage>;
   redoStack = new QStack <QImage>;
   selectedFileName = "#";
   QAndroidJniObject::callStaticMethod<void>("com/imageEditor/android/PhotoArt",
                                             "openAnImage",
                                             "()V");
   while(selectedFileName == "#")
       qApp->processEvents();

   if(QFile(selectedFileName).exists())
   {
       QImage img(selectedFileName);
       image=img;
       ui->imageLabel->setPixmap(QPixmap::fromImage(image));
       ui->imageLabel->setScaledContents( false );
       ui->imageLabel->setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed );
   }

   undoStack->push(image);
   select=0;
}

void MainWindow::ZoomIn()
{
  if(select==1){
      rubberBand->hide();
      QRect rect(a, distination);
      image = image.copy(rect);
      int w=(int)image.width()*2;
      int h=(int)image.height()*2;
      image = image.scaled(w, h,Qt::KeepAspectRatio,Qt::SmoothTransformation);
      ui->imageLabel->setPixmap(QPixmap::fromImage(image));
   }else{

  /**  Q_ASSERT(ui->imageLabel->pixmap());
    scaleFactor *= 1.25;
    ui->imageLabel->resize(scaleFactor *ui->imageLabel->pixmap()->size());*/
      int w=(int)image.width()*1.25;
      int h=(int)image.height()*1.25;
      image = image.scaled(w, h,Qt::KeepAspectRatio,Qt::SmoothTransformation);
      ui->imageLabel->setPixmap(QPixmap::fromImage(image));

      undoStack->push(image);
   }
  select=0;

}

void MainWindow::ZoomOut()
{
    if(select==1){
        rubberBand->hide();
        QRect rect(a, distination);
        image = image.copy(rect);
        int w=(int)image.width()*0.9;
        int h=(int)image.height()*0.9;
        image = image.scaled(w, h,Qt::KeepAspectRatio,Qt::SmoothTransformation);
        ui->imageLabel->setPixmap(QPixmap::fromImage(image));
    }
else{
    int w=(int)image.width()*0.9;
    int h=(int)image.height()*0.9;
    image = image.scaled(w,h,Qt::KeepAspectRatio,Qt::SmoothTransformation);
   // scaleImage(0.8);
   ui->imageLabel->setPixmap(QPixmap::fromImage(image));

    undoStack->push(image);}
    select=0;


}

void MainWindow::rotate(int angle)
{
    if(select==1){
        rubberBand->hide();
    }
    QPixmap rotatedPixmap((QPixmap::fromImage(image) ).size());
    rotatedPixmap.fill(QColor::fromRgb(0, 0, 0, 0)); //the new pixmap must be transparent.
    QPainter* p = new QPainter(&rotatedPixmap);
    QSize size = (QPixmap::fromImage(image) ).size();
    p->translate(size.width()/2,size.height()/2);
    offsetAngle =angle+offsetAngle;
    p->rotate(offsetAngle);

    p->translate(-size.width()/2,-size.height()/2);
    p->drawPixmap(0, 0, (QPixmap::fromImage(image) ));

    p->end();
    delete p;

    ui->imageLabel->setPixmap(rotatedPixmap);

    image = rotatedPixmap.toImage();
    undoStack->push(image);
    select=0;
}



void MainWindow::mousePressEvent(QMouseEvent* event){
    if(select==1){
        origin = event->pos();
        if (!rubberBand)
        rubberBand = new QRubberBand(QRubberBand::Rectangle, this);
        rubberBand->setGeometry(QRect(origin, QSize()));
        rubberBand->show();

     }

}

void MainWindow::mouseMoveEvent(QMouseEvent* event){
        if(select==1){
           rubberBand->setGeometry(QRect(origin, event->pos()).normalized());
        }

}

void MainWindow::mouseReleaseEvent(QMouseEvent *event){
    if(select==1){
    distination=event->globalPos();
    a = mapToGlobal(origin);
    a = ui->imageLabel->mapFromGlobal(a);
    distination = ui->imageLabel->mapFromGlobal(distination);
    QPixmap OriginalPix(*ui->imageLabel->pixmap());
    sx = ui->imageLabel->rect().width();
    sy = ui->imageLabel->rect().height();
    sx = OriginalPix.width() / sx;
    sy = OriginalPix.height() / sy;
    a.setX(int(a.x() * sx));
    distination.setX(int(distination.x() * sx));
    a.setY( int(a.y() * sy));
    distination.setY( int(distination.y() * sy));
    if(a.x()<distination.x()&&a.y()<distination.y()){
    }
    else if(a.x()<distination.x()&&a.y()>distination.y()){
    int temp=a.y();
    a.setY(distination.y());
    distination.setY(temp);
    }
    else if(a.x()>distination.x()&&a.y()<distination.y()){
    int temp=a.x();
    a.setX(distination.x());
    distination.setX(temp);
    }
    else{
    int temp=a.x();
    a.setX(distination.x());
    distination.setX(temp);
    temp=a.y();
    a.setY(distination.y());
    distination.setY(temp);
    }
}
}



//method implementation

void MainWindow::Save() {

    if(select==1){
       rubberBand->hide();
    }
    QAndroidJniObject::callStaticMethod<void>("com/imageEditor/android/PhotoArt",
                                      "createDirIfNotExists",
                                      "()V");
    int random_integer = rand();

       QString s = QString::number(random_integer);
       QString fileName="/storage/sdcard0/PhotoArt/anroid25545sDD55"+s+".png";
       QImageWriter writer(fileName);

       if (writer.write(image)) {
           QMessageBox::information(0, "Saving", QString("Image is saved"));
           QString imagePath="done";
         //  qDebug() << imagePath;
       }
    select=0;
}


void MainWindow::Crop() {
       if(select==1){
       rubberBand->hide();
       QRect rect(a, distination);
       image = image.copy(rect);
       ui->imageLabel->setPixmap(QPixmap::fromImage(image));
       undoStack->push(image);
       }
        select=0;

}

void MainWindow::SelectImage() {

    select=1;
}

void MainWindow::Reset() {

    if(select==1){
       rubberBand->hide();
    }
    while(undoStack->size()>1){
        (undoStack->pop());
    }
    image=undoStack->pop();
    ui->imageLabel->setPixmap(QPixmap::fromImage(image));
    ui->imageLabel->setScaledContents( false );
    ui->imageLabel->setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed );

    undoStack = new QStack <QImage>;
    redoStack = new QStack <QImage>;
    undoStack->push(image);
    select=0;
}

void MainWindow::Undo() {

    if(select==1){
       rubberBand->hide();
    }
    if(undoStack->size() > 1){
    redoStack->push(undoStack->pop());
    image=undoStack->top();
    ui->imageLabel->setPixmap(QPixmap::fromImage(image));
    ui->imageLabel->setScaledContents( false );
    ui->imageLabel->setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed );

    }
    select=0;
}


void MainWindow::Redo() {

    if(select==1){
       rubberBand->hide();
    }
    if(redoStack->size() > 0){
        image=redoStack->top();
        undoStack->push(redoStack->pop());
        ui->imageLabel->setPixmap(QPixmap::fromImage(image));
        ui->imageLabel->setScaledContents( false );
        ui->imageLabel->setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed );

     }
    select=0;
}

void MainWindow::on_DeleteButton_clicked()
{
    if(select==1){
        rubberBand->hide();
    }
      QPixmap pixmap(QPixmap::fromImage(image));
      QMatrix rm;
      rm.rotate(90);
      pixmap = pixmap.transformed(rm);
      ui->imageLabel->setPixmap(pixmap);
      image = pixmap.toImage();
      undoStack->push(image);
      select=0;
}
