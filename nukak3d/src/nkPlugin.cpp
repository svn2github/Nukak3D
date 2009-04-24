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
