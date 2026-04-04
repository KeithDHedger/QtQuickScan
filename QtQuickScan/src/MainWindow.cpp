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

	//delete this->prefs;
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
							this->utils.convertImage(scanPath,QFileInfo(filepath).completeSuffix(),QFileInfo(filepath).absolutePath(),QFileInfo(filepath).baseName());
						}
						break;
					case SAVEASJPGITEM:
						this->utils.convertImage(scanPath,"jpg");
						break;
					case SAVEASPNGITEM:
						this->utils.convertImage(scanPath,"png");
						break;
					case SAVEASPNMITEM:
						this->utils.convertImage(scanPath,"pnm");
						break;
					case QUITITEM:
						qApp->exit();
						break;
					case SCANITEM:
						this->scanner.setOption("tl-x","0");
						this->scanner.setOption("tl-y","0");
						this->scanner.setOption("br-x",qPrintable(QString("%1").arg(this->scanner.paperWidth)));
						this->scanner.setOption("br-y",qPrintable(QString("%1").arg(this->scanner.paperHeight)));
						this->scanner.scanImage(false);
						break;
					case PREVIEWITEM:
						this->scanner.setOption("tl-x","0");
						this->scanner.setOption("tl-y","0");
						this->scanner.setOption("br-x",qPrintable(QString("%1").arg(this->scanner.paperWidth)));
						this->scanner.setOption("br-y",qPrintable(QString("%1").arg(this->scanner.paperHeight)));
						this->scanner.scanImage(true);
						break;
				}
		});
}

QMenu* MainWindowClass::setHelpMenu(QMenuBar *menubar)
{
	QActionGroup		*actions;
	QAction			*act;
	QMenu			*menu;

	menu=menubar->addMenu("&Help");
	actions=new QActionGroup(menu);
	actions->setExclusive(true);

	act=new QAction(QIcon::fromTheme("help-about"),"About",actions);
	act->setData(ABOUTITEM);

	act=new QAction(QIcon::fromTheme("help-about"),"About QT",actions);
	act->setData(ABOUTQTITEM);

	act=new QAction(QIcon::fromTheme("help-contents"),"Help",actions);
	act->setData(HELPITEM);

	act=new QAction(actions);
	act->setSeparator(true);

	menu->addActions(actions->actions());
	QObject::connect(actions,&QActionGroup::triggered,this,[this](QAction *action)
		{
			switch(action->data().toInt())
				{
					case ABOUTITEM:
						this->utils.doAbout();
						break;
					case ABOUTQTITEM:
						QMessageBox::aboutQt(nullptr);
						break;
					case HELPITEM:
						this->utils.showHTML();
						break;
				}
		});
	return(menu);
}

QMenu* MainWindowClass::setCropMenu(QMenuBar *menubar)
{
	QActionGroup		*actions;
	QAction			*act;
	QMenu			*menu;

	menu=menubar->addMenu("&Selection");
	actions=new QActionGroup(menu);
	actions->setExclusive(true);

	act=new QAction("Crop",actions);
	act->setData(CROPTORECTITEM);
	act->setEnabled(false);

	act=new QAction("Hide Selection",actions);
	act->setData(HIDESELITEM);
	act->setEnabled(false);

	act=new QAction("Show Selection",actions);
	act->setData(SHOWSELITEM);
	act->setEnabled(false);

	act=new QAction("Clear Selection",actions);
	act->setData(CLEARSELITEM);
	act->setEnabled(false);

	menu->addActions(actions->actions());
	QObject::connect(actions,&QActionGroup::triggered,this,[this,actions](QAction *action)
		{
			switch(action->data().toInt())
				{
					case CROPTORECTITEM:
						{
							double resd;
							double labeldim;
							double paperdim;
							double mult;

							this->scanner.setOption("tl-x","0");
							this->scanner.setOption("tl-y","0");
							this->scanner.setOption("br-x",qPrintable(QString("%1").arg(this->scanner.paperWidth)));
							this->scanner.setOption("br-y",qPrintable(QString("%1").arg(this->scanner.paperHeight)));

							if(this->label1->selectionRect.width()>0)
								{
									labeldim=(double)this->label1->geometry().height();
									paperdim=(double)this->scanner.paperHeight;
									mult=(double)this->label1->selectionRect.height()+this->label1->selectionRect.top();
									resd=paperdim/labeldim*mult;
									this->scanner.setOption("br-y",qPrintable(QString("%1").arg(resd)));

									mult=(double)this->label1->selectionRect.top();
									resd=paperdim/labeldim*mult;
									this->scanner.setOption("tl-y",qPrintable(QString("%1").arg(resd)));

									labeldim=(double)this->label1->geometry().width();
									paperdim=(double)this->scanner.paperWidth;
									mult=(double)this->label1->selectionRect.width()+this->label1->selectionRect.left();
									resd=paperdim/labeldim*mult;
									this->scanner.setOption("br-x",qPrintable(QString("%1").arg(resd)));

									mult=(double)this->label1->selectionRect.left();
									resd=paperdim/labeldim*mult;
									this->scanner.setOption("tl-x",qPrintable(QString("%1").arg(resd)));

									this->scanner.scanImage(false);
								}
						}
						break;
					case HIDESELITEM:
						this->label1->rubberBand->hide();
						break;
					case SHOWSELITEM:
						this->label1->rubberBand->show();
						break;
					case CLEARSELITEM:
						this->label1->resize=false;
						this->label1->rubberBand->hide(); 
						this->label1->rubberBand->setGeometry(QRect(0,0,0,0));
						this->label1->selectionRect=QRect(0,0,0,0);
						for(int j=0;j<this->cropMenu->actions().size();j++)
							this->cropMenu->actions().at(j)->setEnabled(false);
						break;
				}
		});
	return(menu);
}

