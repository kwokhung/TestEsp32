#pragma GCC diagnostic push
#pragma GCC diagnostic warning "-fpermissive"

#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include "controller.h"

#include "bt.h"
#include "bt_trace.h"
#include "bt_types.h"
#include "btm_api.h"
#include "bta_api.h"
#include "bta_gatt_api.h"
#include "esp_gap_ble_api.h"
#include "esp_gattc_api.h"
#include "esp_gatt_defs.h"
#include "esp_bt_main.h"

#pragma GCC diagnostic pop

//Declare callback functions
static void esp_gap_cb(esp_gap_ble_cb_event_t event, esp_ble_gap_cb_param_t *param);
static void esp_gattc_cb(esp_gattc_cb_event_t event, esp_gatt_if_t gattc_if, esp_ble_gattc_cb_param_t *param);
static void gattc_profile_event_handler(esp_gattc_cb_event_t event, esp_gatt_if_t gattc_if, esp_ble_gattc_cb_param_t *param);

static esp_gatt_srvc_id_t test_service = {
    .id = {
        .uuid = {
            .len = ESP_UUID_LEN_16,
            .uuid = {.uuid16 = 0x00FF}, //uuid of service that need to get characteristics
        },
        .inst_id = 0,
    },
    .is_primary = true,
};

static esp_gatt_id_t notify_descr_id = {
    .uuid = {
        .len = ESP_UUID_LEN_16,
        .uuid = {
            .uuid16 = GATT_UUID_CHAR_CLIENT_CONFIG,
        },
    },
    .inst_id = 0,
};

static bool connect = false;
/* name of BLE device that you want to connect to */
static const char device_name[] = "ESP_GATTS_ON_OFF";
/* from Arduino terminal type 'on' to turn on led, 'off' to off LED */
String command = "";
/*BLE scan params */
static esp_ble_scan_params_t ble_scan_params = {
    .scan_type = BLE_SCAN_TYPE_ACTIVE,
    .own_addr_type = BLE_ADDR_TYPE_PUBLIC,
    .scan_filter_policy = BLE_SCAN_FILTER_ALLOW_ALL,
    .scan_interval = 0x50,
    .scan_window = 0x30};
/* profile info */
#define PROFILE_NUM 1
#define PROFILE_A_APP_ID 0
/* characteristic ids 0 and 1 */
#define CHAR_NUM 2
#define CHARACTERISTIC_ON_ID 0
#define CHARACTERISTIC_OFF_ID 1

struct gattc_profile_inst
{
    esp_gattc_cb_t gattc_cb;
    uint16_t gattc_if;
    uint16_t app_id;
    uint16_t conn_id;
    esp_gatt_id_t char_id[CHAR_NUM];
    uint8_t char_num;
    esp_bd_addr_t remote_bda;
};

/* One gatt-based profile one app_id and one gattc_if, this array will store the gattc_if returned by ESP_GATTS_REG_EVT */
static struct gattc_profile_inst test_profile = {
    .gattc_cb = gattc_profile_event_handler,
    .gattc_if = ESP_GATT_IF_NONE, /* Not get the gatt_if, so initial is ESP_GATT_IF_NONE */
};

