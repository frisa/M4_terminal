#include "roothandler.h"
#include <QDebug>
#include <QtSerialPort/QtSerialPort>
#include <QByteArray>

RootHandler::RootHandler(QObject *parent) : QObject(parent)
{

}

void RootHandler::handleButtonSlot()
{
    QSerialPort serialPort;
    QString serialPortName = "/dev/ttyACM0";
    int serialPortBaudRate = QSerialPort::Baud115200;
    QByteArray ba;
    qint64 bytesWritten;

    serialPort.setPortName(serialPortName);
    serialPort.setBaudRate(QSerialPort::Baud115200);
    serialPort.setDataBits(QSerialPort::Data8);
    serialPort.setParity(QSerialPort::NoParity);
    if (!serialPort.open(QIODevice::WriteOnly)) {
        qDebug("Failed to open port error: %s\n", serialPort.errorString().toLatin1().data());
    }
    else
    {
        qDebug("The serial port %s has been connected sucessfuly\n", serialPortName.toLatin1().data());
        qDebug("Baudrate: %d\n", serialPortBaudRate);
    }

    ba.resize(8);
    ba[0] = 3;
    ba[1] = 2;
    //ba[2] = 0x20; // command PING
    ba[2] = 0x21; // command DOWNLOAD
    ba[3] = 0x00;
    ba[4] = 0x00;
    ba[5] = 0x00;
    ba[6] = 0x20;
    ba[7] = 123;

    bytesWritten = serialPort.write(ba);
    if (bytesWritten == -1) {
            qDebug("Failed to write the data to port , error: ");
        } else if (bytesWritten != ba.size()) {
            qDebug("Failed to write all the data to port ");
        } else if (!serialPort.waitForBytesWritten(5000)) {
            qDebug("Operation timed out or an error occurred for port ");
        }
    else
    {
        qDebug("Data sent to serial interface\n");
    }
}
