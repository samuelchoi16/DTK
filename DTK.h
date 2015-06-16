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

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define	QSTR_TO_CSTR(s)					((s).toStdString().c_str())
#define	QSTR_TO_DSTR(s)					(dcm::String(QSTR_TO_CSTR((s))))
#define	DSTR_TO_CSTR(s)					((s).c_str())
#define	DSTR_TO_QSTR(s)					(QString((s).c_str()))

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define	DTK_VERSION_2_0_0_0				_T("2.0.0.0")
#define	DTK_VERSION						DTK_VERSION_2_0_0_0

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

	class DTKInit;

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
	typedef std::list<Service>			ServiceList;
	class ServiceList2;
	class AssociationInfo;
	class Association;
	class AssociationRequestor;
	class AssociationListener;
	class AssociationListenerMaster;
	class AssociationListenerWorker;

	class Item;
	class MetaInfo;
	class PixelDataProducer;
	class PixelDataConsumer;
	class Dataset;
	class File;
	class DirRecord;
	class Dir;

	class UIDHelper;

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//	DTKInit
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	class DTKInit
	{
	public:
		DTKInit(void);
		virtual ~DTKInit(void);

		static ServiceList2 basicSCUServiceList;
		static ServiceList2 basicSCPServiceList;
	};

#define	DCM_BASIC_SCU_SERVICE_LIST		dcm::DTKInit::basicSCUServiceList
#define	DCM_BASIC_SCP_SERVICE_LIST		dcm::DTKInit::basicSCPServiceList

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//	NLS
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////

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
		static const int AUTO		= -1;
		static const int OFF		= 0;

		static const int ISO_IR_6	= 0;
		static const int ISO_IR_100	= 1;
		static const int ISO_IR_101	= 2;
		static const int ISO_IR_109	= 3;
		static const int ISO_IR_110	= 4;
		static const int ISO_IR_144	= 5;
		static const int ISO_IR_127	= 6;
		static const int ISO_IR_126	= 7;
		static const int ISO_IR_138	= 8;
		static const int ISO_IR_148	= 9;
		static const int ISO_IR_166	= 10;
		static const int ISO_IR_149	= 11;
		static const int ISO_IR_13	= 12;
		static const int ISO_IR_14	= 13;
		static const int ISO_IR_87	= 14;
		static const int ISO_IR_159	= 15;
		static const int ISO_IR_58	= 16;
		static const int ISO_IR_192	= 17;
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

		static String toLocal8Bit(const QString& istr, int nls);
		static QString toUnicode(const String& istr, int nls);

		static String encode(const String& istr, int nls);
		static String decode(const String& istr, int nls = ISO_IR_6);

		static String encodeQ(const QString& istr, int nls);
		static QString decodeQ(const String& istr, int nls = ISO_IR_6);
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//	DateTime
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	class DTKSHARED_EXPORT DateTime : public tm
	{
	public:
		DateTime(void);
		DateTime(int year, int month, int day);
		DateTime(int year, int month, int day, int hour, int minute, int second);

		String format(const String& format) const;

		static DateTime currentDateTime();
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//	CDcmAppEntity
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	class DTKSHARED_EXPORT AppEntity
	{
	public:	// FIXME
		bool _checkApplicationContextName;
		bool _checkImplementationClassUID;
		int _maxPDUSize;

	protected:
		String _aetitle;
		T_ASC_Network* _ascNetworkPtr;

	public:
		AppEntity(void);
		virtual ~AppEntity(void);

		Status init(const String& aetitle, const T_ASC_NetworkRole ascRole, const int listenerPort, const int timeout);
		Status exit(void);

		String getAETitle(void) const;
		T_ASC_Network* getInternal(void);

	#ifdef	_DEBUG
		static void	internalTest(void);
	#endif

	private:
		AppEntity(const AppEntity&);					// do not allow pass-by-value
		const AppEntity& operator=(const AppEntity&);	// do not allow assignment
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//	DcmMessage
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	class DTKSHARED_EXPORT Message : public T_DIMSE_Message
	{
	protected:
		String _sopClassUID;
		String _sopInstanceUID;

	public:	// FIXME
		T_ASC_PresentationContextID _pcId;
		DcmDataset* _dcmDatasetPtr;

	public:
		Message(void);
		Message(Dataset* datasetPtr);

		Message& setDcmDataset(Dataset* datasetPtr);
		virtual void setIDs(T_ASC_Association* ascAssocPtr);

		String getCommandName(void) const;
		Uint16 getStatus(void) const;

		Message& print(const String& filename);

		virtual void onProgress(Uint32 byteCount);

	private:
		Message(const Message&);					// do not allow pass-by-value
		const Message& operator=(const Message&);	// do not allow assignment
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	class DTKSHARED_EXPORT CEchoRQ : public Message
	{
	public:
		CEchoRQ(Dataset* datasetPtr = NULL);
		virtual void setIDs(T_ASC_Association* ascAssocPtr);
	};

	class DTKSHARED_EXPORT CEchoRSP : public Message
	{
	public:
		CEchoRSP(const Message& req, Uint16 status = STATUS_Success, Dataset* datasetPtr = NULL);
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	class DTKSHARED_EXPORT CStoreRQ : public Message
	{
	public:
		CStoreRQ(Dataset* datasetPtr);
		virtual void setIDs(T_ASC_Association* ascAssocPtr);
	};

	class DTKSHARED_EXPORT CStoreRSP : public Message
	{
	public:
		CStoreRSP(const Message& req, Uint16 status = STATUS_Success, Dataset* datasetPtr = NULL);
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	class DTKSHARED_EXPORT CFindRQ : public Message
	{
	public:
		CFindRQ(const String& sopClassUID, Dataset* datasetPtr);
		virtual void setIDs(T_ASC_Association* ascAssocPtr);
	};

	class DTKSHARED_EXPORT CFindRSP : public Message
	{
	public:
		CFindRSP(const Message& req, Uint16 status = STATUS_Success, Dataset* datasetPtr = NULL);
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	class DTKSHARED_EXPORT CGetRQ : public Message
	{
	public:
		CGetRQ(const String& sopClassUID, Dataset* datasetPtr);
		virtual void setIDs(T_ASC_Association* ascAssocPtr);
	};

	class DTKSHARED_EXPORT CGetRSP : public Message
	{
	public:
		CGetRSP(const Message& req, Uint16 status = STATUS_Success, Dataset* datasetPtr = NULL,
				int remaining = -1, int completed = -1, int failure = -1, int warning = -1);
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	class DTKSHARED_EXPORT CMoveRQ : public Message
	{
	public:
		CMoveRQ(const String& sopClassUID, const String& destination, Dataset* datasetPtr);
		virtual void setIDs(T_ASC_Association* ascAssocPtr);
	};

	class DTKSHARED_EXPORT CMoveRSP : public Message
	{
	public:
		CMoveRSP(const Message& req, Uint16 status = STATUS_Success, Dataset* datasetPtr = NULL,
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
		NEventReportRQ(const String& sopClassUID, const String& sopInstanceUID, Uint16 eventTypeId, Dataset* datasetPtr);
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
		NGetRQ(const String& sopClassUID, const String& sopInstanceUID, const DcmTagKey* dcmTagPtr, int dcmTagCount, Dataset* datasetPtr = NULL);
		virtual ~NGetRQ(void);
		virtual void setIDs(T_ASC_Association* ascAssocPtr);
	};

	class DTKSHARED_EXPORT NGetRSP : public Message
	{
	public:
		NGetRSP(const Message& req, Uint16 nStatus = STATUS_Success, Dataset* _dcmDatasetPtr = NULL);
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	class DTKSHARED_EXPORT NSetRQ : public Message
	{
	public:
		NSetRQ(const String& sopClassUID, const String& sopInstanceUID, Dataset* datasetPtr);
		virtual void setIDs(T_ASC_Association* ascAssocPtr);
	};

	class DTKSHARED_EXPORT NSetRSP : public Message
	{
	public:
		NSetRSP(const Message& req, Uint16 status = STATUS_Success, Dataset* datasetPtr = NULL);
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	class DTKSHARED_EXPORT NActionRQ : public Message
	{
	public:
		NActionRQ(const String& sopClassUID, const String& sopInstanceUID, Uint16 actionTypeId, Dataset* datasetPtr = NULL);
		virtual void setIDs(T_ASC_Association* ascAssocPtr);
	};

	class DTKSHARED_EXPORT NActionRSP : public Message
	{
	public:
		NActionRSP(const Message& req, Uint16 status = STATUS_Success, Dataset* datasetPtr = NULL);
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	class DTKSHARED_EXPORT NCreateRQ : public Message
	{
	public:
		NCreateRQ(const String& sopClassUID, const String& sopInstanceUID, Dataset* datasetPtr);
		virtual void setIDs(T_ASC_Association* ascAssocPtr);
	};

	class DTKSHARED_EXPORT NCreateRSP : public Message
	{
	public:
		NCreateRSP(const Message& req, Uint16 status = STATUS_Success, Dataset* datasetPtr = NULL);
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	class DTKSHARED_EXPORT NDeleteRQ : public Message
	{
	public:
		NDeleteRQ(const String& sopClassUID, const String& sopInstanceUID, Dataset* datasetPtr = NULL);
		virtual void setIDs(T_ASC_Association* ascAssocPtr);
	};

	class DTKSHARED_EXPORT NDeleteRSP : public Message
	{
	public:
		NDeleteRSP(const Message& req, Uint16 status = STATUS_Success, Dataset* datasetPtr = NULL);
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
/*
		static void addVerificationService(ServiceList& serviceList);

		static void addStorageSCUServices(ServiceList& serviceList);
		static void addStorageSCPServices(ServiceList& serviceList);

		static void addStorageCommitmentSCUService(ServiceList& serviceList);
		static void addStorageCommitmentSCPService(ServiceList& serviceList);

		static void addQueryRetrieveService(ServiceList& serviceList);
		static void addMWLService(ServiceList& serviceList);
		static void addMPPSService(ServiceList& serviceList);

		static void addGrayscalePrintService(ServiceList& serviceList);
		static void addColorPrintService(ServiceList& serviceList);

		static ServiceList DcmBasicSCUServiceList;
		static ServiceList DcmBasicSCPServiceList;
*/
	};

//	#define	DCM_BASIC_SCU_SERVICE_LIST		dcm::Service::DcmBasicSCUServiceList
//	#define	DCM_BASIC_SCP_SERVICE_LIST		dcm::Service::DcmBasicSCPServiceList

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//	ServiceList
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	class DTKSHARED_EXPORT ServiceList2
	{
	protected:
		ServiceList _serviceList;

	public:
		ServiceList2(void);

		void addService(const String& abstractSyntax, TransferSyntax transferSyntax);
		void addService(const String& abstractSyntax, const TransferSyntaxList& transferSyntaxList);

		void addAllStorageSCUServices(void);
		void addAllStorageSCPServices(void);

		void addAllQueryRetrieveServices(void);

		void addMWLService(void);
		void addMPPSService(void);

		void addStorageCommitmentSCUService(void);
		void addStorageCommitmentSCPService(void);

		void addGrayscalePrintService(void);
		void addColorPrintService(void);

		ServiceList::size_type size() const
		{
			return _serviceList.size();
		}

		ServiceList::const_iterator cbegin() const
		{
			return _serviceList.cbegin();
		}

		ServiceList::const_iterator cend() const
		{
			return _serviceList.cend();
		}
	};

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
		AppEntity* _appEntityPtr;
		T_ASC_Parameters* _ascParamsPtr;
		T_ASC_Association* _ascAssocPtr;
		QMutex _assocMutex;
		int _maxPDUSize;

	public:
		Association(AppEntity* appEntityPtr);
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
		AssociationRequestor(AppEntity* appEntityPtr);
		virtual ~AssociationRequestor(void);

//		Status	connect(const String& calledAETitle, const String& hostname, const Uint16 port, const ServiceList& serviceList = DCM_BASIC_SCU_SERVICE_LIST);
		Status	connect(const String& calledAETitle, const String& hostname, const Uint16 port, const ServiceList2& serviceList = DCM_BASIC_SCU_SERVICE_LIST);
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
		AssociationListener(AppEntity* appEntityPtr);
		virtual ~AssociationListener(void);

		Status	listen(const int nTimeout);
//		Status	accept(const ServiceList& dcmServiceList = DCM_BASIC_SCP_SERVICE_LIST);
		Status	accept(const ServiceList2& serviceList = DCM_BASIC_SCP_SERVICE_LIST);
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
		AssociationListenerMaster(AppEntity* appEntityPtr);
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
		AssociationListenerMaster* _masterPtr;

	public:
		AssociationListenerWorker(AssociationListenerMaster* masterPtr);
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
		DcmItem* _dcmItemPtr;
		int _autoNLS;
		static int _DefaultNLS;

	public:
		static const int POS_FIRST = 0;
		static const int POS_LAST = -1;
		static const int POS_APPEND = -2;

		Item(void);
		Item(DcmItem* _dcmItemPtr);
		Item(const Item& dcmItem);
		virtual ~Item(void);

		Item& operator=(const Item& item);
		Status copyFrom(const Item* sourceItemPtr);

		Status set(DcmItem* _dcmItemPtr, int parentNLS = NLS::DEFAULT);
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

		Status putValue(const DcmTagKey& tag, Uint value, Uint32 pos = 0);
		Status putValue(const DcmTagKey& tag, Sint value, Uint32 pos = 0);
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

		Status getValue(const DcmTagKey& tag, Uint& value, Uint32 pos = 0) const;
		Status getValue(const DcmTagKey& tag, Sint& value, Uint32 pos = 0) const;
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

		Status copyValueFrom(const DcmTagKey& tag, const Item* sourceItemPtr, DcmTagKey sourceTag = DcmTagKey(0xffff, 0xffff));

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
	//	MetaInfo
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	class DTKSHARED_EXPORT MetaInfo :
		public Item
	{
	public:
		MetaInfo(void);
		MetaInfo(DcmMetaInfo* dcmMetaInfoPtr);
		MetaInfo(const MetaInfo& metaInfo);
		virtual ~MetaInfo(void);

		MetaInfo& operator=(const MetaInfo& metaInfo);
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
		virtual bool onGetPixelData(bool encapsulated, Uint32 frameNo, Uint32 dataSize, const Uint8* dataPtr, bool begin, bool end) = 0;
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
		Dataset(void);
		Dataset(DcmDataset* dcmDatasetPtr);
		Dataset(const Dataset& dataset);
		virtual ~Dataset(void);

		Dataset&operator=(const Dataset& dataset);
		Status copyFrom(const Dataset* sourceDatasetPtr);

		Status load(const String& strFilename,
					const E_TransferSyntax readXfer = EXS_Unknown,
					const E_GrpLenEncoding groupLength = EGL_noChange,
					const Uint32 maxReadLength = DCM_MaxReadLength);
		Status save(const String& strFilename,
					const E_TransferSyntax writeXfer = EXS_Unknown,
					const E_EncodingType encodingType = EET_UndefinedLength,
					const E_GrpLenEncoding groupLength = EGL_recalcGL,
					const E_PaddingEncoding padEncoding = EPD_noChange,
					const Uint32 padLength = 0,
					const Uint32 subPadLength = 0);

		////////////////////////////////////////////////////////////////////////////////////////////////////////////

		Status setTransferSyntax(E_TransferSyntax ts, DcmRepresentationParameter* dcmRepParamPtr = NULL);
		E_TransferSyntax getTransferSyntax(void) const;

		////////////////////////////////////////////////////////////////////////////////////////////////////////////

		Status getPixelData(const DcmTagKey& tag, PixelDataConsumer* consumerPtr);

		////////////////////////////////////////////////////////////////////////////////////////////////////////////

		Status deidentify(void);

		////////////////////////////////////////////////////////////////////////////////////////////////////////////

		static Status setDefaultNLS(int nNLS);
		static Status getDefaultNLS(int& nNLS);

		DcmDataset* _getDcmDataset(void) const;
		operator DcmDataset*(void) const;

		////////////////////////////////////////////////////////////////////////////////////////////////////////////

		Status prepareCompositeIODFromMWL(const std::list<Dataset*> mwlDatasetPtrList);		// Prepare Composite IOD from MWL
		Status prepareMPPSFromMWL(const std::list<Dataset*> mwlDatasetsPtrList);			// Prepare MPPS from MWL

		Status prepareMPPSFromCompositeIOD(const Dataset* datasetPtr);						// Prepare MPPS from Composite IOD
		Status prepareCompositeIODFromMPPS(const Dataset* datasetPtr);						// Prepare Composite IOD from MPPS

		Status prepareMPPS2FromCompositeIOD(const Dataset* datasetPtr);						// Prepare MPPS from Composite IOD

		Status prepareStorageCommitmentFromCompositeIOD(const Dataset* datasetPtr);			// Prepare Storage Commitment from Composite IOD
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

		MetaInfo* getFileMetaInfo(void);
		Dataset* getDataset(void);

		const MetaInfo* getFileMetaInfo(void) const;
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
		DirRecord(DcmDirectoryRecord* dcmDirRecordPtr);
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
		DcmDicomDir* _dcmDirPtr;
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

#endif // DTK_H
