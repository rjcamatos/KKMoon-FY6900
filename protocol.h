#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <QDebug>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QByteArray>
#include <QPointF>
#include <types.h>

#define PROTOCOL_READ_DELAY 50
#define PROTOCOL_WRITE_DELAY 5000

class ProtocolRead
{
public:

    QSerialPort *port;

    ProtocolRead(QSerialPort *serialPort);
    ~ProtocolRead();

    //CHANNEL
    double channelWaveform(Channel ch);
    double channelFrequency(Channel ch);
    double channelAmplitude(Channel ch);
    double channelOffset(Channel ch);
    double channelDuty(Channel ch);
    double channelPhase(Channel ch);
    double channelOnOff(Channel ch);

    //MODULATION
    double modulationTriggerMode();
    double modulationTriggerSource();
    double modulationSecondaryFrequencyFSK();
    double modulationMainWaveTrigerPulseAmount();
    double modulationainMainWaveRateAM();
    double modulationainMainWaveFrequencyOffsetFM();
    double modulationainMainWavePhaseOffsetPM();

    //MEASURMENT
    double measurmentGateTime();
    double measurmentExternalFrequency();
    double measurmentExternalCountingValue();
    double measurmentExternalCountingPeriod();
    double measurmentExternalPositivePulseWidth();
    double measurmentExternalNegativePulseWidth();
    double measurmentExternalDutyCycle();

    //SYSTEM (NOT TESTED)
    double systemSynchronizationInformation();
    double systemBuzzerStatus();
    double systemUplinkMode();
    double systemLocalUplinkStatus();
    QString systemLocalID();
    QString systemLocalModel();

};

class ProtocolWrite
{
public:

    QSerialPort *port;

    ProtocolWrite(QSerialPort *serialPort);
    ~ProtocolWrite();

    //CHANNEL
    void channelWaveform(Channel ch, double value);
    void channelFrequency(Channel ch, double value);
    void channelAmplitude(Channel ch, double value);
    void channelOffset(Channel ch, double value);
    void channelDuty(Channel ch, double value);
    void channelPhase(Channel ch, double value);
    void channelOnOff(Channel ch, double value);

    //MODULATION
    void modulationTriggerMode(double value);
    void modulationTriggerSource(double value);
    void modulationSecondaryFrequencyFSK(double value);
    void modulationMainWaveTrigerPulseAmount(double value);
    void modulationainManualTriggerSource();
    void modulationainMainWaveRateAM(double value);
    void modulationainMainWaveFrequencyOffsetFM(double value);
    void modulationainMainWavePhaseOffsetPM(double value);

    //MESUREMENT
    void measurmentCouplingMode(double value);
    void measurmentResetCounter(double value);
    void measurmentPause(double value);
    void measurmentGateTime(double value);

    //SWEEP
    void sweepObject(double value);
    void sweepStart(double value);
    void sweepEnd(double value);
    void sweepTime(double value);
    void sweepMode(double value);
    void sweepDirection(double value);
    void sweepStartStop(double value);
    void sweepSource(double value);

    //SYSTEM (NOT TESTED)
    void systemSaveChannelsParameters(double value);
    void systemLoadChannelsParameters(double value);
    void systemAddSynchronizationModde(double value);
    void systemCancelSynchronizationMode(double value);
    void systemSetBuzzerOnOff(double value);
    void systemSetUplinkMode(double value);
    void systemSetLocalUplinkStatus(double value);

    //ARBITARY WAVE FORM
    void awgUpload(uint channel, uint slot, double *linePoints, uint linePointsSize);

};

#endif // PROTOCOL_H
