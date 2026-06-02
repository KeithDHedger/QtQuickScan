/*
 *
 * ©K. D. Hedger. Wed  8 Apr 16:13:05 BST 2026 keithdhedger@gmail.com

 * This file (Utilities.cpp) is part of QtQuickScan.

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

#include "globals.h"

UtilitiesClass::~UtilitiesClass()
{
}

UtilitiesClass::UtilitiesClass()
{
}

void UtilitiesClass::convertImage(QString inpath,QString type,QString name,QString dir)
{
	QString outPath;
	QString outdir=dir;
	QString outname=name;
	QString outsfx=type;

	if(type.isEmpty()==true)
		outsfx=this->lastSFX;
	if(dir.isEmpty()==true)
		outdir=this->lastDir;
	if(name.isEmpty()==true)
		outname=this->lastName;

	outPath=QString("%1/%2.%3").arg(outdir).arg(outname).arg(outsfx);
	this->lastDir=outdir;
	this->lastName=outname;
	this->lastSFX=outsfx;

	if(type=="pnm")
		{
			system(qPrintable(QString("cp %1 %2").arg(inpath).arg(outPath)));
			return;
		}

	QImageReader reader(inpath);
	if (!reader.canRead())
		{
			qCritical()<<"Cannot read input:"<<reader.errorString();
			return;
		}

	QImage image=reader.read();
	if (image.isNull())
		{
			qCritical()<<"Read failed:"<< reader.errorString();
			return;
		}

	QImageWriter writer(outPath,outsfx.toUtf8());
	writer.setQuality(100);
	if (!writer.write(image))
		{
			qCritical()<<"Write failed:"<< writer.errorString();
			return;
		}
}

void UtilitiesClass::showHTML(void)
{
	QDialog			dialog;
	QVBoxLayout		*layout;
	QTextBrowser		*textBrowser=new QTextBrowser(&dialog);

	textBrowser->setSource(QUrl(this->docPath));
	layout=new QVBoxLayout(&dialog);
    layout->addWidget(textBrowser);
 
	QDialogButtonBox *buttonBox=new QDialogButtonBox(QDialogButtonBox::Close,&dialog);
	QObject::connect(buttonBox,&QDialogButtonBox::rejected,&dialog,&QDialog::reject);
	layout->addWidget( buttonBox);
   
    dialog.setWindowTitle(QString("Help For %1").arg(this->appName));
    dialog.resize(640,480);
    dialog.exec();
}

void UtilitiesClass::doAbout(void)
{
	QMessageBox	msgBox;

	msgBox.setText(this->aboutText);
	msgBox.setIconPixmap(QPixmap(this->pathToIcon));
	msgBox.setWindowTitle(QString("About %1").arg(this->appName));
	msgBox.setTextFormat(Qt::MarkdownText);
	msgBox.setStandardButtons(QMessageBox::Help|QMessageBox::Close);
	int ret=msgBox.exec();
	switch(ret)
		{
			case QMessageBox::Close:
				break;
			case QMessageBox::Help:
				this->showHTML();
				break;
		}
}