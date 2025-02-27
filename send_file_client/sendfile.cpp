#include "sendfile.h"

#include <QHostAddress>
#include <QFile>
#include <QFileInfo>

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
    QFile file(file_path);
    QFileInfo file_info(file);
    int file_size = file_info.size();
    
    file.open(QFile::ReadOnly);
    
    socket_->write((char*)&file_size, 4);
    
    int current_size = 0;
    while (!file.atEnd()) {
        QByteArray line = file.readLine();
        
        current_size += line.size();
        int send_percent = current_size * 100 / file_size;
        
        emit current_percent(send_percent);
        
        socket_->write(line);
    }
}
