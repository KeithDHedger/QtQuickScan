/*
 *
 * ©K. D. Hedger. Tue 31 Mar 12:21:54 BST 2026 keithdhedger@gmail.com

 * This file (ImageLabel.cpp) is part of QtQuickScan.

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

ImageLabelClass::~ImageLabelClass()
{
}

ImageLabelClass::ImageLabelClass()
{
	this->setMouseTracking(true);
	this->rubberBand=new QRubberBand(QRubberBand::Rectangle,this);
}

void ImageLabelClass::mousePressEvent(QMouseEvent *event)
{
 		if(event->modifiers()==Qt::ShiftModifier)
 			{
 				this->expand=true;
				this->resize=true;
				this->doWhat=MOVE;
				sp=event->pos()-this->rubberBand->geometry().topLeft();
				return;
			}
 
 		if(event->modifiers()==Qt::ControlModifier)
			{
				QRect	corners;
				QRect	sides;
				int		cw;
				int		ch;
				int		sw;

				this->expand=true;
				this->resize=true;

				corners=this->rubberBand->geometry();
				corners.setWidth(this->rubberBand->geometry().width()/4);
				corners.setHeight(this->rubberBand->geometry().height()/4);
				cw=corners.width();
				ch=corners.height();

				sides=this->rubberBand->geometry();
				sides.setWidth(this->rubberBand->geometry().width()/2);
				sides.setHeight(this->rubberBand->geometry().height()/2);
				sw=sides.width();

				this->sp=event->pos()-this->rubberBand->geometry().topLeft();
				this->hr=this->rubberBand->geometry();

				corners.moveTopLeft(this->rubberBand->geometry().topLeft());
				if(corners.contains(event->pos()))
					{
						this->doWhat=NW;
						return;
					}

				corners.moveTopLeft(this->rubberBand->geometry().topRight()-QPoint(cw,0));
				if(corners.contains(event->pos()))
					{
						this->doWhat=NE;
						return;
					}

				corners.moveTopLeft(this->rubberBand->geometry().bottomLeft()-QPoint(0,ch));
				if(corners.contains(event->pos()))
					{
						this->doWhat=SW;
						return;
					}

				corners.moveTopLeft(this->rubberBand->geometry().bottomRight()-QPoint(cw,ch));
				if(corners.contains(event->pos()))
					{
						this->doWhat=SE;
						return;
					}

				sides.moveTopLeft(this->rubberBand->geometry().topLeft()+QPoint(cw,0));
				if(sides.contains(event->pos()))
					{
						this->doWhat=N;
						return;
					}

				sides.moveTopLeft(this->rubberBand->geometry().bottomLeft()+QPoint(cw,-ch));
				if(sides.contains(event->pos()))
					{
						this->doWhat=S;
						return;
					}

				sides.moveTopLeft(this->rubberBand->geometry().topLeft()+QPoint(0,ch));
				if(sides.contains(event->pos()))
					{
						this->doWhat=W;
						return;
					}

				sides.moveTopLeft(this->rubberBand->geometry().topRight()+QPoint(-cw,ch));
				if(sides.contains(event->pos()))
					{
						this->doWhat=E;
						return;
					}
				this->expand=false;
				this->resize=false;
				return;
			}
 
        this->startPoint=event->pos();
        this->rubberBand->setGeometry(QRect(this->startPoint,QSize()));
        this->rubberBand->show();
        this->resize=true;
        this->expand=false;
}

void ImageLabelClass::mouseMoveEvent(QMouseEvent *event)
{
	QPoint	pt=event->pos();
	QRect	r=this->hr;

	if(this->expand==true && this->resize==true)
		{
//			if(pt.x()<0)
//				pt.setX(0);
//			if(pt.x()>this->geometry().width())
//				pt.setX(this->geometry().width());
//
//			if(pt.y()<0)
//				pt.setY(0);
//			if(pt.y()>this->geometry().height())
//				pt.setY(this->geometry().height());

			switch(this->doWhat)
				{
					case MOVE:
						{
							QRect r=this->rubberBand->geometry();

							r.moveTopLeft(pt-sp);
							this->rubberBand->setGeometry(r);
							return;
						}
						break;
					case N:
						r.setTop(pt.y()-sp.y());
						break;
					case S:
						r.setBottom((pt.y()-sp.y()+hr.height()));
						break;
					case W:
						r.setLeft(pt.x()-sp.x());
						break;
					case E:
						r.setRight(pt.x()-sp.x()+hr.width());
						break;
					case NW:
						r.setTopLeft(pt-sp);
						break;
					case NE:
						r.setTopRight(QPoint(pt.x()-sp.x()+hr.width(),pt.y()-sp.y()));
						break;
					case SW:
						r.setBottomLeft(QPoint(pt.x()-sp.x(),pt.y()-sp.y()+hr.height()));
						break;
					case SE:
						r.setBottomRight(QPoint(pt.x()-sp.x()+hr.width(),pt.y()-sp.y()+hr.height()));
						break;
				}

			this->rubberBand->setGeometry(r.normalized());
			return;
		}

	if(this->resize==true)
		{
			if(pt.x()<0)
				pt.setX(0);
			if(pt.x()>this->geometry().width())
				pt.setX(this->geometry().width());

			if(pt.y()<0)
				pt.setY(0);
			if(pt.y()>this->geometry().height())
				pt.setY(this->geometry().height());
		
			this->rubberBand->setGeometry(QRect(this->startPoint,pt).normalized());
		}
}

void ImageLabelClass::mouseReleaseEvent(QMouseEvent *event)
{
	this->resize=false;
	QRect	r=this->rubberBand->geometry();

	if(r.x()<0)
		r.setLeft(0);
	if(r.right()>this->geometry().right())
		r.setRight(this->geometry().right());
	if(r.top()<0)
		r.setTop(0);
	if(r.bottom()>this->geometry().bottom())
		r.setBottom(this->geometry().bottom());

	this->rubberBand->setGeometry(r);
	this->selectionRect=this->rubberBand->geometry();

	for(int j=0;j<mwc->cropMenu->actions().size();j++)
		mwc->cropMenu->actions().at(j)->setEnabled(true);
}