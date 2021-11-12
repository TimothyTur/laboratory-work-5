#include "mainwindow.h"

#include <QMenuBar>
#include <QToolBar>
#include <QEvent>
#include <QHelpEvent>
#include <QToolTip>
#include <QCoreApplication>

#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setMinimumSize(800,600);

    _Widget = new Widget(this,menuBar()->height());
    setCentralWidget(_Widget);

    _MenuFile = menuBar()->addMenu(tr("&Файл"));
    _ActionNew = _MenuFile->addAction(tr("Новый"),
                                      _Widget, SLOT(newFile()),
                                      QKeySequence::New);
    _ActionOpen = _MenuFile->addAction(tr("Открыть"),
                                       _Widget, SLOT(openFile()),
                                       QKeySequence::Open);
    _ActionSave = _MenuFile->addAction(tr("Сохранить"),
                                       _Widget, SLOT(saveFile()),
                                       QKeySequence::Save);
    _ActionSaveAs = _MenuFile->addAction(tr("Сохранить как"),
                                         _Widget, SLOT(saveFileAs()),
                                         QKeySequence::SaveAs);
    _ActionQuit = _MenuFile->addAction(tr("&Выход"),
                                       QCoreApplication::instance(),
                                       SLOT(quit()),
                                       QKeySequence::Quit);

    _MenuEdit = menuBar()->addMenu(tr("&Правка"));
    _ActionUndo = _MenuEdit->addAction(tr("Отменить"),
                                       _Widget, SLOT(undo()),
                                       QKeySequence::Undo);
    _ActionRedo = _MenuEdit->addAction(tr("Повторить"),
                                       _Widget, SLOT(redo()),
                                       QKeySequence::Redo);
    _ActionCopy = _MenuEdit->addAction(tr("Копировать"),
                                       _Widget, SLOT(copy()),
                                       QKeySequence::Copy);
    _ActionCut = _MenuEdit->addAction(tr("Вырезать"),
                                      _Widget, SLOT(cut()),
                                      QKeySequence::Cut);
    _ActionPaste = _MenuEdit->addAction(tr("Вставить"),
                                        _Widget, SLOT(paste()),
                                        QKeySequence::Paste);
    _ActionFind = _MenuEdit->addAction(tr("Найти"),
                                       _Widget, SLOT(find()),
                                       QKeySequence::Find);
    _ActionFindReplace = _MenuEdit->addAction(tr("Найти и заменить"),
                                              _Widget, SLOT(findAndReplace()));
    _ActionSelectAll = _MenuEdit->addAction(tr("Выделить все"),
                                            _Widget, SLOT(selectAll()),
                                            QKeySequence::SelectAll);

    _MenuFormat = menuBar()->addMenu(tr("&Формат"));
    _ActionWordWrap = _MenuFormat->addAction(tr("Перенос по словам"),
                                             _Widget, SLOT(switchWordWrap()));
    _ActionWordWrap->setCheckable(true);
    _ActionWordWrap->setChecked(false);
    _ActionChangeFont = _MenuFormat->addAction(tr("Выбор шрифта"),
                                               _Widget, SLOT(showChangeFont()));

    _MenuView = menuBar()->addMenu(tr("&Вид"));
    _ActionChangeBackground = _MenuView->addAction(tr("Выбор цвета фона"),
                                         _Widget, SLOT(showChangeBackground()));
    _ActionChangeLineColor = _MenuView->addAction(
                tr("Выбор цвета текущей строки"),
                                          _Widget, SLOT(showChangeLineColor()));
    _ActionChangeNumeration = _MenuView->addAction(
                tr("Вкл/Выкл отображения нумерации строк"),
                _Widget, SLOT(switchNumeration()));
    _ActionChangeNumeration->setCheckable(true);
    _ActionChangeNumeration->setChecked(true);
    _ActionChangeToolBar = _MenuView->addAction(
                tr("Вкл/Выкл отображения панели инструментов"),
                _Widget, SLOT(switchToolBar()));
    _ActionChangeNumeration->setCheckable(true);
    _ActionChangeNumeration->setChecked(true);
    _ActionChangeState = _MenuView->addAction(
                tr("Вкл/Выкл отображения строки состояния"),
                _Widget, SLOT(switchState()));
    _ActionChangeNumeration->setCheckable(true);
    _ActionChangeNumeration->setChecked(true);
    _ActionChangeHighlight = _MenuView->addAction(
                tr("Вкл/Выкл подсветки синтаксиса"),
                _Widget, SLOT(switchHighlight()));
    _ActionChangeNumeration->setCheckable(true);
    _ActionChangeNumeration->setChecked(true);
    _MenuChangeSyntax = _MenuView->addMenu(tr("Выбор синтаксиса"));
    _C11 = _MenuView->addAction(tr("Си 2011"), _Widget, SLOT(с11Syntax()));
    _Cpp14 = _MenuView->addAction(tr("Си++ 2014"),
                                  _Widget, SLOT(сpp14Syntax()));
    //list of syntaxes available, 1 available always - default
    _MenuChangeStyle = _MenuView->addMenu(
                tr("Выбор/Редактирование стиля подсветки"));
    _ActionChangeStyle = _MenuChangeStyle->addAction(tr("Изменить"),
                                                     _Widget,
                                                     SLOT(showChangeStyle()));
    _ActionLoadStyle = _MenuChangeStyle->addAction(
                tr("Загрузка стиля из файла"),
                _Widget, SLOT(showLoadStyle()));
    //list of styles available, 1 available always - default

    _MenuReference = menuBar()->addMenu(tr("&Справка"));
    _ActionAbout = _MenuReference->addAction(tr("О программе"));
}

