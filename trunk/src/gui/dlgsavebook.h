/****************************************************************************
*   Copyright (C) 2014 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#ifndef DLGSAVEBOOK_H
#define DLGSAVEBOOK_H

#include <QDialog>

namespace Ui {
class DlgSaveBook;
}

class QProcess;

class DlgSaveBook : public QDialog
{
  Q_OBJECT

public:
  explicit DlgSaveBook(QString path, QWidget *parent = 0);
  ~DlgSaveBook();

signals:
  void bookWritten(QString);

protected:
  void accept();

public slots:
  void readOutput();

private:
  QString m_InputPath;
  QString m_OutputPath;
  QProcess* m_process;
  QString m_polyglotText;

  Ui::DlgSaveBook *ui;
};

#endif // DLGSAVEBOOK_H
