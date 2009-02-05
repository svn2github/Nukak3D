/** 
 * @file nkDICOMDictionary.h
 * @brief DICOM Data Dictionary.
 * @details Clases for DICOM Data Dictionary.
 * @author Alexander Pinzón Fernandez
 * @version 0.1
 * @date 26/01/2009 03:51 p.m.
*/

#ifndef _NKDICOMDICTIONARY_H_
#define _NKDICOMDICTIONARY_H_

#include "vtkINRIA3DConfigure.h"
#include  "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include  "wx/wx.h"
#endif

#include "wx/config.h"
#include "wx/confbase.h"

#include "wx/list.h"
#include "wx/string.h"

/**
 * @brief A Data Element Tag (DICOM: Part 6).
 * @details Class for generate key for Data Dictionary.
*/
class nkDictionayKey{
public:
	//The Value Representation of a Data Element describes the data type
	static const wxString nkVR_AE_ApplicationEntity;		//! ApplicationEntity
	static const wxString nkVR_AS_AgeString;				//! AgeString
	static const wxString nkVR_AT_AttributeTag;				//! AttributeTag
	static const wxString nkVR_CS_CodeString;				//! CodeString
	static const wxString nkVR_DA_Date;						//! Date
	static const wxString nkVR_DS_DecimalString ;			//! DecimalString
	static const wxString nkVR_DT_DateTime;					//! DateTime
	static const wxString nkVR_FL_FloatingPointSingle;		//! FloatingPointSingle
	static const wxString nkVR_FD_FloatingPointDouble;		//! FloatingPointDouble
	static const wxString nkVR_IS_IntegerString;			//! IntegerString
	static const wxString nkVR_LO_LongString;				//! LongString
	static const wxString nkVR_LT_LongText;					//! LongText
	static const wxString nkVR_OB_OtherByteString;			//! OtherByteString
	static const wxString nkVR_OF_OtherFloatString;			//! OtherFloatString
	static const wxString nkVR_OW_OtherWordString;			//! OtherWordString
	static const wxString nkVR_PN_PersonName;				//! PersonName
	static const wxString nkVR_SH_ShortString;				//! ShortString
	static const wxString nkVR_SL_SignedLong;				//! SignedLong
	static const wxString nkVR_SQ_SequenceOfItems;			//! SequenceOfItems
	static const wxString nkVR_SS_SignedShort;				//! SignedShort
	static const wxString nkVR_ST_ShortText;				//! ShortText
	static const wxString nkVR_TM_Time;						//! Time
	static const wxString nkVR_UI_UniqueIdentifierUID;		//! UniqueIdentifierUID
	static const wxString nkVR_UL_UnsignedLong;				//! UnsignedLong
	static const wxString nkVR_UN_Unknow;					//! Unknow
	static const wxString nkVR_US_UnsignedShort;			//! UnsignedShort
	static const wxString nkVR_UT_UnlimitedText;			//! UnlimitedText
	/**
	 * @brief Class constructor.
	 * @param a_Tag A Dicom tag. Example: 0010,0010
	 * @param a_ValueRepresentation A Dicom VR: (DA, AE, ...)
	 * @param a_Name A Dicom Name of this tag.
	*/
	nkDictionayKey(wxString a_Tag,
			wxString a_ValueRepresentation,
			wxString a_Name);
	/**
	 * @brief Return the tag code.
	*/
	wxString getTag();
	/**
	 * @brief Return the tag name.
	*/
	wxString getName();
	/**
	 * @brief Return the VR of tag.
	*/
	wxString getValueRepresentation();
	
private:
	wxString Tag;					//! A Dicom tag
	wxString ValueRepresentation;	//! A Dicom VR
	wxString Name;					//! A Dicom Name of this tag
};

WX_DECLARE_LIST(nkDictionayKey, nkListDictionayKey);	//! A Dicom list of tag's
/**
 * @brief A Data Dictionary (DICOM: Part 6).
 * @details Class for generate Data Dictionary.
*/
class nkDICOMDictionary{
public:
	/**
	 * @brief Singleton pattern, get unique instance.
	*/
	static nkDICOMDictionary * getInstance(){
		if( _instance == 0 ){
			_instance = new nkDICOMDictionary();
		}
		return _instance;
	}
	/**
	 * @brief Delete the instance.
	*/
	static void Destroy(){
		if( _instance != 0 ){
			delete( _instance );
			_instance = 0;
		}
	}
	/**
	 * @brief Class constructor.
	*/
	nkDICOMDictionary();
	/**
	 * @brief Delete resources.
	*/
	virtual ~nkDICOMDictionary();

	/**
	 * @brief Get A Data Element Tag.
	 * @param wxString gggg,eeee, gggg the Group Number and eeee the Element Number.
	*/
	nkDictionayKey * getKey(wxString a_Tag);
private:
	static nkDICOMDictionary * _instance;	//! Unique instance of this class.
	nkListDictionayKey my_Dictionary;		//! List of A Data Element Tag.
};


#endif //_NKDICOMDICTIONARY_H_