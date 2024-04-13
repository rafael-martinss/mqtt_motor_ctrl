#include <stdio.h>
#include <string.h>
#include "pico/cyw43_arch.h"

#include "lwip/apps/mqtt.h"
#include "wifi_config.h"

#include "mqtt.h"

// MQTT topic name
#define MQTT_TOPIC "220023201/pico_w"

struct mqtt_connect_client_info_t mqtt_client_info =
    {
        MQTT_TOPIC, /*client id*/
        NULL,       /* user */
        NULL,       /* pass */
        0,          /* keep alive */
        NULL,       /* will_topic */
        NULL,       /* will_msg */
        0,          /* will_qos */
        0           /* will_retain */
#if LWIP_ALTCP && LWIP_ALTCP_TLS
        ,
        NULL
#endif
};

static mqtt_client_t *m_client;
static ip_addr_t m_ip_addr;

static void mqtt_incoming_data_cb(void *arg, const u8_t *data, u16_t len, u8_t flags)
{
  printf("data: %s\n", data);
  char *percentage = strstr(data, "duty_cycle:");
  if (percentage != NULL)
  {
    percentage += strlen("duty_cycle:");
    printf("The percentage request is: %s\n", percentage);
    motor_set_duty_cycle(atoi(percentage));
  }
  else if (strncmp(data, "get_rotation", strlen("get_rotation")) == 0)
  {
    mqtt_send_rotation(motor_get_rotation());
  }
}

static void mqtt_incoming_publish_cb(void *arg, const char *topic, u32_t tot_len)
{
  printf("topic %s\n", topic);
}

static void mqtt_request_cb(void *arg, err_t err)
{
  if (err)
  {
    printf("MQTT client request cb: err %d\n", (int)err);
  }
}

static void mqtt_connection_cb(mqtt_client_t *client, void *arg, mqtt_connection_status_t status)
{
  printf("MQTT client connection cb: status %d\n", (int)status);
  if (status == MQTT_CONNECT_ACCEPTED)
  {
    err_t err = mqtt_sub_unsub(client, MQTT_TOPIC, 0, &mqtt_request_cb, NULL, 1);
    if (err == ERR_OK)
    {
      printf("Subscribed success\n");
    }
    else
    {
      printf("Failed to subscribe: %d\n", err);
    }
  }
  else
  {
    printf("Connection rejected, status: %d\n", status);
  }
}

static int mqtt_initialize_hw()
{

  // initialize wi-fi
  if (cyw43_arch_init() != 0)
  {
    printf("Failure on start Wi-Fi module..\n");
    return 1;
  }

  // enable as client
  cyw43_arch_enable_sta_mode();
  printf("Connecting on %s\n", WIFI_SSID);

  // Try connect with a timeout of 30 seconds.
  if (cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASSWORD, CYW43_AUTH_WPA2_AES_PSK, 30000) != 0)
  {
    printf("Failed to connect\n");
    return 1;
  }
  printf("Connected!\n");
  return 0;
}

int mqtt_initialize()
{
  if (mqtt_initialize_hw())
  {
    return 1;
  }

  if (!ip4addr_aton(MQTT_SERVER, &m_ip_addr))
  {
    printf("IP error!\n");
    return 1;
  }

  printf("Connecting on MQTT Server\n");
  m_client = mqtt_client_new();
  mqtt_set_inpub_callback(m_client, &mqtt_incoming_publish_cb, &mqtt_incoming_data_cb, NULL);

  err_t err = mqtt_client_connect(m_client, &m_ip_addr, 1883, &mqtt_connection_cb, NULL, &mqtt_client_info);
  if (err != ERR_OK)
  {
    printf("MQTT connection error: %d\n", err);
  }
  printf("Connected on MQTT Server!\n");

  return 0;
}

void mqtt_send_rotation(eMotorRotate_t rotation)
{
  char buffer[32] = {0};
  snprintf(buffer, sizeof(buffer), "Motor current rotation: %s", (rotation == eRotateNormal ? "normal  " : "reverse"));
  mqtt_publish(m_client, MQTT_TOPIC, buffer, strlen(buffer), 0, false, &mqtt_request_cb, NULL);
}
