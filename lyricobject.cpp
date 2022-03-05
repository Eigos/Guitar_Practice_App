#include "lyricobject.h"

LyricObject::LyricObject(QWidget* parent) : QHBoxLayout(parent), SectionInfo(EditSectionEnum::Lyric){

   this->setSizeConstraint(QLayout::SetDefaultConstraint);

   this->setAlignment(Qt::AlignTop | Qt::AlignLeft);

    parentWidget = parent;

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
    textEditSizePolicy.setVerticalPolicy(QSizePolicy::Fixed);

    editableTextWidget->setSizePolicy(textEditSizePolicy);
    editableTextWidget->setFixedHeight(30);

    this->addWidget(editableTextWidget);

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
        connect(newLabel, &WordLabel::DoubleClick, [&]{
            deInitLabels();
            InitTextEdit(text);
        });
        labelList.push_back(newLabel);
    }

    for(WordLabel* label : labelList){
        this->addWidget(label);
    }


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
