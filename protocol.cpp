#include "protocol.h"

ProtocolRead::ProtocolRead(QSerialPort *serialPort)
{
    port = serialPort;
}

ProtocolRead::~ProtocolRead()
{

}

double ProtocolRead::channelWaveform(Channel ch)
{
    port->write("R" + QByteArray(1,ch) + "W\n");
    port->waitForBytesWritten(PROTOCOL_READ_DELAY);
    port->waitForReadyRead(PROTOCOL_READ_DELAY);
    QByteArray read = port->readAll();
   return read.toDouble();
}

double ProtocolRead::channelFrequency(Channel ch)
{
    port->write("R" + QByteArray(1,ch) + "F\n");
    port->waitForBytesWritten(PROTOCOL_READ_DELAY);
    port->waitForReadyRead(PROTOCOL_READ_DELAY);
    QByteArray read = port->readAll();
    double retv = read.toDouble();
    if( ! retv ) retv = 1e+9;
    return retv;
}

double ProtocolRead::channelAmplitude(Channel ch)
{
    port->write("R" + QByteArray(1,ch) + "A\n");
    port->waitForBytesWritten(PROTOCOL_READ_DELAY);
    port->waitForReadyRead(PROTOCOL_READ_DELAY);
    QByteArray read = port->readAll();
    return read.toFloat() / 10000;
}

double ProtocolRead::channelOffset(Channel ch)
{
    port->write("R" + QByteArray(1,ch) + "O\n");
    port->waitForBytesWritten(PROTOCOL_READ_DELAY);
    port->waitForReadyRead(PROTOCOL_READ_DELAY);
    QByteArray read = port->readAll() ;
    double retv = read.toDouble();
    if( retv > 12500 ) retv = retv - 0xffffffff - 1;
    return retv / 1000;
}

double ProtocolRead::channelDuty(Channel ch)
{
    port->write("R" + QByteArray(1,ch) + "D\n");
    port->waitForBytesWritten(PROTOCOL_READ_DELAY);
    port->waitForReadyRead(PROTOCOL_READ_DELAY);
    QByteArray read = port->readAll();
    return read.toDouble() / 1000;
}

double ProtocolRead::channelPhase(Channel ch)
{
    port->write("R" + QByteArray(1,ch) + "P\n");
    port->waitForBytesWritten(PROTOCOL_READ_DELAY);
    port->waitForReadyRead(PROTOCOL_READ_DELAY);
    QByteArray read = port->readAll();
    return read.toFloat() / 1000;
}

double ProtocolRead::channelOnOff(Channel ch)
{
    port->write("R" + QByteArray(1,ch) + "N\n");
    port->waitForBytesWritten(PROTOCOL_READ_DELAY);
    port->waitForReadyRead(PROTOCOL_READ_DELAY);
    QByteArray read = port->readAll();
    return read.toDouble();
}

double ProtocolRead::modulationTriggerMode()
{
    port->write("RPF\n");
    port->waitForBytesWritten(PROTOCOL_READ_DELAY);
    port->waitForReadyRead(PROTOCOL_READ_DELAY);
    QByteArray read = port->readAll();
    return read.toDouble();
}

double ProtocolRead::modulationTriggerSource()
{
    port->write("RPM\n");
    port->waitForBytesWritten(PROTOCOL_READ_DELAY);
    port->waitForReadyRead(PROTOCOL_READ_DELAY);
    QByteArray read = port->readAll();
    return read.toDouble();
}

double ProtocolRead::modulationSecondaryFrequencyFSK()
{
    port->write("RFK\n");
    port->waitForBytesWritten(PROTOCOL_READ_DELAY);
    port->waitForReadyRead(PROTOCOL_READ_DELAY);
    QByteArray read = port->readAll();
    double retv = read.toDouble();
    if( ! retv ) retv = 100000;
    return retv;
}

double ProtocolRead::modulationMainWaveTrigerPulseAmount()
{
    port->write("RPN\n");
    port->waitForBytesWritten(PROTOCOL_READ_DELAY);
    port->waitForReadyRead(PROTOCOL_READ_DELAY);
    QByteArray read = port->readAll();
    return read.toDouble();
}

