import sys
import time
import random
import os
from azure.iot.device import IoTHubModuleClient

LOG_FREQUENCY = 10
DEFAULT_HOSTNAME = 'edgehub'

class ModifyEnvironment:
    """
    Helper class to change a given environmental variable to a given value.

    If the value is not set at the object instantiation time, the default is used
    """
    def __init__(self, variable, modify = False, value = None):
        self.variable = variable
        self.modify = modify
        self.value = value

    def __enter__(self):
        self.prev_hostname = os.environ[self.variable]

        if self.modify:
            value = DEFAULT_HOSTNAME if self.value is None else self.value
            os.environ[self.variable] = value

    def __exit__(self, type, value, traceback):
        os.environ[self.variable] = self.prev_hostname

def create_and_connect_client():
    # The SSL verification step can only be verified if the hostname is provided as
    # a domain name, rather than an IP address.
    # In the case you provided a hostname when creating the local node, please
    # set the following boolean to False
    modify = True

    with ModifyEnvironment('IOTEDGE_GATEWAYHOSTNAME', modify):
        print("Creating IoT Hub azure client...")
        client = IoTHubModuleClient.create_from_edge_environment()
        print("Client created!")

    client.connect()
    print("Client connected!")

    return client

def main():
    client = create_and_connect_client()

    msg_counter = 0
    try:
        while True:
            number = random.uniform(0.0, 30.0)
            client.send_message_to_output(f"{number}", "sensoroutput")

            # Print log message at each LOG_FREQUENCY interval
            if msg_counter % LOG_FREQUENCY == 0:
                print(f"Sent data number {msg_counter} -> message = {number}")

            msg_counter = msg_counter + 1
            time.sleep(10)
    except Exception as e:
        print("Unexpected error %s " % e)
        raise
    finally:
        # Graceful exit
        print("Shutting down client")
        client.shutdown()

if __name__ == '__main__':
    main()
