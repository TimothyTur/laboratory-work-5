#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QMenuBar>

class Widget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);


private:
    QMenu*   _MenuFile;
    QAction* _ActionNew; // button
    QAction* _ActionOpen; // dialog
    QAction* _ActionSave; // dialog
    QAction* _ActionSaveAs; // dialog
    QAction* _ActionQuit; // button

    QMenu*   _MenuEdit;
    QAction* _ActionUndo; // button
    QAction* _ActionRedo; // button
    QAction* _ActionCopy; // button
    QAction* _ActionCut; // button
    QAction* _ActionPaste; // button
    QAction* _ActionFind; // nonmodal
    QAction* _ActionFindReplace; // nonmodal
    QAction* _ActionSelectAll; // button

    QMenu*   _MenuFormat;
    QAction* _ActionWordWrap; // check
    QAction* _ActionChangeFont; // modal

    QMenu*   _MenuView;
    QAction* _ActionChangeBackground; // modal
    QAction* _ActionChangeLineColor; // modal
    QAction* _ActionChangeNumeration; // check
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
    Widget(QWidget *parent = nullptr, int dh = 0);

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
    void findAndReplace();
    void selectAll();
    void switchWordWrap();
    void showChangeFont();
    void showChangeBackground();
    void showChangeLineColor();
    void switchNumeration();
    void switchToolBar();
    void switchState();
    void switchHighlight();
    void с11Syntax();
    void сpp14Syntax();
    void showChangeStyle();
    void showLoadStyle();
    //void switchStyle();

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




    QString _Filename;
    bool _FileChanged;
    QString _State;

};

#endif // MAINWINDOW_H
