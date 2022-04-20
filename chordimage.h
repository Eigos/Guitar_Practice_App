#ifndef CHORDIMAGE_H
#define CHORDIMAGE_H

#include "chordmanager.h"

#include <QImage>
#include <QPainter>


class ChordImage
{
public:
    ChordImage(const ChordInformation& chord) : chordInformation(chord){};

    QImage getImage();

private:
    ChordInformation chordInformation;
};

#endif // CHORDIMAGE_H
