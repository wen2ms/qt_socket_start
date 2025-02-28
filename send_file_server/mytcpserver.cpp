#include "mytcpserver.h"

MyTcpServer::MyTcpServer(QObject *parent) : QTcpServer{parent} {}

void MyTcpServer::incomingConnection(qintptr socket_descriptor) {
    emit new_socket_descriptor(socket_descriptor);
}
