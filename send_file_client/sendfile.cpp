#include "sendfile.h"

#include <QHostAddress>

SendFile::SendFile(QObject *parent) : QObject{parent} {
}


void SendFile::connect_server(unsigned short port, QString ip) {
    socket_ = new QTcpSocket;
    
    socket_->connectToHost(QHostAddress(ip), port);
    
    connect(socket_, &QTcpSocket::connected, this, &SendFile::connected);
    
    connect(socket_, &QTcpSocket::disconnected, this, [=]() {
        socket_->close();
        socket_->deleteLater();
        
        emit disconnected();
    });
}

void SendFile::send_file(QString file_path) {
    
}
