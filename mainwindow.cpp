#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    QObject::connect(ui->widget,SIGNAL(resetSub())
                     ,&data,SLOT(resetData()));
}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_actionSelect_Source_triggered()
{
    ui->widget->loadImage();
    ui->widget->update();
}

void MainWindow::on_actionSelect_Destination_triggered()
{
    ui->widget_2->loadImage();
    ui->widget_2->update();
}

void MainWindow::on_actionCut_triggered()
{
    ui->widget_3->maskImage
            =ui->widget->maskImage;
    ui->widget_3->update();

    ui->widget_2->subImage=ui->widget->subImage;
    ui->widget_2->update();

}

void MainWindow::on_actionCrack_Image_triggered()
{

    data.init(ui->widget->subImage,ui->widget->maskImage,
              ui->widget_2->destinationImage,
              ui->widget_2->maskPosition,
              ui->widget->sourceImage,
              ui->widget->srcPosition);
    data.crackImage(0);
}

void MainWindow::on_actionTest_triggered()
{
    data.test();
    ui->widget_3->maskImage=data.testImage;
    ui->widget_3->update();
    ui->widget_2->subImage=data.testImage;
    ui->widget_2->update();
}

void MainWindow::on_actionCal_SparseMatrix_triggered()
{
    data.calculate();
}


void MainWindow::on_push_clicked()
{
    int n=ui->spinBox->value();
    data.solveJacobi(n);
}

void MainWindow::on_actionCrack_Image_Mixed_triggered()
{
    data.init(ui->widget->subImage,ui->widget->maskImage,
              ui->widget_2->destinationImage,
              ui->widget_2->maskPosition,
              ui->widget->sourceImage,
              ui->widget->srcPosition);
    data.crackImage(1);
}
void MainWindow::on_pushButton_toggled(bool)
{

}
void MainWindow::on_pushButton_clicked()
{

}
