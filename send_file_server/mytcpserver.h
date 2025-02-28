#ifndef MYTCPSERVER_H
#define MYTCPSERVER_H

#include <QTcpServer>

class MyTcpServer : public QTcpServer {
    Q_OBJECT
  public:
    explicit MyTcpServer(QObject *parent = nullptr);
    
  protected:
    void incomingConnection(qintptr socket_descriptor) override;

  signals:
    void new_socket_descriptor(qintptr socket_descriptor);
};

#endif  // MYTCPSERVER_H
