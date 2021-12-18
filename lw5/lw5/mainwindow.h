#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "codeeditor.h"
#include "highlighter.h"

#include <QMainWindow>
#include <QWidget>
#include <QMenuBar>
#include <QTextEdit>
#include <QLineEdit>
#include <QLabel>
#include <QTime>
#include <QDate>
#include <QPushButton>
#include <QSettings>

class Widget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

public slots:
    void updateStatus();
    void daySaved();
    void selectC(QString);

private slots:
    void passDay();
    void chooseStyle(QAction*);
    void showChangeStyle();
    void showLoadStyle();
    void setStyleColor();
    void saveStyle();

private:
    void createEditStyleDialog();
    void readStyle(QSettings*);
    void writeStyle(QSettings*);

    QMenu*   _MenuFile;
    QAction* _ActionNew; // button ☺
    QAction* _ActionOpen; // dialog ☺
    QAction* _ActionSave; // dialog ☺
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
    QAction* _ActionChangeToolBar; // check ☺
    QAction* _ActionChangeState; // check ☺
    QAction* _ActionChangeHighlight; // check ☺
    QMenu*   _MenuChangeSyntax; // submenu
    QAction*   _C11; // button ☺
    QAction*   _C18; // button ☺
    QAction*   _Cpp14; // button ☺
    QAction*   _Cpp17; // button ☺
    QAction*   _Cpp20; // button ☺
    QMenu*   _MenuChangeStyle; // submenu
    QAction*   _ActionChangeStyle; // modal ☺
    QAction*   _ActionLoadStyle; // modal ☺
    QAction*   _ActionDefaultStyle; // modal ☺
    QMenu* _MenuReference;
    QAction* _ActionAbout; // ☺

    QPushButton* _KeywordB,
               * _SLCommentB,
               * _MLCommentB,
               * _QuotationB,
               * _SCharB,
               * _FunctionB,
               * _DirectiveB,
               * _AngleB;
    QDialog *_StyleDialog;

    Widget* _Widget;
    QLabel* _Cursor;
    QLabel* _Last;
    QLabel* _Amounts;
    QTimer* _Timer;
    bool _DayPassedC = false,
         _DayPassedS = false;
    QMap <QString, Style> _Styles;
    Style _CurrStyle;
    QAction* _CurrStyleAction;

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
    long getX();
    long getY();
    long lineCount();
    long wordCount();
    long symbCount();
    long kbCount();
    Style getStyle();
    void setStyle(Style);

    QTextDocument *_Doc;
    QTime _LSave,
          _LChange;
    QDate _LSaveDate,
          _LChangeDate;
    QString _File;

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
    void switchHighlight(bool);
    void c11Syntax();
    void c18Syntax();
    void cpp14Syntax();
    void cpp17Syntax();
    void cpp20Syntax();
    void showAbout(); // ֎

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
    Highlighter* _Highlighter;
    QLineEdit* _FindW;
    QLineEdit* _ReplaceW;
    bool _Changed;
    MainWindow* _MainWindow;

};

#endif // MAINWINDOW_H
