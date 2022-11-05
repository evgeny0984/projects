#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QComboBox>
#include <QLineEdit>
#include <QCheckBox>
#include <QPushButton>
#include <QTextEdit>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDateTime>
#include <QDebug>
#include <QFile>
#include <QHostAddress>
#include <QNetworkInterface>
#include <QTcpServer>
#include <QTcpSocket>
#include <QTime>
#include <QTimer>
#include <QSet>
#include <QMap>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    QLabel *labelIP;
    QLabel *labelPort;
    QLabel *labelTime;
    QLabel *labelClients;
    QComboBox *combo;
    QLineEdit *editPort;
    QLineEdit *editTime;
    QLineEdit *editClients;
    QLineEdit *lineSend;
    QCheckBox *check;
    QPushButton *buttonStart;
    QPushButton *buttonStop;
    QPushButton *buttonSend;
    QTextEdit *textEdit;
    QWidget *mainWidget;
    QHostAddress serverAddress;
    QTcpServer *tcpServer;
    QTcpSocket *tcpSocket;
    quint16 serverPort;
    quint16 blockSize;
    QTime time;
    QTimer *timer;
    QSet<QTcpSocket*> clients;
    QMap<QTcpSocket*, QString> users;

private slots:
    void getIP();
    void startServer();
    void stopServer();
    void onLog();
    void onSend();
    void incomingConnection();
    void onRead();
    void onDisconnected();
    void timeCount();
    void closeConnection();
    void setUserList();
};

#endif // MAINWINDOW_H
