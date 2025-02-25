#include "mainwindow.h"

#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    
    ui->port->setText("9999");
    
    server_ = new QTcpServer(this);
    
    connect(server_, &QTcpServer::newConnection, this, [=]() {
        socket_ = server_->nextPendingConnection();
        
        connect(socket_, &QTcpSocket::readyRead, this, [=]() {
            QByteArray data = socket_->readAll();
            
            ui->record->append("Client says: " + data);
        });
    });
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_set_listen_clicked() {
    unsigned short port = ui->port->text().toUShort();
    
    server_->listen(QHostAddress::Any, port);
    
    ui->set_listen->setDisabled(true);
}

void MainWindow::on_send_message_clicked() {
    QString message = ui->message->toPlainText();
    
    socket_->write(message.toUtf8());
    
    ui->message->append("Server says: " + message);
}
