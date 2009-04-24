///////////////////////////////////////////////////////////////////////////////
// Plugin architecture example                                               //
//                                                                           //
// This code serves as an example to the plugin architecture discussed in    //
// the article and can be freely used                                        //
///////////////////////////////////////////////////////////////////////////////
#ifndef __NKITKFILTERSERVER__H__
#define __NKITKFILTERSERVER__H__

#include "nkEngine.h"
#include "nkImageToImageFilter.h"
#include "nkMacro.h"

#include <vector>
#include <string>

#include <itkImageToImageFilter.h>

namespace nukak3d {
    
/// Manages graphics related stuff

class nkITKFilterServer {
  public:
    /// Engine graphics driver
    class nkITKFilter {
      public:
        virtual ~nkITKFilter() {}
        virtual const std::string &getName() const = 0;
		virtual const std::string &getDescription() const = 0;
		MAKRO_DECLARE_createitkFilter()
    };
	

    /// Destructor
    NKENGINE_API ~nkITKFilterServer() {
      for(nkITKFilterVector::reverse_iterator It = m_nkITKFilterVector.rbegin();
          It != m_nkITKFilterVector.rend();
          ++It)
        delete *It;
    }

    /// Allows plugins to add new graphics drivers
    NKENGINE_API void addnkITKFilter(std::auto_ptr<nkITKFilter> GD) {
      m_nkITKFilterVector.push_back(GD.release());
    }

    /// Get the total number of registered graphics drivers
    NKENGINE_API size_t getnkITKFilterCount() const {
      return m_nkITKFilterVector.size();
    }
    /// Access a driver by its index
    NKENGINE_API nkITKFilter &getkITKFilter(size_t Index) {
      return *m_nkITKFilterVector.at(Index);
    }

	NKENGINE_API nkITKFilter &getkITKFilter(std::string FilterName) {
		size_t tam = m_nkITKFilterVector.size();
		size_t it;
		for(it = 0; it<tam; it++){
			std::string cad = (*m_nkITKFilterVector.at(it)).getName();
			if(cad.compare(FilterName) == 0){
				return *m_nkITKFilterVector.at(it);
			}
		}
		nukak3d::nkITKFilterServer::nkITKFilter * mNULL;
      return *mNULL;
    }

	NKENGINE_API bool existITKFilter(std::string FilterName)
	{
		size_t tam = m_nkITKFilterVector.size();
		size_t it;
		for(it = 0; it<tam; it++){
			std::string cad = (*m_nkITKFilterVector.at(it)).getName();
			if(cad.compare(FilterName) == 0){
				return true;
			}
		}
		return false;
	}



  private:
    /// A vector of graphics drivers
    typedef std::vector<nkITKFilter *> nkITKFilterVector;

    nkITKFilterVector m_nkITKFilterVector; ///< All available graphics drivers
};

} // namespace nukak3d

#endif // __NKITKFILTERSERVER__H__