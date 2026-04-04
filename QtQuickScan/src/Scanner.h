/*
 *
 * ©K. D. Hedger. Sun 22 Mar 15:30:39 GMT 2026 keithdhedger@gmail.com

 * This file (Scanner.h) is part of Qt6Scan.

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

#ifndef _SCANNER_
#define _SCANNER_

class ScannerClass
{
	public:
		ScannerClass();
		~ScannerClass();

		QString		deviceName;
		QString		resolution;
		QString		defaultResolution;
		QString		colourMode="Color";
		double		paperWidth;
		double		paperHeight;
		SANE_Handle	hdl=NULL;

		void			getDefaultResolution(void);
		void			setDevice(QString devname);
		void			scanImage(bool preview);

		bool			setOption(const char *optname,const void *value);
		void 		getOption(const char *optname);
	private:
		double		rangeDoubleFrom;
		double		rangeDoubleTo;
		int			rangeIntFrom;
		int			rangeIntTo;
		int			fixedVal;
		FILE			*ofp=stdout;

		void			check(SANE_Status status,const char *msg);
};

#endif
