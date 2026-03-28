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

#include <QApplication>
#include <QMainWindow>
#include <QCloseEvent>
#include <QMenuBar>
#include <QActionGroup>
#include <QVBoxLayout>
#include <QImage>
#include <QLabel>
#include <QFile>
#include <QFileInfo>
#include <QImageReader>
#include <QImageWriter>
#include <QProcess>

#include <sane/sane.h>
#include <unistd.h>

#include "Utilities.h"
class UtilitiesClass;

#include "Scanner.h"
class ScannerClass;

#include "MainWindow.h"
class MainWindowClass;


#define BUFFERSIZE 1048510

enum fileEnums {PREVIEWITEM=1,SCANITEM,DIV1,OPENINGIMPITEM,DIV3,SAVEITEM,SAVEASJPGITEM,SAVEASPNGITEM,SAVEASPNMITEM,DIV2,QUITITEM};

extern MainWindowClass	*mwc;
extern QString			tmpFolderPath;
extern QString			previewPath;
extern QString			scanPath;

#endif
