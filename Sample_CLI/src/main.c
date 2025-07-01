#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <openthread/ip6.h>
#include <openthread/udp.h>
#include <zephyr/net/udp.h>
#include <zephyr/device.h>
#include <zephyr/drivers/uart.h>
#include <zephyr/sys/printk.h>
#include <zephyr/devicetree.h>

/* 1000 msec = 1 sec */
#define SLEEP_TIME_MS 1000

/* STEP 10.1.1 - Define the size of the receive buffer */
#define RECEIVE_BUFF_SIZE 10
/* STEP 10.2 - Define the receiving timeout period */
#define RECEIVE_TIMEOUT 100
/* STEP 5.1 - Get the device pointers of the LEDs through gpio_dt_spec */
static const struct gpio_dt_spec led0 = GPIO_DT_SPEC_GET(DT_ALIAS(led0), gpios);
static const struct gpio_dt_spec led1 = GPIO_DT_SPEC_GET(DT_ALIAS(led1), gpios);
static const struct gpio_dt_spec led2 = GPIO_DT_SPEC_GET(DT_ALIAS(led2), gpios);

// a callback function
void udpRCV_Cb(void *context, otMessage *message, const otMessageInfo *messageInfo)
{
	uint16_t payloadLength = otMessageGetLength(message) - otMessageGetOffset(message);
	char buf[payloadLength + 1];
	otMessageRead(message, otMessageGetOffset(message), buf, payloadLength);
	buf[payloadLength] = '\0'; // Null-terminate the string
	printf("Received UDP message: %s\n", buf);
	if (buf[0] == '1')
	{
		gpio_pin_toggle_dt(&led0);
	}
	else if (buf[0] == '2')
	{
		gpio_pin_toggle_dt(&led1);
	}
	else if (buf[0] == '3')
	{
		gpio_pin_toggle_dt(&led2);
	}

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
	int ret;

	/* STEP 5.2 - Verify that the LED devices are ready */
	// Since all the LEDs are associated with the same port, &gpio0, we only need to check one of the devices.

	if (!device_is_ready(led0.port))
	{
		printk("GPIO device is not ready\r\n");
		return 1;
	}

	/* STEP 6 - Configure the GPIOs of the LEDs */
	ret = gpio_pin_configure_dt(&led0, GPIO_OUTPUT_ACTIVE);
	if (ret < 0)
	{
		return 1;
	}
	ret = gpio_pin_configure_dt(&led1, GPIO_OUTPUT_ACTIVE);
	if (ret < 0)
	{
		return 1;
	}
	ret = gpio_pin_configure_dt(&led2, GPIO_OUTPUT_ACTIVE);
	if (ret < 0)
	{
		return 1;
	}

	udp_init();
	while (1)
	{
		k_msleep(SLEEP_TIME_MS);
	}
}