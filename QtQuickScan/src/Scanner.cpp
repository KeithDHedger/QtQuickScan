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
	SANE_Status	s;
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
							//case SANE_TYPE_BOOL://0 TODO
							//	
							//	break;
							case SANE_TYPE_INT://1
								{
									//fprintf(stderr,"SANE_TYPE_INT>>>>>desc->type=%i desc=%s\n",desc->type,desc->desc);
									SANE_Int res = atoi((const char*)value);
									s = sane_control_option(this->hdl, i, SANE_ACTION_SET_VALUE, &res, 0);
								}
								break;
							case SANE_TYPE_FIXED://2
								{
									//fprintf(stderr,"SANE_TYPE_FIXED>>>>>desc->type=%i desc=%s\n",desc->type,desc->desc);
									SANE_Word res=SANE_FIX(atoi((const char*)value));
									s = sane_control_option(this->hdl, i, SANE_ACTION_SET_VALUE, &res, 0);
								}
								break;
							case SANE_TYPE_STRING://3
								//fprintf(stderr,"SANE_TYPE_STRING>>>>>desc->type=%i desc=%s\n",desc->type,desc->desc);
								s = sane_control_option(this->hdl, i, SANE_ACTION_SET_VALUE, (void*)value, 0);
 								break;
  							default:
  								//fprintf(stderr,"default>>>>>desc->type=%i desc=%s\n",desc->type,desc->desc);
  								break;
						}

					check(s, optname);
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
		qDebug()<<"Opened "<<devname;
	else
		qDebug()<<"Failed to open"<<devname;
}

void ScannerClass::scanImage(void)
{
	SANE_Status					status;
	const SANE_Option_Descriptor	*desc;
	unsigned char				buf[BUFFERSIZE];
	const char					*device_name=qPrintable(this->deviceName);
	SANE_Int						len;
	int							total_bytes = 0;
	int							hundred_percent;
	SANE_Parameters				params;
	double						progr=0;

// Typical option names: "resolution", "br-x", "br-y", "tl-x", "tl-y", "mode"
//for built in test scanners
	const char *mode = "Color pattern";
	set_option("test-picture", mode, (strlen(mode)+1));

	set_option("mode",qPrintable(this->colourMode),(strlen(qPrintable(this->colourMode))+1));
	set_option("resolution",qPrintable(this->resolution),(strlen(qPrintable(this->resolution))+1));

	// Start the scan
	status = sane_start(this->hdl);
	check(status, "sane_start");

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
	len=0;
	buf[0]=0;

	sane_read(this->hdl, buf, BUFFERSIZE, &len);
	if(len==0)
		return;

	hundred_percent = params.bytes_per_line * params.lines * ((params.format == SANE_FRAME_RGB || params.format == SANE_FRAME_GRAY) ? 1:3);

	fwrite(buf, 1, len, out);
	while (true)
		{
			status = sane_read(this->hdl, buf, BUFFERSIZE, &len);
			if (status == SANE_STATUS_EOF || len == 0) break;
			check(status, "sane_read");
			fwrite(buf, 1, len, out);
	 		 total_bytes += (SANE_Word) len;
         	 progr = ((total_bytes * 100.) / (double) hundred_percent);
         	 if (progr > 100.)
	  		  	progr = 100.;
	   		 fprintf (stderr, "Progress: %3.1f%%\r", progr);
		}

	fclose(out);
	printf("Saved output.pnm (%d x %d)\n", params.pixels_per_line, params.lines);
}