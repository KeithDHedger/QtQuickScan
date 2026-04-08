/*
 *
 * ©K. D. Hedger. Wed  8 Apr 16:12:47 BST 2026 keithdhedger@gmail.com

 * This file (Scanner.h) is part of QtQuickScan.

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

#ifndef _SCANNER_
#define _SCANNER_

class ScannerClass
{
	public:
		ScannerClass();
		~ScannerClass();

		QString		deviceName;
		QString		resolution="75";
		QString		defaultResolution="75";
		QString		colourMode="Color";
		QString		inEnglishMode="Colour";

		double		paperWidth;
		double		paperHeight;
		SANE_Handle	hdl=NULL;

		void			getDefaultResolution(void);
		void			setDevice(QString devname);
		void			scanImage(bool preview);

		bool			setOption(const char *optname,const void *value);
		void 		getOption(const char *optname);
		void			resetSize(void);
		void			setMargins(void);

		double		rangeDoubleTo;
		double		rangeDoubleFrom;
		int			fixedVal;
		double		doubleVal;

	private:
		int			rangeIntFrom;
		int			rangeIntTo;
		FILE			*ofp=stdout;

		void			check(SANE_Status status,const char *msg);
};

#endif
