#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "protocol.h"
#include "readwrite.h"

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //MENU BAR
    connect(ui->actionExit,&QAction::triggered,this,&QApplication::quit);
    connect(ui->actionAuto_Connect,&QAction::triggered,this,[=]() { emit requestPortOpen(""); } );

    connect(ui->actionDisconect,&QAction::triggered,this,[=](){
        timerParametersRead->stop();
        ui->actionAuto_Connect->setEnabled(true);
        ui->actionDisconect->setEnabled(false);
        ui->statusbar->showMessage(tr("Disconnected !"));
        if( ui->CONTROL_READ_PARAMETERS->isChecked() ) ui->CONTROL_READ_PARAMETERS->click();
        emit requestPortClose();
    });
    connect(ui->actionAbout,&QAction::triggered,this,[=](){
        QMessageBox message(QMessageBox::Information,tr("About"),tr("Open Source Software\nDigital Signal Generator Controller\nFY6900"),QMessageBox::Ok,this);
        message.exec();
    });

    //TAB CONTROL SIGNALS
    connect(ui->CONTROL_READ_PARAMETERS,&QPushButton::toggled,this,&MainWindow::onToggled_ReadParameters);
    connect(ui->CH1_SET_PARAMETERS,&QPushButton::clicked,this,&MainWindow::onClick_CH1_SetParameters);
    connect(ui->CH2_SET_PARAMETERS,&QPushButton::clicked,this,&MainWindow::onClick_CH2_SetParameters);
    connect(ui->SWEEP_SET_PARAMETERS,&QPushButton::clicked,this,&MainWindow::onToggled_Sweep_SetParameters);
    connect(ui->MODULATION_SET_PARAMETERS,&QPushButton::clicked,this,&MainWindow::onClick_Modulation_SetParameters);
    connect(ui->MEASURMENT_SET_PARAMETERS,&QPushButton::toggled,this,&MainWindow::onToggled_Measurment_SetParameters);
    connect(ui->SWEEP_OBJECT,&QComboBox::currentIndexChanged,this,&MainWindow::onIndexChanged_SweepObject);

    //TAB WAVE EDITOR SIGNALS
    connect(ui->WAVE_EDITOR_SHAPE,&QComboBox::currentIndexChanged,ui->WAVE_EDITOR,[=](uint value){ ui->WAVE_EDITOR->shape = value;});
    connect(ui->WAVE_EDITOR_LEFT_CONTIGUOUS,&QSpinBox::valueChanged,ui->WAVE_EDITOR,[=](uint value){ ui->WAVE_EDITOR->leftContiguous = value;});
    connect(ui->WAVE_EDITOR_RIGHT_CONTIGUOUS,&QSpinBox::valueChanged,ui->WAVE_EDITOR,[=](uint value){ ui->WAVE_EDITOR->rightContiguous = value;});
    connect(ui->WAVE_EDITOR_ABSOLUTE_CONTIGUOS,&QCheckBox::toggled,ui->WAVE_EDITOR,[=](uint value){ ui->WAVE_EDITOR->absoluteContiguous = value;});
    connect(ui->WAVE_EDITOR_CLEAR,&QCheckBox::clicked,ui->WAVE_EDITOR,[=](){ ui->WAVE_EDITOR->startEdit = false; ui->WAVE_EDITOR->update();});
    connect(ui->WVAE_EDITOR_CHANNEL,&QComboBox::currentIndexChanged,ui->WAVE_EDITOR,[=](uint value){ ui->WAVE_EDITOR->channelSelected = value;});
    connect(ui->WAVE_EDITOR_SLOT,&QSpinBox::valueChanged,ui->WAVE_EDITOR,[=](uint value){ ui->WAVE_EDITOR->slotSelected = value;});
    connect(ui->WAVE_EDITOR_SAVE,&QPushButton::clicked,ui->WAVE_EDITOR,&WaveFormEditor::onButtonSave);
    connect(ui->WVAE_EDITOR_CHANNEL,&QComboBox::currentIndexChanged,ui->WAVE_EDITOR,&WaveFormEditor::onChannelOrSlotChange);
    connect(ui->WAVE_EDITOR_SLOT,&QSpinBox::valueChanged,ui->WAVE_EDITOR,&WaveFormEditor::onChannelOrSlotChange);
    connect(ui->WAVE_EDITOR_UPLOAD,&QPushButton::clicked,ui->WAVE_EDITOR,[=](){

        t_waveform data;

        data.slot = ui->WAVE_EDITOR_SLOT->value();
        uint index = 64;
        if(  ui->WVAE_EDITOR_CHANNEL->currentIndex() == 0 ) {
            data.channel = Channel::CH1;
            index = ui->CH1_Waveform->count() - 64 - 1;
            ui->CH1_Waveform->setCurrentIndex(index + data.slot);
        }
        if( ui->WVAE_EDITOR_CHANNEL->currentIndex() == 1 ) {
            data.channel = Channel::CH2;
            index = ui->CH2_Waveform->count() - 64 - 1;
            ui->CH2_Waveform->setCurrentIndex(index + data.slot);
        }
        data.waveform_index = index + data.slot;
        ui->WAVE_EDITOR->getWaveform(data);

        qDebug() << "CHANNEL " << data.channel;
        qDebug() << "SLOT " << data.slot;
        qDebug() << "SIZE " << data.linePointsSize;
        qDebug() << "POINTS" << data.linePoints;

        emit requestUploadWaveform(data);
    });

    ui->WAVE_EDITOR_SHAPE->setCurrentIndex(0);
    ui->WAVE_EDITOR_LEFT_CONTIGUOUS->setValue(25);
    ui->WAVE_EDITOR_RIGHT_CONTIGUOUS->setValue(25);
    ui->WAVE_EDITOR_ABSOLUTE_CONTIGUOS->setCheckState(Qt::Checked);
    ui->WVAE_EDITOR_CHANNEL->setCurrentIndex(0);
    ui->WAVE_EDITOR_SLOT->setValue(1);

    onIndexChanged_SweepObject(0);

    for( uint n = 0; n < 64; n++ ) {
        ui->CH1_Waveform->addItem(QString("AWG-%1").arg(n+1));
        ui->CH2_Waveform->addItem(QString("AWG-%1").arg(n+1));
    }

    ui->actionDisconect->setEnabled(false);
    ui->statusbar->showMessage(tr("Disconnected !"));

    /*
    timerWidgetFocus = new QTimer(this);
    connect(timerWidgetFocus,&QTimer::timeout,this,&MainWindow::onTimerWidgetFocus);
    timerWidgetFocus->setInterval(500);
    */

    timerParametersRead = new QTimer(this);
    connect(timerParametersRead,&QTimer::timeout,this,[=]{
        bool checked = ui->CONTROL_READ_PARAMETERS->isChecked();
        onToggled_ReadParameters(checked);
    });
    timerParametersRead->setInterval(1500);


    timerMeasurment = new QTimer(this);
    connect(timerMeasurment,&QTimer::timeout,this,[=](){
        qDebug() << "TIMEOUT";
        t_measurment data;
        memset(&data,0,sizeof(t_measurment));
        data.type = ui->MEASURMENT_TYPE->currentIndex();
        emit requestMeasurment(Mode::READ,data);
    });
    timerMeasurment->setInterval(250);

    worker = new ReadWrite();
    worker->moveToThread(&workerThread);
    connect(&workerThread, &QThread::finished, worker, &QObject::deleteLater);

    connect(this, &MainWindow::requestChannel, worker, &ReadWrite::handleChannel);
    connect(worker, &ReadWrite::responseChannel, this, &MainWindow::handleChannel);

    connect(this, &MainWindow::requestModulation, worker, &ReadWrite::handleModulation);
    connect(worker, &ReadWrite::responseModulation, this, &MainWindow::handleModulation);

    connect(this, &MainWindow::requestSweep, worker, &ReadWrite::handleSweep);
    connect(worker, &ReadWrite::responseSweep, this, &MainWindow::handleSweep);

    connect(this, &MainWindow::requestMeasurment, worker, &ReadWrite::handleMeasurment);
    connect(worker, &ReadWrite::responseMeasurment, this, &MainWindow::handleMeasurment);

    connect(this, &MainWindow::requestPortOpen, worker, &ReadWrite::handlePortOpen);
    connect(worker, &ReadWrite::responsePortOpen, this, &MainWindow::handlePortOpen, Qt::BlockingQueuedConnection);

    connect(this,&MainWindow::requestPortClose, worker, &ReadWrite::handlePortClose);

    connect(this,&MainWindow::requestUploadWaveform, worker, &ReadWrite::handleUploadWaveForm);

    connect(worker, &ReadWrite::responseMessage, this, &MainWindow::handleMessage, Qt::BlockingQueuedConnection);

    workerThread.start();
}

