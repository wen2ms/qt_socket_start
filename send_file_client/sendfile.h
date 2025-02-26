#ifndef SENDFILE_H
#define SENDFILE_H

#include <QObject>
#include <QTcpSocket>

class SendFile : public QObject {
    Q_OBJECT
  public:
    explicit SendFile(QObject *parent = nullptr);
    
    void connect_server(unsigned short port, QString ip);
    
    void send_file(QString file_path);

  signals:
    void connected();
    
    void disconnected();
    
  private:
    QTcpSocket* socket_;
};

#endif  // SENDFILE_H
