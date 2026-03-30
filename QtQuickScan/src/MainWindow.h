/*
 *
 * ©K. D. Hedger. Sun 22 Mar 15:12:32 GMT 2026 keithdhedger@gmail.com

 * This file (MainWindow.h) is part of Qt6Scan.

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

#ifndef _MAINWINDOW_
#define _MAINWINDOW_

extern QString	scanPath;
extern QString	previewPath;

class MainWindowClass: public QMainWindow
{
	public:
		MainWindowClass();
		~MainWindowClass();

		QMenuBar			menuBar;
		QMenu			*fileMenu=NULL;
		QMenu			*deviceMenu=NULL;
		QMenu			*resoMenu=NULL;
		QMenu			*colourMenu=NULL;
		QMenu			*helpMenu=NULL;
		QLabel			*label;

		ScannerClass		scanner;
		UtilitiesClass	utils;

		void				loadImage(QString filename);
		void				setSensitive(void);

	private:
		bool				gotScan=false;
		void				setFileMenu(void);
		void				setDeviceMenu(void);
		void				setResoMenu(void);
		void				setColourMenu(void);
		QMenu			*setHelpMenu(QMenuBar *menubar);

	protected:
	    void				closeEvent(QCloseEvent *event) override;
		void				resizeEvent(QResizeEvent *event)	override
			{
				QWidget::resizeEvent(event);
				if(QFileInfo::exists(scanPath))
					this->loadImage(scanPath);
				else if(QFileInfo::exists(previewPath))
					this->loadImage(previewPath);
			}
};

#endif
