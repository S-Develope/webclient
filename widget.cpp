#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    connect(&socket_tcp, &QTcpSocket::connected, this, &Widget::onConnected);
    connect(&socket_ssl, &QSslSocket::connected, this, &Widget::onConnected);
    connect(&socket_tcp, &QTcpSocket::disconnected, this, &Widget::onDisconnected);
    connect(&socket_ssl, &QSslSocket::disconnected, this, &Widget::onDisconnected);

    ui->pbDisconnect->setEnabled(false);
    ui->pbSend->setEnabled(false);
}

Widget::~Widget()
{
    delete ui;
}

bool isSSLConnection = false;
void Widget::on_CKssl_stateChanged(int arg1)
{
    isSSLConnection = (arg1 == Qt::Checked);
}

void Widget::onConnected()
{
    ui->pteMessage->insertPlainText(isSSLConnection ? "HTTPS Connected !!\n" : "HTTP Connected !!\n");
    ui->CKssl->setEnabled(false);
    ui->pbConnect->setEnabled(false);
    ui->pbDisconnect->setEnabled(true);
    ui->pbSend->setEnabled(true);
}
void Widget::on_pbConnect_clicked()
{
    if (isSSLConnection) {
        socket_ssl.connectToHost(ui->leHost->text(), 443);
    } else {
        socket_tcp.connectToHost(ui->leHost->text(), 80);
    }
}

void Widget::onDisconnected()
{
    ui->pteMessage->insertPlainText("Disconnected !!\n");
    ui->CKssl->setEnabled(true);
    ui->pbConnect->setEnabled(true);
    ui->pbDisconnect->setEnabled(false);
    ui->pbSend->setEnabled(false);
}
void Widget::on_pbDisconnect_clicked()
{
    if(isSSLConnection){
        socket_ssl.close();
    }else{
        socket_tcp.close();
    }
}

void Widget::on_pbSend_clicked()
{
    if (isSSLConnection) {
        socket_ssl.write(ui->pteSend->toPlainText().toUtf8());
        ui->pteMessage->insertPlainText(socket_ssl.readAll());
    } else {
        socket_tcp.write(ui->pteSend->toPlainText().toUtf8());
        ui->pteMessage->insertPlainText(socket_tcp.readAll());
    }
}
