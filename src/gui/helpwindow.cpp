/***************************************************************************
                          helpwindow.cpp -  Help Window
                             -------------------
    begin                : 01/10/2005
    copyright            : (C) 2005 Heinz R. Hopfgartner <heinz.hopfgartner@gmx.at>
                           (C) 2006 Michal Rudolf <mrudolf@kdewebdev.org>
***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   The code is based on Trolltech Qt 3 - Help browser example            *
 ***************************************************************************/

#include "helpwindow.h"
#include "settings.h"

#include <qapplication.h>
#include <qcombobox.h>
#include <qdatastream.h>
#include <qdir.h>
#include <qevent.h>
#include <q3filedialog.h>
#include <qfile.h>
#include <qfile.h>
#include <qfileinfo.h>
#include <qicon.h>
#include <qlineedit.h>
#include <qmenubar.h>
#include <qmessagebox.h>
#include <qobject.h>
#include <q3paintdevicemetrics.h>
#include <qpainter.h>
#include <qpixmap.h>
#include <q3popupmenu.h>
#include <qprinter.h>
#include <q3simplerichtext.h>
#include <qstatusbar.h>
#include <q3stylesheet.h>
#include <q3textbrowser.h>
#include <qtextstream.h>
#include <q3toolbar.h>
#include <qtoolbutton.h>
//Added by qt3to4:
#include <QCloseEvent>
#include <Q3Frame>

#include <ctype.h>

HelpWindow::HelpWindow()
  : Q3MainWindow(0, "", Qt::WDestructiveClose),
    m_pathCombo(0)
{

  setName("HelpWindow");
  configure();

  m_browser = new Q3TextBrowser(this);

  m_browser->mimeSourceFactory()->setFilePath(".");
  m_browser->setFrameStyle(Q3Frame::Panel | Q3Frame::Sunken);
  connect(m_browser, SIGNAL(sourceChanged(const QString&)),
	   this, SLOT(sourceChanged(const QString&)));

  setCentralWidget(m_browser);

  if (!m_home.isEmpty())
    m_browser->setSource(m_home);

  connect(m_browser, SIGNAL(highlighted(const QString&)), statusBar(), SLOT(message(const QString&)));
  resize(640,700);

  QIcon icon_fileopen(QPixmap(AppSettings->iconPath("fileopen")));
  QIcon icon_print(QPixmap(AppSettings->iconPath("print")));

  Q3PopupMenu* file = new Q3PopupMenu(this);
  file->insertItem(icon_fileopen, tr("&Open File"), this, SLOT(openFile()), Qt::CTRL + Qt::Key_O);
  file->insertItem(icon_print,  tr("&Print"), this, SLOT(print()), Qt::CTRL + Qt::Key_P);
  file->insertSeparator();
  file->insertItem(tr("&Close"), this, SLOT(hide()), Qt::CTRL + Qt::Key_C);

  QIcon icon_back(QPixmap(AppSettings->iconPath("back")));
  QIcon icon_forward(QPixmap(AppSettings->iconPath("forward")));
  QIcon icon_home(QPixmap(AppSettings->iconPath("home")));

  Q3PopupMenu* go = new Q3PopupMenu(this);
  m_backwardId = go->insertItem(icon_back, tr("&Backward"), m_browser, SLOT(backward()),
			       Qt::CTRL + Qt::Key_Left);
  m_forwardId = go->insertItem(icon_forward, 
			      tr("&Forward"), m_browser, SLOT(forward()),
			      Qt::CTRL + Qt::Key_Right);
  go->insertItem(icon_home, tr("&Home"), m_browser, SLOT(home()));

  Q3PopupMenu* help = new Q3PopupMenu(this);
  help->insertItem(tr("&About"), this, SLOT(about()));
  help->insertItem(tr("About &Qt"), this, SLOT(aboutQt()));

  menuBar()->insertItem(tr("&File"), file);
  menuBar()->insertItem(tr("&Go"), go);
  menuBar()->insertSeparator();
  menuBar()->insertItem(tr("&Help"), help);

  menuBar()->setItemEnabled(m_forwardId, FALSE);
  menuBar()->setItemEnabled(m_backwardId, FALSE);
  connect(m_browser, SIGNAL(backwardAvailable(bool)),
	   this, SLOT(setBackwardAvailable(bool)));
  connect(m_browser, SIGNAL(forwardAvailable(bool)),
	   this, SLOT(setForwardAvailable(bool)));


  Q3ToolBar* toolbar = new Q3ToolBar(this);
  addToolBar(toolbar, "Toolbar");
  QToolButton* button;

  button = new QToolButton(icon_back, tr("Backward"), "", m_browser, SLOT(backward()), toolbar);
  connect(m_browser, SIGNAL(backwardAvailable(bool)), button, SLOT(setEnabled(bool)));
  button->setEnabled(FALSE);
  button = new QToolButton(icon_forward, tr("Forward"), "", m_browser, SLOT(forward()), toolbar);
  connect(m_browser, SIGNAL(forwardAvailable(bool)), button, SLOT(setEnabled(bool)));
  button->setEnabled(FALSE);
  button = new QToolButton(icon_home, tr("Home"), "", m_browser, SLOT(home()), toolbar);

  toolbar->addSeparator();

  m_pathCombo = new QComboBox(TRUE, toolbar);
  connect(m_pathCombo, SIGNAL(activated(const QString &)),
	   this, SLOT(pathSelected(const QString &)));
  toolbar->setStretchableWidget(m_pathCombo);
  setRightJustification(TRUE);
  setDockEnabled(Qt::DockLeft, FALSE);
  setDockEnabled(Qt::DockRight, FALSE);

  m_pathCombo->insertItem(m_home);
  m_browser->setFocus();

}


