#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QLabel>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

  public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    
  private slots:        
    void on_send_message_clicked();
    
    void on_connect_clicked();
    
    void on_disconnect_clicked();
    
  private:
    Ui::MainWindow *ui;
    
    QTcpSocket* socket_;
    
    QLabel* status_;
};
#endif  // MAINWINDOW_H
