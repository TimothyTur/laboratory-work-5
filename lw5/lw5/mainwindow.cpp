#include "mainwindow.h"

#include <QMenuBar>
#include <QStatusBar>
#include <QToolBar>
#include <QEvent>
#include <QHelpEvent>
#include <QToolTip>
#include <QCoreApplication>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QGridLayout>
#include <QPlainTextEdit>
#include <QFontDialog>
#include <QColorDialog>
#include <QMimeDatabase>
#include <QTextDocument>






#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setFixedSize(800,600);

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
    _ActionWordWrap = _MenuFormat->addAction(tr("Перенос по словам"));
    _ActionWordWrap->setCheckable(true);
    _ActionWordWrap->setChecked(true);
    _ActionChangeFont = _MenuFormat->addAction(tr("Выбор шрифта"),
                                               _Widget, SLOT(showChangeFont()));

    _MenuView = menuBar()->addMenu(tr("&Вид"));
    _ActionChangeBackground = _MenuView->addAction(tr("Выбор цвета фона"),
                                         _Widget, SLOT(showChangeBackground()));
    _ActionChangeLineColor = _MenuView->addAction(
                                               tr("Выбор цвета текущей строки"),
                                          _Widget, SLOT(showChangeLineColor()));
    _ActionChangeNumeration = _MenuView->addAction(
                                    tr("Вкл/Выкл отображения нумерации строк"));
    _ActionChangeNumeration->setCheckable(true);
    _ActionChangeNumeration->setChecked(true);
    _ActionChangeToolBar = _MenuView->addAction(
                                tr("Вкл/Выкл отображения панели инструментов"));
    _ActionChangeToolBar->setCheckable(true);
    _ActionChangeToolBar->setChecked(true);
    _ActionChangeState = _MenuView->addAction(
                                   tr("Вкл/Выкл отображения строки состояния"));
    _ActionChangeState->setCheckable(true);
    _ActionChangeState->setChecked(true);
    _ActionChangeHighlight = _MenuView->addAction(
                                           tr("Вкл/Выкл подсветки синтаксиса"));
    _ActionChangeHighlight->setCheckable(true);
    _ActionChangeHighlight->setChecked(true);
    _MenuChangeSyntax = _MenuView->addMenu(tr("Выбор синтаксиса"));
    _C11 = _MenuChangeSyntax->addAction(tr("Си 2011"),
                                                    _Widget, SLOT(c11Syntax()));
    _Cpp14 = _MenuChangeSyntax->addAction(tr("Си++ 2014"),
                                                  _Widget, SLOT(cpp14Syntax()));
    //list of syntaxes available, 1 available always - default
    _MenuChangeStyle = _MenuView->addMenu(
                                    tr("Выбор/Редактирование стиля подсветки"));
    _ActionChangeStyle = _MenuChangeStyle->addAction(tr("Изменить"),
                                              _Widget, SLOT(showChangeStyle()));
    _ActionLoadStyle = _MenuChangeStyle->addAction(
                 tr("Загрузка стиля из файла"), _Widget, SLOT(showLoadStyle()));
    //list of styles available, 1 available always - default

    _MenuReference = menuBar()->addMenu(tr("&Справка"));
    _ActionAbout = _MenuReference->addAction(tr("О программе"));

    connect(_ActionWordWrap, &QAction::toggled,
                                              _Widget, &Widget::switchWordWrap);
    connect(_ActionChangeNumeration, &QAction::toggled,
                                            _Widget, &Widget::switchNumeration);
    connect(_ActionChangeToolBar, &QAction::toggled,
                                               _Widget, &Widget::switchToolBar);
    connect(_ActionChangeState, &QAction::toggled,
                                          statusBar(), &QStatusBar::setVisible);
    connect(_ActionChangeHighlight, &QAction::toggled,
                                             _Widget, &Widget::switchHighlight);
    _ActionWordWrap->setChecked(true);
    _ActionChangeNumeration->setChecked(true);
    _ActionChangeToolBar->setChecked(true);
    _ActionChangeState->setChecked(true);
    _ActionChangeHighlight->setChecked(true);

    _Cursor = new QLabel("x: 0, y: 0");
    _Last = new QLabel("");
    _Amounts = new QLabel("Строки: 0, слова: 0, символы: 0, размер: 0");

    statusBar()->addPermanentWidget(_Cursor, 1);
    statusBar()->addPermanentWidget(_Last, 1);
    statusBar()->addPermanentWidget(_Amounts, 1);

}
void MainWindow::updateStatus() {
    cursorPos->setText("x: " + QString::number(std::to_string(posX)) + ", y: " + QString::fromStdString(std::to_string(posY)));
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


Widget::Widget(MainWindow *parent, int dh)
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

    _TextField = new CodeEditor(this);
    _TextField->setGeometry(0,_ToolBar->height()*0.7,
                       800,600-dh/2-_ToolBar->height()/0.7);
    _TextField->setContextMenuPolicy(Qt::NoContextMenu);
    _Doc = _TextField->document();

    connect(_Doc, &QTextDocument::modificationChanged,
            parent->_ActionSave, &QAction::setEnabled);
    connect(_Doc, &QTextDocument::modificationChanged,
            this, &QWidget::setWindowModified);
    connect(_Doc, &QTextDocument::contentsChanged,
            parent, &MainWindow::updateStatus);
}
void Widget::newFile() {
    _File.clear();
    _TextField->clear();
    setWindowTitle(QString());
}
void Widget::openFile() {
    QString fileName = QFileDialog::getOpenFileName(this, "Выберите файл");
    QFile file(fileName);
    _File = fileName;
    if (!file.open(QIODevice::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, "Ошибка", "Не открыть файл: "
                                                          + file.errorString());
        return;
    }
    setWindowTitle(fileName);
    //_TextField->setText(text);
    QByteArray data = file.readAll();
    QMimeDatabase db;
    const QString &mimeType =
                           db.mimeTypeForFileNameAndData(fileName, data).name();
    if (mimeType == "text/plain")
        _TextField->setPlainText(QString::fromUtf8(data));
    //setname(f);
    file.close();
}
void Widget::saveFile() {
    QString fileName;
    // If we don't have a filename from before, get one.
    if (_File.isEmpty()) {
        fileName = QFileDialog::getSaveFileName(this, "Сохранить");
        _File = fileName;
    } else {
        fileName = _File;
    }
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, "Ошибка", "Не сохранить файл: "
                                                          + file.errorString());
        return;
    }
    setWindowTitle(fileName);
    QTextStream out(&file);
    QString text = _TextField->toPlainText();
    out << text;
    file.close();
}
void Widget::saveFileAs() {
    QString fileName = QFileDialog::getSaveFileName(this, "Save as");
    QFile file(fileName);

    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, "Ошибка", "Не сохранить файл: "
                                                          + file.errorString());
        return;
    }
    _File = fileName;
    setWindowTitle(fileName);
    QTextStream out(&file);
    QString text = _TextField->toPlainText();
    out << text;
    file.close();
}

