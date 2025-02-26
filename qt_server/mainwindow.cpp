#include "mainwindow.h"

#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    
    setWindowTitle("Server");
    
    ui->port->setText("9999");
    
    server_ = new QTcpServer(this);
    
    connect(server_, &QTcpServer::newConnection, this, [=]() {
        socket_ = server_->nextPendingConnection();
        
        status_->setPixmap(QPixmap(":/resource/checked.png").scaled(15, 15));
        
        connect(socket_, &QTcpSocket::readyRead, this, [=]() {
            QByteArray data = socket_->readAll();
            
            ui->record->append("Client says: " + data);
        });
        
        connect(socket_, &QTcpSocket::disconnected, this, [=]() {
            socket_->close();
            socket_->deleteLater();
            
            status_->setPixmap(QPixmap(":/resource/unchecked.png").scaled(15, 15));
        });
    });
    
    status_ = new QLabel();
    
    status_->setPixmap(QPixmap(":/resource/unchecked.png").scaled(15, 15));
    
    ui->statusbar->addWidget(new QLabel("Connection Status: "));
    ui->statusbar->addWidget(status_);
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
    
    ui->record->append("Server says: " + message);
}
