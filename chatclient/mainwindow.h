#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QTextEdit>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDateTime>
#include <QDebug>
#include <QHostAddress>
#include <QTcpSocket>
#include <QTime>
#include <QTimer>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    QLabel *labelIP;
    QLabel *labelPort;
    QLabel *labelName;
    QLabel *labelUsers;
    QLineEdit *editIP;
    QLineEdit *editPort;
    QLineEdit *editName;
    QLineEdit *lineSend;
    QComboBox *combo;
    QPushButton *buttonConnect;
    QPushButton *buttonDisconnect;
    QPushButton *buttonSend;
    QTextEdit *textEdit;
    QWidget *mainWidget;
    QTcpSocket *tcpSocket;
    QHostAddress serverAddress;
    quint16 serverPort;
    quint16 blockSize;
    QTime time;
    QTimer *timer;

private slots:
    void clientConnect();
    void clientDisconnect();
    void onSend();
    void onRead();
    void onConnected();
    void checkingConnection();
};

#endif // MAINWINDOW_H
