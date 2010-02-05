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
 * @file nkDICOMDictionary.cpp
 * @brief DICOM Data Dictionary.
 * @details Clases for DICOM Data Dictionary.
 * @author Alexander Pinzón Fernandez
 * @version 0.1
 * @date 26/01/2009 03:51 p.m.
*/

#include "nkDICOMDictionary.h"
#include "wx/listimpl.cpp"

WX_DEFINE_LIST(nkListDictionayKey);

nkDICOMDictionary * nkDICOMDictionary::_instance = 0;

nkDICOMDictionary::nkDICOMDictionary(){
	my_Dictionary.Append(new nkDictionayKey("0000,0000", "UL", "Group 0000 Length"));
	my_Dictionary.Append(new nkDictionayKey("0000,0001", "UL", "Group 0000 Length to End (RET)"));
	my_Dictionary.Append(new nkDictionayKey("0000,0002", "UI", "Affected SOP Class UID"));
	my_Dictionary.Append(new nkDictionayKey("0000,0003", "UI", "Requested SOP Class UID"));
	my_Dictionary.Append(new nkDictionayKey("0000,0010", "SH", "Recognition Code (RET)"));
	my_Dictionary.Append(new nkDictionayKey("0000,0100", "US", "Command Field"));
	my_Dictionary.Append(new nkDictionayKey("0000,0110", "US", "Message ID"));
	my_Dictionary.Append(new nkDictionayKey("0000,0120", "US", "Message Id being Responded to"));
	my_Dictionary.Append(new nkDictionayKey("0000,0200", "AE", "Initiator (RET)"));
	my_Dictionary.Append(new nkDictionayKey("0000,0300", "AE", "Receiver (RET)"));
	my_Dictionary.Append(new nkDictionayKey("0000,0400", "AE", "Find Location (RET)"));
	my_Dictionary.Append(new nkDictionayKey("0000,0600", "AE", "Move Destination"));
	my_Dictionary.Append(new nkDictionayKey("0000,0700", "US", "Priority"));
	my_Dictionary.Append(new nkDictionayKey("0000,0800", "US", "Data Set Type"));
	my_Dictionary.Append(new nkDictionayKey("0000,0850", "US", "Number of Matches (RET)"));
	my_Dictionary.Append(new nkDictionayKey("0000,0860", "US", "Response Sequence Number (RET)"));
	my_Dictionary.Append(new nkDictionayKey("0000,0900", "US", "Status"));
	my_Dictionary.Append(new nkDictionayKey("0000,0901", "AT", "Offending Element"));
	my_Dictionary.Append(new nkDictionayKey("0000,0902", "LO", "Error Comment"));
	my_Dictionary.Append(new nkDictionayKey("0000,0903", "US", "Error ID"));
	my_Dictionary.Append(new nkDictionayKey("0000,1000", "UI", "Affected SOP Instance UID"));
	my_Dictionary.Append(new nkDictionayKey("0000,1001", "UI", "Requested SOP Instance UID"));
	my_Dictionary.Append(new nkDictionayKey("0000,1002", "US", "Event Type ID"));
	my_Dictionary.Append(new nkDictionayKey("0000,1005", "AT", "Attribute Identifier List"));
	my_Dictionary.Append(new nkDictionayKey("0000,1008", "US", "Action Type ID"));
	my_Dictionary.Append(new nkDictionayKey("0000,1012", "UI", "Requested SOP Instance UID List"));
	my_Dictionary.Append(new nkDictionayKey("0000,1020", "US", "Number of Remaining Sub-operations"));
	my_Dictionary.Append(new nkDictionayKey("0000,1021", "US", "Number of Completed Sub-operations"));
	my_Dictionary.Append(new nkDictionayKey("0000,1022", "US", "Number of Failed Sub-operations"));
	my_Dictionary.Append(new nkDictionayKey("0000,1023", "US", "Number of Warning Sub-operations"));
	my_Dictionary.Append(new nkDictionayKey("0000,1030", "AE", "Move Originator Application Entity Title"));
	my_Dictionary.Append(new nkDictionayKey("0000,1031", "US", "Move Originator Message ID"));
	my_Dictionary.Append(new nkDictionayKey("0000,5010", "LO", "Message Set ID (RET)"));
	my_Dictionary.Append(new nkDictionayKey("0000,5020", "LO", "End Message Set ID (RET)"));
	my_Dictionary.Append(new nkDictionayKey("0002,0000", "UL", "Group 0002 Length"));
	my_Dictionary.Append(new nkDictionayKey("0002,0001", "OB", "File Meta Information Version"));
	my_Dictionary.Append(new nkDictionayKey("0002,0002", "UI", "Media Stored SOP Class UID"));
	my_Dictionary.Append(new nkDictionayKey("0002,0003", "UI", "Media Stored SOP Instance UID"));
	my_Dictionary.Append(new nkDictionayKey("0002,0010", "UI", "Transfer Syntax UID"));
	my_Dictionary.Append(new nkDictionayKey("0002,0012", "UI", "Implementation Class UID"));
	my_Dictionary.Append(new nkDictionayKey("0002,0013", "SH", "Implementation Version Name"));
	my_Dictionary.Append(new nkDictionayKey("0002,0016", "AE", "Source Application Entity Title"));
	my_Dictionary.Append(new nkDictionayKey("0002,0100", "UI", "Private Information Creator UID"));
	my_Dictionary.Append(new nkDictionayKey("0002,0102", "OB", "Private Information"));
	my_Dictionary.Append(new nkDictionayKey("0004,0000", "UL", "Group 0004 Length"));
	my_Dictionary.Append(new nkDictionayKey("0004,1130", "CS", "File-set ID"));
	my_Dictionary.Append(new nkDictionayKey("0004,1141", "CS", "File-set Descriptor File File ID"));
	my_Dictionary.Append(new nkDictionayKey("0004,1142", "CS", "File-set Descriptor File Format"));
	my_Dictionary.Append(new nkDictionayKey("0004,1200", "UL", "Root Directory Entity's First Directory Record Offset"));
	my_Dictionary.Append(new nkDictionayKey("0004,1202", "UL", "Root Directory Entity's Last Directory Record Offset"));
	my_Dictionary.Append(new nkDictionayKey("0004,1212", "US", "File-set Consistence Flag"));
	my_Dictionary.Append(new nkDictionayKey("0004,1220", "SQ", "Directory Record Sequence"));
	my_Dictionary.Append(new nkDictionayKey("0004,1400", "UL", "Next Directory Record Offset"));
	my_Dictionary.Append(new nkDictionayKey("0004,1410", "US", "Record In-use Flag"));
	my_Dictionary.Append(new nkDictionayKey("0004,1420", "UL", "Referenced Lower-level Directory Entity Offset"));
	my_Dictionary.Append(new nkDictionayKey("0004,1430", "CS", "Directory Record Type"));
	my_Dictionary.Append(new nkDictionayKey("0004,1432", "UI", "Private Record UID"));
	my_Dictionary.Append(new nkDictionayKey("0004,1500", "CS", "Referenced File ID"));
	my_Dictionary.Append(new nkDictionayKey("0004,1510", "UI", "Referenced SOP Class UID in File"));
	my_Dictionary.Append(new nkDictionayKey("0004,1511", "UI", "Referenced SOP Instance UID in File"));
	my_Dictionary.Append(new nkDictionayKey("0004,1600", "UL", "Number of References"));
	my_Dictionary.Append(new nkDictionayKey("0008,0000", "UL", "Group 0008 Length"));
	my_Dictionary.Append(new nkDictionayKey("0008,0001", "UL", "Group 0008 Length to End (RET)"));
	my_Dictionary.Append(new nkDictionayKey("0008,0005", "CS", "Specific Character Set"));
	my_Dictionary.Append(new nkDictionayKey("0008,0008", "CS", "Image Type"));
	my_Dictionary.Append(new nkDictionayKey("0008,0010", "SH", "Recognition Code (RET)"));
	my_Dictionary.Append(new nkDictionayKey("0008,0012", "DA", "Instance Creation Date"));
	my_Dictionary.Append(new nkDictionayKey("0008,0013", "TM", "Instance Creation Time"));
	my_Dictionary.Append(new nkDictionayKey("0008,0014", "UI", "Instance Creator UID"));
	my_Dictionary.Append(new nkDictionayKey("0008,0016", "UI", "SOP Class UID"));
	my_Dictionary.Append(new nkDictionayKey("0008,0018", "UI", "SOP Instance UID"));
	my_Dictionary.Append(new nkDictionayKey("0008,0020", "DA", "Study Date"));
	my_Dictionary.Append(new nkDictionayKey("0008,0021", "DA", "Series Date"));
	my_Dictionary.Append(new nkDictionayKey("0008,0022", "DA", "Acquisition Date"));
	my_Dictionary.Append(new nkDictionayKey("0008,0023", "DA", "Image Date"));
	my_Dictionary.Append(new nkDictionayKey("0008,0024", "DA", "Overlay Date"));
	my_Dictionary.Append(new nkDictionayKey("0008,0025", "DA", "Curve Date"));
	my_Dictionary.Append(new nkDictionayKey("0008,0030", "TM", "Study Time"));
	my_Dictionary.Append(new nkDictionayKey("0008,0031", "TM", "Series Time"));
	my_Dictionary.Append(new nkDictionayKey("0008,0032", "TM", "Acquisition Time"));
	my_Dictionary.Append(new nkDictionayKey("0008,0033", "TM", "Image Time"));
	my_Dictionary.Append(new nkDictionayKey("0008,0034", "TM", "Overlay Time"));
	my_Dictionary.Append(new nkDictionayKey("0008,0035", "TM", "Curve Time"));
	my_Dictionary.Append(new nkDictionayKey("0008,0040", "US", "Data Set Type (RET)"));
	my_Dictionary.Append(new nkDictionayKey("0008,0041", "SH", "Data Set Subtype (RET)"));
	my_Dictionary.Append(new nkDictionayKey("0008,0042", "CS", "Nuclear Medicine Series Type"));
	my_Dictionary.Append(new nkDictionayKey("0008,0050", "SH", "Accession Number"));
	my_Dictionary.Append(new nkDictionayKey("0008,0052", "CS", "Query/Retrieve Level"));
	my_Dictionary.Append(new nkDictionayKey("0008,0054", "AE", "Retrieve AE Title"));
	my_Dictionary.Append(new nkDictionayKey("0008,0058", "AE", "Failed SOP Instance UID List"));
	my_Dictionary.Append(new nkDictionayKey("0008,0060", "CS", "Modality"));
	my_Dictionary.Append(new nkDictionayKey("0008,0064", "CS", "Conversion Type"));
	my_Dictionary.Append(new nkDictionayKey("0008,0070", "LO", "Manufacturer"));
	my_Dictionary.Append(new nkDictionayKey("0008,0080", "LO", "Institution Name"));
	my_Dictionary.Append(new nkDictionayKey("0008,0081", "ST", "Institution Address"));
	my_Dictionary.Append(new nkDictionayKey("0008,0082", "SQ", "Institution Code Sequence"));
	my_Dictionary.Append(new nkDictionayKey("0008,0090", "PN", "Referring Physician's Name"));
	my_Dictionary.Append(new nkDictionayKey("0008,0092", "ST", "Referring Physician's Address"));
	my_Dictionary.Append(new nkDictionayKey("0008,0094", "SH", "Referring Physician's Telephone Numbers"));
	my_Dictionary.Append(new nkDictionayKey("0008,0100", "SH", "Code Value"));
	my_Dictionary.Append(new nkDictionayKey("0008,0102", "SH", "Coding Scheme Designator"));
	my_Dictionary.Append(new nkDictionayKey("0008,0104", "LO", "Code Meaning"));
	my_Dictionary.Append(new nkDictionayKey("0008,1000", "SH", "Network ID (RET)"));
	my_Dictionary.Append(new nkDictionayKey("0008,1010", "SH", "Station Name"));
	my_Dictionary.Append(new nkDictionayKey("0008,1030", "LO", "Study Description"));
	my_Dictionary.Append(new nkDictionayKey("0008,1032", "SQ", "Procedure Code Sequence"));
	my_Dictionary.Append(new nkDictionayKey("0008,103E", "LO", "Series Description"));
	my_Dictionary.Append(new nkDictionayKey("0008,1040", "LO", "Institutional Department Name"));
	my_Dictionary.Append(new nkDictionayKey("0008,1050", "PN", "Attending Physician's Name"));
	my_Dictionary.Append(new nkDictionayKey("0008,1060", "PN", "Name of Physician(s) Reading Study"));
	my_Dictionary.Append(new nkDictionayKey("0008,1070", "PN", "Operator's Name"));
	my_Dictionary.Append(new nkDictionayKey("0008,1080", "LO", "Admitting Diagnoses Description"));
	my_Dictionary.Append(new nkDictionayKey("0008,1084", "SQ", "Admitting Diagnosis Code Sequence"));
	my_Dictionary.Append(new nkDictionayKey("0008,1090", "LO", "Manufacturer's Model Name"));
	my_Dictionary.Append(new nkDictionayKey("0008,1100", "SQ", "Referenced Results Sequence"));
	my_Dictionary.Append(new nkDictionayKey("0008,1110", "SQ", "Referenced Study Sequence"));
	my_Dictionary.Append(new nkDictionayKey("0008,1111", "SQ", "Referenced Study Component Sequence"));
	my_Dictionary.Append(new nkDictionayKey("0008,1115", "SQ", "Referenced Series Sequence"));
	my_Dictionary.Append(new nkDictionayKey("0008,1120", "SQ", "Referenced Patient Sequence"));
	my_Dictionary.Append(new nkDictionayKey("0008,1125", "SQ", "Referenced Visit Sequence"));
	my_Dictionary.Append(new nkDictionayKey("0008,1130", "SQ", "Referenced Overlay Sequence"));
	my_Dictionary.Append(new nkDictionayKey("0008,1140", "SQ", "Referenced Image Sequence"));
	my_Dictionary.Append(new nkDictionayKey("0008,1145", "SQ", "Referenced Curve Sequence"));
	my_Dictionary.Append(new nkDictionayKey("0008,1150", "UI", "Referenced SOP Class UID"));
	my_Dictionary.Append(new nkDictionayKey("0008,1155", "UI", "Referenced SOP Instance UID"));
	my_Dictionary.Append(new nkDictionayKey("0008,2111", "ST", "Derivation Description"));
	my_Dictionary.Append(new nkDictionayKey("0008,2112", "SQ", "Source Image Sequence"));
	my_Dictionary.Append(new nkDictionayKey("0008,2120", "SH", "Stage Name"));
	my_Dictionary.Append(new nkDictionayKey("0008,2122", "IS", "Stage Number"));
	my_Dictionary.Append(new nkDictionayKey("0008,2124", "IS", "Number of Stages"));
	my_Dictionary.Append(new nkDictionayKey("0008,2129", "IS", "Number of Event Timers"));
	my_Dictionary.Append(new nkDictionayKey("0008,2128", "IS", "View Number"));
	my_Dictionary.Append(new nkDictionayKey("0008,212A", "IS", "Number of Views in Stage"));
	my_Dictionary.Append(new nkDictionayKey("0008,2130", "DS", "Event Elapsed Time(s)"));
	my_Dictionary.Append(new nkDictionayKey("0008,2132", "LO", "Event Timer Name(s)"));
	my_Dictionary.Append(new nkDictionayKey("0008,2142", "IS", "Start Trim"));
	my_Dictionary.Append(new nkDictionayKey("0008,2143", "IS", "Stop Trim"));
	my_Dictionary.Append(new nkDictionayKey("0008,2144", "IS", "Recommended Display Frame Rate"));
	my_Dictionary.Append(new nkDictionayKey("0008,2200", "CS", "Transducer Position"));
	my_Dictionary.Append(new nkDictionayKey("0008,2204", "CS", "Transducer Orientation"));
	my_Dictionary.Append(new nkDictionayKey("0008,2208", "CS", "Anatomic Structure"));
	my_Dictionary.Append(new nkDictionayKey("0008,4000", "SH", "Group 0008 Comments (RET)"));
	my_Dictionary.Append(new nkDictionayKey("0010,0000", "UL", "Group 0010 Length"));
	my_Dictionary.Append(new nkDictionayKey("0010,0010", "PN", "Patient's Name"));
	my_Dictionary.Append(new nkDictionayKey("0010,0020", "LO", "Patient ID"));
	my_Dictionary.Append(new nkDictionayKey("0010,0021", "LO", "Issuer of Patient ID"));
	my_Dictionary.Append(new nkDictionayKey("0010,0030", "DA", "Patient's Birth Date"));
	my_Dictionary.Append(new nkDictionayKey("0010,0032", "TM", "Patient's Birth Time"));
	my_Dictionary.Append(new nkDictionayKey("0010,0040", "CS", "Patient's Sex"));
	my_Dictionary.Append(new nkDictionayKey("0010,0042", "SH", "Patient's Social Security Number"));
	my_Dictionary.Append(new nkDictionayKey("0010,0050", "SQ", "Patient's Insurance Plan Code Sequence"));
	my_Dictionary.Append(new nkDictionayKey("0010,1000", "LO", "Other Patient IDs"));
	my_Dictionary.Append(new nkDictionayKey("0010,1001", "PN", "Other Patient Names"));
	my_Dictionary.Append(new nkDictionayKey("0010,1005", "PN", "Patient's Maiden Name"));
	my_Dictionary.Append(new nkDictionayKey("0010,1010", "AS", "Patient's Age"));
	my_Dictionary.Append(new nkDictionayKey("0010,1020", "DS", "Patient's Size"));
	my_Dictionary.Append(new nkDictionayKey("0010,1030", "DS", "Patient's Weight"));
	my_Dictionary.Append(new nkDictionayKey("0010,1040", "LO", "Patient's Address"));
	my_Dictionary.Append(new nkDictionayKey("0010,1050", "SH", "Insurance Plan Identification (RET)"));
	my_Dictionary.Append(new nkDictionayKey("0010,1060", "PN", "Patient's Mother's Maiden Name"));
	my_Dictionary.Append(new nkDictionayKey("0010,1080", "LO", "Military Rank"));
	my_Dictionary.Append(new nkDictionayKey("0010,1081", "LO", "Branch of Service"));
	my_Dictionary.Append(new nkDictionayKey("0010,1090", "LO", "Medical Record Locator"));
	my_Dictionary.Append(new nkDictionayKey("0010,2000", "LO", "Medical Alerts"));
	my_Dictionary.Append(new nkDictionayKey("0010,2110", "LO", "Contrast Allergies"));
	my_Dictionary.Append(new nkDictionayKey("0010,2150", "LO", "Country of Residence"));
	my_Dictionary.Append(new nkDictionayKey("0010,2152", "LO", "Region of Residence"));
	my_Dictionary.Append(new nkDictionayKey("0010,2154", "SH", "Patient's Telephone Numbers"));
	my_Dictionary.Append(new nkDictionayKey("0010,2160", "SH", "Ethnic Group"));
	my_Dictionary.Append(new nkDictionayKey("0010,2180", "SH", "Occupation"));
	my_Dictionary.Append(new nkDictionayKey("0010,21A0", "CS", "Smoking Status"));
	my_Dictionary.Append(new nkDictionayKey("0010,21B0", "LT", "Additional Patient History"));
	my_Dictionary.Append(new nkDictionayKey("0010,21C0", "US", "Pregnancy Status"));
	my_Dictionary.Append(new nkDictionayKey("0010,21D0", "DA", "Last Menstrual Date"));
	my_Dictionary.Append(new nkDictionayKey("0010,21F0", "LO", "Patient's Religious Preference"));
	my_Dictionary.Append(new nkDictionayKey("0010,4000", "LT", "Patient Comments"));
	my_Dictionary.Append(new nkDictionayKey("0018,0000", "UL", "Group 0018 Length"));
	my_Dictionary.Append(new nkDictionayKey("0018,0010", "LO", "Contrast/Bolus Agent"));
	my_Dictionary.Append(new nkDictionayKey("0018,0015", "CS", "Body Part Examined"));
	my_Dictionary.Append(new nkDictionayKey("0018,0020", "CS", "Scanning Sequence"));
	my_Dictionary.Append(new nkDictionayKey("0018,0021", "CS", "Sequence Variant"));
	my_Dictionary.Append(new nkDictionayKey("0018,0022", "CS", "Scan Options"));
	my_Dictionary.Append(new nkDictionayKey("0018,0023", "CS", "MR Acquisition Type"));
	my_Dictionary.Append(new nkDictionayKey("0018,0024", "SH", "Sequence Name"));
	my_Dictionary.Append(new nkDictionayKey("0018,0025", "CS", "Angio Flag"));
	my_Dictionary.Append(new nkDictionayKey("0018,0030", "LO", "Radionuclide"));
	my_Dictionary.Append(new nkDictionayKey("0018,0031", "LO", "Radiopharmaceutical"));
	my_Dictionary.Append(new nkDictionayKey("0018,0032", "DS", "Energy Window Centerline"));
	my_Dictionary.Append(new nkDictionayKey("0018,0033", "DS", "Energy Window Total Width"));
	my_Dictionary.Append(new nkDictionayKey("0018,0034", "LO", "Intervention Drug Name"));
	my_Dictionary.Append(new nkDictionayKey("0018,0035", "TM", "Intervention Drug Start Time"));
	my_Dictionary.Append(new nkDictionayKey("0018,0040", "IS", "Cine Rate"));
	my_Dictionary.Append(new nkDictionayKey("0018,0050", "DS", "Slice Thickness"));
	my_Dictionary.Append(new nkDictionayKey("0018,0060", "DS", "KVP"));
	my_Dictionary.Append(new nkDictionayKey("0018,0070", "IS", "Counts Accumulated"));
	my_Dictionary.Append(new nkDictionayKey("0018,0071", "CS", "Acquisition Termination Condition"));
	my_Dictionary.Append(new nkDictionayKey("0018,0072", "DS", "Effective Series Duration"));
	my_Dictionary.Append(new nkDictionayKey("0018,0080", "DS", "Repetition Time"));
	my_Dictionary.Append(new nkDictionayKey("0018,0081", "DS", "Echo Time"));
	my_Dictionary.Append(new nkDictionayKey("0018,0082", "DS", "Inversion Time"));
	my_Dictionary.Append(new nkDictionayKey("0018,0083", "DS", "Number of Averages"));
	my_Dictionary.Append(new nkDictionayKey("0018,0084", "DS", "Imaging Frequency"));
	my_Dictionary.Append(new nkDictionayKey("0018,0085", "SH", "Imaged Nucleus"));
	my_Dictionary.Append(new nkDictionayKey("0018,0086", "IS", "Echo Numbers(s)"));
	my_Dictionary.Append(new nkDictionayKey("0018,0087", "DS", "Magnetic Field Strength"));
	my_Dictionary.Append(new nkDictionayKey("0018,0088", "DS", "Spacing Between Slices"));
	my_Dictionary.Append(new nkDictionayKey("0018,0089", "IS", "Number of Phase Encoding Steps"));
	my_Dictionary.Append(new nkDictionayKey("0018,0090", "DS", "Data Collection Diameter"));
	my_Dictionary.Append(new nkDictionayKey("0018,0091", "IS", "Echo Train Length"));
	my_Dictionary.Append(new nkDictionayKey("0018,0093", "DS", "Percent Sampling"));
	my_Dictionary.Append(new nkDictionayKey("0018,0094", "DS", "Percent Phase Field of View"));
	my_Dictionary.Append(new nkDictionayKey("0018,0095", "DS", "Pixel Bandwidth"));
	my_Dictionary.Append(new nkDictionayKey("0018,1000", "LO", "Device Serial Number"));
	my_Dictionary.Append(new nkDictionayKey("0018,1004", "LO", "Plate ID"));
	my_Dictionary.Append(new nkDictionayKey("0018,1010", "LO", "Secondary Capture Device ID"));
	my_Dictionary.Append(new nkDictionayKey("0018,1012", "DA", "Date of Secondary Capture"));
	my_Dictionary.Append(new nkDictionayKey("0018,1014", "TM", "Time of Secondary Capture"));
	my_Dictionary.Append(new nkDictionayKey("0018,1016", "LO", "Secondary Capture Device Manufacturer"));
	my_Dictionary.Append(new nkDictionayKey("0018,1018", "LO", "Secondary Capture Device Manufacturer's Model Name"));
	my_Dictionary.Append(new nkDictionayKey("0018,1019", "LO", "Secondary Capture Device Software Version(s)"));
	my_Dictionary.Append(new nkDictionayKey("0018,1020", "LO", "Software Versions(s)"));
	my_Dictionary.Append(new nkDictionayKey("0018,1022", "SH", "Video Image Format Acquired"));
	my_Dictionary.Append(new nkDictionayKey("0018,1023", "LO", "Digital Image Format Acquired"));
	my_Dictionary.Append(new nkDictionayKey("0018,1030", "LO", "Protocol Name"));
	my_Dictionary.Append(new nkDictionayKey("0018,1040", "LO", "Contrast/Bolus Route"));
	my_Dictionary.Append(new nkDictionayKey("0018,1041", "DS", "Contrast/Bolus Volume"));
	my_Dictionary.Append(new nkDictionayKey("0018,1042", "TM", "Contrast/Bolus Start Time"));
	my_Dictionary.Append(new nkDictionayKey("0018,1043", "TM", "Contrast/Bolus Stop Time"));
	my_Dictionary.Append(new nkDictionayKey("0018,1044", "DS", "Contrast/Bolus Total Dose"));
	my_Dictionary.Append(new nkDictionayKey("0018,1045", "IS", "Syringe Counts"));
	my_Dictionary.Append(new nkDictionayKey("0018,1050", "DS", "Spatial Resolution"));
	my_Dictionary.Append(new nkDictionayKey("0018,1060", "DS", "Trigger Time"));
	my_Dictionary.Append(new nkDictionayKey("0018,1061", "LO", "Trigger Source or Type"));
	my_Dictionary.Append(new nkDictionayKey("0018,1062", "IS", "Nominal Interval"));
	my_Dictionary.Append(new nkDictionayKey("0018,1063", "DS", "Frame Time"));
	my_Dictionary.Append(new nkDictionayKey("0018,1064", "LO", "Framing Type"));
	my_Dictionary.Append(new nkDictionayKey("0018,1065", "DS", "Frame Time Vector"));
	my_Dictionary.Append(new nkDictionayKey("0018,1066", "DS", "Frame Delay"));
	my_Dictionary.Append(new nkDictionayKey("0018,1070", "LO", "Radionuclide Route"));
	my_Dictionary.Append(new nkDictionayKey("0018,1071", "DS", "Radionuclide Volume"));
	my_Dictionary.Append(new nkDictionayKey("0018,1072", "TM", "Radionuclide Start Time"));
	my_Dictionary.Append(new nkDictionayKey("0018,1073", "TM", "Radionuclide Stop Time"));
	my_Dictionary.Append(new nkDictionayKey("0018,1074", "DS", "Radionuclide Total Dose"));
	my_Dictionary.Append(new nkDictionayKey("0018,1080", "CS", "Beat Rejection Flag"));
	my_Dictionary.Append(new nkDictionayKey("0018,1081", "IS", "Low R-R Value"));
	my_Dictionary.Append(new nkDictionayKey("0018,1082", "IS", "High R-R Value"));
	my_Dictionary.Append(new nkDictionayKey("0018,1083", "IS", "Intervals Acquired"));
	my_Dictionary.Append(new nkDictionayKey("0018,1084", "IS", "Intervals Rejected"));
	my_Dictionary.Append(new nkDictionayKey("0018,1085", "LO", "PVC Rejection"));
	my_Dictionary.Append(new nkDictionayKey("0018,1086", "IS", "Skip Beats"));
	my_Dictionary.Append(new nkDictionayKey("0018,1088", "IS", "Heart Rate"));
	my_Dictionary.Append(new nkDictionayKey("0018,1090", "IS", "Cardiac Number of Images"));
	my_Dictionary.Append(new nkDictionayKey("0018,1094", "IS", "Trigger Window"));
	my_Dictionary.Append(new nkDictionayKey("0018,1100", "DS", "Reconstruction Diameter"));
	my_Dictionary.Append(new nkDictionayKey("0018,1110", "DS", "Distance Source to Detector"));
	my_Dictionary.Append(new nkDictionayKey("0018,1111", "DS", "Distance Source to Patient"));
	my_Dictionary.Append(new nkDictionayKey("0018,1120", "DS", "Gantry/Detector Tilt"));
	my_Dictionary.Append(new nkDictionayKey("0018,1030", "DS", "Table Height"));
	my_Dictionary.Append(new nkDictionayKey("0018,1131", "DS", "Table Traverse"));
	my_Dictionary.Append(new nkDictionayKey("0018,1140", "CS", "Rotation Direction"));
	my_Dictionary.Append(new nkDictionayKey("0018,1141", "DS", "Angular Position"));
	my_Dictionary.Append(new nkDictionayKey("0018,1142", "DS", "Radial Position"));
	my_Dictionary.Append(new nkDictionayKey("0018,1143", "DS", "Scan Arc"));
	my_Dictionary.Append(new nkDictionayKey("0018,1144", "DS", "Angular Step"));
	my_Dictionary.Append(new nkDictionayKey("0018,1145", "DS", "Center of Rotation Offset"));
	my_Dictionary.Append(new nkDictionayKey("0018,1146", "DS", "Rotation Offset"));
	my_Dictionary.Append(new nkDictionayKey("0018,1147", "CS", "Field of View Shape"));
	my_Dictionary.Append(new nkDictionayKey("0018,1149", "IS", "Field of View Dimensions(s)"));
	my_Dictionary.Append(new nkDictionayKey("0018,1150", "IS", "Exposure Time"));
	my_Dictionary.Append(new nkDictionayKey("0018,1151", "IS", "X-ray Tube Current"));
	my_Dictionary.Append(new nkDictionayKey("0018,1152", "IS", "Exposure"));
	my_Dictionary.Append(new nkDictionayKey("0018,1160", "SH", "Filter Type"));
	my_Dictionary.Append(new nkDictionayKey("0018,1170", "IS", "Generator Power"));
	my_Dictionary.Append(new nkDictionayKey("0018,1180", "SH", "Collimator/grid Name"));
	my_Dictionary.Append(new nkDictionayKey("0018,1181", "CS", "Collimator Type"));
	my_Dictionary.Append(new nkDictionayKey("0018,1182", "IS", "Focal Distance"));
	my_Dictionary.Append(new nkDictionayKey("0018,1183", "DS", "X Focus Center"));
	my_Dictionary.Append(new nkDictionayKey("0018,1184", "DS", "Y Focus Center"));
	my_Dictionary.Append(new nkDictionayKey("0018,1190", "DS", "Focal Spot(s)"));
	my_Dictionary.Append(new nkDictionayKey("0018,1200", "DA", "Date of Last Calibration"));
	my_Dictionary.Append(new nkDictionayKey("0018,1201", "TM", "Time of Last Calibration"));
	my_Dictionary.Append(new nkDictionayKey("0018,1210", "SH", "Convolution Kernel"));
	my_Dictionary.Append(new nkDictionayKey("0018,1240", "DS", "Upper/Lower Pixel Values (RET)"));
	my_Dictionary.Append(new nkDictionayKey("0018,1242", "IS", "Actual Frame Duration"));
	my_Dictionary.Append(new nkDictionayKey("0018,1243", "IS", "Count Rate"));
	my_Dictionary.Append(new nkDictionayKey("0018,1250", "SH", "Receiving Coil"));
	my_Dictionary.Append(new nkDictionayKey("0018,1151", "SH", "Transmitting Coil"));
	my_Dictionary.Append(new nkDictionayKey("0018,1160", "SH", "Screen Type"));
	my_Dictionary.Append(new nkDictionayKey("0018,1261", "LO", "Phosphor Type"));
	my_Dictionary.Append(new nkDictionayKey("0018,1300", "IS", "Scan Velocity"));
	my_Dictionary.Append(new nkDictionayKey("0018,1301", "CS", "Whole Body Technique"));
	my_Dictionary.Append(new nkDictionayKey("0018,1302", "IS", "Scan Length"));
	my_Dictionary.Append(new nkDictionayKey("0018,1310", "US", "Acquisition Matrix"));
	my_Dictionary.Append(new nkDictionayKey("0018,1312", "CS", "Phase Encoding Direction"));
	my_Dictionary.Append(new nkDictionayKey("0018,1314", "DS", "Flip Angle"));
	my_Dictionary.Append(new nkDictionayKey("0018,1315", "CS", "Variable Flip Angle Flag"));
	my_Dictionary.Append(new nkDictionayKey("0018,1316", "DS", "SAR"));
	my_Dictionary.Append(new nkDictionayKey("0018,1318", "DS", "dB/dt"));
	my_Dictionary.Append(new nkDictionayKey("0018,1400", "LO", "Acquisition Device Processing Description"));
	my_Dictionary.Append(new nkDictionayKey("0018,1401", "LO", "Acquisition Device Processing Code"));
	my_Dictionary.Append(new nkDictionayKey("0018,1402", "CS", "Cassette Orientation"));
	my_Dictionary.Append(new nkDictionayKey("0018,1403", "CS", "Cassette Size"));
	my_Dictionary.Append(new nkDictionayKey("0018,1404", "US", "Exposures on Plate"));
	my_Dictionary.Append(new nkDictionayKey("0018,1405", "IS", "Relative X-ray Exposure"));
	my_Dictionary.Append(new nkDictionayKey("0018,4000", "SH", "Group 0018 Comments (RET)"));
	my_Dictionary.Append(new nkDictionayKey("0018,5000", "SH", "Output Power"));
	my_Dictionary.Append(new nkDictionayKey("0018,5010", "LO", "Transducer Data"));
	my_Dictionary.Append(new nkDictionayKey("0018,5012", "DS", "Focus Depth"));
	my_Dictionary.Append(new nkDictionayKey("0018,5020", "LO", "Preprocessing Function"));
	my_Dictionary.Append(new nkDictionayKey("0018,5021", "LO", "Postprocessing Function"));
	my_Dictionary.Append(new nkDictionayKey("0018,5022", "DS", "Mechanical Index"));
	my_Dictionary.Append(new nkDictionayKey("0018,5024", "DS", "Thermal Index"));
	my_Dictionary.Append(new nkDictionayKey("0018,5026", "DS", "Cranial Thermal Index"));
	my_Dictionary.Append(new nkDictionayKey("0018,5027", "DS", "Soft Tissue Thermal Index"));
	my_Dictionary.Append(new nkDictionayKey("0018,5028", "DS", "Soft Tissue-focus Thermal Index"));
	my_Dictionary.Append(new nkDictionayKey("0018,5029", "DS", "Soft Tissue-surface Thermal Index"));
	my_Dictionary.Append(new nkDictionayKey("0018,5030", "IS", "Dynamic Range (RET)"));
	my_Dictionary.Append(new nkDictionayKey("0018,5040", "IS", "Total Gain (RET)"));
	my_Dictionary.Append(new nkDictionayKey("0018,5050", "IS", "Depth of Scan Field"));
	my_Dictionary.Append(new nkDictionayKey("0018,5100", "CS", "Patient Position"));
	my_Dictionary.Append(new nkDictionayKey("0018,5101", "CS", "View Position"));
	my_Dictionary.Append(new nkDictionayKey("0018,5210", "DS", "Image Transformation Matrix"));
	my_Dictionary.Append(new nkDictionayKey("0018,5212", "DS", "Image Translation Vector"));
	my_Dictionary.Append(new nkDictionayKey("0018,6000", "DS", "Sensitivity"));
	my_Dictionary.Append(new nkDictionayKey("0018,6011", "SQ", "Sequence of Ultrasound Regions"));
	my_Dictionary.Append(new nkDictionayKey("0018,6012", "US", "Region Spatial Format"));
	my_Dictionary.Append(new nkDictionayKey("0018,6014", "US", "Region Data Type"));
	my_Dictionary.Append(new nkDictionayKey("0018,6016", "UL", "Region Flags"));
	my_Dictionary.Append(new nkDictionayKey("0018,6018", "UL", "Region Location Min X0"));
	my_Dictionary.Append(new nkDictionayKey("0018,601A", "UL", "Region Location Min Y0"));
	my_Dictionary.Append(new nkDictionayKey("0018,601C", "UL", "Region Location Max X1"));
	my_Dictionary.Append(new nkDictionayKey("0018,601E", "UL", "Region Location Max Y1"));
	my_Dictionary.Append(new nkDictionayKey("0018,6020", "SL", "Reference Pixel X0"));
	my_Dictionary.Append(new nkDictionayKey("0018,6022", "SL", "Reference Pixel Y0"));
	my_Dictionary.Append(new nkDictionayKey("0018,6024", "US", "Physical Units X Direction"));
	my_Dictionary.Append(new nkDictionayKey("0018,6026", "US", "Physical Units Y Direction"));
	my_Dictionary.Append(new nkDictionayKey("0018,1628", "FD", "Reference Pixel Physical Value X"));
	my_Dictionary.Append(new nkDictionayKey("0018,602A", "FD", "Reference Pixel Physical Value Y"));
	my_Dictionary.Append(new nkDictionayKey("0018,602C", "FD", "Physical Delta X"));
	my_Dictionary.Append(new nkDictionayKey("0018,602E", "FD", "Physical Delta Y"));
	my_Dictionary.Append(new nkDictionayKey("0018,6030", "UL", "Transducer Frequency"));
	my_Dictionary.Append(new nkDictionayKey("0018,6031", "CS", "Transducer Type"));
	my_Dictionary.Append(new nkDictionayKey("0018,6032", "UL", "Pulse Repetition Frequency"));
	my_Dictionary.Append(new nkDictionayKey("0018,6034", "FD", "Doppler Correction Angle"));
	my_Dictionary.Append(new nkDictionayKey("0018,6036", "FD", "Sterring Angle"));
	my_Dictionary.Append(new nkDictionayKey("0018,6038", "UL", "Doppler Sample Volume X Position"));
	my_Dictionary.Append(new nkDictionayKey("0018,603A", "UL", "Doppler Sample Volume Y Position"));
	my_Dictionary.Append(new nkDictionayKey("0018,603C", "UL", "TM-Line Position X0"));
	my_Dictionary.Append(new nkDictionayKey("0018,603E", "UL", "TM-Line Position Y0"));
	my_Dictionary.Append(new nkDictionayKey("0018,6040", "UL", "TM-Line Position X1"));
	my_Dictionary.Append(new nkDictionayKey("0018,6042", "UL", "TM-Line Position Y1"));
	my_Dictionary.Append(new nkDictionayKey("0018,6044", "US", "Pixel Component Organization"));
	my_Dictionary.Append(new nkDictionayKey("0018,6046", "UL", "Pixel Component Organization"));
	my_Dictionary.Append(new nkDictionayKey("0018,6048", "UL", "Pixel Component Range Start"));
	my_Dictionary.Append(new nkDictionayKey("0018,604A", "UL", "Pixel Component Range Stop"));
	my_Dictionary.Append(new nkDictionayKey("0018,604C", "US", "Pixel Component Physical Units"));
	my_Dictionary.Append(new nkDictionayKey("0018,604E", "US", "Pixel Component Data Type"));
	my_Dictionary.Append(new nkDictionayKey("0018,6050", "UL", "Number of Table Break Points"));
	my_Dictionary.Append(new nkDictionayKey("0018,6052", "UL", "Table of X Break Points"));
	my_Dictionary.Append(new nkDictionayKey("0018,6054", "FD", "Table of Y Break Points"));
	my_Dictionary.Append(new nkDictionayKey("0020,0000", "UL", "Group 0020 Length"));
	my_Dictionary.Append(new nkDictionayKey("0020,000D", "UI", "Study Instance UID"));
	my_Dictionary.Append(new nkDictionayKey("0020,000E", "UI", "Series Instance UID"));
	my_Dictionary.Append(new nkDictionayKey("0020,0010", "SH", "Study ID"));
	my_Dictionary.Append(new nkDictionayKey("0020,0011", "IS", "Series Number"));
	my_Dictionary.Append(new nkDictionayKey("0020,0012", "IS", "Acquisition Number"));
	my_Dictionary.Append(new nkDictionayKey("0020,0013", "IS", "Image Number"));
	my_Dictionary.Append(new nkDictionayKey("0020,0014", "IS", "Isotope Number"));
	my_Dictionary.Append(new nkDictionayKey("0020,0015", "IS", "Phase Number"));
	my_Dictionary.Append(new nkDictionayKey("0020,0016", "IS", "Interval Number"));
	my_Dictionary.Append(new nkDictionayKey("0020,0017", "IS", "Time Slot Number"));
	my_Dictionary.Append(new nkDictionayKey("0020,0018", "IS", "Angle Number"));
	my_Dictionary.Append(new nkDictionayKey("0020,0020", "CS", "Patient Orientation"));
	my_Dictionary.Append(new nkDictionayKey("0020,0022", "US", "Overlay Number"));
	my_Dictionary.Append(new nkDictionayKey("0020,0024", "US", "Curve Number"));
	my_Dictionary.Append(new nkDictionayKey("0020,0030", "DS", "Image Position (RET)"));
	my_Dictionary.Append(new nkDictionayKey("0020,0032", "DS", "Image Position (Patient)"));
	my_Dictionary.Append(new nkDictionayKey("0020,0035", "DS", "Image Orientation (RET)"));
	my_Dictionary.Append(new nkDictionayKey("0020,0037", "DS", "Image Orientation (Patient)"));
	my_Dictionary.Append(new nkDictionayKey("0020,0050", "DS", "Location (RET)"));
	my_Dictionary.Append(new nkDictionayKey("0020,0052", "UI", "Frame of Reference UID"));
	my_Dictionary.Append(new nkDictionayKey("0020,0060", "CS", "Laterality"));
	my_Dictionary.Append(new nkDictionayKey("0020,0070", "SH", "Image Geometry Type (RET)"));
	my_Dictionary.Append(new nkDictionayKey("0020,0080", "UI", "Masking Image UID"));
	my_Dictionary.Append(new nkDictionayKey("0020,0100", "IS", "Temporal Position Identifier"));
	my_Dictionary.Append(new nkDictionayKey("0020,0105", "IS", "Number of Temporal Positions"));
	my_Dictionary.Append(new nkDictionayKey("0020,0110", "DS", "Temporal Resolution"));
	my_Dictionary.Append(new nkDictionayKey("0020,1000", "IS", "Series in Study"));
	my_Dictionary.Append(new nkDictionayKey("0020,1001", "IS", "Acquisitions in Series (RET)"));
	my_Dictionary.Append(new nkDictionayKey("0020,1002", "IS", "Images in Acquisition"));
	my_Dictionary.Append(new nkDictionayKey("0020,1004", "IS", "Acquisition in Study"));
	my_Dictionary.Append(new nkDictionayKey("0020,1020", "SH", "Reference (RET)"));
	my_Dictionary.Append(new nkDictionayKey("0020,1040", "LO", "Position Reference Indicator"));
	my_Dictionary.Append(new nkDictionayKey("0020,1041", "DS", "Slice Location"));
	my_Dictionary.Append(new nkDictionayKey("0020,1070", "IS", "Other Study Numbers"));
	my_Dictionary.Append(new nkDictionayKey("0020,1200", "IS", "Number of Patient Related Studies"));
	my_Dictionary.Append(new nkDictionayKey("0020,1202", "IS", "Number of Patient Related Series"));
	my_Dictionary.Append(new nkDictionayKey("0020,1204", "IS", "Number of Patient Related Images"));
	my_Dictionary.Append(new nkDictionayKey("0020,1206", "IS", "Number of Study Related Series"));
	my_Dictionary.Append(new nkDictionayKey("0020,1208", "IS", "Number of Study Related Images"));
	my_Dictionary.Append(new nkDictionayKey("0020,3100", "SH", "Source Image ID (RET)s"));
	my_Dictionary.Append(new nkDictionayKey("0020,3401", "SH", "Modifying Device ID (RET)"));
	my_Dictionary.Append(new nkDictionayKey("0020,3402", "SH", "Modified Image ID (RET)"));
	my_Dictionary.Append(new nkDictionayKey("0020,3403", "SH", "Modified Image Date (RET)"));
	my_Dictionary.Append(new nkDictionayKey("0020,3404", "SH", "Modifying Device Manufacturer (RET)"));
	my_Dictionary.Append(new nkDictionayKey("0020,3405", "SH", "Modified Image Time (RET)"));
	my_Dictionary.Append(new nkDictionayKey("0020,3406", "SH", "Modified Image Description (RET)"));
	my_Dictionary.Append(new nkDictionayKey("0020,4000", "LT", "Image Comments"));
	my_Dictionary.Append(new nkDictionayKey("0020,5000", "US", "Original Image Identification (RET)"));
	my_Dictionary.Append(new nkDictionayKey("0020,5002", "SH", "Original Image Identification Nomenclature (RET)"));
	my_Dictionary.Append(new nkDictionayKey("0028,0000", "UL", "Group 0028 Length"));
	my_Dictionary.Append(new nkDictionayKey("0028,0002", "US", "Samples per Pixel"));
	my_Dictionary.Append(new nkDictionayKey("0028,0004", "CS", "Photometric Interpretation"));
	my_Dictionary.Append(new nkDictionayKey("0028,0005", "US", "Image Dimensions (RET)"));
	my_Dictionary.Append(new nkDictionayKey("0028,0006", "US", "Planar Configuration"));
	my_Dictionary.Append(new nkDictionayKey("0028,0008", "IS", "Number of Frames"));
	my_Dictionary.Append(new nkDictionayKey("0028,0009", "AT", "Frame Increment Pointer"));
	my_Dictionary.Append(new nkDictionayKey("0028,0010", "US", "Rows"));
	my_Dictionary.Append(new nkDictionayKey("0028,0011", "US", "Columns"));
	my_Dictionary.Append(new nkDictionayKey("0028,0030", "DS", "Pixel Spacing"));
	my_Dictionary.Append(new nkDictionayKey("0028,0031", "DS", "Zoom Factor"));
	my_Dictionary.Append(new nkDictionayKey("0028,0032", "DS", "Zoom Center"));
	my_Dictionary.Append(new nkDictionayKey("0028,0034", "IS", "Pixel Aspect Ratio"));
	my_Dictionary.Append(new nkDictionayKey("0028,0040", "SH", "Image Format (RET)"));
	my_Dictionary.Append(new nkDictionayKey("0028,0050", "SH", "Manipulated Image (RET)"));
	my_Dictionary.Append(new nkDictionayKey("0028,0051", "CS", "Corrected Image"));
	my_Dictionary.Append(new nkDictionayKey("0028,0060", "SH", "Compression Code (RET)"));
	my_Dictionary.Append(new nkDictionayKey("0028,0100", "US", "Bits Allocated"));
	my_Dictionary.Append(new nkDictionayKey("0028,0101", "US", "Bits Stored"));
	my_Dictionary.Append(new nkDictionayKey("0028,0102", "US", "High Bit"));
	my_Dictionary.Append(new nkDictionayKey("0028,0103", "US", "Pixel Representation"));
	my_Dictionary.Append(new nkDictionayKey("0028,0104", "US", "Smallest Valid Pixel Value (RET)"));
	my_Dictionary.Append(new nkDictionayKey("0028,0105", "US", "Largest Valid Pixel Value (RET)"));
	my_Dictionary.Append(new nkDictionayKey("0028,0106", "US", "Smallest Image Pixel Value"));
	my_Dictionary.Append(new nkDictionayKey("0028,0107", "US", "Largest Image Pixel Value"));
	my_Dictionary.Append(new nkDictionayKey("0028,0108", "US", "Smallest Pixel Value in Series"));
	my_Dictionary.Append(new nkDictionayKey("0028,0109", "US", "Largest Pixel Value in Series"));
	my_Dictionary.Append(new nkDictionayKey("0028,0120", "US", "Pixel Padding Value"));
	my_Dictionary.Append(new nkDictionayKey("0028,0200", "US", "Image Location (RET)"));
	my_Dictionary.Append(new nkDictionayKey("0028,1050", "DS", "Window Center"));
	my_Dictionary.Append(new nkDictionayKey("0028,1051", "DS", "Window Width"));
	my_Dictionary.Append(new nkDictionayKey("0028,1052", "DS", "Rescale Intercept"));
	my_Dictionary.Append(new nkDictionayKey("0028,1053", "DS", "Rescale Slope"));
	my_Dictionary.Append(new nkDictionayKey("0028,1054", "LO", "Rescale Type"));
	my_Dictionary.Append(new nkDictionayKey("0028,1055", "LO", "Window Center & Width Explanation"));
	my_Dictionary.Append(new nkDictionayKey("0028,1080", "SH", "Gray Scale (RET)"));
	my_Dictionary.Append(new nkDictionayKey("0028,1100", "US", "Gray Lookup Table Descriptor (RET)"));
	my_Dictionary.Append(new nkDictionayKey("0028,1101", "US", "Red Palette Color Lookup Table Descriptor"));
	my_Dictionary.Append(new nkDictionayKey("0028,1102", "US", "Green Palette Color Lookup Table Descriptor"));
	my_Dictionary.Append(new nkDictionayKey("0028,1103", "US", "Blue Palette Color Lookup Table Descriptor"));
	my_Dictionary.Append(new nkDictionayKey("0028,1200", "US", "Gray Lookup Table Data (RET)"));
	my_Dictionary.Append(new nkDictionayKey("0028,1201", "US", "Red Palette Color Lookup Table Data"));
	my_Dictionary.Append(new nkDictionayKey("0028,1202", "US", "Green Palette Color Lookup Table Data"));
	my_Dictionary.Append(new nkDictionayKey("0028,1203", "US", "Blue Palette Color Lookup Table Data"));
	my_Dictionary.Append(new nkDictionayKey("0028,3000", "SQ", "Modality LUT Sequence"));
	my_Dictionary.Append(new nkDictionayKey("0028,3002", "US", "LUT Descriptor"));
	my_Dictionary.Append(new nkDictionayKey("0028,3003", "LO", "LUT Explanation"));
	my_Dictionary.Append(new nkDictionayKey("0028,3004", "LO", "Madality LUT Type"));
	my_Dictionary.Append(new nkDictionayKey("0028,3006", "US", "LUT Data"));
	my_Dictionary.Append(new nkDictionayKey("0028,3010", "SQ", "VOI LUT Sequence"));
	my_Dictionary.Append(new nkDictionayKey("0028,4000", "SH", "Group 0028 Comments (RET)"));
	my_Dictionary.Append(new nkDictionayKey("0032,0000", "UL", "Group 0032 Length"));
	my_Dictionary.Append(new nkDictionayKey("0032,000A", "CS", "Study Status ID"));
	my_Dictionary.Append(new nkDictionayKey("0032,000C", "CS", "Study Priority ID"));
	my_Dictionary.Append(new nkDictionayKey("0032,0012", "LO", "Study ID Issuer"));
	my_Dictionary.Append(new nkDictionayKey("0032,0032", "DA", "Study Verified Date"));
	my_Dictionary.Append(new nkDictionayKey("0032,0033", "TM", "Study Verified Time"));
	my_Dictionary.Append(new nkDictionayKey("0032,0034", "DA", "Study Read Date"));
	my_Dictionary.Append(new nkDictionayKey("0032,0035", "TM", "Study Read Time"));
	my_Dictionary.Append(new nkDictionayKey("0032,1000", "DA", "Scheduled Study Start Date"));
	my_Dictionary.Append(new nkDictionayKey("0032,1001", "TM", "Scheduled Study Start Time"));
	my_Dictionary.Append(new nkDictionayKey("0032,1010", "DA", "Scheduled Study Stop Date"));
	my_Dictionary.Append(new nkDictionayKey("0032,1011", "TM", "Scheduled Study Stop Time"));
	my_Dictionary.Append(new nkDictionayKey("0032,1020", "LO", "Scheduled Study Location"));
	my_Dictionary.Append(new nkDictionayKey("0032,1021", "AE", "Scheduled Study Location AE Title(s)"));
	my_Dictionary.Append(new nkDictionayKey("0032,1030", "LO", "Reason  for Study"));
	my_Dictionary.Append(new nkDictionayKey("0032,1032", "PN", "Requesting Physician"));
	my_Dictionary.Append(new nkDictionayKey("0032,1033", "LO", "Requesting Service"));
	my_Dictionary.Append(new nkDictionayKey("0032,1040", "DA", "Study Arrival Date"));
	my_Dictionary.Append(new nkDictionayKey("0032,1041", "TM", "Study Arrival Time"));
	my_Dictionary.Append(new nkDictionayKey("0032,1050", "DA", "Study Completion Date"));
	my_Dictionary.Append(new nkDictionayKey("0032,1051", "TM", "Study Completion Time"));
	my_Dictionary.Append(new nkDictionayKey("0032,1055", "CS", "Study Component Status ID"));
	my_Dictionary.Append(new nkDictionayKey("0032,1060", "LO", "Requested Procedure Description"));
	my_Dictionary.Append(new nkDictionayKey("0032,1064", "SQ", "Requested Procedure Code Sequence"));
	my_Dictionary.Append(new nkDictionayKey("0032,1070", "LO", "Requested Contrast Agent"));
	my_Dictionary.Append(new nkDictionayKey("0032,4000", "LT", "Study Comments"));
	my_Dictionary.Append(new nkDictionayKey("0038,0000", "UL", "Group 0038 Length"));
	my_Dictionary.Append(new nkDictionayKey("0038,0004", "SQ", "Referenced Patient Alias Sequence"));
	my_Dictionary.Append(new nkDictionayKey("0038,0008", "CS", "Visit Status ID"));
	my_Dictionary.Append(new nkDictionayKey("0038,0010", "LO", "Admissin ID"));
	my_Dictionary.Append(new nkDictionayKey("0038,0011", "LO", "Issuer of Admission ID"));
	my_Dictionary.Append(new nkDictionayKey("0038,0016", "LO", "Route of Admissions"));
	my_Dictionary.Append(new nkDictionayKey("0038,001A", "DA", "Scheduled Admissin Date"));
	my_Dictionary.Append(new nkDictionayKey("0038,001B", "TM", "Scheduled Adission Time"));
	my_Dictionary.Append(new nkDictionayKey("0038,001C", "DA", "Scheduled Discharge Date"));
	my_Dictionary.Append(new nkDictionayKey("0038,001D", "TM", "Scheduled Discharge Time"));
	my_Dictionary.Append(new nkDictionayKey("0038,001E", "LO", "Scheduled Patient Institution Residence"));
	my_Dictionary.Append(new nkDictionayKey("0038,0020", "DA", "Admitting Date"));
	my_Dictionary.Append(new nkDictionayKey("0038,0021", "TM", "Admitting Time"));
	my_Dictionary.Append(new nkDictionayKey("0038,0030", "DA", "Discharge Date"));
	my_Dictionary.Append(new nkDictionayKey("0038,0032", "TM", "Discharge Time"));
	my_Dictionary.Append(new nkDictionayKey("0038,0040", "LO", "Discharge Diagnosis Description"));
	my_Dictionary.Append(new nkDictionayKey("0038,0044", "SQ", "Discharge Diagnosis Code Sequence"));
	my_Dictionary.Append(new nkDictionayKey("0038,0050", "LO", "Special Needs"));
	my_Dictionary.Append(new nkDictionayKey("0038,0300", "LO", "Current Patient Location"));
	my_Dictionary.Append(new nkDictionayKey("0038,0400", "LO", "Patient's Institution Residence"));
	my_Dictionary.Append(new nkDictionayKey("0038,0500", "LO", "Patient State"));
	my_Dictionary.Append(new nkDictionayKey("0038,4000", "LT", "Visit Comments"));
	my_Dictionary.Append(new nkDictionayKey("0088,0000", "UL", "Group 0088 Length"));
	my_Dictionary.Append(new nkDictionayKey("0088,0130", "SH", "Storage Media File-set ID"));
	my_Dictionary.Append(new nkDictionayKey("0088,0140", "UI", "Storage Media File-set UID"));
	my_Dictionary.Append(new nkDictionayKey("2000,0000", "UL", "Group 2000 Length"));
	my_Dictionary.Append(new nkDictionayKey("2000,0010", "IS", "Number of Copies"));
	my_Dictionary.Append(new nkDictionayKey("2000,0020", "CS", "Print Priority"));
	my_Dictionary.Append(new nkDictionayKey("2000,0030", "CS", "Medium Type"));
	my_Dictionary.Append(new nkDictionayKey("2000,0040", "CS", "Film Destination"));
	my_Dictionary.Append(new nkDictionayKey("2000,0050", "LO", "Film Session Label"));
	my_Dictionary.Append(new nkDictionayKey("2000,0060", "IS", "Memory Allocation"));
	my_Dictionary.Append(new nkDictionayKey("2000,0500", "SQ", "Referenced Film Box Sequence"));
	my_Dictionary.Append(new nkDictionayKey("2010,0000", "UL", "Group 2010 Length"));
	my_Dictionary.Append(new nkDictionayKey("2010,0010", "ST", "Image Display Format"));
	my_Dictionary.Append(new nkDictionayKey("2010,0030", "CS", "Annotation Display Format ID"));
	my_Dictionary.Append(new nkDictionayKey("2010,0040", "CS", "Film Orientation"));
	my_Dictionary.Append(new nkDictionayKey("2010,0050", "CS", "Film Size ID"));
	my_Dictionary.Append(new nkDictionayKey("2010,0060", "CS", "Magnification Type"));
	my_Dictionary.Append(new nkDictionayKey("2010,0080", "CS", "Smoothing Type"));
	my_Dictionary.Append(new nkDictionayKey("2010,0100", "CS", "Border Density"));
	my_Dictionary.Append(new nkDictionayKey("2010,0110", "CS", "Empty Image Density"));
	my_Dictionary.Append(new nkDictionayKey("2010,0120", "US", "Min Density"));
	my_Dictionary.Append(new nkDictionayKey("2010,0130", "US", "Max Density"));
	my_Dictionary.Append(new nkDictionayKey("2010,0140", "CS", "Trim"));
	my_Dictionary.Append(new nkDictionayKey("2010,0150", "ST", "Configuration Information"));
	my_Dictionary.Append(new nkDictionayKey("2010,0500", "SQ", "Referenced Film Session Sequence"));
	my_Dictionary.Append(new nkDictionayKey("2010,0510", "SQ", "Referenced Basic Image Box Sequence"));
	my_Dictionary.Append(new nkDictionayKey("2010,0520", "SQ", "Referenced Basic Annotation Box Sequence"));
	my_Dictionary.Append(new nkDictionayKey("2020,0000", "UL", "Group 2020 Length"));
	my_Dictionary.Append(new nkDictionayKey("2020,0010", "US", "Image Position"));
	my_Dictionary.Append(new nkDictionayKey("2020,0020", "CS", "Polarity"));
	my_Dictionary.Append(new nkDictionayKey("2020,0030", "DS", "Requested Image Size"));
	my_Dictionary.Append(new nkDictionayKey("2020,0110", "SQ", "Preformatted Greyscale Image Sequence"));
	my_Dictionary.Append(new nkDictionayKey("2020,0111", "SQ", "Preformatted Color Image Sequence"));
	my_Dictionary.Append(new nkDictionayKey("2020,0130", "SQ", "Referenced Image Overlay Box Sequence"));
	my_Dictionary.Append(new nkDictionayKey("2020,0140", "SQ", "Referenced VOI LUT Sequence"));
	my_Dictionary.Append(new nkDictionayKey("2030,0000", "UL", "Group 2030 Length"));
	my_Dictionary.Append(new nkDictionayKey("2030,0010", "US", "Annotation Position"));
	my_Dictionary.Append(new nkDictionayKey("2030,0020", "LO", "Text String"));
	my_Dictionary.Append(new nkDictionayKey("2040,0000", "UL", "Group 2040 Length"));
	my_Dictionary.Append(new nkDictionayKey("2040,0010", "SQ", "Referenced Overlay Plane Sequence"));
	my_Dictionary.Append(new nkDictionayKey("2040,0011", "US", "Refenced Overlay Plane Groups"));
	my_Dictionary.Append(new nkDictionayKey("2040,0060", "CS", "Overlay Magnification Type"));
	my_Dictionary.Append(new nkDictionayKey("2040,0070", "CS", "Overlay Smoothing Type"));
	my_Dictionary.Append(new nkDictionayKey("2040,0080", "CS", "Overlay Foreground Density"));
	my_Dictionary.Append(new nkDictionayKey("2040,0090", "CS", "overlay Mode"));
	my_Dictionary.Append(new nkDictionayKey("2040,0100", "CS", "Threshold Density"));
	my_Dictionary.Append(new nkDictionayKey("2040,0500", "SQ", "Referenced Image Box Sequence"));
	my_Dictionary.Append(new nkDictionayKey("2100,0000", "UL", "Group 2100 Length"));
	my_Dictionary.Append(new nkDictionayKey("2100,0020", "CS", "Execution Status"));
	my_Dictionary.Append(new nkDictionayKey("2100,0030", "CS", "Execution Status Info"));
	my_Dictionary.Append(new nkDictionayKey("2100,0040", "DA", "Creation Date"));
	my_Dictionary.Append(new nkDictionayKey("2100,0050", "TM", "Creation Time"));
	my_Dictionary.Append(new nkDictionayKey("2100,0070", "AE", "Originator"));
	my_Dictionary.Append(new nkDictionayKey("2100,0500", "SQ", "Referenced Print Job Sequence"));
	my_Dictionary.Append(new nkDictionayKey("2110,0000", "UL", "Group 2110 Length"));
	my_Dictionary.Append(new nkDictionayKey("2110,0010", "CS", "Printer Status"));
	my_Dictionary.Append(new nkDictionayKey("2110,0020", "CS", "Printer Status Info"));
	my_Dictionary.Append(new nkDictionayKey("2110,0030", "ST", "Printer Name"));
	my_Dictionary.Append(new nkDictionayKey("4000,0000", "UL", "Group 4000 Length (RET)"));
	my_Dictionary.Append(new nkDictionayKey("4000,0010", "SH", "Arbitray (RET)"));
	my_Dictionary.Append(new nkDictionayKey("4000,4000", "LT", "Group 4000 Comments (RET)"));
	my_Dictionary.Append(new nkDictionayKey("4008,0000", "UL", "Group 4008 Length"));
	my_Dictionary.Append(new nkDictionayKey("4008,0040", "SH", "Results ID"));
	my_Dictionary.Append(new nkDictionayKey("4008,0042", "LO", "Results ID Issuer"));
	my_Dictionary.Append(new nkDictionayKey("4008,0050", "SQ", "Referenced Interpretation Sequence"));
	my_Dictionary.Append(new nkDictionayKey("4008,0100", "DA", "Interpretation Recorded Date"));
	my_Dictionary.Append(new nkDictionayKey("4008,0101", "TM", "Interpretation Recorded Time"));
	my_Dictionary.Append(new nkDictionayKey("4008,0102", "PN", "Interpretation Recorder"));
	my_Dictionary.Append(new nkDictionayKey("4008,0103", "LO", "Reference to Recorded Sound"));
	my_Dictionary.Append(new nkDictionayKey("4008,0108", "DA", "Interpretation Transcription Time"));
	my_Dictionary.Append(new nkDictionayKey("4008,0109", "TM", "Interpretation Transcription Time"));
	my_Dictionary.Append(new nkDictionayKey("4008,010A", "PN", "Interpretation Transcriber"));
	my_Dictionary.Append(new nkDictionayKey("4008,010B", "ST", "Interpretation Text"));
	my_Dictionary.Append(new nkDictionayKey("4008,010C", "PN", "Interpretation Author"));
	my_Dictionary.Append(new nkDictionayKey("4008,0111", "SQ", "Interpretation Approver Sequence"));
	my_Dictionary.Append(new nkDictionayKey("4008,0112", "DA", "Interpretation Approval Date"));
	my_Dictionary.Append(new nkDictionayKey("4008,0113", "TM", "Interpretation Approval Time"));
	my_Dictionary.Append(new nkDictionayKey("4008,0114", "PN", "Physician Approving Interpretation"));
	my_Dictionary.Append(new nkDictionayKey("4008,0115", "LT", "Interpretation Diagnosis Description"));
	my_Dictionary.Append(new nkDictionayKey("4008,0117", "SQ", "Diagnosis Code Sequence"));
	my_Dictionary.Append(new nkDictionayKey("4008,0118", "SQ", "Results Distribution List Sequence"));
	my_Dictionary.Append(new nkDictionayKey("4008,0119", "PN", "Distribution Name"));
	my_Dictionary.Append(new nkDictionayKey("4008,011A", "LO", "Distribution Address"));
	my_Dictionary.Append(new nkDictionayKey("4008,0200", "SH", "Interpretation ID"));
	my_Dictionary.Append(new nkDictionayKey("4008,0202", "LO", "Interpretation ID Issuer"));
	my_Dictionary.Append(new nkDictionayKey("4008,0210", "CS", "Interpretation Type ID"));
	my_Dictionary.Append(new nkDictionayKey("4008,0212", "CS", "Interpretation Status ID"));
	my_Dictionary.Append(new nkDictionayKey("4008,0300", "ST", "Impression"));
	my_Dictionary.Append(new nkDictionayKey("4008,4000", "SH", "Group 4008 Comments"));
	my_Dictionary.Append(new nkDictionayKey("5000,0000", "UL", "Group 5000 Length"));
	my_Dictionary.Append(new nkDictionayKey("5000,0005", "US", "Curve Dimensions"));
	my_Dictionary.Append(new nkDictionayKey("5000,0010", "US", "Number of Points"));
	my_Dictionary.Append(new nkDictionayKey("5000,0020", "CS", "Type of Data"));
	my_Dictionary.Append(new nkDictionayKey("5000,0022", "LO", "Curve Description"));
	my_Dictionary.Append(new nkDictionayKey("5000,0030", "SH", "Axis Units"));
	my_Dictionary.Append(new nkDictionayKey("5000,0040", "SH", "Axis Labels"));
	my_Dictionary.Append(new nkDictionayKey("5000,0103", "US", "Data Value Representation"));
	my_Dictionary.Append(new nkDictionayKey("5000,0104", "US", "Minimum Coordinate Value"));
	my_Dictionary.Append(new nkDictionayKey("5000,0105", "US", "Maximum Coordinate Value"));
	my_Dictionary.Append(new nkDictionayKey("5000,0106", "SH", "Curve Range"));
	my_Dictionary.Append(new nkDictionayKey("5000,0110", "US", "Curve Data Descriptor"));
	my_Dictionary.Append(new nkDictionayKey("5000,0112", "US", "Coordinate Start Value"));
	my_Dictionary.Append(new nkDictionayKey("5000,0114", "US", "Coordinate Step Value"));
	my_Dictionary.Append(new nkDictionayKey("5000,2000", "US", "Audio Type"));
	my_Dictionary.Append(new nkDictionayKey("5000,2002", "US", "Audio Sample Format"));
	my_Dictionary.Append(new nkDictionayKey("5000,2004", "US", "Number of Channels"));
	my_Dictionary.Append(new nkDictionayKey("5000,2006", "UL", "Number of Samples"));
	my_Dictionary.Append(new nkDictionayKey("5000,2008", "UL", "Sample Rate"));
	my_Dictionary.Append(new nkDictionayKey("5000,200A", "UL", "Total Time"));
	my_Dictionary.Append(new nkDictionayKey("5000,200C", "OX", "Audio Sample Data"));
	my_Dictionary.Append(new nkDictionayKey("5000,200E", "LT", "Audio Comments"));
	my_Dictionary.Append(new nkDictionayKey("5000,3000", "OX", "Curve Data"));
	my_Dictionary.Append(new nkDictionayKey("6000,0000", "UL", "Group 6000 Length"));
	my_Dictionary.Append(new nkDictionayKey("6000,0010", "US", "Rows"));
	my_Dictionary.Append(new nkDictionayKey("6000,0011", "US", "Columns"));
	my_Dictionary.Append(new nkDictionayKey("6000,0015", "IS", "Number of Frames in Overlay"));
	my_Dictionary.Append(new nkDictionayKey("6000,0040", "CS", "Overlay Type"));
	my_Dictionary.Append(new nkDictionayKey("6000,0050", "SS", "Origin"));
	my_Dictionary.Append(new nkDictionayKey("6000,0060", "SH", "Compression Code (RET)"));
	my_Dictionary.Append(new nkDictionayKey("6000,0100", "US", "Bits Allocated"));
	my_Dictionary.Append(new nkDictionayKey("6000,0102", "US", "Bit Position"));
	my_Dictionary.Append(new nkDictionayKey("6000,0110", "SH", "Overlay Format (RET)"));
	my_Dictionary.Append(new nkDictionayKey("6000,0200", "US", "Overlay Location (RET)"));
	my_Dictionary.Append(new nkDictionayKey("6000,1100", "US", "Overlay Descriptor - Gray"));
	my_Dictionary.Append(new nkDictionayKey("6000,1101", "US", "Overlay Descriptor - Red"));
	my_Dictionary.Append(new nkDictionayKey("6000,1102", "US", "Overlay Descriptor - Green"));
	my_Dictionary.Append(new nkDictionayKey("6000,1103", "US", "Overlay Descriptor - Blue"));
	my_Dictionary.Append(new nkDictionayKey("6000,1200", "US", "Overlays - Gray"));
	my_Dictionary.Append(new nkDictionayKey("6000,1201", "US", "Overlays - Red"));
	my_Dictionary.Append(new nkDictionayKey("6000,1202", "US", "Overlays - Green"));
	my_Dictionary.Append(new nkDictionayKey("6000,1203", "US", "Overlays - Blue"));
	my_Dictionary.Append(new nkDictionayKey("6000,1301", "IS", "ROI Area"));
	my_Dictionary.Append(new nkDictionayKey("6000,1302", "DS", "ROI Mean"));
	my_Dictionary.Append(new nkDictionayKey("6000,1303", "DS", "ROI Standard Deviation"));
	my_Dictionary.Append(new nkDictionayKey("6000,3000", "OW", "Overlay Data"));
	my_Dictionary.Append(new nkDictionayKey("6000,4000", "SH", "Group 6000 Comments (RET)"));
	my_Dictionary.Append(new nkDictionayKey("7FE0,0000", "UL", "Group 7FE0 Length"));
	my_Dictionary.Append(new nkDictionayKey("7FE0,0010", "OX", "Pixel Data"));
	my_Dictionary.Append(new nkDictionayKey("FFFE,E000", "DL", "Item"));
	my_Dictionary.Append(new nkDictionayKey("FFFE,E00D", "DL", "Item Delimitation Item"));
	my_Dictionary.Append(new nkDictionayKey("FFFE,E0DD", "DL", "Sequence Delimitation Item"));
}

