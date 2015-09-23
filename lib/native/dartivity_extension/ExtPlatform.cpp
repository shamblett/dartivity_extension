/* 
 * File:   ExtAll.cpp
 * Package: dartivity_extension
 * Author: S. Hamblett <steve.hamblett@linux.com>
 * Copyright S. Hamblett 2015
 *
 * Created on 8 September 2015, 08:34
 */

#include "ExtPlatform.h"

// Resource mutex
std::mutex resourceMutex;

// Find Resource callback class

class resourceFindCallback {
public:

    static void foundResource(std::shared_ptr<OCResource> resource);

    static Dart_CObject* m_message;
    static bool m_callbackInvoked;

};
Dart_CObject* resourceFindCallback::m_message;
bool resourceFindCallback::m_callbackInvoked;

void resourceFindCallback::foundResource(std::shared_ptr<OCResource> resource) {

    Dart_CObject result;

    // Indicate invocation
    m_callbackInvoked = true;

    try {
        // Check for a null resource
        if (resource == NULL) {
            // Return a boolean here to indicate no resources found
            Dart_CObject* servicePortObject = m_message->value.as_array.values[EXT_SERVICE_PORT];
            Dart_Port reply_port_id = servicePortObject->value.as_send_port.id;
            result.type = Dart_CObject_kBool;
            result.value.as_bool = false;
            Dart_PostCObject(reply_port_id, &result);
#ifdef DEBUG
            std::cout << "<<< foundResource - returned invalid result" << std::endl;
#endif
        } else {
#ifdef DEBUG
            //std::cout << "<<< foundResource - resource id is " << resource->uniqueIdentifier() << std::endl;
#endif
            // Build and return the result for a found resource
            Dart_CObject* servicePortObject = m_message->value.as_array.values[EXT_SERVICE_PORT];
            Dart_Port reply_port_id = servicePortObject->value.as_send_port.id;
            result.type = Dart_CObject_kInt64;
            result.value.as_int64 = (int64_t) resource.get();
            Dart_PostCObject(reply_port_id, &result);
#ifdef DEBUG
            std::cout << "<<< foundResource - returned valid result id is " << resource->uniqueIdentifier() << std::endl;
#endif
        }
    } catch (std::exception& e) {
        std::cout << "Exception in foundResource: " << e.what() << std::endl;
    }

}

void platformFindResource(Dart_Port dest_port_id,
        Dart_CObject* message) {


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
            std::cout << "Connectivity - " << connectivity << std::endl;
            std::cout << "<<< Param Block >>>" << std::endl;
#endif         
            // Call find resource, mutexed
            resourceMutex.lock();
            resourceFindCallback::m_message = message;
            resourceFindCallback::m_callbackInvoked = false;
#ifdef DEBUG
            std::cout << "<<< platformFindResource - in mutex calling findResource" << std::endl;
#endif
            OCStackResult res = OCPlatform::findResource(host, resourceName,
                    CT_DEFAULT, resourceFindCallback::foundResource);
            if (res != OC_STACK_OK) {
                std::cout << "<<< platformFindResource call failed, result is " << res << std::endl;
            }
#ifdef DEBUG
            std::cout << "<<< platformFindResource - in mutex returned from findResource call" << std::endl;
#endif
            // Wait until we either find the resource or give up, in which case we
            // return failed to find resource. Note, to allow the iotivity client to work better
            // we do lots of little sleeps rather than a few big ones.
            // TODO add params to tune this loop rather than rely on hard code.
            int waitCount = 0;
            while (!resourceFindCallback::m_callbackInvoked) {
                waitCount++;
                if (waitCount == 10000) {
                    // Return a boolean here to indicate no resources found
                    Dart_CObject* servicePortObject = message->value.as_array.values[EXT_SERVICE_PORT];
                    Dart_Port reply_port_id = servicePortObject->value.as_send_port.id;
                    Dart_CObject result;
                    result.type = Dart_CObject_kBool;
                    result.value.as_bool = false;
                    Dart_PostCObject(reply_port_id, &result);

#ifdef DEBUG
                    std::cout << "<<< platformFindResource - in mutex returned not found result" << std::endl;
#endif
                    break;
                } else {
                    usleep(500); //microseconds
                }
            }
        }
        // Either we have found the resource or we have timed out, either way release the mutex
        resourceMutex.unlock();

    } else {

        // Failure - return a null result objects   
        Dart_CObject* servicePortObject = message->value.as_array.values[EXT_SERVICE_PORT];
        Dart_Port reply_port_id = servicePortObject->value.as_send_port.id;
        Dart_CObject result;
        result.type = Dart_CObject_kNull;
        Dart_PostCObject(reply_port_id, &result);
    }

}

void platformCfg(Dart_Port dest_port_id,
        Dart_CObject * message) {

    // Get the service port
    Dart_CObject* servicePortObject = message->value.as_array.values[EXT_SERVICE_PORT];
    Dart_Port reply_port_id = servicePortObject->value.as_send_port.id;

    // Arg check and parameter extraction
    if (message->type == Dart_CObject_kArray &&
            PLATFORM_CFG_PARAMS == message->value.as_array.length) {

        Dart_CObject* param2 = message->value.as_array.values[2];
        Dart_CObject* param3 = message->value.as_array.values[3];
        Dart_CObject* param4 = message->value.as_array.values[4];
        Dart_CObject* param5 = message->value.as_array.values[5];
        Dart_CObject* param6 = message->value.as_array.values[6];
        Dart_CObject* param7 = message->value.as_array.values[7];

        // Parameter check
        if (param2->type == Dart_CObject_kInt32 &&
                param3->type == Dart_CObject_kInt32 &&
                param4->type == Dart_CObject_kInt32 &&
                param5->type == Dart_CObject_kInt32 &&
                param6->type == Dart_CObject_kString &&
                param7->type == Dart_CObject_kInt32) {
            int service = param2->value.as_int32;
            int mode = param3->value.as_int32;
            int qos = param4->value.as_int32;
            int clientConn = param5->value.as_int32;
            std::string ip = std::string(param6->value.as_string);
            int port = param7->value.as_int32;

            // Setup and call the Iotivity function

#ifdef DEBUG
            std::cout << "<<< platformCfg - Param Block >>>" << std::endl;
            std::cout << "Service Type - " << service << std::endl;
            std::cout << "Mode - " << mode << std::endl;
            std::cout << "IP - " << ip << std::endl;
            std::cout << "Port - " << port << std::endl;
            std::cout << "Qos - " << qos << std::endl;
            std::cout << "Client Connectivity - " << clientConn << std::endl;
            std::cout << "<<< Param Block >>>" << std::endl;
#endif            
            PlatformConfig cfg{
                (OC::ServiceType)service,
                (OC::ModeType)mode,
                ip,
                (uint16_t) port,
                (OC::QualityOfService)qos,
                NULL
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
