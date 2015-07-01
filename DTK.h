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

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

//#define	QSTR_TO_CSTR(s)					((s).toStdString().c_str())
#define	QSTR_TO_CSTR(s)					((s).toLocal8Bit().constData())
#define	QSTR_TO_DSTR(s)					(dcm::String(QSTR_TO_CSTR((s))))
#define	DSTR_TO_CSTR(s)					((s).c_str())
#define	DSTR_TO_QSTR(s)					(QString((s).c_str()))

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define	DTK_VERSION_1_0_0				_T("1.0.0")
#define	DTK_VERSION						DTK_VERSION_1_0_0

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
		CEchoRQ(Dataset* dataset = NULL);
		virtual void setIDs(T_ASC_Association* ascAssoc);
	};

	class DTKSHARED_EXPORT CEchoRSP : public Message
	{
	public:
		CEchoRSP(const Message& req, Uint16 status = STATUS_Success, Dataset* dataset = NULL);
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	class DTKSHARED_EXPORT CStoreRQ : public Message
	{
	public:
		CStoreRQ(Dataset* dataset);
		virtual void setIDs(T_ASC_Association* ascAssoc);
	};

	class DTKSHARED_EXPORT CStoreRSP : public Message
	{
	public:
		CStoreRSP(const Message& req, Uint16 status = STATUS_Success, Dataset* dataset = NULL);
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	class DTKSHARED_EXPORT CFindRQ : public Message
	{
	public:
		CFindRQ(const String& sopClassUID, Dataset* dataset);
		virtual void setIDs(T_ASC_Association* ascAssoc);
	};

	class DTKSHARED_EXPORT CFindRSP : public Message
	{
	public:
		CFindRSP(const Message& req, Uint16 status = STATUS_Success, Dataset* dataset = NULL);
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	class DTKSHARED_EXPORT CGetRQ : public Message
	{
	public:
		CGetRQ(const String& sopClassUID, Dataset* dataset);
		virtual void setIDs(T_ASC_Association* ascAssoc);
	};

	class DTKSHARED_EXPORT CGetRSP : public Message
	{
	public:
		CGetRSP(const Message& req, Uint16 status = STATUS_Success, Dataset* dataset = NULL,
				int remaining = -1, int completed = -1, int failure = -1, int warning = -1);
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	class DTKSHARED_EXPORT CMoveRQ : public Message
	{
	public:
		CMoveRQ(const String& sopClassUID, const String& destination, Dataset* dataset);
		virtual void setIDs(T_ASC_Association* ascAssoc);
	};

	class DTKSHARED_EXPORT CMoveRSP : public Message
	{
	public:
		CMoveRSP(const Message& req, Uint16 status = STATUS_Success, Dataset* dataset = NULL,
				 int remaining = -1, int completed = -1, int failure = -1, int warning = -1);
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	class DTKSHARED_EXPORT CCancelRQ : public Message
	{
	public:
		CCancelRQ(const Message& req);
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	class DTKSHARED_EXPORT NEventReportRQ : public Message
	{
	public:
		NEventReportRQ(const String& sopClassUID, const String& sopInstanceUID, Uint16 eventTypeId, Dataset* dataset);
		virtual void setIDs(T_ASC_Association* ascAssocPtr);
	};

	class DTKSHARED_EXPORT NEventReportRSP : public Message
	{
	public:
		NEventReportRSP(const Message& req, Uint16 status = STATUS_Success, Dataset* datasetPtr = NULL);
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	class DTKSHARED_EXPORT NGetRQ : public Message
	{
	public:
		NGetRQ(const String& sopClassUID, const String& sopInstanceUID, const DcmTagKey* dcmTag, int dcmTagCount, Dataset* dataset = NULL);
		virtual ~NGetRQ(void);
		virtual void setIDs(T_ASC_Association* ascAssoc);
	};

	class DTKSHARED_EXPORT NGetRSP : public Message
	{
	public:
		NGetRSP(const Message& req, Uint16 nStatus = STATUS_Success, Dataset* _dcmDataset = NULL);
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	class DTKSHARED_EXPORT NSetRQ : public Message
	{
	public:
		NSetRQ(const String& sopClassUID, const String& sopInstanceUID, Dataset* dataset);
		virtual void setIDs(T_ASC_Association* ascAssoc);
	};

	class DTKSHARED_EXPORT NSetRSP : public Message
	{
	public:
		NSetRSP(const Message& req, Uint16 status = STATUS_Success, Dataset* dataset = NULL);
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	class DTKSHARED_EXPORT NActionRQ : public Message
	{
	public:
		NActionRQ(const String& sopClassUID, const String& sopInstanceUID, Uint16 actionTypeId, Dataset* dataset = NULL);
		virtual void setIDs(T_ASC_Association* ascAssoc);
	};

	class DTKSHARED_EXPORT NActionRSP : public Message
	{
	public:
		NActionRSP(const Message& req, Uint16 status = STATUS_Success, Dataset* dataset = NULL);
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	class DTKSHARED_EXPORT NCreateRQ : public Message
	{
	public:
		NCreateRQ(const String& sopClassUID, const String& sopInstanceUID, Dataset* dataset);
		virtual void setIDs(T_ASC_Association* ascAssocPtr);
	};

	class DTKSHARED_EXPORT NCreateRSP : public Message
	{
	public:
		NCreateRSP(const Message& req, Uint16 status = STATUS_Success, Dataset* dataset = NULL);
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	class DTKSHARED_EXPORT NDeleteRQ : public Message
	{
	public:
		NDeleteRQ(const String& sopClassUID, const String& sopInstanceUID, Dataset* dataset = NULL);
		virtual void setIDs(T_ASC_Association* ascAssoc);
	};

	class DTKSHARED_EXPORT NDeleteRSP : public Message
	{
	public:
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
		Service(void)
		{
			_role = ASC_SC_ROLE_DEFAULT;
		}

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
		ServiceList(bool includeEcho = true);

		bool hasService(const String& abstractSyntax, TransferSyntax transferSyntax);
		bool hasService(const String& abstractSyntax, const String& transferSyntaxUID);

		void addService(const String& abstractSyntax, TransferSyntax transferSyntax);
		void addService(const String& abstractSyntax, const String& transferSyntaxUID);

		void addService(const String& abstractSyntax, const TransferSyntaxList& transferSyntaxList);
		void addService(const String& abstractSyntax, const StringList& transferSyntaxUIDList);

		void addVerificationService(void);

		void addAllStorageSCUServices(void);
		void addAllStorageSCPServices(void);

		void addAllQueryRetrieveServices(void);

		void addMWLService(void);
		void addMPPSService(void);

		void addStorageCommitmentSCUService(void);
		void addStorageCommitmentSCPService(void);

		void addGrayscalePrintService(void);
		void addColorPrintService(void);

		ServiceListType::size_type size() const;
		ServiceListType::const_iterator cbegin() const;
		ServiceListType::const_iterator cend() const;

		static const ServiceList& getBasicSCUServiceList(void);
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
		String applicationContextName;

		String callingAETitle;
		String callingPresentationAddress;
		String callingImplementationClassUID;
		String callingImplementationVersion;

		String calledAETitle;
		String calledPresentationAddress;
		String calledImplementationClassUID;
		String calledImplementationVersion;

		String respondingAETitle;

		Uint32 maxPDUSizeOfMine;
		Uint32 maxPDUSizeOfPeer;

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
		Association(AppEntity* appEntity);
		virtual ~Association(void);

		Status close(void);

		Status sendMessage(Message& req);
		Status receiveMessage(Message& rsp, int timeout);

		Status getAssocInfo(AssociationInfo& assocInfo);

		static Status verify(const String& localAETitle, const String& aetitle, const String& hostname, const Uint16 port, const Uint16 timeout = 15);
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
		AssociationRequestor(AppEntity* appEntity);
		virtual ~AssociationRequestor(void);

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
		AssociationListener(AppEntity* appEntity);
		virtual ~AssociationListener(void);

		Status	listen(const int nTimeout);
		Status	accept(const ServiceList& serviceList = DCM_BASIC_SCP_SERVICE_LIST);
		Status	reject(void);
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
		AssociationListenerMaster(AppEntity* appEntity);
		virtual ~AssociationListenerMaster(void);

		Status startlisten(void);
		Status stoplisten(void);
		bool isListening(void);

	protected:
		virtual void run(void);
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
		AssociationListenerWorker(AssociationListenerMaster* masterListener);
		virtual ~AssociationListenerWorker(void);

		Status startlisten(void);
		Status stoplisten(void);	// not required, nor implemented
		bool isListening(void);

	protected:
		virtual void run(void);
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

		Item(void);
		Item(DcmItem* _dcmItem);
		Item(const Item& dcmItem);
		virtual ~Item(void);

		Item& operator=(const Item& item);
		Status copyFrom(const Item* sourceItem);

		Status set(DcmItem* _dcmItem, int parentNLS = NLS::DEFAULT);
		Status clear(void);

		Status print(const char* filename) const;
		Status print(const String& filename) const;
		Status print(const QString& filename) const;

		Status printXML(const char* filename) const;
		Status printXML(const String& filename) const;
		Status printXML(const QString& filename) const;

		////////////////////////////////////////////////////////////////////////////////////////////////////////////

		Status putString(const DcmTagKey& tag, const char* value, int nls = NLS::AUTO);
		Status putString(const DcmTagKey& tag, const String& value, int nls = NLS::AUTO);
		Status putString(const DcmTagKey& tag, const QString& value, int nls = NLS::AUTO);
			// AE, AS, AT, CS, DA, DS, DT, FL, FD, IS, LO, LT, OB, OF, OW, PN, SH, SL, SS, ST, TM, UI, UL, US, UT

#if SIZEOF_LONG == 4
		Status putValue(const DcmTagKey& tag, Uint value, Uint32 pos = 0);
		Status putValue(const DcmTagKey& tag, Sint value, Uint32 pos = 0);
#endif
		Status putValue(const DcmTagKey& tag, Uint16 value, Uint32 pos = 0);							// US
		Status putValue(const DcmTagKey& tag, Sint16 value, Uint32 pos = 0);							// SS
		Status putValue(const DcmTagKey& tag, Uint32 value, Uint32 pos = 0);							// UL
		Status putValue(const DcmTagKey& tag, Sint32 value, Uint32 pos = 0);							// SL
		Status putValue(const DcmTagKey& tag, Float32 value, Uint32 pos = 0);							// FL
		Status putValue(const DcmTagKey& tag, Float64 value, Uint32 pos = 0);							// FD

		Status putValue(const DcmTagKey& tag, const Uint8* valuePtr, Uint32 length);					// AT, OW, US/SS, OB/OW
		Status putValue(const DcmTagKey& tag, const Uint16* valuePtr, Uint32 length);					// AT, OW, US/SS, OB/OW
		Status putValue(const DcmTagKey& tag, const Sint16* valuePtr, Uint32 length);					// SS, US/SS
		Status putValue(const DcmTagKey& tag, const Float32* valuePtr, Uint32 length);					// FL
		Status putValue(const DcmTagKey& tag, const Float64* valuePtr, Uint32 length);					// FD

		Status putEmpty(const DcmTagKey& tag);
		Status removeValue(const DcmTagKey& tag);

		////////////////////////////////////////////////////////////////////////////////////////////////////////////

		Status getString(const DcmTagKey& tag, String& value, Sint32 pos = -1, int nls = NLS::AUTO) const;
		Status getString(const DcmTagKey& tag, QString& value, Sint32 pos = -1, int nls = NLS::AUTO) const;
			// AE, AS, AT, CS, DA, DS, DT, FL, FD, IS, LO, LT, OB, OF, OW, PN, SH, SL, SS, ST, TM, UI, UL, US, UT

#if SIZEOF_LONG == 4
		Status getValue(const DcmTagKey& tag, Uint& value, Uint32 pos = 0) const;
		Status getValue(const DcmTagKey& tag, Sint& value, Uint32 pos = 0) const;
#endif
		Status getValue(const DcmTagKey& tag, Uint16& value, Uint32 pos = 0) const;						// US
		Status getValue(const DcmTagKey& tag, Uint32& value, Uint32 pos = 0) const;						// UL
		Status getValue(const DcmTagKey& tag, Sint16& value, Uint32 pos = 0) const;						// SS
		Status getValue(const DcmTagKey& tag, Sint32& value, Uint32 pos = 0) const;						// SL
		Status getValue(const DcmTagKey& tag, Float32& value, Uint32 pos = 0) const;					// FL
		Status getValue(const DcmTagKey& tag, Float64& value, Uint32 pos = 0) const;					// FD

		Status getValue(const DcmTagKey& tag, Uint8& value, Uint32 pos = 0) const;						// OB
		Status getValue(const DcmTagKey& tag, const Uint8*& valuePtr, Ulong* lengthPtr = NULL) const;	// OB

		Status getValue(const DcmTagKey& tag, const Uint16*& valuePtr, Ulong* countPtr = NULL) const;	// AT, OW, US/SS, OB/OW
		Status getValue(const DcmTagKey& tag, const Uint32*& valuePtr, Ulong* countPtr = NULL) const;	// UL
		Status getValue(const DcmTagKey& tag, const Sint16*& valuePtr, Ulong* countPtr = NULL) const;	// AT, OW, US/SS, OB/OW
		Status getValue(const DcmTagKey& tag, const Sint32*& valuePtr, Ulong* countPtr = NULL) const;	// SL
		Status getValue(const DcmTagKey& tag, const Float32*& valuePtr, Ulong* countPtr = NULL) const;	// FL
		Status getValue(const DcmTagKey& tag, const Float64*& valuePtr, Ulong* countPtr = NULL) const;	// FD

		////////////////////////////////////////////////////////////////////////////////////////////////////////////

		Status putDate(const DcmTagKey& tag, const DateTime& dt);								// DA, DT
		Status putDate(const DcmTagKey& tag, const QDateTime& qdt);								// DA, DT
		Status getDate(const DcmTagKey& tag, DateTime& dt) const;								// DA, DT
		Status getDate(const DcmTagKey& tag, QDateTime& qdt) const;								// DA, DT

		Status putDateTime(const DcmTagKey& dtag, const DcmTagKey& ttag, const DateTime& dt);	// DA, TM
		Status putDateTime(const DcmTagKey& dtag, const DcmTagKey& ttag, const QDateTime& qdt);	// DA, TM
		Status getDateTime(const DcmTagKey& dtag, const DcmTagKey& ttag, DateTime& dt) const;	// DA, TM
		Status getDateTime(const DcmTagKey& dtag, const DcmTagKey& ttag, QDateTime& qdt) const;	// DA, TM

		////////////////////////////////////////////////////////////////////////////////////////////////////////////

		Status putItem(const DcmTagKey& tag, Item& item, Sint32 pos = POS_APPEND);
		Status getItem(const DcmTagKey& tag, Item& item, Sint32 pos = 0) const;
		Sint32 getItemCount(const DcmTagKey& tag) const;

		////////////////////////////////////////////////////////////////////////////////////////////////////////////

		Status putCode(const String& codeValue, const String& codingSchemeDesignator, const String& codingSchemeVersion, const String& codeMeaning);
		Status putCode(const QString& codeValue, const QString& codingSchemeDesignator, const QString& codingSchemeVersion, const QString& codeMeaning);
		Status getCode(String& codeValue, String& codingSchemeDesignator, String& codingSchemeVersion, String& codeMeaning) const;
		Status getCode(QString& codeValue, QString& codingSchemeDesignator, QString& codingSchemeVersion, QString& codeMeaning) const;

		Status putCode(const DcmTagKey& tag, Item& item,
						const String& codeValue, const String& codingSchemeDesignator, const String& codingSchemeVersion, const String& codeMeaning,
						Sint32 pos = POS_APPEND);
		Status putCode(const DcmTagKey& tag, Item& item,
						const QString& codeValue, const QString& codingSchemeDesignator, const QString& codingSchemeVersion, const QString& codeMeaning,
						Sint32 pos = POS_APPEND);
		Status getCode(const DcmTagKey& tag, Item& item,
						String& codeValue, String& codingSchemeDesignator, String& codingSchemeVersion, String& codeMeaning,
						Sint32 pos = 0) const;
		Status getCode(const DcmTagKey& tag, Item& item,
						QString& codeValue, QString& codingSchemeDesignator, QString& codingSchemeVersion, QString& codeMeaning,
						Sint32 pos = 0) const;

		////////////////////////////////////////////////////////////////////////////////////////////////////////////

		Status putRefSOP(const String& sopClassUID, const String& sopInstanceUID);
		Status putRefSOP(const QString& sopClassUID, const QString& sopInstanceUID);
		Status getRefSOP(String& sopClassUID, String& sopInstanceUID) const;
		Status getRefSOP(QString& sopClassUID, QString& sopInstanceUID) const;

		Status putRefSOP(const DcmTagKey& tag, Item& item, const String& sopClassUID, const String& sopInstanceUID, Sint32 pos = POS_APPEND);
		Status putRefSOP(const DcmTagKey& tag, Item& item, const QString& sopClassUID, const QString& sopInstanceUID, Sint32 pos = POS_APPEND);
		Status getRefSOP(const DcmTagKey& tag, Item& item, String& sopClassUID, String& sopInstanceUID, Sint32 pos = 0) const;
		Status getRefSOP(const DcmTagKey& tag, Item& item, QString& sopClassUID, QString& sopInstanceUID, Sint32 pos = 0) const;

		////////////////////////////////////////////////////////////////////////////////////////////////////////////

		Status getVR(const DcmTagKey& tag, DcmEVR& vr) const;
		Status getVM(const DcmTagKey& tag, Uint32& vm) const;
		Status getVL(const DcmTagKey& tag, Uint32& vl) const;

		DcmEVR getVR(const DcmTagKey& tag) const;
		Sint32 getVM(const DcmTagKey& tag) const;
		Sint32 getVL(const DcmTagKey& tag) const;

		bool hasTag(const DcmTagKey& tag) const;
		bool hasTagValue(const DcmTagKey& tag) const;
		Status getTagList(TagList& tagList) const;

		////////////////////////////////////////////////////////////////////////////////////////////////////////////

		Status copyValueFrom(const DcmTagKey& tag, const Item* sourceItem, DcmTagKey sourceTag = DcmTagKey(0xffff, 0xffff));

		////////////////////////////////////////////////////////////////////////////////////////////////////////////

		Status setNLS(int nls);
		Status getNLS(int& nls) const;
		Status setAutoNLS(int nls);
		Status getAutoNLS(int& nls) const;

		////////////////////////////////////////////////////////////////////////////////////////////////////////////

		DcmItem* _getDcmItem(void) const;
		operator DcmItem*(void) const;
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//	PixelDataProducer
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	class DTKSHARED_EXPORT PixelDataProducer
	{
	public:
	};

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
		 * @brief getPixelData gets pixel data
		 * @param tag pixel data tag: in most cases, DCM_PixelData is used
		 * @param consumer pixel data consumer callback
		 * @return status
		 */
		Status getPixelData(const DcmTagKey& tag, PixelDataConsumer* consumer);

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
		File(void);
		File(const File& file);
		virtual ~File(void);

		File& operator=(const File& file);

		Status load(const String& filename,
					const E_TransferSyntax readXfer = EXS_Unknown,
					const E_GrpLenEncoding groupLength = EGL_noChange,
					const Uint32 maxReadLength = DCM_MaxReadLength,
					const E_FileReadMode readMode = ERM_autoDetect);
		Status save(const String& strFilename,
					const E_TransferSyntax writeXfer = EXS_Unknown,
					const E_EncodingType encodingType = EET_UndefinedLength,
					const E_GrpLenEncoding groupLength = EGL_recalcGL,
					const E_PaddingEncoding padEncoding = EPD_noChange,
					const Uint32 padLength = 0,
					const Uint32 subPadLength = 0,
					const E_FileWriteMode fileWriteMode = EWM_fileformat);
		Status clear(void);

		Status print(const String& filename);
		Status printXML(const String& filename);

		MetaInfo& metaInfo(void);
		Dataset& dataset(void);

		MetaInfo* getMetaInfo(void);
		Dataset* getDataset(void);

		const MetaInfo* getMetaInfo(void) const;
		const Dataset* getDataset(void) const;

		DcmFileFormat* getDcmFileFormat(void) const;
		operator DcmFileFormat*(void) const;
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
		DirRecord(void);
		DirRecord(DcmDirectoryRecord* dcmDirRecord);
		DirRecord(const DirRecord& dirRecord);
		virtual ~DirRecord(void);

		Status setFileId(const String& fileId);
		Status getFileId(String& fileId) const;
		String fileId(void) const;

		Status putRecord(const E_DirRecType recordType, const String& fileId, DirRecord& subRecord);
		Status getRecord(const E_DirRecType recordType, const File &file, DirRecord& subRecord);

		Status copyValue(const File &file);

		E_DirRecType getRecordType(void);

		static E_DirRecType getRecordTypeFromSOPClass(const String& sopClassUID);
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
		Dir(void);
		Dir(const String& dicomdir);
		virtual ~Dir(void);

		Status getRootRecord(DirRecord& rootRecord);
		OFCondition addFile(const String &filename);

		OFCondition save(const E_EncodingType encodingType = EET_UndefinedLength, const E_GrpLenEncoding groupLength = EGL_withoutGL);

		Status print(const String& filename);

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
		UIDHelper(const String& uidRoot);

		String generateUID(void);

		static String getSOPClassName(const String& sopClassUID);
		static String getSOPClassUID(const String& sopClassName);
		static bool isStorageSOPClassUID(const String& sopClassUID);
		static bool isImageSOPClassUID(const String& strSOPClassUID);
		static String generateUID(const String& uidRoot);
		static String getModality(const String& sopClassUID);
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
