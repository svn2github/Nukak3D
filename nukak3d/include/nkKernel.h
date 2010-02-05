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

#ifndef __NKKERNEL__H__
#define __NKKERNEL__H__

#include "nkEngine.h"
#include "nkITKFilterServer.h"
#include "nkPlugin.h"

#include <string>
#include <map>

namespace nukak3d {

/// The engine's system core
class nkKernel {
  public:
	/*NKENGINE_EXPORTS nkVTKInteractorServer &getnkVTKInteractorServer()
	{ 
		return m_RenWinInteractorServer; 
	}
	NKENGINE_EXPORTS nkVTKRenderWindowServer &getnkVTKRenderWindowServer() 
	{ 
		return m_RenderWindowServer; 
	}*/
	NKENGINE_API nkITKFilterServer &getnkITKFilterServer()
    { 
		return m_itkFilterServer; 
	}
	
	/*NKENGINE_EXPORTS nkVTKFilterServer &getnkVTKFilterServer()
	{ 
		return m_vtkFilterServer; 
	}*/
        
    /// Loads a plugin
    NKENGINE_API bool loadPlugin(const std::string &sFilename) {
		if(m_LoadedPlugins.find(sFilename) == m_LoadedPlugins.end())
		{
			nkPlugin aPlugin(sFilename);
			if(aPlugin.isLoaded())
			{
				m_LoadedPlugins.insert(PluginMap::value_type(
				sFilename,
				aPlugin
				)).first->second.registerPlugin(*this);
				return true;
			}
		}
		return false;
    }
    
  private:
    /// Map of plugins by their associated file names
    typedef std::map<std::string, nkPlugin> PluginMap;
    
    PluginMap       m_LoadedPlugins;  ///< All plugins currently loaded
    nkITKFilterServer m_itkFilterServer; 
    //nkVTKFilterServer m_vtkFilterServer; 
	//nkVTKRenderWindowServer m_RenderWindowServer;
	//nkVTKInteractorServer m_RenWinInteractorServer;
};

} // namespace nukak3d

#endif // NKKERNEL