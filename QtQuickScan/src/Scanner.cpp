/*
 *
 * ©K. D. Hedger. Sun 22 Mar 15:30:39 GMT 2026 keithdhedger@gmail.com

 * This file (Scanner.cpp) is part of Qt6Scan.

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

#include "globals.h"

ScannerClass::~ScannerClass()
{
	if(this->hdl!=NULL)
		sane_close(this->hdl);
	sane_exit();
}

ScannerClass::ScannerClass()
{
	sane_init(NULL, NULL);
}

void ScannerClass::check(SANE_Status status, const char *msg)
{
	if (status != SANE_STATUS_GOOD)
		{
			fprintf(stderr, "%s: %s\n", msg, sane_strstatus(status));
			if(this->hdl!=NULL)
				sane_close(this->hdl);
			sane_exit();
			exit(1);
		}
}

bool ScannerClass::set_option(const char *optname, const void *value, size_t size)
{
	SANE_Status s;
		// iterate options until name matches
	for (SANE_Int i = 1; ; ++i)
		{
			const SANE_Option_Descriptor *opt = sane_get_option_descriptor(this->hdl, i);
			if (!opt) break;
			if (opt->name && strcmp(opt->name, optname) == 0)
				{
					const SANE_Option_Descriptor *desc=sane_get_option_descriptor(this->hdl, i);
					//fprintf(stderr,">>>>>desc->type=%i desc=%s\n",desc->type,desc->desc);
					switch(desc->type)
						{
							//case SANE_TYPE_BOOL://0
							//	
							//	break;
							case SANE_TYPE_INT://1
								{
									fprintf(stderr,"SANE_TYPE_INT>>>>>desc->type=%i desc=%s\n",desc->type,desc->desc);
									SANE_Int res = atoi((const char*)value);

									//SANE_Word res=SANE_UNFIX(atoi((const char*)value));
									//s = sane_control_option(this->hdl, i, SANE_ACTION_SET_VALUE,75, 0);
									s = sane_control_option(this->hdl, i, SANE_ACTION_SET_VALUE, &res, 0);
								}
								break;
							case SANE_TYPE_FIXED://2
								{
									fprintf(stderr,"SANE_TYPE_FIXED>>>>>desc->type=%i desc=%s\n",desc->type,desc->desc);
									SANE_Word res=SANE_FIX(atoi((const char*)value));
									s = sane_control_option(this->hdl, i, SANE_ACTION_SET_VALUE, &res, 0);
								}
								break;
							case SANE_TYPE_STRING://3
								fprintf(stderr,"SANE_TYPE_STRING>>>>>desc->type=%i desc=%s\n",desc->type,desc->desc);
								s = sane_control_option(this->hdl, i, SANE_ACTION_SET_VALUE, (void*)value, 0);
 								break;
  							default:
  								fprintf(stderr,"default>>>>>desc->type=%i desc=%s\n",desc->type,desc->desc);
  								break;
						}

					check(s, optname);
					return true;
				}
		}
	fprintf(stderr,"UNKNOWN>>>>>optname=%s val=%s\n",optname,(const char*)value);
	return false;
//	for (SANE_Int i = 1; ; ++i)
//		{
//			const SANE_Option_Descriptor *opt = sane_get_option_descriptor(this->hdl, i);
//			if (!opt) break;
//			if (opt->name && strcmp(opt->name, optname) == 0)
//				{
//					SANE_Status s = sane_control_option(this->hdl, i, SANE_ACTION_SET_VALUE, (void*)value, nullptr);
//					this->check(s, optname);
//					return true;
//				}
//		}
//	return false;
}

void ScannerClass::setDevice(QString devname)
{
	SANE_Status	st;

	if(this->hdl!=NULL)
		sane_close(this->hdl);
	this->deviceName=devname;

	st=sane_open(qPrintable(this->deviceName),&this->hdl);
	if(st==SANE_STATUS_GOOD)
		qDebug()<<"Opened "<<devname;
	else
		qDebug()<<"Failed to open"<<devname;
}

void ScannerClass::scanImage(void)
{
	SANE_Status status;
	status = sane_init(nullptr, nullptr);
	this->check(status, "sane_init");
	const SANE_Option_Descriptor *desc;
	const char *device_name=qPrintable(this->deviceName);
	//status = sane_open(device_name,&this->hdl);
	//this->check(status, "sane_open");

	for (int i=0;; ++i)
		{
			desc = sane_get_option_descriptor(this->hdl, i);
			if (!desc) break;

			if (desc->type == SANE_TYPE_BOOL)
				{
					fprintf(stderr,"%s\n	",desc->name);
					fprintf(stderr,"[=(");
					if (desc->cap & SANE_CAP_AUTOMATIC)
						fprintf(stderr,"auto|");
					fprintf(stderr,"yes|no)]\n");
				}
			else
				{
					if (desc->constraint_type == SANE_CONSTRAINT_RANGE)
						{
							fprintf(stderr,"%s\n	",desc->name);
							switch(desc->type)
								{
								case SANE_TYPE_INT:
									fprintf(stderr,"%i ... %i\n",desc->constraint.range->min,desc->constraint.range->max);
									break;
								case SANE_TYPE_FIXED:
									fprintf(stderr,"%g ... %g\n",SANE_UNFIX (desc->constraint.range->min),SANE_UNFIX (desc->constraint.range->max));
									break;
								default:
									fprintf(stderr,"type=%i\n",desc->type);
									break;
								}
						}
					if (desc->constraint_type == SANE_CONSTRAINT_WORD_LIST)
						{
							fprintf(stderr,"%s\n	",desc->name);
							for(int j=1; j<=desc->constraint.word_list[0]; j++)
								fprintf(stderr,"%i|",desc->constraint.word_list[j]);
							fprintf(stderr,"\n");
						}
					if (desc->constraint_type == SANE_CONSTRAINT_STRING_LIST)
						{
							fprintf(stderr,"%s\n	",desc->name);
							int cnt=0;
							while(desc->constraint.string_list[cnt]!=NULL)
								fprintf(stderr,"%s|",desc->constraint.string_list[cnt++]);
							fprintf(stderr,"\n");
						}

				}

		}

	// Typical option names: "resolution", "br-x", "br-y", "tl-x", "tl-y", "mode"
	// Set mode to "Color" (some devices use "Color", "Gray", "Lineart")
 //for test scanners
   const char *mode = "Color pattern";
   set_option("test-picture", mode, (strlen(mode)+1));

//	const char *cmode = "Color";
	set_option("mode",qPrintable(this->colourMode),(strlen(qPrintable(this->colourMode))+1));
	set_option("resolution",qPrintable(this->resolution),(strlen(qPrintable(this->resolution))+1));

	// Start the scan
	status = sane_start(this->hdl);
	check(status, "sane_start");

	SANE_Parameters params;
	status = sane_get_parameters(this->hdl, &params);
	check(status, "sane_get_parameters");

	// Determine PNM header
	const char *magic = (params.format == SANE_FRAME_GRAY) ? "P5" : "P6";
	FILE *out = fopen("/tmp/output.pnm", "wb");
	if (!out)
		{
			perror("fopen");
			sane_close(this->hdl);
			sane_exit();
			return;
		}
	fprintf(out, "%s\n%d %d\n255\n", magic, params.pixels_per_line, params.lines);







	// Read loop
	const size_t BUFSZ = 32*1024;
	unsigned char buf[BUFSZ];
	SANE_Int len;

len=0;
int cnt=0;
buf[0]=0;
	do
	{
		cnt++;
		if(cnt>10)
			break;
		sane_read(this->hdl, buf, BUFSZ, &len);
		if(len==0)
			{
				return;
				this->setDevice(this->deviceName);
				sleep(1);
				fprintf(stderr,"loops=%i handle=%p\n",cnt,this->hdl);
			}
	} while(len==0);

int total_bytes = 0;
int  hundred_percent;
hundred_percent = params.bytes_per_line * params.lines * ((params.format == SANE_FRAME_RGB || params.format == SANE_FRAME_GRAY) ? 1:3);
double progr=0;

if(len!=0)
	{
		fwrite(buf, 1, len, out);
		while (true)
		{
			status = sane_read(this->hdl, buf, BUFSZ, &len);
			if (status == SANE_STATUS_EOF || len == 0) break;
			check(status, "sane_read");
			fwrite(buf, 1, len, out);
	 		 total_bytes += (SANE_Word) len;
         	 progr = ((total_bytes * 100.) / (double) hundred_percent);
         	 if (progr > 100.)
	  		  	progr = 100.;
	   		 fprintf (stderr, "Progress: %3.1f%%\r", progr);
		}
	}





	//while (true)
//		{
//			status = sane_read(this->hdl, buf, BUFSZ, &len);
//			if (status == SANE_STATUS_EOF || len == 0) break;
//			check(status, "sane_read");
//			fwrite(buf, 1, len, out);
//		}
//
	fclose(out);
//	sane_close(this->hdl);
//	sane_exit();
	printf("Saved output.pnm (%d x %d)\n", params.pixels_per_line, params.lines);
}