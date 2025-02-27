#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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
    
  signals:
    void start_connect(unsigned short port, QString ip);
    
    void send_file(const QString& file_path);
    
  private slots:
    void on_connect_server_clicked();
    
    void on_set_file_clicked();
    
    void on_send_file_clicked();
    
  private:
    Ui::MainWindow *ui;
};
#endif  // MAINWINDOW_H