void Widget::undo() {
    _TextField->undo();
}
void Widget::redo() {
    _TextField->redo();
}

void Widget::copy() {
    _TextField->copy();
}
void Widget::cut() {
    _TextField->cut();
}
void Widget::paste() {
    _TextField->paste();
}

void Widget::find() {
    QDialog *FindDialog = new QDialog(this);
    FindDialog->setModal(0);
    FindDialog->setFixedSize(250, 100);

    QFrame *frame = new QFrame(this);
    frame->setFrameStyle(0);

    QLabel *TextFind = new QLabel(tr("Найти: "), frame);
    _FindW = new QLineEdit(frame);

    QPushButton *NextB = new QPushButton(tr("Следующий"), frame);
    QPushButton *BackB = new QPushButton(tr("Предыдущий"), frame);

    QGridLayout *fl = new QGridLayout(frame);
    fl->addWidget(TextFind, 0, 0);
    fl->addWidget(_FindW, 0, 1);
    frame->setLayout(fl);

    QGridLayout *l = new QGridLayout(this);
    l->addWidget(frame, 0, 0, 1, 0);
    l->addWidget(NextB, 1, 0);
    l->addWidget(BackB, 1, 1);
    FindDialog->setLayout(l);

    connect(NextB, &QPushButton::clicked, this, &Widget::findNext);
    connect(BackB, &QPushButton::clicked, this, &Widget::findPrev);

    FindDialog->show();
}
void Widget::findNext() {
    //QString searchString = _FindW->text();
    _TextField->find(_FindW->text(), QTextDocument::FindWholeWords);
}
void Widget::findPrev() {
    //QString searchString = findword->text();
    _TextField->find(_FindW->text(),
                   QTextDocument::FindWholeWords | QTextDocument::FindBackward);
}
void Widget::findAndReplace() {
    QDialog *FindDialog = new QDialog(this);
    FindDialog->setModal(0);
    FindDialog->setFixedSize(250, 125);

    QFrame *frame = new QFrame(this);
    frame->setFrameStyle(0);
    QLabel *TextFind = new QLabel(tr("Найти: "), frame);
    QLabel *TextReplace = new QLabel(tr("Заменить: "), frame);

    _FindW = new QLineEdit(frame);
    _ReplaceW = new QLineEdit(frame);

    QPushButton *NextB = new QPushButton(tr("Следующее"), frame);
    QPushButton *BackB = new QPushButton(tr("Предыдущее"), frame);
    QPushButton *ReplB = new QPushButton(tr("Заменить"), frame);

    QGridLayout *fl = new QGridLayout(frame);
    fl->addWidget(TextFind, 0, 0);
    fl->addWidget(TextReplace, 1, 0);
    fl->addWidget(_FindW, 0, 1);
    fl->addWidget(_ReplaceW, 1, 1);
    frame->setLayout(fl);

    QGridLayout *l = new QGridLayout(this);
    l->addWidget(frame, 0, 0, 1, 0);
    l->addWidget(NextB, 1, 0);
    l->addWidget(BackB, 1, 1);
    l->addWidget(ReplB, 1, 2);
    FindDialog->setLayout(l);

    connect(NextB, &QPushButton::clicked, this, &Widget::findNext);
    connect(BackB, &QPushButton::clicked, this, &Widget::findPrev);
    connect(ReplB, &QPushButton::clicked, this, &Widget::replace);

    FindDialog->show();
}
void Widget::replace() {
    if(_ReplaceW!=0) {
        _TextField->textCursor().removeSelectedText();
        _TextField->textCursor().insertText(_ReplaceW->text());
    }
}