void MainWindow::handleChannel( Mode mode, Channel channel, t_channel data)
{
    if( mode == Mode::READ && channel == Channel::CH1 ) {
        ui->CH1_Waveform->setCurrentIndex(data.waveform);
        ui->CH1_Frequency->setValue(data.frequency);
        ui->CH1_Amplitude->setValue(data.amplitude);
        ui->CH1_Offset->setValue(data.offset);
        ui->CH1_Dutty->setValue(data.duty);
        ui->CH1_Phase->setValue(data.phase);
        ui->CH1_OnOff->setCheckState((Qt::CheckState)data.onoff);
        return;
    }

    if( mode == Mode::WRITE && channel == Channel::CH1 ) {
        data.waveform = ui->CH1_Waveform->currentIndex();
        data.frequency = ui->CH1_Frequency->value();
        data.amplitude = ui->CH1_Amplitude->value();
        data.offset = ui->CH1_Offset->value();
        data.duty = ui->CH1_Dutty->value();
        data.phase = ui->CH1_Phase->value();
        data.onoff = ui->CH1_OnOff->isChecked();
        emit requestChannel(mode,channel,data);
        return;
    }

    if( mode == Mode::READ && channel == Channel::CH2 ) {
        ui->CH2_Waveform->setCurrentIndex(data.waveform);
        ui->CH2_Frequency->setValue(data.frequency);
        ui->CH2_Amplitude->setValue(data.amplitude);
        ui->CH2_Offset->setValue(data.offset);
        ui->CH2_Dutty->setValue(data.duty);
        ui->CH2_Phase->setValue(data.phase);
        ui->CH2_OnOff->setCheckState((Qt::CheckState)data.onoff);
        return;
    }

    if( mode == Mode::WRITE && channel == Channel::CH2 ) {
        data.waveform = ui->CH2_Waveform->currentIndex();
        data.frequency = ui->CH2_Frequency->value();
        data.amplitude = ui->CH2_Amplitude->value();
        data.offset = ui->CH2_Offset->value();
        data.duty = ui->CH2_Dutty->value();
        data.phase = ui->CH2_Phase->value();
        data.onoff = ui->CH2_OnOff->isChecked();
        emit requestChannel(mode,channel,data);
        return;
    }
}

