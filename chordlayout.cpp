#include "chordlayout.h"

ChordLayout::ChordLayout(QWidget* parent) :
    QHBoxLayout(parent),
    SectionInfo(EditSectionEnum::Chord)
{

    this->setSizeConstraint(QLayout::SetDefaultConstraint);

    this->setAlignment(Qt::AlignTop | Qt::AlignLeft);

    parentWidget = parent;


}

void ChordLayout::AddChord(ChordInformation chordInfo)
{
    ChordLabel* newChord = new ChordLabel(chordInfo);

    connect(newChord, &ChordLabel::ShoudlDeleted, [=]{
        DeleteChord(newChord);
    });

    chordList.push_back(newChord);
    this->addWidget(newChord);

}

void ChordLayout::DeleteChord(ChordLabel* chord){
    this->removeWidget(chord);
    disconnect(chord);
    chordList.remove(chord);
    delete chord;
}

QWidget *ChordLayout::getWidget()
{
    return parentWidget;
}

bool ChordLayout::isEmpty()
{
    return chordList.empty();
}



ChordLabel::ChordLabel()
{

}

ChordLabel::ChordLabel(const ChordInformation &chordInfo) : ChordInformation(chordInfo)
{
    std::string newStr = key + suffix;
    this->setText(newStr.c_str());
}

void ChordLabel::mousePressEvent(QMouseEvent *ev)
{
    if(ev->button() == Qt::MouseButton::MiddleButton){
        emit ShoudlDeleted();
    }
}
