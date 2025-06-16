#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <openthread/ip6.h>
#include <openthread/udp.h>
#include <zephyr/net/udp.h>

/* 1000 msec = 1 sec */
#define SLEEP_TIME_MS 1000

// a callback function
void udpRCV_Cb(void *context, otMessage *message, const otMessageInfo *messageInfo)
{
	uint16_t payloadLength = otMessageGetLength(message) - otMessageGetOffset(message);
	char buf[payloadLength + 1];
	otMessageRead(message, otMessageGetOffset(message), buf, payloadLength);
	buf[payloadLength] = '\0'; // Null-terminate the string
	printf("Received UDP message: %s\n", buf);

	// Print the sender's address and port
	char addrStr[OT_IP6_ADDRESS_STRING_SIZE];
	otIp6AddressToString(&messageInfo->mPeerAddr, addrStr, sizeof(addrStr));
	printf("From: %s:%d\n", addrStr, messageInfo->mPeerPort);
}

static void udp_init(void)
{
	otError error = OT_ERROR_NONE;
	otInstance *myInstance = openthread_get_default_instance();

	otUdpSocket mySocket;
	otSockAddr sockAddr;
	memset(&sockAddr, 0, sizeof(sockAddr));
	sockAddr.mPort = 6060; // Set the local port to listen on, the same as the destination port

	do
	{
		error = otUdpOpen(myInstance, &mySocket, udpRCV_Cb, NULL);
		if (error != OT_ERROR_NONE)
		{
			// printf("Failed to open UDP socket: %d\n", error);
			break;
		}
		error = otUdpBind(myInstance, &mySocket, &sockAddr, OT_NETIF_THREAD); // Backbone or unspecified is also possible
	} while (false); // by JL

	if (error != OT_ERROR_NONE)
	{
		printf("Error occurred while initializing UDP: %d\n", error);
	}
}

int main(void)
{
	udp_init();
	while (1)
	{
		k_msleep(SLEEP_TIME_MS); // Sleep for 1000 milliseconds (1 second)
	}
}
