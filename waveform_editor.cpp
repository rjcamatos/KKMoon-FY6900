#include "waveform_editor.h"

WaveFormEditor::WaveFormEditor(QWidget *parent) : QWidget(parent)
{
    setFocus(Qt::FocusReason::ActiveWindowFocusReason);

    linePoints = (QPointF*)malloc(sizeof(QPointF) * linePointsSize);
    markPoints = (QRectF*)malloc(sizeof(QRectF) * markPointsSize);
}

void WaveFormEditor::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);

    painter.setWindow(0,0,windowSizeX,windowSizeY);
    painter.setViewport(0,0,width(),height());
    painter.translate(0,0);

    if( ! startEdit ) {
        resetLinesAndMarks();
    }

    painter.setPen(QPen(QBrush(Qt::gray),0.4));
    painter.drawLine(QPointF(0,windowSizeY/2),QPointF(windowSizeX,windowSizeY/2));

    painter.setPen(QPen(QBrush(Qt::black),0.4));
    painter.drawPolyline(linePoints,linePointsSize);

    painter.setPen(QPen(QBrush(Qt::blue),0.4));
    painter.drawRects(markPoints,markPointsSize);

   if( hoverPoint > -1 ) {
        int lc = linePointsSize / 100 * leftContiguous;
        int rc = linePointsSize / 100 * rightContiguous;
        while( (hoverPoint-lc) < 0 ) lc--;
        while( (hoverPoint+rc) > ((int)linePointsSize-1) ) rc--;
        painter.setPen(QPen(QBrush(Qt::green),0.4));
        painter.drawRects(&markPoints[hoverPoint-lc],lc+rc);
   }

}

void WaveFormEditor::mousePressEvent(QMouseEvent *event)
{
    qreal eventY = tY(event->position().y());
    //qDebug() << "MOUSE PRESS " << eventY;
    for(uint n = 0; n < markPointsSize; n++ ) {
        if( markPoints[n].y() > (eventY - sY(2)) && markPoints[n].y() < (eventY + sY(2)) ) {
                selectedPoint = hoverPoint;
                startEdit = true;
                //qDebug() << "SELECTED POINT " << selectedPoint;
                break;
        }
    }

}

void WaveFormEditor::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    selectedPoint = -1;
}

void WaveFormEditor::showEvent(QShowEvent *event)
{
    Q_UNUSED(event);
    onChannelOrSlotChange();
}

void WaveFormEditor::mouseMoveEvent(QMouseEvent *event)
{

    qreal eventX = tX(event->position().x());

    for(uint n = 0; n < markPointsSize; n++ ) {
        if( markPoints[n].x() > (eventX - tX(4)) && markPoints[n].x() < (eventX + tX(4)) ) {
                //qDebug() << "HOVER POINT " << n;
                hoverPoint = n;
                update();
        }
    }

    int lc = linePointsSize / 100 * leftContiguous;
    int rc = linePointsSize / 100 * rightContiguous;

    while( (selectedPoint-lc) < 0 ) lc--;

    while( (selectedPoint+rc) > ((int)linePointsSize-1) ) rc--;

    if( selectedPoint > -1 ) {
        switch( shape ) {
            case 0:
                drawFree(event,lc,rc);
                break;
            case 1:
                drawRectangle(event,lc,rc);
                break;
            case 2:
                drawTriangle(event,lc,rc);
                break;
            case 3:
                drawArch(event,lc,rc);
                break;
            case 4:
                drawInflexedArch(event,lc,rc);
                break;
            case 5:
                drawOgee(event,lc,rc);
                break;
        }
    }

}

void WaveFormEditor::drawFree(QMouseEvent *event, int contiguousLeft, int contiguousRight)
{
    Q_UNUSED(contiguousLeft);
    Q_UNUSED(contiguousRight);

    qreal posY = tY(event->position().y());
    qreal posX = tX(event->position().x());

    for( uint n = 0; n < linePointsSize; n++ ) {
        if( linePoints[n].x() > (posX - sX(2)) && linePoints[n].x() < (posX + sX(2))  ) {
            linePoints[n].setY(posY);
            markPoints[n].setRect(linePoints[n].x(),posY-tY(0.5),tX(0.1),tY(0.04));
        }
    }

    update();

}


void WaveFormEditor::drawRectangle(QMouseEvent *event, int contiguousLeft, int contiguousRight)
{

    qreal posY = tY(event->position().y());

    for( int n = 0; n < contiguousLeft; n++ ) {
        linePoints[selectedPoint-n].setY(posY);
        markPoints[(selectedPoint-n)].setRect(linePoints[selectedPoint-n].x(),posY-tY(0.5),tX(0.1),tY(0.04));
    }

    for( int n = 0; n < contiguousRight; n++ ) {
        linePoints[selectedPoint+n].setY(posY);
        markPoints[(selectedPoint+n)].setRect(linePoints[selectedPoint+n].x(),posY-tY(0.5),tX(0.1),tY(0.04));
    }

    update();

}

