#include <QtWidgets>
#include <QtNetwork>

#include <stdlib.h>

#include "server.h"
#include "bonjourserviceregister.h"
#include "utility.h"

Server::Server(QWidget *parent)
    : QDialog(parent)
{
    setWindowModality(Qt::ApplicationModal);
    statusLabel = new QLabel;
    quitButton = new QPushButton(tr("Quit"));
    quitButton->setAutoDefault(false);

    tcpServer = new QTcpServer(this);
    if (!tcpServer->listen()) {
        QMessageBox::critical(this, tr("Bonjour Server"),
                              tr("Unable to start the server: %1.")
                              .arg(tcpServer->errorString()));
        close();
        return;
    }

    statusLabel->setText(tr("The server is running on port %1.\n"
                            "Run the Bonjour Client now.")
                         .arg(tcpServer->serverPort()));

    connect(quitButton, SIGNAL(clicked()), this, SLOT(close()));
    connect(tcpServer, SIGNAL(newConnection()), this, SLOT(readExchangeKey()));

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addStretch(1);
    buttonLayout->addWidget(quitButton);
    buttonLayout->addStretch(1);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(statusLabel);
    mainLayout->addLayout(buttonLayout);
    setLayout(mainLayout);
    
    bonjourRegister = new BonjourServiceRegister(this);
    bonjourRegister->registerService(BonjourRecord(tr("Bonjour Server on %1").arg(QHostInfo::localHostName()),
                                            QLatin1String("_bananapeer._tcp"), QString()),
                                     tcpServer->serverPort());
    setWindowTitle(tr("Bonjour Server"));
}

void Server::readExchangeKey() {
    qDebug() << "Received connection request from peer";
    QTcpSocket *tcpSocket = tcpServer->nextPendingConnection();
    A = getExchangeKey();

    int b = getRandom();
    quint64 B = Utility::getExchangeKey(b);
    qDebug() << "Key to exchange: " << B;

    //send exchange key
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_0);
    out << B;
    tcpSocket->write(block);

    quint64 secret = Utility::powmodp(A,b);
    qDebug() << "Secret key = " << secret;
}

quint64 Server::getExchangeKey()
{
    bool ok;
    QString text = NULL;
    do {
        text = QInputDialog::getText(this, tr("PIN entry"),
                                         tr("Insert PIN displayed on peer:"), QLineEdit::Normal,
                                         tr(""), &ok);
    } while (!(ok && !text.isEmpty()));
    //qDebug() << "Pin entered: " << text;

    return text.toULongLong();
}

int Server::getRandom()
{
    srand(time(NULL));
    int num = rand() % 90 + 10; //from 10 to 100
    return num;
}