void MainWindowClass::setDeviceMenu(void)
{
	QActionGroup			*actions;
	QAction				*act;
	bool					first=true;
	SANE_Status			status;
	const SANE_Device	**device_list;

//	bool					dbgdev=false;

	if(this->deviceMenu==NULL)
		{
			this->deviceMenu=this->menuBar.addMenu("&Devices");
//			dbgdev=true;
		}	
	status=sane_get_devices(&device_list,SANE_FALSE);
	if(status!= SANE_STATUS_GOOD)
		{
			qDebug()<<"No scanners found ...";
			return;
		}

	actions=new QActionGroup(this->deviceMenu);

	act=new QAction("Reload Devices",actions);
	act=new QAction(actions);
	act->setSeparator(true);

//	if(dbgdev==true)
//		{
//			act=new QAction("no device",actions);
//			act->setCheckable(true);
//		}

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
			if(action->text()=="Reload Devices")
				{
					SANE_Status			status;
					const SANE_Device	**device_list;

					status=sane_get_devices(&device_list,SANE_FALSE);
					this->deviceMenu->clear();
					this->setDeviceMenu();
					return;
				}
			this->scanner.setDevice(action->text());
			if(this->scanner.hdl!=NULL)
				this->scanner.getDefaultResolution();
			else
				{
					this->deviceMenu->clear();
					this->setDeviceMenu();
					this->scanner.setDevice(this->scanner.deviceName);

				}
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

this->prefs.setPrefsName(PACKAGE_NAME);


	this->label1=new ImageLabelClass;
	this->label1->setText("Loading Device Info ...");

	layout->addWidget(this->label1);
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

	this->cropMenu=setCropMenu(&this->menuBar);
	this->helpMenu=setHelpMenu(&this->menuBar);

	this->utils.aboutText="<b>QtQuickScan</b>\n\nVersion " PACKAGE_VERSION "\n\nSimple scanner frontend for QT6\n\n©K.D.Hedger 2026\n\n<a href=\"" GLOBALWEBSITE "\">Homepage</a>\n\n<a href=\"mailto:" MYEMAIL "\">Email Me</a>";
	this->utils.docPath=QString("%1/docs/help.html").arg(DATADIR);
	if(QFileInfo::exists(this->utils.docPath)==false)
		this->utils.docPath=QFileInfo("../resources/docs/help.html").canonicalFilePath();

	this->utils.pathToIcon=QString("%1/pixmaps/QtQuickScan.png").arg(DATADIR);
	if(QFileInfo::exists(this->utils.pathToIcon)==false)
		this->utils.pathToIcon=QFileInfo("../resources/pixmaps/QtQuickScan.png").canonicalFilePath();

	this->utils.appName=PACKAGE_NAME;
	this->prefs.setPrefsName(PACKAGE_NAME);

	this->setMenuBar(&this->menuBar);
	
	this->utils.lastDir=this->prefs.getFilePref("app/dir");
	this->utils.lastName=this->prefs.getFilePref("app/name");
	this->utils.lastSFX=this->prefs.getFilePref("app/sfx");

	if(QFileInfo::exists(this->utils.lastDir)==false)
		this->utils.lastDir="/tmp";
}

void MainWindowClass::closeEvent(QCloseEvent *event)
{
	event->accept();
}

void MainWindowClass::loadImage(QString filename)
{
	if(QFileInfo::exists(filename)==false)
		return;

	QImage image(filename,"pnm");
	//QImage image2=image.scaled(this->width()-50,this->height()-50,Qt::KeepAspectRatio);
	this->image2=image.scaled(this->width()-50,this->height()-50,Qt::KeepAspectRatio);
	///mwc->label1->setPixmap(QPixmap::fromImage(image2));
	mwc->label1->setPixmap(QPixmap::fromImage(this->image2));
}

void MainWindowClass::setSensitive(void)
{
	bool enable;

	enable=QFileInfo::exists(scanPath);
	for(int j=DIV1;j<DIV2;j++)
		this->fileMenu->actions().at(j)->setEnabled(enable);

	this->cropMenu->setEnabled(enable);
}

void MainWindowClass::resizeEvent(QResizeEvent *event)
{
	QWidget::resizeEvent(event);

	if(QFileInfo::exists(scanPath))
		this->loadImage(scanPath);
	else if(QFileInfo::exists(previewPath))
		this->loadImage(previewPath);
}