MenuBar::MenuBar(QWidget *parent)
    : QMenuBar(parent)
{

}

bool MenuBar::event(QEvent *e)
{
  // keep behavior of base class
  bool ret = QMenuBar::event(e);
  // check whether this is a help event
  if (e->type() == QEvent::ToolTip) {
    const QHelpEvent *const pQHelpEvent = (const QHelpEvent*)e;
    const QAction *pQAction = activeAction();
    if (pQAction && !pQAction->toolTip().isEmpty()) {
      QToolTip::showText(pQHelpEvent->globalPos(), pQAction->toolTip());
      return ret;
    }
  }
  QToolTip::hideText();
  return ret;
}


Widget::Widget(QWidget *parent, int dh)
    : QWidget(parent)
{
    setMinimumSize(800,600-dh);
    _ToolBar = new MenuBar(this);
    //_ToolBar->setToolTipsVisible(true);
    _ToolBar->setDefaultUp(true);
    _ToolNew = _ToolBar->addAction(tr("Новый"), this, SLOT(newFile()));
    _ToolNew->setIcon(QIcon(":images/new.png"));
    _ToolOpen = _ToolBar->addAction(tr("Открыть"), this, SLOT(openFile()));
    _ToolOpen->setIcon(QIcon(":images/open.png"));
    _ToolSave = _ToolBar->addAction(tr("Сохранить"), this, SLOT(saveFile()));
    _ToolSave->setIcon(QIcon(":images/save.png"));
    _ToolUndo = _ToolBar->addAction(tr("Отменить"), this, SLOT(undo()));
    _ToolUndo->setIcon(QIcon(":images/undo.png"));
    _ToolRedo = _ToolBar->addAction(tr("Повторить"), this, SLOT(redo()));
    _ToolRedo->setIcon(QIcon(":images/redo.png"));
    _ToolCopy = _ToolBar->addAction(tr("Копировать"), this, SLOT(copy()));
    _ToolCopy->setIcon(QIcon(":images/copy.png"));
    _ToolCut = _ToolBar->addAction(tr("Вырезать"), this, SLOT(cut()));
    _ToolCut->setIcon(QIcon(":images/cut.png"));
    _ToolPaste = _ToolBar->addAction(tr("Вставить"), this, SLOT(paste()));
    _ToolPaste->setIcon(QIcon(":images/paste.png"));
    _ToolMenuFind = _ToolBar->addMenu(tr("Найти / Найти и заменить"));
    _ToolMenuFind->setIcon(QIcon(":images/find.jpg"));
    _ToolFind = _ToolMenuFind->addAction(tr("Найти"), this, SLOT(find()));
    _ToolReplace = _ToolMenuFind->addAction(tr("Найти и заменить"),
                                            this, SLOT(findAndReplace()));

}
void Widget::newFile() {                                                  // !!!
    std::cout << "nf\n";
}
void Widget::openFile() {                                                 // !!!
    std::cout << "of\n";
}
void Widget::saveFile() {                                                 // !!!
    std::cout << "sf\n";
}
void Widget::saveFileAs() {                                               // !!!
    std::cout << "sfa\n";
}
void Widget::undo() {                                                     // !!!
    std::cout << "u\n";
}
void Widget::redo() {                                                     // !!!
    std::cout << "r\n";
}
void Widget::copy() {                                                     // !!!
    std::cout << "co\n";
}
void Widget::cut() {                                                      // !!!
    std::cout << "cu\n";
}
void Widget::paste() {                                                    // !!!
    std::cout << "p\n";
}
void Widget::find() {                                                     // !!!
    std::cout << "f\n";
}
void Widget::findAndReplace() {                                           // !!!
    std::cout << "far\n";
}
void Widget::selectAll() {                                                // !!!
    std::cout << "sa\n";
}
void Widget::switchWordWrap() {                                           // !!!
    std::cout << "sww\n";
}
void Widget::showChangeFont() {                                           // !!!
    std::cout << "scf\n";
}
void Widget::showChangeBackground() {                                     // !!!
    std::cout << "scb\n";
}
void Widget::showChangeLineColor() {                                      // !!!
    std::cout << "sclc\n";
}
void Widget::switchNumeration() {                                         // !!!
    std::cout << "sn\n";
}
void Widget::switchToolBar() {                                            // !!!
    std::cout << "stb\n";
}
void Widget::switchState() {                                              // !!!
    std::cout << "sst\n";
}
void Widget::switchHighlight() {                                          // !!!
    std::cout << "sh\n";
}
void Widget::с11Syntax() {                                                // !!!
    std::cout << "c11\n";
}
void Widget::сpp14Syntax() {                                              // !!!
    std::cout << "cpp14\n";
}
void Widget::showChangeStyle() {                                          // !!!
    std::cout << "scs\n";
}
void Widget::showLoadStyle() {                                            // !!!
    std::cout << "sls\n";
}
//void switchStyle() {                                                    // !!!
//    std::cout << "ssy\n";
//}