double ProtocolRead::modulationainMainWaveRateAM()
{
    port->write("RPR\n");
    port->waitForBytesWritten(PROTOCOL_READ_DELAY);
    port->waitForReadyRead(PROTOCOL_READ_DELAY);
    QByteArray read = port->readAll();
    return read.toDouble() / 10;
}

double ProtocolRead::modulationainMainWaveFrequencyOffsetFM()
{
    port->write("RFM\n");
    port->waitForBytesWritten(PROTOCOL_READ_DELAY);
    port->waitForReadyRead(PROTOCOL_READ_DELAY);
    QByteArray read = port->readAll();
    return read.toDouble();
}

double ProtocolRead::modulationainMainWavePhaseOffsetPM()
{
    port->write("RPP\n");
    port->waitForBytesWritten(PROTOCOL_READ_DELAY);
    port->waitForReadyRead(PROTOCOL_READ_DELAY);
    QByteArray read = port->readAll();
    return read.toDouble() / 1000;
}

double ProtocolRead::measurmentGateTime()
{
    port->write("RCG\n");
    port->waitForBytesWritten(PROTOCOL_READ_DELAY);
    port->waitForReadyRead(PROTOCOL_READ_DELAY);
    QByteArray read = port->readAll();
    return read.toDouble();
}

double ProtocolRead::measurmentExternalFrequency()
{
    port->write("RCF\n");
    port->waitForBytesWritten(PROTOCOL_READ_DELAY);
    port->waitForReadyRead(PROTOCOL_READ_DELAY);
    QByteArray read = port->readAll();
    return read.toDouble();
}

double ProtocolRead::measurmentExternalCountingValue()
{
    port->write("RCC\n");
    port->waitForBytesWritten(PROTOCOL_READ_DELAY);
    port->waitForReadyRead(PROTOCOL_READ_DELAY);
    QByteArray read = port->readAll();
    return read.toDouble();
}

double ProtocolRead::measurmentExternalCountingPeriod()
{
    port->write("RCT\n");
    port->waitForBytesWritten(PROTOCOL_READ_DELAY);
    port->waitForReadyRead(PROTOCOL_READ_DELAY);
    QByteArray read = port->readAll();
    return read.toDouble();
}

double ProtocolRead::measurmentExternalPositivePulseWidth()
{
    port->write("RC+\n");
    port->waitForBytesWritten(PROTOCOL_READ_DELAY);
    port->waitForReadyRead(PROTOCOL_READ_DELAY);
    QByteArray read = port->readAll();
    return read.toDouble();
}

double ProtocolRead::measurmentExternalNegativePulseWidth()
{
    port->write("RC-\n");
    port->waitForBytesWritten(PROTOCOL_READ_DELAY);
    port->waitForReadyRead(PROTOCOL_READ_DELAY);
    QByteArray read = port->readAll();
    return read.toDouble();
}

double ProtocolRead::measurmentExternalDutyCycle()
{
    port->write("RCD\n");
    port->waitForBytesWritten(PROTOCOL_READ_DELAY);
    port->waitForReadyRead(PROTOCOL_READ_DELAY);
    QByteArray read = port->readAll();
    return read.toDouble() / 10;
}

//DOWN HERE NOT TESTED
double ProtocolRead::systemSynchronizationInformation()
{
    port->write("RSA\n");
    port->waitForBytesWritten(PROTOCOL_READ_DELAY);
    port->waitForReadyRead(PROTOCOL_READ_DELAY);
    QByteArray read = port->readAll();
    return read.toDouble();
}

double ProtocolRead::systemBuzzerStatus()
{
    port->write("RBZ\n");
    port->waitForBytesWritten(PROTOCOL_READ_DELAY);
    port->waitForReadyRead(PROTOCOL_READ_DELAY);
    QByteArray read = port->readAll();
    return read.toDouble();
}

double ProtocolRead::systemUplinkMode()
{
    port->write("RMS\n");
    port->waitForBytesWritten(PROTOCOL_READ_DELAY);
    port->waitForReadyRead(PROTOCOL_READ_DELAY);
    QByteArray read = port->readAll();
    return read.toDouble();
}