void HelpWindow::setBackwardAvailable(bool b)
{
  menuBar()->setItemEnabled(m_backwardId, b);
}

void HelpWindow::setForwardAvailable(bool b)
{
  menuBar()->setItemEnabled(m_forwardId, b);
}


void HelpWindow::sourceChanged(const QString& url)
{
  if (m_browser->documentTitle().isNull())
    setCaption("ChessX - Helpviewer - " + url);
  else
    setCaption("ChessX - Helpviewer - " + m_browser->documentTitle()) ;

  if (!url.isEmpty() && m_pathCombo) {
    bool exists = FALSE;
    int i;
    for (i = 0; i < m_pathCombo->count(); ++i) {
      if (m_pathCombo->text(i) == url) {
	exists = TRUE;
	break;
      }
    }
    if (!exists) {
      m_pathCombo->insertItem(url, 0);
      m_pathCombo->setCurrentItem(0);
    } else
      m_pathCombo->setCurrentItem(i);
  }
}

HelpWindow::~HelpWindow()
{

}

void HelpWindow::about()
{

  QMessageBox::about(this, "About ChessX",
		     "ChessX\n\n"
		     "Copyright 2005-2006\n"
		     "GPL\n\n"
		     "http://chessx.sourceforge.net/\n");
}


void HelpWindow::aboutQt()
{
  QMessageBox::aboutQt(this, "ChessX - Help");
}

void HelpWindow::openFile()
{
#ifndef QT_NO_FILEDIALOG
  QString fn = Q3FileDialog::getOpenFileName(QString::null, QString::null, this);
  if (!fn.isEmpty())
    m_browser->setSource(fn);
#endif
}


void HelpWindow::print()
{
#ifndef QT_NO_PRINTER
  QPrinter printer(QPrinter::HighResolution);
  printer.setFullPage(TRUE);
  if (printer.setup(this)) {
    QPainter p(&printer);
    if(!p.isActive()) // starting printing failed
      return;
    Q3PaintDeviceMetrics metrics(p.device());
    int dpiy = metrics.logicalDpiY();
    int margin = (int) ((2/2.54)*dpiy); // 2 cm margins
    QRect view(margin, margin, metrics.width() - 2*margin, metrics.height() - 2*margin);
    Q3SimpleRichText richText(m_browser->text(),
			      QFont(),
			      m_browser->context(),
			      m_browser->styleSheet(),
			      m_browser->mimeSourceFactory(),
			      view.height());
    richText.setWidth(&p, view.width());
    int page = 1;
    do {
      richText.draw(&p, margin, margin, view, colorGroup());
      view.moveBy(0, view.height());
      p.translate(0 , -view.height());
      p.drawText(view.right() - p.fontMetrics().width(QString::number(page)),
		  view.bottom() + p.fontMetrics().ascent() + 5, QString::number(page));
      if (view.top() - margin >= richText.height())
	break;
      printer.newPage();
      page++;
    } while (TRUE);
  }
#endif
}

void HelpWindow::pathSelected(const QString &_path)
{
  m_browser->setSource(_path);
}

void HelpWindow::configure()
{
  /** store default - read initial help page to/from the ChessX settings file */
  AppSettings->beginGroup("/helpbrowser/");
  m_home = QDir(AppSettings->readEntry("home", AppSettings->dataPath() + "/doc/index.html")).absPath();
  AppSettings->endGroup();
}

void HelpWindow::closeEvent(QCloseEvent* e)
{
  hide();
  e->ignore();
}