void MainWindow::handleModulation(Mode mode, t_modulation data)
{
    if( mode == Mode::READ ) {
        QCheckBox* modulationASK[] = {
            ui->MODULATION_ASK_CH2,
            ui->MODULATION_ASK_EXT_AC,
            ui->MODULATION_ASK_MANU,
            ui->MODULATION_ASK_EXT_DC
        };

        QCheckBox* modulationFSK[] = {
            ui->MODULATION_FSK_CH2,
            ui->MODULATION_FSK_EXT_AC,
            ui->MODULATION_FSK_MANU,
            ui->MODULATION_FSK_EXT_DC
        };

        QCheckBox* modulationPSK[] = {
            ui->MODULATION_PSK_CH2,
            ui->MODULATION_PSK_EXT_AC,
            ui->MODULATION_PSK_MANU,
            ui->MODULATION_PSK_EXT_DC
        };

        QCheckBox* modulationBURS[] = {
            ui->MODULATION_BURS_CH2,
            ui->MODULATION_BURS_EXT_AC,
            ui->MODULATION_BURS_MANU,
            ui->MODULATION_BURS_EXT_DC
        };

        QCheckBox* modulationAM[] = {
            ui->MODULATION_AM_CH2,
            ui->MODULATION_AM_VCO_IN
        };

        QCheckBox* modulationFM[] = {
            ui->MODULATION_FM_CH2,
            ui->MODULATION_FM_VCO_IN
        };

        QCheckBox* modulationPM[] = {
            ui->MODULATION_PM_CH2,
            ui->MODULATION_PM_VCO_IN
        };

        ui->MODULATION_TAB->setCurrentIndex(data.trigger_mode);
        switch( (int)data.trigger_mode ) {
            case 0:
                modulationFSK[(int)data.trigger_source]->setChecked(true);
                break;
            case 1:
                modulationASK[(int)data.trigger_source]->setChecked(true);
                break;
            case 2:
                modulationPSK[(int)data.trigger_source]->setChecked(true);
                break;
            case 3:
                modulationBURS[(int)data.trigger_source]->setChecked(true);
                break;
            case 4:
                modulationAM[(int)data.trigger_source]->setChecked(true);
                break;
            case 5:
                modulationFM[(int)data.trigger_source]->setChecked(true);
                break;
            case 6:
                modulationPM[(int)data.trigger_source]->setChecked(true);
                break;
        }


        ui->MODULATION_FSK_FREQUENCY->setValue(data.frequency_fsk);
        ui->MODULATION_BURS_NO->setValue(data.pulse_amount);
        ui->MODULATION_AM_RATE->setValue(data.rate_am);
        ui->MODULATION_FM_FREQUENCY_OFFSET->setValue(data.frequency_offset_fm);
        ui->MODULATION_PM_PHASE_OFFSET->setValue(data.phase_offset_pm);
        return;
    }

    if( mode == Mode::WRITE ) {
        ProtocolWrite write(&serialPort);

        data.trigger_mode = ui->MODULATION_TAB->currentIndex();

        int checkBoxIndex = 0;
        switch( ui->MODULATION_TAB->currentIndex() ) {
            case 0:
                foreach( QCheckBox *box, ui->MODULATION_FSK_SOURCE->findChildren<QCheckBox*>() ){
                    if(box->isChecked() ) break;
                    checkBoxIndex++;
                }
                break;
            case 1:
                foreach( QCheckBox *box, ui->MODULATION_ASK_SOURCE->findChildren<QCheckBox*>() ){
                    if(box->isChecked() ) break;
                    checkBoxIndex++;
                }
                break;
            break;
            case 2:
                foreach( QCheckBox *box, ui->MODULATION_PSK_SOURCE->findChildren<QCheckBox*>() ){
                    if(box->isChecked() ) break;
                    checkBoxIndex++;
                }
                break;
            case 3:
                foreach( QCheckBox *box, ui->MODULATION_BURS_SOURCE->findChildren<QCheckBox*>() ){
                    if(box->isChecked() ) break;
                    checkBoxIndex++;
                }

                break;
            case 4:
                foreach( QCheckBox *box, ui->MODULATION_AM_SOURCE->findChildren<QCheckBox*>() ){
                    if(box->isChecked() ) break;
                    checkBoxIndex++;
                }
                break;
            case 5:
                foreach( QCheckBox *box, ui->MODULATION_FM_SOURCE->findChildren<QCheckBox*>() ){
                    if(box->isChecked() ) break;
                    checkBoxIndex++;
                }
                break;
            case 6:
                foreach( QCheckBox *box, ui->MODULATION_PM_SOURCE->findChildren<QCheckBox*>() ){
                    if(box->isChecked() ) break;
                    checkBoxIndex++;
                }
                break;
        }

        data.trigger_source = checkBoxIndex;
        data.frequency_fsk = ui->MODULATION_FSK_FREQUENCY->value();
        data.pulse_amount = ui->MODULATION_BURS_NO->value();
        data.rate_am = ui->MODULATION_AM_RATE->value();
        data.frequency_offset_fm = ui->MODULATION_FM_FREQUENCY_OFFSET->value();
        data.phase_offset_pm = ui->MODULATION_PM_PHASE_OFFSET->value();
        //write.modulationainManualTriggerSource();
        emit requestModulation(mode,data);
        return;
    }
}

