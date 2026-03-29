/*
 *
 * ©K. D. Hedger. Sun 22 Mar 15:01:35 GMT 2026 keithdhedger@gmail.com

 * This file (main.cpp) is part of Qt6Scan.

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

#include "globals.h"

int main(int argc,char** argv)
{
	QApplication	app(argc,argv);
	char			tmpfoldertemplate[]="/tmp/QtQuickScan-XXXXXX";

//	QSplashScreen ss=QSplashScreen(QString(DATADIR)+"/pixmaps/QtQuickScan.png",Qt::FramelessWindowHint|Qt::X11BypassWindowManagerHint);
//	ss.show();
	//qApp->processEvents();
	tmpFolderPath=mkdtemp(tmpfoldertemplate);
	if(tmpFolderPath.isEmpty()==true)
		{
			qDebug()<<"Can't create temporary folder,quitting ...";
			exit (100);
		}

	previewPath=QString("%1/preview.pnm").arg(tmpFolderPath);
	scanPath=QString("%1/scan.pnm").arg(tmpFolderPath);
	
	mwc=new MainWindowClass;
	mwc->show();

	mwc->loadImage(scanPath);
	mwc->scanner.setDevice(mwc->scanner.deviceName);
	//ss.hide();

	app.exec();

	delete mwc;

	system(qPrintable(QString("rm -r %1").arg(tmpFolderPath)));
	return 0;
}