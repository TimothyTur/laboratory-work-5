#ifndef HIGHLIGHTER_H
#define HIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QRegularExpression>
#include <QTextCharFormat>
#include <QStringList>


struct Style {
    QTextCharFormat singleLineCommentFormat,
                    directiveFormat,
                    keywordFormat,
                    angleFormat,
                    classFormat,
                    singleCharFormat,
                    quotationFormat,
                    functionFormat,
                    multiLineCommentFormat;
};

class Highlighter : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    Highlighter(QTextDocument *parent = nullptr);

    bool getVisible() const;
    QString getStandart() const;
    Style getStyle() const;

public slots:
    void setVisibility(bool);
    void setStandart(QString);
    void setDefStyle(bool);
    void setStyle(Style);

protected:
    void highlightBlock(const QString &text) override;

private:
    void setRules();

    struct HighlightingRule
    {
        QRegularExpression pattern;
        QTextCharFormat format;
    };
    QList<HighlightingRule> _Rules;
    QRegularExpression comesspres, comespres;
    QStringList _DiPatt, _KeyPatt;
    bool _Visib = true;
    Style _Style;
    QString _S;
};

#endif // HIGHLIGHTER_H
