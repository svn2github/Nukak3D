/** 
 * @file nkUtilities.cpp
 * @brief Utilities functions.
 * @details Functions and macros for general prupose.
 * @author Alexander Pinzón Fernandez
 * @version 0.2
 * @date 01/30/2009 12:49 p.m.
*/

#include "nkUtilities.h"

wxString nkUtilities::prv_strNukak3DPath = (wxStandardPaths::Get().GetExecutablePath())
.Left(wxStandardPaths::Get().GetExecutablePath().Find("nukak3d"));

wxString nkUtilities::prv_strNukak3DDataDir = wxString("");

nkUtilities::nkUtilities(){
	//Nothing
}

wxString nkUtilities::getNukak3DPath(){
	if(prv_strNukak3DPath.Length()<1){
		prv_strNukak3DPath = (wxStandardPaths::Get().GetExecutablePath())
			.Left(wxStandardPaths::Get().GetExecutablePath().Find("nukak3d"));
	}
	return prv_strNukak3DPath;
}

wxString nkUtilities::getNukak3DVersion(){
	return _nkVersion_ ;
}

wxString nkUtilities::getNukak3DDataDir(){
	wxString a_dataDirConfigSystem = "";
	if(prv_strNukak3DDataDir.Length()>2){
		return prv_strNukak3DDataDir;
	}else{
		bool a_find = readConfigBase("Paths/DataDir", &a_dataDirConfigSystem, "");
		bool buscar = !a_find;
		if(a_find){
			if(mkDir(a_dataDirConfigSystem)){
				buscar = false;
			}else{
				buscar = true;
			}
		}
		if(buscar){
			a_dataDirConfigSystem = wxStandardPaths::Get().GetUserLocalDataDir() + wxFileName::GetPathSeparator() + wxString("data");
			if (!mkDir(a_dataDirConfigSystem)){
				a_dataDirConfigSystem = wxStandardPaths::Get().GetDocumentsDir() + wxFileName::GetPathSeparator() + 
						wxString("Nukak3D") +  wxFileName::GetPathSeparator() + 
						wxString("data");
				if (!mkDir(a_dataDirConfigSystem)){
					a_dataDirConfigSystem = wxStandardPaths::Get().GetTempDir() + wxFileName::GetPathSeparator() + 
						wxString("Nukak3D") +  wxFileName::GetPathSeparator() + 
						wxString("data");
					mkDir(a_dataDirConfigSystem);
				}
			}
		}
	}
	if (a_dataDirConfigSystem.Length()>2){
		prv_strNukak3DDataDir = a_dataDirConfigSystem;
	}
	if (isValidDirName(prv_strNukak3DDataDir))	writeConfigBase("Paths/DataDir",prv_strNukak3DDataDir);
	return prv_strNukak3DDataDir;
}

wxString nkUtilities::getStringFromCharToken(wxString a_string, wxChar a_token, int index){
	wxString a_result = "";
	int i=-1;
	int ind =0;
	wxString temp = a_string;
	if(temp.Length()>0){
		while(i<index && ind != wxNOT_FOUND){
			i = i + 1;
			ind = temp.Find(a_token);
			if(ind != wxNOT_FOUND){
				if(i == index){
					a_result = temp.Left(ind);
				}else{
					temp = temp.Mid(ind+1);
				}
			}
		}

	}
	return a_result;
}

int nkUtilities::countCharFromString(wxString a_string, wxChar a_charToFind){
	int i;
	int n = a_string.Length();
	int cant = 0;
	for (i=0; i<n; i++ ){
		if(a_string.GetChar(i) == a_charToFind) cant++;
	}
	return cant;
}

bool nkUtilities::writeConfigBase(const wxString& key, const wxString& value){
	bool canwrite = false;
	wxConfigBase *my_Config = wxConfigBase::Get();
	if ( my_Config != NULL){
		canwrite = my_Config->Write(key, value);
	}
	return canwrite;
}

bool nkUtilities::writeConfigBase(const wxString& key, long value){
	bool canwrite = false;
	wxConfigBase *my_Config = wxConfigBase::Get();
	if ( my_Config != NULL){
		canwrite = my_Config->Write(key, value);
	}
	return canwrite;
}

bool nkUtilities::readConfigBase(const wxString& key, wxString* str, const wxString& defaultVal){
	bool canRead = false;
	wxConfigBase *my_Config = wxConfigBase::Get();
	if ( my_Config != NULL){
		canRead = my_Config->Read(key, str, defaultVal);
	}
	return canRead;
}

bool nkUtilities::readConfigBase(const wxString& key, long* al, long defaultVal){
	bool canRead = false;
	wxConfigBase *my_Config = wxConfigBase::Get();
	if ( my_Config != NULL){
		canRead = my_Config->Read(key, al, defaultVal);
	}
	return canRead;
}

bool nkUtilities::existNukak3DDataDir(){
	return existDir(prv_strNukak3DDataDir);
}

wxString nkUtilities::getExecutableExtension(){
	#ifdef __WIN32__
		return wxString(".exe");
	#else
		return wxString("");
	#endif //__WIN32__
}

bool nkUtilities::appendNukak3DDataDir(const wxString& dirName, int perm, int flags){
	bool appendOk = false;
	wxString a_path = "";
	if(existDir( getNukak3DDataDir()) ){	
		a_path.Append(getNukak3DDataDir());
		a_path.Append(wxFileName::GetPathSeparator());
		a_path.Append(dirName);
		appendOk = mkDir(a_path, perm, flags);
	}
	return appendOk;
}

bool nkUtilities::setWorkingDirectory(const wxString& dir){
	return wxSetWorkingDirectory(dir);
}

bool nkUtilities::setWorkingDirectoryNukak3D(){
	return wxSetWorkingDirectory(prv_strNukak3DPath);
}

bool nkUtilities::appendDir(const wxString& dirPath, const wxString& dirName, int perm , int flags ){
	bool appendOk = false;
	wxString a_path = "";
	if(mkDir(dirPath)){	
		a_path.Append(dirPath);
		a_path.Append(wxFileName::GetPathSeparator());
		a_path.Append(dirName);
		appendOk = mkDir(a_path, perm, flags);
	}
	return appendOk;
}

bool nkUtilities::copyFile(const wxString& file1, const wxString& file2, bool overwrite){
	return  wxCopyFile(file1, file2, overwrite );
}

bool nkUtilities::existDir(const wxString& dir){
	return wxFileName::DirExists(dir);
}

bool nkUtilities::isDirWritable(const wxString& dir){
	return wxFileName::IsDirWritable(dir);
}

bool nkUtilities::mkDir(const wxString& dir, int perm, int flags){
	if(existDir(dir)){
		if(isDirWritable(dir))	return true;
		else return false;
	}
	if (! isValidDirName(dir)) return false;
	return wxFileName::Mkdir(dir, perm, flags);
}

bool nkUtilities::existFile(const wxString& a_file){
	return wxFileName::FileExists(a_file);
}

bool nkUtilities::isValidDirName(const wxString& dir){
	wxFileName my_wxF(dir);
	return my_wxF.IsOk();
}