double ProtocolRead::systemLocalUplinkStatus()
{
    port->write("RUL\n");
    port->waitForBytesWritten(PROTOCOL_READ_DELAY);
    port->waitForReadyRead(PROTOCOL_READ_DELAY);
    QByteArray read = port->readAll();
    return read.toDouble();
}

 QString ProtocolRead::systemLocalID()
{
    port->write("UID\n");
    port->waitForBytesWritten(PROTOCOL_READ_DELAY);
    port->waitForReadyRead(PROTOCOL_READ_DELAY);
    QByteArray read = port->readAll();
    return QString::fromStdString(read.toStdString()).remove("\n");
}

QString ProtocolRead::systemLocalModel()
{
    port->write("UMO\n");
    port->waitForBytesWritten(PROTOCOL_READ_DELAY);
    port->waitForReadyRead(PROTOCOL_READ_DELAY);
    QByteArray read = port->readAll();
    return QString(read).remove("\n");
}

//UP HERE NOT TESTED

ProtocolWrite::ProtocolWrite(QSerialPort *serialPort)
{
    port = serialPort;
}

ProtocolWrite::~ProtocolWrite()
{

}

void ProtocolWrite::channelWaveform(Channel ch, double value)
{
    port->write("W" + QByteArray(1,ch) + "W" + QString::number(value,'f',0).toLocal8Bit() + "\n");
    port->waitForBytesWritten(PROTOCOL_WRITE_DELAY);
    port->waitForReadyRead(PROTOCOL_WRITE_DELAY);
    qDebug() << __FUNCTION__ <<  " RETURN: " << port->readAll();
}

void ProtocolWrite::channelFrequency(Channel ch, double value) {
    QString val = QString::number(value,'f',0);
    while( val.length() < 15 ) val.prepend('0');
    port->write("W" + QByteArray(1,ch) + "F" + val.toLocal8Bit() + "\n");
    port->waitForBytesWritten(PROTOCOL_WRITE_DELAY);
    port->waitForReadyRead(PROTOCOL_WRITE_DELAY);
    qDebug() << __FUNCTION__ <<  " RETURN: " << port->readAll();
}

void ProtocolWrite::channelAmplitude(Channel ch, double value)
{
    QString val = QString::number(value);
    port->write("W" + QByteArray(1,ch) + "A" + val.toLocal8Bit() + "\n");
    port->waitForBytesWritten(PROTOCOL_WRITE_DELAY);
    port->waitForReadyRead(PROTOCOL_WRITE_DELAY);
    qDebug() << __FUNCTION__ <<  " RETURN: " << port->readAll();
}

void ProtocolWrite::channelOffset(Channel ch, double value)
{
    QString val = QString::number(value);
    port->write("W" + QByteArray(1,ch) + "O" + val.toLocal8Bit() + "\n");
    port->waitForBytesWritten(PROTOCOL_WRITE_DELAY);
    port->waitForReadyRead(PROTOCOL_WRITE_DELAY);
    qDebug() << __FUNCTION__ <<  " RETURN: " << port->readAll();
}

void ProtocolWrite::channelDuty(Channel ch, double value)
{
    QString val = QString::number(value,'f',3);
    port->write("W" + QByteArray(1,ch) + "D" + val.toLocal8Bit() + "\n");
    port->waitForBytesWritten(PROTOCOL_WRITE_DELAY);
    port->waitForReadyRead(PROTOCOL_WRITE_DELAY);
    qDebug() << __FUNCTION__ <<  " RETURN: " << port->readAll();
}

void ProtocolWrite::channelPhase(Channel ch, double value)
{
    QString val = QString::number(value);
    port->write("W" + QByteArray(1,ch) + "P" + val.toLocal8Bit() + "\n");
    port->waitForBytesWritten(PROTOCOL_WRITE_DELAY);
    port->waitForReadyRead(PROTOCOL_WRITE_DELAY);
    qDebug() << __FUNCTION__ <<  " RETURN: " << port->readAll();
}

void ProtocolWrite::channelOnOff(Channel ch, double value)
{
    QString val = QString::number(value,'f',0);
    port->write("W" + QByteArray(1,ch) + "N" + val.toLocal8Bit() + "\n");
    port->waitForBytesWritten(PROTOCOL_WRITE_DELAY);
    port->waitForReadyRead(PROTOCOL_WRITE_DELAY);
    qDebug() << __FUNCTION__ <<  " RETURN: " << port->readAll();
}

