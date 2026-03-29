/*
 *
 * ©K. D. Hedger. Sat 28 Mar 12:57:12 GMT 2026 keithdhedger@gmail.com

 * This file (Utilities.h) is part of QtQuickScan.

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

#ifndef _UTILITIES_
#define _UTILITIES_

class UtilitiesClass
{
	public:
		UtilitiesClass();
		~UtilitiesClass();

		QString	lastDir="/tmp";
		QString	lastName="output";
		QString	lastSFX="pnm";

		void		convertImage(QString type="",QString dir="",QString name="");
};

#endif
