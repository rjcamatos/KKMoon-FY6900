#ifndef WAVEFORM_EDITOR_H
#define WAVEFORM_EDITOR_H

#include <QDebug>
#include <QWidget>
#include <QMessageBox>
#include <QApplication>
#include <QPainter>
#include <QPaintDevice>
#include <QMouseEvent>
#include <QtMath>
#include <QDir>
#include <QPointF>
#include "types.h"

class WaveFormEditor : public QWidget
{

    Q_OBJECT

public:

    explicit WaveFormEditor(QWidget *parent = nullptr);

    uint shape = 0;
    uint leftContiguous = 0;
    uint rightContiguous = 0;
    uint absoluteContiguous = 0;
    bool startEdit = false;
    uint channelSelected = 0;
    uint slotSelected = 1;

    void getWaveform(t_waveform &data);

private:

    const uint linePointsSize = 8192;
    const uint markPointsSize = 8192;

    const qreal windowSizeX = 320;
    const qreal windowSizeY = 240;

    QPointF *linePoints = nullptr;
    QRectF *markPoints = nullptr;

    int selectedPoint = -1;
    int hoverPoint = -1;

    void paintEvent(QPaintEvent *event);
    void mousePressEvent (QMouseEvent *event);
    void mouseReleaseEvent (QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void showEvent(QShowEvent *event);

    void drawFree(QMouseEvent *event, int contiguousLeft, int contiguousRight);
    void drawRectangle(QMouseEvent *event, int contiguousLeft, int contiguousRight);
    void drawTriangle(QMouseEvent *event, int contiguousLeft, int contiguousRight);
    void drawArch(QMouseEvent *event, int contiguousLeft, int contiguousRight);
    void drawInflexedArch(QMouseEvent *event, int contiguousLeft, int contiguousRight);
    void drawOgee(QMouseEvent *event, int contiguousLeft, int contiguousRight);

    void resetLinesAndMarks();



    //transform
    qreal tX(qreal x);
    qreal tY(qreal y);

    //scale
    qreal sX(qreal x);
    qreal sY(qreal y);


public slots:
    void onButtonSave();
    void onChannelOrSlotChange();

};

#endif // WAVEFORM_EDITOR_H
