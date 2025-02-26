#include "mainwindow.h"

#include <QHostAddress>

#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    
    setWindowTitle("Client");
    
    ui->port->setText("9999");
    ui->ip->setText("127.0.0.1");
    
    ui->disconnect->setDisabled(true);
    
    socket_ = new QTcpSocket(this);
    
    connect(socket_, &QTcpSocket::readyRead, this, [=]() {
        QByteArray data = socket_->readAll();
        
        ui->record->append("Server says: " + data);
    });
    
    connect(socket_, &QTcpSocket::disconnected, this, [=]() {
        socket_->close();
        
        status_->setPixmap(QPixmap(":/resource/unchecked.png").scaled(15, 15));
        
        ui->record->append("Server has disconnected from the client...");
        
        ui->connect->setDisabled(false);
        ui->disconnect->setDisabled(true);
    });
    
    connect(socket_, &QTcpSocket::connected, this, [=]() {        
        status_->setPixmap(QPixmap(":/resource/checked.png").scaled(15, 15));
        
        ui->record->append("Successfully connected to the server...");
        
        ui->connect->setDisabled(true);
        ui->disconnect->setDisabled(false);
    });
    
    status_ = new QLabel();
    
    status_->setPixmap(QPixmap(":/resource/unchecked.png").scaled(15, 15));
    
    ui->statusbar->addWidget(new QLabel("Connection Status: "));
    ui->statusbar->addWidget(status_);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_send_message_clicked() {
    QString message = ui->message->toPlainText();
    
    socket_->write(message.toUtf8());
    
    ui->message->append("Client says: " + message);
}

void MainWindow::on_connect_clicked() {
    QString ip = ui->ip->text();
    unsigned short port = ui->port->text().toUShort();
    
    socket_->connectToHost(QHostAddress(ip), port);
}

void MainWindow::on_disconnect_clicked() {
    socket_->close();
    
    ui->disconnect->setDisabled(true);
    ui->connect->setDisabled(false);
}
