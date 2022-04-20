#ifndef LYRICOBJECT_H
#define LYRICOBJECT_H

#include <qobject.h>
#include <qlabel.h>
#include <QHBoxLayout>
#include <QTextEdit>
#include <functional>
#include <QMouseEvent>

#include "section.h"
#include "chordlayout.h"
#include "chordmanager.h"


class TextEdit;
class WordLabel;
class LyricObject;

class LyricObject : public QVBoxLayout, public SectionInfo
{
    Q_OBJECT
public:
    LyricObject(QWidget* parent, bool isEditable = true);
    ~LyricObject();
    QWidget* getWidget();

    void AddChordToPickedWord(ChordInformation chordInformation);
    void AddChord(ChordLabel* chord);

    std::vector<ChordLabel*> getAllAttachedChords();

    bool isEmpty();

    std::string getText();
    void setText(std::string newText);

public slots:
    void LostFocus();
    void LastPickedWordFunc(WordLabel* targetWord);

private slots:
    void ChordDeleted(ChordLabel* targetChord);

signals:
    //void PressedLabel(WordLabel* pressedLabel);

private:

    bool isEditable;

    QWidget* lyricLayoutWidget;
    QHBoxLayout* lyricLayout;

    QWidget* chordWidget;

    bool isChordWidgetEmpty();
    void HideChordWidget();
    void ShowChordWidget();

    TextEdit *editableTextWidget = nullptr;

    std::vector<WordLabel*> labelList;
    std::string text;
    WordLabel* lastPickedWord = nullptr;
    bool canPickWord; // true

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

private slots:
   // void PressedLabelPrivate(WordLabel* pressedLabel);

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
    void focusInEvent(QFocusEvent *event) override;
    void focusOutEvent(QFocusEvent *event) override;
    void mousePressEvent(QMouseEvent *ev) override;


};

class WordLabel : public QLabel{
Q_OBJECT
signals:
    void DoubleClick();
    void Picked(WordLabel* targetWord);

public:
    void mousePressEvent(QMouseEvent *ev) override;

    bool alreadyPicked = false;


};

#endif // LYRICOBJECT_H