void WaveFormEditor::drawTriangle(QMouseEvent *event, int contiguousLeft, int contiguousRight)
{

    qreal lHeight = tY(event->position().y());
    qreal lIncrement = (lHeight - linePoints[selectedPoint-contiguousLeft].y()) / contiguousLeft ;
    for( int n = 0; n < contiguousLeft; n++ ) {
        linePoints[selectedPoint-n].setY(lHeight);
        markPoints[selectedPoint-n].setRect(linePoints[selectedPoint-n].x(),lHeight-tY(0.5),tX(0.1),tY(0.04));
        lHeight -= lIncrement;
    }

    qreal rHeight = tY(event->position().y());
    qreal rIncrement = (rHeight - linePoints[selectedPoint+contiguousRight].y()) / contiguousRight;
    for( int n = 0; n < contiguousRight; n++ ) {
        linePoints[selectedPoint+n].setY(rHeight);
        markPoints[selectedPoint+n].setRect(linePoints[selectedPoint+n].x(),rHeight-tY(0.5),tX(0.1),tY(0.04));
        rHeight -= rIncrement;
    }

    update();

}

void WaveFormEditor::drawArch(QMouseEvent *event, int contiguousLeft, int contiguousRight)
{
    qreal angle = 0;
    qreal height = tY(event->position().y());

    qreal lIncrement = 90.0 / (qreal)contiguousLeft;
    qreal lHeight = height - linePoints[selectedPoint-contiguousLeft].y();
    for( int n = 0; n < contiguousLeft; n++ ) {
        linePoints[selectedPoint-n].setY( angle * lHeight + linePoints[selectedPoint-contiguousLeft].y() );
        markPoints[selectedPoint-n].setRect(linePoints[selectedPoint-n].x(),linePoints[selectedPoint-n].y()-tY(0.02),tX(0.1),tY(0.04));
        angle = qCos(qDegreesToRadians(n*lIncrement));
    }

    qreal rIncrement = 90.0 / (qreal)contiguousRight ;
    qreal rHeight = height - linePoints[selectedPoint+contiguousRight].y();
    for( int n = 0; n < contiguousRight; n++ ) {
        linePoints[selectedPoint+n].setY( angle * rHeight + linePoints[selectedPoint+contiguousRight].y() );
        markPoints[selectedPoint+n].setRect(linePoints[selectedPoint+n].x(),linePoints[selectedPoint+n].y()-tY(0.02),tX(0.1),tY(0.04));
        angle = qCos(qDegreesToRadians(n*rIncrement));
    }

    //FIX MISSING POINT
    linePoints[selectedPoint].setY( tY(event->position().y()) );

    update();
}

void WaveFormEditor::drawInflexedArch(QMouseEvent *event, int contiguousLeft, int contiguousRight)
{

    qreal angle = 0;
    qreal height = tY(event->position().y());

    qreal lHeight = height - linePoints[selectedPoint-contiguousLeft].y();
    qreal rHeight = height - linePoints[selectedPoint+contiguousRight].y();

    qreal lIncrement = (90.0 / M_PI_2) / contiguousLeft;
    int lCount = contiguousLeft-1;
    for( int n = 0; n < contiguousLeft; n++ ) {
        if( (int)(selectedPoint-lCount) < 0 ) {
            lCount--;
            continue;
        }
        linePoints[selectedPoint-lCount].setY( angle * (lHeight/M_PI_2) + linePoints[selectedPoint-contiguousLeft].y() );
        markPoints[selectedPoint-lCount].setRect(linePoints[selectedPoint-lCount].x(),linePoints[selectedPoint-lCount].y()-tY(0.02),tX(0.1),tY(0.04));
        angle = qAsin(qDegreesToRadians(n*lIncrement));
        lCount--;
    }

    qreal rIncrement =(90.0 / M_PI_2) / contiguousRight;
    int rCount = contiguousRight-1;
    for( int n = 0; n < contiguousRight; n++ ) {
        if( (int)(selectedPoint+rCount) > (int)linePointsSize ) {
            rCount--;
            continue;
        }
        linePoints[selectedPoint+rCount].setY( angle * (rHeight/M_PI_2) + linePoints[selectedPoint+contiguousRight].y() );
        markPoints[selectedPoint+rCount].setRect(linePoints[selectedPoint+rCount].x(),linePoints[selectedPoint+rCount].y()-tY(0.02),tX(0.1),tY(0.04));
        angle = qAsin(qDegreesToRadians(n*rIncrement));
        rCount--;
    }

    //FIX MISSING POINT
    linePoints[selectedPoint+contiguousRight-1].setY(linePoints[selectedPoint+contiguousRight].y());

    update();
}

