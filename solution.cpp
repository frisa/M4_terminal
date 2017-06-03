#include "solution.h"
#include <QtSerialPort/QtSerialPort>

Solution::Solution(QObject *parent) : QObject(parent)
{

}

Solution::Solution(QStringListModel * model) : m_model(model)
{

}

void Solution::updateSolutions(const QString text)
{
    QStringList list;
    for (int i = 1; i <= 3; ++i) {
        list << QString("%1:%2").arg(text).arg(i);
    }
    m_model->setStringList(list);
    qDebug("[janxx] %s called \n", __FUNCTION__);
}

void Solution::load()
{
    QSerialPort serialPort;
    QStringList logList;
    QByteArray ba;
    qint64 bytesWritten;

    serialPort.setPortName("/dev/ttyACM0");
    serialPort.setBaudRate(QSerialPort::Baud115200);
    serialPort.setDataBits(QSerialPort::Data8);
    serialPort.setParity(QSerialPort::NoParity);

    logList << QString("Loading the program");
    if (!serialPort.open(QIODevice::WriteOnly)) {
        logList << QString("Failed to open port");
    }
    else
    {
        logList << QString("The serial port has been connected sucessfuly");
    }

    ba.resize(8);
    ba[0] = 3;
    ba[1] = 2;
    ba[2] = 0x21; // command DOWNLOAD
    ba[3] = 0x00;
    ba[4] = 0x00;
    ba[5] = 0x00;
    ba[6] = 0x20;
    ba[7] = 123;
    logList << QString("Sending data to serial interface");
    for (int i=0; i<8; i++)
    {
        logList << QString("data[%1] = %2").arg(i).arg(ba[i]);
    }

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
    m_model->setStringList(logList);
}