nkDICOMDictionary::~nkDICOMDictionary(){
	my_Dictionary.DeleteContents(true);
	_instance = NULL;
}

nkDictionayKey * nkDICOMDictionary::getKey(wxString a_Tag){
	nkListDictionayKey::compatibility_iterator node = my_Dictionary.GetFirst();
	nkDictionayKey * result = NULL;
	bool not_founf = true;
    while (node && not_founf){
		nkDictionayKey *current = node->GetData();
		if (current->getTag().CompareTo(a_Tag) == 0){
			not_founf = false;
			result = current;
		}   
        node = node->GetNext();
    }
	return result;
}



const wxString nkDictionayKey::nkVR_AE_ApplicationEntity = "AE";		//! ApplicationEntity
const wxString nkDictionayKey::nkVR_AS_AgeString = "AS";				//! AgeString
const wxString nkDictionayKey::nkVR_AT_AttributeTag = "AT";			//! AttributeTag
const wxString nkDictionayKey::nkVR_CS_CodeString = "CS";			//! CodeString
const wxString nkDictionayKey::nkVR_DA_Date = "DA";					//! Date
const wxString nkDictionayKey::nkVR_DS_DecimalString = "DS";			//! DecimalString
const wxString nkDictionayKey::nkVR_DT_DateTime = "DT";				//! DateTime
const wxString nkDictionayKey::nkVR_FL_FloatingPointSingle = "FL";	//! FloatingPointSingle
const wxString nkDictionayKey::nkVR_FD_FloatingPointDouble = "FD";	//! FloatingPointDouble
const wxString nkDictionayKey::nkVR_IS_IntegerString = "IS";			//! IntegerString
const wxString nkDictionayKey::nkVR_LO_LongString = "LO";			//! LongString
const wxString nkDictionayKey::nkVR_LT_LongText = "LT";				//! LongText
const wxString nkDictionayKey::nkVR_OB_OtherByteString = "OB";		//! OtherByteString
const wxString nkDictionayKey::nkVR_OF_OtherFloatString = "OF";		//! OtherFloatString
const wxString nkDictionayKey::nkVR_OW_OtherWordString = "OW";		//! OtherWordString
const wxString nkDictionayKey::nkVR_PN_PersonName = "PN";			//! PersonName
const wxString nkDictionayKey::nkVR_SH_ShortString = "SH";			//! ShortString
const wxString nkDictionayKey::nkVR_SL_SignedLong = "SL";			//! SignedLong
const wxString nkDictionayKey::nkVR_SQ_SequenceOfItems = "SQ";		//! SequenceOfItems
const wxString nkDictionayKey::nkVR_SS_SignedShort = "SS";			//! SignedShort
const wxString nkDictionayKey::nkVR_ST_ShortText = "ST";				//! ShortText
const wxString nkDictionayKey::nkVR_TM_Time = "TM";					//! Time
const wxString nkDictionayKey::nkVR_UI_UniqueIdentifierUID = "UI";	//! UniqueIdentifierUID
const wxString nkDictionayKey::nkVR_UL_UnsignedLong = "UL";			//! UnsignedLong
const wxString nkDictionayKey::nkVR_UN_Unknow = "UN";				//! Unknow
const wxString nkDictionayKey::nkVR_US_UnsignedShort = "US";			//! UnsignedShort
const wxString nkDictionayKey::nkVR_UT_UnlimitedText = "UT";			//! UnlimitedText


nkDictionayKey::nkDictionayKey(wxString a_Tag, wxString a_ValueRepresentation, wxString a_Name){
	Tag = a_Tag;
	ValueRepresentation = a_ValueRepresentation;
	Name = a_Name;
}

wxString nkDictionayKey::getTag(){
	return Tag;
}

wxString nkDictionayKey::getValueRepresentation(){
	return ValueRepresentation;
}

wxString nkDictionayKey::getName(){
	return Name;
}