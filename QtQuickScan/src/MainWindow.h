/*
 *
 * ©K. D. Hedger. Wed  8 Apr 16:10:08 BST 2026 keithdhedger@gmail.com

 * This file (MainWindow.h) is part of QtQuickScan.

 * QtQuickScan is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * QtQuickScan is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with QtQuickScan.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _MAINWINDOW_
#define _MAINWINDOW_

extern QString	scanPath;
extern QString	previewPath;

class MainWindowClass: public QMainWindow
{
	public:
		MainWindowClass();
		~MainWindowClass();

		QMenuBar				menuBar;
		QMenu				*fileMenu=NULL;
		QMenu				*deviceMenu=NULL;
		QMenu				*resoMenu=NULL;
		QMenu				*colourMenu=NULL;
		QMenu				*helpMenu=NULL;
		QMenu				*cropMenu=NULL;
		QImage				image2;
		QLabel				*statusText=NULL;
		QLabel				*statusText2=NULL;
		QAction				*toggleBandMenuItem;
		QString				realDataDir;

		prefsWidgetsClass	prefs;
		ImageLabelClass		*label1;
		ScannerClass			scanner;
		UtilitiesClass		utils;

		void					loadImage(QString filename);
		void					setSensitive(void);
		void					setInfoBar(void);

	private:
		bool					gotScan=false;
		void					setFileMenu(void);
		void					setDeviceMenu(void);
		void					setResoMenu(void);
		void					setColourMenu(void);
		QMenu				*setHelpMenu(QMenuBar *menubar);
		QMenu				*setCropMenu(QMenuBar *menubar);
		QImage				fullImage;

	protected:
	    void					closeEvent(QCloseEvent *event) override;
		void					resizeEvent(QResizeEvent *event)	override;
};

#endif
