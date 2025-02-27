#ifndef RECVFILE_H
#define RECVFILE_H

#include <QThread>
#include <QTcpSocket>

class RecvFile : public QThread {
    Q_OBJECT
  public:
    explicit RecvFile(QObject *parent = nullptr);
    
    explicit RecvFile(QTcpSocket* socket, QObject *parent = nullptr);
    
  protected:
    void run() override;

  signals:
    void recv_completed();
    
  private:
    QTcpSocket* socket_;
};

#endif  // RECVFILE_H
