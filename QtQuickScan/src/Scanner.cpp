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
   					//qDebug()<<"Default resolution"<<resol;
   					this->defaultResolution=QString("%1").arg(resol);
   					this->resolution=QString("%1").arg(resol);
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
							//if(desc->constraint_type==SANE_CONSTRAINT_RANGE)
							case SANE_TYPE_BOOL:
								qDebug()<<"bool TODO";
								break;
							case SANE_TYPE_INT:
								resol=*(SANE_Int *)val;
								//qDebug()<<"SANE_TYPE_INT constraint_type"<<opt->constraint_type;
								if(opt->constraint_type)
									{
										this->rangeIntFrom=SANE_UNFIX(opt->constraint.range->min);
										this->rangeIntTo=SANE_UNFIX(opt->constraint.range->max);
									}
								else
									this->fixedVal=resol;
								//qDebug()<<"SANE_TYPE_INT getOption"<<optname<<resol;
								
								break;
							case SANE_TYPE_FIXED:
								resol=(int)(SANE_UNFIX(*(SANE_Fixed *)val)+0.5);
								////qDebug()<<"SANE_TYPE_FIXED constraint_type"<<opt->constraint_type;
								//qDebug()<<"SANE_TYPE_FIXED getOption"<<optname<<resol;
								//qDebug()<<"SANE_TYPE_FIXED"<<SANE_UNFIX (opt->constraint.range->min)<<SANE_UNFIX (opt->constraint.range->max);
								if(opt->constraint_type)
									{
										this->rangeDoubleFrom=SANE_UNFIX(opt->constraint.range->min);
										this->rangeDoubleTo=SANE_UNFIX(opt->constraint.range->max);
									}
								else
									this->fixedVal=resol;

								break;
							case SANE_TYPE_STRING:
								//qDebug()<<"SANE_TYPE_STRING constraint_type"<<opt->constraint_type;
								//qDebug()<<"SANE_TYPE_STRING"<<optname<<QString("%1").arg((char*)val);
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
									//fprintf(stderr,"SANE_TYPE_FIXED val=%g\n",res);
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
	//fprintf(stderr,"UNKNOWN>>>>>optname=%s val=%s\n",optname,(const char*)value);
	return false;
}

void ScannerClass::setDevice(QString devname)
{
	SANE_Status	st;

	mwc->label1->setPixmap(QPixmap());
	mwc->label1->setText(QString("Querying '%1'  ...").arg(devname));
	qApp->processEvents();

	if(this->hdl!=NULL)
		{
//reset def reso
			setOption("resolution",qPrintable(this->defaultResolution));
			sane_close(this->hdl);
			this->hdl=NULL;
		}
	this->deviceName=devname;

	st=sane_open(qPrintable(this->deviceName),&this->hdl);
	if(st==SANE_STATUS_GOOD)
		{
			//qDebug()<<"Opened"<<devname;
			this->resolution="";
			this->defaultResolution="";
			this->getDefaultResolution();
			this->getOption("mode");
			this->getOption("resolution");
			this->getOption("test-picture");

			this->getOption("tl-x");
			this->paperWidth=this->rangeDoubleTo;
			this->getOption("br-y");
			this->paperHeight=this->rangeDoubleTo;

			system(qPrintable(QString("rm %1 %2 2>/dev/null").arg(previewPath).arg(scanPath)));
			mwc->cropMenu->actions().at(CLEARSELITEM)->activate(QAction::Trigger);
			mwc->label1->setText("");
			mwc->resoMenu->actions().at(0)->setChecked(true);
			mwc->setSensitive();
			mwc->setWindowTitle(QString("QtQuickScan - %1").arg(devname));
			mwc->setInfoBar();
			return;
		}
	else
		qDebug()<<"Failed to open"<<devname;
}

