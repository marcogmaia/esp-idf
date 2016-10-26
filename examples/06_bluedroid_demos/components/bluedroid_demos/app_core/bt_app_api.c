/**
 ****************************************************************************************
 *
 * @file bt_app_api.c
 *
 * @brief Application entry point
 *
 * Copyright (C) Espressif 2016
 * Created by Yulong at 2016/10/17
 *
 *
 ****************************************************************************************
 */

#include "bt_app_api.h"
#include "btm_ble_api.h"
//#include "btm_ble_int.h"


void API_Ble_AppConfigAdvData(tESP_BLE_ADV_DATA *adv_data,
											tAPI_SET_ADV_DATA_CMPL_CBACK *p_adv_data_cback)
{
	if(adv_data != NULL)
	{
		ESP_AppBleConfigadvData(adv_data, p_adv_data_cback);
	}else{
		LOG_ERROR("The adv_data is NULL\n");
	}

	if(++adv_data != NULL)
	{
		ESP_AppBleSetScanRsp(adv_data,NULL);
	}
			
}


void API_Ble_AppStartAdvertising(tESP_API_BLE_ADV_PARAMS_ALL *ble_adv_params)
{
	if (!API_BLE_ISVALID_PARAM(ble_adv_params->adv_int_min, BTM_BLE_ADV_INT_MIN, BTM_BLE_ADV_INT_MAX) ||
        !API_BLE_ISVALID_PARAM(ble_adv_params->adv_int_max, BTM_BLE_ADV_INT_MIN, BTM_BLE_ADV_INT_MAX))
    {
    	LOG_ERROR("Invalid advertisting interval parameters.\n");
        return ;
    }

	if ((ble_adv_params->adv_type < API_NON_DISCOVERABLE) && 
		(ble_adv_params->adv_type > API_BROADCASTER_MODE) )
	{
		LOG_ERROR("Invalid advertisting type parameters.\n");
		return;
	}

	if ((ble_adv_params->adv_filter_policy < API_ADV_NON_CONN) && 
		(ble_adv_params->adv_filter_policy > API_ADV_DIRECT) )
	{
		LOG_ERROR("Invalid advertisting type parameters.\n");
		return;
	}

	///
	BTA_DmSetBleAdvParamsAll(ble_adv_params->adv_int_min,
							 ble_adv_params->adv_int_max,
							 ble_adv_params->adv_type,
							 ble_adv_params->addr_type_own,
							 ble_adv_params->channel_map,
							 ble_adv_params->adv_filter_policy,
							 ble_adv_params->p_dir_bda);

	
}


void API_Ble_SetScanParams (tESP_BLE_SCAN_PARAMS *scan_params, tGATT_IF client_if,
                            		            tBLE_SCAN_PARAM_SETUP_CBACK scan_param_setup_cback)
{
	if (API_BLE_ISVALID_PARAM(scan_params->scan_intv, BTM_BLE_SCAN_INT_MIN, BTM_BLE_SCAN_INT_MAX) &&
        API_BLE_ISVALID_PARAM(scan_params->scan_win, BTM_BLE_SCAN_WIN_MIN, BTM_BLE_SCAN_WIN_MAX) &&
       (scan_params->scan_type == BTM_BLE_SCAN_MODE_ACTI || scan_params->scan_type == BTM_BLE_SCAN_MODE_PASS))
	{
		BTA_DmSetBleScanFilterParams(client_if, 
									scan_params->scan_intv,
									scan_params->scan_win,
									scan_params->scan_type,
									scan_params->scan_fil_policy,
									scan_params->addr_type_own,
									scan_param_setup_cback);
	}
}


void API_Ble_StartScanning (UINT8 duration, tBTA_DM_SEARCH_CBACK *p_results_cb)
{
	if((duration != 0) && (p_results_cb != NULL))
	{
		///Start scan the device
		BTA_DmBleObserve(true, duration, p_results_cb);	
	}else{
		LOG_ERROR("The scan duration or p_results_cb invalid\n");
	}
}

void API_Ble_AppStopAdvertising(void)
{
	bool stop_adv = true;
	
	BTA_DmBleBroadcast(stop_adv);
}

void API_Ble_AppUpdateConnectionParams(BD_ADDR bd_addr, UINT16 min_int, 
												UINT16 max_int, UINT16 latency, UINT16 timeout)
{
	if (min_int > max_int){
		min_int = max_int;
	}

	if (min_int < BTM_BLE_CONN_INT_MIN || max_int > BTM_BLE_CONN_INT_MAX){
		LOG_ERROR("Invalid interval value.\n");
	}

	 BTA_DmBleUpdateConnectionParams(bd_addr, min_int, max_int,
                                     latency, timeout);
	
}

void API_Ble_SetPacketDataLength(BD_ADDR remote_device, UINT16 tx_data_length)
{
	if (tx_data_length > BTM_BLE_DATA_SIZE_MAX){
       tx_data_length =  BTM_BLE_DATA_SIZE_MAX;
	}else if (tx_data_length < BTM_BLE_DATA_SIZE_MIN){
       tx_data_length =  BTM_BLE_DATA_SIZE_MIN;
	}

	BTA_DmBleSetDataLength(remote_device, tx_data_length);
}


