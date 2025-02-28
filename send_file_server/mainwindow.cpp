#include "mainwindow.h"

#include <QHostAddress>
#include <QMessageBox>
#include <QTcpSocket>
#include <QDebug>

#include "./ui_mainwindow.h"
#include "recvfile.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    
    ui->port->setText("9999");
    
    qDebug() << "Server Main Thread:" << QThread::currentThread();
    
    server_ = new MyTcpServer(this);
    
    connect(server_, &MyTcpServer::new_socket_descriptor, this, [=](qintptr socket_descriptor) {        
        RecvFile* subthread = new RecvFile(socket_descriptor);
        
        subthread->start();
        
        connect(subthread, &RecvFile::recv_completed, this, [=]() {            
            subthread->quit();
            subthread->wait();
            subthread->deleteLater();
            
            QMessageBox::information(this, "Receiving File", "Receiving file completed");
        });
    });
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_set_listen_clicked() {
    unsigned short port = ui->port->text().toUShort();
    
    server_->listen(QHostAddress::Any, port);
}
