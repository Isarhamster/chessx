/****************************************************************************
*   Copyright (C) 2014 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#ifndef DLGSAVEBOOK_H
#define DLGSAVEBOOK_H

#include <QDialog>

namespace Ui {
class DlgSaveBook;
}

class DlgSaveBook : public QDialog
{
  Q_OBJECT

public:
  explicit DlgSaveBook(QString path, QWidget *parent = 0);
  ~DlgSaveBook();

  void getBookParameters(QString& out, int& maxPly, int& minGame, bool&  uniform);

signals:
  void bookWritten(QString);

protected:
  void accept();

public slots:

private:
  QString m_InputPath;
  QString m_OutputPath;
  Ui::DlgSaveBook *ui;

};

#endif // DLGSAVEBOOK_H