void ProtocolWrite::modulationTriggerMode(double value)
{
    QString val = QString::number(value,'f',0);
    port->write("WPF" + val.toLocal8Bit() + "\n");
    port->waitForBytesWritten(PROTOCOL_WRITE_DELAY);
    port->waitForReadyRead(PROTOCOL_WRITE_DELAY);
    qDebug() << __FUNCTION__ <<  " RETURN: " << port->readAll();
}

void ProtocolWrite::modulationTriggerSource(double value)
{
    QString val = QString::number(value,'f',0);
    port->write("WPM" + val.toLocal8Bit() + "\n");
    port->waitForBytesWritten(PROTOCOL_WRITE_DELAY);
    port->waitForReadyRead(PROTOCOL_WRITE_DELAY);
    qDebug() << __FUNCTION__ <<  " RETURN: " << port->readAll();
}


void ProtocolWrite::modulationSecondaryFrequencyFSK(double value)
{
    QString val = QString::number(value,'f',1);
    port->write("WFK" + val.toLocal8Bit() + "\n");
    port->waitForBytesWritten(PROTOCOL_WRITE_DELAY);
    port->waitForReadyRead(PROTOCOL_WRITE_DELAY);
    qDebug() << __FUNCTION__ <<  " RETURN: " << port->readAll();
}

void ProtocolWrite::modulationMainWaveTrigerPulseAmount(double value)
{
    QString val = QString::number(value,'f',0);
    port->write("WPN" + val.toLocal8Bit() + "\n");
    port->waitForBytesWritten(PROTOCOL_WRITE_DELAY);
    port->waitForReadyRead(PROTOCOL_WRITE_DELAY);
    qDebug() << __FUNCTION__ <<  " RETURN: " << port->readAll();
}

void ProtocolWrite::modulationainManualTriggerSource()
{
    port->write("WPO\n");
    port->waitForBytesWritten(PROTOCOL_WRITE_DELAY);
    port->waitForReadyRead(PROTOCOL_WRITE_DELAY);
    qDebug() << __FUNCTION__ <<  " RETURN: " << port->readAll();
}

void ProtocolWrite::modulationainMainWaveRateAM(double value)
{
    QString val = QString::number(value);
    port->write("WPR" + val.toLocal8Bit() + "\n");
    port->waitForBytesWritten(PROTOCOL_WRITE_DELAY);
    port->waitForReadyRead(PROTOCOL_WRITE_DELAY);
    qDebug() << __FUNCTION__ <<  " RETURN: " << port->readAll();
}

void ProtocolWrite::modulationainMainWaveFrequencyOffsetFM(double value)
{
    QString val = QString::number(value,'f',1);
    port->write("WFM" + val.toLocal8Bit() + "\n");
    port->waitForBytesWritten(PROTOCOL_WRITE_DELAY);
    port->waitForReadyRead(PROTOCOL_WRITE_DELAY);
    qDebug() << __FUNCTION__ <<  " RETURN: " << port->readAll();
}

void ProtocolWrite::modulationainMainWavePhaseOffsetPM(double value)
{
    QString val = QString::number(value);
    port->write("WPP" + val.toLocal8Bit() + "\n");
    port->waitForBytesWritten(PROTOCOL_WRITE_DELAY);
    port->waitForReadyRead(PROTOCOL_WRITE_DELAY);
    qDebug() << __FUNCTION__ <<  " RETURN: " << port->readAll();
}

void ProtocolWrite::measurmentCouplingMode(double value)
{
    QString val = QString::number(value,'f');
    port->write("WCC" + val.toLocal8Bit() + "\n");
    port->waitForBytesWritten(PROTOCOL_WRITE_DELAY);
    port->waitForReadyRead(PROTOCOL_WRITE_DELAY);
    qDebug() << __FUNCTION__ <<  " RETURN: " << port->readAll();
}

void ProtocolWrite::measurmentResetCounter(double value)
{
    QString val = QString::number(value,'f');
    port->write("WCZ" + val.toLocal8Bit() + "\n");
    port->waitForBytesWritten(PROTOCOL_WRITE_DELAY);
    port->waitForReadyRead(PROTOCOL_WRITE_DELAY);
    qDebug() << __FUNCTION__ <<  " RETURN: " << port->readAll();
}

