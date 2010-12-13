/**
 * Utility to get and set FAT attributes directly through the 
 * kernel FAT ioctl() calls.
 *
 * Copyright (C) 2010 by Josh Henderson <digitalpeer@digitalpeer.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <linux/msdos_fs.h>

#ifdef DEFINE_IOCTLS

#ifndef FAT_IOCTL_GET_ATTRIBUTES
#define FAT_IOCTL_GET_ATTRIBUTES	_IOR('r', 0x10, __u32)
#endif

#ifndef FAT_IOCTL_SET_ATTRIBUTES
#define FAT_IOCTL_SET_ATTRIBUTES	_IOW('r', 0x11, __u32)
#endif

#endif

static void hex_print(const char* data, int length)
{
   int ptr = 0;
   for(;ptr < length;ptr++)
   {
      printf("0x%02x ",(unsigned char)*(data+ptr));
   }
}

int main(int argc, char** argv)
{
   unsigned int cmd = 0;
   unsigned int attr = ATTR_NONE;
   int ret = 0;
   int fd = 0;

   if (argc < 4)
   {
      fprintf(stderr,"usage: %s [get|set] filename [BASE_10_VALUE]\n",argv[0]);
      return -1;
   }
	
   fd = open(argv[2], 0);
   if (fd < 0)
   {
      fprintf(stderr,"can't open file: %s\n", argv[2]);
      return -1;
   }
   else
   {
      printf("opened file: %s\n", argv[2]);
   }

   if (strcasecmp(argv[1],"set") == 0)
   {
      attr = atoi(argv[3]);
      cmd = FAT_IOCTL_SET_ATTRIBUTES;
   }
   else if (strcasecmp(argv[1],"get") == 0)
   {
      cmd = FAT_IOCTL_GET_ATTRIBUTES;
   }
   else
   {
      fprintf(stderr,"unknown option %s\n",argv[1]);
      goto quit;
   }

   if ((ret = ioctl(fd,cmd,(unsigned long)&attr)) != 0)
   {
      fprintf(stderr,"ioctl: %s\n",strerror(errno));
   }

   if (ret > -1)
   {
      if (argv[1][0] == 's')
      {
	 printf("set file attributes to ");
      }
      else if (argv[1][0] == 'g')
      {
	 printf("file attributes are ");
      }
		
      hex_print((char*)&attr,1);

      printf("\n");
   }

 quit:

   close(fd);
	
   return ret;
}
