#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    labelIP = new QLabel(this);
    labelIP->setText("IP Адрес");
    combo = new QComboBox(this);
    labelPort = new QLabel(this);
    labelPort->setText("TCP Порт");
    editPort = new QLineEdit(this);
    editPort->setMaxLength(5);
    editPort->setText("1234");
    editPort->setValidator(new QIntValidator(1, 65535, this));
    check = new QCheckBox(this);
    check->setChecked(true);
    check->setText("Лог");
    labelTime = new QLabel(this);
    labelTime->setText("Время работы");
    editTime = new QLineEdit(this);
    editTime->setText("00:00:00");
    editTime->setDisabled(true);
    labelClients = new QLabel(this);
    labelClients->setText("Пользователи");
    editClients = new QLineEdit(this);
    editClients->setText("0");
    editClients->setDisabled(true);
    buttonStart = new QPushButton(this);
    buttonStart->setText("Старт");
    connect(buttonStart, SIGNAL(clicked()), this, SLOT(startServer()));
    buttonStop = new QPushButton(this);
    buttonStop->setEnabled(false);
    buttonStop->setText("Стоп");
    connect(buttonStop, SIGNAL(clicked()), this, SLOT(stopServer()));
    textEdit = new QTextEdit(this);
    textEdit->setReadOnly(true);
    connect(textEdit, SIGNAL(textChanged()), this, SLOT(onLog()));
    lineSend = new QLineEdit(this);
    connect(lineSend, SIGNAL(returnPressed()), this, SLOT(onSend()));
    buttonSend = new QPushButton(this);
    buttonSend->setText("Отправить");
    connect(buttonSend, SIGNAL(clicked()), this, SLOT(onSend()));
    QGridLayout *glayout = new QGridLayout;
    glayout->addWidget(labelIP, 0, 0);
    glayout->addWidget(combo, 0, 1);
    glayout->addWidget(labelPort, 1, 0);
    glayout->addWidget(editPort, 1, 1);
    glayout->addWidget(check, 0, 2);
    glayout->addWidget(labelTime, 0, 3);
    glayout->addWidget(editTime, 0, 4);
    glayout->addWidget(labelClients, 1, 3);
    glayout->addWidget(editClients, 1, 4);
    glayout->addWidget(buttonStart, 0, 5);
    glayout->addWidget(buttonStop, 1, 5);
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
    tcpServer = new QTcpServer(this);
    tcpSocket = new QTcpSocket(this);
    time = QTime(0, 0, 0, 0);
    timer = new QTimer(this);
    this->getIP();
    connect(tcpServer, SIGNAL(newConnection()), this, SLOT(incomingConnection()));
    connect(timer, SIGNAL(timeout()), this, SLOT(timeCount()));
}

MainWindow::~MainWindow()
{
}

void MainWindow::getIP()
{
    foreach (const QHostAddress &address, QNetworkInterface::allAddresses())
    {
        if (address.protocol() == QAbstractSocket::IPv4Protocol)
        {
            combo->addItem(address.toString());
        }
    }
}

void MainWindow::startServer()
{
    if (combo->currentIndex() == 0)
    {
        serverAddress.setAddress(combo->currentText());
    }
    else if (combo->currentIndex() == 1)
    {
        serverAddress.setAddress(combo->currentText());
    }
    serverPort = editPort->text().toInt();
    combo->setEnabled(false);
    editPort->setEnabled(false);
    check->setEnabled(false);
    buttonStart->setEnabled(false);
    buttonStop->setEnabled(true);
    tcpServer->listen(serverAddress, serverPort);
    if (!tcpServer->isListening())
    {
        qDebug() << "Server is not listening.";
        textEdit->append(tcpServer->errorString());
    }
    else
    {
        qDebug() << "Server is listening.";
        textEdit->append(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss ") + tr("Server started on %1:%2").arg(serverAddress.toString()).arg(serverPort));
    }
    timer->start(1000);
}

void MainWindow::stopServer()
{
    combo->setEnabled(true);
    editPort->setEnabled(true);
    check->setEnabled(true);
    editTime->setText("00:00:00");
    editClients->setText("0");
    buttonStart->setEnabled(true);
    buttonStop->setEnabled(false);
    textEdit->append(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss ") + "Server stopped.");
    time = QTime(0, 0, 0, 0);
    timer->stop();
    QString key = "message", data = "Connection closed.";
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << (quint16)0;
    out << key << data;
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));
    foreach (QTcpSocket *socket, clients)
    {
        socket->write(block);
        socket->flush();
        socket->close();
        socket->deleteLater();
    }
    tcpServer->close();
}