void MainWindow::handleSweep(Mode mode, t_sweep data) {
    if( mode == Mode::WRITE ) {
        data.source = ui->SWEEP_SOURCE->currentIndex();
        data.object = ui->SWEEP_OBJECT->currentIndex();
        data.start = ui->SWEEP_START->value();
        data.end = ui->SWEEP_END->value();
        data.time = ui->SWEEP_TIME->value();
        data.mode = ui->SWEEP_MODE->currentIndex();
        data.direction = ui->SWEEP_DIRECTION->currentIndex();
        data.start_stop = ui->SWEEP_SET_PARAMETERS->isChecked();
        emit requestSweep(mode,data);
        return;
    }
}

void MainWindow::handleMeasurment(Mode mode, t_measurment data) {

    if( mode == Mode::READ ) {
        switch( (int)data.type ) {
            case 0:
                ui->MEASURMENT_FREQUENCY->setSuffix("Hz");
                ui->MEASURMENT_FREQUENCY->setValue(data.frequency);
                break;
            case 1:
                ui->MEASURMENT_FREQUENCY->setSuffix("");
                ui->MEASURMENT_FREQUENCY->setValue(data.counting_value);
                break;
        }
        ui->MEASURMENT_POSITIVE_PULSE->setValue(data.positive_pulse_width);
        ui->MEASURMENT_NEGATIVE_PULSE->setValue(data.negative_pulse_width);
        ui->MEASURMENT_DUTY->setValue(data.duty_cycle);
        ui->MEASURMENT_COUNTING_PERIOD->setValue(data.counting_period);
        ui->MEASURMENT_TIME->setCurrentIndex(data.gate_time);
        return;
    }

    if( mode == Mode::WRITE ) {
        data.type = ui->MEASURMENT_TYPE->currentIndex();
        data.coupling_mode = ui->MEASURMENT_COUPLING->currentIndex();
        data.reset_counter = !ui->MEASURMENT_RESET->isChecked();
        data.pause = ui->MEASURMENT_SET_PARAMETERS->isChecked();
        data.gate_time = ui->MEASURMENT_TIME->currentIndex();
        ui->MEASURMENT_RESET->setChecked(false);
        emit requestMeasurment(mode,data);
        return;
    }

}

