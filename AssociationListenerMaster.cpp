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

#include "DTK.h"
#include "DTKinternal.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

Logger AssociationListenerMaster::_logger = Logger::getInstance("dcm.AssociationListenerMaster");

AssociationListenerMaster::AssociationListenerMaster(AppEntity* appEntity)
	: AssociationListener(appEntity)
{
	_started = false;
	_stopped = true;
}

AssociationListenerMaster::~AssociationListenerMaster(void)
{
}

Status AssociationListenerMaster::startlisten(void)
{
	DCMTK_LOG4CPLUS_DEBUG(_logger, "startListen: starting master listener...");

	start();

	_started = true;
	_stopped = false;
	_counter = 0;

	return EC_Normal;
}

Status AssociationListenerMaster::stoplisten(void)
{
	_started = false;

	while(!_stopped)
	{
		DCMTK_LOG4CPLUS_DEBUG(_logger, "stopListen: waiting for master listener to die...");
		DTK_SLEEP(1);
	}

	join();

	DCMTK_LOG4CPLUS_DEBUG(_logger, "stopListen: master listener stopped.");

	return EC_Normal;
}

bool AssociationListenerMaster::isListening(void)
{
	return _started && !_stopped;
}

void AssociationListenerMaster::run(void)
{
	DCMTK_LOG4CPLUS_DEBUG(_logger, "run: master listener started.");

	while(_started) {
		Status stat = listen(1);
		if (stat == DUL_NOASSOCIATIONREQUEST) {
			DCMTK_LOG4CPLUS_TRACE(_logger, "run: master listener listen() timeout");
			continue;
		}
		if (stat != EC_Normal) {
			DCMTK_LOG4CPLUS_DEBUG_FMT(_logger, "run: master listener listen() returned 0x%04X. (%s)", stat.code(), stat.text());
			close();
			break;
		}

		AssociationListenerWorker* workerPtr = clone();
		workerPtr->startlisten();
	}

	while(_counter > 0) {
		DCMTK_LOG4CPLUS_DEBUG_FMT(_logger, "run: waiting for slave listeners(%d) to die...", _counter);
		DTK_SLEEP(1);
	}

	_stopped = true;

	DCMTK_LOG4CPLUS_DEBUG(_logger, "run: master listener ended.");
}
