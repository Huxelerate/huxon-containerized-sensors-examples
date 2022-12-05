import sys
import time
import random
import os
from azure.iot.device import IoTHubModuleClient

LOG_FREQUENCY = 10

def create_and_connect_client():
    HOSTNAME = 'IOTEDGE_GATEWAYHOSTNAME'

    previous_gateway_hostname = os.environ[HOSTNAME]
    os.environ[HOSTNAME] = 'edgehub'

    print("Creating IoT Hub azure client...")

    client = IoTHubModuleClient.create_from_edge_environment()
    print("Client created!")

    client.connect()
    print("Client connected!")
    os.environ[HOSTNAME] = previous_gateway_hostname

    return client

def main():
    client = create_and_connect_client()

    msg_counter = 0
    try:
        while True:
            number = random.uniform(0.0, 30.0)
            client.send_message_to_output(f"{number}", "sensoroutput")

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
