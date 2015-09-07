/* 
 * File:   ExtAll.h
 * Package: amqp_extension
 * Author: Steve Hamblett
 * Copyright steve@OSCF 2014
 *
 * Created on 20 January 2014, 08:06
 */

#ifndef EXTALL_H
#define	EXTALL_H

/* General includes */
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <cstdlib>
#include <pthread.h>
#include <condition_variable>
#include <ostream>

/* Iotivity includes */
#include "OCPlatform.h"
#include "OCApi.h"

using namespace OC;


/* Dart includes */
#include "include/dart_native_api.h"

/**
 * General library wide native functions
 */
Dart_NativeFunction ResolveName(Dart_Handle name,
        int argc,
        bool* auto_setup_scope);

/**
 * General library wide Dart handle checker
 */

Dart_Handle HandleError(Dart_Handle handle);

/**
 * General library wide exception thrower
 */
void throwDartivityException(const char* message);


#endif	/* EXTALL_H */

