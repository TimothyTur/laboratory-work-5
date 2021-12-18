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
#include <QTimer>
#include <QFormLayout>







#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    _Timer = new QTimer();
    _Timer->setSingleShot(true);
    _Timer->start((((24-QTime::currentTime().hour())*60
                    -QTime::currentTime().minute())*60
                    -QTime::currentTime().second())*1000);
    connect(_Timer, SIGNAL(timeout()), this, SLOT(passDay()));

    setFixedSize(800,600);
    statusBar();
    _Widget = new Widget(this,menuBar()->height());
    setCentralWidget(_Widget);

    createEditStyleDialog();

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
    _C11->setCheckable(true);
    _C11->setChecked(true);
    _C18 = _MenuChangeSyntax->addAction(tr("Си 2018"),
                                                    _Widget, SLOT(c18Syntax()));
    _C18->setCheckable(true);
    _C18->setChecked(false);
    _Cpp14 = _MenuChangeSyntax->addAction(tr("Си++ 2014"),
                                                  _Widget, SLOT(cpp14Syntax()));
    _Cpp14->setCheckable(true);
    _Cpp14->setChecked(false);
    _Cpp17 = _MenuChangeSyntax->addAction(tr("Си++ 2017"),
                                                  _Widget, SLOT(cpp17Syntax()));
    _Cpp17->setCheckable(true);
    _Cpp17->setChecked(false);
    _Cpp20 = _MenuChangeSyntax->addAction(tr("Си++ 2020"),
                                                  _Widget, SLOT(cpp20Syntax()));
    _Cpp20->setCheckable(true);
    _Cpp20->setChecked(false);
    _MenuChangeStyle = _MenuView->addMenu(
                                    tr("Выбор/Редактирование стиля подсветки"));
    _ActionChangeStyle = _MenuChangeStyle->addAction(tr("Изменить"),
                                              this, SLOT(showChangeStyle()));
    _ActionLoadStyle = _MenuChangeStyle->addAction(
                 tr("Загрузка стиля из файла"), this, SLOT(showLoadStyle()));
    _ActionDefaultStyle = _MenuChangeStyle->addAction(tr("Default"));
    _ActionDefaultStyle->setCheckable(true);
    _ActionDefaultStyle->setChecked(true);
    connect(_MenuChangeStyle, SIGNAL(triggered(QAction*)),
                        this, SLOT(chooseStyle(QAction*)));
    _CurrStyleAction = _ActionDefaultStyle;
    _Styles["Default"] = _Widget->getStyle();

    _MenuReference = menuBar()->addMenu(tr("&Справка"));
    _ActionAbout = _MenuReference->addAction(tr("О программе"),
                                                    _Widget, SLOT(showAbout()));

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
    _Last = new QLabel("Последние сохранение: -; Последние изменение: -");
    _Amounts = new QLabel("Строки: 0, слова: 0, символы: 0, размер: 0");

    statusBar()->addPermanentWidget(_Cursor, 1);
    statusBar()->addPermanentWidget(_Last, 5);
    statusBar()->addPermanentWidget(_Amounts, 1);

    QDir dir = QDir::currentPath();
    QStringList filter;
    filter << "*.ini";
    for(auto filename: dir.entryList(filter)) {
        QSettings *ss = new QSettings(filename,QSettings::IniFormat,this);
        readStyle(ss);

        QString styleName = QFileInfo(filename).baseName();

        QAction *act = _MenuChangeStyle->addAction(styleName);
        act->setCheckable(true);
        _Styles[act->text()] = _CurrStyle;
    }

    _CurrStyle = _Styles["Default"];
}
void MainWindow::updateStatus() {
    _Cursor->setText("x: "+QString::number(_Widget->getX())+
                     ", y: " + QString::number(_Widget->getY()));
    if(_Widget->_Doc->isModified()){
        _Widget->_LChange = QTime::currentTime();
        _Widget->_LChangeDate = QDate::currentDate();
        _DayPassedC = false;
    }
    QString lst("Последние сохранение: ");
    if(!(_Widget->_LSave.isNull())) {
        if(_DayPassedS)
            lst += _Widget->_LSaveDate.toString();
        else
            lst += _Widget->_LSave.toString();
    }
    else
        lst += "-";
    lst += "; Последние изменение: ";
    if(!(_Widget->_LChange.isNull())) {
        if(_DayPassedC)
            lst += _Widget->_LChangeDate.toString();
        else
            lst += _Widget->_LChange.toString();
    }
    else
        lst += "-";
    _Last ->setText(lst);

    _Amounts->setText("Строки: " + QString::number(_Widget->lineCount()) +
                      "; Слова: " + QString::number(_Widget->wordCount()) +
                      "; Символы: " + QString::number(_Widget->symbCount()) +
               "; Размер в килобайтах: " + QString::number(_Widget->kbCount()));

    QString title;
    if(_Widget->_LChange>_Widget->_LSave) {
        title = "*";
    }
    title += _Widget->_File;
    if(title.length()>32) {
        title.chop(title.length()-32);
        title+="...";
    }
    setWindowTitle(title);
}
void MainWindow::daySaved() {
    _DayPassedS = false;
    updateStatus();
}
void MainWindow::selectC(QString c) {
    _C11->setChecked(false);
    _C18->setChecked(false);
    _Cpp14->setChecked(false);
    _Cpp17->setChecked(false);
    _Cpp20->setChecked(false);
    if(c=="C11")
        _C11->setChecked(true);
    else if(c=="C18")
        _C18->setChecked(true);
    else if(c=="C++14")
        _Cpp14->setChecked(true);
    else if(c=="C++17")
        _Cpp17->setChecked(true);
    else
        _Cpp20->setChecked(true);
}
void MainWindow::passDay() {
    _DayPassedC = true;
    _DayPassedS = true;
    updateStatus();
    _Timer->stop();
    _Timer->setSingleShot(false);
    _Timer->start(24*3600*1000);
}

