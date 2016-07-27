/*
 *
 *  Copyright (C) 2015, IRM Inc.
 *  All rights reserved.  See LICENSE file for details.
 *
 *  This software and supporting documentation were developed by
 *		IRM Inc., Korea.
 *  through the contract with
 *		Seoul National University Bundang Hospital
 *  under the support of
 *		Ministry of Trade, Industry and Energy, Republic of Korea.
 *
 *  Author:  Samuel Choi (samuelchoi@irm.kr)
 *
 */

/*
				   GNU LESSER GENERAL PUBLIC LICENSE
					   Version 3, 29 June 2007

 Copyright (C) 2007 Free Software Foundation, Inc. <http://fsf.org/>
 Everyone is permitted to copy and distribute verbatim copies
 of this license document, but changing it is not allowed.


  This version of the GNU Lesser General Public License incorporates
the terms and conditions of version 3 of the GNU General Public
License, supplemented by the additional permissions listed below.

  0. Additional Definitions.

  As used herein, "this License" refers to version 3 of the GNU Lesser
General Public License, and the "GNU GPL" refers to version 3 of the GNU
General Public License.

  "The Library" refers to a covered work governed by this License,
other than an Application or a Combined Work as defined below.

  An "Application" is any work that makes use of an interface provided
by the Library, but which is not otherwise based on the Library.
Defining a subclass of a class defined by the Library is deemed a mode
of using an interface provided by the Library.

  A "Combined Work" is a work produced by combining or linking an
Application with the Library.  The particular version of the Library
with which the Combined Work was made is also called the "Linked
Version".

  The "Minimal Corresponding Source" for a Combined Work means the
Corresponding Source for the Combined Work, excluding any source code
for portions of the Combined Work that, considered in isolation, are
based on the Application, and not on the Linked Version.

  The "Corresponding Application Code" for a Combined Work means the
object code and/or source code for the Application, including any data
and utility programs needed for reproducing the Combined Work from the
Application, but excluding the System Libraries of the Combined Work.

  1. Exception to Section 3 of the GNU GPL.

  You may convey a covered work under sections 3 and 4 of this License
without being bound by section 3 of the GNU GPL.

  2. Conveying Modified Versions.

  If you modify a copy of the Library, and, in your modifications, a
facility refers to a function or data to be supplied by an Application
that uses the facility (other than as an argument passed when the
facility is invoked), then you may convey a copy of the modified
version:

   a) under this License, provided that you make a good faith effort to
   ensure that, in the event an Application does not supply the
   function or data, the facility still operates, and performs
   whatever part of its purpose remains meaningful, or

   b) under the GNU GPL, with none of the additional permissions of
   this License applicable to that copy.

  3. Object Code Incorporating Material from Library Header Files.

  The object code form of an Application may incorporate material from
a header file that is part of the Library.  You may convey such object
code under terms of your choice, provided that, if the incorporated
material is not limited to numerical parameters, data structure
layouts and accessors, or small macros, inline functions and templates
(ten or fewer lines in length), you do both of the following:

   a) Give prominent notice with each copy of the object code that the
   Library is used in it and that the Library and its use are
   covered by this License.

   b) Accompany the object code with a copy of the GNU GPL and this license
   document.

  4. Combined Works.

  You may convey a Combined Work under terms of your choice that,
taken together, effectively do not restrict modification of the
portions of the Library contained in the Combined Work and reverse
engineering for debugging such modifications, if you also do each of
the following:

   a) Give prominent notice with each copy of the Combined Work that
   the Library is used in it and that the Library and its use are
   covered by this License.

   b) Accompany the Combined Work with a copy of the GNU GPL and this license
   document.

   c) For a Combined Work that displays copyright notices during
   execution, include the copyright notice for the Library among
   these notices, as well as a reference directing the user to the
   copies of the GNU GPL and this license document.

   d) Do one of the following:

	   0) Convey the Minimal Corresponding Source under the terms of this
	   License, and the Corresponding Application Code in a form
	   suitable for, and under terms that permit, the user to
	   recombine or relink the Application with a modified version of
	   the Linked Version to produce a modified Combined Work, in the
	   manner specified by section 6 of the GNU GPL for conveying
	   Corresponding Source.

	   1) Use a suitable shared library mechanism for linking with the
	   Library.  A suitable mechanism is one that (a) uses at run time
	   a copy of the Library already present on the user's computer
	   system, and (b) will operate properly with a modified version
	   of the Library that is interface-compatible with the Linked
	   Version.

   e) Provide Installation Information, but only if you would otherwise
   be required to provide such information under section 6 of the
   GNU GPL, and only to the extent that such information is
   necessary to install and execute a modified version of the
   Combined Work produced by recombining or relinking the
   Application with a modified version of the Linked Version. (If
   you use option 4d0, the Installation Information must accompany
   the Minimal Corresponding Source and Corresponding Application
   Code. If you use option 4d1, you must provide the Installation
   Information in the manner specified by section 6 of the GNU GPL
   for conveying Corresponding Source.)

  5. Combined Libraries.

  You may place library facilities that are a work based on the
Library side by side in a single library together with other library
facilities that are not Applications and are not covered by this
License, and convey such a combined library under terms of your
choice, if you do both of the following:

   a) Accompany the combined library with a copy of the same work based
   on the Library, uncombined with any other library facilities,
   conveyed under the terms of this License.

   b) Give prominent notice with the combined library that part of it
   is a work based on the Library, and explaining where to find the
   accompanying uncombined form of the same work.

  6. Revised Versions of the GNU Lesser General Public License.

  The Free Software Foundation may publish revised and/or new versions
of the GNU Lesser General Public License from time to time. Such new
versions will be similar in spirit to the present version, but may
differ in detail to address new problems or concerns.

  Each version is given a distinguishing version number. If the
Library as you received it specifies that a certain numbered version
of the GNU Lesser General Public License "or any later version"
applies to it, you have the option of following the terms and
conditions either of that published version or of any later version
published by the Free Software Foundation. If the Library as you
received it does not specify a version number of the GNU Lesser
General Public License, you may choose any version of the GNU Lesser
General Public License ever published by the Free Software Foundation.

  If the Library as you received it specifies that a proxy can decide
whether future versions of the GNU Lesser General Public License shall
apply, that proxy's public statement of acceptance of any version is
permanent authorization for you to choose that version for the
Library.

*/

#ifndef DTK_H
#define DTK_H

#include "DTK_global.h"

#include "dcmtk/oflog/oflog.h"
#include "dcmtk/oflog/config.h"
#include "dcmtk/oflog/appender.h"
#include "dcmtk/oflog/fstreams.h"
#include "dcmtk/oflog/helpers/property.h"
#include "dcmtk/oflog/helpers/timehelp.h"

#include "dcmtk/ofstd/ofstd.h"
#include "dcmtk/ofstd/ofconapp.h"
#include "dcmtk/ofstd/ofcmdln.h"
#include "dcmtk/ofstd/ofstream.h"

#include "dcmtk/dcmdata/dctk.h"
#include "dcmtk/dcmdata/dcuid.h"
#include "dcmtk/dcmdata/dcrledrg.h"
#include "dcmtk/dcmdata/dcpxitem.h"
#include "dcmtk/dcmdata/dcfilefo.h"
#include "dcmtk/dcmdata/dcdict.h"
#include "dcmtk/dcmdata/dcdicent.h"
#include "dcmtk/dcmdata/dcdeftag.h"
#include "dcmtk/dcmdata/dcddirif.h"

#include "dcmtk/dcmnet/assoc.h"
#include "dcmtk/dcmnet/dimse.h"
#include "dcmtk/dcmnet/diutil.h"

