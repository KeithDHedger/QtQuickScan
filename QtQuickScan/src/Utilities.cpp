/*
 *
 * ©K. D. Hedger. Sat 28 Mar 12:57:12 GMT 2026 keithdhedger@gmail.com

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

void UtilitiesClass::convertImage(QString type)
{
	const QString inPath=scanPath;
	const QString outPath=QString("/tmp/output.%1").arg(type);
	const QString outFormat=type; // or "png", "webp", etc.

	if(type=="pnm")
		{
			system(qPrintable(QString("cp %1 %2").arg(scanPath).arg(outPath)));
			qDebug()<<"Saved"<<outPath;
			return;
		}

	QImageReader reader(inPath);
	if (!reader.canRead())
		{
			qCritical()<<"Cannot read input:"<<reader.errorString();
			return;
		}

	// Optionally force a format or scale: reader.setScaledSize({800,600});
	QImage image=reader.read();
	if (image.isNull())
		{
			qCritical()<<"Read failed:"<< reader.errorString();
			return;
		}

	QImageWriter writer(outPath,outFormat.toUtf8());
	// Set quality 0-100 (only used by some formats like JPEG, WebP)
	writer.setQuality(100);
	// Optionally set compression (PNG): writer.setCompression(9);

	if (!writer.write(image))
		{
			qCritical()<<"Write failed:"<< writer.errorString();
			return;
		}

	qDebug()<<"Saved"<<outPath;
	return;
}