void WaveFormEditor::drawOgee(QMouseEvent *event, int contiguousLeft, int contiguousRight)
{
    qreal angle = 0;
    qreal height = tY(event->position().y());

    qreal lIncrement = 90.0 / (qreal)contiguousLeft / M_PI_2;
    qreal lHeight = height - linePoints[selectedPoint-contiguousLeft].y();
    for( int n = 0; n < contiguousLeft; n++ ) {
        if( (int)(selectedPoint-n) < 0 ) break;
        linePoints[selectedPoint-n].setY( angle * (lHeight/M_PI_2) + linePoints[selectedPoint-contiguousLeft].y() );
        markPoints[selectedPoint-n].setRect(linePoints[selectedPoint-n].x(),linePoints[selectedPoint-n].y()-tY(0.02),tX(0.1),tY(0.04));
        angle = qAcos(qDegreesToRadians(n*lIncrement));
    }

    qreal rIncrement = 90.0 / (qreal)contiguousRight / M_PI_2 ;
    qreal rHeight = height - linePoints[selectedPoint+contiguousRight].y();
    for( int n = 0; n < contiguousRight; n++ ) {
        if( (int)(selectedPoint+n) > (int)(linePointsSize-1) ) break;
        linePoints[selectedPoint+n].setY( angle * (rHeight/M_PI_2) + linePoints[selectedPoint+contiguousRight].y() );
        markPoints[selectedPoint+n].setRect(linePoints[selectedPoint+n].x(),linePoints[selectedPoint+n].y()-tY(0.02),tX(0.1),tY(0.04));
        angle = qAcos(qDegreesToRadians(n*rIncrement));
    }

    //FIX MISSING CENTRAL POINT
    linePoints[selectedPoint].setY( tY(event->position().y()) );
    //markPoints[selectedPoint].setRect(linePoints[selectedPoint].x(),linePoints[selectedPoint].y()-tY(0.02),tX(0.1),tY(0.04));

    update();
}

void WaveFormEditor::resetLinesAndMarks()
{
    qreal posX = 0;
    qreal posY = windowSizeY/2.0;
    qreal increment =  (qreal) windowSizeX / (qreal)linePointsSize;

    posX = 0;
    for(uint n = 0; n < linePointsSize; n++ ) {
        linePoints[n].setX(posX);
        linePoints[n].setY(posY);
        posX += increment;
    }

    posX = 0;
    for(uint n = 0; n < markPointsSize; n++ ) {
        markPoints[n].setRect(posX-tX(0.5),posY,tX(0.1),tY(0.04));
        posX += increment;
    }

    startEdit = false;

}

qreal WaveFormEditor::tX(qreal x)
{
    return x / ((qreal)width() / windowSizeX);
}

qreal WaveFormEditor::tY(qreal y)
{
    return y / ((qreal)height() / (windowSizeY));
}

qreal WaveFormEditor::sX(qreal x)
{
    return x / ((qreal)width() / windowSizeX);
}

qreal WaveFormEditor::sY(qreal y)
{
    return y / ((qreal)height() / (windowSizeY));
}

void WaveFormEditor::onButtonSave()
{
    QFile file("waveform/CH_" + QString::number(channelSelected) + "-SLOT_" + QString::number(slotSelected) + ".csv");

    if( ! file.open(QIODevice::WriteOnly) ) return;

    for(uint n = 0; n < linePointsSize; n++) {
        qreal value = linePoints[n].y();
        value = (value*(1/windowSizeY)-0.5)*-2;
        file.write( QString::number(value,'E').toLocal8Bit() + '\n');
    }

    file.close();
}

void WaveFormEditor::onChannelOrSlotChange()
{

    QFile file("waveform/CH_" + QString::number(channelSelected) + "-SLOT_" + QString::number(slotSelected) + ".csv");

    resetLinesAndMarks();

    if( file.open(QIODevice::ReadOnly) ) {
        for(uint n = 0; n < linePointsSize; n++) {
            qreal value = file.readLine().toDouble();
            value = ((value*windowSizeY)-windowSizeY)/-2;
            linePoints[n].setY(value);
            markPoints[n].setRect(n*((qreal)windowSizeX/(qreal)linePointsSize),value-tY(0.02),tX(0.1),tY(0.04));
        }
        file.close();
        startEdit = true;
    }

    update();
}


void WaveFormEditor::getWaveform(t_waveform &data) {
    data.linePointsSize = linePointsSize;
    data.linePoints = (double*)malloc(sizeof(double)*linePointsSize);
    for( uint n = 0; n  < linePointsSize; n++ ) {
        data.linePoints[n] = linePoints[n].y() * (1/windowSizeY);
    }
}