void API_Ble_SetRandAddress(BD_ADDR rand_addr)
{
	if (rand_addr != NULL){
		BTA_DmSetRandAddress(rand_addr);
	}else{
		LOG_ERROR("Invalid randrom address.\n");
	}
}

void API_Ble_ConfigLocalPrivacy(BOOLEAN privacy_enable)
{
	 BTA_DmBleConfigLocalPrivacy(privacy_enable);
}




void API_Ble_GattcAppRegister(tBT_UUID *p_app_uuid, tBTA_GATTC_CBACK *p_client_cb)
{
	if (p_app_uuid != NULL)
	{
		BTA_GATTC_AppRegister(p_app_uuid, *p_client_cb);
	}else{
		LOG_ERROR("The app uuid invalid.\n");
	}
}

void API_Ble_GattcAppDeregister(tBTA_GATTC_IF client_if)
{
	
}

void API_Ble_GattcOpen(tBTA_GATTC_IF client_if, BD_ADDR remote_bda,
                    BOOLEAN is_direct, tBTA_GATT_TRANSPORT transport)
{
	
}

void API_GattcCancelOpen(tBTA_GATTC_IF client_if, BD_ADDR remote_bda, BOOLEAN is_direct)
{
	
}

void API_Ble_GattcClose(UINT16 conn_id)
{
	
}


void API_Ble_GattcConfigureMTU (UINT16 conn_id, UINT16 mtu)
{
	
}

tBTA_GATT_STATUS  API_Ble_GattcGetFirstChar (UINT16 conn_id, tBTA_GATT_SRVC_ID *p_srvc_id,
                                          	tBT_UUID *p_char_uuid_cond,
                                          	tBTA_GATTC_CHAR_ID *p_char_result,
                                          	tBTA_GATT_CHAR_PROP *p_property)
{
	
}

tBTA_GATT_STATUS  API_Ble_GattcGetNextChar (UINT16 conn_id,
                                         tBTA_GATTC_CHAR_ID *p_start_char_id,
                                         tBT_UUID           *p_char_uuid_cond,
                                         tBTA_GATTC_CHAR_ID *p_char_result,
                                         tBTA_GATT_CHAR_PROP    *p_property)
{
	
}

tBTA_GATT_STATUS  API_Ble_GattcGetNextCharDescr (UINT16 conn_id,
                                             tBTA_GATTC_CHAR_DESCR_ID *p_start_descr_id,
                                             tBT_UUID           *p_descr_uuid_cond,
                                             tBTA_GATTC_CHAR_DESCR_ID *p_descr_result)
{
	
}

tBTA_GATT_STATUS  BTA_GATTC_GetFirstIncludedService(UINT16 conn_id, tBTA_GATT_SRVC_ID *p_srvc_id,
                                                    tBT_UUID *p_uuid_cond, tBTA_GATTC_INCL_SVC_ID *p_result)
{
	
}

void API_Ble_GattcReadCharacteristic(UINT16 conn_id, tBTA_GATTC_CHAR_ID *p_char_id,
                                  tBTA_GATT_AUTH_REQ auth_req)
{
	
}


void API_Ble_GattcReadCharDescr (UINT16 conn_id,
                             					tBTA_GATTC_CHAR_DESCR_ID  *p_descr_id,
                              					tBTA_GATT_AUTH_REQ auth_req)
{
	
}

void API_Ble_GattcReadMultiple(UINT16 conn_id, tBTA_GATTC_MULTI *p_read_multi,
                            				tBTA_GATT_AUTH_REQ auth_req)
{
	
}

void API_Ble_GattcWriteCharValue ( UINT16 conn_id,
                               							 tBTA_GATTC_CHAR_ID *p_char_id,
                                						 tBTA_GATTC_WRITE_TYPE  write_type,
                                						 UINT16 len,
                                						 UINT8 *p_value,
                                						 tBTA_GATT_AUTH_REQ auth_req)
{
	
}

void API_Ble_GattcWriteCharDescr (UINT16 conn_id,
                              					 tBTA_GATTC_CHAR_DESCR_ID *p_char_descr_id,
                              					 tBTA_GATTC_WRITE_TYPE  write_type,
                              					 tBTA_GATT_UNFMT      *p_data,
                              					 tBTA_GATT_AUTH_REQ auth_req)
{
	
}


void API_Ble_GattcPrepareWrite  (UINT16 conn_id, tBTA_GATTC_CHAR_ID *p_char_id,
                              						UINT16 offset, UINT16 len, UINT8 *p_value,
                              						tBTA_GATT_AUTH_REQ auth_req)
{
	
}

void  API_Ble_GattcExecuteWrite (UINT16 conn_id, BOOLEAN is_execute)
{
	
}

void API_Ble_GattcSendIndConfirm (UINT16 conn_id, tBTA_GATTC_CHAR_ID *p_char_id)
{
	
}

tBTA_GATT_STATUS API_Ble_GattcRegisterForNotifications (tBTA_GATTC_IF client_if,
                                                     BD_ADDR bda,
                                                     tBTA_GATTC_CHAR_ID *p_char_id)
{
	
}

tBTA_GATT_STATUS API_Ble_GattcDeregisterForNotifications (tBTA_GATTC_IF client_if,
                                                       							BD_ADDR bda,
                                                       							tBTA_GATTC_CHAR_ID *p_char_id)
{
	
}








