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

#include <sys/stat.h>
#include <unistd.h>



int compress(FILE * f, int offset, int size,char* fname)
{
    uint32_t i;
    long curr_pos = ftell(f);
    fseek(f,offset,SEEK_SET);
    
    FILE *fi = fopen(fname,"r");
    fseek(fi,0,SEEK_SET);
    for(i = 0; i<size;i++)
    {
	char c=fgetc(fi);
	fputc(c,f);
    }
    fclose(fi);
    fseek(f,curr_pos,SEEK_SET);
    return 0;
}


int main()
{
    FILE *cfg = fopen("cfg.txt","rt");
    char header[10];
    unsigned char version;
    uint32_t filecount;
    
            
    FILE *fout = fopen("snd_my.pak","wb");

    fscanf(cfg,"Header: %s\n",header);
    printf("Header: %s\n",header);
    fscanf(cfg,"Version: %d\n",&version);
    printf("Version: %d\n",version);
    fscanf(cfg,"Filecount: %lu\n",&filecount);
    printf("Filecount: %lu\n", filecount);


    char paddind[248];
    uint32_t tt;
    for(tt = 0; tt<248; tt++) paddind[tt]=0;
    
    fwrite("SND",3,1,fout);
    version=1;
    fwrite(&version,sizeof(version),1,fout);
    fwrite(&filecount,sizeof(filecount),1,fout);
    fwrite(&paddind,sizeof(paddind),1,fout);
    
    uint32_t curr_off = filecount*12 +  256;
    uint32_t k;
    for(k = 0; k<filecount;k++)
    {
	uint32_t i;
	uint32_t type;
	uint32_t offset;
	uint32_t size;
	char fname[255];
	fscanf(cfg,"id = %u Type = %u File = %s\n",&i,&type,fname);
	printf("id = %u Type = %u File = %s\n",i,type,fname);
	if(type != 0)
	{
	    struct stat buf;
	    stat(fname, &buf);
	    long fsize = buf.st_size;
	    printf("filesize = %lu\n",fsize);
	    compress(fout,curr_off,fsize,fname);
	    fwrite(&type,sizeof(type),1,fout);
	    offset = curr_off;
	    fwrite(&offset,sizeof(offset),1,fout);
	    size = fsize;
	    fwrite(&size,sizeof(size),1,fout);
	    curr_off += fsize;
	}
	else
	{
	    uint32_t tmp = 0;
	    fwrite(&tmp,sizeof(tmp),1,fout);
	    fwrite(&tmp,sizeof(tmp),1,fout);
	    fwrite(&tmp,sizeof(tmp),1,fout);
	}
    }
    fclose(cfg);
    fclose(fout);

    return 0;
}