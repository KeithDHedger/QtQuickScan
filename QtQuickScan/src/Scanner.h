/*
 *
 * ©K. D. Hedger. Sun 22 Mar 15:30:39 GMT 2026 keithdhedger@gmail.com

 * This file (Scanner.h) is part of Qt6Scan.

 * Qt6Scan is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * Qt6Scan is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with Qt6Scan.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _SCANNER_
#define _SCANNER_
//
//#define SANE_STATUS_WARMING_UP 12
//#define OUTPUT_PNM      0
//#define OUTPUT_TIFF     1
//#define OUTPUT_PNG      2
//#define OUTPUT_JPEG     3
//#define STRIP_HEIGHT	256	/* # lines we increment image height */
//
//typedef struct
//{
//  uint8_t *data;
//  int width;    /*WARNING: this is in bytes, get pixel width from param*/
//  int height;
//  int x;
//  int y;
//}
//Image;
//
//static const char *prog_name="qt6scan";
//static int verbose=1;
//static int progress = 1;
//static int output_format = OUTPUT_PNM;
//static int resolution_optind = -1, resolution_value = 0;
//static const char *icc_profile = NULL;
//static SANE_Byte *buffer;
//static size_t buffer_size=1000000;

class ScannerClass
{
	public:
		ScannerClass();
		~ScannerClass();

		QString		deviceName;
		QString		resolution="75";
		QString		colourMode="Color";

		void			setDevice(QString devname);
		void			scanImage(void);

	private:
		SANE_Handle	hdl=NULL;
		FILE			*ofp=stdout;

		void			check(SANE_Status status, const char *msg);
		bool			set_option(const char *optname, const void *value, size_t size);
};

#endif