void ProtocolWrite::measurmentPause(double value)
{
    QString val = QString::number(value,'f');
    port->write("WCP" + val.toLocal8Bit() + "\n");
    port->waitForBytesWritten(PROTOCOL_WRITE_DELAY);
    port->waitForReadyRead(PROTOCOL_WRITE_DELAY);
    qDebug() << __FUNCTION__ <<  " RETURN: " << port->readAll();
}

void ProtocolWrite::measurmentGateTime(double value)
{
    QString val = QString::number(value,'f');
    port->write("WCG" + val.toLocal8Bit() + "\n");
    port->waitForBytesWritten(PROTOCOL_WRITE_DELAY);
    port->waitForReadyRead(PROTOCOL_WRITE_DELAY);
    qDebug() << __FUNCTION__ <<  " RETURN: " << port->readAll();
}

void ProtocolWrite::sweepObject(double value)
{
    QString val = QString::number(value,'f',0);
    port->write("SOB" + val.toLocal8Bit() + "\n");
    port->waitForBytesWritten(PROTOCOL_WRITE_DELAY);
    port->waitForReadyRead(PROTOCOL_WRITE_DELAY);
    qDebug() << __FUNCTION__ <<  " RETURN: " << port->readAll();
}

void ProtocolWrite::sweepStart(double value)
{
    QString val = QString::number(value,'f',1);
    port->write("SST" + val.toLocal8Bit() + "\n");
    port->waitForBytesWritten(PROTOCOL_WRITE_DELAY);
    port->waitForReadyRead(PROTOCOL_WRITE_DELAY);
    qDebug() << __FUNCTION__ <<  " RETURN: " << port->readAll();
}

void ProtocolWrite::sweepEnd(double value)
{
    QString val = QString::number(value,'f',1);
    port->write("SEN" + val.toLocal8Bit() + "\n");
    port->waitForBytesWritten(PROTOCOL_WRITE_DELAY);
    port->waitForReadyRead(PROTOCOL_WRITE_DELAY);
    qDebug() << __FUNCTION__ <<  " RETURN: " << port->readAll();
}

void ProtocolWrite::sweepTime(double value)
{
    QString val = QString::number(value);
    port->write("STI" + val.toLocal8Bit() + "\n");
    port->waitForBytesWritten(PROTOCOL_WRITE_DELAY);
    port->waitForReadyRead(PROTOCOL_WRITE_DELAY);
    qDebug() << __FUNCTION__ <<  " RETURN: " << port->readAll();
}

void ProtocolWrite::sweepMode(double value)
{
    QString val = QString::number(value,'f',0);
    port->write("SMO" + val.toLocal8Bit() + "\n");
    port->waitForBytesWritten(PROTOCOL_WRITE_DELAY);
    port->waitForReadyRead(PROTOCOL_WRITE_DELAY);
    qDebug() << __FUNCTION__ <<  " RETURN: " << port->readAll();
}

void ProtocolWrite::sweepDirection(double value)
{
    QString val = QString::number(value,'f',0);
    port->write("SMD" + val.toLocal8Bit() + "\n");
    port->waitForBytesWritten(PROTOCOL_WRITE_DELAY);
    port->waitForReadyRead(PROTOCOL_WRITE_DELAY);
    qDebug() << __FUNCTION__ <<  " RETURN: " << port->readAll();
}

void ProtocolWrite::sweepStartStop(double value)
{
    QString val = QString::number(value,'f',0);
    port->write("SBE" + val.toLocal8Bit() + "\n");
    port->waitForBytesWritten(PROTOCOL_WRITE_DELAY);
    port->waitForReadyRead(PROTOCOL_WRITE_DELAY);
    qDebug() << __FUNCTION__ <<  " RETURN: " << port->readAll();
}

void ProtocolWrite::sweepSource(double value)
{
    QString val = QString::number(value,'f',0);
    port->write("SXY" + val.toLocal8Bit() + "\n");
    port->waitForBytesWritten(PROTOCOL_WRITE_DELAY);
    port->waitForReadyRead(PROTOCOL_WRITE_DELAY);
    qDebug() << __FUNCTION__ <<  " RETURN: " << port->readAll();
}

//DOWN HERE NOT TESTED
void ProtocolWrite::systemSaveChannelsParameters(double value)
{
    QString val = QString::number(value,'f',0);
    port->write("USN" + val.toLocal8Bit() + "\n");
    port->waitForBytesWritten(PROTOCOL_WRITE_DELAY);
    port->waitForReadyRead(PROTOCOL_WRITE_DELAY);
    qDebug() << __FUNCTION__ <<  " RETURN: " << port->readAll();
}

