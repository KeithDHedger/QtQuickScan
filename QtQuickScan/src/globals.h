/*
 *
 * ©K. D. Hedger. Sun 22 Mar 15:08:09 GMT 2026 keithdhedger@gmail.com

 * This file (globals.h) is part of Qt6Scan.

 * Qt6Scan is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation,either version 3 of the License,or
 * (at your option) any later version.

 * Qt6Scan is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with Qt6Scan.  If not,see <http://www.gnu.org/licenses/>.
*/

#ifndef _GLOBALS_
#define _GLOBALS_

#include <QActionGroup>
#include <QApplication>
#include <QCheckBox>
#include <QCloseEvent>
#include <QColorDialog>
#include <QComboBox>
#include <QDialog>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QFontDialog>
#include <QHBoxLayout>
#include <QImage>
#include <QImageReader>
#include <QImageWriter>
#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>
#include <QMenuBar>
#include <QMessageBox>
#include <QProcess>
#include <QPushButton>
#include <QSettings>
#include <QSpinBox>
#include <QSplashScreen>
#include <QString>
#include <QVBoxLayout>
#include <QWidget>
#include <QString>
#include <QImageWriter>
#include <QImageReader>
#include <QSettings>
#include <QMessageBox>
#include <QTextBrowser>
#include <QDialogButtonBox>

#include <sane/sane.h>
#include <unistd.h>

#include "../../config.h"

#include "Utilities.h"
class UtilitiesClass;

#include "QT_prefsWidgets.h"
class prefsWidgetsClass;

#include "Scanner.h"
class ScannerClass;

#include "MainWindow.h"
class MainWindowClass;

#define BUFFERSIZE 1048510

enum helpEnums{ABOUTITEM=600,ABOUTQTITEM,HELPITEM};
enum fileEnums {PREVIEWITEM=1,SCANITEM,DIV1,OPENINGIMPITEM,DIV3,SAVEITEM,SAVEASJPGITEM,SAVEASPNGITEM,SAVEASPNMITEM,DIV2,QUITITEM};

extern MainWindowClass	*mwc;
extern QString			tmpFolderPath;
extern QString			previewPath;
extern QString			scanPath;

#endif
