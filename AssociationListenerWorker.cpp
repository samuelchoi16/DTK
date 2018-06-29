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

Logger AssociationListenerWorker::_logger = Logger::getInstance("dcm.AssociationListenerWorker");

AssociationListenerWorker::AssociationListenerWorker(AssociationListenerMaster* masterListener)
	: AssociationListener(masterListener->_appEntity)
	, _masterListener(masterListener)
{
	_appEntity = masterListener->_appEntity;
	_ascParams = masterListener->_ascParams;
	_ascAssoc = masterListener->_ascAssoc;
	_maxPDUSize = masterListener->_maxPDUSize;
}

AssociationListenerWorker::~AssociationListenerWorker(void)
{
}

Status AssociationListenerWorker::startlisten(void)
{
	DCMTK_LOG4CPLUS_DEBUG(_logger, "startListen: starting worker listener...");

	start();
	return EC_Normal;
}

bool AssociationListenerWorker::isListening(void)
{
	return _masterListener->isListening();
}

void AssociationListenerWorker::run(void)
{
	DCMTK_LOG4CPLUS_DEBUG(_logger, "run: worker listener started.");

	_masterListener->_masterMutex.lock();
	_masterListener->_counter++;
	_masterListener->_masterMutex.unlock();

	onlisten();

	_masterListener->_masterMutex.lock();
	_masterListener->_counter--;
	_masterListener->_masterMutex.unlock();

	close();

	delete this;

	DCMTK_LOG4CPLUS_DEBUG(_logger, "run: worker listener ended.");
}