void ProtocolWrite::systemLoadChannelsParameters(double value)
{
    QString val = QString::number(value,'f',0);
    port->write("ULN" + val.toLocal8Bit() + "\n");
    port->waitForBytesWritten(PROTOCOL_WRITE_DELAY);
    port->waitForReadyRead(PROTOCOL_WRITE_DELAY);
    qDebug() << __FUNCTION__ <<  " RETURN: " << port->readAll();
}

void ProtocolWrite::systemAddSynchronizationModde(double value)
{
    QString val = QString::number(value,'f',0);
    port->write("USA" + val.toLocal8Bit() + "\n");
    port->waitForBytesWritten(PROTOCOL_WRITE_DELAY);
    port->waitForReadyRead(PROTOCOL_WRITE_DELAY);
    qDebug() << __FUNCTION__ <<  " RETURN: " << port->readAll();
}

void ProtocolWrite::systemCancelSynchronizationMode(double value)
{
    QString val = QString::number(value,'f',0);
    port->write("USD" + val.toLocal8Bit() + "\n");
    port->waitForBytesWritten(PROTOCOL_WRITE_DELAY);
    port->waitForReadyRead(PROTOCOL_WRITE_DELAY);
    qDebug() << __FUNCTION__ <<  " RETURN: " << port->readAll();
}

void ProtocolWrite::systemSetBuzzerOnOff(double value)
{
    QString val = QString::number(value,'f',0);
    port->write("UBZ" + val.toLocal8Bit() + "\n");
    port->waitForBytesWritten(PROTOCOL_WRITE_DELAY);
    port->waitForReadyRead(PROTOCOL_WRITE_DELAY);
    qDebug() << __FUNCTION__ <<  " RETURN: " << port->readAll();
}

void ProtocolWrite::systemSetUplinkMode(double value)
{
    QString val = QString::number(value,'f',0);
    port->write("UMS" + val.toLocal8Bit() + "\n");
    port->waitForBytesWritten(PROTOCOL_WRITE_DELAY);
    port->waitForReadyRead(PROTOCOL_WRITE_DELAY);
    qDebug() << __FUNCTION__ <<  " RETURN: " << port->readAll();
}

void ProtocolWrite::systemSetLocalUplinkStatus(double value)
{
    QString val = QString::number(value,'f',0);
    port->write("UUL" + val.toLocal8Bit() + "\n");
    port->waitForBytesWritten(PROTOCOL_WRITE_DELAY);
    port->waitForReadyRead(PROTOCOL_WRITE_DELAY);
    qDebug() << __FUNCTION__ <<  " RETURN: " << port->readAll();
}
//UP HERE NOT TESTED

void ProtocolWrite::awgUpload(uint channel, uint slot, double *linePoints, uint linePointsSize)
{

    qDebug() << __FUNCTION__ << "DDS_WAVE channel: " << channel << " slot: " << slot << "RETURN: " << port->readAll();
    qDebug() << __FUNCTION__ << "SELECTED " << slot;

    port->write("DDS_WAVE" + QString::number(slot).toLocal8Bit() + "\n");
    port->waitForBytesWritten(PROTOCOL_WRITE_DELAY);
    port->waitForReadyRead(PROTOCOL_WRITE_DELAY);

    QByteArray dataArray;
    for(uint n = 0; n < linePointsSize; n++ ) {
        double data = linePoints[n] * -1;
        if (data < -1.0) {
          data = -1.0;
        } else if (data > 1.0) {
          data = 1.0;
        }
        int data14 = (int)((data+1.0)/2.0*16383 + 0.5);
        char upper7 = data14 >> 7;
        char lower7 = data14 & 0x7f;
        dataArray.append(lower7);
        dataArray.append(upper7);
    };

    port->write(dataArray);
    port->waitForBytesWritten(PROTOCOL_WRITE_DELAY);

    port->waitForReadyRead(PROTOCOL_WRITE_DELAY);
    qDebug() << __FUNCTION__ <<  " RETURN: " << port->readAll();

    port->waitForReadyRead(PROTOCOL_WRITE_DELAY);
    qDebug() << __FUNCTION__ <<  " RETURN: " << port->readAll();

}


