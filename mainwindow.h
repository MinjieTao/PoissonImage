#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "data.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    

private slots:

    void on_actionSelect_Source_triggered();

    void on_actionSelect_Destination_triggered();

    void on_actionCut_triggered();

    void on_actionCrack_Image_triggered();

    void on_actionTest_triggered();

    void on_actionCal_SparseMatrix_triggered();

    void on_pushButton_toggled(bool checked);

    void on_pushButton_clicked();

    void on_push_clicked();

    void on_actionCrack_Image_Mixed_triggered();

private:
    Ui::MainWindow *ui;

    Data data;

};

#endif // MAINWINDOW_H