void MainWindow::handlePortOpen(QString portName, QString modelName, QString modelUID, bool searchEnd) {
    if( searchEnd ){
        if( ui->actionAuto_Connect->isEnabled() ) {
            QMessageBox message(QMessageBox::Warning,tr("Device found ..."),tr("No more port devices to search for !"), QMessageBox::Ok,this);
            message.exec();
        }
    } else {
        QMessageBox message(QMessageBox::Question,tr("Device found ..."),tr("Port: ") + portName + "\n" + tr("Model: ") + modelName + "\n" + tr("UID: ") + modelUID + "\n\n" + tr("Want to use this device ?"), QMessageBox::Yes|QMessageBox::No,this);
        if( message.exec() == QMessageBox::Yes ) {
            emit requestPortOpen(portName);
            ui->actionAuto_Connect->setEnabled(false);
            ui->actionDisconect->setEnabled(true);
            ui->statusbar->showMessage(tr("Connected !"));
            ui->CONTROL_READ_PARAMETERS->click();
        }
    }

}

void MainWindow::handleMessage(QMessageBox::Icon icon, QString title, QString text,  QMessageBox::StandardButtons buttons)
{
    QMessageBox message(icon,title,text,buttons,this);
    message.exec();
}

void MainWindow::onToggled_ReadParameters(bool checked)
{
    if( checked ) {
        //CHANNELS
        t_channel data_channel;
        memset(&data_channel,0,sizeof(t_channel));
        emit requestChannel(Mode::READ,Channel::CH1,data_channel);
        memset(&data_channel,0,sizeof(t_channel));
        emit requestChannel(Mode::READ,Channel::CH2,data_channel);

        //MODULATION
        t_modulation data_modulation;
        memset(&data_modulation,0,sizeof(t_modulation));
        emit requestModulation(Mode::READ,data_modulation);

        //SWEEP
        t_sweep data_sweep;
        memset(&data_sweep,0,sizeof(t_sweep));
        emit requestSweep(Mode::READ,data_sweep);

        //MEASURMENT
        //t_measurment data_measurment;
        //memset(&data_measurment,0,sizeof(t_measurment));
        //emit requestMeasurment(Mode::READ,data_measurment);

        if( ! timerParametersRead->isActive() ) {
            timerParametersRead->start();
        }
    } else {
        timerParametersRead->stop();
    }

}

void MainWindow::onClick_CH1_SetParameters()
{
    t_channel data;
    memset(&data,0,sizeof(t_channel));
    handleChannel(Mode::WRITE,Channel::CH1,data);
}

void MainWindow::onClick_CH2_SetParameters()
{
    t_channel data;
    memset(&data,0,sizeof(t_channel));
    handleChannel(Mode::WRITE,Channel::CH2,data);
}

