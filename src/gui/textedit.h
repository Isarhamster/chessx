/****************************************************************************
*   Copyright (C) 2015 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/
/****************************************************************************
**
** Copyright (C) 2014 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the demonstration applications of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL21$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia. For licensing terms and
** conditions see http://qt.digia.com/licensing. For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 or version 3 as published by the Free
** Software Foundation and appearing in the file LICENSE.LGPLv21 and
** LICENSE.LGPLv3 included in the packaging of this file. Please review the
** following information to ensure the GNU Lesser General Public License
** requirements will be met: https://www.gnu.org/licenses/lgpl.html and
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights. These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef TEXTEDIT_H
#define TEXTEDIT_H

#include "toolmainwindow.h"

#include <QMap>
#include <QPointer>
#include <QUrl>
#include <QImage>
#include <QTextEdit>

QT_BEGIN_NAMESPACE
class QAction;
class QComboBox;
class QFontComboBox;
class QSpinBox;
class QTextEdit;
class QTextCharFormat;
class QMenu;
class QMimeData;
class QPrinter;
QT_END_NAMESPACE

class PasteTextEdit : public QTextEdit
{
    Q_OBJECT
public:
    explicit PasteTextEdit(QWidget *parent = 0);

    bool canInsertUsingMimeData(const QMimeData *source) const;
    void insertUsingMimeData(const QMimeData *source);
    void resizeImage(QPair<double, double> factor);
    void insertImage(const QImage &image);

protected:
    virtual bool canInsertFromMimeData(const QMimeData *source) const;
    virtual void insertFromMimeData(const QMimeData *source);
    bool eventFilter(QObject *obj, QEvent *event);
private:
    void dropTextFile(const QUrl &url);
    void dropImage(const QUrl &url, const QImage &image);
    int m_imageCounter;
};

class TextEdit : public ToolMainWindow
{
    Q_OBJECT

public:
    TextEdit(QWidget *parent = 0, QMenu* menu = 0);
    bool saveDocument();

protected:
    bool canInsertFromMimeData(const QMimeData *source) const;
    void insertFromMimeData(const QMimeData *source);

private:
    void setupFileActions(QMenu* xmenu);
    void setupEditActions(QMenu* menu);
    void setupTextActions(QMenu* menu);
    void setupStyleActions();

    bool load(const QString &f);
    void setCurrentFileName(const QString &fileName);

signals:
    void requestBoardImage(QImage& image, double scaling);

private slots:
    void fileNew();
    void fileOpen();
    bool fileSave();
    bool fileSaveAs();
    void filePrint();
    void filePrintPreview();
    void filePrintPdf();

    void textBold();
    void textUnderline();
    void textItalic();
    void textFamily(const QString &f);
    void textSize(const QString &p);
    void textStyle(int styleIndex);
    void textColor();
    void textAlign(QAction *a);

    void currentCharFormatChanged(const QTextCharFormat &format);
    void cursorPositionChanged();

    void clipboardDataChanged();
    void printPreview(QPrinter *);

    void imageResize();
    void pickBoard();

    void setupDocumentActions();
private:
    void mergeFormatOnWordOrSelection(const QTextCharFormat &format);
    void fontChanged(const QFont &f);
    void colorChanged(const QColor &c);
    void alignmentChanged(Qt::Alignment a);

    QAction *actionSave;
    QAction *actionTextBold;
    QAction *actionTextUnderline;
    QAction *actionTextItalic;
    QAction *actionTextColor;
    QAction *actionAlignLeft;
    QAction *actionAlignCenter;
    QAction *actionAlignRight;
    QAction *actionAlignJustify;
    QAction *actionUndo;
    QAction *actionRedo;
    QAction *actionCut;
    QAction *actionCopy;
    QAction *actionPaste;
    QAction *actionImageSize;
    QAction *actionPickBoard;

    QFontComboBox *comboFont;
    QComboBox *comboSize;
    QSpinBox *imageSize;

    QString fileName;
    PasteTextEdit *textEdit;
};

#endif // TEXTEDIT_H