void ScannerClass::scanImage(bool preview)
{
	QPushButton					*cancelbtn=NULL;
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
	const char					*magic;
	char							*formatstring;
	int							bpp=1;
	int							wid;
	int							hite;
	int							finalsize;

	mwc->label1->setPixmap(QPixmap());
	if(preview==true)
		mwc->label1->setText("Acquiring Preview ...");
	else
		mwc->label1->setText("Preparing Scan ...");

	qApp->processEvents();

//timeval start, end;
//gettimeofday(&start, NULL);

// Typical option names: "resolution","br-x","br-y","tl-x","tl-y","mode"
//for built in test scanners
	this->setOption("test-picture","Color pattern");

	this->setOption("mode",qPrintable(this->colourMode));

	if(preview==true)
		{
			this->setOption("resolution",qPrintable(this->defaultResolution));
		}
	else
		{
			if(this->resolution.isEmpty())
				{
					this->getDefaultResolution();
					this->setOption("resolution",qPrintable(this->defaultResolution));
				}
			else
				this->setOption("resolution",qPrintable(this->resolution));
		}

	// Start the scan
	status=sane_start(this->hdl);
	check(status,"sane_start");

	status=sane_get_parameters(this->hdl,&params);
	check(status,"sane_get_parameters");

	// Determine PNM header
	magic=(params.format==SANE_FRAME_GRAY) ? "P5" : "P6";
	asprintf(&formatstring,"%s\n%d %d\n255\n",magic,params.pixels_per_line,params.lines);

	if(magic[1]=='6')
		bpp=3;
	wid=params.pixels_per_line*bpp;
	hite=params.lines;

	finalsize=wid*hite-1;
	finalsize+=strlen(formatstring);
	if(preview==true)
		out=fopen(qPrintable(previewPath),"w");
	else
		out=fopen(qPrintable(scanPath),"w");
   
	fseek(out,finalsize,SEEK_SET);
	fputc('\0',out);
	fclose(out);

	if(preview==true)
		out=fopen(qPrintable(previewPath),"r+");
	else
		out=fopen(qPrintable(scanPath),"r+");
	fprintf(out,"%s\n%d %d\n255\n",magic,params.pixels_per_line,params.lines);

// Read loop
	len=0;
	buf[0]=0;

	sane_read(this->hdl,buf,BUFFERSIZE,&len);
	if(len==0)
		return;

	hundred_percent=params.bytes_per_line*params.lines*((params.format==SANE_FRAME_RGB || params.format==SANE_FRAME_GRAY) ? 1:3);
	fwrite(buf,1,len,out);

	if(preview==false)
		{
			cancelbtn=new QPushButton("Cancel Scan");
			cancelbtn->setMaximumHeight(mwc->statusBar()->height()-4);
			mwc->statusBar()->addPermanentWidget(cancelbtn,0);
			QObject::connect(cancelbtn,&QPushButton::clicked,[this,out]()
				{
					sane_cancel(this->hdl);
					return;
				});
		}

	while(true)
		{
			status=sane_read(this->hdl,buf,BUFFERSIZE,&len);
			if(status==SANE_STATUS_EOF || len==0) break;
			check(status,"sane_read");
			fwrite(buf,1,len,out);
			total_bytes+=(SANE_Word)len;
			progr=((total_bytes*100.0)/(double)hundred_percent);
			if(progr>100.0)
				progr=100.0;
			mwc->statusBar()->showMessage(QString("Progress: %1%").arg(progr,0,'f',2));
			if(preview==true)
				mwc->loadImage(previewPath);
			else
				mwc->loadImage(scanPath);
			qApp->processEvents();
		}

	fclose(out);

	//printf("\nSaved output.pnm (%d x %d)\n",params.pixels_per_line,params.lines);
	mwc->statusBar()->showMessage(QString("Progress: 100.00%"));

	if(preview==false)
		{
			mwc->loadImage(scanPath);
			mwc->setSensitive();
		}
	else
		{
			mwc->loadImage(previewPath);
		}

	if(cancelbtn!=NULL)
		{
			mwc->statusBar()->removeWidget(cancelbtn);
			delete cancelbtn;
			mwc->statusBar()->showMessage("");
		}

	mwc->setInfoBar();
//fprintf(stderr,"\n");
// gettimeofday(&end, NULL);
//timeval result;
//timersub(&end, &start, &result);
//    double elapsedTime = result.tv_sec + result.tv_usec / 1000000.0;
//qDebug()<<"Elapsed time: " << elapsedTime ;
}


