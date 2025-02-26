#include "mainwindow.h"

#include <QThread>
#include <QMessageBox>

#include "./ui_mainwindow.h"
#include "sendfile.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    
    ui->ip->setText("127.0.0.1");
    ui->port->setText("9999");
        
    ui->progressBar->setRange(0, 100);
    ui->progressBar->setValue(0);
    
    QThread* send_file_thread = new QThread;
    
    SendFile* worker = new SendFile;
    
    worker->moveToThread(send_file_thread);
    
    connect(this, &MainWindow::start_connect, worker, &SendFile::connect_server);
    
    connect(worker, &SendFile::connected, this, [=]() {
        QMessageBox::information(this, "Connect Server", "Successfully connected to the server");
    });
    
    connect(worker, &SendFile::disconnected, this, [=]() {
        send_file_thread->quit();
        send_file_thread->wait();
        
        worker->deleteLater();
        send_file_thread->deleteLater();
    });
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_connect_server_clicked() {
    QString ip = ui->ip->text();
    unsigned short port = ui->port->text().toUShort();
    
    emit start_connect(port, ip);
}
