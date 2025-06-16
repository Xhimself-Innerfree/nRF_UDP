#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <openthread/ip6.h>
#include <openthread/udp.h>
#include <zephyr/net/udp.h>

/* 1000 msec = 1 sec */
#define SLEEP_TIME_MS 1000

#define BUTTON_NODE DT_NODELABEL(button0)

static const struct gpio_dt_spec button0_spec = GPIO_DT_SPEC_GET(BUTTON_NODE, gpios);
static struct gpio_callback button0_cb;

static void udp_send(void)
{
	/* This function contains the logic to send a UDP packet.*/
	otError error = OT_ERROR_NONE;
	const char *buf = "Hello, OT! This is FNLab";

	otInstance *myInstance;
	myInstance = openthread_get_default_instance();
	otUdpSocket mySocket;

	otMessageInfo messageInfo;
	memset(&messageInfo, 0, sizeof(messageInfo));
	otIp6AddressFromString("fe80:0:0:0:3cc1:906d:c871:8532", &messageInfo.mPeerAddr); // ff03::1 is the all nodes multicast address
	messageInfo.mPeerPort = 6060;													  // Set the destination port

	do
	{
		error = otUdpOpen(myInstance, &mySocket, NULL, NULL);
		if (error != OT_ERROR_NONE)
		{
			// printf("Failed to open UDP socket: %d\n", error);
			break;
		}

		otMessage *test_Message = otUdpNewMessage(myInstance, NULL);
		error = otMessageAppend(test_Message, buf, (uint16_t)strlen(buf));
		if (error != OT_ERROR_NONE)
		{
			// printf("Failed to append message: %d\n", error);
			break;
		}

		error = otUdpSend(myInstance, &mySocket, test_Message, &messageInfo);
		if (error != OT_ERROR_NONE)
		{
			// printf("Failed to send UDP message: %d\n", error);
			break;
		}
		error = otUdpClose(myInstance, &mySocket);

	} while (false); // Replace with actual condition to check if the socket is open

	if (error != OT_ERROR_NONE)
	{
		printf("Error occurred: %d\n", error);
	}
	else
	{
		printf("UDP message sent successfully!\n");
	}
}

void button0_pressed(const struct device *gpiob, struct gpio_callback *cb, uint32_t pins)
{
	udp_send();
}

void main(void)
{
	gpio_pin_configure_dt(&button0_spec, GPIO_INPUT); // GPIO_INPUT | GPIO_INPUT
	gpio_pin_interrupt_configure_dt(&button0_spec, GPIO_INT_EDGE_TO_ACTIVE);
	gpio_init_callback(&button0_cb, button0_pressed, BIT(button0_spec.pin));
	gpio_add_callback(button0_spec.port, &button0_cb);

	while (1)
	{
		k_msleep(SLEEP_TIME_MS);
	}
}
