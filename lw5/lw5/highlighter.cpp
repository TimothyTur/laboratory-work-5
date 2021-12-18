#include "highlighter.h"

Highlighter::Highlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent), _Visib(true)
{
    _DiPatt << "^#include" << "^#ifndef"<< "^#define" << "^#undef" << "^#if"
            << "^#ifndef" << "^#else" << "^#elif" << "^#endif" << "^#line"
            << "^#error" << "^#warning" << "^#pragma";
    setStandart("C11/18");
    setDefStyle(true);
}

void Highlighter::highlightBlock(const QString &text)
{
    if(! _Visib) return;
    for(const HighlightingRule &rule: _Rules) {
        QRegularExpressionMatchIterator matchIterator =
                                                 rule.pattern.globalMatch(text);
        while (matchIterator.hasNext()){
            QRegularExpressionMatch match = matchIterator.next();
            setFormat(match.capturedStart(), match.capturedLength(),
                                                                   rule.format);
        }
    }
    setCurrentBlockState(0);

    int startIndex = 0;
    if (previousBlockState() != 1)
        startIndex = text.indexOf(comesspres);

    while (startIndex >= 0){
        QRegularExpressionMatch match = comespres.match(text, startIndex);
        int endIndex = match.capturedStart();
        int commentLength = 0;
        if (endIndex == -1){
            setCurrentBlockState(1);
            commentLength = text.length() - startIndex;
        }
        else{
            commentLength = endIndex - startIndex + match.capturedLength();
        }
        setFormat(startIndex, commentLength, _Style.multiLineCommentFormat);
        startIndex = text.indexOf(comesspres, startIndex + commentLength);
    }
}

