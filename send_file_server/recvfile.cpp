#include "recvfile.h"

#include <QDebug>
#include <QFile>

RecvFile::RecvFile(QObject *parent) : QThread{parent} {}

RecvFile::RecvFile(qintptr socket_descriptor, QObject* parent) : socket_descriptor_(socket_descriptor), QThread{parent} {}

void RecvFile::run() {
    qDebug() << "Server Subthread:" << QThread::currentThread();
    
    socket_ = new QTcpSocket;
    
    socket_->setSocketDescriptor(socket_descriptor_);
    
    QFile* file = new QFile("recv.txt");
    
    file->open(QFile::WriteOnly);
    
    connect(socket_, &QTcpSocket::readyRead, this, [=]() {
        static int received_size = 0;
        static int total_file_size = 0;
        
        if (received_size == 0) {
            socket_->read((char*)&total_file_size, 4);
        }
        
        QByteArray recv_data = socket_->readAll();
        
        received_size += recv_data.size();
        
        file->write(recv_data);
        
        if (received_size == total_file_size) {
            socket_->deleteLater();
            
            file->close();
            file->deleteLater();
            
            emit recv_completed();
        }
    });
    
    exec();    
}
