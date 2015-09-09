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
 * Service port
 */
void platformServicePort(Dart_NativeArguments arguments);

/**
 * Wrapped platform class handler
 */
void wrappedPlatformCfg(Dart_Port dest_port_id,
        Dart_CObject* message);

#endif	/* EXTPLATFORM_H */

