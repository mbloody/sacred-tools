/** 
 * main.cpp 
 * This file is part of the Sacred Tools Project http://plg.org.ua/sacred
 * 
 * Sacred Tools
 * Copyright (C) 2009 Evteev Maxim 
 * 
 * This program is free software; you can redistribute it and/or modify 
 * it under the terms of the GNU General Public License as published by 
 * the Free Software Foundation; either version 2 of the License, or 
 * (at your option) any later version. 
 * 
 * This program is distributed in the hope that it will be useful, 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
 * GNU General Public License for more details. 
 * 
 * You should have received a copy of the GNU General Public License 
 * along with this program; if not, write to the Free Software 
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA. 
*/

#include <stdio.h>
#include <stdint.h>
#include <string.h>


int extract(FILE * f, int offset, int size,char* fname)
{
    long curr_pos = ftell(f);
    fseek(f,offset,SEEK_SET);
    
    FILE *fo = fopen(fname,"w");
    uint32_t i;
    for(i = 0; i<size;i++)
    {
	char c=fgetc(f);
	fputc(c,fo);
    }
    fclose(fo);
    fseek(f,curr_pos,SEEK_SET);
    return 0;
}


int main()
{

    char header[3];
    unsigned char version;
    FILE * f = fopen("sound.pak","r");

    fread(header,sizeof(header),1,f);
    fread(&version,sizeof(version),1,f);
    char tmp[5];
    
    uint32_t tt;
    for(tt = 0; tt<5; tt++) tmp[tt]=0;
    strncpy(tmp,header,3);
    printf("Header: %s\n",tmp);
    printf("Version: %d\n",version);
    
    uint32_t filecount;
    char padding[248];

    fread(&filecount,sizeof(filecount),1,f);
    fread(&padding,sizeof(padding),1,f);
    
    printf("Filecount: %lu\n", filecount);
    uint32_t i;
    for(i = 0; i< filecount; i++)
    {
	uint32_t type;
	uint32_t offset;
	uint32_t size;
	fread(&type,sizeof(type),1,f);
	fread(&offset,sizeof(offset),1,f);
	fread(&size,sizeof(size),1,f);
	char fname[255];
	if(type != 0)
	    sprintf(fname,"data/file%08d.%d",i,type);
	else
	    sprintf(fname,"(null)");
	printf("id = %u Type = %u File = %s\n",i,type,fname);
	if(size>0) extract(f,offset,size,fname);
    }
    fclose(f);
    
    return 0;
}