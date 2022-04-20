#include "chordimage.h"

QImage ChordImage::getImage()
{
    QImage image(100, 120, QImage::Format_RGB32);
    QPainter painter(&image);
    //painter.fillRect(image.rect(), Qt::yellow);

    //space between edges
    const int32_t horizontalSpace = 10;
    const int32_t verticalSpace = 20;

    //Line count
    const int32_t horizontalLineCount = 5;
    const int32_t verticalLineCount = 4;

    //space between two lines
    const int32_t horizontalMargin = (image.width() - (horizontalSpace * 2)) / horizontalLineCount;
    const int32_t verticalMargin = (image.height() - (verticalSpace * 2)) / verticalLineCount;


    //Draw horizontal lines
    for(uint32_t i = 0; i < verticalLineCount + 1; i++){
        painter.drawLine(horizontalSpace, verticalSpace + (verticalMargin * i), horizontalSpace + (horizontalMargin * horizontalLineCount), verticalSpace + (verticalMargin * i));
    }

    //Draw vertical lines
    for(uint32_t i = 0; i < horizontalLineCount + 1; i++){
        painter.drawLine(horizontalSpace + (horizontalMargin * i), verticalSpace, horizontalSpace + (horizontalMargin * i), verticalSpace + (verticalMargin * verticalLineCount));
    }

    //QFont newFont = painter.font();
    //newFont.setPointSizeF(static_cast<float>(newFont.pointSize()) / 2);
    //painter.setFont(newFont);

    //Write base fret
    QString baseFretStr = "Base Fret: ";
    baseFretStr.append(static_cast<char>(chordInformation.posInfo.baseFret + '0'));
    painter.drawText(0,0, image.width(), 20, Qt::AlignHCenter, baseFretStr);

    //Draw frets
    for(uint32_t i = 0; i < 6; i++){

        char fret = chordInformation.posInfo.frets[i];

        if( fret == -1){
            fret = 'X';
        }
        else if(fret == 0){
            fret = 'O';
        }
        else{
            continue;
        }

        painter.drawText(static_cast<int32_t>(horizontalSpace - 3 + (horizontalMargin * i)), verticalSpace, {fret});
    }

    QPen newPen;
    newPen.setColor(Qt::magenta);
    newPen.setBrush(Qt::magenta);

    QPen defPen = painter.pen();
    painter.setPen(newPen);
    painter.setBrush(Qt::magenta);



    //Draw fingers
    for(uint32_t i = 0; i < 6; i++){

        char fingers = chordInformation.posInfo.fingers[i];
        char fret = chordInformation.posInfo.frets[i];

        if(fret > 0){
            int32_t x = horizontalSpace + (horizontalMargin * i) - (horizontalMargin / 2);
            int32_t y = verticalSpace + (verticalMargin * (fret - 1));
            painter.drawEllipse(x, y, horizontalMargin, verticalMargin);

            painter.setPen(defPen);
            painter.drawText(x, y, horizontalMargin, verticalMargin, Qt::AlignCenter ,std::to_string(fingers).c_str());
            painter.setPen(newPen);
        }



        //painter.drawText(static_cast<int32_t>(horizontalSpace - 3 + (horizontalMargin * i)), verticalSpace, {static_cast<char>(fingers + '0')});
    }

    painter.setPen(defPen);

    return image;
}