void Widget::selectAll() {
    _TextField->selectAll();
}
void Widget::switchWordWrap(bool arg) {
    _TextField->setLineWrapMode(
                    arg?(QPlainTextEdit::WidgetWidth):(QPlainTextEdit::NoWrap));
}

void Widget::showChangeFont() {
    QFontDialog *Dialog = new QFontDialog(this);
    Dialog->exec();
    _TextField->setFont(Dialog->selectedFont());
}
void Widget::showChangeBackground() {
    QColor NewColor = QColorDialog::getColor(
                                       _TextField->getBGColor(), this);
    if(NewColor.isValid()) {
        QPalette p;
        p.setColor(QPalette::Active, QPalette::Base, NewColor);
        p.setColor(QPalette::Inactive, QPalette::Base, NewColor);
        _TextField->setPalette(p);
    }
}
void Widget::showChangeLineColor() {
    QColor NewColor = QColorDialog::getColor(
                _TextField->getLColor(), this);
    if(NewColor.isValid())
        _TextField->setLineColor(NewColor);
}
void Widget::switchNumeration(bool arg) {
    _TextField->flag = arg;
    _TextField->updateLineNumberAreaWidth(0);
}
void Widget::switchToolBar(bool arg) {                                    // !!!
    _ToolBar->setVisible(arg);
}
void Widget::switchState(bool arg) {                                      // !!!
    std::cout << "sst\n";
}
void Widget::switchHighlight(bool arg) {                                  // !!!
    std::cout << "sh\n";
}
void Widget::c11Syntax() {                                                // !!!
    std::cout << "c11\n";
}
void Widget::cpp14Syntax() {                                              // !!!
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
