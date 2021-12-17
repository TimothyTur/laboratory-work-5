#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "codeeditor.h"

#include <QMainWindow>
#include <QWidget>
#include <QMenuBar>
#include <QTextEdit>
#include <QLineEdit>
#include <QLabel>

class Widget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

public slots:
    void updateStatus();

private:
    QMenu*   _MenuFile;
    QAction* _ActionNew; // button ☺
    QAction* _ActionOpen; // dialog ☺
public: QAction* _ActionSave; // dialog ☺☺☺
private:
    QAction* _ActionSaveAs; // dialog ☺
    QAction* _ActionQuit; // button ☺

    QMenu*   _MenuEdit;
    QAction* _ActionUndo; // button ☺
    QAction* _ActionRedo; // button ☺
    QAction* _ActionCopy; // button ☺
    QAction* _ActionCut; // button ☺
    QAction* _ActionPaste; // button ☺
    QAction* _ActionFind; // nonmodal ☺
    QAction* _ActionFindReplace; // nonmodal ☺
    QAction* _ActionSelectAll; // button ☺

    QMenu*   _MenuFormat;
    QAction* _ActionWordWrap; // check ☺
    QAction* _ActionChangeFont; // modal ☺

    QMenu*   _MenuView;
    QAction* _ActionChangeBackground; // modal ☺
    QAction* _ActionChangeLineColor; // modal ☺
    QAction* _ActionChangeNumeration; // check ☺
    QAction* _ActionChangeToolBar; // check
    QAction* _ActionChangeState; // check
    QAction* _ActionChangeHighlight; // check
    QMenu*   _MenuChangeSyntax; // submenu
    QAction*   _C11; // button
    QAction*   _Cpp14; // button
    QMenu*   _MenuChangeStyle; // submenu
    QAction*   _ActionChangeStyle; // modal
    QAction*   _ActionLoadStyle; // modal
    //QAction**  _ActionsStyles; // buttons
      //list of styles available, 1 available always - default

    QMenu* _MenuReference;
    QAction* _ActionAbout;

    Widget* _Widget;
    QLabel* _Cursor;
    QLabel* _Last;
    QLabel* _Amounts;
};

// потому что нет правильной реализации tooltip лол
class MenuBar: public QMenuBar {
public:
    explicit MenuBar(QWidget *parent = nullptr);

protected:
    virtual bool event(QEvent *e) override;
};

class Widget : public QWidget {
    Q_OBJECT

public:
    Widget(MainWindow *parent = nullptr, int dh = 0);

public slots:
    void newFile();
    void openFile();
    void saveFile();
    void saveFileAs();
    void undo();
    void redo();
    void copy();
    void cut();
    void paste();
    void find();
    void findNext();
    void findPrev();
    void findAndReplace();
    void replace();
    void selectAll();
    void switchWordWrap(bool);
    void showChangeFont();
    void showChangeBackground();
    void showChangeLineColor();
    void switchNumeration(bool);
    void switchToolBar(bool);
    void switchState(bool);
    void switchHighlight(bool);
    void c11Syntax();
    void cpp14Syntax();
    void showChangeStyle();
    void showLoadStyle();
    //void switchStyle(); ֎

private:
    MenuBar* _ToolBar;
    QAction*  _ToolNew;
    QAction*  _ToolOpen;
    QAction*  _ToolSave;
    QAction*  _ToolUndo;
    QAction*  _ToolRedo;
    QAction*  _ToolCopy;
    QAction*  _ToolCut;
    QAction*  _ToolPaste;
    QMenu*    _ToolMenuFind; // submenu
    QAction*    _ToolFind; // nonmodal
    QAction*    _ToolReplace; // nonmodal

    CodeEditor* _TextField;
    QTextDocument *_Doc;
    QLineEdit* _FindW;
    QLineEdit* _ReplaceW;
    QString _File;
    bool _Changed;

};

#endif // MAINWINDOW_H
