/* 
 * File:   ExtResource.cpp
 * Package: dartivity_extension
 * Author: S. Hamblett <steve.hamblett@linux.com>
 * Copyright S. Hamblett 2015
 *
 * Created on 8 September 2015, 08:34
 */

#include "ExtResource.h"

void resourceHost(Dart_Port dest_port_id,
        Dart_CObject* message) {


    if (message->type == Dart_CObject_kArray &&
            RESOURCE_HOST_PARAMS == message->value.as_array.length) {

        Dart_CObject* param2 = message->value.as_array.values[2];


        // Parameter check
        if (param2->type == Dart_CObject_kInt64) {
            uint64_t ptr = param2->value.as_int64;
                     
            // Cast into a resource pointer
            OCResource* resource = reinterpret_cast<OCResource*>(ptr);
            
            // Perform the operation
            std::string host = resource->host();
            
            // Return the result
            Dart_CObject* servicePortObject = message->value.as_array.values[EXT_SERVICE_PORT];
            Dart_Port reply_port_id = servicePortObject->value.as_send_port.id;
            Dart_CObject result;
            result.type = Dart_CObject_kString;
            result.value.as_string =  const_cast<char*>(host.c_str());
            Dart_PostCObject(reply_port_id, &result);

        }
    } else {

        // Failure - return a null result objects   
        Dart_CObject* servicePortObject = message->value.as_array.values[EXT_SERVICE_PORT];
        Dart_Port reply_port_id = servicePortObject->value.as_send_port.id;
        Dart_CObject result;
        result.type = Dart_CObject_kNull;
        Dart_PostCObject(reply_port_id, &result);
    }
}

void resourceUri(Dart_Port dest_port_id,
        Dart_CObject* message) {


    if (message->type == Dart_CObject_kArray &&
            RESOURCE_URI_PARAMS == message->value.as_array.length) {

        Dart_CObject* param2 = message->value.as_array.values[2];


        // Parameter check
        if (param2->type == Dart_CObject_kInt64) {
            uint64_t ptr = param2->value.as_int64;
                     
            // Cast into a resource pointer
            OCResource* resource = reinterpret_cast<OCResource*>(ptr);
            
            // Perform the operation
            std::string uri = resource->uri();
            
            // Return the result
            Dart_CObject* servicePortObject = message->value.as_array.values[EXT_SERVICE_PORT];
            Dart_Port reply_port_id = servicePortObject->value.as_send_port.id;
            Dart_CObject result;
            result.type = Dart_CObject_kString;
            result.value.as_string =  const_cast<char*>(uri.c_str());
            Dart_PostCObject(reply_port_id, &result);

        }
    } else {

        // Failure - return a null result objects   
        Dart_CObject* servicePortObject = message->value.as_array.values[EXT_SERVICE_PORT];
        Dart_Port reply_port_id = servicePortObject->value.as_send_port.id;
        Dart_CObject result;
        result.type = Dart_CObject_kNull;
        Dart_PostCObject(reply_port_id, &result);
    }
}

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
        switch (command) {

            case RESOURCE_HOST:
                resourceHost(dest_port_id, message);
                return;
                
            case RESOURCE_URI:
                resourceUri(dest_port_id, message);
                return;
                
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
