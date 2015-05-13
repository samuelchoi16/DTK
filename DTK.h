#ifndef DTK_H
#define DTK_H

#include "DTK_global.h"

#include "dcmtk/oflog/config.h"
#include "dcmtk/oflog/appender.h"
#include "dcmtk/oflog/fstreams.h"
#include "dcmtk/oflog/helpers/property.h"
#include "dcmtk/oflog/helpers/timehelp.h"

#include "dcmtk/ofstd/ofstd.h"				/* for OFStandard */
#include "dcmtk/ofstd/ofconapp.h"			/* for OFConsoleApplication */
#include "dcmtk/ofstd/ofcmdln.h"			/* for OFCommandLine */
#include "dcmtk/ofstd/ofstream.h"

#include "dcmtk/dcmdata/dctk.h"				/* for various dcmdata headers */
#include "dcmtk/dcmdata/dcuid.h"			/* for dcmtk version name */
#include "dcmtk/dcmdata/dcrledrg.h"			/* for DcmRLEDecoderRegistration */
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

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define	DTK_VERSION_2_0_0_0				_T("2.0.0.0")
#define	DTK_VERSION						DTK_VERSION_2_0_0_0

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define	ITEM_POS_FIRST					(0)
#define	ITEM_POS_LAST					(-1)
#define	ITEM_POS_APPEND					(-2)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define	DTK_NLS_AUTO					(-1)
#define	DTK_NLS_OFF						0

#define	DTK_NLS_ISO_IR_6				0
#define	DTK_NLS_ISO_IR_100				1
#define	DTK_NLS_ISO_IR_101				2
#define	DTK_NLS_ISO_IR_109				3
#define	DTK_NLS_ISO_IR_110				4
#define	DTK_NLS_ISO_IR_148				5
#define	DTK_NLS_ISO_IR_144				6
#define	DTK_NLS_ISO_IR_127				7
#define	DTK_NLS_ISO_IR_126				8
#define	DTK_NLS_ISO_IR_138				9
#define	DTK_NLS_ISO_IR_166				10
#define	DTK_NLS_ISO_IR_149				11
#define	DTK_NLS_ISO_IR_13				12
#define	DTK_NLS_ISO_IR_14				13
#define	DTK_NLS_ISO_IR_87				14
#define	DTK_NLS_ISO_IR_159				15
#define	DTK_NLS_ISO_IR_192				16

#define	DTK_NLS_COUNT					17

#define	DTK_NLS_ISO_2022_IR_6_87		(DTK_NLS_ISO_IR_6 | DTK_NLS_ISO_IR_87 << 8)
										// Japanese: ISO 646 + JIS X 0208
#define	DTK_NLS_ISO_2022_IR_13_87		(DTK_NLS_ISO_IR_13 | DTK_NLS_ISO_IR_87 << 8)
										// Japanese: JIS X 0201 + JIS X 0208
#define	DTK_NLS_ISO_2022_IR_13_87_159	(DTK_NLS_ISO_IR_13 | DTK_NLS_ISO_IR_87 << 8 | DTK_NLS_ISO_IR_159 << 16)
										// Japanese: JIS X 0201 + JIS X 0208 + JIS X 0212)
#define	DTK_NLS_ISO_2022_IR_6_149		(DTK_NLS_ISO_IR_6 | DTK_NLS_ISO_IR_149 << 8)
										// Korean: ISO 646 + KS X 1001