void esp_log_buffer_hex(uint8_t *buf, int len)
{
    Serial.println();
    for (int i = 0; i < len; i++)
    {
        Serial.print(buf[i], HEX);
        Serial.print("\t");
    }
    Serial.println();
}
/*this function write value to specific characteristic */
void set_led(uint8_t val)
{
    uint8_t test = 1;
    esp_ble_gattc_write_char(
        test_profile.gattc_if,
        test_profile.conn_id,
        &test_service,
        &test_profile.char_id[val],
        sizeof(test),
        &test,
        ESP_GATT_WRITE_TYPE_RSP,
        ESP_GATT_AUTH_REQ_NONE);
}
/* this callback will be invoked to process incomming events */
static void gattc_profile_event_handler(esp_gattc_cb_event_t event, esp_gatt_if_t gattc_if, esp_ble_gattc_cb_param_t *param)
{
    uint16_t conn_id = 0;
    esp_ble_gattc_cb_param_t *p_data = (esp_ble_gattc_cb_param_t *)param;

    switch (event)
    {
    case ESP_GATTC_REG_EVT:
        printf("ESP_GATTC_REG_EVT\n");
        esp_ble_gap_set_scan_params(&ble_scan_params);
        break;
    /* when connection is set up, the event comes */
    case ESP_GATTC_OPEN_EVT:
        conn_id = p_data->open.conn_id;
        memcpy(test_profile.remote_bda, p_data->open.remote_bda, sizeof(esp_bd_addr_t));
        printf("ESP_GATTC_OPEN_EVT conn_id %d, if %d, status %d, mtu %d\n", conn_id, gattc_if, p_data->open.status, p_data->open.mtu);
        esp_ble_gattc_search_service(gattc_if, conn_id, NULL);
        /* backup char ids for set_led() function */
        test_profile.gattc_if = gattc_if;
        test_profile.conn_id = conn_id;
        test_profile.char_num = 0;
        break;
    /* list service id */
    case ESP_GATTC_SEARCH_RES_EVT:
    {
        esp_gatt_srvc_id_t *srvc_id = &p_data->search_res.srvc_id;
        conn_id = p_data->search_res.conn_id;
        printf("ESP_GATTC_SEARCH_RES_EVT RES: conn_id = %x\n", conn_id);
        if (srvc_id->id.uuid.len == ESP_UUID_LEN_16)
        {
            printf("UUID16: %x\n", srvc_id->id.uuid.uuid.uuid16);
        }
        else if (srvc_id->id.uuid.len == ESP_UUID_LEN_32)
        {
            printf("UUID32: %x\n", srvc_id->id.uuid.uuid.uuid32);
        }
        else if (srvc_id->id.uuid.len == ESP_UUID_LEN_128)
        {
            printf("UUID128:");
            esp_log_buffer_hex(srvc_id->id.uuid.uuid.uuid128, ESP_UUID_LEN_128);
        }
        else
        {
            printf("UNKNOWN LEN %d\n", srvc_id->id.uuid.len);
        }
        break;
    }
    case ESP_GATTC_SEARCH_CMPL_EVT:
        conn_id = p_data->search_cmpl.conn_id;
        printf("ESP_GATTC_SEARCH_CMPL_EVT: conn_id = %x, status %d\n", conn_id, p_data->search_cmpl.status);
        /* get characteristics of service */
        esp_ble_gattc_get_characteristic(gattc_if, conn_id, &test_service, NULL);
        break;
    /* list all characteristics of a service */
    case ESP_GATTC_GET_CHAR_EVT:
        if (p_data->get_char.status != ESP_GATT_OK)
        {
            break;
        }
        printf("ESP_GATTC_GET_CHAR_EVT: conn_id = %x, status %d\n", p_data->get_char.conn_id, p_data->get_char.status);
        printf("ESP_GATTC_GET_CHAR_EVT: srvc_id = %04x, char_id = %04x\n", p_data->get_char.srvc_id.id.uuid.uuid.uuid16, p_data->get_char.char_id.uuid.uuid.uuid16);
        /* backup char ids for set_led() function */
        test_profile.char_id[test_profile.char_num] = p_data->get_char.char_id;
        test_profile.char_num++;
        /* continue getting characteristics of service */
        esp_ble_gattc_get_characteristic(gattc_if, conn_id, &test_service, &p_data->get_char.char_id);
        break;
    case ESP_GATTC_SRVC_CHG_EVT:
    {
        esp_bd_addr_t bda;
        memcpy(bda, p_data->srvc_chg.remote_bda, sizeof(esp_bd_addr_t));
        printf("ESP_GATTC_SRVC_CHG_EVT, bd_addr:%08x%04x\n", (bda[0] << 24) + (bda[1] << 16) + (bda[2] << 8) + bda[3], (bda[4] << 8) + bda[5]);
        break;
    }
    default:
        break;
    }
}
/* this callback is responsile for scanning BLE devices */
static void esp_gap_cb(esp_gap_ble_cb_event_t event, esp_ble_gap_cb_param_t *param)
{
    uint8_t *adv_name = NULL;
    uint8_t adv_name_len = 0;
    switch (event)
    {
    case ESP_GAP_BLE_SCAN_PARAM_SET_COMPLETE_EVT:
    {
        printf("ESP_GAP_BLE_SCAN_PARAM_SET_COMPLETE_EVT\n");
        //the unit of the duration is second
        uint32_t duration = 30;
        esp_ble_gap_start_scanning(duration);
        break;
    }
    case ESP_GAP_BLE_SCAN_START_COMPLETE_EVT:
        printf("ESP_GAP_BLE_SCAN_START_COMPLETE_EVT\n");
        //scan start complete event to indicate scan start successfully or failed
        if (param->scan_start_cmpl.status != ESP_BT_STATUS_SUCCESS)
        {
            printf("Scan start failed");
        }
        break;
    /* processing scan result */
    case ESP_GAP_BLE_SCAN_RESULT_EVT:
    {
        esp_ble_gap_cb_param_t *scan_result = (esp_ble_gap_cb_param_t *)param;
        switch (scan_result->scan_rst.search_evt)
        {
        case ESP_GAP_SEARCH_INQ_RES_EVT:
            printf("ESP_GAP_BLE_SCAN_RESULT_EVT, Searched Adv Data Len %d, Scan Response Len %d\n", scan_result->scan_rst.adv_data_len, scan_result->scan_rst.scan_rsp_len);
            adv_name = esp_ble_resolve_adv_data(scan_result->scan_rst.ble_adv, ESP_BLE_AD_TYPE_NAME_CMPL, &adv_name_len);
            if (adv_name != NULL)
            {
                if (strlen(device_name) == adv_name_len && strncmp((char *)adv_name, device_name, adv_name_len) == 0)
                {
                    printf("Searched device %s\n", device_name);
                    /* if connection is established then stop scanning */
                    if (connect == false)
                    {
                        connect = true;
                        printf("Connect to the remote device.");
                        esp_ble_gap_stop_scanning();
                        esp_ble_gattc_open(test_profile.gattc_if, scan_result->scan_rst.bda, true);
                    }
                }
            }
            break;
        case ESP_GAP_SEARCH_INQ_CMPL_EVT:
            printf("ESP_GAP_SEARCH_INQ_CMPL_EVT\n");
            break;
        default:
            break;
        }
        break;
    }
    case ESP_GAP_BLE_SCAN_STOP_COMPLETE_EVT:
        printf("ESP_GAP_BLE_SCAN_STOP_COMPLETE_EVT\n");
        if (param->scan_stop_cmpl.status != ESP_BT_STATUS_SUCCESS)
        {
            printf("Scan stop failed");
        }
        else
        {
            printf("Stop scan successfully");
        }
        break;

    case ESP_GAP_BLE_ADV_STOP_COMPLETE_EVT:
        printf("ESP_GAP_BLE_ADV_STOP_COMPLETE_EVT\n");
        if (param->adv_stop_cmpl.status != ESP_BT_STATUS_SUCCESS)
        {
            printf("Adv stop failed");
        }
        else
        {
            printf("Stop adv successfully");
        }
        break;

    default:
        break;
    }
}

