/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionNewFile;
    QAction *actionOpenFile;
    QAction *actionSaveFile;
    QAction *actionNoHelp;
    QAction *actionSomeSettings;
    QAction *actionundo;
    QAction *actionredo;
    QWidget *centralwidget;
    QWidget *LayerEditorWidget;
    QFrame *frame;
    QPushButton *processorButton;
    QMenuBar *menubar;
    QMenu *menuMenu;
    QMenu *menuSettings;
    QMenu *menuHelp;
    QStatusBar *statusbar;
    QToolBar *toolBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1281, 658);
        actionNewFile = new QAction(MainWindow);
        actionNewFile->setObjectName("actionNewFile");
        actionNewFile->setEnabled(true);
        actionOpenFile = new QAction(MainWindow);
        actionOpenFile->setObjectName("actionOpenFile");
        actionSaveFile = new QAction(MainWindow);
        actionSaveFile->setObjectName("actionSaveFile");
        actionNoHelp = new QAction(MainWindow);
        actionNoHelp->setObjectName("actionNoHelp");
        actionSomeSettings = new QAction(MainWindow);
        actionSomeSettings->setObjectName("actionSomeSettings");
        actionundo = new QAction(MainWindow);
        actionundo->setObjectName("actionundo");
        actionredo = new QAction(MainWindow);
        actionredo->setObjectName("actionredo");
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        LayerEditorWidget = new QWidget(centralwidget);
        LayerEditorWidget->setObjectName("LayerEditorWidget");
        LayerEditorWidget->setGeometry(QRect(290, 0, 991, 581));
        frame = new QFrame(centralwidget);
        frame->setObjectName("frame");
        frame->setGeometry(QRect(0, 0, 291, 521));
        frame->setFrameShape(QFrame::Shape::StyledPanel);
        frame->setFrameShadow(QFrame::Shadow::Raised);
        processorButton = new QPushButton(centralwidget);
        processorButton->setObjectName("processorButton");
        processorButton->setGeometry(QRect(0, 520, 291, 61));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1281, 21));
        menuMenu = new QMenu(menubar);
        menuMenu->setObjectName("menuMenu");
        menuSettings = new QMenu(menubar);
        menuSettings->setObjectName("menuSettings");
        menuHelp = new QMenu(menubar);
        menuHelp->setObjectName("menuHelp");
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);
        toolBar = new QToolBar(MainWindow);
        toolBar->setObjectName("toolBar");
        MainWindow->addToolBar(Qt::ToolBarArea::TopToolBarArea, toolBar);

        menubar->addAction(menuMenu->menuAction());
        menubar->addAction(menuSettings->menuAction());
        menubar->addAction(menuHelp->menuAction());
        menuMenu->addAction(actionNewFile);
        menuMenu->addAction(actionOpenFile);
        menuMenu->addAction(actionSaveFile);
        menuSettings->addAction(actionSomeSettings);
        menuHelp->addAction(actionNoHelp);
        toolBar->addAction(actionundo);
        toolBar->addAction(actionredo);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        actionNewFile->setText(QCoreApplication::translate("MainWindow", "NewFile", nullptr));
        actionOpenFile->setText(QCoreApplication::translate("MainWindow", "OpenFile", nullptr));
        actionSaveFile->setText(QCoreApplication::translate("MainWindow", "SaveFile", nullptr));
        actionNoHelp->setText(QCoreApplication::translate("MainWindow", "NoHelp", nullptr));
        actionSomeSettings->setText(QCoreApplication::translate("MainWindow", "SomeSettings", nullptr));
        actionundo->setText(QCoreApplication::translate("MainWindow", "undo", nullptr));
        actionredo->setText(QCoreApplication::translate("MainWindow", "redo", nullptr));
        processorButton->setText(QCoreApplication::translate("MainWindow", "Processor", nullptr));
        menuMenu->setTitle(QCoreApplication::translate("MainWindow", "Menu", nullptr));
        menuSettings->setTitle(QCoreApplication::translate("MainWindow", "Settings", nullptr));
        menuHelp->setTitle(QCoreApplication::translate("MainWindow", "Help", nullptr));
        toolBar->setWindowTitle(QCoreApplication::translate("MainWindow", "toolBar", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
