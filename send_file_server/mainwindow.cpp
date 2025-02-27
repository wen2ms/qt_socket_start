#include "mainwindow.h"

#include <QHostAddress>
#include <QMessageBox>
#include <QTcpSocket>

#include "./ui_mainwindow.h"
#include "recvfile.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    
    server_ = new QTcpServer;
    
    connect(server_, &QTcpServer::newConnection, this, [=]() {
        QTcpSocket* socket = server_->nextPendingConnection();
        
        RecvFile* subthread = new RecvFile(socket);
        
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
