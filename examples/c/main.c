// Copyright (c) Microsoft. All rights reserved.
// Copyright (c) Huxelerate.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include <stdio.h>
#include <stdlib.h>

#include "iothub_module_client_ll.h"
#include "iothub_client_options.h"
#include "iothub_message.h"
#include "azure_c_shared_utility/threadapi.h"
#include "azure_c_shared_utility/crt_abstractions.h"
#include "azure_c_shared_utility/platform.h"
#include "azure_c_shared_utility/shared_util_options.h"
#include "iothubtransportmqtt.h"
#include "iothub.h"
#include "time.h"

#define TEXT_LENGTH 16
#define LOG_FREQUENCY 10

typedef struct MESSAGE_INSTANCE_TAG
{
    IOTHUB_MESSAGE_HANDLE messageHandle;
    size_t messageTrackingId;  // For tracking the messages within the user callback.
} MESSAGE_INSTANCE;

static void SendConfirmationCallback(IOTHUB_CLIENT_CONFIRMATION_RESULT result, void* userContextCallback)
{
    // The context corresponds to which message# we were at when we sent.
    MESSAGE_INSTANCE* messageInstance = (MESSAGE_INSTANCE*)userContextCallback;
    IoTHubMessage_Destroy(messageInstance->messageHandle);
    free(messageInstance);
}

// Allocates a context for callback and clones the message
static MESSAGE_INSTANCE* CreateMessageInstance(IOTHUB_MESSAGE_HANDLE message)
{
    MESSAGE_INSTANCE* messageInstance = (MESSAGE_INSTANCE*)malloc(sizeof(MESSAGE_INSTANCE));
    if (NULL == messageInstance) {
        printf("Failed allocating 'MESSAGE_INSTANCE' for pipelined message\r\n");
    } else {
        memset(messageInstance, 0, sizeof(*messageInstance));

        if ((messageInstance->messageHandle = IoTHubMessage_Clone(message)) == NULL) {
            free(messageInstance);
            messageInstance = NULL;
        }
    }

    return messageInstance;
}

static IOTHUB_MODULE_CLIENT_LL_HANDLE InitializeConnection()
{
    IOTHUB_MODULE_CLIENT_LL_HANDLE iotHubModuleClientHandle;

    if (IoTHub_Init() != 0) {
        iotHubModuleClientHandle = NULL;
    } else if ((iotHubModuleClientHandle = IoTHubModuleClient_LL_CreateFromEnvironment(MQTT_Protocol)) == NULL) {
        printf("ERROR: IoTHubModuleClient_LL_CreateFromEnvironment failed\r\n");
    }

    return iotHubModuleClientHandle;
}

static void DeInitializeConnection(IOTHUB_MODULE_CLIENT_LL_HANDLE iotHubModuleClientHandle)
{
    if (iotHubModuleClientHandle != NULL)
    {
        IoTHubModuleClient_LL_Destroy(iotHubModuleClientHandle);
    }
    IoTHub_Deinit();
}

static int write_to_mqtt(char *data, int size, IOTHUB_MODULE_CLIENT_LL_HANDLE iotHubModuleClientHandle)
{
    static uint32_t msg_counter = 0;

    const char* topic = "sensoroutput";
    msg_counter++;

    IOTHUB_MESSAGE_HANDLE message = IoTHubMessage_CreateFromByteArray(data, size);
    MESSAGE_INSTANCE *messageInstance = CreateMessageInstance(message);
    if(messageInstance == NULL) {
        return 1;
    }

    IOTHUBMESSAGE_DISPOSITION_RESULT clientResult;
    clientResult = IoTHubModuleClient_LL_SendEventToOutputAsync(iotHubModuleClientHandle, messageInstance->messageHandle, topic, SendConfirmationCallback, (void *)messageInstance);
    if(clientResult != IOTHUB_CLIENT_OK) {
        IoTHubMessage_Destroy(messageInstance->messageHandle);
        free(messageInstance);
        return 1;
    }

    if (msg_counter == 1 || msg_counter % LOG_FREQUENCY == 0) {
        printf("Sent data number %d -> message = %s\n", msg_counter, data);
        fflush(stdout);
    }

    return 0;
}

// Simple function to fill in random data, just as an example.
void get_random_temperature_data(char * data) {
    float random_number = (float)rand() / (float)(RAND_MAX / 30.0);

    sprintf(data, "%.6f", random_number);
}

void iothub_module()
{
    IOTHUB_MODULE_CLIENT_LL_HANDLE iotHubModuleClientHandle;

    srand((unsigned int)time(NULL));

    if ((iotHubModuleClientHandle = InitializeConnection()) != NULL)
    {
        // The receiver just loops constantly waiting for messages.
        while (true)
        {
            IoTHubModuleClient_LL_DoWork(iotHubModuleClientHandle);

            char data[TEXT_LENGTH];

            get_random_temperature_data(data);

            // Write the data to MQTT
            if (write_to_mqtt(data, TEXT_LENGTH, iotHubModuleClientHandle) != 0) {
                printf("Error: write to MQTT failed! Exiting...");
                break;
            }

            // Sleep for 10 seconds
            ThreadAPI_Sleep(10000);
        }
    }

    DeInitializeConnection(iotHubModuleClientHandle);
}

int main(void)
{
    iothub_module();
    return 0;
}
