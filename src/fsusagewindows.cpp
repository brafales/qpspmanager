/***************************************************************************
 *   Copyright (C) 2007 by Bernat Ràfales,,,   *
 *   bernat@rafales-mulet.com   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "fsusage.h"
#include <windows.h>

typedef BOOL (WINAPI *PFNGETDISKFREESPACEEX)(LPCTSTR, PULARGE_INTEGER, PULARGE_INTEGER, PULARGE_INTEGER);

int FSUsage::getFSUsage(QString fsRoot)
{
    return -1;
}


quint64 FSUsage::getFSSize(QString fsRoot)
{
    return 0;
}


quint64 FSUsage::getFSUsed(QString fsRoot)
{
    return 0;
}


quint64 FSUsage::getFSFree(QString fsRoot)
{
	HINSTANCE hModule = ::LoadLibraryA("KERNEL32.DLL");
	if(hModule)
	{
	  PFNGETDISKFREESPACEEX pDiskFreeSpaceEx = NULL;
	
	  // Determine function to use
	  pDiskFreeSpaceEx = reinterpret_cast<PFNGETDISKFREESPACEEX>(::GetProcAddress(hModule,
	                                                                              "GetDiskFreeSpaceExA"));
	  if(!pDiskFreeSpaceEx)
	  {
	    DWORD dwSectorsPerCluster = 0;
	    DWORD dwBytesPerSector    = 0; 
	    DWORD dwFreeClusters      = 0; 
	    DWORD dwClusters          = 0; 
	
	    if(::GetDiskFreeSpaceA(fsRoot.toUtf8(),
	                          &dwSectorsPerCluster,
	                          &dwBytesPerSector,
	                          &dwFreeClusters,
	                          &dwClusters) == true)
	      {
	      	// Release library
	  			::FreeLibrary(hModule);
	      	return dwFreeClusters * dwBytesPerSector * dwSectorsPerCluster;
	      }
	  }
	  else
	  {
	    ULARGE_INTEGER uliFreeBytesAvailableToCaller;
	    ULARGE_INTEGER uliTotalNumberOfBytes;
	    ULARGE_INTEGER uliTotalNumberOfFreeBytes;
	
	    if(::GetDiskFreeSpaceExA(fsRoot.toUtf8(),
	                            &uliFreeBytesAvailableToCaller,
	                            &uliTotalNumberOfBytes,
	                            &uliTotalNumberOfFreeBytes) == true)
	    {
	    	// Release library
	  		::FreeLibrary(hModule);
	    	return static_cast<int>(static_cast<__int64>(uliFreeBytesAvailableToCaller.QuadPart / (1024 * 1024)));
	    }
	  }
	}
	return 0;
}