void MainWindow::chooseStyle(QAction* act) {
    if(act != _CurrStyleAction and act != _ActionChangeStyle and
            act != _ActionLoadStyle) {
        _CurrStyleAction->setChecked(false);
        act->setChecked(true);
        _Widget->setStyle(_Styles[act->text()]);
        _CurrStyleAction = act;
    }
}
void MainWindow::showChangeStyle() {
    _CurrStyle = _Widget->getStyle();
    QPalette p;

    p.setColor(QPalette::Active, QPalette::Button,
               _CurrStyle.keywordFormat.foreground().color());
    p.setColor(QPalette::Inactive,QPalette::Button,
               _CurrStyle.keywordFormat.foreground().color());
    _KeywordB->setPalette(p);

    p.setColor(QPalette::Active, QPalette::Button,
               _CurrStyle.singleLineCommentFormat.foreground().color());
    p.setColor(QPalette::Inactive, QPalette::Button,
               _CurrStyle.singleLineCommentFormat.foreground().color());
    _SLCommentB->setPalette(p);

    p.setColor(QPalette::Active, QPalette::Button,
               _CurrStyle.multiLineCommentFormat.foreground().color());
    p.setColor(QPalette::Inactive, QPalette::Button,
               _CurrStyle.multiLineCommentFormat.foreground().color());
    _MLCommentB->setPalette(p);

    p.setColor(QPalette::Active, QPalette::Button,
               _CurrStyle.quotationFormat.foreground().color());
    p.setColor(QPalette::Inactive, QPalette::Button,
               _CurrStyle.quotationFormat.foreground().color());
    _QuotationB->setPalette(p);

    p.setColor(QPalette::Active, QPalette::Button,
               _CurrStyle.singleCharFormat.foreground().color());
    p.setColor(QPalette::Inactive, QPalette::Button,
               _CurrStyle.singleCharFormat.foreground().color());
    _SCharB->setPalette(p);

    p.setColor(QPalette::Active, QPalette::Button,
               _CurrStyle.functionFormat.foreground().color());
    p.setColor(QPalette::Inactive, QPalette::Button,
               _CurrStyle.functionFormat.foreground().color());
    _FunctionB->setPalette(p);

    p.setColor(QPalette::Active, QPalette::Button,
               _CurrStyle.directiveFormat.foreground().color());
    p.setColor(QPalette::Inactive, QPalette::Button,
               _CurrStyle.directiveFormat.foreground().color());
    _DirectiveB->setPalette(p);

    p.setColor(QPalette::Active, QPalette::Button,
               _CurrStyle.angleFormat.foreground().color());
    p.setColor(QPalette::Inactive, QPalette::Button,
               _CurrStyle.angleFormat.foreground().color());
    _AngleB->setPalette(p);

    _StyleDialog->exec();
}
void MainWindow::showLoadStyle() {
    QDir dir = QDir::currentPath();
    QString fileName = QFileDialog::getOpenFileName(
                                    _StyleDialog, "Сохранить стиль",
                                    dir.path(), "Initialization files (*.ini)");
    if(!fileName.isEmpty()){
        QSettings *styleSettings = new
                                  QSettings(fileName,QSettings::IniFormat,this);
        readStyle(styleSettings);

        QString styleName = QFileInfo(fileName).baseName();

        QAction *act = _MenuChangeStyle->addAction(styleName);
        act->setCheckable(true);
        act->setChecked(true);
        _CurrStyleAction->setChecked(false);
        _CurrStyleAction = act;
        _Styles[act->text()] = _CurrStyle;
        _Widget->setStyle(_Styles[act->text()]);
    }
}
void MainWindow::readStyle(QSettings* ss)
{
    _CurrStyle.directiveFormat.setForeground(
                            ss->value("style/directiveFormat").value<QColor>());
    _CurrStyle.functionFormat.setForeground(
                             ss->value("style/functionFormat").value<QColor>());
    _CurrStyle.keywordFormat.setForeground(
                              ss->value("style/keywordFormat").value<QColor>());
    _CurrStyle.multiLineCommentFormat.setForeground(
                     ss->value("style/multiLineCommentFormat").value<QColor>());
    _CurrStyle.quotationFormat.setForeground(
                            ss->value("style/quotationFormat").value<QColor>());
    _CurrStyle.singleCharFormat.setForeground(
                           ss->value("style/singleCharFormat").value<QColor>());
    _CurrStyle.singleLineCommentFormat.setForeground(
                    ss->value("style/singleLineCommentFormat").value<QColor>());
    _CurrStyle.angleFormat.setForeground(
                                ss->value("style/angleFormat").value<QColor>());
}
void MainWindow::createEditStyleDialog() {
    _StyleDialog = new QDialog(this);

    QBoxLayout *l = new QBoxLayout(QBoxLayout::TopToBottom);
    QFormLayout *tl = new QFormLayout();

    _KeywordB = new QPushButton(_StyleDialog);
    connect(_KeywordB, SIGNAL(clicked()), this, SLOT(setStyleColor()));
    _SLCommentB = new QPushButton(_StyleDialog);
    connect(_SLCommentB, SIGNAL(clicked()), this, SLOT(setStyleColor()));
    _MLCommentB = new QPushButton(_StyleDialog);
    connect(_MLCommentB, SIGNAL(clicked()), this, SLOT(setStyleColor()));
    _QuotationB = new QPushButton(_StyleDialog);
    connect(_QuotationB, SIGNAL(clicked()), this, SLOT(setStyleColor()));
    _SCharB = new QPushButton(_StyleDialog);
    connect(_SCharB, SIGNAL(clicked()), this, SLOT(setStyleColor()));
    _FunctionB = new QPushButton(_StyleDialog);
    connect(_FunctionB, SIGNAL(clicked()), this, SLOT(setStyleColor()));
    _DirectiveB = new QPushButton(_StyleDialog);
    connect(_DirectiveB, SIGNAL(clicked()), this, SLOT(setStyleColor()));
    _AngleB = new QPushButton(_StyleDialog);
    connect(_AngleB, SIGNAL(clicked()), this, SLOT(setStyleColor()));
    QPushButton *SaveB = new QPushButton(tr("Сохранить"), _StyleDialog);
    connect(SaveB, SIGNAL(clicked()), this, SLOT(saveStyle()));

    tl->addRow("Keyword",_KeywordB);
    tl->addRow("Single comment",_SLCommentB);
    tl->addRow("Multiline comment",_MLCommentB);
    tl->addRow("Quotation",_QuotationB);
    tl->addRow("Char quotation",_SCharB);
    tl->addRow("Function",_FunctionB);
    tl->addRow("Directives",_DirectiveB);
    tl->addRow("Angle brackets",_AngleB);
    l->addLayout(tl);
    l->addWidget(SaveB);

    _StyleDialog->setLayout(l);
}
void MainWindow::setStyleColor()
{
    QColor c;

    if(QObject::sender()==_KeywordB)
        c = _CurrStyle.keywordFormat.foreground().color();
    else if(sender()==_SLCommentB)
        c = _CurrStyle.singleLineCommentFormat.foreground().color();
    else if(sender()==_MLCommentB)
        c = _CurrStyle.multiLineCommentFormat.foreground().color();
    else if(sender()==_QuotationB)
        c = _CurrStyle.quotationFormat.foreground().color();
    else if(sender()==_SCharB)
        c = _CurrStyle.singleCharFormat.foreground().color();
    else if(sender()==_FunctionB)
        c = _CurrStyle.functionFormat.foreground().color();
    else if(sender()==_DirectiveB)
        c = _CurrStyle.directiveFormat.foreground().color();
    else if(sender()==_AngleB)
        c = _CurrStyle.angleFormat.foreground().color();
    else return;

    QPalette p;
    QColor nc = QColorDialog::getColor(c, _StyleDialog);
    if(nc.isValid())
    {
        p.setColor(QPalette::Active, QPalette::Button, nc);
        p.setColor(QPalette::Inactive, QPalette::Button, nc);
        if(QObject::sender()==_KeywordB){
            _CurrStyle.keywordFormat.setForeground(nc);
            _KeywordB->setPalette(p);
        }
        else if(QObject::sender()==_SLCommentB){
            _CurrStyle.singleLineCommentFormat.setForeground(nc);
            _SLCommentB->setPalette(p);
        }
        else if(QObject::sender()==_MLCommentB){
            _CurrStyle.multiLineCommentFormat.setForeground(nc);
            _MLCommentB->setPalette(p);
        }
        else if(QObject::sender()==_QuotationB){
            _CurrStyle.quotationFormat.setForeground(nc);
            _QuotationB->setPalette(p);
        }
        else if(QObject::sender()==_SCharB){
            _CurrStyle.singleCharFormat.setForeground(nc);
            _SCharB->setPalette(p);
        }
        else if(QObject::sender()==_FunctionB){
            _CurrStyle.functionFormat.setForeground(nc);
            _FunctionB->setPalette(p);
        }
        else if(QObject::sender()==_DirectiveB){
            _CurrStyle.directiveFormat.setForeground(nc);
            _DirectiveB->setPalette(p);
        }
        else if(QObject::sender()==_AngleB){
            _CurrStyle.angleFormat.setForeground(nc);
            _AngleB->setPalette(p);
        }
    }
}
void MainWindow::saveStyle()
{
    QDir dir = QDir::currentPath();
    QString fileName =
            QFileDialog::getSaveFileName(_StyleDialog, QString("Save Style"),
                                    dir.path(), "Initialization files (*.ini)");
    if(!fileName.isEmpty()){
        QString styleName = QFileInfo(fileName).baseName();
        _Widget->setStyle(_CurrStyle);

        if(fileName.right(4) != ".ini"){
            fileName += ".ini";
        }
        QSettings *ss = new QSettings(fileName,
                                                 QSettings::IniFormat,this);
        writeStyle(ss);
        ss->sync();
        _StyleDialog->close();

        QAction *act = _MenuChangeStyle->addAction(styleName);
        act->setCheckable(true);
        _Styles[act->text()] = _CurrStyle;
        _CurrStyleAction = act;
        _CurrStyleAction->setChecked(true);

        _Widget->setStyle(_CurrStyle);
    }
}
void MainWindow::writeStyle(QSettings* ss)
{
    ss->setValue("style/keywordFormat",
                 _CurrStyle.keywordFormat.foreground().color());
    ss->setValue("style/singleLineCommentFormat",
                 _CurrStyle.singleLineCommentFormat.foreground().color());
    ss->setValue("style/multiLineCommentFormat",
                 _CurrStyle.multiLineCommentFormat.foreground().color());
    ss->setValue("style/quotationFormat",
                 _CurrStyle.quotationFormat.foreground().color());
    ss->setValue("style/singleCharFormat",
                 _CurrStyle.singleCharFormat.foreground().color());
    ss->setValue("style/functionFormat",
                 _CurrStyle.functionFormat.foreground().color());
    ss->setValue("style/directiveFormat",
                 _CurrStyle.directiveFormat.foreground().color());
    ss->setValue("style/angleFormat",
                 _CurrStyle.angleFormat.foreground().color());
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
    _MainWindow = parent;
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
    _Highlighter = new Highlighter(_Doc);

    connect(_Doc, &QTextDocument::modificationChanged,
            this, &QWidget::setWindowModified);
    connect(_Doc, &QTextDocument::contentsChanged,
            parent, &MainWindow::updateStatus);
}
long Widget::getX() {
    return _TextField->textCursor().positionInBlock();
}
long Widget::getY() {
    return _TextField->textCursor().blockNumber() + 1;
}
long Widget::lineCount() {
    return _TextField->blockCount();
}
long Widget::wordCount() {
    return _TextField->toPlainText().split(" ",
                         Qt::SkipEmptyParts, Qt::CaseInsensitive).length()  +
            _TextField->toPlainText().split("\n",
                         Qt::SkipEmptyParts, Qt::CaseInsensitive).length() - 1;
}
long Widget::symbCount() {
    return _TextField->toPlainText().length();
}
long Widget::kbCount() {
    return QFileInfo(_File).size()/1024;
}
Style Widget::getStyle() {
    return _Highlighter->getStyle();
}
void Widget::setStyle(Style s) {
    _Highlighter->setStyle(s);
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
    _MainWindow->daySaved();
    _MainWindow->updateStatus();
}
void Widget::saveFile() {
    if (_File.isEmpty())
        _File = QFileDialog::getSaveFileName(this, "Сохранить");
    QFile file(_File);
    if (!file.open(QIODevice::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, "Ошибка", "Не сохранить файл: "
                                                          + file.errorString());
        return;
    }
    setWindowTitle(_File);
    QTextStream out(&file);
    QString text = _TextField->toPlainText();
    out << text;
    file.close();
    _LSave = QTime::currentTime();
    _LSaveDate = QDate::currentDate();
    _MainWindow->daySaved();
    _MainWindow->updateStatus();
}
void Widget::saveFileAs() {
    QString _File = QFileDialog::getSaveFileName(this, "Save as");
    QFile file(_File);

    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, "Ошибка", "Не сохранить файл: "
                                                          + file.errorString());
        return;
    }
    setWindowTitle(_File);
    QTextStream out(&file);
    QString text = _TextField->toPlainText();
    out << text;
    file.close();
    _LSave = QTime::currentTime();
    _LSaveDate = QDate::currentDate();
    _MainWindow->daySaved();
    _MainWindow->updateStatus();
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
void Widget::switchToolBar(bool arg) {
    _ToolBar->setVisible(arg);
}
void Widget::switchHighlight(bool arg) {
    _Highlighter->setVisibility(arg);
}

