#include "lyricobject.h"


void LyricObject::ChordDeleted(ChordLabel* targetChord)
{
    targetChord->deleteLater();


    if (chordWidget->children().size() <= 1){
        HideChordWidget();
    }
}

void LyricObject::LastPickedWordFunc(WordLabel *targetWord)
{
    if(targetWord == lastPickedWord){
        lastPickedWord = nullptr;
        return;
    }
    else if(lastPickedWord != nullptr){

        QPalette palet;
        palet.setColor(QPalette::WindowText, QColor(Qt::black));
        lastPickedWord->setPalette(palet);
        lastPickedWord->alreadyPicked = false;
    }

    lastPickedWord = targetWord;
}

bool LyricObject::isChordWidgetEmpty()
{
    return chordWidget->children().size() == 0;
}

void  LyricObject::HideChordWidget()
{
    chordWidget->setVisible(false);
}

void LyricObject::ShowChordWidget()
{
    chordWidget->setVisible(true);
}

LyricObject::LyricObject(QWidget* parent, bool isEditable) : QVBoxLayout(parent), SectionInfo(EditSectionEnum::Lyric){


   parentWidget = parent;

   lyricLayoutWidget = new QWidget(parent);
   lyricLayout = new QHBoxLayout(lyricLayoutWidget);

   this->setContentsMargins(0,0,0,0);
   //this->setGeometry(QRect(300,250,173,80));


   lyricLayout->setSizeConstraint(QLayout::SetDefaultConstraint);
   lyricLayout->setAlignment(Qt::AlignTop | Qt::AlignLeft);
   lyricLayoutWidget->setSizePolicy(QSizePolicy{QSizePolicy::Maximum,QSizePolicy::Maximum});

   chordWidget = new QWidget();
   chordWidget->setSizePolicy(QSizePolicy{QSizePolicy::Fixed,QSizePolicy::Fixed});
   chordWidget->setFixedSize({1200,17});
   chordWidget->setVisible(false);

   this->addWidget(chordWidget);
   this->addWidget(lyricLayoutWidget);

   canPickWord = true;
   this->isEditable = isEditable;

   if(isEditable){
       InitTextEdit();
   }
}

LyricObject::~LyricObject()
{
    for(ChordLabel* chord : getAllAttachedChords()){
        chord->deleteLater();
    }

    if(chordWidget != nullptr){
        chordWidget->deleteLater();
    }


    for(WordLabel* word : labelList){
        word->deleteLater();
    }

    lyricLayout->deleteLater();
    lyricLayoutWidget->deleteLater();

    if(labelList.size() == 0){
        editableTextWidget->deleteLater();
    }

}

QWidget *LyricObject::getWidget()
{
    return parentWidget;
}

void LyricObject::AddChordToPickedWord(ChordInformation chordInformation)
{
    if(lastPickedWord == nullptr){
        return;
    }

    ChordLabel* newChord = new ChordLabel(chordInformation);
    uint32_t pickedChordPos = 0;
    const int32_t fontWSize = 10;
    for(WordLabel* word : labelList){
        pickedChordPos++;
        if(word == lastPickedWord){
            break;
        }
    }

    if(isChordWidgetEmpty()){
        chordWidget->setVisible(true);
    }

    newChord->setSizePolicy(QSizePolicy{QSizePolicy::Minimum,QSizePolicy::Minimum});
    newChord->setParent(chordWidget);
    newChord->move(pickedChordPos * fontWSize - (fontWSize/2), 0);
    newChord->setVisible(true);
    connect(newChord, &ChordLabel::ShoudlDeleted, [=]{newChord->deleteLater(); if(chordWidget->children().size() == 1) {chordWidget->setVisible(false);} });

}

void LyricObject::AddChord(ChordLabel *chord)
{
    chord->setParent(chordWidget);
    ShowChordWidget();
}

std::vector<ChordLabel *> LyricObject::getAllAttachedChords()
{
    QObjectList childrenList = chordWidget->children();
    std::vector<ChordLabel *> childrenVector;

    for(uint32_t i = 0; i < childrenList.count(); i++){
        childrenVector.push_back(static_cast<ChordLabel *>(childrenList.at(i)));
    }

    return childrenVector;
}

