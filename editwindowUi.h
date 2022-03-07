/********************************************************************************
** Form generated from reading UI file 'editwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.2.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef EDITWINDOWUI_H
#define EDITWINDOWUI_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

class Ui_EditWindow
{
public:
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    QVBoxLayout *layout_section_options;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QVBoxLayout *verticalLayout_3;
    QVBoxLayout *layout_buttons;
    QPushButton *button_add_lyrics;
    QPushButton *button_add_chords;
    QPushButton *button_section_comment;
    QPushButton *button_chord_lyric_attach;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1280, 720);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        sizePolicy.setHeightForWidth(centralwidget->sizePolicy().hasHeightForWidth());
        centralwidget->setSizePolicy(sizePolicy);
        gridLayout = new QGridLayout(centralwidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        layout_section_options = new QVBoxLayout(centralwidget);
        layout_section_options->setObjectName(QString::fromUtf8("layout_section_options"));

        gridLayout->addLayout(layout_section_options, 0, 0, 1, 1);

        scrollArea = new QScrollArea(centralwidget);
        scrollArea->setObjectName(QString::fromUtf8("scrollArea"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(scrollArea->sizePolicy().hasHeightForWidth());
        scrollArea->setSizePolicy(sizePolicy1);
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QString::fromUtf8("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 414, 657));
        verticalLayout_3 = new QVBoxLayout(scrollAreaWidgetContents);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalLayout_3->setAlignment(Qt::AlignTop);
        //Spacer
        QSpacerItem* verticalSpacer = new QSpacerItem(0,0,QSizePolicy::Minimum,QSizePolicy::MinimumExpanding);
        verticalLayout_3->addSpacerItem(verticalSpacer);
        scrollArea->setWidget(scrollAreaWidgetContents);

        gridLayout->addWidget(scrollArea, 0, 1, 1, 1);

        layout_buttons = new QVBoxLayout();
        layout_buttons->setObjectName(QString::fromUtf8("layout_buttons"));
        layout_buttons->setSizeConstraint(QLayout::SetDefaultConstraint);
        QSizePolicy sizePolicy2(QSizePolicy::Minimum, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);

        button_add_lyrics = new QPushButton(centralwidget);
        button_add_lyrics->setObjectName(QString::fromUtf8("button_add_lyrics"));
        sizePolicy2.setHeightForWidth(button_add_lyrics->sizePolicy().hasHeightForWidth());
        button_add_lyrics->setSizePolicy(sizePolicy2);

        layout_buttons->addWidget(button_add_lyrics);

        button_add_chords = new QPushButton(centralwidget);
        button_add_chords->setObjectName(QString::fromUtf8("button_add_chords"));
        sizePolicy2.setHeightForWidth(button_add_chords->sizePolicy().hasHeightForWidth());
        button_add_chords->setSizePolicy(sizePolicy2);

        layout_buttons->addWidget(button_add_chords);

        button_section_comment = new QPushButton(centralwidget);
        button_section_comment->setObjectName(QString::fromUtf8("button_section_comment"));
        sizePolicy2.setHeightForWidth(button_section_comment->sizePolicy().hasHeightForWidth());
        button_section_comment->setSizePolicy(sizePolicy2);

        layout_buttons->addWidget(button_section_comment);

        button_chord_lyric_attach = new QPushButton(centralwidget);
        button_chord_lyric_attach->setObjectName(QString::fromUtf8("button_chord_lyric_attach"));
        sizePolicy2.setHeightForWidth(button_chord_lyric_attach->sizePolicy().hasHeightForWidth());
        button_chord_lyric_attach->setSizePolicy(sizePolicy2);

        layout_buttons->addWidget(button_chord_lyric_attach);

        gridLayout->addLayout(layout_buttons, 0, 2, 1, 1);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 1280, 21));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        button_add_lyrics->setText(QCoreApplication::translate("MainWindow", "Add Lyrics", nullptr));
        button_add_chords->setText(QCoreApplication::translate("MainWindow", "Add Chords", nullptr));
        button_section_comment->setText(QCoreApplication::translate("MainWindow", "Add Comment Section", nullptr));
        button_chord_lyric_attach->setText(QCoreApplication::translate("MainWindow", "Lyric / Chord attach", nullptr));
    } // retranslateUi

};

namespace UiEdit {
    class MainWindowEdit: public Ui_EditWindow {};
} // namespace Ui

#endif // EDITWINDOWUI_H
