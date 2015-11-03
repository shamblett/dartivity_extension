/* 
 * File:   ExtPlatform.h
 * Package: dartivity_extension
 * Author: Steve Hamblett
 * Copyright Steve Hamblett 2015
 *
 * Created on 08 September 2015, 11:50
 */

#ifndef EXTPLATFORM_H
#define	EXTPLATFORM_H

#include "ExtAll.h"

/**
 * Function identifiers
 */
const int PLATFORM_CFG = 1;
const int PLATFORM_FIND_RESOURCE = 2;

/**
 * Number of command parameters
 */
const int PLATFORM_CFG_PARAMS = 10;
const int PLATFORM_FIND_RESOURCES_PARAMS = 5;

/**
 * Number of return parameters
 */
const int PLATFORM_FIND_RESOURCES_RET_PARAMS = 7;

/**
 * Find resource wait
 */
const int PLATFORM_FIND_WAIT = 6000;

/**
 * Find resource time slice
 */
const int PLATFORM_FIND_SLICE = 500; //us

/**
 * Seconds to wait for discovery
 * 
 *  (PLATFORM_FIND_WAIT * PLATFORM_FIND_SLICE) / 1000000
 */

/**
 * Max number of discoverable resources
 */
const int MAX_DISCOVERABLE_RESOURCES = 30;

/**
 * Service port
 */
void platformServicePort(Dart_NativeArguments arguments);

/**
 * Wrapped platform class handler
 */
void wrappedPlatformService(Dart_Port dest_port_id,
        Dart_CObject* message);

#endif	/* EXTPLATFORM_H */

