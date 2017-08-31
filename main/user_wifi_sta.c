#include "user_wifi_sta.h"

#include <stdio.h>
#include "esp_wifi.h"
#include "esp_log.h"
#include "esp_event_loop.h"


static esp_err_t event_handler(void *ctx, system_event_t *event)
{
	wifi_ap_record_t my_wifi_info;
	
    switch(event->event_id) 
	{
		case SYSTEM_EVENT_STA_START:                         // wifi STA 开始
			printf("[WIFI STA] STA start event!\r\n");
			ESP_ERROR_CHECK(esp_wifi_connect());                 // 连接wifi
			break;
		case SYSTEM_EVENT_STA_GOT_IP:                        // STA模式下，分配到了IP
			printf("[WIFI STA] Get IP event!\r\n");
			printf("[WIFI STA] ESP32 IP: %s !\r\n", ip4addr_ntoa(&event->event_info.got_ip.ip_info.ip));
			break;
		case SYSTEM_EVENT_STA_CONNECTED:                     // STA模式下，连接上了wifi	
			printf("[WIFI STA] Wifi STA connect event!\r\n");
			esp_wifi_sta_get_ap_info(&my_wifi_info);             // STA模式下，获取模块连接上的wifi热点的信息
			printf("[WIFI STA] Connect to : %s!\r\n", my_wifi_info.ssid);
			break;
		case SYSTEM_EVENT_STA_DISCONNECTED:                  // STA模式下，断开了与wifi的连接
			printf("[WIFI STA] Wifi STA disconnect event, reconnect!\r\n");
			ESP_ERROR_CHECK( esp_wifi_connect() );                // 重新连接wifi              
			break;
		default:
			break;
    }
	
    return ESP_OK;	
}


void user_wifi_config_sta(void)
{
	printf("[WIFI STA] STA config start, connect to Wifi : %s \r\n", WIFI_SSID);
	
    tcpip_adapter_init();
	
    ESP_ERROR_CHECK( esp_event_loop_init(event_handler, NULL) );
	
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK( esp_wifi_init(&cfg) );
	ESP_ERROR_CHECK( esp_wifi_set_storage(WIFI_STORAGE_RAM) );
	
    wifi_config_t wifi_config = 
	{
		.sta = 
		{
			.ssid = WIFI_SSID,
			.password = WIFI_PASS
		},
    };
    ESP_ERROR_CHECK( esp_wifi_set_mode(WIFI_MODE_STA) );
    ESP_ERROR_CHECK( esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config) );
    ESP_ERROR_CHECK( esp_wifi_start() );	
}