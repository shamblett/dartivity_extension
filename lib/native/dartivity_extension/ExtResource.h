/* 
 * File:   ExtResource.h
 * Package: dartivity_extension
 * Author: Steve Hamblett
 * Copyright Steve Hamblett 2015
 *
 * Created on 08 September 2015, 11:50
 */

#ifndef EXTRESOURCE_H
#define	EXTRESOURCE_H

#include "ExtAll.h"

/**
 * Function identifiers
 */
const int RESOURCE_PUT = 1;
const int RESOURCE_GET = 2;

/**
 * Number of command parameters
 */
const int RESOURCE_PUT_PARAMS = 3;
const int RESOURCE_GET_PARAMS = 3;

/**
 * Service port
 */
void resourceServicePort(Dart_NativeArguments arguments);

/**
 * Wrapped resource class handler
 */
void wrappedResourceService(Dart_Port dest_port_id,
        Dart_CObject* message);

#endif	/* EXTRESOURCE_H */

