/* 
 * File:   ExtAll.cpp
 * Package: dartivity_extension
 * Author: S. Hamblett <steve.hamblett@linux.com>
 * Copyright S. Hamblett 2015
 *
 * Created on 8 September 2015, 08:34
 */

#include "ExtPlatform.h"

static std::string dbFile;

static FILE* client_open(const char* /*path*/, const char *mode) {
    return fopen(dbFile.c_str(), mode);
}

void platformFindResource(Dart_Port dest_port_id,
        Dart_CObject* message) {

   

    // Get the service port
    

    // Resource callback

    struct X {

        void foundResource(std::shared_ptr<OCResource> resource) {


            // Build and return the result
            Dart_CObject result;
            result.type = Dart_CObject_kBool;
            result.value.as_bool = true;
            Dart_CObject* servicePortObject = message->value.as_array.values[EXT_SERVICE_PORT];
            Dart_Port reply_port_id = servicePortObject->value.as_send_port.id;
            Dart_PostCObject(reply_port_id, &result);
            return;
        }
    };

    // Arg check and parameter extraction
    if (message->type == Dart_CObject_kArray &&
            PLATFORM_FIND_RESOURCES_PARAMS == message->value.as_array.length) {

        Dart_CObject* param2 = message->value.as_array.values[2];
        Dart_CObject* param3 = message->value.as_array.values[3];
        Dart_CObject* param4 = message->value.as_array.values[4];

        // Parameter check
        if (param2->type == Dart_CObject_kString &&
                param3->type == Dart_CObject_kString &&
                param4->type == Dart_CObject_kInt32) {
            std::string host = std::string(param2->value.as_string);
            std::string resourceName = std::string(param3->value.as_string);
            int connectivity = param4->value.as_int32;

#ifdef DEBUG
            std::cout << "<<< platformFindResource - Param Block >>>" << std::endl;
            std::cout << "Host - " << host << std::endl;
            std::cout << "Resource Name - " << resourceName << std::endl;
            std::cout << "<<< Param Block >>>" << std::endl;
#endif         


            // Call find resource
            OCPlatform::findResource(host, resourceName,
                    CT_DEFAULT, &X::foundResource);


        }
    }

    // Failure - return a null result objects   
    Dart_CObject* servicePortObject = message->value.as_array.values[EXT_SERVICE_PORT];
    Dart_Port reply_port_id = servicePortObject->value.as_send_port.id;
    Dart_CObject result;
    result.type = Dart_CObject_kNull;
    Dart_PostCObject(reply_port_id, &result);

}

void platformCfg(Dart_Port dest_port_id,
        Dart_CObject* message) {

    // Get the service port
    Dart_CObject* servicePortObject = message->value.as_array.values[EXT_SERVICE_PORT];
    Dart_Port reply_port_id = servicePortObject->value.as_send_port.id;

    // Arg check and parameter extraction
    if (message->type == Dart_CObject_kArray &&
            PLATFORM_CFG_PARAMS == message->value.as_array.length) {

        Dart_CObject* param2 = message->value.as_array.values[2];
        Dart_CObject* param3 = message->value.as_array.values[3];
        Dart_CObject* param4 = message->value.as_array.values[4];
        Dart_CObject* param7 = message->value.as_array.values[7];
        Dart_CObject* param8 = message->value.as_array.values[8];
        Dart_CObject* param9 = message->value.as_array.values[9];

        // Parameter check
        if (param2->type == Dart_CObject_kInt32 &&
                param3->type == Dart_CObject_kInt32 &&
                param4->type == Dart_CObject_kInt32 &&
                param7->type == Dart_CObject_kString &&
                param8->type == Dart_CObject_kInt32 &&
                param9->type == Dart_CObject_kString) {
            int service = param2->value.as_int32;
            int mode = param3->value.as_int32;
            int qos = param4->value.as_int32;
            std::string ip = std::string(param7->value.as_string);
            int port = param8->value.as_int32;
            dbFile = std::string(param9->value.as_string);

            // Setup and call the Iotivity function
            OCPersistentStorage ps{client_open, fread, fwrite, fclose, unlink};
#ifdef DEBUG
            std::cout << "<<<platformCfg - Param Block >>>" << std::endl;
            std::cout << "Service Type - " << service << std::endl;
            std::cout << "Mode - " << mode << std::endl;
            std::cout << "IP - " << ip << std::endl;
            std::cout << "Port - " << port << std::endl;
            std::cout << "Qos - " << qos << std::endl;
            std::cout << "Db File - " << dbFile << std::endl;
            std::cout << "<<< Param Block >>>" << std::endl;
#endif            
            PlatformConfig cfg{
                (OC::ServiceType)service,
                (OC::ModeType)mode,
                ip,
                (uint16_t) port,
                (OC::QualityOfService)qos,
                &ps
            };
            OCPlatform::Configure(cfg);

            // Build and return the result
            Dart_CObject result;
            result.type = Dart_CObject_kBool;
            result.value.as_bool = true;
            Dart_PostCObject(reply_port_id, &result);
            return;
        }
    }

    // Failure - return a null result object
    Dart_CObject result;
    result.type = Dart_CObject_kNull;
    Dart_PostCObject(reply_port_id, &result);

}

void wrappedPlatformService(Dart_Port dest_port_id,
        Dart_CObject* message) {
    // Get the reply port, we have to assume this is OK or
    // we can't reply with null to the caller.
    Dart_CObject* servicePortObject = message->value.as_array.values[EXT_SERVICE_PORT];
    Dart_Port reply_port_id = servicePortObject->value.as_send_port.id;

    // Switch on the incoming command
    Dart_CObject* commandObject = message->value.as_array.values[EXT_COMMAND];
    if (commandObject->type == Dart_CObject_kInt32) {
        int command = commandObject->value.as_int32;
        switch (command) {

            case PLATFORM_CFG:
                platformCfg(dest_port_id, message);
                return;

            case PLATFORM_FIND_RESOURCE:
                platformFindResource(dest_port_id, message);
                return;

            default:
#ifdef DEBUG
                std::cout << "wrappedPlatformService::Oops invalid command - value is " << command << " " << std::endl;
#endif
                break;
        }
    }
    // Failure - return a null result object
    Dart_CObject result;
    result.type = Dart_CObject_kNull;
    Dart_PostCObject(reply_port_id, &result);
}

void platformServicePort(Dart_NativeArguments arguments) {
    Dart_EnterScope();
    Dart_SetReturnValue(arguments, Dart_Null());
    Dart_Port service_port =
            Dart_NewNativePort("PlatformService", wrappedPlatformService, true);
    if (service_port != ILLEGAL_PORT) {
        Dart_Handle send_port = HandleError(Dart_NewSendPort(service_port));
        Dart_SetReturnValue(arguments, send_port);
    }
    Dart_ExitScope();
}
