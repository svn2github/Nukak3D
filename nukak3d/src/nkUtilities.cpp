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

/** 
 * @file nkUtilities.cpp
 * @brief Utilities functions.
 * @details Functions and macros for general prupose.
 * @author Alexander Pinzón Fernandez
 * @version 0.2
 * @date 01/30/2009 12:49 p.m.
*/

#include "nkUtilities.h"

wxString nkUtilities::prv_strNukak3DPath = wxString("");

wxString nkUtilities::prv_strNukak3DDataDir = wxString("");

wxArrayString nkUtilities::prv_strNukak3DPluginsPaths;

nkUtilities::nkUtilities(){
	//Nothing
}

wxString nkUtilities::getNukak3DPath(){
	if(prv_strNukak3DPath.Length()<1){
		prv_strNukak3DPath = wxStandardPaths::Get().GetExecutablePath();
		prv_strNukak3DPath = prv_strNukak3DPath.Left(prv_strNukak3DPath.find_last_of("nukak3d"));
		prv_strNukak3DPath = prv_strNukak3DPath.Left(prv_strNukak3DPath.Len() - 7);
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
			a_dataDirConfigSystem = wxStandardPaths::Get().GetUserLocalDataDir();
			if(mkDir(a_dataDirConfigSystem))
			{
				a_dataDirConfigSystem.Append(wxFileName::GetPathSeparator());
				a_dataDirConfigSystem.Append(wxString("data"));
				if(!mkDir(a_dataDirConfigSystem))
				{
					a_dataDirConfigSystem = "";
				}
			}else
			{
				a_dataDirConfigSystem = wxStandardPaths::Get().GetDocumentsDir() + wxFileName::GetPathSeparator() + wxString("Nukak3D");
				if(mkDir(a_dataDirConfigSystem))
				{
					a_dataDirConfigSystem.Append(wxFileName::GetPathSeparator());
					a_dataDirConfigSystem.Append(wxString("data"));
					mkDir(a_dataDirConfigSystem);
					if(!mkDir(a_dataDirConfigSystem))
					{
						a_dataDirConfigSystem = "";
					}
				}else
				{
					a_dataDirConfigSystem = wxStandardPaths::Get().GetTempDir() + wxFileName::GetPathSeparator() + wxString("Nukak3D");
					if (mkDir(a_dataDirConfigSystem)){
						a_dataDirConfigSystem.Append(wxFileName::GetPathSeparator());
						a_dataDirConfigSystem.Append(wxString("data"));
						mkDir(a_dataDirConfigSystem);
						if(!mkDir(a_dataDirConfigSystem))
						{
							a_dataDirConfigSystem = "";
						}
					}else{
						a_dataDirConfigSystem = "";
					}
				}
			}
		}
	}
	if (a_dataDirConfigSystem.Length()>10){
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

bool nkUtilities::writePluginsPaths(void){
	wxString strPlugins = "";
	wxChar charTokenSparater = '|';
	size_t i, cant = prv_strNukak3DPluginsPaths.size();
	for (i=0; i<cant; i++){
		strPlugins  = strPlugins + prv_strNukak3DPluginsPaths.Item(i) + wxString("|");
	}
	return writeConfigBase(_("Paths/Plugins"), strPlugins);
}

bool nkUtilities::readPluginsPaths(void){
	wxString strPlugins = "";
	bool readPlugin = readConfigBase(_("Paths/Plugins"), &strPlugins, "");
	if(readPlugin)	{
		wxChar charTokenSparater = '|';
		size_t i, cant = countCharFromString(strPlugins, charTokenSparater);
		prv_strNukak3DPluginsPaths.Clear();
		for(i=0; i<cant; i++){
			prv_strNukak3DPluginsPaths.Add(getStringFromCharToken(strPlugins, charTokenSparater,i));
		}
	}
	return readPlugin;
}

void nkUtilities::addNukak3DPluginPath(const wxString& dir){
	bool okAdd = true;
	if(nkUtilities::existDir(dir)){
		size_t i, cant = prv_strNukak3DPluginsPaths.size();
		for(i=0; i<cant; i++){
			if(prv_strNukak3DPluginsPaths.Item(i).Cmp(dir) == 0){
				okAdd = false;
			}
		}
		if(okAdd){
			prv_strNukak3DPluginsPaths.Add(dir);
		}
	}
}

wxArrayString& nkUtilities::getNukak3DPluginsPaths(void){
	return prv_strNukak3DPluginsPaths;
}

wxArrayString& nkUtilities::getNukak3DPluginsNames(void){
	wxArrayString *strPlugins, strPluginsTemp;
	strPlugins = new wxArrayString();
	size_t i, j, canPluginsTemp, cantPaths = prv_strNukak3DPluginsPaths.size();
	wxString pluginEXT = wxString("*") + wxString(wxDynamicLibrary::GetDllExt());
	strPluginsTemp.Add("");
	for(i=0; i<cantPaths; i++){
		strPluginsTemp.Clear();
		wxDir::GetAllFiles(prv_strNukak3DPluginsPaths.Item(i), &strPluginsTemp, pluginEXT);
		canPluginsTemp = strPluginsTemp.size();
		for(j=0; j<canPluginsTemp; j++){
			strPlugins->Add( strPluginsTemp.Item(j));
			//prv_strNukak3DPluginsPaths.Item(i)<<wxFileName::GetPathSeparator();
		}
	}
	return *strPlugins;
}