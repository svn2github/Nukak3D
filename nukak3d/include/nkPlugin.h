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

#ifndef __NKPLUGIN__H__
#define __NKPLUGIN__H__

#include "nkEngine.h"
#include <string>

#if defined (WIN32) || defined (_WIN32_)
	#define WIN32_LEAN_AND_MEAN
	#include <windows.h>
	#define NKHANDLER HMODULE
#else
#ifdef __APPLE__
	#include <mach-o/dyld.h>
	#include <dlfcn.h>
	#include <stdarg.h>
	#define NKHANDLER void * 
#else //UNIX
	#include <dlfcn.h>
	#define NKHANDLER void * 
#endif //__APPLE__
#endif //WIN32

namespace nukak3d {

class nkKernel;

/// Representation of a plugin
class nkPlugin {
  public:
    /// Initialize and load plugin
    NKENGINE_API nkPlugin(const std::string &sFilename);
    /// Copy existing plugin instance
    NKENGINE_API nkPlugin(const nkPlugin &Other);
    /// Unload a plugin
    NKENGINE_API ~nkPlugin();
	NKENGINE_API bool isLoaded();

  //
  // Plugin implementation
  //
  public:
    /// Query the plugin for its expected engine version
    NKENGINE_API int getEngineVersion() const {
      return m_pfnGetEngineVersion();
    }

    /// Register the plugin to a kernel
    NKENGINE_API void registerPlugin(nkKernel &K) {
      m_pfnRegisterPlugin(K);
    }
    
  private:
    /// Too lazy to this now...
    nkPlugin &operator =(const nkPlugin &Other);

    /// Signature for the version query function
    typedef int  fnGetEngineVersion();
    /// Signature for the plugin's registration function
    typedef void fnRegisterPlugin(nkKernel &);
	
	NKHANDLER             m_hDLL;                ///<  DLL handle
	
    size_t             *m_pDLLRefCount;        ///< Number of references to the DLL
    fnGetEngineVersion *m_pfnGetEngineVersion; ///< Version query function
    fnRegisterPlugin   *m_pfnRegisterPlugin;   ///< Plugin registration function
};

} // namespace MyEngine

#endif // __NKPLUGIN__H__