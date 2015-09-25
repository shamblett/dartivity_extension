/* 
 * File:   ExtResource.cpp
 * Package: dartivity_extension
 * Author: S. Hamblett <steve.hamblett@linux.com>
 * Copyright S. Hamblett 2015
 *
 * Created on 8 September 2015, 08:34
 */

#include "ExtResource.h"

void wrappedResourceService(Dart_Port dest_port_id,
        Dart_CObject * message) {
    // Get the reply port, we have to assume this is OK or
    // we can't reply with null to the caller.
    Dart_CObject* servicePortObject = message->value.as_array.values[EXT_SERVICE_PORT];
    Dart_Port reply_port_id = servicePortObject->value.as_send_port.id;

    // Switch on the incoming command
    Dart_CObject* commandObject = message->value.as_array.values[EXT_COMMAND];
    if (commandObject->type == Dart_CObject_kInt32) {
        int command = commandObject->value.as_int32;
        switch (command ) {
                
                // TODO add put/get etc
            
            default:
#ifdef DEBUG
                std::cout << "wrappedResourceService::Oops invalid command - value is " << command << " " << std::endl;
#endif
                break;
        }
    }
    // Failure - return a null result object
    Dart_CObject result;
    result.type = Dart_CObject_kNull;
    Dart_PostCObject(reply_port_id, &result);
}

void resourceServicePort(Dart_NativeArguments arguments) {
    Dart_EnterScope();
    Dart_SetReturnValue(arguments, Dart_Null());
    Dart_Port service_port =
            Dart_NewNativePort("ResourceService", wrappedResourceService, true);
    if (service_port != ILLEGAL_PORT) {
        Dart_Handle send_port = HandleError(Dart_NewSendPort(service_port));
        Dart_SetReturnValue(arguments, send_port);
    }
    Dart_ExitScope();
}
