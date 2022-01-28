#ifndef READWRITE_H
#define READWRITE_H

#include <QObject>
#include <QThread>
#include <QSerialPort>
#include <QMessageBox>
#include "protocol.h"

class ReadWrite : public QObject
{
    Q_OBJECT
public:

    ReadWrite();


private:
    QSerialPort *port;

public slots:
    void handleChannel(Mode mode, Channel channel, t_channel data);
    void handleModulation(Mode mode, t_modulation data);
    void handleSweep(Mode mode, t_sweep sweep);
    void handleMeasurment(Mode mode, t_measurment data);
    void handleUploadWaveForm(t_waveform data);
    void handlePortOpen(QString portName);
    void handlePortClose();

signals:
    void responseChannel(Mode mode, Channel channel, t_channel data);
    void responseModulation(Mode mode, t_modulation data);
    void responseSweep(Mode mode, t_sweep sweep);
    void responseMeasurment(Mode mode, t_measurment data);
    void responsePortOpen(QString portName, QString modelName, QString modelUID, bool searchEnd);
    void responseMessage(QMessageBox::Icon icon, QString title, QString text,  QMessageBox::StandardButtons buttons);

};

#endif // READWRITE_H
