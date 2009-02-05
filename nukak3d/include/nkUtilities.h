/** 
 * @file nkUtilities.h
 * @brief Utilities functions.
 * @details Functions and macros for general prupose.
 * @author Alexander Pinzón Fernandez
 * @version 0.2
 * @date 01/30/2009 12:49 p.m.
*/

#ifndef _NKUTILITIES_H_
#define _NKUTILITIES_H_

#include  "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include  "wx/wx.h"
#endif

#include "wx/config.h"
#include "wx/confbase.h"
#include "wx/dir.h"
#include "wx/file.h"
#include "wx/fileconf.h"
#include "wx/filefn.h"
#include "wx/filename.h"
#include "wx/stdpaths.h"
#include "wx/string.h"

#define _nkVersion_ wxString("4.6.0")

/**
 * @brief Class utility functions.
*/
class nkUtilities{
public:
	/**
	 * @brief Class constructor.
	*/
	nkUtilities();

	static enum {
		ID_OPEN_FILE = wxID_HIGHEST + 1500, /**< Open file. */
		ID_OTHER_FILE
	};
	/**
	 * @brief Function that returns the directory where the program nukak3d.
	 * return The path if it is, otherwise it returns an empty string.
	*/
	static wxString getNukak3DPath();
	/**
	 * @brief Function that returns the number version of nukak3d.
	*/
	static wxString getNukak3DVersion();
	/**
	 * @brief Function that returns the user data directory.
	 * return The path if it is, otherwise it returns an empty string.
	*/
	static wxString getNukak3DDataDir();
	/**
	 * @brief Function that searches for a parameter in a string that is separated by a token.
	 * return The parameter if it is, otherwise it returns an empty string.
	*/
	static wxString getStringFromCharToken(wxString a_string, wxChar a_token, int index = 0);
	/**
	 * @brief Count the number of occurrences of a character.
	 * return The number of occurrences if it is, otherwise it returns an 0.
	*/
	static int countCharFromString(wxString a_string, wxChar a_charToFind);
	/**
	 * @brief Write a text string in the registry of the operating system..
	 * return True if they could perform the operation, false otherwise.
	*/
	static bool writeConfigBase(const wxString& key, const wxString& value);
	/**
	 * @brief Write a number in the registry of the operating system..
	 * return True if they could perform the operation, false otherwise.
	*/
	static bool writeConfigBase(const wxString& key, long value);
	/**
	 * @brief Read a text string of the registry of the operating system..
	 * return True if they could perform the operation, false otherwise.
	*/
	static bool readConfigBase(const wxString& key, wxString* str, const wxString& defaultVal) ;
	/**
	 * @brief Read a number of the registry of the operating system..
	 * return True if they could perform the operation, false otherwise.
	*/
	static bool readConfigBase(const wxString& key, long* al, long defaultVal);
	/**
	 * @brief Find the data directory nukak3d.
	 * return True if it exists and is writable, false otherwise.
	*/
	static bool existNukak3DDataDir();
	/**
	 * @brief Find the file extension for executables depending on your operating system.
	 * return .exe if the operating system is windows, otherwise empty.
	*/
	static wxString getExecutableExtension();
	/**
	 * @brief Creates a directory on a given path.
	 * return True if they could perform the operation, false otherwise.
	*/
	static bool appendNukak3DDataDir(const wxString& dirName, int perm = 0777, int flags = 0);
	/**
	 * @brief Turn as a working directory.
	 * return True if they could perform the operation, false otherwise.
	*/
	static bool setWorkingDirectory(const wxString& dir);
	/**
	 * @brief Activate the working directory where you will find nukak3d.
	 * return True if they could perform the operation, false otherwise.
	*/
	static bool setWorkingDirectoryNukak3D();
	/**
	 * @brief Make a new directory.
	 * return True if they could perform the operation, false otherwise.
	*/
	static bool mkDir(const wxString& dir, int perm = 0777, int flags = 0);
	/**
	 * @brief Creates a directory on a given path.
	 * return True if they could perform the operation, false otherwise.
	*/
	static bool appendDir(const wxString& dirPath, const wxString& dirName, int perm = 0777, int flags = 0);
	/**
	 * @brief Copy file1 to file2 direction.
	 * return True if they could perform the operation, false otherwise.
	*/
	static bool copyFile(const wxString& file1, const wxString& file2, bool overwrite = true);
	/**
	 * @brief Find a directory.
	 * return True if directory exists, false otherwise.
	*/
	static bool existDir(const wxString& dir);
	/**
	 * @brief Find a directory, and verify permissions.
	 * return True if directory exists an is wrotable, false otherwise.
	*/
	static bool isDirWritable(const wxString& dir);
	/**
	 * @brief Checks whether the filename is valid in the current operating system.
	 * return True if is valid name, false otherwise.
	*/
	static bool isValidDirName(const wxString& dir);
	/**
	 * @brief Find a file.
	 * return True if file exists, false otherwise.
	*/
	static bool existFile(const wxString& a_file);

private:
	static wxString prv_strNukak3DPath;			//! Variable to store the default working directory
	static wxString prv_strNukak3DDataDir;		//! Variable to store the data directory
};

#endif //_NKUTILITIES_H_