void MainWindow::onClick_Modulation_SetParameters()
{
    t_modulation data;
    memset(&data,0,sizeof(t_modulation));
    handleModulation(Mode::WRITE,data);
}

void MainWindow::onToggled_Sweep_SetParameters(bool checked)
{
    Q_UNUSED(checked);
    t_sweep data;
    memset(&data,0,sizeof(t_sweep));
    handleSweep(Mode::WRITE,data);
}

void MainWindow::onToggled_Measurment_SetParameters(bool checked)
{
    t_measurment data;
    memset(&data,0,sizeof(t_measurment));
    handleMeasurment(Mode::WRITE,data);
   if( checked ) {
       timerMeasurment->start();
    } else {
        timerMeasurment->stop();
   }
}



void MainWindow::onIndexChanged_SweepObject(int index)
{
    ui->SWEEP_TIME->setSuffix("s");
    ui->SWEEP_TIME->setDecimals(2);
    ui->SWEEP_TIME->setMaximum(100);
    ui->SWEEP_TIME->setMinimum(0);
    ui->SWEEP_TIME->setValue(50);

    switch(index) {
    case 0:
        ui->SWEEP_START->setSuffix("Hz");
        ui->SWEEP_START->setDecimals(1);
        ui->SWEEP_START->setMaximum(100000000);
        ui->SWEEP_START->setMinimum(0);
        ui->SWEEP_START->setValue(100000);
        ui->SWEEP_END->setSuffix("Hz");
        ui->SWEEP_END->setDecimals(1);
        ui->SWEEP_END->setMaximum(100000000);
        ui->SWEEP_END->setMinimum(0);
        ui->SWEEP_END->setValue(100);
        break;
    case 1:
        ui->SWEEP_START->setSuffix("V");
        ui->SWEEP_START->setDecimals(3);
        ui->SWEEP_START->setMaximum(25);
        ui->SWEEP_START->setMinimum(0);
        ui->SWEEP_START->setValue(12.5);
        ui->SWEEP_END->setSuffix("V");
        ui->SWEEP_END->setDecimals(3);
        ui->SWEEP_END->setMaximum(100);
        ui->SWEEP_END->setMinimum(0);
        ui->SWEEP_END->setValue(0);
        break;
    case 2:
        ui->SWEEP_START->setSuffix("V");
        ui->SWEEP_START->setDecimals(3);
        ui->SWEEP_START->setMaximum(25);
        ui->SWEEP_START->setMinimum(-12.5);
        ui->SWEEP_START->setValue(5);
        ui->SWEEP_END->setSuffix("V");
        ui->SWEEP_END->setDecimals(3);
        ui->SWEEP_END->setMaximum(25);
        ui->SWEEP_END->setMinimum(-12.5);
        ui->SWEEP_END->setValue(0);
        break;
    case 3:
        ui->SWEEP_START->setSuffix("%");
        ui->SWEEP_START->setDecimals(1);
        ui->SWEEP_START->setMaximum(100);
        ui->SWEEP_START->setMinimum(0);
        ui->SWEEP_START->setValue(50);
        ui->SWEEP_END->setSuffix("%");
        ui->SWEEP_END->setDecimals(1);
        ui->SWEEP_END->setMaximum(100);
        ui->SWEEP_END->setMinimum(0);
        ui->SWEEP_END->setValue(10);
        break;
    }
}

void MainWindow::onTimerWidgetFocus()
{
    QList<QWidget*> childs = findChildren<QWidget*>(QRegularExpression(".*"));
    for(uint n = 0; n < childs.count(); n++ ) {
        if( ! childs[n]->hasFocus() ) continue;
        if( childs[n] == focusWidget ) return;
        if( dynamic_cast<QSpinBox*>(childs[n]) || dynamic_cast<QDoubleSpinBox*>(childs[n])
            || dynamic_cast<QCheckBox*>(childs[n])|| dynamic_cast<QComboBox*>(childs[n])
            || dynamic_cast<QTabWidget*>(childs[n])) {
            focusWidget = childs[n];
            qDebug() << "GOT FOCUS " << focusWidget->objectName();
            return;
        }
    }
    focusWidget = nullptr;
}

MainWindow::~MainWindow()
{
    serialPort.close();
    delete ui;
}



