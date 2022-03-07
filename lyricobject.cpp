#include "lyricobject.h"

void ShowSize(QWidget* wi){

    qDebug() << "x y:" << wi->pos() << "w" << wi->width() << "h" << wi->height();
}

void LyricObject::AddChorddd(){
    ChordLabel* newLabel = new ChordLabel({"C", "minor", 0});
    newLabel->setSizePolicy(QSizePolicy{QSizePolicy::Minimum,QSizePolicy::Minimum});
    newLabel->setParent(chordWidget);
    newLabel->move(40,0);
    qDebug() << "Widget Child count after adding another widget: " << chordWidget->children().size();
}

LyricObject::LyricObject(QWidget* parent) : QVBoxLayout(parent), SectionInfo(EditSectionEnum::Lyric){


   parentWidget = parent;

   lyricLayoutWidget = new QWidget(parent);
   lyricLayout = new QHBoxLayout(lyricLayoutWidget);

   this->setContentsMargins(0,0,0,0);
   //this->setGeometry(QRect(300,250,173,80));


   lyricLayout->setSizeConstraint(QLayout::SetDefaultConstraint);
   lyricLayout->setAlignment(Qt::AlignTop | Qt::AlignLeft);
   lyricLayoutWidget->setSizePolicy(QSizePolicy{QSizePolicy::Minimum,QSizePolicy::Minimum});

   chordWidget = new QWidget();
   chordWidget->setSizePolicy(QSizePolicy{QSizePolicy::Fixed,QSizePolicy::Fixed});
   chordWidget->setFixedSize({5000,30});

   ShowSize(chordWidget);


   this->addWidget(chordWidget);
   this->addWidget(lyricLayoutWidget);

   ChordLabel* newLabel = new ChordLabel({"C", "minor", 0});
   newLabel->setSizePolicy(QSizePolicy{QSizePolicy::Minimum,QSizePolicy::Minimum});
   newLabel->setParent(chordWidget);
   newLabel->move(20,0);



   InitTextEdit();
}

QWidget *LyricObject::getWidget()
{
    return parentWidget;
}

bool LyricObject::isEmpty()
{
    return shouldDelete;
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

    //this->addWidget(editableTextWidget);
       this->insertWidget(1, editableTextWidget);
    connect(editableTextWidget, &TextEdit::LostFocus, std::bind(&LyricObject::AddChorddd,this));
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


    ChordLabel* newLabel = new ChordLabel({"C", "minor", 0});
    newLabel->setSizePolicy(QSizePolicy{QSizePolicy::Minimum,QSizePolicy::Minimum});
    newLabel->setParent(chordWidget);
    newLabel->move(40,0);

    for(const char word : text){
        WordLabel* newLabel = new WordLabel();
        newLabel->setText({word});
        connect(newLabel, &WordLabel::DoubleClick, [&]{ // right click
            InitTextEdit(text);
            deInitLabels();
        });
        labelList.push_back(newLabel);
    }

    for(int i = 0; i < labelList.size(); i++){
        WordLabel* label= labelList[i];
        if(label->text() == "A"){
            lyricLayout->addWidget(label);//new ChordLabel({"C", "minor", 0})
            //newLabel->repaint();
            //chordWidget->repaint();
            //this->removeWidget(chordWidget);
            //this->insertWidget(0,chordWidget);
        }else{
            lyricLayout->addWidget(label);
        }

    }

    //for(WordLabel* label : labelList){
    //    if(label->text() == "A"){
    //        lyricLayout->addWidget(label);//new ChordLabel({"C", "minor", 0})
    //        ChordLabel* newLabel = new ChordLabel({"C", "minor", 0});
    //        newLabel->setSizePolicy(QSizePolicy{QSizePolicy::Minimum,QSizePolicy::Minimum});
    //        newLabel->setParent(chordWidget);
    //        newLabel->move(40,0);
    //        this->addWidget(chordWidget);


    //        ShowSize(newLabel);
    //        //this->addWidget(newLabel);
    //    }else{
    //        lyricLayout->addWidget(label);
    //    }
    //}

//TEXT: merhabAlar

}

void LyricObject::deInitTextEdit()
{

    disconnect(editableTextWidget);

    this->removeWidget(editableTextWidget);

    delete editableTextWidget;

}

void LyricObject::deInitLabels()
{
    for(WordLabel* label : labelList){
        disconnect(label);
        this->removeWidget(label);
        delete label;
    }
    labelList.clear();
}


QWidget* LyricObject::AttachChord(ChordLabel* newLabel, WordLabel* word)
{
    QWidget* obj = new QWidget;
    obj->resize(20,30);


    QVBoxLayout* newLayout = new QVBoxLayout(obj);
    newLayout->setSpacing(0);
    newLayout->setSizeConstraint(QLayout::SetFixedSize);
    newLayout->addWidget(newLabel);
    newLayout->addWidget(word);

    return obj;
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
}