void MainWindow::onLog()
{
    if (check->isChecked() && tcpServer->isListening())
    {
        QFile file("chatserver.log");
        file.open(QIODevice::WriteOnly | QIODevice::Append);
        QTextStream textStream(&file);
        textStream << textEdit->toPlainText() << "\n";
        file.flush();
        file.close();
    }
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
        line = "Server:" + line;
        QByteArray block;
        QDataStream out(&block, QIODevice::WriteOnly);
        out << (quint16)0;
        out << key << line;
        out.device()->seek(0);
        out << (quint16)(block.size() - sizeof(quint16));
        if (clients.size() > 0)
        {
            foreach (QTcpSocket *socket, clients)
            {
                socket->write(block);
                socket->flush();
            }
        }
    }
}

void MainWindow::incomingConnection()
{
    tcpSocket = tcpServer->nextPendingConnection();
    connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(onRead()));
    connect(tcpSocket, SIGNAL(disconnected()), this, SLOT(onDisconnected()));
}

void MainWindow::onRead()
{
    QTcpSocket *tcpSocket = (QTcpSocket*)sender();
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
        closeConnection();
        return;
    }
    QString key, stream, name, data;
    in >> key >> stream;
    if (key == "username")
    {
        clients.insert(tcpSocket);
        users.insert(tcpSocket, stream);
        setUserList();
        for (auto it = users.begin(); it != users.end(); it++)
        {
            if (it.key() == tcpSocket)
            {
                name = it.value();
                textEdit->append(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss ") + tr("Client %1 connected from %2:%3").arg(name).arg(tcpSocket->peerAddress().toString()).arg(tcpSocket->peerPort()));
                break;
            }
        }
        key = "message";
        data = tr("Client %1 connected.").arg(name);
        QByteArray block;
        QDataStream out(&block, QIODevice::WriteOnly);
        out << (quint16)0;
        out << key << data;
        out.device()->seek(0);
        out << (quint16)(block.size() - sizeof(quint16));
        foreach (QTcpSocket *socket, clients)
        {
            if (tcpSocket != socket)
            {
                socket->write(block);
                socket->flush();
            }
        }
    }
    else if (key == "message")
    {
        for (auto it = users.begin(); it != users.end(); it++)
        {
            if (it.key() == tcpSocket)
            {
                name = it.value();
                textEdit->append(name + ":" + stream);
                break;
            }
        }
        key = "message";
        data = name + ":" + stream;
        QByteArray block;
        QDataStream out(&block, QIODevice::WriteOnly);
        out << (quint16)0;
        out << key << data;
        out.device()->seek(0);
        out << (quint16)(block.size() - sizeof(quint16));
        foreach (QTcpSocket *socket, clients)
        {
            if (tcpSocket != socket)
            {
                socket->write(block);
                socket->flush();
            }
        }
    }
}

void MainWindow::onDisconnected()
{
    QTcpSocket *tcpSocket = (QTcpSocket*)sender();
    QString name, key, data;
    for (auto it = users.begin(); it != users.end(); it++)
    {
        if (it.key() == tcpSocket)
        {
            name = it.value();
            textEdit->append(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss ") + tr("Client %1 disconnected.").arg(name));
            key = "message";
            data = tr("Client %1 disconnected.").arg(name);
            QByteArray block;
            QDataStream out(&block, QIODevice::WriteOnly);
            out << (quint16)0;
            out << key << data;
            out.device()->seek(0);
            out << (quint16)(block.size() - sizeof(quint16));
            foreach (QTcpSocket *socket, clients)
            {
                socket->write(block);
                socket->flush();
            }
            clients.remove(tcpSocket);
            users.remove(tcpSocket);
            setUserList();
            break;
        }
    }
    tcpSocket->close();
    tcpSocket->deleteLater();
}

void MainWindow::timeCount()
{
    time = time.addSecs(1);
    editTime->setText(time.toString("hh:mm:ss"));
}

void MainWindow::closeConnection()
{
    QString address, port, str;
    int nPort = tcpSocket->peerPort();
    port = QString::number(nPort);
    address = tcpSocket->peerAddress().toString();
    str = address + ":" + port;
    qDebug() << "Connection from" << str;
    tcpSocket->flush();
    tcpSocket->disconnectFromHost();
}

void MainWindow::setUserList()
{
    QString key, data, str;
    int num = clients.size();
    str = QString::number(num);
    editClients->setText(str);
    QStringList userList;
    foreach (QString user, users.values())
    {
        userList << user;
    }
    key = "userlist";
    data = userList.join(",") + "\n";
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << (quint16)0;
    out << key << data;
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));
    foreach (QTcpSocket *socket, clients)
    {
        socket->write(block);
        socket->flush();
    }
}
