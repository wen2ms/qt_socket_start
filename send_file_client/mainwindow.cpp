#include "mainwindow.h"

#include <QThread>
#include <QMessageBox>
#include <QFileDialog>
#include <QDebug>

#include "./ui_mainwindow.h"
#include "sendfile.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    
    qDebug() << "Client Main Thread:" << QThread::currentThread();
    
    ui->ip->setText("127.0.0.1");
    ui->port->setText("9999");
        
    ui->progressBar->setRange(0, 100);
    ui->progressBar->setValue(0);
    
    QThread* send_file_thread = new QThread;
    
    SendFile* worker = new SendFile;
    
    worker->moveToThread(send_file_thread);
    
    connect(this, &MainWindow::start_connect, worker, &SendFile::connect_server);
    
    connect(this, &MainWindow::send_file, worker, &SendFile::send_file);
    
    connect(worker, &SendFile::connected, this, [=]() {
        QMessageBox::information(this, "Connect Server", "Successfully connected to the server");
    });
    
    connect(worker, &SendFile::disconnected, this, [=]() {
        send_file_thread->quit();
        send_file_thread->wait();
        
        worker->deleteLater();
        send_file_thread->deleteLater();
    });
    
    connect(worker, &SendFile::current_percent, ui->progressBar, &QProgressBar::setValue);
    
    send_file_thread->start();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_connect_server_clicked() {
    QString ip = ui->ip->text();
    unsigned short port = ui->port->text().toUShort();
    
    emit start_connect(port, ip);
}

void MainWindow::on_set_file_clicked() {
    QString file_path = QFileDialog::getOpenFileName();
    
    if (file_path.isEmpty()) {
        QMessageBox::warning(this, "Open File", "The file path selected cannot be empty");
        
        return;
    }
    
    ui->file_path->setText(file_path);
}

void MainWindow::on_send_file_clicked() {
    emit send_file(ui->file_path->text());
}