void Widget::c11Syntax() {
    _MainWindow->selectC("C11");
    _Highlighter->setStandart("C11/18");
}
void Widget::c18Syntax() {
    _MainWindow->selectC("C18");
    _Highlighter->setStandart("C11/18");
}
void Widget::cpp14Syntax() {
    _MainWindow->selectC("C++14");
    _Highlighter->setStandart("C++14");
}
void Widget::cpp17Syntax() {
    _MainWindow->selectC("C++17");
    _Highlighter->setStandart("C++17");
}
void Widget::cpp20Syntax() {
    _MainWindow->selectC("C++20");
    _Highlighter->setStandart("C++20");
}

void Widget::showAbout() {
    QDialog* Dialog = new QDialog(this);
    Dialog->setFixedSize(500, 250);
    QGridLayout *l = new QGridLayout(Dialog);
    Dialog->setWindowTitle("О программе");

    QPixmap me( ":images/mememememe.jpg" );
    me = me.scaled(350, 150, Qt::IgnoreAspectRatio);
    QLabel *pic = new QLabel(this);
    pic->setPixmap(me);
    QLabel *dateOfBuild = new QLabel();
    dateOfBuild->setText(tr(" Версия QT ") + QT_VERSION_STR +
                          "\n Тур Тимофей \n и мои 2 последние нервные клетки" +
                          "\n сделали это за 48 часов");

    l->addWidget(pic, 0, 0, Qt::AlignCenter);
    l->addWidget(dateOfBuild);
    Dialog->exec();
}
