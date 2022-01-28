#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QThread>
#include <QMessageBox>
#include <QSerialPort>
#include <QAction>
#include "readwrite.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }

QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:    

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void handleChannel(Mode mode, Channel channel, t_channel data);
    void handleModulation(Mode mode, t_modulation data);
    void handleSweep(Mode mode, t_sweep data);
    void handleMeasurment(Mode mode, t_measurment data);
    void handlePortOpen(QString portName, QString modelName, QString modelUID, bool searchEnd);
    void handleMessage(QMessageBox::Icon icon, QString title, QString text,  QMessageBox::StandardButtons buttons);

    void onTimerWidgetFocus();

    void onToggled_ReadParameters(bool checked);
    void onClick_CH1_SetParameters();
    void onClick_CH2_SetParameters();
    void onClick_Modulation_SetParameters();
    void onToggled_Measurment_SetParameters(bool checked);
    void onToggled_Sweep_SetParameters(bool checked);
    void onIndexChanged_SweepObject(int index);

private:
    Ui::MainWindow *ui = nullptr;
    QSerialPort serialPort;
    QTimer *timerWidgetFocus = nullptr;
    QTimer *timerParametersRead = nullptr;
    QTimer *timerMeasurment = nullptr;
    QWidget *focusWidget = nullptr;

    QThread workerThread;
    ReadWrite *worker;


signals:
    void requestChannel(Mode mode, Channel channel, t_channel data);
    void requestModulation(Mode mode, t_modulation data);
    void requestSweep(Mode mode, t_sweep data);
    void requestMeasurment(Mode mode, t_measurment data);
    void requestUploadWaveform(t_waveform data);
    void requestPortOpen(QString portName);
    void requestPortClose();
};
#endif // MAINWINDOW_H
