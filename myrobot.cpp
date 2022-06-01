#include "myrobot.h"
#include <QString>
#include <QTextCodec>

MyRobot::MyRobot(QObject *parent) : QObject(parent) {
    DataToSend.resize(9);
    DataToSend[0] = 0xFF;
    //qDebug() << DataToSend.at(0);
    DataToSend[1] = 0x07;
    DataToSend[2] = 0x64;
    DataToSend[3] = 0x0;
    DataToSend[4] = 0x64;
    DataToSend[5] = 0x0;
    DataToSend[6] = 0x50;
    short crc = MyRobot::Crc16(&DataToSend, 7);
    DataToSend[7] = crc;
    DataToSend[8] = crc >> 8;
    DataReceived.resize(21);
    TimerEnvoi = new QTimer();
    // setup signal and slot
    connect(TimerEnvoi, SIGNAL(timeout()), this, SLOT(MyTimerSlot())); //Send data to wifibot timer
}


void MyRobot::doConnect() {
    socket = new QTcpSocket(this); // socket creation
    connect(socket, SIGNAL(connected()),this, SLOT(connected()));
    connect(socket, SIGNAL(disconnected()),this, SLOT(disconnected()));
    connect(socket, SIGNAL(bytesWritten(qint64)),this, SLOT(bytesWritten(qint64)));
    connect(socket, SIGNAL(readyRead()),this, SLOT(readyRead()));
    qDebug() << "connecting..."; // this is not blocking call
    //socket->connectToHost("LOCALHOST", 15020);
    socket->connectToHost("192.168.1.106", 15020); // connection to wifibot
    // we need to wait...
    if(!socket->waitForConnected(5000)) {
        qDebug() << "Error: " << socket->errorString();
        return;
    }
    TimerEnvoi->start(75);



}

void MyRobot::disConnect() {
    TimerEnvoi->stop();
    socket->close();
}

void MyRobot::connected() {
    qDebug() << "connected..."; // Hey server, tell me about you.
}

void MyRobot::disconnected() {
    qDebug() << "disconnected...";
}

void MyRobot::bytesWritten(qint64 bytes) {
    qDebug() << bytes << " bytes written...";
}

void MyRobot::readyRead() {
    qDebug() << "reading..."; // read the data from the socket
    DataReceived = socket->readAll();
    emit updateUI(DataReceived);
    qDebug() << DataReceived[0] << DataReceived[1] << DataReceived[2];
}

void MyRobot::MyTimerSlot() {
    qDebug() << "Timer...";
    while(Mutex.tryLock());
    socket->write(DataToSend);
    Mutex.unlock();
}

quint16 MyRobot::Crc16(QByteArray *adresse, int taille)//unsigned char *adresse, unsigned char taille)
{
    unsigned int crc = 0xFFFF;
    unsigned int polynome = 0xA001;
    unsigned int cptOctet = 0;
    unsigned int cptBit = 0;
    unsigned int parity = 0;

    for(cptOctet = 1; cptOctet < taille; cptOctet++)
    {
        crc ^= (unsigned char)(adresse->data()[cptOctet]); //xor -> ^=
        for(cptBit = 0; cptBit <= 7; cptBit++)
        {
            parity = crc;
            crc >>= 1;
            if(parity % 2 == true)
                crc ^= polynome;
        }
    }
    return crc;
}


