#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include <QtAndroidExtras>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    QImage image;
    QStack<QImage> * undoStack;
    QStack<QImage> * redoStack;
    QString fileName;
    QPixmap imageP;
    double scaleFactor=1;
    int select;
    int openImage=0;
    ~MainWindow();


private slots:
    void on_Backbutton_clicked();
    void on_GalleryButton_clicked();


    void on_ExiButton_clicked();

    void on_SaveButton_clicked();

    void on_OpenButton_clicked();

    void on_RedoButton_clicked();

    void on_UndoButton_clicked();

    void on_ZoomoutButton_clicked();

    void on_ZoomInButton_clicked();

    void on_ResetButton_clicked();

    void on_SelectButton_clicked();

    void on_CropButton_clicked();

    void on_horizontalSlider_valueChanged(int value);

    void on_DeleteButton_clicked();

private:
    Ui::MainWindow *ui;
    QRubberBand* rubberBand{rubberBand = NULL};
    QPoint origin;
    QPoint distination;
    QPoint a;
    void Open();
    void Save();
    void Crop();
    void SelectImage();
    void Undo();
    void Redo();
    void Reset();
    void rotate(int angle);
    void ZoomOut();
    void ZoomIn();
protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
};

#endif // MAINWINDOW_H
