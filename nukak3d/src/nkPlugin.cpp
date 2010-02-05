/**
 * ***** BEGIN GPL LICENSE BLOCK *****
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 3
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 * The Original Code is Copyright (C) 2007-2010 by Bioingenium Research Group.
 * Bogota - Colombia
 * All rights reserved.
 *
 * Author(s): Alexander Pinzón Fernández.
 *
 * ***** END GPL LICENSE BLOCK *****
 */

#include "nkPlugin.h"
#include <stdexcept>
#include <iostream>
#include <wx/dynlib.h>

using namespace std;
using namespace nukak3d;

/** Loads the specified plugin as a dynamic library and locates
    the plugin's exported functions
    
    @param  sFilename  Filename of the plugin to load
*/
nkPlugin::nkPlugin(const std::string &sFilename) :
  m_hDLL(0),
  m_pfnGetEngineVersion(0),
  m_pfnRegisterPlugin(0),
  m_pDLLRefCount(0) {
	m_pDLLRefCount = new size_t(0);
	m_hDLL = wxDynamicLibrary::RawLoad(sFilename.c_str());		
	if(m_hDLL)
	{
		try 
		{
			m_pfnGetEngineVersion = reinterpret_cast<fnGetEngineVersion *>(
				wxDynamicLibrary::RawGetSymbol(m_hDLL, "getEngineVersion")
			);

			m_pfnRegisterPlugin = reinterpret_cast<fnRegisterPlugin *>(			
				wxDynamicLibrary::RawGetSymbol(m_hDLL,"registerPlugin")
			);

			// If the functions aren't found, we're going to assume this is
			// a plain simple DLL and not one of our plugins
			if(!m_pfnGetEngineVersion || ! m_pfnRegisterPlugin)
			{
				wxDynamicLibrary::Unload(m_hDLL);
				m_hDLL = NULL;
				//throw runtime_error(string("Nukak3D: '") + sFilename + "' is not a valid plugin");
			}else 
				++*m_pDLLRefCount ;
		}catch(...) {
			wxDynamicLibrary::Unload(m_hDLL);
			m_hDLL = NULL;
			//throw;
		}
	}
} 

/** Creates a copy of a plugin that has already been loaded.
    Required to provide correct semantics for storing plugins in
    an STL map container.
    
    @param  Other  Other plugin instance to copy
*/
nkPlugin::nkPlugin(const nkPlugin &Other) :
  m_hDLL(Other.m_hDLL),
  m_pfnGetEngineVersion(Other.m_pfnGetEngineVersion),
  m_pfnRegisterPlugin(Other.m_pfnRegisterPlugin),
  m_pDLLRefCount(Other.m_pDLLRefCount) {
  /// Increase DLL reference counter
  ++*m_pDLLRefCount;
}

/** Destroys the plugin, unloading its library when no more references
    to it exist.
*/
nkPlugin::~nkPlugin() {
  // Only unload the DLL if there are no more references to it
  if(!--*m_pDLLRefCount) {
    delete m_pDLLRefCount;
	wxDynamicLibrary::Unload(m_hDLL);
    //::FreeLibrary(m_hDLL);
  }
}

bool nkPlugin::isLoaded(){
	if(m_hDLL) return true;
	return false;
}
