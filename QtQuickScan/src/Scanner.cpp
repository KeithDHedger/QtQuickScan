/*
 *
 * ©K. D. Hedger. Sun 22 Mar 15:30:39 GMT 2026 keithdhedger@gmail.com

 * This file (Scanner.cpp) is part of Qt6Scan.

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

ScannerClass::~ScannerClass()
{
	if(this->hdl!=NULL)
		sane_close(this->hdl);
	sane_exit();
}

ScannerClass::ScannerClass()
{
	sane_init(NULL,NULL);
}

void ScannerClass::check(SANE_Status status,const char *msg)
{
	if(status!=SANE_STATUS_GOOD)
		{
			fprintf(stderr,"%s: %s\n",msg,sane_strstatus(status));
			if(this->hdl!=NULL)
				sane_close(this->hdl);
			sane_exit();
			exit(1);
		}
}

void ScannerClass::getDefaultResolution(void)
{
	const SANE_Option_Descriptor *opt;
	void							*val;
	int							resol=75;

	this->defaultResolution="75";//good guess
	for(int j=0;;++j)
		{
			opt=sane_get_option_descriptor(this->hdl,j);
			if(!opt)
				return;
			if(opt->name && strcmp(opt->name,"resolution")==0)
				{
					val=alloca(opt->size);
					if(!val)
						return;
					sane_control_option(this->hdl,j,SANE_ACTION_GET_VALUE,val,0);
					if(opt->type==SANE_TYPE_INT)
						resol=*(SANE_Int *)val;
					else
						resol=(int)(SANE_UNFIX(*(SANE_Fixed *)val)+0.5);
   					qDebug()<<"Default resolution"<<resol;
   					this->defaultResolution=QString("%1").arg(resol);
					return;
   				}
		}
}

void ScannerClass::getOption(const char *optname)
{
	SANE_Status					s;
	const SANE_Option_Descriptor	*opt;
	void							*val;
	int							resol=1;

/* iterate options to find one by name,e.g. "mode" */
	for(int i=0;;++i)
		{
			opt=sane_get_option_descriptor(this->hdl,i);
			if(!opt)
				return;
			if(opt->name && strcmp(opt->name,optname)==0)
				{
//printf("Found option '%s' (index %d)\n",opt->name,i);
//printf("Type: %d,Unit: %d,Cap: 0x%x\n",opt->type,opt->unit,opt->cap);
					val=alloca(opt->size);
					if(!val)
						return;
					sane_control_option(this->hdl,i,SANE_ACTION_GET_VALUE,val,0);
					switch(opt->type)
						{
							case SANE_TYPE_BOOL:
								qDebug()<<"bool TODO";
								break;
							case SANE_TYPE_INT:
								resol=*(SANE_Int *)val;
								//qDebug()<<"SANE_TYPE_INT getOption"<<optname<<resol;
								break;
							case SANE_TYPE_FIXED:
								resol=(int)(SANE_UNFIX(*(SANE_Fixed *)val)+0.5);
								//qDebug()<<"SANE_TYPE_FIXED getOption"<<optname<<resol;
								break;
							case SANE_TYPE_STRING:
								//qDebug()<<"string"<<optname<<QString("%1").arg((char*)val);
								break;
							case SANE_TYPE_BUTTON:
							case SANE_TYPE_GROUP:
								qDebug()<<"bg TODO"<<optname;
								break;
						}
				}
		}
}

bool ScannerClass::setOption(const char *optname,const void *value)
{
	SANE_Status	s;
// iterate options until name matches
	for (SANE_Int i=1; ; ++i)
		{
			const SANE_Option_Descriptor *opt=sane_get_option_descriptor(this->hdl,i);
			if(!opt) break;
			if(opt->name && strcmp(opt->name,optname)==0)
				{
					const SANE_Option_Descriptor *desc=sane_get_option_descriptor(this->hdl,i);
					//fprintf(stderr,">>>>>desc->type=%i desc=%s\n",desc->type,desc->desc);
					switch(desc->type)
						{
							//case SANE_TYPE_BOOL://0 TODO
							//	
							//	break;
							case SANE_TYPE_INT://1
								{
									//fprintf(stderr,"SANE_TYPE_INT>>>>>desc->type=%i desc=%s\n",desc->type,desc->desc);
									SANE_Int res=atoi((const char*)value);
									s=sane_control_option(this->hdl,i,SANE_ACTION_SET_VALUE,&res,0);
								}
								break;
							case SANE_TYPE_FIXED://2
								{
									//fprintf(stderr,"SANE_TYPE_FIXED>>>>>desc->type=%i desc=%s\n",desc->type,desc->desc);
									SANE_Word res=SANE_FIX(atoi((const char*)value));
									s=sane_control_option(this->hdl,i,SANE_ACTION_SET_VALUE,&res,0);
								}
								break;
							case SANE_TYPE_STRING://3
								//fprintf(stderr,"SANE_TYPE_STRING>>>>>desc->type=%i desc=%s\n",desc->type,desc->desc);
								s=sane_control_option(this->hdl,i,SANE_ACTION_SET_VALUE,(void*)value,0);
 								break;
  							default:
  								//fprintf(stderr,"default>>>>>desc->type=%i desc=%s\n",desc->type,desc->desc);
  								break;
						}

					check(s,optname);
					return true;
				}
		}
	fprintf(stderr,"UNKNOWN>>>>>optname=%s val=%s\n",optname,(const char*)value);
	return false;
}

