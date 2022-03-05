#ifndef LYRICOBJECT_H
#define LYRICOBJECT_H

#include <qobject.h>
#include <qlabel.h>
#include <QHBoxLayout>
#include <QTextEdit>
#include <functional>
#include <QMouseEvent>

#include "section.h"

class TextEdit;
class WordLabel;
class LyricObject;

class LyricObject : public QHBoxLayout, public SectionInfo
{
    Q_OBJECT
public:
    LyricObject(QWidget* parent);
    QWidget* getWidget();

    bool isEmpty();

public slots:
    void LostFocus();


private:

    TextEdit *editableTextWidget;

    std::vector<WordLabel*> labelList;
    std::string text;

    void ShowLabels();
    void ShowTextEdit();
    void HideLabels();
    void HideTextEdit();

    void InitTextEdit();
    void InitTextEdit(std::string str);
    void InitLabels();
    void deInitTextEdit();
    void deInitLabels();

    QWidget* parentWidget;

    bool shouldDelete = false;


};

class TextEdit : public QTextEdit
{
Q_OBJECT

private:
    bool isInFocus = false;

signals:
    void LostFocus();
    void ShouldDelete();

public:
    void focusInEvent(QFocusEvent *event);
    void focusOutEvent(QFocusEvent *event);
    void mousePressEvent(QMouseEvent *ev) override;
};

class WordLabel : public QLabel{
Q_OBJECT
signals:
    void DoubleClick();

public:
    void mousePressEvent(QMouseEvent *ev) override;


};

#endif // LYRICOBJECT_H
