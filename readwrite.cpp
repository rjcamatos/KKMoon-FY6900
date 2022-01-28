#include "readwrite.h"

ReadWrite::ReadWrite()
{
    port = new QSerialPort(this);

    connect(port,&QSerialPort::errorOccurred,this,[=](QSerialPort::SerialPortError error){
        if( error == QSerialPort::PermissionError) {
            emit responseMessage(QMessageBox::Warning,tr("Data terminal error ..."),tr("Permissin error: (") + QString::number(error) + tr(") !"),QMessageBox::Ok);
            port->close();
        }
    });
}

void ReadWrite::handleChannel(Mode mode, Channel channel, t_channel data)
{
    if( ! port->isDataTerminalReady() ) return;

    if( mode == Mode::READ ) {
        ProtocolRead read(port);
        data.waveform = read.channelWaveform(channel);
        data.frequency = read.channelFrequency(channel);
        data.amplitude = read.channelAmplitude(channel);
        data.offset = read.channelOffset(channel);
        data.duty = read.channelDuty(channel);
        data.phase = read.channelPhase(channel);
        data.onoff = read.channelOnOff(channel);
        emit responseChannel(mode,channel,data);
    }

    if( mode == Mode::WRITE) {
        ProtocolWrite write(port);
        write.channelWaveform(channel,data.waveform);
        write.channelFrequency(channel,data.frequency);
        write.channelAmplitude(channel,data.amplitude);
        write.channelOffset(channel,data.offset);
        write.channelDuty(channel,data.duty);
        write.channelPhase(channel,data.phase);
        write.channelOnOff(channel,data.onoff);
    }

}

void ReadWrite::handleModulation(Mode mode, t_modulation data)
{
   if( ! port->isDataTerminalReady() ) return;

   if( mode == Mode::READ ) {
        ProtocolRead read(port);
        data.trigger_mode = read.modulationTriggerMode();
        data.trigger_source = read.modulationTriggerSource();
        data.frequency_fsk = read.modulationSecondaryFrequencyFSK();
        data.pulse_amount = read.modulationMainWaveTrigerPulseAmount();
        data.rate_am = read.modulationainMainWaveRateAM();
        data.frequency_offset_fm = read.modulationainMainWaveFrequencyOffsetFM();
        data.phase_offset_pm = read.modulationainMainWavePhaseOffsetPM();
        emit responseModulation(mode,data);
   }

   if( mode == Mode::WRITE ) {
        ProtocolWrite write(port);
        write.modulationTriggerMode(data.trigger_mode);
        write.modulationTriggerSource(data.trigger_source);
        write.modulationSecondaryFrequencyFSK(data.frequency_fsk);
        write.modulationMainWaveTrigerPulseAmount(data.pulse_amount);
        write.modulationainMainWaveRateAM(data.rate_am);
        write.modulationainMainWaveFrequencyOffsetFM(data.frequency_offset_fm);
        write.modulationainMainWavePhaseOffsetPM(data.phase_offset_pm);
   }

}

void ReadWrite::handleSweep(Mode mode, t_sweep data)
{
   if( ! port->isDataTerminalReady() ) return;

    if( mode == Mode::WRITE ) {
        ProtocolWrite write(port);;
        write.sweepSource(data.source);
        write.sweepObject(data.object);
        write.sweepStart(data.start);
        write.sweepEnd(data.end);
        write.sweepTime(data.time);
        write.sweepMode(data.mode);
        write.sweepDirection(data.direction);
        write.sweepStartStop(data.start_stop);
    }

}

void ReadWrite::handleMeasurment(Mode mode, t_measurment data)
{
    if( ! port->isDataTerminalReady() ) return;

    if( mode == Mode::READ ) {
        ProtocolRead read(port);
        if( data.type == 0 ) data.frequency = read.measurmentExternalFrequency();
        if( data.type == 1 ) data.counting_value = read.measurmentExternalCountingValue();
        data.positive_pulse_width = read.measurmentExternalPositivePulseWidth();
        data.negative_pulse_width = read.measurmentExternalNegativePulseWidth();
        data.duty_cycle = read.measurmentExternalDutyCycle();
        data.counting_period = read.measurmentExternalCountingPeriod();
        data.gate_time = read.measurmentGateTime();
        emit responseMeasurment(mode,data);
    }

    if( mode == Mode::WRITE ) {
        ProtocolWrite write(port);
        write.measurmentCouplingMode(data.coupling_mode);
        write.measurmentResetCounter(data.reset_counter);
        write.measurmentPause(data.pause);
        write.measurmentGateTime(data.gate_time);
    }

}

void ReadWrite::handleUploadWaveForm(t_waveform data)
{
   if( ! port->isDataTerminalReady() ) return;

    ProtocolWrite write(port);
    write.channelWaveform(data.channel,data.waveform_index);
    write.channelOnOff(data.channel,0);
    write.awgUpload(data.channel,data.slot,data.linePoints,data.linePointsSize);
    write.channelOnOff(data.channel,1);

    emit responseMessage(QMessageBox::Information,tr("Waveform upload ..."),tr("File upload !"),QMessageBox::Ok);
}

void ReadWrite::handlePortOpen(QString portName)
{
    if( port->isDataTerminalReady() ) return;

    port->setBaudRate(QSerialPort::Baud115200);

    if( portName != "" ) {
        port->setPortName(portName);
        port->open(QSerialPort::OpenModeFlag::ReadWrite);
        return;
    }

    for( uint n = 0; n < 10; n++ ) {
        port->setPortName(QString("COM%1").arg(n));
        if( port->open(QSerialPort::OpenModeFlag::ReadWrite) ) {
            ProtocolRead read(port);
            QString modelName = read.systemLocalModel();
            QString modelUID = read.systemLocalID();
            if( modelName.mid(0,3).toLower() != "fy6" ) continue;
            port->close();
            emit responsePortOpen(port->portName(),modelName,modelUID,false);
        }
    }
    emit responsePortOpen("","","",true);
}

void ReadWrite::handlePortClose()
{
    if( ! port->isDataTerminalReady() ) return;
    port->close();
}


