/*
 *
 * ©K. D. Hedger. Tue 31 Mar 12:21:54 BST 2026 keithdhedger@gmail.com

 * This file (ImageLabel.h) is part of QtQuickScan.

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

#ifndef _IMAGELABEL_
#define _IMAGELABEL_

enum {N=0,NE,E,SE,S,SW,W,NW,MOVE};

class ImageLabelClass : public QLabel
{
	public:
		ImageLabelClass();
		~ImageLabelClass();

		QRect		selectionRect;
		QRubberBand	*rubberBand;
 
		bool			resize=false;
		bool			expand=false;
		int 			doWhat=-1;
		

	protected:
		void			mousePressEvent(QMouseEvent *event) override;
		void			mouseMoveEvent(QMouseEvent *event) override;
		void			mouseReleaseEvent(QMouseEvent *event) override;

	private:
		QPoint		startPoint;
		//int 			sx=-1;
		//int 			sy=-1;
		QPoint		sp;
		QRect		hr;
};

#endif