void ScannerClass::setDevice(QString devname)
{
	SANE_Status	st;

	if(this->hdl!=NULL)
		sane_close(this->hdl);
	this->deviceName=devname;

	st=sane_open(qPrintable(this->deviceName),&this->hdl);
	if(st==SANE_STATUS_GOOD)
		{
			qDebug()<<"Opened"<<devname;
			this->getDefaultResolution();
			this->getOption("mode");
			this->getOption("resolution");
			this->getOption("test-picture");

			mwc->loadImage(scanPath);
		}
	else
		qDebug()<<"Failed to open"<<devname;
}

void ScannerClass::scanImage(bool preview)
{
	SANE_Status					status;
	const SANE_Option_Descriptor	*desc;
	unsigned char				buf[BUFFERSIZE];
	const char					*device_name=qPrintable(this->deviceName);
	SANE_Int						len;
	int							total_bytes=0;
	int							hundred_percent;
	SANE_Parameters				params;
	double						progr=0;
	FILE							*out;

	mwc->label->setText("");
	qApp->processEvents();

// Typical option names: "resolution","br-x","br-y","tl-x","tl-y","mode"
//for built in test scanners
	const char *mode="Color pattern";
	setOption("test-picture",mode);

	setOption("mode",qPrintable(this->colourMode));

	if(preview==true)
		{
			this->getDefaultResolution();
			//this->resolution=this->defaultResolution;
			setOption("resolution",qPrintable(this->defaultResolution));
		}
	else
		{
			if(this->resolution.isEmpty())
				{
					this->getDefaultResolution();
					setOption("resolution",qPrintable(this->defaultResolution));
				}
			else
				setOption("resolution",qPrintable(this->resolution));
		}
//	if(this->resolution.isEmpty()==true)
//		this->resolution=this->defaultResolution;
//	if(preview==false)
//		setOption("resolution",qPrintable(this->resolution));
//	else
//		setOption("resolution",qPrintable(this->defaultResolution));

	// Start the scan
	status=sane_start(this->hdl);
	check(status,"sane_start");

	status=sane_get_parameters(this->hdl,&params);
	check(status,"sane_get_parameters");

	// Determine PNM header
	const char *magic=(params.format==SANE_FRAME_GRAY) ? "P5" : "P6";

	if(preview==true)
		out=fopen(qPrintable(previewPath),"wb");
	else
		out=fopen(qPrintable(scanPath),"wb");

	if(!out)
		{
			perror("fopen");
			sane_close(this->hdl);
			sane_exit();
			return;
		}
	fprintf(out,"%s\n%d %d\n255\n",magic,params.pixels_per_line,params.lines);

// Read loop
	len=0;
	buf[0]=0;

	sane_read(this->hdl,buf,BUFFERSIZE,&len);
	if(len==0)
		return;

	hundred_percent=params.bytes_per_line*params.lines*((params.format==SANE_FRAME_RGB || params.format==SANE_FRAME_GRAY) ? 1:3);

	fwrite(buf,1,len,out);
	while (true)
		{
			status=sane_read(this->hdl,buf,BUFFERSIZE,&len);
			if(status==SANE_STATUS_EOF || len==0) break;
			check(status,"sane_read");
			fwrite(buf,1,len,out);
			total_bytes+=(SANE_Word) len;
			progr=((total_bytes*100.0)/(double)hundred_percent);
			if(progr>100.0)
				progr=100.0;
			fprintf (stderr,"Progress: %3.1f%%\r",progr);
			if(preview==true)
				mwc->loadImage(previewPath);
			else
				mwc->loadImage(scanPath);
			qApp->processEvents();
		}

	fclose(out);

	printf("Saved output.pnm (%d x %d)\n",params.pixels_per_line,params.lines);
	if(preview==false)
		{
			mwc->loadImage(scanPath);
			system(qPrintable(QString("cp %1 /tmp/output.pnm").arg(scanPath)));
		}
	else
		mwc->loadImage(previewPath);
}

QImage ScannerClass::getPreviewImage(QString filepath)
{
	QFile	file(filepath);
	int		magic=0;
	int		cols=0;
	int		lines=0;
	char		r,g,b;

	if(!file.open(QIODevice::ReadOnly))
		return(QImage());

	QString mstr=file.readLine().trimmed();
	QString sstr=file.readLine().trimmed();
	QString wstr=sstr.section(" ",0,0);
	QString hstr=sstr.section(" ",1,1);
	QString lstr=file.readLine().trimmed();

	cols=wstr.toInt();
	lines=hstr.toInt();
	if(mstr=="P6")
		{
			QImage image(cols,lines,QImage::Format_RGB888);
			image.fill("white");
			for(int y=0;y<lines;y++)
				{
					for(int x=0;x<cols;x++)
						{
							if(file.atEnd()==true)
								return(image);
							file.getChar(&r);
							file.getChar(&g);
							file.getChar(&b);
							image.setPixel(x,y,qRgb(r,g,b));
						}
				}
			return(image);
		}

	if(mstr=="P5")
		{
			QImage image(cols,lines,QImage::Format_Grayscale8);
			image.fill("white");
			for(int y=0;y<lines;y++)
				{
					for(int x=0;x<cols;x++)
						{
							if(file.atEnd()==true)
								return(image);
							file.getChar(&r);
							g=r;
							b=r;
							image.setPixel(x,y,qRgb(r,g,b));
						}
				}
			return(image);
		}

	file.close();
	return(QImage());
}