#include <list>
#include <QString>
#include <QDateTime>
#include <QMutex>
#include <QtXml>

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define	QSTR_TO_CSTR(s)					((s).toLocal8Bit().constData())
#define	QSTR_TO_DSTR(s)					(dcm::String(QSTR_TO_CSTR((s))))
#define	DSTR_TO_CSTR(s)					((s).c_str())
#define	DSTR_TO_QSTR(s)					(QString((s).c_str()))

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define	DTK_VERSION_1_2_0				_T("1.2.0")
#define	DTK_VERSION						DTK_VERSION_1_2_0

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define	EXS_JPEGBaseline				EXS_JPEGProcess1
#define	EXS_JPEGExtended				EXS_JPEGProcess2_4
#define	EXS_JPEGLossless				EXS_JPEGProcess14SV1

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace dcm {
	typedef	signed int					Sint;
	typedef	unsigned int				Uint;

	typedef	unsigned long				Ulong;
	typedef signed long					Slong;

	typedef OFLogger					Logger;

	typedef OFCondition					Status;

	typedef OFString					String;
	typedef std::list<String>			StringList;

	typedef DcmTag						Tag;
	typedef	std::list<Tag>				TagList;

	typedef E_TransferSyntax			TransferSyntax;
	typedef std::list<TransferSyntax>	TransferSyntaxList;

	class NLS;

	class AppEntity;

	class Message;
	class CEchoRQ;
	class CEchoRSP;
	class CStoreRQ;
	class CStoreRSP;
	class CGetRQ;
	class CGetRSP;
	class CFindRQ;
	class CFindRSP;
	class CMoveRQ;
	class CMoveRSP;
	class NEventReportRQ;
	class NEventReportRSP;
	class NGetRQ;
	class NGetRSP;
	class NSetRQ;
	class NSetRSP;
	class NActionRQ;
	class NActionRSP;
	class NCreateRQ;
	class NCreateRSP;
	class NDeleteRQ;
	class NDeleteRSP;

	class Service;
	class ServiceList;
	class AssociationInfo;
	class Association;
	class AssociationRequestor;
	class AssociationListener;
	class AssociationListenerMaster;
	class AssociationListenerWorker;

	class Item;

	class PixelDataProducer;
	class PixelDataConsumer;
	class Dataset;

	class MetaInfo;
	class File;

	class DirRecord;
	class Dir;

	class UIDHelper;

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//	NLS
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	/**
	 * @brief The NLS class
	 */
	class DTKSHARED_EXPORT NLS
	{
	protected:
		struct CharacterSet {
			String	name1;
			String	name2;
			String	escapeSequence;
		};

		static const CharacterSet _characterSets[];

	public:
		/**
		 * @brief AUTO enables automatic NLS encoding/decoding
		 */
		static const int AUTO		= -1;
		/**
		 * @brief OFF disables NLS encoding/decoding
		 */
		static const int OFF		= 0;

		/**
		 * @brief ISO_IR_6 DICOM default character set
		 */
		static const int ISO_IR_6	= 0;

		/**
		 * @brief ISO_IR_100 ISO/IEC 8859-1 Latin-1 character set
		 * Perhaps the most widely used part of ISO/IEC 8859, covering most Western European languages: Danish (partial),[1] Dutch (partial),[2] English, Faeroese, Finnish (partial),[3] French (partial),[3] German, Icelandic, Irish, Italian, Norwegian, Portuguese, Rhaeto-Romanic, Scottish Gaelic, Spanish, Catalan, and Swedish. Languages from other parts of the world are also covered, including: Eastern European Albanian, Southeast Asian Indonesian, as well as the African languages Afrikaans and Swahili. The missing euro sign and capital Ÿ are in the revised version ISO/IEC 8859-15 (see below). The corresponding IANA character set is ISO-8859-1.
		 */
		static const int ISO_IR_100	= 1;

		/**
		 * @brief ISO_IR_101 ISO/IEC 8859-2 Latin-2 character set
		 * Supports those Central and Eastern European languages that use the Latin alphabet, including Bosnian, Polish, Croatian, Czech, Slovak, Slovene, Serbian, and Hungarian. The missing euro sign can be found in version ISO/IEC 8859-16.
		 */
		static const int ISO_IR_101	= 2;

		/**
		 * @brief ISO_IR_109 ISO/IEC 8859-3 Latin-3 character set
		 * Turkish, Maltese, and Esperanto. Largely superseded by ISO/IEC 8859-9 for Turkish and Unicode for Esperanto.
		 */
		static const int ISO_IR_109	= 3;

		/**
		 * @brief ISO_IR_110 ISO/IEC 8859-4 Latin-4 character set
		 * Estonian, Latvian, Lithuanian, Greenlandic, and Sami.
		 */
		static const int ISO_IR_110	= 4;

		/**
		 * @brief ISO_IR_144 ISO/IEC 8859-5 Latin/Cyrillic character set
		 * Covers mostly Slavic languages that use a Cyrillic alphabet, including Belarusian, Bulgarian, Macedonian, Russian, Serbian, and Ukrainian (partial).
		 */
		static const int ISO_IR_144	= 5;

		/**
		 * @brief ISO_IR_127 ISO/IEC 8859-6 Latin/Arabic character set
		 * Covers the most common Arabic language characters. Doesn't support other languages using the Arabic script. Needs to be BiDi and cursive joining processed for display.
		 */
		static const int ISO_IR_127	= 6;

		/**
		 * @brief ISO_IR_126 ISO/IEC 8859-7 Latin/Greek character set
		 * Covers the modern Greek language (monotonic orthography). Can also be used for Ancient Greek written without accents or in monotonic orthography, but lacks the diacritics for polytonic orthography. These were introduced with Unicode.
		 */
		static const int ISO_IR_126	= 7;

		/**
		 * @brief ISO_IR_138 ISO/IEC 8859-8 Latin/Hebrew character set
		 * Covers the modern Hebrew alphabet as used in Israel. In practice two different encodings exist, logical order (needs to be BiDi processed for display) and visual (left-to-right) order (in effect, after bidi processing and line breaking).
		 */
		static const int ISO_IR_138	= 8;

		/**
		 * @brief ISO_IR_148 ISO/IEC 8859-9 Latin-5/Turkish character set
		 * Largely the same as ISO/IEC 8859-1, replacing the rarely used Icelandic letters with Turkish ones.
		 */
		static const int ISO_IR_148	= 9;

		/**
		 * @brief ISO_IR_166 ISO/IEC 8859-11 Latin/Thai character set
		 * Contains characters needed for the Thai language. Virtually identical to TIS 620.
		 */
		static const int ISO_IR_166	= 10;

		/**
		 * @brief ISO_IR_149
		 * Korean
		 */
		static const int ISO_IR_149	= 11;

		/**
		 * @brief ISO_IR_13
		 */
		static const int ISO_IR_13	= 12;

		/**
		 * @brief ISO_IR_14
		 */
		static const int ISO_IR_14	= 13;

		/**
		 * @brief ISO_IR_87
		 */
		static const int ISO_IR_87	= 14;

		/**
		 * @brief ISO_IR_159
		 */
		static const int ISO_IR_159	= 15;

		/**
		 * @brief ISO_IR_58 Chinese GB2312 character set
		 */
		static const int ISO_IR_58	= 16;

		/**
		 * @brief ISO_IR_192 Unicode utf-8 character set
		 */
		static const int ISO_IR_192	= 17;

		/**
		 * @brief GB18030 Chinese GB18030 character set
		 */
		static const int GB18030	= 18;

		static const int ISO_2022_IR_13_87	= (ISO_IR_13 | ISO_IR_87 << 8);
										// Japanese: JIS X 0201 + JIS X 0208
		static const int ISO_2022_IR_6_149	= (ISO_IR_6 | ISO_IR_149 << 8);
										// Korean: ISO 646 + KS X 1001
		static const int ISO_2022_IR_6_58	= (ISO_IR_6 | ISO_IR_58 << 8);
										// Chinese: ISO 646 + GB2312

		static const int DEFAULT			= ISO_IR_6;
		static const int LATIN1				= ISO_IR_100;
		static const int LATIN2				= ISO_IR_101;
		static const int LATIN3				= ISO_IR_109;
		static const int LATIN4				= ISO_IR_110;
		static const int CYRILLIC			= ISO_IR_144;
		static const int ARABIC				= ISO_IR_127;
		static const int GREEK				= ISO_IR_126;
		static const int HEBREW				= ISO_IR_138;
		static const int LATIN5				= ISO_IR_148;
		static const int THAI				= ISO_IR_166;
		static const int KOREAN				= ISO_2022_IR_6_149;
		static const int JAPANESE			= ISO_2022_IR_13_87;
		static const int CHINESE_GB2312		= ISO_2022_IR_6_58;
		static const int CHINESE_GB18030	= GB18030;
		static const int UNICODE_UTF8		= ISO_IR_192;

		/**
		 * @brief characterSetCount returns the number of character sets supported by DTK
		 * @return the number of character sets
		 */
		static int characterSetCount(void);

		static String characterSetName1(int nls);
		static String characterSetName2(int nls);
		static String characterSetEscapeSequence(int nls);

		/**
		 * @brief toLocal8Bit converts unicode string (QString) to local 8 bit string
		 * @param istr input unicode string
		 * @param nls encoding/decoding scheme
		 * @return 8 bit string
		 */
		static String toLocal8Bit(const QString& istr, int nls);

		/**
		 * @brief toUnicode converts 8 bit string to unicode string (QString)
		 * @param istr input 8 bit string
		 * @param nls encoding/decoding scheme
		 * @return unicode string
		 */
		static QString toUnicode(const String& istr, int nls);

		/**
		 * @brief encode encodes 8 bit string to DICOM-encoded one
		 * @param istr input 8 bit string
		 * @param nls encoding scheme
		 * @return DICOM-encoded string
		 */
		static String encode(const String& istr, int nls);

		/**
		 * @brief decode decodes DICOM-encoded string
		 * @param istr DICOM-encoded string
		 * @param nls decoding scheme
		 * @return decoded 8 bit string
		 */
		static String decode(const String& istr, int nls = ISO_IR_6);

		/**
		 * @brief encodeQ encodes unicode string (QString) to DICOM-encoded one
		 * @param istr input unicode string
		 * @param nls encoding scheme
		 * @return DICOM-encoded string
		 */
		static String encodeQ(const QString& istr, int nls);

		/**
		 * @brief decodeQ decodes DICOM-encoded string
		 * @param istr DICOM-encoded encoded
		 * @param nls decoding scheme
		 * @return decoded unicode string (QString)
		 */
		static QString decodeQ(const String& istr, int nls = ISO_IR_6);
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//	DateTime
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	class DTKSHARED_EXPORT DateTime : public tm
	{
	public:
		/**
		 * @brief DateTime default constructor
		 */
		DateTime(void);

		/**
		 * @brief DateTime constructor
		 * @param year year
		 * @param month month (01..12)
		 * @param day day (01..31)
		 */
		DateTime(int year, int month, int day);

		/**
		 * @brief DateTime constructor
		 * @param year year
		 * @param month month (01..12)
		 * @param day day (01..31)
		 * @param hour (00..23)
		 * @param minute (00..59)
		 * @param second (00..59)
		 */
		DateTime(int year, int month, int day, int hour, int minute, int second);

		/**
		 * @brief format creates formatted date/time string
		 * @param format date/time format in strftime() (e.g. %y%M%d %H%m%s)
		 * @return formatted string
		 */
		String format(const String& format) const;

		/**
		 * @brief currentDateTime gets current date/time
		 * @return current date/time
		 */
		static DateTime currentDateTime();
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//	AppEntity
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	/**
	 * @brief The AppEntity class
	 */
	class DTKSHARED_EXPORT AppEntity
	{
	public:
		/**
		 * @brief _checkApplicationContextName is used to check the validity of application context name
		 * @todo should be protected or private, and add related methods
		 */
		bool _checkApplicationContextName;

		/**
		 * @brief _checkImplementationClassUID is used to check the validity of implementation class UID
		 * @todo should be protected or private, and add related methods
		 */
		bool _checkImplementationClassUID;

		/**
		 * @brief _maxPDUSize specifies the maximum PDU size
		 * @todo should be protected or private, and add related methods
		 */
		int _maxPDUSize;

	protected:
		String _aetitle;
		T_ASC_Network* _ascNetwork;

	public:
		/**
		 * @brief AppEntity constructor
		 */
		AppEntity(void);

		/**
		 * @brief ~AppEntity destructor
		 */
		virtual ~AppEntity(void);

		/**
		 * @brief init initializes DICOM Application Entity
		 * @param aetitle AE Title
		 * @param ascRole association role
		 * @param listenerPort listening port
		 * @param timeout timeout in second
		 * @return status
		 */
		Status init(const String& aetitle, const T_ASC_NetworkRole ascRole, const int listenerPort, const int timeout);

		/**
		 * @brief exit finalizes DICOM Application Entity
		 * @return status
		 */
		Status exit(void);

		/**
		 * @brief getAETitle returns AE Title
		 * @return AE Title
		 */
		String getAETitle(void) const;

		/**
		 * @brief getInternal returns internal T_ASC_Network*
		 * @return T_ASC_Network*
		 */
		T_ASC_Network* getInternal(void);

	#ifdef	_DEBUG
		static void	internalTest(void);
	#endif

	private:
		/**
		 * @brief AppEntity
		 */
		AppEntity(const AppEntity&);					// do not allow pass-by-value

		/**
		 * @brief operator =
		 * @return
		 */
		const AppEntity& operator=(const AppEntity&);	// do not allow assignment
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//	Message
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	/**
	 * @brief The Message class
	 */
	class DTKSHARED_EXPORT Message : public T_DIMSE_Message
	{
	protected:
		String _sopClassUID;
		String _sopInstanceUID;

	public:	// FIXME
		T_ASC_PresentationContextID _pcId;
		DcmDataset* _dcmDataset;

	public:
		/**
		 * @brief Message default constructor
		 */
		Message(void);
		/**
		 * @brief Message constructor
		 * @param dataset pointer to the existing Dataset
		 */
		Message(Dataset* dataset);

		/**
		 * @brief setDcmDataset set Dataset
		 * @param dataset pointer to the existing Dataset
		 * @return self
		 */
		Message& setDcmDataset(Dataset* dataset);
		virtual void setIDs(T_ASC_Association* ascAssocPtr);

		String getCommandName(void) const;
		/**
		 * @brief getStatus gets status code
		 * @return status code
		 */
		Uint16 getStatus(void) const;

		/**
		 * @brief print prints message contents to text file
		 * @param filename text file name
		 * @return self
		 */
		Message& print(const String& filename);

		virtual void onProgress(Uint32 byteCount);

	private:
		Message(const Message&);					// do not allow pass-by-value
		const Message& operator=(const Message&);	// do not allow assignment
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	/**
	 * @brief The CEchoRQ class
	 */
	class DTKSHARED_EXPORT CEchoRQ : public Message
	{
	public:
		/**
		 * @brief CEchoRQ constructor
		 * @param dataset
		 */
		CEchoRQ(Dataset* dataset = NULL);

		/**
		 * @brief setIDs sets association to this message
		 * @param ascAssoc
		 */
		virtual void setIDs(T_ASC_Association* ascAssoc);
	};

	class DTKSHARED_EXPORT CEchoRSP : public Message
	{
	public:
		/**
		 * @brief CEchoRSP constructor
		 * @param req
		 * @param status
		 * @param dataset
		 */
		CEchoRSP(const Message& req, Uint16 status = STATUS_Success, Dataset* dataset = NULL);
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	class DTKSHARED_EXPORT CStoreRQ : public Message
	{
	public:
		/**
		 * @brief CStoreRQ constructor
		 * @param dataset
		 */
		CStoreRQ(Dataset* dataset);

		/**
		 * @brief setIDs sets association to this message
		 * @param ascAssoc
		 */
		virtual void setIDs(T_ASC_Association* ascAssoc);
	};

	class DTKSHARED_EXPORT CStoreRSP : public Message
	{
	public:
		/**
		 * @brief CStoreRSP constructor
		 * @param req
		 * @param status
		 * @param dataset
		 */
		CStoreRSP(const Message& req, Uint16 status = STATUS_Success, Dataset* dataset = NULL);
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	class DTKSHARED_EXPORT CFindRQ : public Message
	{
	public:
		/**
		 * @brief CFindRQ constructor
		 * @param sopClassUID
		 * @param dataset
		 */
		CFindRQ(const String& sopClassUID, Dataset* dataset);

		/**
		 * @brief setIDs sets association to this message
		 * @param ascAssoc
		 */
		virtual void setIDs(T_ASC_Association* ascAssoc);
	};

	class DTKSHARED_EXPORT CFindRSP : public Message
	{
	public:
		/**
		 * @brief CFindRSP constructor
		 * @param req
		 * @param status
		 * @param dataset
		 */
		CFindRSP(const Message& req, Uint16 status = STATUS_Success, Dataset* dataset = NULL);
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	class DTKSHARED_EXPORT CGetRQ : public Message
	{
	public:
		/**
		 * @brief CGetRQ constructor
		 * @param sopClassUID
		 * @param dataset
		 */
		CGetRQ(const String& sopClassUID, Dataset* dataset);

		/**
		 * @brief setIDs sets association to this message
		 * @param ascAssoc
		 */
		virtual void setIDs(T_ASC_Association* ascAssoc);
	};

	class DTKSHARED_EXPORT CGetRSP : public Message
	{
	public:
		/**
		 * @brief CGetRSP constructor
		 * @param req
		 * @param status
		 * @param dataset
		 * @param remaining
		 * @param completed
		 * @param failure
		 * @param warning
		 */
		CGetRSP(const Message& req, Uint16 status = STATUS_Success, Dataset* dataset = NULL,
				int remaining = -1, int completed = -1, int failure = -1, int warning = -1);
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	class DTKSHARED_EXPORT CMoveRQ : public Message
	{
	public:
		/**
		 * @brief CMoveRQ constructor
		 * @param sopClassUID
		 * @param destination
		 * @param dataset
		 */
		CMoveRQ(const String& sopClassUID, const String& destination, Dataset* dataset);

		/**
		 * @brief setIDs sets association to this message
		 * @param ascAssoc
		 */
		virtual void setIDs(T_ASC_Association* ascAssoc);
	};

	class DTKSHARED_EXPORT CMoveRSP : public Message
	{
	public:
		/**
		 * @brief CMoveRSP constructor
		 * @param req
		 * @param status
		 * @param dataset
		 * @param remaining
		 * @param completed
		 * @param failure
		 * @param warning
		 */
		CMoveRSP(const Message& req, Uint16 status = STATUS_Success, Dataset* dataset = NULL,
				 int remaining = -1, int completed = -1, int failure = -1, int warning = -1);
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	class DTKSHARED_EXPORT CCancelRQ : public Message
	{
	public:
		/**
		 * @brief CCancelRQ constructor
		 * @param req
		 */
		CCancelRQ(const Message& req);
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	class DTKSHARED_EXPORT NEventReportRQ : public Message
	{
	public:
		/**
		 * @brief NEventReportRQ constructor
		 * @param sopClassUID
		 * @param sopInstanceUID
		 * @param eventTypeId
		 * @param dataset
		 */
		NEventReportRQ(const String& sopClassUID, const String& sopInstanceUID, Uint16 eventTypeId, Dataset* dataset);

		/**
		 * @brief setIDs sets association to this message
		 * @param ascAssocPtr
		 */
		virtual void setIDs(T_ASC_Association* ascAssocPtr);
	};

	class DTKSHARED_EXPORT NEventReportRSP : public Message
	{
	public:
		/**
		 * @brief NEventReportRSP constructor
		 * @param req
		 * @param status
		 * @param datasetPtr
		 */
		NEventReportRSP(const Message& req, Uint16 status = STATUS_Success, Dataset* datasetPtr = NULL);
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	class DTKSHARED_EXPORT NGetRQ : public Message
	{
	public:
		/**
		 * @brief NGetRQ constructor
		 * @param sopClassUID
		 * @param sopInstanceUID
		 * @param dcmTag
		 * @param dcmTagCount
		 * @param dataset
		 */
		NGetRQ(const String& sopClassUID, const String& sopInstanceUID, const DcmTagKey* dcmTag, int dcmTagCount, Dataset* dataset = NULL);

		/**
		 * @brief ~NGetRQ destructor
		 */
		virtual ~NGetRQ(void);

		/**
		 * @brief setIDs sets association to this message
		 * @param ascAssoc
		 */
		virtual void setIDs(T_ASC_Association* ascAssoc);
	};

	class DTKSHARED_EXPORT NGetRSP : public Message
	{
	public:
		/**
		 * @brief NGetRSP constructor
		 * @param req
		 * @param nStatus
		 * @param _dcmDataset
		 */
		NGetRSP(const Message& req, Uint16 nStatus = STATUS_Success, Dataset* _dcmDataset = NULL);
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	class DTKSHARED_EXPORT NSetRQ : public Message
	{
	public:
		/**
		 * @brief NSetRQ constructor
		 * @param sopClassUID
		 * @param sopInstanceUID
		 * @param dataset
		 */
		NSetRQ(const String& sopClassUID, const String& sopInstanceUID, Dataset* dataset);

		/**
		 * @brief setIDs sets association to this message
		 * @param ascAssoc
		 */
		virtual void setIDs(T_ASC_Association* ascAssoc);
	};

	class DTKSHARED_EXPORT NSetRSP : public Message
	{
	public:
		/**
		 * @brief NSetRSP constructor
		 * @param req
		 * @param status
		 * @param dataset
		 */
		NSetRSP(const Message& req, Uint16 status = STATUS_Success, Dataset* dataset = NULL);
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	class DTKSHARED_EXPORT NActionRQ : public Message
	{
	public:
		/**
		 * @brief NActionRQ constructor
		 * @param sopClassUID
		 * @param sopInstanceUID
		 * @param actionTypeId
		 * @param dataset
		 */
		NActionRQ(const String& sopClassUID, const String& sopInstanceUID, Uint16 actionTypeId, Dataset* dataset = NULL);

		/**
		 * @brief setIDs sets association to this message
		 * @param ascAssoc
		 */
		virtual void setIDs(T_ASC_Association* ascAssoc);
	};

	class DTKSHARED_EXPORT NActionRSP : public Message
	{
	public:
		/**
		 * @brief NActionRSP constructor
		 * @param req
		 * @param status
		 * @param dataset
		 */
		NActionRSP(const Message& req, Uint16 status = STATUS_Success, Dataset* dataset = NULL);
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	class DTKSHARED_EXPORT NCreateRQ : public Message
	{
	public:
		/**
		 * @brief NCreateRQ constructor
		 * @param sopClassUID
		 * @param sopInstanceUID
		 * @param dataset
		 */
		NCreateRQ(const String& sopClassUID, const String& sopInstanceUID, Dataset* dataset);

		/**
		 * @brief setIDs sets association to this message
		 * @param ascAssocPtr
		 */
		virtual void setIDs(T_ASC_Association* ascAssocPtr);
	};

	class DTKSHARED_EXPORT NCreateRSP : public Message
	{
	public:
		/**
		 * @brief NCreateRSP constructor
		 * @param req
		 * @param status
		 * @param dataset
		 */
		NCreateRSP(const Message& req, Uint16 status = STATUS_Success, Dataset* dataset = NULL);
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	class DTKSHARED_EXPORT NDeleteRQ : public Message
	{
	public:
		/**
		 * @brief NDeleteRQ constructor
		 * @param sopClassUID
		 * @param sopInstanceUID
		 * @param dataset
		 */
		NDeleteRQ(const String& sopClassUID, const String& sopInstanceUID, Dataset* dataset = NULL);

		/**
		 * @brief setIDs sets association to this message
		 * @param ascAssoc
		 */
		virtual void setIDs(T_ASC_Association* ascAssoc);
	};

	class DTKSHARED_EXPORT NDeleteRSP : public Message
	{
	public:
		/**
		 * @brief NDeleteRSP constructor
		 * @param req
		 * @param status
		 * @param dataset
		 */
		NDeleteRSP(const Message& req, Uint16 status = STATUS_Success, Dataset* dataset = NULL);
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//	Service
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	class DTKSHARED_EXPORT Service
	{
	public:
		T_ASC_SC_ROLE _role;					// Role
		String _abstractSyntax;					// SOP Class UID
		TransferSyntaxList _transferSyntaxList;	// Transfer Syntax List

	public:
		/**
		 * @brief Service constructor
		 */
		Service(void)
		{
			_role = ASC_SC_ROLE_DEFAULT;
		}

		/**
		 * @brief addBasicTransferSyntaxes adds basic transfer syntaxes (implicit little endian, explicit little endian, explicit big endian)
		 */
		void addBasicTransferSyntaxes(void);
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//	ServiceList
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	typedef std::list<Service>			ServiceListType;

	class DTKSHARED_EXPORT ServiceList
	{
	protected:
		ServiceListType _serviceList;

		static ServiceList _basicSCUServiceList;
		static ServiceList _basicSCPServiceList;

	public:
		/**
		 * @brief ServiceList constructor
		 * @param includeEcho flag to include DICOM Verification Service as default
		 */
		ServiceList(bool includeEcho = true);

		/**
		 * @brief hasService checks if this service list has the specified abstract syntax/transfer syntax pair
		 * @param abstractSyntax
		 * @param transferSyntax
		 * @return
		 */
		bool hasService(const String& abstractSyntax, TransferSyntax transferSyntax);

		/**
		 * @brief hasService checks if this service list has the specified abstract syntax/transfer syntax pair
		 * @param abstractSyntax
		 * @param transferSyntaxUID
		 * @return
		 */
		bool hasService(const String& abstractSyntax, const String& transferSyntaxUID);

		/**
		 * @brief addService adds abstract syntax/transfer syntax pair to this service list
		 * @param abstractSyntax
		 * @param transferSyntax
		 */
		void addService(const String& abstractSyntax, TransferSyntax transferSyntax);

		/**
		 * @brief addService adds abstract syntax/transfer syntax pair to this service list
		 * @param abstractSyntax
		 * @param transferSyntaxUID
		 */
		void addService(const String& abstractSyntax, const String& transferSyntaxUID);

		/**
		 * @brief addService adds abstract syntax/transfer syntax pair to this service list
		 * @param abstractSyntax
		 * @param transferSyntaxList
		 */
		void addService(const String& abstractSyntax, const TransferSyntaxList& transferSyntaxList);

		/**
		 * @brief addService adds abstract syntax/transfer syntax pair to this service list
		 * @param abstractSyntax
		 * @param transferSyntaxUIDList
		 */
		void addService(const String& abstractSyntax, const StringList& transferSyntaxUIDList);

		/**
		 * @brief addVerificationService adds DICOM Verification Service to this service list
		 */
		void addVerificationService(void);

		/**
		 * @brief addAllStorageSCUServices adds all Storage SCU services to this service list
		 */
		void addAllStorageSCUServices(void);

		/**
		 * @brief addAllStorageSCPServices adds all Storage SCP services to this service list
		 */
		void addAllStorageSCPServices(void);

		/**
		 * @brief addAllQueryRetrieveServices adds all Query/Retrieve services to this service list
		 */
		void addAllQueryRetrieveServices(void);

		/**
		 * @brief addMWLService adds MWL service to this service list
		 */
		void addMWLService(void);

		/**
		 * @brief addMPPSService adds MPPS service to this service list
		 */
		void addMPPSService(void);

		/**
		 * @brief addStorageCommitmentSCUService adds Storage Commitment SCU service to this service list
		 */
		void addStorageCommitmentSCUService(void);

		/**
		 * @brief addStorageCommitmentSCPService adds Storage Commitment SCP service to this service list
		 */
		void addStorageCommitmentSCPService(void);

		/**
		 * @brief addGrayscalePrintService adds Grayscale Print service to this service list
		 */
		void addGrayscalePrintService(void);

		/**
		 * @brief addColorPrintService adds Color Print service to this service list
		 */
		void addColorPrintService(void);

		/**
		 * @brief size returns the number of abstract syntaxes in this service list
		 * @return
		 */
		ServiceListType::size_type size() const;

		/**
		 * @brief cbegin returns the first position of this service list
		 * @return
		 */
		ServiceListType::const_iterator cbegin() const;

		/**
		 * @brief cend returns the last position of this service list
		 * @return
		 */
		ServiceListType::const_iterator cend() const;

		/**
		 * @brief getBasicSCUServiceList returns the service list containing all SCU services
		 * @return
		 */
		static const ServiceList& getBasicSCUServiceList(void);

		/**
		 * @brief getBasicSCPServiceList returns the service list containing all SCP services
		 * @return
		 */
		static const ServiceList& getBasicSCPServiceList(void);
	};

#define	DCM_BASIC_SCU_SERVICE_LIST		dcm::ServiceList::getBasicSCUServiceList()
#define	DCM_BASIC_SCP_SERVICE_LIST		dcm::ServiceList::getBasicSCPServiceList()

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//	AssociationInfo
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	class DTKSHARED_EXPORT AssociationInfo
	{
	public:
		/**
		 * @brief applicationContextName DICOM Application Context Name
		 */
		String applicationContextName;

		/**
		 * @brief callingAETitle calling AE Title
		 */
		String callingAETitle;

		/**
		 * @brief callingPresentationAddress caller's presentation address
		 */
		String callingPresentationAddress;

		/**
		 * @brief callingImplementationClassUID caller's implementation class UID
		 */
		String callingImplementationClassUID;

		/**
		 * @brief callingImplementationVersion caller's implementation version
		 */
		String callingImplementationVersion;

		/**
		 * @brief calledAETitle called AE Title
		 */
		String calledAETitle;

		/**
		 * @brief calledPresentationAddress callee's presentation address
		 */
		String calledPresentationAddress;

		/**
		 * @brief calledImplementationClassUID callee's implementation class UID
		 */
		String calledImplementationClassUID;

		/**
		 * @brief calledImplementationVersion callee's implementation version
		 */
		String calledImplementationVersion;

		/**
		 * @brief respondingAETitle responding AE Title
		 */
		String respondingAETitle;

		/**
		 * @brief maxPDUSizeOfMine maximum PDU size of this AE
		 */
		Uint32 maxPDUSizeOfMine;

		/**
		 * @brief maxPDUSizeOfPeer maximum PDU size of peer AE
		 */
		Uint32 maxPDUSizeOfPeer;

		/**
		 * @brief AssociationInfo constructor
		 */
		AssociationInfo(void)
		{
			maxPDUSizeOfMine = 0;
			maxPDUSizeOfPeer = 0;
		}
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//	Association
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	class DTKSHARED_EXPORT Association
	{
	protected:
		static Logger _logger;

	protected:
		AppEntity* _appEntity;
		T_ASC_Parameters* _ascParams;
		T_ASC_Association* _ascAssoc;
		QMutex _assocMutex;
		int _maxPDUSize;

	public:
		/**
		 * @brief Association constructor
		 * @param appEntity
		 */
		Association(AppEntity* appEntity);

		/**
		 * @brief ~Association destructor
		 */
		virtual ~Association(void);

		/**
		 * @brief close closes the association
		 * @return
		 */
		Status close(void);

		/**
		 * @brief sendMessage sends message to remote AE
		 * @param req
		 * @return
		 */
		Status sendMessage(Message& req);

		/**
		 * @brief receiveMessage receives message from remote AE
		 * @param rsp
		 * @param timeout
		 * @return
		 */
		Status receiveMessage(Message& rsp, int timeout);

		/**
		 * @brief getAssocInfo retrieves association information
		 * @param assocInfo
		 * @return
		 */
		Status getAssocInfo(AssociationInfo& assocInfo);

		/**
		 * @brief verify performs DICOM Verification Service
		 * @param localAETitle
		 * @param aetitle
		 * @param hostname
		 * @param port
		 * @param timeout
		 * @return
		 */
		static Status verify(const String& localAETitle, const String& aetitle, const String& hostname, const Uint16 port, const Uint16 timeout = 15);

		/**
		 * @brief verify performs DICOM Verification Service
		 * @param localAETitle
		 * @param aetitle
		 * @param hostname
		 * @param port
		 * @param timeout
		 * @return
		 */
		static Status verify(const QString& localAETitle, const QString& aetitle, const QString& hostname, const Uint16 port, const Uint16 timeout = 15);

	private:
		Association(const Association&);					// do not allow pass-by-value
		const Association& operator=(const Association&);	// do not allow assignment
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//	AssociationRequestor
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	class DTKSHARED_EXPORT AssociationRequestor :
		public Association
	{
	protected:
		static Logger _logger;

	public:
		/**
		 * @brief AssociationRequestor constructor
		 * @param appEntity
		 */
		AssociationRequestor(AppEntity* appEntity);

		/**
		 * @brief ~AssociationRequestor destructor
		 */
		virtual ~AssociationRequestor(void);

		/**
		 * @brief connect creates an association to remote AE
		 * @param calledAETitle
		 * @param hostname
		 * @param port
		 * @param serviceList
		 * @return
		 */
		Status	connect(const String& calledAETitle, const String& hostname, const Uint16 port, const ServiceList& serviceList = DCM_BASIC_SCU_SERVICE_LIST);
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//	AssociationListener
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	class DTKSHARED_EXPORT AssociationListener :
		public Association
	{
	protected:
		static Logger _logger;

	public:
		/**
		 * @brief AssociationListener constructor
		 * @param appEntity
		 */
		AssociationListener(AppEntity* appEntity);

		/**
		 * @brief ~AssociationListener destructor
		 */
		virtual ~AssociationListener(void);

		/**
		 * @brief listen waits for an association
		 * @param nTimeout
		 * @return
		 */
		Status	listen(const int nTimeout);

		/**
		 * @brief accept accepts this association
		 * @param serviceList
		 * @return
		 */
		Status	accept(const ServiceList& serviceList = DCM_BASIC_SCP_SERVICE_LIST);

		/**
		 * @brief reject rejects this association
		 * @return
		 */
		Status	reject(void);

		/**
		 * @brief abort aborts this association
		 * @return
		 */
		Status	abort(void);
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//	AssociationListenerMaster
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	class DTKSHARED_EXPORT AssociationListenerMaster :
		public AssociationListener,
		public OFThread
	{
	protected:
		static Logger _logger;
		bool _started;
		bool _stopped;
		Uint32 _counter;
		QMutex _masterMutex;

	public:
		/**
		 * @brief AssociationListenerMaster constructor
		 * @param appEntity
		 */
		AssociationListenerMaster(AppEntity* appEntity);

		/**
		 * @brief ~AssociationListenerMaster destructor
		 */
		virtual ~AssociationListenerMaster(void);

		/**
		 * @brief startlisten spawns a new master listener thread
		 * @return
		 */
		Status startlisten(void);

		/**
		 * @brief stoplisten stops listening
		 * @return
		 */
		Status stoplisten(void);

		/**
		 * @brief isListening returns the listening status
		 * @return
		 */
		bool isListening(void);

	protected:
		/**
		 * @brief run main thread of master listener
		 */
		virtual void run(void);

		/**
		 * @brief clone creates new worker listener (MUST be implemented in the derived class)
		 * @return
		 */
		virtual AssociationListenerWorker* clone(void) = 0;

		friend class AssociationListenerWorker;
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//	AssociationListenerWorker
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	class DTKSHARED_EXPORT AssociationListenerWorker :
		public AssociationListener,
		public OFThread
	{
	protected:
		static Logger _logger;
		AssociationListenerMaster* _masterListener;

	public:
		/**
		 * @brief AssociationListenerWorker constructor
		 * @param masterListener
		 */
		AssociationListenerWorker(AssociationListenerMaster* masterListener);

		/**
		 * @brief ~AssociationListenerWorker destructor
		 */
		virtual ~AssociationListenerWorker(void);

		/**
		 * @brief startlisten starts worker listener thread
		 * @return
		 */
		Status startlisten(void);

		/**
		 * @brief stoplisten stops listening
		 * @return
		 */
		Status stoplisten(void);	// not required, nor implemented

		/**
		 * @brief isListening returns listening status
		 * @return
		 */
		bool isListening(void);

	protected:
		/**
		 * @brief run main thread of worker listener
		 */
		virtual void run(void);

		/**
		 * @brief onlisten worker callback (MUST be implemented in the derived class)
		 */
		virtual void onlisten(void) = 0;
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//	Item
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	class DTKSHARED_EXPORT Item
	{
	protected:
		bool _created;
		DcmItem* _dcmItem;
		int _autoNLS;
		static int _DefaultNLS;

	public:
		static const int POS_FIRST = 0;
		static const int POS_LAST = -1;
		static const int POS_APPEND = -2;

		/**
		 * @brief Item
		 */
		Item(void);

		/**
		 * @brief Item
		 * @param _dcmItem
		 */
		Item(DcmItem* _dcmItem);

		/**
		 * @brief Item
		 * @param dcmItem
		 */
		Item(const Item& dcmItem);

		/**
		 * @brief ~Item
		 */
		virtual ~Item(void);

		/**
		 * @brief operator =
		 * @param item
		 * @return
		 */
		Item& operator=(const Item& item);

		/**
		 * @brief copyFrom
		 * @param sourceItem
		 * @return
		 */
		Status copyFrom(const Item* sourceItem);

		/**
		 * @brief set
		 * @param _dcmItem
		 * @param parentNLS
		 * @return
		 */
		Status set(DcmItem* _dcmItem, int parentNLS = NLS::DEFAULT);

		/**
		 * @brief clear
		 * @return
		 */
		Status clear(void);

		/**
		 * @brief print
		 * @param filename
		 * @return
		 */
		Status print(const char* filename) const;

		/**
		 * @brief print
		 * @param filename
		 * @return
		 */
		Status print(const String& filename) const;

		/**
		 * @brief print
		 * @param filename
		 * @return
		 */
		Status print(const QString& filename) const;

		/**
		 * @brief printXML
		 * @param filename
		 * @return
		 */
		Status printXML(const char* filename) const;

		/**
		 * @brief printXML
		 * @param filename
		 * @return
		 */
		Status printXML(const String& filename) const;

		/**
		 * @brief printXML
		 * @param filename
		 * @return
		 */
		Status printXML(const QString& filename) const;

		////////////////////////////////////////////////////////////////////////////////////////////////////////////

		/**
		 * @brief putString
		 * @param tag
		 * @param value
		 * @param nls
		 * @return
		 */
		Status putString(const DcmTagKey& tag, const char* value, int nls = NLS::AUTO);

		/**
		 * @brief putString
		 * @param tag
		 * @param value
		 * @param nls
		 * @return
		 */
		Status putString(const DcmTagKey& tag, const String& value, int nls = NLS::AUTO);

		/**
		 * @brief putString
		 * @param tag
		 * @param value
		 * @param nls
		 * @return
		 */
		Status putString(const DcmTagKey& tag, const QString& value, int nls = NLS::AUTO);
			// AE, AS, AT, CS, DA, DS, DT, FL, FD, IS, LO, LT, OB, OF, OW, PN, SH, SL, SS, ST, TM, UI, UL, US, UT

#if SIZEOF_LONG == 4
		/**
		 * @brief putValue
		 * @param tag
		 * @param value
		 * @param pos
		 * @return
		 */
		Status putValue(const DcmTagKey& tag, Uint value, Uint32 pos = 0);

		/**
		 * @brief putValue
		 * @param tag
		 * @param value
		 * @param pos
		 * @return
		 */
		Status putValue(const DcmTagKey& tag, Sint value, Uint32 pos = 0);
#endif
		/**
		 * @brief putValue
		 * @param tag
		 * @param value
		 * @param pos
		 * @return
		 */
		Status putValue(const DcmTagKey& tag, Uint16 value, Uint32 pos = 0);							// US

		/**
		 * @brief putValue
		 * @param tag
		 * @param value
		 * @param pos
		 * @return
		 */
		Status putValue(const DcmTagKey& tag, Sint16 value, Uint32 pos = 0);							// SS

		/**
		 * @brief putValue
		 * @param tag
		 * @param value
		 * @param pos
		 * @return
		 */
		Status putValue(const DcmTagKey& tag, Uint32 value, Uint32 pos = 0);							// UL

		/**
		 * @brief putValue
		 * @param tag
		 * @param value
		 * @param pos
		 * @return
		 */
		Status putValue(const DcmTagKey& tag, Sint32 value, Uint32 pos = 0);							// SL

		/**
		 * @brief putValue
		 * @param tag
		 * @param value
		 * @param pos
		 * @return
		 */
		Status putValue(const DcmTagKey& tag, Float32 value, Uint32 pos = 0);							// FL

		/**
		 * @brief putValue
		 * @param tag
		 * @param value
		 * @param pos
		 * @return
		 */
		Status putValue(const DcmTagKey& tag, Float64 value, Uint32 pos = 0);							// FD

		/**
		 * @brief putValue
		 * @param tag
		 * @param valuePtr
		 * @param length
		 * @return
		 */
		Status putValue(const DcmTagKey& tag, const Uint8* valuePtr, Uint32 length);					// AT, OW, US/SS, OB/OW

		/**
		 * @brief putValue
		 * @param tag
		 * @param valuePtr
		 * @param length
		 * @return
		 */
		Status putValue(const DcmTagKey& tag, const Uint16* valuePtr, Uint32 length);					// AT, OW, US/SS, OB/OW

		/**
		 * @brief putValue
		 * @param tag
		 * @param valuePtr
		 * @param length
		 * @return
		 */
		Status putValue(const DcmTagKey& tag, const Sint16* valuePtr, Uint32 length);					// SS, US/SS

		/**
		 * @brief putValue
		 * @param tag
		 * @param valuePtr
		 * @param length
		 * @return
		 */
		Status putValue(const DcmTagKey& tag, const Float32* valuePtr, Uint32 length);					// FL

		/**
		 * @brief putValue
		 * @param tag
		 * @param valuePtr
		 * @param length
		 * @return
		 */
		Status putValue(const DcmTagKey& tag, const Float64* valuePtr, Uint32 length);					// FD

		/**
		 * @brief putEmpty
		 * @param tag
		 * @return
		 */
		Status putEmpty(const DcmTagKey& tag);

		/**
		 * @brief removeValue
		 * @param tag
		 * @return
		 */
		Status removeValue(const DcmTagKey& tag);

		////////////////////////////////////////////////////////////////////////////////////////////////////////////

		/**
		 * @brief getString
		 * @param tag
		 * @param value
		 * @param pos
		 * @param nls
		 * @return
		 */
		Status getString(const DcmTagKey& tag, String& value, Sint32 pos = -1, int nls = NLS::AUTO) const;

		/**
		 * @brief getString
		 * @param tag
		 * @param value
		 * @param pos
		 * @param nls
		 * @return
		 */
		Status getString(const DcmTagKey& tag, QString& value, Sint32 pos = -1, int nls = NLS::AUTO) const;
			// AE, AS, AT, CS, DA, DS, DT, FL, FD, IS, LO, LT, OB, OF, OW, PN, SH, SL, SS, ST, TM, UI, UL, US, UT

#if SIZEOF_LONG == 4
		/**
		 * @brief getValue
		 * @param tag
		 * @param value
		 * @param pos
		 * @return
		 */
		Status getValue(const DcmTagKey& tag, Uint& value, Uint32 pos = 0) const;

		/**
		 * @brief getValue
		 * @param tag
		 * @param value
		 * @param pos
		 * @return
		 */
		Status getValue(const DcmTagKey& tag, Sint& value, Uint32 pos = 0) const;
#endif
		/**
		 * @brief getValue
		 * @param tag
		 * @param value
		 * @param pos
		 * @return
		 */
		Status getValue(const DcmTagKey& tag, Uint16& value, Uint32 pos = 0) const;						// US

		/**
		 * @brief getValue
		 * @param tag
		 * @param value
		 * @param pos
		 * @return
		 */
		Status getValue(const DcmTagKey& tag, Uint32& value, Uint32 pos = 0) const;						// UL

		/**
		 * @brief getValue
		 * @param tag
		 * @param value
		 * @param pos
		 * @return
		 */
		Status getValue(const DcmTagKey& tag, Sint16& value, Uint32 pos = 0) const;						// SS

		/**
		 * @brief getValue
		 * @param tag
		 * @param value
		 * @param pos
		 * @return
		 */
		Status getValue(const DcmTagKey& tag, Sint32& value, Uint32 pos = 0) const;						// SL

		/**
		 * @brief getValue
		 * @param tag
		 * @param value
		 * @param pos
		 * @return
		 */
		Status getValue(const DcmTagKey& tag, Float32& value, Uint32 pos = 0) const;					// FL

		/**
		 * @brief getValue
		 * @param tag
		 * @param value
		 * @param pos
		 * @return
		 */
		Status getValue(const DcmTagKey& tag, Float64& value, Uint32 pos = 0) const;					// FD

		/**
		 * @brief getValue
		 * @param tag
		 * @param value
		 * @param pos
		 * @return
		 */
		Status getValue(const DcmTagKey& tag, Uint8& value, Uint32 pos = 0) const;						// OB

		/**
		 * @brief getValue
		 * @param tag
		 * @param valuePtr
		 * @param lengthPtr
		 * @return
		 */
		Status getValue(const DcmTagKey& tag, const Uint8*& valuePtr, Ulong* lengthPtr = NULL) const;	// OB

		/**
		 * @brief getValue
		 * @param tag
		 * @param valuePtr
		 * @param countPtr
		 * @return
		 */
		Status getValue(const DcmTagKey& tag, const Uint16*& valuePtr, Ulong* countPtr = NULL) const;	// AT, OW, US/SS, OB/OW

		/**
		 * @brief getValue
		 * @param tag
		 * @param valuePtr
		 * @param countPtr
		 * @return
		 */
		Status getValue(const DcmTagKey& tag, const Uint32*& valuePtr, Ulong* countPtr = NULL) const;	// UL

		/**
		 * @brief getValue
		 * @param tag
		 * @param valuePtr
		 * @param countPtr
		 * @return
		 */
		Status getValue(const DcmTagKey& tag, const Sint16*& valuePtr, Ulong* countPtr = NULL) const;	// AT, OW, US/SS, OB/OW

		/**
		 * @brief getValue
		 * @param tag
		 * @param valuePtr
		 * @param countPtr
		 * @return
		 */
		Status getValue(const DcmTagKey& tag, const Sint32*& valuePtr, Ulong* countPtr = NULL) const;	// SL

		/**
		 * @brief getValue
		 * @param tag
		 * @param valuePtr
		 * @param countPtr
		 * @return
		 */
		Status getValue(const DcmTagKey& tag, const Float32*& valuePtr, Ulong* countPtr = NULL) const;	// FL

		/**
		 * @brief getValue
		 * @param tag
		 * @param valuePtr
		 * @param countPtr
		 * @return
		 */
		Status getValue(const DcmTagKey& tag, const Float64*& valuePtr, Ulong* countPtr = NULL) const;	// FD

		////////////////////////////////////////////////////////////////////////////////////////////////////////////

		/**
		 * @brief putDate
		 * @param tag
		 * @param dt
		 * @return
		 */
		Status putDate(const DcmTagKey& tag, const DateTime& dt);								// DA, DT

		/**
		 * @brief putDate
		 * @param tag
		 * @param qdt
		 * @return
		 */
		Status putDate(const DcmTagKey& tag, const QDateTime& qdt);								// DA, DT

		/**
		 * @brief getDate
		 * @param tag
		 * @param dt
		 * @return
		 */
		Status getDate(const DcmTagKey& tag, DateTime& dt) const;								// DA, DT

		/**
		 * @brief getDate
		 * @param tag
		 * @param qdt
		 * @return
		 */
		Status getDate(const DcmTagKey& tag, QDateTime& qdt) const;								// DA, DT

		/**
		 * @brief putDateTime
		 * @param dtag
		 * @param ttag
		 * @param dt
		 * @return
		 */
		Status putDateTime(const DcmTagKey& dtag, const DcmTagKey& ttag, const DateTime& dt);	// DA, TM

		/**
		 * @brief putDateTime
		 * @param dtag
		 * @param ttag
		 * @param qdt
		 * @return
		 */
		Status putDateTime(const DcmTagKey& dtag, const DcmTagKey& ttag, const QDateTime& qdt);	// DA, TM

		/**
		 * @brief getDateTime
		 * @param dtag
		 * @param ttag
		 * @param dt
		 * @return
		 */
		Status getDateTime(const DcmTagKey& dtag, const DcmTagKey& ttag, DateTime& dt) const;	// DA, TM

		/**
		 * @brief getDateTime
		 * @param dtag
		 * @param ttag
		 * @param qdt
		 * @return
		 */
		Status getDateTime(const DcmTagKey& dtag, const DcmTagKey& ttag, QDateTime& qdt) const;	// DA, TM

		////////////////////////////////////////////////////////////////////////////////////////////////////////////

		/**
		 * @brief putItem
		 * @param tag
		 * @param item
		 * @param pos
		 * @return
		 */
		Status putItem(const DcmTagKey& tag, Item& item, Sint32 pos = POS_APPEND);

		/**
		 * @brief getItem
		 * @param tag
		 * @param item
		 * @param pos
		 * @return
		 */
		Status getItem(const DcmTagKey& tag, Item& item, Sint32 pos = 0) const;

		/**
		 * @brief getItemCount
		 * @param tag
		 * @return
		 */
		Sint32 getItemCount(const DcmTagKey& tag) const;

		////////////////////////////////////////////////////////////////////////////////////////////////////////////

		/**
		 * @brief putCode
		 * @param codeValue
		 * @param codingSchemeDesignator
		 * @param codingSchemeVersion
		 * @param codeMeaning
		 * @return
		 */
		Status putCode(const String& codeValue, const String& codingSchemeDesignator, const String& codingSchemeVersion, const String& codeMeaning);

		/**
		 * @brief putCode
		 * @param codeValue
		 * @param codingSchemeDesignator
		 * @param codingSchemeVersion
		 * @param codeMeaning
		 * @return
		 */
		Status putCode(const QString& codeValue, const QString& codingSchemeDesignator, const QString& codingSchemeVersion, const QString& codeMeaning);

		/**
		 * @brief getCode
		 * @param codeValue
		 * @param codingSchemeDesignator
		 * @param codingSchemeVersion
		 * @param codeMeaning
		 * @return
		 */
		Status getCode(String& codeValue, String& codingSchemeDesignator, String& codingSchemeVersion, String& codeMeaning) const;

		/**
		 * @brief getCode
		 * @param codeValue
		 * @param codingSchemeDesignator
		 * @param codingSchemeVersion
		 * @param codeMeaning
		 * @return
		 */
		Status getCode(QString& codeValue, QString& codingSchemeDesignator, QString& codingSchemeVersion, QString& codeMeaning) const;

		/**
		 * @brief putCode
		 * @param tag
		 * @param item
		 * @param codeValue
		 * @param codingSchemeDesignator
		 * @param codingSchemeVersion
		 * @param codeMeaning
		 * @param pos
		 * @return
		 */
		Status putCode(const DcmTagKey& tag, Item& item,
						const String& codeValue, const String& codingSchemeDesignator, const String& codingSchemeVersion, const String& codeMeaning,
						Sint32 pos = POS_APPEND);

		/**
		 * @brief putCode
		 * @param tag
		 * @param item
		 * @param codeValue
		 * @param codingSchemeDesignator
		 * @param codingSchemeVersion
		 * @param codeMeaning
		 * @param pos
		 * @return
		 */
		Status putCode(const DcmTagKey& tag, Item& item,
						const QString& codeValue, const QString& codingSchemeDesignator, const QString& codingSchemeVersion, const QString& codeMeaning,
						Sint32 pos = POS_APPEND);

		/**
		 * @brief getCode
		 * @param tag
		 * @param item
		 * @param codeValue
		 * @param codingSchemeDesignator
		 * @param codingSchemeVersion
		 * @param codeMeaning
		 * @param pos
		 * @return
		 */
		Status getCode(const DcmTagKey& tag, Item& item,
						String& codeValue, String& codingSchemeDesignator, String& codingSchemeVersion, String& codeMeaning,
						Sint32 pos = 0) const;

		/**
		 * @brief getCode
		 * @param tag
		 * @param item
		 * @param codeValue
		 * @param codingSchemeDesignator
		 * @param codingSchemeVersion
		 * @param codeMeaning
		 * @param pos
		 * @return
		 */
		Status getCode(const DcmTagKey& tag, Item& item,
						QString& codeValue, QString& codingSchemeDesignator, QString& codingSchemeVersion, QString& codeMeaning,
						Sint32 pos = 0) const;

		////////////////////////////////////////////////////////////////////////////////////////////////////////////

		/**
		 * @brief putRefSOP
		 * @param sopClassUID
		 * @param sopInstanceUID
		 * @return
		 */
		Status putRefSOP(const String& sopClassUID, const String& sopInstanceUID);

		/**
		 * @brief putRefSOP
		 * @param sopClassUID
		 * @param sopInstanceUID
		 * @return
		 */
		Status putRefSOP(const QString& sopClassUID, const QString& sopInstanceUID);

		/**
		 * @brief getRefSOP
		 * @param sopClassUID
		 * @param sopInstanceUID
		 * @return
		 */
		Status getRefSOP(String& sopClassUID, String& sopInstanceUID) const;

		/**
		 * @brief getRefSOP
		 * @param sopClassUID
		 * @param sopInstanceUID
		 * @return
		 */
		Status getRefSOP(QString& sopClassUID, QString& sopInstanceUID) const;

		/**
		 * @brief putRefSOP
		 * @param tag
		 * @param item
		 * @param sopClassUID
		 * @param sopInstanceUID
		 * @param pos
		 * @return
		 */
		Status putRefSOP(const DcmTagKey& tag, Item& item, const String& sopClassUID, const String& sopInstanceUID, Sint32 pos = POS_APPEND);

		/**
		 * @brief putRefSOP
		 * @param tag
		 * @param item
		 * @param sopClassUID
		 * @param sopInstanceUID
		 * @param pos
		 * @return
		 */
		Status putRefSOP(const DcmTagKey& tag, Item& item, const QString& sopClassUID, const QString& sopInstanceUID, Sint32 pos = POS_APPEND);

		/**
		 * @brief getRefSOP
		 * @param tag
		 * @param item
		 * @param sopClassUID
		 * @param sopInstanceUID
		 * @param pos
		 * @return
		 */
		Status getRefSOP(const DcmTagKey& tag, Item& item, String& sopClassUID, String& sopInstanceUID, Sint32 pos = 0) const;

		/**
		 * @brief getRefSOP
		 * @param tag
		 * @param item
		 * @param sopClassUID
		 * @param sopInstanceUID
		 * @param pos
		 * @return
		 */
		Status getRefSOP(const DcmTagKey& tag, Item& item, QString& sopClassUID, QString& sopInstanceUID, Sint32 pos = 0) const;

		////////////////////////////////////////////////////////////////////////////////////////////////////////////

		/**
		 * @brief getVR
		 * @param tag
		 * @param vr
		 * @return
		 */
		Status getVR(const DcmTagKey& tag, DcmEVR& vr) const;

		/**
		 * @brief getVM
		 * @param tag
		 * @param vm
		 * @return
		 */
		Status getVM(const DcmTagKey& tag, Uint32& vm) const;

		/**
		 * @brief getVL
		 * @param tag
		 * @param vl
		 * @return
		 */
		Status getVL(const DcmTagKey& tag, Uint32& vl) const;

		/**
		 * @brief getVR
		 * @param tag
		 * @return
		 */
		DcmEVR getVR(const DcmTagKey& tag) const;

		/**
		 * @brief getVM
		 * @param tag
		 * @return
		 */
		Sint32 getVM(const DcmTagKey& tag) const;

		/**
		 * @brief getVL
		 * @param tag
		 * @return
		 */
		Sint32 getVL(const DcmTagKey& tag) const;

		/**
		 * @brief hasTag
		 * @param tag
		 * @return
		 */
		bool hasTag(const DcmTagKey& tag) const;

		/**
		 * @brief hasTagValue
		 * @param tag
		 * @return
		 */
		bool hasTagValue(const DcmTagKey& tag) const;

		/**
		 * @brief getTagList
		 * @param tagList
		 * @return
		 */
		Status getTagList(TagList& tagList) const;

		virtual bool isValidTag(const DcmTagKey& tag);

		////////////////////////////////////////////////////////////////////////////////////////////////////////////

		/**
		 * @brief exportToXML
		 * @param xmlFilename
		 * @return
		 */
		Status exportToXML(const String xmlFilename) const;

		/**
		 * @brief exportToXML
		 * @param xmlFilename
		 * @return
		 */
		Status exportToXML(const QString xmlFilename) const;

		/**
		 * @brief exportToXML
		 * @param parentElement
		 * @return
		 */
		Status exportToXML(QDomElement& parentElement) const;

		/**
		 * @brief exportPixelDataToXML
		 * @param tag
		 * @param dcmElement
		 * @return
		 */
		virtual Status exportPixelDataToXML(const DcmTagKey& tag, QDomElement& dcmElement) const;

		/**
		 * @brief importFromXML
		 * @param xmlFilename
		 * @return
		 */
		Status importFromXML(const String xmlFilename);

		/**
		 * @brief importFromXML
		 * @param xmlFilename
		 * @return
		 */
		Status importFromXML(const QString xmlFilename);

		/**
		 * @brief importFromXML
		 * @param parentElement
		 * @return
		 */
		Status importFromXML(const QDomElement& parentElement);

		/**
		 * @brief importPixelDataFromXML
		 * @param tag
		 * @param dcmElement
		 * @return
		 */
		virtual Status importPixelDataFromXML(const DcmTagKey& tag, const QDomElement& dcmElement);

		////////////////////////////////////////////////////////////////////////////////////////////////////////////

		/**
		 * @brief exportToJSON
		 * @param jsonFilename
		 * @return
		 */
		Status exportToJSON(const String jsonFilename) const;

		/**
		 * @brief exportToJSON
		 * @param jsonFilename
		 * @return
		 */
		Status exportToJSON(const QString jsonFilename) const;

		/**
		 * @brief exportToJSON
		 * @param jsonObject
		 * @return
		 */
		Status exportToJSON(QJsonObject& jsonObject) const;

		/**
		 * @brief exportPixelDataToJSON
		 * @param tag
		 * @param attrValue
		 * @return
		 */
		virtual Status exportPixelDataToJSON(const DcmTagKey& tag, QJsonObject& attrValue) const;

		/**
		 * @brief importFromJSON
		 * @param jsonFilename
		 * @return
		 */
		Status importFromJSON(const String jsonFilename);

		/**
		 * @brief importFromJSON
		 * @param jsonFilename
		 * @return
		 */
		Status importFromJSON(const QString jsonFilename);

		/**
		 * @brief importFromJSON
		 * @param jsonObject
		 * @return
		 */
		Status importFromJSON(const QJsonObject& jsonObject);

		/**
		 * @brief importPixelDataFromJSON
		 * @param tag
		 * @param attrValue
		 * @return
		 */
		virtual Status importPixelDataFromJSON(const DcmTagKey& tag, const QJsonObject& attrValue);

		////////////////////////////////////////////////////////////////////////////////////////////////////////////

		/**
		 * @brief copyValueFrom
		 * @param tag
		 * @param sourceItem
		 * @param sourceTag
		 * @return
		 */
		Status copyValueFrom(const DcmTagKey& tag, const Item* sourceItem, DcmTagKey sourceTag = DcmTagKey(0xffff, 0xffff));

		////////////////////////////////////////////////////////////////////////////////////////////////////////////

		/**
		 * @brief setNLS
		 * @param nls
		 * @return
		 */
		Status setNLS(int nls);

		/**
		 * @brief getNLS
		 * @param nls
		 * @return
		 */
		Status getNLS(int& nls) const;

		/**
		 * @brief setAutoNLS
		 * @param nls
		 * @return
		 */
		Status setAutoNLS(int nls);

		/**
		 * @brief getAutoNLS
		 * @param nls
		 * @return
		 */
		Status getAutoNLS(int& nls) const;

		////////////////////////////////////////////////////////////////////////////////////////////////////////////

		/**
		 * @brief _getDcmItem
		 * @return
		 */
		DcmItem* _getDcmItem(void) const;

		/**
		 * @brief operator DcmItem *
		 */
		operator DcmItem*(void) const;
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//	PixelDataStruct
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	struct DTKSHARED_EXPORT PixelDataFrame
	{
		Uint32	_frameSize;
		Uint8*	_frameData;

		/**
		 * @brief PixelDataFrame
		 * @param size
		 * @param data
		 */
		PixelDataFrame(Uint32 size, Uint8* data) {
			_frameSize = size;
			_frameData = data;
		}
	};

	typedef std::list<PixelDataFrame>			PixelDataFrameListType;

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//	PixelDataConsumer
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	class DTKSHARED_EXPORT PixelDataConsumer
	{
	public:
		/**
		 * @brief onGetPixelData is called on each pixel data fragment
		 * @param encapsulated is true on encapsulated frame, or false on uncompressed frame
		 * @param frameNo frame number (1..n)
		 * @param size size of this fragment
		 * @param data data of this fragment
		 * @param begin is true on first fragment in each frame
		 * @param end is true on last fragment in each frame
		 * @return
		 */
		virtual bool onGetPixelData(bool encapsulated, Uint32 frameNo, Uint32 size, const Uint8* data, bool begin, bool end) = 0;
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//	Dataset
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	class DTKSHARED_EXPORT Dataset :
		public Item
	{
	protected:
		static Logger _logger;

	public:
		/**
		 * @brief Dataset constructor
		 */
		Dataset(void);

		/**
		 * @brief Dataset constructor
		 * @param dcmDataset existing DcmDataset
		 */
		Dataset(DcmDataset* dcmDataset);

		/**
		 * @brief Dataset copy constructor
		 * @param dataset existing Dataset
		 */

		Dataset(const Dataset& dataset);
		/**
		 * @brief ~Dataset destructor
		 */
		virtual ~Dataset(void);

		/**
		 * @brief operator = copy assignment operator
		 * @param dataset existing Dataset
		 * @return self
		 */
		Dataset&operator=(const Dataset& dataset);

		/**
		 * @brief copyFrom copies all attributes from source dataset
		 * @param sourceDataset source Dataset
		 * @return status
		 */
		Status copyFrom(const Dataset* sourceDataset);

		/**
		 * @brief load reads dataset from DICOM stream file
		 * @param filename file name
		 * @param readXfer transfer syntax
		 * @param groupLength whether to include group length tags
		 * @param maxReadLength
		 * @return
		 */
		Status load(const String& filename,
					const E_TransferSyntax readXfer = EXS_Unknown,
					const E_GrpLenEncoding groupLength = EGL_noChange,
					const Uint32 maxReadLength = DCM_MaxReadLength);

		/**
		 * @brief save writes dataset to DICOM stream file
		 * @param filename file name
		 * @param writeXfer transfer syntax
		 * @param encodingType encoding type
		 * @param groupLength whether to include group length tags
		 * @param padEncoding
		 * @param padLength
		 * @param subPadLength
		 * @return
		 */
		Status save(const String& filename,
					const E_TransferSyntax writeXfer = EXS_Unknown,
					const E_EncodingType encodingType = EET_UndefinedLength,
					const E_GrpLenEncoding groupLength = EGL_recalcGL,
					const E_PaddingEncoding padEncoding = EPD_noChange,
					const Uint32 padLength = 0,
					const Uint32 subPadLength = 0);

		////////////////////////////////////////////////////////////////////////////////////////////////////////////

		/**
		 * @brief registerCodecs initializes codecs for transcoding
		 */
		static void registerCodecs(void);

		/**
		 * @brief unregisterCodecs cleans up codecs
		 */
		static void unregisterCodecs(void);

		/**
		 * @brief setTransferSyntax changes transfer syntax
		 * @param ts transfer syntax
		 * @param dcmRepParam transfer syntax-specific paremeters
		 * @return status
		 */
		Status setTransferSyntax(E_TransferSyntax ts, DcmRepresentationParameter* dcmRepParam = NULL);

		/**
		 * @brief getTransferSyntax gets the current transfer syntax
		 * @return transfer syntax
		 */
		E_TransferSyntax getTransferSyntax(void) const;

		////////////////////////////////////////////////////////////////////////////////////////////////////////////

		/**
		 * @brief putPixelData
		 * @param tag pixel data tag: in most cases, DCM_PixelData is used
		 * @param pixelDataFrameList
		 * @return status
		 */
		Status putPixelData(const DcmTagKey& tag, const PixelDataFrameListType& pixelDataFrameList);

		/**
		 * @brief getPixelData gets pixel data
		 * @param tag data tag: in most cases, DCM_PixelData is used
		 * @param consumer pixel data consumer callback
		 * @return status
		 */
		Status getPixelData(const DcmTagKey& tag, PixelDataConsumer* consumer);

		////////////////////////////////////////////////////////////////////////////////////////////////////////////

		/**
		 * @brief isValidTag
		 * @param tag
		 * @return
		 */
		bool isValidTag(const DcmTagKey& tag);

		////////////////////////////////////////////////////////////////////////////////////////////////////////////

		/**
		 * @brief exportPixelDataToXML
		 * @param tag
		 * @param dcmElement
		 * @return
		 */
		Status exportPixelDataToXML(const DcmTagKey& tag, QDomElement& dcmElement) const;

		/**
		 * @brief importPixelDataFromXML
		 * @param tag
		 * @param dcmElement
		 * @return
		 */
		Status importPixelDataFromXML(const DcmTagKey& tag, const QDomElement& dcmElement);

		/**
		 * @brief exportPixelDataToJSON
		 * @param tag
		 * @param attrValue
		 * @return
		 */
		Status exportPixelDataToJSON(const DcmTagKey& tag, QJsonObject& attrValue) const;

		/**
		 * @brief importPixelDataFromJSON
		 * @param tag
		 * @param attrValue
		 * @return
		 */
		Status importPixelDataFromJSON(const DcmTagKey& tag, const QJsonObject& attrValue);

		////////////////////////////////////////////////////////////////////////////////////////////////////////////

		/**
		 * @brief deidentify de-identifies patient identity according to DICOM PS 3.15 Attribute Confidentiality Profiles
		 * @return status
		 */
		Status deidentify(void);

		////////////////////////////////////////////////////////////////////////////////////////////////////////////

		/**
		 * @brief setDefaultNLS sets default encoding/decoding scheme
		 * @param nls default encoding/decoding scheme
		 * @return status
		 */
		static Status setDefaultNLS(int nls);

		/**
		 * @brief getDefaultNLS gets default encoding/decoding scheme
		 * @param nls default encoding/decoding scheme
		 * @return status
		 */
		static Status getDefaultNLS(int& nls);

		/**
		 * @brief _getDcmDataset returns internal DcmDataset
		 * @return DcmDataset*
		 */
		DcmDataset* _getDcmDataset(void) const;

		/**
		 * @brief operator DcmDataset * returns internal DcmDataset
		 */
		operator DcmDataset*(void) const;

		////////////////////////////////////////////////////////////////////////////////////////////////////////////

		/**
		 * @brief prepareCompositeIODFromMWL copies/generates required attributes for DICOM Composite IOD from MWL
		 * @param mwlDatasetList MWL dataset list
		 * @return status
		 */
		Status prepareCompositeIODFromMWL(const std::list<Dataset*> mwlDatasetList);		// Prepare Composite IOD from MWL

		/**
		 * @brief prepareMPPSFromMWL copies/generates required attributes for DICOM MPPS from MWL
		 * @param mwlDatasetList MWL dataset list
		 * @return status
		 */
		Status prepareMPPSFromMWL(const std::list<Dataset*> mwlDatasetList);				// Prepare MPPS from MWL

		/**
		 * @brief prepareMPPSFromCompositeIOD copies required attributes for DICOM MPPS from Composite IOD
		 * @param dataset Composite IOD dataset
		 * @return status
		 */
		Status prepareMPPSFromCompositeIOD(const Dataset* dataset);							// Prepare MPPS from Composite IOD

		/**
		 * @brief prepareCompositeIODFromMPPS copies required attributes for DICOM Composite IOD from MPPS
		 * @param dataset MPPS dataset
		 * @return status
		 */
		Status prepareCompositeIODFromMPPS(const Dataset* dataset);							// Prepare Composite IOD from MPPS

		/**
		 * @brief prepareMPPS2FromCompositeIOD copies required attributes for DICOM MPPS from Composite IOS
		 * @param dataset Composite IOD dataset
		 * @return status
		 */
		Status prepareMPPS2FromCompositeIOD(const Dataset* dataset);						// Prepare MPPS from Composite IOD

		/**
		 * @brief prepareStorageCommitmentFromCompositeIOD copies/generates required attributes for Storage Commitment from Composite IOD
		 * @param dataset Composite IOD dataset
		 * @return status
		 */
		Status prepareStorageCommitmentFromCompositeIOD(const Dataset* dataset);			// Prepare Storage Commitment from Composite IOD
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//	MetaInfo
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	class DTKSHARED_EXPORT MetaInfo :
		public Item
	{
	public:
		/**
		 * @brief MetaInfo constructor
		 */
		MetaInfo(void);

		/**
		 * @brief MetaInfo constructor
		 * @param dcmMetaInfo existing DcmMetaInfo
		 */
		MetaInfo(DcmMetaInfo* dcmMetaInfo);

		/**
		 * @brief MetaInfo copy constructor
		 * @param metaInfo existing MetaInfo
		 */
		MetaInfo(const MetaInfo& metaInfo);

		/**
		 * @brief ~MetaInfo destructor
		 */
		virtual ~MetaInfo(void);

		/**
		 * @brief operator = copy assignment operator
		 * @param metaInfo existing MetaInfo
		 * @return
		 */
		MetaInfo& operator=(const MetaInfo& metaInfo);

		////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool isValidTag(const DcmTagKey& tag);
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//	File
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	class DTKSHARED_EXPORT File
	{
	protected:
		DcmFileFormat* _dcmFileFormatPtr;
		MetaInfo _metaInfo;
		Dataset _dataset;

	public:
		/**
		 * @brief File
		 */
		File(void);

		/**
		 * @brief File
		 * @param file
		 */
		File(const File& file);

		/**
		 * @brief ~File
		 */
		virtual ~File(void);

		/**
		 * @brief operator =
		 * @param file
		 * @return
		 */
		File& operator=(const File& file);

		/**
		 * @brief load
		 * @param filename
		 * @param readXfer
		 * @param groupLength
		 * @param maxReadLength
		 * @param readMode
		 * @return
		 */
		Status load(const String& filename,
					const E_TransferSyntax readXfer = EXS_Unknown,
					const E_GrpLenEncoding groupLength = EGL_noChange,
					const Uint32 maxReadLength = DCM_MaxReadLength,
					const E_FileReadMode readMode = ERM_autoDetect);

		/**
		 * @brief save
		 * @param strFilename
		 * @param writeXfer
		 * @param encodingType
		 * @param groupLength
		 * @param padEncoding
		 * @param padLength
		 * @param subPadLength
		 * @param fileWriteMode
		 * @return
		 */
		Status save(const String& strFilename,
					const E_TransferSyntax writeXfer = EXS_Unknown,
					const E_EncodingType encodingType = EET_UndefinedLength,
					const E_GrpLenEncoding groupLength = EGL_recalcGL,
					const E_PaddingEncoding padEncoding = EPD_noChange,
					const Uint32 padLength = 0,
					const Uint32 subPadLength = 0,
					const E_FileWriteMode fileWriteMode = EWM_fileformat);

		/**
		 * @brief clear
		 * @return
		 */
		Status clear(void);

		/**
		 * @brief print
		 * @param filename
		 * @return
		 */
		Status print(const String& filename);

		/**
		 * @brief printXML
		 * @param filename
		 * @return
		 */
		Status printXML(const String& filename);

		/**
		 * @brief metaInfo
		 * @return
		 */
		MetaInfo& metaInfo(void);

		/**
		 * @brief dataset
		 * @return
		 */
		Dataset& dataset(void);

		/**
		 * @brief getMetaInfo
		 * @return
		 */
		MetaInfo* getMetaInfo(void);

		/**
		 * @brief getDataset
		 * @return
		 */
		Dataset* getDataset(void);

		/**
		 * @brief getMetaInfo
		 * @return
		 */
		const MetaInfo* getMetaInfo(void) const;

		/**
		 * @brief getDataset
		 * @return
		 */
		const Dataset* getDataset(void) const;

		/**
		 * @brief getDcmFileFormat
		 * @return
		 */
		DcmFileFormat* getDcmFileFormat(void) const;

		/**
		 * @brief operator DcmFileFormat *
		 */
		operator DcmFileFormat*(void) const;

		////////////////////////////////////////////////////////////////////////////////////////////////////////////

		/**
		 * @brief exportToXML
		 * @param xmlFilename
		 * @return
		 */
		Status exportToXML(const String xmlFilename) const;

		/**
		 * @brief exportToXML
		 * @param xmlFilename
		 * @return
		 */
		Status exportToXML(const QString xmlFilename) const;

		/**
		 * @brief exportToXML
		 * @param parentElement
		 * @return
		 */
		Status exportToXML(QDomElement& parentElement) const;

		/**
		 * @brief importFromXML
		 * @param xmlFilename
		 * @return
		 */
		Status importFromXML(const String xmlFilename);

		/**
		 * @brief importFromXML
		 * @param xmlFilename
		 * @return
		 */
		Status importFromXML(const QString xmlFilename);

		/**
		 * @brief importFromXML
		 * @param parentElement
		 * @return
		 */
		Status importFromXML(const QDomElement& parentElement);

		////////////////////////////////////////////////////////////////////////////////////////////////////////////

		/**
		 * @brief exportToJSON
		 * @param jsonFilename
		 * @return
		 */
		Status exportToJSON(const String jsonFilename) const;

		/**
		 * @brief exportToJSON
		 * @param jsonFilename
		 * @return
		 */
		Status exportToJSON(const QString jsonFilename) const;

		/**
		 * @brief exportToJSON
		 * @param jsonObject
		 * @return
		 */
		Status exportToJSON(QJsonObject& jsonObject) const;

		/**
		 * @brief importFromJSON
		 * @param jsonFilename
		 * @return
		 */
		Status importFromJSON(const String jsonFilename);

		/**
		 * @brief importFromJSON
		 * @param jsonFilename
		 * @return
		 */
		Status importFromJSON(const QString jsonFilename);

		/**
		 * @brief importFromJSON
		 * @param jsonObject
		 * @return
		 */
		Status importFromJSON(const QJsonObject& jsonObject);
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//	DirRecord
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	class DTKSHARED_EXPORT DirRecord :
		public Item
	{
	protected:
		String _fileId;

	public:
		/**
		 * @brief DirRecord
		 */
		DirRecord(void);

		/**
		 * @brief DirRecord
		 * @param dcmDirRecord
		 */
		DirRecord(DcmDirectoryRecord* dcmDirRecord);

		/**
		 * @brief DirRecord
		 * @param dirRecord
		 */
		DirRecord(const DirRecord& dirRecord);

		/**
		 * @brief ~DirRecord
		 */
		virtual ~DirRecord(void);

		/**
		 * @brief setFileId
		 * @param fileId
		 * @return
		 */
		Status setFileId(const String& fileId);

		/**
		 * @brief getFileId
		 * @param fileId
		 * @return
		 */
		Status getFileId(String& fileId) const;

		/**
		 * @brief fileId
		 * @return
		 */
		String fileId(void) const;

		/**
		 * @brief putRecord
		 * @param recordType
		 * @param fileId
		 * @param subRecord
		 * @return
		 */
		Status putRecord(const E_DirRecType recordType, const String& fileId, DirRecord& subRecord);

		/**
		 * @brief getRecord
		 * @param recordType
		 * @param file
		 * @param subRecord
		 * @return
		 */
		Status getRecord(const E_DirRecType recordType, const File &file, DirRecord& subRecord);

		/**
		 * @brief copyValue
		 * @param file
		 * @return
		 */
		Status copyValue(const File &file);

		/**
		 * @brief getRecordType
		 * @return
		 */
		E_DirRecType getRecordType(void);

		/**
		 * @brief getRecordTypeFromSOPClass
		 * @param sopClassUID
		 * @return
		 */
		static E_DirRecType getRecordTypeFromSOPClass(const String& sopClassUID);

		/**
		 * @brief getRecordTypeName
		 * @param recordType
		 * @return
		 */
		static String getRecordTypeName(const E_DirRecType recordType);
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//	Dir
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	class DTKSHARED_EXPORT Dir
	{
	protected:
		static Logger _logger;
		DcmDicomDir* _dcmDir;
		String _baseDir;
		StringList _sourceFileList;
		StringList _targetFileList;

	public:
		/**
		 * @brief Dir
		 */
		Dir(void);

		/**
		 * @brief Dir
		 * @param dicomdir
		 */
		Dir(const String& dicomdir);

		/**
		 * @brief ~Dir
		 */
		virtual ~Dir(void);

		/**
		 * @brief getRootRecord
		 * @param rootRecord
		 * @return
		 */
		Status getRootRecord(DirRecord& rootRecord);

		/**
		 * @brief addFile
		 * @param filename
		 * @return
		 */
		OFCondition addFile(const String &filename);

		/**
		 * @brief save
		 * @param encodingType
		 * @param groupLength
		 * @return
		 */
		OFCondition save(const E_EncodingType encodingType = EET_UndefinedLength, const E_GrpLenEncoding groupLength = EGL_withoutGL);

		/**
		 * @brief print
		 * @param filename
		 * @return
		 */
		Status print(const String& filename);

		/**
		 * @brief generateFileId
		 * @return
		 */
		String generateFileId(void);
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//	UIDHelper
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	/**
	 * @brief The UIDHelper class
	 */
	class DTKSHARED_EXPORT UIDHelper
	{
	protected:
		String _uidRoot;

	public:
		/**
		 * @brief UIDHelper
		 * @param uidRoot
		 */
		UIDHelper(const String& uidRoot);

		/**
		 * @brief generateUID
		 * @return
		 */
		String generateUID(void);

		/**
		 * @brief getSOPClassName
		 * @param sopClassUID
		 * @return
		 */
		static String getSOPClassName(const String& sopClassUID);

		/**
		 * @brief getSOPClassUID
		 * @param sopClassName
		 * @return
		 */
		static String getSOPClassUID(const String& sopClassName);

		/**
		 * @brief isStorageSOPClassUID
		 * @param sopClassUID
		 * @return
		 */
		static bool isStorageSOPClassUID(const String& sopClassUID);

		/**
		 * @brief isImageSOPClassUID
		 * @param strSOPClassUID
		 * @return
		 */
		static bool isImageSOPClassUID(const String& strSOPClassUID);

		/**
		 * @brief generateUID
		 * @param uidRoot
		 * @return
		 */
		static String generateUID(const String& uidRoot);

		/**
		 * @brief getModality
		 * @param sopClassUID
		 * @return
		 */
		static String getModality(const String& sopClassUID);

		/**
		 * @brief getDataSize
		 * @param sopClassUID
		 * @return
		 */
		static Uint32 getDataSize(const String& sopClassUID);
	};
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef	DCMTK_LOG4CPLUS_ERROR_FMT
#define	DCMTK_LOG4CPLUS_ERROR_FMT(logger, logFmt, ...)
#endif

#ifndef	DCMTK_LOG4CPLUS_WARN_FMT
#define	DCMTK_LOG4CPLUS_WARN_FMT(logger, logFmt, ...)
#endif

#ifndef	DCMTK_LOG4CPLUS_INFO_FMT
#define	DCMTK_LOG4CPLUS_INFO_FMT(logger, logFmt, ...)
#endif

#ifndef	DCMTK_LOG4CPLUS_DEBUG_FMT
#define	DCMTK_LOG4CPLUS_DEBUG_FMT(logger, logFmt, ...)
#endif

#ifndef	DCMTK_LOG4CPLUS_TRACE_FMT
#define	DCMTK_LOG4CPLUS_TRACE_FMT(logger, logFmt, ...)
#endif

#endif // DTK_H
