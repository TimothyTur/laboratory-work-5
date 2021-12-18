#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include <QPlainTextEdit>

class CodeEditor : public QPlainTextEdit
{
    Q_OBJECT

public:
    CodeEditor(QWidget *parent = 0);

    void lineNumberAreaPaintEvent(QPaintEvent*);
    int lineNumberAreaWidth();
    QColor getBGColor() const;
    QColor getLColor() const;
    bool isVisibleLineNumberArea() const;

    bool flag = 1;

protected:
    void resizeEvent(QResizeEvent*);

public slots:
    void updateLineNumberAreaWidth(int);
    void highlightCurrentLine();
    void updateLineNumberArea(const QRect &, int);
    void setBackgroundColor(QColor newColor);
    void setLineColor(QColor newColor);

private:
    QWidget *lineNumberArea;
    QColor BGColor = Qt::white,
           LColor = Qt::yellow;
};

class LineNumberArea : public QWidget
{
public:
    LineNumberArea(CodeEditor *editor = 0);
    QSize sizeHint() const;

protected:
    void paintEvent(QPaintEvent *event);

private:
    CodeEditor *codeEditor;
};

#endif // CODEEDITOR_H