bool LyricObject::isEmpty()
{
    return shouldDelete;
}

std::string LyricObject::getText()
{
    return this->text;
}

void LyricObject::setText(std::string newText)
{
    if(editableTextWidget != nullptr){
        editableTextWidget->setText(newText.c_str());
    }
    else{
        deInitLabels();
        this->text = newText;
        InitLabels();
    }

}

void LyricObject::LostFocus()
{

    text = editableTextWidget->toPlainText().toStdString();

    deInitTextEdit();

    InitLabels();
}


void LyricObject::InitTextEdit()
{

    editableTextWidget = new TextEdit;

    QSizePolicy textEditSizePolicy;
    textEditSizePolicy.setVerticalPolicy(QSizePolicy::Preferred);

    editableTextWidget->setSizePolicy(textEditSizePolicy);
    editableTextWidget->setFixedHeight(30);

    this->insertWidget(1, editableTextWidget);
    connect(editableTextWidget, &TextEdit::LostFocus, std::bind(&LyricObject::LostFocus, this));
    connect(editableTextWidget, &TextEdit::LostFocus, editableTextWidget, &QObject::deleteLater);
    connect(editableTextWidget, &TextEdit::ShouldDelete, editableTextWidget, &QObject::deleteLater);
    connect(editableTextWidget, &TextEdit::ShouldDelete, editableTextWidget, [=]{shouldDelete = true;});


}

void LyricObject::InitTextEdit(std::string str){
    InitTextEdit();
    editableTextWidget->setText(str.c_str());

}

void LyricObject::InitLabels()
{

    for(const char word : text){
        WordLabel* newLabel = new WordLabel();
        newLabel->setText({word});
        newLabel->setMargin(0);
        if(word == '\t'){
            QPalette palet;
            newLabel->setAutoFillBackground(true);
            palet.setColor(QPalette::Window, QColor(Qt::black));
            newLabel->setPalette(palet);
        }
        connect(newLabel, &WordLabel::DoubleClick, [&]{ // right click
            InitTextEdit(text);
            deInitLabels();
        });
        connect(newLabel, &WordLabel::Picked, this, &LyricObject::LastPickedWordFunc);
        labelList.push_back(newLabel);
    }

    for(int i = 0; i < labelList.size(); i++){
        WordLabel* label= labelList[i];
        lyricLayout->addWidget(label);
    }

}

void LyricObject::deInitTextEdit()
{

    disconnect(editableTextWidget);

    editableTextWidget->deleteLater();

    this->removeWidget(editableTextWidget);


}

void LyricObject::deInitLabels()
{
    for(WordLabel* label : labelList){
        disconnect(label);
        this->removeWidget(label);
        label->deleteLater();
    }
    labelList.clear();
}

void TextEdit::focusInEvent(QFocusEvent *event)
{
    isInFocus = true;
}

void TextEdit::focusOutEvent(QFocusEvent *event)
{
    isInFocus = false;
    if(!this->toPlainText().isEmpty()){
        emit LostFocus();
    }
}

void TextEdit::mousePressEvent(QMouseEvent *ev)
{
    if(ev->button() == Qt::MouseButton::MiddleButton){
        emit ShouldDelete();
    }
}


void WordLabel::mousePressEvent(QMouseEvent *ev)
{
    if(ev->button() == Qt::MouseButton::RightButton){

        emit DoubleClick();
    }

    if(ev->button() == Qt::MouseButton::LeftButton){

        if(alreadyPicked == false){
            QPalette palet;
            palet.setColor(QPalette::WindowText, QColor(Qt::red));
            this->setPalette(palet);
            alreadyPicked = !alreadyPicked;
        }
        else{
            QPalette palet;
            palet.setColor(QPalette::WindowText, QColor(Qt::black));
            this->setPalette(palet);
            alreadyPicked = !alreadyPicked;
        }

        emit Picked(this);
    }
}












