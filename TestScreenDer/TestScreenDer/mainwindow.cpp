#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "AreaSelector.h"

#include <QDebug>
#include <QFileDialog>
#include <QDir>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    areaSelector = new AreaSelector();

    setWindowTitle( "ScreenCapture");
    QIcon icon( QString::fromUtf8( ":/icons/mainIcon.jpg" ) );
    setWindowIcon( icon );

    //tab widget
    ui->tabWidget->setCurrentIndex(0); //always open on first tab

    //button properties
    ui->pushButtonFullscreen->setCheckable(true);
    ui->pushButtonFullscreen->setIcon(QIcon(":/icons/fullscreen.png"));

    ui->pushButtonSelectArea->setCheckable(true);
    ui->pushButtonSelectArea->setIcon(QIcon(":/icons/area.png"));

    ui->pushButtonPause->setDisabled(true);
    ui->pushButtonResume->setDisabled(true);
    ui->pushButtonStop->setDisabled(true);

    //line edit default text
    ui->lineEditPath->setText(QDir::homePath());

    //audio default selection
    ui->radioButtonYes->setChecked(true);

    //radio buttons
    ui->radioButton24->setChecked(true);
    ui->radioButton60->setMouseTracking(true);
    ui->radioButton60->setToolTip("High performances required");

    //connect
    connect(this                    , SIGNAL( signal_close() )      , areaSelector, SLOT( close() ) );
    connect(this                    , SIGNAL( signal_selection() )  , areaSelector, SLOT( slot_init() ) );
    connect(ui->pushButtonSelectArea, SIGNAL( toggled(bool) )       , areaSelector, SLOT( setVisible( bool ) ) );
    connect(ui->pushButtonSelectArea, SIGNAL( clicked() )           , areaSelector, SLOT( update() ) );
    connect(this                    , SIGNAL(signal_recording(bool)), areaSelector, SLOT( slot_recordMode(bool) ) );
    connect(this                    , SIGNAL( signal_reset_areaSelector()), areaSelector, SLOT (slot_areaReset()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent( QCloseEvent *event )
{
    Q_UNUSED(event);
    emit signal_close();
}

void MainWindow::enable_or_disable_tabs(bool val){
    QList<QWidget*>list = ui->tab_1->findChildren<QWidget*>();
    QList<QWidget*>list2 = ui->tab_2->findChildren<QWidget*>();
    list.append(list2);
    foreach(QWidget* w, list){
      w->setEnabled(val);
    }
}

void MainWindow::on_pushButtonSelectArea_clicked()
{
    bool prevstate = ui->pushButtonSelectArea->isChecked();
    ui->pushButtonFullscreen->setChecked(false);
    ui->pushButtonSelectArea->setChecked(true);
    if(prevstate){
        emit signal_selection();
    }
}

void MainWindow::on_pushButtonFullscreen_clicked()
{
    ui->pushButtonSelectArea->setChecked(false);
    ui->pushButtonFullscreen->setChecked(true);
    areaSelector->setVisible(false);
}

void MainWindow::on_toolButton_clicked()
{
    QString path = QFileDialog::getExistingDirectory(this, "Select a directory", QDir::homePath());
    ui->lineEditPath->setText(path);
}

void MainWindow::on_pushButtonStart_clicked()
{
    if(ui->pushButtonFullscreen->isChecked() | ui->pushButtonSelectArea->isChecked() ){
        ui->pushButtonStop->setEnabled(true);
        ui->pushButtonPause->setEnabled(true);
        ui->pushButtonStart->setDisabled(true);
      enable_or_disable_tabs(false);
     if(ui->pushButtonSelectArea->isChecked()) emit signal_recording(true);
    }else{
        //TODO minimize!
        return;
    }
}

void MainWindow::on_pushButtonPause_clicked()
{
    ui->pushButtonResume->setEnabled(true);
    ui->pushButtonPause->setEnabled(false);
}

void MainWindow::on_pushButtonResume_clicked()
{
    ui->pushButtonPause->setEnabled(true);
    ui->pushButtonResume->setEnabled(false);
}

void MainWindow::on_pushButtonStop_clicked()
{
    enable_or_disable_tabs(true);
    ui->pushButtonStart->setEnabled(true);
    if(ui->pushButtonSelectArea->isChecked()){
        emit signal_recording(false);
        emit signal_reset_areaSelector();
    }

    ui->pushButtonPause->setDisabled(true);
    ui->pushButtonResume->setDisabled(true);
    ui->pushButtonStop->setDisabled(true);
    ui->lineEditPath->setText(QDir::homePath());
    ui->radioButtonYes->setChecked(true);
    ui->radioButton24->setChecked(true);

}
