/*
 *
 * ©K. D. Hedger. Sun 22 Mar 15:12:32 GMT 2026 keithdhedger@gmail.com

 * This file (MainWindow.cpp) is part of Qt6Scan.

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

MainWindowClass::~MainWindowClass()
{
	QSettings	settings("KDHedger",PACKAGE_NAME);
	QRect		rg;
	QRect		rf;

	rg=this->geometry();
	rf=this->frameGeometry();
	rf.setHeight(rf.height()-(rf.height()-rg.height()));
	rf.setWidth(rf.width()-(rf.width()-rg.width()));
	settings.setValue("app/geometry",rf);
	settings.setValue("app/name",this->utils.lastName);
	settings.setValue("app/dir",this->utils.lastDir);
	settings.setValue("app/sfx",this->utils.lastSFX);
}

void MainWindowClass::setFileMenu(void)
{
	QActionGroup		*actions;
	QAction			*act;

	this->fileMenu=this->menuBar.addMenu("&File");
	actions=new QActionGroup(this->fileMenu);
	actions->setExclusive(true);

	act=new QAction(QIcon::fromTheme("scanner"),"Preview",actions);
	act->setData(PREVIEWITEM);
	act=new QAction(QIcon::fromTheme("scanner"),"Scan",actions);
	act->setData(SCANITEM);

	act=new QAction(actions);
	act->setSeparator(true);

	act=new QAction(QIcon::fromTheme("document-open"),"Open In GIMP",actions);
	act->setData(OPENINGIMPITEM);

	act=new QAction(actions);
	act->setSeparator(true);

	act=new QAction(QIcon::fromTheme("document-save"),"Save",actions);
	act->setShortcut(QKeySequence::SaveAs);
	act->setData(SAVEITEM);

	act=new QAction(QIcon::fromTheme("document-save"),"Save As JPEG",actions);
	act->setData(SAVEASJPGITEM);

	act=new QAction(QIcon::fromTheme("document-save"),"Save As PNG",actions);
	act->setData(SAVEASPNGITEM);

	act=new QAction(QIcon::fromTheme("document-save"),"Save As PNM",actions);
	act->setData(SAVEASPNMITEM);

	act=new QAction(actions);
	act->setSeparator(true);

	act=new QAction(QIcon::fromTheme("application-exit"),"Quit",actions);
	act->setShortcut(QKeySequence::Quit);
	act->setData(QUITITEM);

	this->fileMenu->addActions(actions->actions());
	QObject::connect(actions,&QActionGroup::triggered,this,[this](QAction *action)
		{
			switch(action->data().toInt())
				{
					case OPENINGIMPITEM:
						QProcess::startDetached("gimp",QStringList()<<qPrintable(scanPath));
						break;
					case SAVEITEM:
						{
							QString filepath=QFileDialog::getSaveFileName(this,"Select file",this->utils.lastDir+"/"+this->utils.lastName+"."+this->utils.lastSFX,"All Images (*.png *.jpg *.jpeg *.bmp *.tif *.tiff *.webp *.pbm *.pgm *.ppm *.xbm *.xpm *.pnm)");
							this->utils.convertImage(QFileInfo(filepath).completeSuffix(),QFileInfo(filepath).absolutePath(),QFileInfo(filepath).baseName());
						}
						break;
					case SAVEASJPGITEM:
						this->utils.convertImage("jpg");
						break;
					case SAVEASPNGITEM:
						this->utils.convertImage("png");
						break;
					case SAVEASPNMITEM:
						this->utils.convertImage("pnm");
						break;
					case QUITITEM:
						qApp->exit();
						break;
					case SCANITEM:
						this->scanner.scanImage(false);
						qDebug()<<this->scanner.deviceName<<this->scanner.resolution<<this->scanner.colourMode;
						break;
					case PREVIEWITEM:
						this->scanner.scanImage(true);
						qDebug()<<this->scanner.deviceName<<this->scanner.resolution<<this->scanner.colourMode;
						break;
				}
		});
}

void MainWindowClass::setDeviceMenu(void)
{
	QActionGroup			*actions;
	QAction				*act;
	bool					first=true;
	SANE_Status			status;
	const SANE_Device	**device_list;

	this->deviceMenu=this->menuBar.addMenu("&Devices");

	status=sane_get_devices (&device_list,SANE_FALSE);
	if(status!= SANE_STATUS_GOOD)
		{
			qDebug()<<"No scanners found ...";
			return;
		}

	actions=new QActionGroup(this->deviceMenu);

	for(int j=0;device_list[j];++j)
		{
			act=new QAction(device_list[j]->name,actions);
			act->setCheckable(true);
			if(first==true)
				{
					act->setChecked(true);
					this->scanner.deviceName=device_list[j]->name;
					first=false;
				}	
		  }

	this->deviceMenu->addActions(actions->actions());
	QObject::connect(actions,&QActionGroup::triggered,this,[this](QAction *action)
		{
			this->scanner.setDevice(action->text());
			this->scanner.getDefaultResolution();
		});
}

void MainWindowClass::setResoMenu(void)
{
	QActionGroup		*actions;
	QAction			*act;
	QStringList		resolist={"Default","75","150","300","600","1200"};
	bool				first=true;

	this->resoMenu=this->menuBar.addMenu("&Resolution");
	actions=new QActionGroup(this->resoMenu);

	for(int j=0;j<resolist.size();j++)
		{
			act=new QAction(resolist.at(j),actions);
			act->setData(resolist.at(j));
			act->setCheckable(true);
			if(first==true)
				{
					act->setChecked(true);
					first=false;
				}	
		}

	this->resoMenu->addActions(actions->actions());
	QObject::connect(actions,&QActionGroup::triggered,this,[this](QAction *action)
		{
			if(action->data().toString()=="Default")
				this->scanner.resolution=this->scanner.defaultResolution;
			else
				this->scanner.resolution=action->data().toString();
			qDebug()<<"Set resolution to"<<action->data().toString();
		});
}

void MainWindowClass::setColourMenu(void)
{
	QActionGroup		*actions;
	QAction			*act;
	QStringList		collist={"Color","Gray"};
	QStringList		engcollist={"Colour","Grey"};
	bool				first=true;

	this->colourMenu=this->menuBar.addMenu("&Colour");
	actions=new QActionGroup(this->colourMenu);

	for(int j=0;j<collist.size();j++)
		{
			act=new QAction(engcollist.at(j),actions);
			act->setData(collist.at(j));
			act->setCheckable(true);
			if(first==true)
				{
					act->setChecked(true);
					first=false;
				}	
		}

	this->colourMenu->addActions(actions->actions());
	QObject::connect(actions,&QActionGroup::triggered,this,[this](QAction *action)
		{
			qDebug()<<"Set colour mode to"<<action->text();
			this->scanner.colourMode=action->data().toString();
		});
}

MainWindowClass::MainWindowClass()
{
	QVBoxLayout	*layout=new QVBoxLayout;
	QWidget		*widg=new QWidget;
	QSettings	settings("KDHedger",PACKAGE_NAME);
	QRect		r(50,50,800,600);

	r=settings.value("app/geometry",QVariant(r)).value<QRect>();

	this->setGeometry(r);
	this->label=new QLabel;
	this->label->setText("Loading Device Info ...");

	layout->addWidget(this->label);
	layout->setAlignment(Qt::AlignCenter);

	widg->setLayout(layout);
	this->setCentralWidget(widg);
//>>> yucky
	widg->show();
	this->show();
	this->repaint();
	widg->repaint();
	qApp->processEvents();
	qApp->processEvents();
	qApp->processEvents();
//<<<
	this->setWindowTitle("QtQuickScan");

	this->setFileMenu();
	this->setDeviceMenu();
	this->setResoMenu();
	this->setColourMenu();

	this->setMenuBar(&this->menuBar);
}

void MainWindowClass::closeEvent(QCloseEvent *event)
{
	event->accept();
}

void MainWindowClass::loadImage(QString filename)
{
	if(QFileInfo::exists(filename)==false)
		return;

	QImage image=scanner.getPreviewImage(filename);
	QImage image2=image.scaled(this->width()-50,this->height()-50,Qt::KeepAspectRatio);
	mwc->label->setPixmap(QPixmap::fromImage(image2));
}

void MainWindowClass::setSensitive(void)
{
	bool enable;

	enable=QFileInfo::exists(scanPath);
	for(int j=DIV1;j<DIV2;j++)
		this->fileMenu->actions().at(j)->setEnabled(enable);	
}
