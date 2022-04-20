#ifndef CHORDLAYOUT_H
#define CHORDLAYOUT_H

#include <qobject.h>
#include <qlabel.h>
#include <QHBoxLayout>
#include <QTextEdit>
#include <functional>
#include <QMouseEvent>

#include <list>

#include "chordmanager.h"
#include "section.h"

class ChordLabel;
class ChordLayout;

class ChordLayout : public QHBoxLayout, public SectionInfo
{
    Q_OBJECT
public:
    ChordLayout(QWidget* parent = nullptr);
    ~ChordLayout();
    void AddChord(ChordInformation chordInfo);
    void DeleteChord(ChordLabel* chord);
    QWidget *getWidget();
    const std::list<ChordLabel*> getAllChords();

    bool isEmpty();

private:
    //std::vector<ChordLabel*> chordList;
    std::list<ChordLabel*> chordList;
    QWidget* parentWidget;

};


class ChordLabel : public QLabel, public ChordInformation{
Q_OBJECT

public:
    ChordLabel();
    ChordLabel(const ChordInformation& chordInfo);

    void mousePressEvent(QMouseEvent *ev) override;

signals:
    void ShoudlDeleted(ChordLabel*);

};

#endif // CHORDLAYOUT_H
