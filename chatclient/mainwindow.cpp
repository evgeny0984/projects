#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    labelIP = new QLabel(this);
    labelIP->setText("Адрес");
    editIP = new QLineEdit(this);
    editIP->setText("127.0.0.1");
    labelPort = new QLabel(this);
    labelPort->setText("Порт");
    editPort = new QLineEdit(this);
    editPort->setMaxLength(5);
    editPort->setText("1234");
    editPort->setValidator(new QIntValidator(1, 65535, this));
    labelName = new QLabel(this);
    labelName->setText("Имя");
    editName = new QLineEdit(this);
    editName->setText("Ник");
    labelUsers = new QLabel(this);
    labelUsers->setText("Пользователи");
    combo = new QComboBox(this);
    buttonConnect = new QPushButton(this);
    buttonConnect->setText("Подключиться");
    connect(buttonConnect, SIGNAL(clicked()), this, SLOT(clientConnect()));
    buttonDisconnect = new QPushButton(this);
    buttonDisconnect->setEnabled(false);
    buttonDisconnect->setText("Отключиться");
    connect(buttonDisconnect, SIGNAL(clicked()), this, SLOT(clientDisconnect()));
    textEdit = new QTextEdit(this);
    textEdit->setReadOnly(true);
    lineSend = new QLineEdit(this);
    connect(lineSend, SIGNAL(returnPressed()), this, SLOT(onSend()));
    buttonSend = new QPushButton(this);
    buttonSend->setText("Отправить");
    connect(buttonSend, SIGNAL(clicked()), this, SLOT(onSend()));
    QGridLayout *glayout = new QGridLayout;
    glayout->addWidget(labelIP, 0, 0);
    glayout->addWidget(editIP, 0, 1);
    glayout->addWidget(labelPort, 1, 0);
    glayout->addWidget(editPort, 1, 1);
    glayout->addWidget(labelName, 0, 2);
    glayout->addWidget(editName, 0, 3);
    glayout->addWidget(labelUsers, 1, 2);
    glayout->addWidget(combo, 1, 3);
    glayout->addWidget(buttonConnect, 0, 4);
    glayout->addWidget(buttonDisconnect, 1, 4);
    QVBoxLayout *vlayout = new QVBoxLayout;
    vlayout->addLayout(glayout);
    QHBoxLayout *hlayout = new QHBoxLayout;
    hlayout->addWidget(lineSend);
    hlayout->addWidget(buttonSend);
    vlayout->addWidget(textEdit);
    vlayout->addLayout(hlayout);
    mainWidget = new QWidget;
    setCentralWidget(mainWidget);
    mainWidget->setLayout(vlayout);
    lineSend->setFocus();
    tcpSocket = new QTcpSocket(this);
    time = QTime(0, 0, 2, 0);
    timer = new QTimer(this);
    connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(onRead()));
    connect(tcpSocket, SIGNAL(connected()), this, SLOT(onConnected()));
    connect(timer, SIGNAL(timeout()), this, SLOT(checkingConnection()));
}

MainWindow::~MainWindow()
{
}

void MainWindow::clientConnect()
{
    serverAddress.setAddress(editIP->text());
    serverPort = editPort->text().toInt();
    editIP->setEnabled(false);
    editPort->setEnabled(false);
    editName->setEnabled(false);
    buttonConnect->setEnabled(false);
    buttonDisconnect->setEnabled(true);
    tcpSocket->connectToHost(serverAddress, serverPort);
    textEdit->append(tr("Trying %1:%2...").arg(serverAddress.toString()).arg(serverPort));
    timer->start(5000);
}

void MainWindow::clientDisconnect()
{
    editIP->setEnabled(true);
    editPort->setEnabled(true);
    editName->setEnabled(true);
    combo->clear();
    buttonConnect->setEnabled(true);
    buttonDisconnect->setEnabled(false);
    textEdit->append("Disconnected from host.");
    tcpSocket->disconnectFromHost();
    time = QTime(0, 0, 2, 0);
    timer->stop();
}

void MainWindow::onSend()
{
    QString key = "message", line = lineSend->text();
    if (line.isEmpty())
    {
        textEdit->append("]");
        lineSend->clear();
    }
    else
    {
        textEdit->append(line);
        if (tcpSocket->state() == QTcpSocket::ConnectedState)
        {
            QByteArray block;
            QDataStream out(&block, QIODevice::WriteOnly);
            out << (quint16)0;
            out << key << line;
            out.device()->seek(0);
            out << (quint16)(block.size() - sizeof(quint16));
            tcpSocket->write(block);
            tcpSocket->flush();
        }
    }
}

void MainWindow::onRead()
{
    QDataStream in(tcpSocket);
    blockSize = 0;
    if (blockSize == 0)
    {
        if (tcpSocket->bytesAvailable() < (int)sizeof(quint16))
        {
            return;
        }
        in >> blockSize;
        qDebug() << "blockSize" << blockSize;
    }
    if (tcpSocket->bytesAvailable() < blockSize)
    {
        return;
    }
    QString key, stream;
    in >> key >> stream;
    if (key == "message")
    {
        textEdit->append(stream);
    }
    else if (key == "userlist")
    {
        QStringList list = stream.split(",");
        combo->clear();
        combo->addItems(list);
    }
}

void MainWindow::onConnected()
{
    QString key = "username", data = editName->text();
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << (quint16)0;
    out << key << data;
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));
    tcpSocket->write(block);
    tcpSocket->flush();
    textEdit->append(tr("Connected to %1:%2").arg(serverAddress.toString()).arg(serverPort));
}

void MainWindow::checkingConnection()
{
    if (tcpSocket->state() == QTcpSocket::UnconnectedState)
    {
        time = time.addSecs(-1);
        int secs = time.second();
        if (secs > 0)
        {
            textEdit->append(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss ") + tr("No connection to %1:%2").arg(serverAddress.toString()).arg(serverPort));
        }
        else
        {
            timer->stop();
            textEdit->append(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss ") + tr("Connection timeout."));
        }
    }
}