void Highlighter::setRules() {
    _Rules.clear();
    HighlightingRule r;

    for(const QString & pattern : _KeyPatt) {
        r.pattern = QRegularExpression(pattern);
        r.format = _Style.keywordFormat;
        _Rules.append(r);
    }

    r.pattern = QRegularExpression("\".*\"");
    r.format = _Style.quotationFormat;
    _Rules.append(r);
    r.pattern = QRegularExpression("\'.*\'");
    r.format = _Style.singleCharFormat;
    _Rules.append(r);
    r.pattern = QRegularExpression("\\b[A-Za-z0-9_]+(?=\\()");
    r.format = _Style.functionFormat;
    _Rules.append(r);
    r.pattern = QRegularExpression("//[^\n]*");
    r.format = _Style.singleLineCommentFormat;
    _Rules.append(r);

    comesspres = QRegularExpression("/\\*");
    comespres = QRegularExpression("\\*/");

    for(const QString& pattern : _DiPatt) {
        r.pattern = QRegularExpression(pattern);
        r.format = _Style.directiveFormat;
        _Rules.append(r);
    }

    r.pattern = QRegularExpression("<.*>");
    _Rules.append(r);
    rehighlight();
}
void Highlighter::setVisibility(bool f) {
    _Visib = f;
    rehighlight();
}
void Highlighter::setStandart(QString arg) {
    _S = arg;
    if (arg == "C11/18") {
        _KeyPatt.clear();
        _KeyPatt << "\\bauto\\b" << "\\bbreak\\b" << "\\bcase\\b"
                 << "\\bchar\\b" << "\\bconst\\b" << "\\bcontinue\\b"
                 << "\\bdefault\\b" << "\\bdo\\b" << "\\belse\\b"
                 << "\\bdouble\\b" << "\\belse\\b" << "\\benum\\b"
                 << "\\bextern\\b" << "\\bfloat\\b" << "\\bfor\\b"
                 << "\\bgoto\\b" << "\\bif\\b" << "\\binline\\b" << "\\bint\\b"
                 << "\\blong\\b" << "\\bregister\\b" << "\\brestrict\\b"
                 << "\\breturn\\b" << "\\bshort\\b" << "\\bsigned\\b"
                 << "\\bsizeof\\b" << "\\bstatic\\b" << "\\bstruct\\b"
                 << "\\bswitch\\b" << "\\btypedef\\b" << "\\bunion\\b"
                 << "\\bunsigned\\b" << "\\bvoid\\b" << "\\bvolatile\\b"
                 << "\\bwhile\\b" << "\\balignas\\b" << "\\balignof\\b"
                 << "\\batomic_\\w+\\b" << "\\bbool\\b" << "\\bcomplex\\b"
                 << "\\bimaginary\\b" << "\\bnoreturn\\b"
                 << "\\bstatic_assert\\b" << "\\bthread_local\\b";
    }
    else if (arg == "C++14") {
        _KeyPatt.clear();
        _KeyPatt << "\\balignas\\b" << "\\balignof\\b" << "\\bchar16_t\\b"
                 << "\\bchar32_t\\b" << "\\bconstexpr\\b" << "\\bdecltype\\b"
                 << "\\bnoexcept\\b" << "\\bnullptr\\b"
                 << "\\bstatic_assert\\b" << "\\bthread_local\\b"
                 << "\\band\\b" << "\\band_eq\\b" << "\\basm\\b"
                 << "\\bauto\\b" << "\\bitand\\b" << "\\bitor\\b"
                 << "\\bbool\\b" << "\\bbreak\\b" << "\\bcase\\b"
                 << "\\bcatch\\b" << "\\bchar\\b" << "\\bclass\\b"
                 << "\\bcompl\\b" << "\\bconst\\b" << "\\bconst_cast\\b"
                 << "\\bcontinue\\b" << "\\bdefault\\b" << "\\bdelete\\b"
                 << "\\bdo\\b" << "\\bdouble\\b" << "\\bdynamic_cast\\b"
                 << "\\belse\\b" << "\\benum\\b" << "\\bexplicit\\b"
                 << "\\bexport\\b" << "\\bextern\\b" << "\\bfalse\\b"
                 << "\\bfloat\\b" << "\\bfor\\b" << "\\bfriend\\b"
                 << "\\bgoto\\b" << "\\bif\\b" << "\\binline\\b"
                 << "\\bint\\b" << "\\blong\\b" << "\\bmutable\\b"
                 << "\\bnamespace\\b" << "\\bnew\\b" << "\\bnot\\b"
                 << "\\bnot_eq\\b" << "\\boperator\\b" << "\\bor\\b"
                 << "\\bor_eq\\b" << "\\bprivate\\b" << "\\bprotected\\b"
                 << "\\bpublic\\b" << "\\bregister\\b"
                 << "\\breinterpret_cast\\b" << "\\breturn\\b"
                 << "\\bshort\\b" << "\\bsigned\\b" << "\\bsizeof\\b"
                 << "\\bstatic\\b" << "\\bstatic_cast\\b" << "\\bstruct\\b"
                 << "\\bswitch\\b" << "\\btemplate\\b" << "\\bthis\\b"
                 << "\\bthrow\\b" << "\\btrue\\b" << "\\btry\\b"
                 << "\\btypedef\\b" << "\\btypeid\\b" << "\\bunion\\b"
                 << "\\bunsigned\\b" << "\\busing\\b" << "\\bvirtual\\b"
                 << "\\bvoid\\b" << "\\bvolatile\\b" << "\\bwchar_t\\b"
                 << "\\bwhile\\b" << "\\bxor\\b" << "\\bxor_eq\\b";
    }
    else if (arg == "C++17") {
        _KeyPatt.clear();
        _KeyPatt << "\\balignas\\b" << "\\balignof\\b" << "\\bchar16_t\\b"
                 << "\\bchar32_t\\b" << "\\bconstexpr\\b" << "\\bdecltype\\b"
                 << "\\bnoexcept\\b" << "\\bnullptr\\b" << "\\bstatic_assert\\b"
                 << "\\bthread_local\\b" << "\\band\\b" << "\\band_eq\\b"
                 << "\\basm\\b" << "\\bauto\\b" << "\\bitand\\b" << "\\bitor\\b"
                 << "\\bbool\\b" << "\\bbreak\\b" << "\\bcase\\b"
                 << "\\bcatch\\b" << "\\bchar\\b" << "\\bclass\\b"
                 << "\\bcompl\\b" << "\\bconst\\b" << "\\bconst_cast\\b"
                 << "\\bcontinue\\b" << "\\bdefault\\b" << "\\bdelete\\b"
                 << "\\bdo\\b" << "\\bdouble\\b" << "\\bdynamic_cast\\b"
                 << "\\belse\\b" << "\\benum\\b" << "\\bexplicit\\b"
                 << "\\bexport\\b" << "\\bextern\\b" << "\\bfalse\\b"
                 << "\\bfloat\\b" << "\\bfor\\b" << "\\bfriend\\b"
                 << "\\bgoto\\b" << "\\bif\\b" << "\\binline\\b" << "\\bint\\b"
                 << "\\blong\\b" << "\\bmutable\\b" << "\\bnamespace\\b"
                 << "\\bnew\\b" << "\\bnot\\b" << "\\bnot_eq\\b"
                 << "\\boperator\\b" << "\\bor\\b" << "\\bor_eq\\b"
                 << "\\bprivate\\b" << "\\bprotected\\b" << "\\bpublic\\b"
                 << "\\bregister\\b" << "\\breinterpret_cast\\b"
                 << "\\breturn\\b" << "\\bshort\\b" << "\\bsigned\\b"
                 << "\\bsizeof\\b" << "\\bstatic\\b" << "\\bstatic_cast\\b"
                 << "\\bstruct\\b" << "\\bswitch\\b" << "\\btemplate\\b"
                 << "\\bthis\\b" << "\\bthrow\\b" << "\\btrue\\b" << "\\btry\\b"
                 << "\\btypedef\\b" << "\\btypeid\\b"  << "\\bunion\\b"
                 << "\\bunsigned\\b" << "\\busing\\b" << "\\bvirtual\\b"
                 << "\\bvoid\\b" << "\\bvolatile\\b" << "\\bwchar_t\\b"
                 << "\\bwhile\\b" << "\\bxor\\b" << "\\bxor_eq\\b";
    }
    else if (arg == "C++20") {
        _KeyPatt.clear();
        _KeyPatt << "\\bchar8_t\\b" << "\\bconcept\\b" << "\\bconsteval\\b"
                 << "\\bconstinit\\b" << "\\bco_await\\b" << "\\bco_return\\b"
                 <<  "\\bco_yield\\b" << "\\brequires\\b" << "\\balignas\\b"
                 << "\\balignof\\b" << "\\bchar16_t\\b" << "\\bchar32_t\\b"
                 << "\\bconstexpr\\b" << "\\bdecltype\\b" << "\\bnoexcept\\b"
                 << "\\bnullptr\\b" << "\\bstatic_assert\\b"
                 << "\\bthread_local\\b" << "\\band\\b" << "\\band_eq\\b"
                 << "\\basm\\b" << "\\bauto\\b" << "\\bbitand\\b"
                 << "\\bbitor\\b" << "\\bbool\\b" << "\\bbreak\\b"
                 << "\\bcase\\b" << "\\bcatch\\b" << "\\bchar\\b"
                 << "\\bclass\\b" << "\\bcompl\\b" << "\\bconst\\b"
                 << "\\bconst_cast\\b" << "\\bcontinue\\b" << "\\bdefault\\b"
                 << "\\bdelete\\b" << "\\bdo\\b" << "\\bdouble\\b"
                 << "\\bdynamic_cast\\b" << "\\belse\\b" << "\\benum\\b"
                 << "\\bexplicit\\b" << "\\bexport\\b" << "\\bextern\\b"
                 << "\\bfalse\\b" << "\\bfloat\\b" << "\\bfor\\b"
                 << "\\bfriend\\b" << "\\goto\\b" << "\\bif\\b"
                 << "\\binline\\b" << "\\bint\\b" << "\\blong\\b"
                 << "\\bmutable\\b" << "\\bnamespace\\b" << "\\bnew\\b"
                 << "\\bnot\\b" << "\\bnot_eq\\b" << "\\boperator\\b"
                 << "\\bor\\b" << "\\bor_eq\\b" << "\\bprivate\\b"
                 << "\\bprotected\\b" << "\\bpublic\\b"
                 << "\\bregister\\b" << "\\breinterpret_cast\\b"
                 << "\\breturn\\b" << "\\bshort\\b" << "\\bsigned\\b"
                 << "\\bsizeof\\b" << "\\bstatic\\b" << "\\bstatic_cast\\b"
                 << "\\bstruct\\b" << "\\bswitch\\b" << "\\btemplate\\b"
                 << "\\bthis\\b" << "\\bthrow\\b" << "\\btrue\\b" << "\\btry\\b"
                 << "\\btypedef\\b" << "\\btypeid\\b" << "\\bunion\\b"
                 << "\\bunsigned\\b" << "\\busing\\b" << "\\bvirtual\\b"
                 << "\\bvoid\\b" << "\\bvolatile\\b" << "\\bwchar_t\\b"
                 << "\\bwhile\\b" << "\\bxor\\b"  << "\\bxor_eq\\b";
        }
    else return;
    setRules();
}

bool Highlighter::getVisible() const {
    return _Visib;
}
QString Highlighter::getStandart() const {
    return _S;
}
Style Highlighter::getStyle() const {
    return _Style;
}
void Highlighter::setDefStyle(bool change) {
    if(! change) return;
    _Style.functionFormat.setForeground(Qt::darkCyan);
    _Style.quotationFormat.setForeground(Qt::darkGreen);
    _Style.keywordFormat.setForeground(Qt::darkRed);
    _Style.keywordFormat.setFontWeight(QFont::Bold);
    _Style.singleLineCommentFormat.setForeground(Qt::darkGray);
    _Style.multiLineCommentFormat.setForeground(Qt::darkGray);
    _Style.directiveFormat.setForeground(Qt::darkMagenta);
    _Style.directiveFormat.setFontWeight(QFont::Bold);
    _Style.singleCharFormat.setForeground(Qt::darkGreen);
    setRules();
}
void Highlighter::setStyle(Style style) {
    _Style = style;
    _Style.directiveFormat.setFontWeight(QFont::Bold);
    _Style.keywordFormat.setFontWeight(QFont::Bold);
    setRules();
}
