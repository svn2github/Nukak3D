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
 * @file nkAdminPluginGui.h
 * @brief Preprocessor definitions.
 * @details Preprocessor definitions, for load dynamic library in windows.
 * @author Alexander Pinzón Fernandez
 * @date 13/04/2009 4:11 p.m.
*/
#ifndef __NKENGINE__H__
#define __NKENGINE__H__

#if defined (WIN32) || defined (_WIN32_)
	#ifdef NKENGINE_EXPORTS
		#define NKENGINE_API __declspec(dllexport)
	#else
		#define NKENGINE_API __declspec(dllimport)
	#endif
#else
	#define NKENGINE_API
#endif

#endif // __NKENGINE__H__