#define	DTK_NLS_DEFAULT					DTK_NLS_ISO_IR_6
#define	DTK_NLS_LATIN1					DTK_NLS_ISO_IR_100
#define	DTK_NLS_LATIN2					DTK_NLS_ISO_IR_101
#define	DTK_NLS_LATIN3					DTK_NLS_ISO_IR_109
#define	DTK_NLS_LATIN4					DTK_NLS_ISO_IR_110
#define	DTK_NLS_LATIN5					DTK_NLS_ISO_IR_148
#define	DTK_NLS_CYRILLIC				DTK_NLS_ISO_IR_144
#define	DTK_NLS_ARABIC					DTK_NLS_ISO_IR_127
#define	DTK_NLS_GREEK					DTK_NLS_ISO_IR_126
#define	DTK_NLS_HEBREW					DTK_NLS_ISO_IR_138
#define	DTK_NLS_THAI					DTK_NLS_ISO_IR_166
#define	DTK_NLS_KOREAN					DTK_NLS_ISO_2022_IR_6_149
#define	DTK_NLS_JAPANESE0				DTK_NLS_ISO_IR_13
#define	DTK_NLS_JAPANESE1				DTK_NLS_ISO_2022_IR_6_87
#define	DTK_NLS_JAPANESE2				DTK_NLS_ISO_2022_IR_13_87
#define	DTK_NLS_JAPANESE3				DTK_NLS_ISO_2022_IR_13_87_159
#define	DTK_NLS_JAPANESE				DTK_NLS_JAPANESE2
#define	DTK_NLS_UNICODE_UTF8			DTK_NLS_ISO_IR_192

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define	EXS_JPEGBaseline				EXS_JPEGProcess1TransferSyntax
#define	EXS_JPEGExtended				EXS_JPEGProcess2_4TransferSyntax
#define	EXS_JPEGLossless				EXS_JPEGProcess14SV1TransferSyntax

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace dcm {
	typedef	signed int					Sint;
	typedef	unsigned int				Uint;

	typedef	unsigned long				Ulong;
	typedef signed long					Slong;

	typedef OFCondition					Status;
	typedef OFString					String;
	typedef DcmTag						Tag;

	class Logger;
//	class Status;
	class NLS;
//	class Tag;

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
	class AssociationInfo;
	class Association;
	class AssociationRequestor;
	class AssociationListener;
	class AssociationListenerMaster;
	class AssociationListenerWorker;

	class Item;
	class MetaInfo;
	class Dataset;
	class File;
	class Dir;

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//	Logger
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	class DTKSHARED_EXPORT Logger : public OFLogger
	{
	public:
		Logger(const dcmtk::log4cplus::Logger& base);
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//	Status
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
	class DTKSHARED_EXPORT Status : public OFCondition
	{
	public:
		Status(void);
		Status(const OFCondition& cond);
		Status(const OFConditionConst& cond);
		virtual ~Status(void);

		Uint16 code(void) const;
		String text(void) const;
		bool isOK(void) const;
	};
*/
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//	NLS
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	class DTKSHARED_EXPORT NLS
	{
	/*
	protected:
		std::string _s;

	public:
		String(void);
		String(const char ch);
		String(const char* psz);
		String(const std::string& ostr);
		String(const OFString& ostr);

		const String& operator=(char ch);
		const String& operator=(const char* psz);
		const String& operator=(const std::string& ostr);
		const String& operator=(const OFString& ostr);

		operator const char*() const;
		operator const OFString() const;

		const char* toConstData() const;

		static String format(const String format, ...);

		size_t size() const;
		bool isEmpty() const;
		int operator==(const String& s) const;
		int operator!=(const String& s) const;
		int compare(const String& s) const;
		String substr(size_t a, size_t b) const;

		////////////////////////////////////////////////////////////////////////////////////////////////////////////
		String encode(int nls);
		String decode(int nls = DTK_NLS_ISO_IR_6);
*/
	public:
		static String encode(const String& str, int nls);
		static String decode(const String& str, int nls = DTK_NLS_ISO_IR_6);
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
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//	Tag
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
	class DTKSHARED_EXPORT Tag : public DcmTag
	{
	public:
		Tag(void);
		Tag(Uint16 g, Uint16 e);
		Tag(const DcmTagKey& key);
		Tag(const Tag& dcmTag);
		~Tag(void);

		Tag& operator=(const Tag& dcmTag);

	//	void		SetVR(DcmEVR eVR);
		DcmEVR		GetVR(void) const;
		String	getVRName(void) const;

	//	void	Set(const CDcmTag& dcmTag);
	//	void	Set(Uint16 g, Uint16 e);
	//	void	Get(Uint16* pg, Uint16* pe) const;
	//	CString	GetTagName(void);

	//	void	setPrivateCreator(const CString& strPrivCreator);
	//	CString	getPrivateCreator(void) const;

	//	void	lookupVRinDictionary(void);
	//	bool	isSignable(void) const;
	//	bool	isUnknownVR(void) const;

	//	int operator==(const CDcmTag& dcmTag) const;
	//	int operator!=(const CDcmTag& dcmTag) const;
	//	int operator<(const CDcmTag& dcmTag) const;
	//	int operator>(const CDcmTag& dcmTag) const;
	//	int operator<=(const CDcmTag& dcmTag) const;
	//	int operator>=(const CDcmTag& dcmTag) const;

		String	toString() const;
	};
*/
	typedef	std::list<Tag>				TagList;

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
	//	QString _aetitle1;
		T_ASC_Network* _ascNetworkPtr;

	public:
		AppEntity(void);
		virtual ~AppEntity(void);

		Status init(const String& aetitle, const T_ASC_NetworkRole ascRole, const int listenerPort, const int timeout);
	//	Status init(const QString& aetitle, const T_ASC_NetworkRole ascRole, const int listenerPort, const int timeout);
		Status exit(void);

		String getAETitle(void) const;
	//	QString getAETitle1(void) const;
		void* getInternal(void);

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
	//	CDcmService
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	typedef	std::list<E_TransferSyntax>	TransferSyntaxList;
	typedef std::list<Service>			ServiceList;

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

		void addBasicTransferSyntaxList(void);

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
	};

	#define	DCM_BASIC_SCU_SERVICE_LIST		dcm::Service::DcmBasicSCUServiceList
	#define	DCM_BASIC_SCP_SERVICE_LIST		dcm::Service::DcmBasicSCPServiceList

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

		Uint32 maxPDUSizeOfThis;
		Uint32 maxPDUSizeOfPeer;

		AssociationInfo(void)
		{
			maxPDUSizeOfThis = 0;
			maxPDUSizeOfPeer = 0;
		}
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//	Association
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	class DTKSHARED_EXPORT Association
	{
	protected:
		static dcmtk::log4cplus::Logger _logger;

	protected:
		AppEntity* _appEntityPtr;
		T_ASC_Parameters* _ascParamsPtr;
		T_ASC_Association* _ascAssocPtr;
	//	CMutex m_mtxAssoc;
		int _maxPDUSize;

	public:
		Association(AppEntity* appEntityPtr);
		virtual ~Association(void);

		Status close(void);

		Status sendMessage(Message& req);
		Status receiveMessage(Message& rsp, int timeout);

		Status getAssocInfo(AssociationInfo& assocInfo);

		static Status verify(const String& localAETitle, const String& aetitle, const String& hostname, const Uint16 port, const Uint16 timeout = 15);
	//	static Status verify(const QString& localAETitle, const QString& aetitle, const QString& hostname, const Uint16 port, const Uint16 timeout = 15);

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
	public:
		AssociationRequestor(AppEntity* appEntityPtr);
		virtual ~AssociationRequestor(void);

		Status	connect(const String& calledAETitle, const String& hostname, const Uint16 port, const ServiceList& serviceList = DCM_BASIC_SCU_SERVICE_LIST);
	//	Status	connect(const QString& calledAETitle, const QString& hostname, const Uint16 port, const ServiceList& serviceList = DCM_BASIC_SCU_SERVICE_LIST);
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//	AssociationListener
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	class DTKSHARED_EXPORT AssociationListener :
		public Association
	{
	public:
		AssociationListener(AppEntity* appEntityPtr);
		virtual ~AssociationListener(void);

		Status	listen(const int nTimeout);
		Status	accept(const ServiceList& dcmServiceList = DCM_BASIC_SCP_SERVICE_LIST);
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
		bool _started;
		bool _stopped;
		Uint32 _counter;
	//	CMutex m_mtx;

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
		Item(void);
		Item(DcmItem* _dcmItemPtr);
		Item(const Item& dcmItem);
		virtual ~Item(void);

		Item& operator=(const Item& item);
		Status copyFrom(const Item* sourceItemPtr);

		Status set(DcmItem* _dcmItemPtr, int parentNLS = DTK_NLS_DEFAULT);
		Status clear(void);
		Status print(const String& filename) const;
		Status printXML(const String& filename) const;

		////////////////////////////////////////////////////////////////////////////////////////////////////////////

		Status putValue(const DcmTagKey& tag, const String& value, int nls = DTK_NLS_AUTO);
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

		////////////////////////////////////////////////////////////////////////////////////////////////////////////

		Status getValue(const DcmTagKey& tag, String& value, Sint32 pos = -1, int nls = DTK_NLS_AUTO) const;
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

		Status getValue(const DcmTagKey& tag, const Uint16*& valuePtr, Ulong* lengthPtr = NULL) const;	// AT, OW, US/SS, OB/OW
		Status getValue(const DcmTagKey& tag, const Uint32*& valuePtr, Ulong* lengthPtr = NULL) const;	// UL
		Status getValue(const DcmTagKey& tag, const Sint16*& valuePtr, Ulong* lengthPtr = NULL) const;	// AT, OW, US/SS, OB/OW
		Status getValue(const DcmTagKey& tag, const Sint32*& valuePtr, Ulong* lengthPtr = NULL) const;	// SL
		Status getValue(const DcmTagKey& tag, const Float32*& valuePtr, Ulong* lengthPtr = NULL) const;	// FL
		Status getValue(const DcmTagKey& tag, const Float64*& valuePtr, Ulong* lengthPtr = NULL) const;	// FD

		////////////////////////////////////////////////////////////////////////////////////////////////////////////

		Status putItem(const DcmTagKey& tag, Item& item, Sint32 pos = ITEM_POS_APPEND);
		Status getItem(const DcmTagKey& tag, Item& item, Sint32 pos = 0) const;
		Sint32 getItemCount(const DcmTagKey& tag) const;

		////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//	CDcmStatus	putPixelItem(const DcmTagKey& tag, const CDcmPixelSequence& dcmPixelSequence);
		Status getPixelItem(const DcmTagKey& tag, Uint32 pos, Uint8*& dataPtr, Uint32* lengthPtr) const;
		Sint32 getPixelItemCount(const DcmTagKey& tag) const;

		DcmPixelSequence* GetPixelSequence(const DcmTagKey& tag) const;

		////////////////////////////////////////////////////////////////////////////////////////////////////////////

		Status putDate(const DcmTagKey& tag, const DateTime& dt);								// DA, DT
		Status getDate(const DcmTagKey& tag, DateTime& dt) const;								// DA, DT

		Status putDateTime(const DcmTagKey& dtag, const DcmTagKey& ttag, const DateTime& dt);	// DA, TM
		Status getDateTime(const DcmTagKey& dtag, const DcmTagKey& ttag, DateTime& dt) const;	// DA, TM

		////////////////////////////////////////////////////////////////////////////////////////////////////////////

		Status putCode(const String& codeValue, const String& codingSchemeDesignator, const String& codingSchemeVersion, const String& codeMeaning);
		Status getCode(String& codeValue, String& codingSchemeDesignator, String& codingSchemeVersion, String& codeMeaning) const;

		Status putCode(const DcmTagKey& tag, Item& item,
						const String& codeValue, const String& codingSchemeDesignator, const String& codingSchemeVersion, const String& codeMeaning,
						Sint32 pos = ITEM_POS_APPEND);
		Status getCode(const DcmTagKey& tag, Item& item,
						String& codeValue, String& codingSchemeDesignator, String& codingSchemeVersion, String& codeMeaning,
						Sint32 pos = 0) const;

		////////////////////////////////////////////////////////////////////////////////////////////////////////////

		Status putRefSOP(const String& sopClassUID, const String& sopInstanceUID);
		Status getRefSOP(String& sopClassUID, String& sopInstanceUID) const;

		Status putRefSOP(const DcmTagKey& tag, Item& item, const String& sopClassUID, const String& sopInstanceUID, Sint32 pos = ITEM_POS_APPEND);
		Status getRefSOP(const DcmTagKey& tag, Item& item, String& sopClassUID, String& sopInstanceUID, Sint32 pos = 0) const;

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
		Status removeValue(const DcmTagKey& tag);

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
	//	Dataset
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	class DTKSHARED_EXPORT Dataset :
		public Item
	{
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
					const E_TransferSyntax writeXfer = EXS_LittleEndianExplicit,
					const E_EncodingType encodingType = EET_UndefinedLength,
					const E_GrpLenEncoding groupLength = EGL_recalcGL,
					const E_PaddingEncoding padEncoding = EPD_noChange,
					const Uint32 padLength = 0,
					const Uint32 subPadLength = 0);

		Status setTransferSyntax(E_TransferSyntax ts, DcmRepresentationParameter* dcmRepParamPtr = NULL);
		E_TransferSyntax getOriginalTransferSyntax(void) const;
		E_TransferSyntax getCurrentTransferSyntax(void) const;

		Status getPixelData(const DcmTagKey& tag, E_TransferSyntax ts, DcmRepresentationParameter* dcmRepParamPtr = NULL);

		static Status setDefaultNLS(int nNLS);
		static Status getDefaultNLS(int& nNLS);

		DcmDataset* _getDcmDataset(void) const;
		operator DcmDataset*(void) const;

		////////////////////////////////////////////////////////////////////////////////////////////////////////////

		static void	registerCodecs(void);
		static void	unregisterCodecs(void);

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
					const E_TransferSyntax writeXfer = EXS_LittleEndianExplicit,
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

		class MetaInfo* getFileMetaInfo(void);
		class Dataset* getDataset(void);

		DcmFileFormat* getDcmFileFormat(void) const;
		operator DcmFileFormat*(void) const;
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//	Dir
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	class DTKSHARED_EXPORT Dir
		: public DicomDirInterface
	{
	protected:

	public:
		Dir(void);
	//	CDcmDir(const CDcmDir& dcmDir);
		virtual ~Dir(void);

	//	CDcmDir& operator=(const CDcmDir& dcmDir);


	//	CDcmStatus AddDcmFile(const CString& strFile);
		Status save(void);
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