static void esp_gattc_cb(esp_gattc_cb_event_t event, esp_gatt_if_t gattc_if, esp_ble_gattc_cb_param_t *param)
{
    printf("EVT %d, gattc if %d", event, gattc_if);

    /* If event is register event, store the gattc_if for each profile */
    if (event == ESP_GATTC_REG_EVT)
    {
        if (param->reg.status == ESP_GATT_OK)
        {
            test_profile.gattc_if = gattc_if;
        }
        else
        {
            printf("Reg app failed, app_id %04x, status %d",
                   param->reg.app_id,
                   param->reg.status);
            return;
        }
    }
    do
    {
        if (gattc_if == ESP_GATT_IF_NONE || /* ESP_GATT_IF_NONE, not specify a certain gatt_if, need to call every profile cb function */
            gattc_if == test_profile.gattc_if)
        {
            if (test_profile.gattc_cb)
            {
                test_profile.gattc_cb(event, gattc_if, param);
            }
        }
    } while (0);
}

void ble_client_appRegister(void)
{
    esp_err_t status;

    printf("register callback");

    //register the scan callback function to the gap module
    if ((status = esp_ble_gap_register_callback(esp_gap_cb)) != ESP_OK)
    {
        printf("gap register error, error code = %x", status);
        return;
    }

    //register the callback function to the gattc module
    if ((status = esp_ble_gattc_register_callback(esp_gattc_cb)) != ESP_OK)
    {
        printf("gattc register error, error code = %x", status);
        return;
    }
    esp_ble_gattc_app_register(PROFILE_A_APP_ID);
}

void gattc_client_test(void)
{
    esp_bluedroid_init();
    esp_bluedroid_enable();
    ble_client_appRegister();
}

void setup()
{
    Serial.begin(115200);
    btStart();
    gattc_client_test();
}
/* processing Terminal command, type 'on' to turn on led, 'off' to off LED */
void loop()
{
    if (Serial.available() > 0)
    {
        while (Serial.available())
        {
            command += (char)Serial.read();
            if (command == "on")
            {
                Serial.println("ON");
                set_led(0);
            }
            else if (command == "off")
            {
                Serial.println("OFF");
                set_led(1);
            }
        }
        command = "";
    }
}