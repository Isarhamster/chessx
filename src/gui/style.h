/****************************************************************************
*   Copyright (C) 2014 by Jens Nissen jens-chessx@gmx.net                   *
****************************************************************************/

#ifndef STYLE_H
#define STYLE_H

#include <QProxyStyle>

class QApplication;

class Style : public QProxyStyle {
  Q_OBJECT

 public:
  Style();
  explicit Style(QStyle *style);

  QStyle *baseStyle();

  void polish(QPalette &palette) override;
  void polish(QApplication *app) override;

  void loadStyle(QApplication *app);
  void modifyPalette(QPalette& palette);

  static bool isDarkTheme;

 private:
  bool isDark() const;
  QStyle *styleBase();

};



#endif // STYLE_H
