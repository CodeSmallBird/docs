#include "stack_ltlcom.h"
#include "IdleCommon.h"
#include "wgui.h"
#include "mmi_frm_input_gprot.h"
#include <app_datetime.h>
#include "ProtocolEvents.h"
#include "DateTimeGProt.h"
#include "mmi_rp_app_bluetooth_def.h"
#include <App_str.h>
#include <Nvram_common_defs.h>
#include <conversions.h>
#include <Eint.h>
#include <L4c2uem_struct.h>
#include "MessagesMiscell.h"
#include "SMSApi.h"
#include "EventsGprot.h"
#include "BTMMISpp.h"
#include "ems.h"
#include "SmsGuiInterfaceType.h"
#include "SMSFunc.h"
#include "csmcc_enums.h"
#include "cbm_api.h"
#include "custom_data_account.h"
#include "Mmi_rp_app_audioplayer_def.h"
#include "NwInfoSrvGprot.h"
#include "math.h"
#include "common_nvram_editor_data_item.h" 
#include "soc_consts.h"
#include "soc_api.h"
#include "string.h"
#include "cbm_api.h"
#include "SmsSrvGprot.h"
#include "TimerEvents.h"
#include "ctype.h"
#include "cbm_api.h"
#include "EngineerModeAppResDef.h"
#include "app2soc_struct.h"
//#include "rtc_sw_new.h"
#include "custom_mmi_default_value.h"
#include "UcmSrvGprot.h"
#include "TimerEvents.h"
#include "mmi_rp_srv_gpio_def.h"
#include "GlobalResDef.h"
#include "ProfilesSrvGprot.h"

#include "wgui_inputs_internal.h"
#include "CommonScreensProt.h"
#include "drvsignals.h"

#ifndef __MTK_TARGET__
#include "mmiapi_struct.h" /* HAL */
#endif
#ifdef __MMI_INDIC_ALG__
#include "IndicEngineGprot.h"
#endif 

//#include "WguiCategoryGprot.h"
#include "Wgui_softkeys.h"

#include "immProt.h"

#include "MenuCuiGprot.h"

#include "mmi_rp_srv_editor_def.h"
#include "mmi_rp_menu_misc_def.h"

#include "MMIDataType.h"
#include "gui_data_types.h"
#include "gui_typedef.h"
#include "kal_general_types.h"
#include "ImeGprot.h"
#include "IndicRuleEngineCommon.h"
#include "gui_inputs.h"
#include "MMI_features.h"
#include "wgui_inputs.h"
#include "CustThemesRes.h"
#include "gui.h"
#include "wgui.h"
#include "Unicodexdcl.h"
#include "kal_public_api.h"
#include "DebugInitDef_Int.h"
#include "wgui_categories_inputs.h"
#include "string.h"
#include "wgui_inline_edit.h"
#include "gui_config.h"
#include "gui_theme_struct.h"
#include "gdi_datatype.h"
#include "gdi_include.h"
#include "wgui_categories_list.h"
#include "wgui_categories_util.h"
#include "CommonScreens.h"
#include "CustDataRes.h"
#include "GlobalResDef.h"
#include "GlobalConstants.h"
#include "mmi_frm_input_gprot.h"
#include "mmi_frm_history_gprot.h"
#include "wgui_include.h"
#include "CustMenuRes.h"
#include "IdleMain.h"
#include "math.h"
#include "stdlib.h"
#include "mmi_frm_events_gprot.h"
#include "nbr_public_struct.h"
#include "mdi_gps.h"
#include "dcl.h"         /*rtc to get time*/
#include "dcl_rtc.h"
#include "lhat_gprs.h"
#include "dcl_uart.h"
#include "app_datetime.h"

typedef struct
{
    char apn[40];
    int port;
}mmi_children_nvram_struct;

typedef enum
{
	SOC_LOGIN_FAILURE,
    SOC_LOGIN_OK,
	SOC_LOGIN_SERVICE_TIMEOUT
} soc_login_result_enum;

#ifdef __LHAT_BT4__
extern U8 g_bt_noti_enable;
#endif
extern char g_lhat_imei[20];
extern U8 g_phone_batt_percent;
extern int g_lhat_verno;
extern char g_custom_name[];
extern gas_nbr_cell_info_struct g_cell_info;
extern char gpio_reserve0_pin;
#ifdef __LHAT_GPS__
extern lhat_gps_struct g_lhat_gps;
#endif
Device_lhat lhat_device;

kal_int8 g_receive_times = 0;
soc_login_result_enum g_login_result = 0;
char g_imsi_num[17] = {0};
kal_uint32 g_calorie = 0; // QYG add
kal_uint32 g_acct_id = 0;
kal_int8 g_heart_beat = 0;

#if defined(__LHAT_COMMON__)
typedef unsigned char   u8;
typedef signed char     i8;
typedef unsigned short  u16;
typedef signed short    i16;
typedef unsigned int    u32;
typedef signed int      i32;

kal_uint8 g_start_to_gps_flag = 0;
kal_uint8 n_gprs_send_fail_count = 0;
kal_bool g_goto_sleep_flag = KAL_FALSE;
kal_bool g_ext_eint2_flag = KAL_FALSE ;
kal_bool g_bike_lock_flag = KAL_FALSE ;
kal_bool g_goto_power_5v_flag = KAL_FALSE ;
kal_bool g_ble_ext_gpio_in_flag = KAL_FALSE ;
kal_bool g_gprs_login_status_flag = KAL_FALSE ;
kal_bool g_gprs_record_status_flag = KAL_FALSE ;
//kal_bool g_gprs_open_status_flag = KAL_FALSE ;
kal_bool g_gprs_riding_status_flag = KAL_FALSE ;
kal_bool g_config_status_flag = KAL_FALSE ;
kal_bool g_getconfig_status_flag = KAL_FALSE ;
kal_bool g_breath_heart_flag=KAL_FALSE;
kal_bool g_save_nv_flag=KAL_FALSE;
kal_bool g_get_ble_mac_addr_nv_flag=KAL_FALSE;
//kal_bool g_ble_heartbeat_in_flag = KAL_FALSE ;
//kal_bool b_ble_to_lock_in_flag = KAL_FALSE ;
kal_uint8 n_warning_count = 0 ;
kal_uint16 n_breath_heart_count = 0 ;
kal_uint16 n_ble_send_count = 0 ;
kal_uint32 n_ble_receive_count = 0 ;

U8 n_acc_status = 0 ;
kal_uint8 g_gsensor_moive_flag = 0 ;
#if defined(__LHAT_SUNLIGHT_ENERGY_CHARGE_IN_COMMON__)
kal_bool b_sunlight_charge_in_flag = KAL_FALSE ;
kal_uint8 n_sunlight_count = 0;
#endif
kal_uint8 n_status = 0;
kal_uint8 n_vbat = 80;
kal_uint8 n_interval = 2;
kal_uint8 n_version = 100;
kal_uint8 n_gps_gnss = 2;
kal_uint8 n_sos_mode = 0;
kal_uint8 n_break_power_status = 0;
kal_uint8 n_gps_count = 0;
char s_user_id[100] = {"00000000-0000-0000-0000-000000000005"} ;
char s_trade_no[100] = {"123"} ;
char s_EncryptData[100] = {0} ;
char s_BMS_control_Data[50] = {0} ;
char s_BMS_ID_Data[50] = {0} ;
char s_BMS_control_data_buff[100] = {0} ;
char s_BMS_infor_data_buff[100] = {0} ;
char s_BMS_ID_data_buff[100] = {0} ;

kal_uint32 n_data_value = 0 ;
#define 	MTK2503_HUA_VERSION		101

#define 	GPRS_TEST		0
#endif
static kal_uint8 lhat_sleep_handle;

char *IMEI = "869013020000001";
//char *MAC = "d4c3257985f5";
char *MAC = "ba0eb38b61ee";
//unsigned char device_id[6] = {0xf2,0x37,0x80,0x8c,0x0e,0x56};
//unsigned char *device_id = "ba0eb38b61ee";
//unsigned char *device_id = "BA0EB38B61EE";
unsigned char *device_id = "10111213141516";
Device_ble_mac BLE_MAC;


/*unsigned char AES_KEY[81] = {
0x38,0x36,0x39,0x30,0x31,0x33,0x30,0x32,0x30,0x30,
0x30,0x30,0x30,0x30,0x31,0x64,0x34,0x63,0x33,0x32,
0x35,0x37,0x39,0x38,0x35,0x66,0x35,0xf7,0xb8,0x0e,
0x27,0xca,0x69,0xad,0x6a,0xab,0x53,0xef,0x89,0x4f,
0x23,0x5f,0x77,0x97,0x7d,0x82,0xfe,0xb5,0xb9,0xd8,
0x1c,0xa6,0xef,0x2f,0x95,0x90,0xb8,0x63,0x88,0xef,
0xf0,0x2e,0x3a,0xd8,0x5a,0x23,0x83,0xac,0x92,0x8b,
0xfa,0x34,0xea,0xf0,0xca,0xe6,0xf1,0x49,0x9c,0xaa,
};*/

unsigned char *AES_KEY = "QWXBTbbAVx8B2FA6F5Vecs2ChylCXzYIluCKuhiXIUOKd5EGKrf9cDzzIiHMyQ6D3CcY8nnjA7zdEa3f";

unsigned char aes_key[80] = {0};

#if defined(__LHAT_HUAYUAN_PROTOCOL__) || defined(__LHAT_HUAYUAN_SHANDONG__) || defined(__LHAT_TIANQIN_XINGMAI_COMMON__) || defined(__LHAT_TREE_YEAR_COMMON__) || defined(__LHAT_SHANGXUN_COMMON__) || defined(__LHAT_GUANGXIN_COMMON__)
Device_info_upload uploadInfo;
unsigned int Current_Voltage_ex(DCL_ADC_CHANNEL_TYPE_ENUM ch);

#endif

#if defined(__LHAT_RECEIVE_SMS__)	
kal_bool b_start_tcp_flag = KAL_FALSE ;
extern void srv_sample_send_sms(
		char *ucs2_content,
        char *ucs2_number
	);
extern void SendTextMessage(char *pnumber, char *ptext);
extern void mmi_sample_send_sms(S8* number, U16* content);

extern U8 srv_get_sms_to_gprs(void);
extern void srv_set_sms_to_gprs_zero(void);
extern void srv_sms_test_sms_pror(void);
extern void srv_sms_test_sms_pror_ext(void);
extern void srv_sms_send_position_ext(void);
extern void srv_sms_send_reset_system_ext(void);
#endif
extern kal_uint8 get_vbat_voltage_percent_ext(kal_uint32 n_vbat_value);

#if defined( __LHAT_HUAYUAN__) || defined(__LHAT_HUAYUAN_PROTOCOL__) || defined(__LHAT_HUAYUAN_QILI__) || defined(__LHAT_TREE_YEAR_COMMON__)

sockaddr_struct g_ip_addr =
{
    SOC_SOCK_STREAM,
    4,
    80,
    {120,76,74,11}//{112, 74, 13, 191}//{112, 74, 124, 112}
};

#elif defined(__LHAT_SHANGXUN_COMMON__)
sockaddr_struct g_ip_addr =
{
    SOC_SOCK_STREAM,
    4,
    9000,
    {106,14,220,213}//{101, 201, 114, 240}//101.201.114.240
};

#elif defined(__LHAT_GUANGXIN_COMMON__)
sockaddr_struct g_ip_addr =
{
    SOC_SOCK_STREAM,
    4,
    //80,//9580,//4130,
    //{114,55,100,103}//{114, 55, 100, 103}// 114.55.100.103  80//211.149.248.204//{39, 108, 161, 230}//{211, 149, 244, 219}
    //9888,
    //{101,37,81,242}//101.37.81.242 9888
    7878,
    {114,55,100,103}
};


#elif defined(__LHAT_HUAYUAN_SHANDONG__)
sockaddr_struct g_ip_addr =
{
    SOC_SOCK_STREAM,
    4,
    80,
    {112, 74, 124, 112}
};

#elif defined(__LHAT_TIANQIN_XINGMAI_COMMON__)

sockaddr_struct g_ip_addr =
{
    SOC_SOCK_STREAM,
    4,
    8185,
    {106, 003, 230, 229}
};


#elif defined(__LHAT_HUAYUAN_LIANHAI__)
/*typedef unsigned char   u8;
typedef signed char     i8;
typedef unsigned short  u16;
typedef signed short    i16;
typedef unsigned int    u32;
typedef signed int      i32;

kal_uint8 g_start_to_gps_flag = 0;
kal_uint8 n_gprs_send_fail_count = 0;
kal_bool g_goto_sleep_flag = 0 ;
kal_bool g_gsensor_moive_flag = 0 ;
*/

sockaddr_struct g_ip_addr =
{
    SOC_SOCK_STREAM,
    4,
    8091,//8919,
    {123, 125, 139, 114}
};

void squeeze(char *s,int c) ;

#elif defined(__LHAT_HUAYUAN_FEIMAOTUI__)
sockaddr_struct g_ip_addr =
{
    SOC_SOCK_STREAM,
    4,
    2345,//8919,
    {120, 76, 225, 208}
};

void squeeze(char *s,int c) ;

#else
sockaddr_struct g_ip_addr =
{
    SOC_SOCK_STREAM,
    4,
    81,
    {120, 76, 189, 56}
};
#endif

typedef struct
{
    kal_int32    st_longitude;
	kal_int32    st_latitude;
	char st_speed[6] ;
	char st_degree[6] ;
} mmi_gps_loss_point_gps_data_struct;

typedef struct
{
    char    st_longitude[11];
	char    st_latitude[11];
	//char st_speed[6] ;
	//char st_degree[6] ;
} mmi_gps_original_data_struct;


kal_int8 g_socket_id = -1;
mmi_children_nvram_struct g_Children;
char g_be_stole = 0;
kal_uint32 g_ridingSecs = 0;
kal_int8 g_lockCount = 0;
U8 g_sent_stole_info = 0;
U8 g_riding_flag = 0;
char g_guard_flag = 0;
kal_int8 g_setted_time = 0;
char g_guard_num[21];
U8 g_gprs_cmd = 0;
kal_bool g_first_flag = KAL_FALSE;
kal_uint8 n_first_connect_count = 0 ;
kal_uint8 g_gps_time_count = 0 ;
kal_uint16 g_gps_time_sleep_count = 0 ;
kal_uint32 g_gps_long_no_send_count = 0 ;
kal_uint16 g_gprs_no_send_count = 0 ;
kal_uint16 n_no_connect_to_server_count = 0 ;
kal_uint8 g_gps_get_time_count = 0 ;
kal_uint8 g_gps_second_count = 0 ;
//kal_int32 save_1min_point_gps_data[100][2] = {0};
//kal_uint8 save_1min_point_count = 0 ;
kal_bool g_ext_vol_check_flag = KAL_FALSE;
kal_uint8 n_ext_vol_check_count = 0 ;

kal_uint8 g_gps_no_gprs_count = 0 ;
kal_uint32 nlatitude  = 0 ;
kal_uint32 nlongitude = 0 ;
kal_uint32 gsensor_old_data = 0 ;

kal_int32 old_longitude = 0 ;
kal_int32 old_latitude  = 0 ;

kal_int32 save_longitude = 0 ;
kal_int32 save_latitude  = 0 ;
kal_int32 save_mileage  = 0 ;

//kal_int32 back_gps_data[240][2] = {0};
kal_uint16 back_count = 0 ;

//kal_int32 loss_point_gps_data[200][5] = {0};
kal_uint8 loss_point_count = 0 ;

mmi_gps_loss_point_gps_data_struct st_back_gps_data[150] ;

mmi_gps_loss_point_gps_data_struct st_loss_point_gps[150] ;
mmi_gps_original_data_struct st_shangxun_point_gps[150] ;

//kal_int32 loss_mileage_array[200] = {0};
kal_bool b_send_gps_data_flag = KAL_FALSE ;

kal_uint8 send_gps_data_fail_count = 0 ;
kal_bool b_is_connect_tcp_flag = KAL_FALSE ;
kal_bool b_lock_unclok_alarm_flag = KAL_FALSE ;
kal_bool b_motor_open_flag = KAL_FALSE ;
kal_uint8 n_lock_unclok_alarm_count = 0 ;

kal_uint8 send_mcu_count = 0 ;
kal_uint8 iccid_buf[21] = "10000000000000000000";

#if defined(__LHAT_SHANGXUN_COMMON__) || defined(__LHAT_GUANGXIN_COMMON__)
kal_bool b_bike_lock_open_flag = KAL_FALSE;//KAL_TRUE
kal_bool b_bike_lock_close_flag = KAL_TRUE ;//KAL_FALSE
kal_bool b_close_service_connect_flag = KAL_FALSE ;
kal_uint8 n_gps_updata_status = 0 ;
char back_gps_latitude[11] = {0};
char back_gps_longitude[11] = {0};
#endif
kal_bool b_alarm_trigger_flag = KAL_FALSE;


#if defined(__LHAT_HUAYUAN_SHANDONG__)
unsigned long int nTimeFre  = 5;//10;
#elif defined(__LHAT_1MIN_GET_GPS_COMMON__)
unsigned long int nTimeFre  = 120;//60;//300;
//unsigned long int nGpsUpdataFre  = 60;
#elif defined(__LHAT_SHANGXUN_COMMON__)
unsigned long int nTimeFre  = 60;//60;
#elif defined(__LHAT_GUANGXIN_COMMON__)
unsigned long int nTimeFre  = 60;//60;
#elif defined(__LHAT_SUNLIGHT_ENERGY_CHARGE_IN_COMMON__)
unsigned long int nTimeFre  = 120;
#else
unsigned long int nTimeFre  = 60;//15;//30;//60;//30 ;
#endif



extern kal_int8 soc_shutdown(kal_int8 s, kal_uint8 how);
#ifdef __LHAT_BT4__
extern void bt40_send_t6_to_app(char * jingdu, char * weidu);
extern void bt40_send_r5_to_app(void);
extern void bt40_send_riding_data(void);
#endif
#ifdef __LHAT_GPS__
extern void lhat_open_gps(void);
extern void lhat_gps_callback(mdi_gps_parser_info_enum type, void *buffer, U32 length);
#endif

#ifdef __LHAT_GSENSOR__
extern void bma250_read_xyz(void);

extern kal_uint8 bma250_get_moive_flag(void);
extern void bma250_set_gsensor_shake_count_zero(void);
#endif

void gprs_heartbeat_time(void);
void bt40_send_riding_data(void);
void gprs_soc_close(void);
unsigned int mmi_get_warning_status(void);
void mmi_set_warning_status_zero(void);
void mmi_gprs_socket_close_handle(void);
U32 mmi_gprs_get_timestamp(void);
int lhat_websocket_main(void);
void createwebsocketkey(char *out);
void test_encrypt_ecb(unsigned char *in,unsigned char *out);
void test_decrypt_ecb(char *in);
unsigned int build_websocket_msg(unsigned char* buf,unsigned int len);
unsigned char randomVal(void);

extern int base64_encode(const unsigned char *in,  unsigned long len,   
                        unsigned char *out)  ;
extern U8 srv_nw_info_get_signal_strength_in_percentage(mmi_sim_enum sim);


#if defined(__LHAT_GUANGXIN_COMMON__)

void mmi_ble_start_open_lock_time(void);
#endif

void gprs_get_acct_id(S8 *apn)
{
	kal_uint8 myapp_id = 0;
	cbm_app_info_struct info;
	info.app_str_id = 0;
	info.app_icon_id = 0;
	info.app_type = DTCNT_APPTYPE_EMAIL;

	srv_dtcnt_get_acc_id_by_apn(apn, &g_acct_id);
	kal_prompt_trace(MOD_CC, "g_acct_id get by apn %d \n ",g_acct_id);
	cbm_register_app_id_with_app_info(&info, &myapp_id); 
	if(strlen(g_Children.apn) > 0)
		g_acct_id = cbm_set_app_id(0, myapp_id);
	else
		g_acct_id = 83918337;
   	kal_prompt_trace(MOD_CC, "g_acct_id %d myapp_id =%d\n ",g_acct_id,myapp_id);
	cbm_deregister_app_id(myapp_id);
}

void gprs_acct_id(void)
{
	if(strncmp(g_imsi_num, "46000", 5) == 0 || strncmp(g_imsi_num, "46002", 5) == 0 ||
		strncmp(g_imsi_num, "46007", 5) == 0)
		strcpy(g_Children.apn, "cmnet");
	else if(strncmp(g_imsi_num, "46004", 5) == 0)
		strcpy(g_Children.apn, "cmmtm");//strcpy(g_Children.apn, "CMMTM");
	else if(strncmp(g_imsi_num, "46001", 5) == 0)
		strcpy(g_Children.apn, "uninet");	
	else if(strncmp(g_imsi_num, "23433", 5) == 0 || strncmp(g_imsi_num, "23434", 5) == 0)
		strcpy(g_Children.apn, "everywhere");                                /* ”¢π˙*/	
	else if(strncmp(g_imsi_num, "72431", 5) == 0)
		strcpy(g_Children.apn, "gprs.oi.com.br");                            /* ∞ÕŒ˜*/   
	else if(strncmp(g_imsi_num, "31041", 5) == 0 || strncmp(g_imsi_num, "310410", 5) == 0)
		strcpy(g_Children.apn, "att.mvno");                                  /* √¿π˙*/  
	else if(strncmp(g_imsi_num, "50502", 5) == 0)
		strcpy(g_Children.apn, "yesinternet");                            /* ∞ƒ¥Û¿˚—«*/	
	else if(strncmp(g_imsi_num, "40420", 5) == 0)
		strcpy(g_Children.apn, "www");                                       /* ”°∂»*/	
	else if(strncmp(g_imsi_num, "26003", 5) == 0)
		strcpy(g_Children.apn, "internet");                                  /* ≤®¿º*/	
	else if(strncmp(g_imsi_num, "51010", 5) == 0)
		strcpy(g_Children.apn, "internet");                                  /* ”°∂»ƒ·Œ˜—«*/   
	else if(strncmp(g_imsi_num, "24804", 5) == 0)
		strcpy(g_Children.apn, "send.ee");                                    /*∞Æ…≥ƒ·—«*/  
	else if(strncmp(g_imsi_num, "20801", 5) == 0)
		strcpy(g_Children.apn, "orange.fr");                                  /*∑®π˙ */   
	else if(strncmp(g_imsi_num, "20810", 5) == 0)
		strcpy(g_Children.apn, "wapsfr");                                     /*∑®π˙ */   
	else if(strncmp(g_imsi_num, "20820", 5) == 0)
		strcpy(g_Children.apn, "ebouygtel.com");                               /*∑®π˙ */
	else if(strncmp(g_imsi_num, "45400", 5) == 0 || strncmp(g_imsi_num, "45402", 5) == 0)
		strcpy(g_Children.apn, "CSL");                                        /*œ„∏€ */   
	else if(strncmp(g_imsi_num, "45401", 5) == 0 || strncmp(g_imsi_num, "45410", 5) == 0)
		strcpy(g_Children.apn, "NEW WORLD");                                 /*œ„∏€ */   
	else if(strncmp(g_imsi_num, "45404", 5) == 0)
		strcpy(g_Children.apn, "Orange");                                    /*œ„∏€ */   
	else if(strncmp(g_imsi_num, "45406", 5) == 0)
		strcpy(g_Children.apn, "SMC");                                      /*œ„∏€ */   
	else if(strncmp(g_imsi_num, "45412", 5) == 0)
		strcpy(g_Children.apn, "PEOPLES");                                  /*œ„∏€ */   
	else if(strncmp(g_imsi_num, "45416", 5) == 0)
		strcpy(g_Children.apn, "SUNDAY");                                    /*œ„∏€ */   
	else if(strncmp(g_imsi_num, "45418", 5) == 0)
		strcpy(g_Children.apn, "HK TELECOM");                                 /*œ„∏€ */   
	else if(strncmp(g_imsi_num, "52501", 5) == 0 || strncmp(g_imsi_num, "52502", 5) == 0)
		strcpy(g_Children.apn, "internet");                                  /*–¬º”∆¬ */  
	else if(strncmp(g_imsi_num, "52503", 5) == 0 || strncmp(g_imsi_num, "52504", 5) == 0)
		
		strcpy(g_Children.apn, "mobilenet");                                  /*–¬º”∆¬ */  
	else if(strncmp(g_imsi_num, "52505", 5) == 0)
		strcpy(g_Children.apn, "shwapint");                                  /*–¬º”∆¬ */  
	else if(strncmp(g_imsi_num, "47002", 5) == 0 || strncmp(g_imsi_num, "47003", 5) == 0)
		strcpy(g_Children.apn, "blweb");                                    /*√œº”¿≠ */  
#if 1		 
	else if(strncmp(g_imsi_num, "310260", 6) == 0)
		strcpy(g_Children.apn, "fast.t-mobile.com");                                  /*√¿π˙*/   
	else if(strncmp(g_imsi_num, "23450", 5) == 0)
		strcpy(g_Children.apn, "hologram");                                  /*√¿π˙*/ 
#endif	
   	kal_prompt_trace(MOD_TST, " APN   %s  g_imsi_num=%s\n ",g_Children.apn,g_imsi_num);
	gprs_get_acct_id(g_Children.apn);
}

void mmi_set_time(char *Icare_time)
{
	MYTIME mytime;
	char tmp_buf[5]={0};
	U8 i = 0;
	
	g_setted_time = 1;
	GetDateTime(&mytime);
	/*  20130810184352  */
	memset(tmp_buf, 0, sizeof(tmp_buf));
	strncpy(tmp_buf, Icare_time, 4);
    mytime.nYear = atoi(tmp_buf);
	i = 4;

	memset(tmp_buf, 0, sizeof(tmp_buf));
	strncpy(tmp_buf, Icare_time + i, 2);
    mytime.nMonth = atoi(tmp_buf);
	i += 2;

	memset(tmp_buf, 0, sizeof(tmp_buf));
	strncpy(tmp_buf, Icare_time + i, 2);
    mytime.nDay = atoi(tmp_buf);
	i += 2;

	memset(tmp_buf, 0, sizeof(tmp_buf));
	strncpy(tmp_buf, Icare_time + i, 2);
    mytime.nHour = atoi(tmp_buf);
	i += 2;

	memset(tmp_buf, 0, sizeof(tmp_buf));
	strncpy(tmp_buf, Icare_time + i, 2);
    mytime.nMin = atoi(tmp_buf);
	i += 2;

	memset(tmp_buf, 0, sizeof(tmp_buf));
	strncpy(tmp_buf, Icare_time + i, 2);
    mytime.nSec = atoi(tmp_buf);

	PhnsetSendSetTimeReqMessage(&mytime);
}

void mmi_parse_phb_data(char * p2)
{
	char * p = NULL;
	
	// {S4:18671340101}
	memset(g_guard_num, 0, sizeof(g_guard_num));
	if(p = strpbrk(p2, "}"))
	{
		*p++ = '\0';
		strcpy(g_guard_num, p2);
		kal_prompt_trace(MOD_TST, "mmi_parse_phb_data=%s", p2);
	}
}

void mmi_lock_bike(U8 lock_flag)
{
	//GPIO_ModeSetup(gpio_reserve0_pin, 0);
	//GPIO_InitIO(1, gpio_reserve0_pin);
	//if(lock_flag)
	//	GPIO_WriteIO(1, gpio_reserve0_pin);
	//else
	//	GPIO_WriteIO(0, gpio_reserve0_pin);
}

void mmi_parse_guard_data(char * p)
{
	char lock_flag = g_guard_flag;
	char *p2 = NULL;

	kal_prompt_trace(MOD_TST, "parse_guard_data1");
	if(p2 = strpbrk(p, "}"))
	{
		kal_prompt_trace(MOD_TST, "parse_guard_data2");
		*p2++ = '\0';
		g_guard_flag = atoi(p);
		if(0 == g_riding_flag)
		{
			if(0 == lock_flag && 1 == g_guard_flag)
				lhat_open_gps();
			else if(1 == lock_flag && 0 == g_guard_flag)
				mdi_gps_uart_close(mdi_get_gps_port(), MDI_GPS_UART_MODE_LOCATION, lhat_gps_callback);
		}
		g_be_stole = 0;
		g_sent_stole_info = 0;
		kal_prompt_trace(MOD_TST, "parse_guard_data3=%d", g_guard_flag);
		if(g_guard_flag == 0)
			g_lockCount = 0;
		kal_prompt_trace(MOD_TST, "parse_guard_data4");
		mmi_lock_bike(g_guard_flag);
	}
}



#if defined(__LHAT_COMMON__)
/*Ω´◊÷∑˚¥Æs÷–≥ˆœ÷µƒ◊÷∑˚c…æ≥˝*/ 
void squeeze(char *s,int c) 
{ 
    int i,j; 
    for (i = 0, j = 0; s[i] != '\0'; i++) 
    { 
        if (s[i] != c) 
        { 
            s[j++] = s[i]; 
        } 
    } 
    s[j] = '\0';    //’‚“ªÃı”Ôæ‰«ßÕÚ≤ªƒ‹Õ¸º«£¨◊÷∑˚¥ÆµƒΩ· ¯±Íº« 
} 

//posx=NMEA_Comma_Pos(p1,3); 						  
u8 NMEA_Comma_Pos(u8 *buf,u8 cx)
{	 		    
	u8 *p=buf;
	while(cx)
	{		 
		//if(*buf=='*'||*buf<' '||*buf>'z')
		//	return 0XFF;//”ˆµΩ'*'ªÚ’ﬂ∑«∑®◊÷∑˚,‘Ú≤ª¥Ê‘⁄µ⁄cx∏ˆ∂∫∫≈
		if(*buf==',')
			cx--;
		buf++;
	}
	return buf-p;	 
}

u8 lhat_rn_Comma_Pos(u8 *buf,u8 cx)
{	 		    
	u8 *p=buf;
	while(cx)
	{		 
		//if(*buf=='*'||*buf<' '||*buf>'z')
		//	return 0XFF;//”ˆµΩ'*'ªÚ’ﬂ∑«∑®◊÷∑˚,‘Ú≤ª¥Ê‘⁄µ⁄cx∏ˆ∂∫∫≈
		if(*buf=='\r\n')
			cx--;
		buf++;
	}
	return buf-p;	 
}

//m^n∫Ø ˝
//∑µªÿ÷µ:m^n¥Œ∑Ω.
u32 NMEA_Pow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}
//str◊™ªªŒ™ ˝◊÷,“‘','ªÚ’ﬂ'*'Ω· ¯
//buf: ˝◊÷¥Ê¥¢«¯
//dx:–° ˝µ„Œª ˝,∑µªÿ∏¯µ˜”√∫Ø ˝
//∑µªÿ÷µ:◊™ªª∫Ûµƒ ˝÷µ
int NMEA_Str2num(u8 *buf,u8*dx)
{
	u8 *p=buf;
	u32 ires=0,fres=0;
	u8 ilen=0,flen=0,i;
	u8 mask=0;
	int res;
	while(1) //µ√µΩ’˚ ˝∫Õ–° ˝µƒ≥§∂»
	{
		if(*p=='-'){mask|=0X02;p++;}// «∏∫ ˝
		if(*p==','||(*p=='*'))break;//”ˆµΩΩ· ¯¡À
		if(*p=='.'){mask|=0X01;p++;}//”ˆµΩ–° ˝µ„¡À
		else if(*p>'9'||(*p<'0'))	//”–∑«∑®◊÷∑˚
		{	
			ilen=0;
			flen=0;
			break;
		}	
		if(mask&0X01)flen++;
		else ilen++;
		p++;
	}
	if(mask&0X02)buf++;	//»•µÙ∏∫∫≈
	for(i=0;i<ilen;i++)	//µ√µΩ’˚ ˝≤ø∑÷ ˝æ›
	{  
		ires+=NMEA_Pow(10,ilen-1-i)*(buf[i]-'0');
	}
	if(flen>5)flen=5;	//◊Ó∂‡»°5Œª–° ˝
	*dx=flen;	 		//–° ˝µ„Œª ˝
	for(i=0;i<flen;i++)	//µ√µΩ–° ˝≤ø∑÷ ˝æ›
	{  
		fres+=NMEA_Pow(10,flen-1-i)*(buf[ilen+1+i]-'0');
	} 
	res=ires*NMEA_Pow(10,flen)+fres;
	if(mask&0X02)res=-res;		   
	return res;
}	  			

void NMEA_GPRMC_Analysis(u8 *latitude_buf , u8 *longitude_buf)
{
	//u32 temp;	
	//u8 dx;	

	kal_prompt_trace(MOD_TST, "NMEA_GPRMC_Analysis.latitude_buf111111=%s,longitude_buf111 =%s,", latitude_buf,longitude_buf);
	nlatitude = atoi(latitude_buf);
	kal_prompt_trace(MOD_TST, "NMEA_GPRMC_Analysis.latitude_buf=%d,", nlatitude);
	nlongitude = atoi(longitude_buf);
	kal_prompt_trace(MOD_TST, "NMEA_GPRMC_Analysis.longitude_buf=%d,", nlongitude);
	//temp = NMEA_Str2num(latitude_buf,&dx);	
	//kal_prompt_trace(MOD_TST, "NMEA_GPRMC_Analysis.latitude_buf=%d,dx=%d", temp,dx);
	
	//dx = 2 ;
	//temp = NMEA_Str2num(longitude_buf,&dx);	
	//kal_prompt_trace(MOD_TST, "NMEA_GPRMC_Analysis.longitude_buf=%d, ", temp);
}
/*
double rad(double d)
 {
  const double PI = 3.1415926535898;
  return d * PI / 180.0;
 }


 // ¥”¡Ω∏ˆgps◊¯±Íµ„£®æ≠Œ≥∂»£©ªÒµ√¡Ωµ„µƒ÷±œﬂæ‡¿Î£¨µ•Œª «√◊
 int CalcDistance(float fLati1, float fLong1, float fLati2, float fLong2)
 {
  const float EARTH_RADIUS = 6378.137;


  double radLat1 = rad(fLati1);
  double radLat2 = rad(fLati2);
  double a = radLat1 - radLat2;
  double b = rad(fLong1) - rad(fLong2);
  double s = 2 * asin(sqrt(pow(sin(a/2),2) + cos(radLat1)*cos(radLat2)*pow(sin(b/2),2)));
  s = s * EARTH_RADIUS;
  s = (int)(s * 10000000) / 10000;
  return s;
 }
}
*/
/*
u32 rad(u32 d)
{
	const double PI = 3.1415926535898;
	return d * PI / 180.0;
}


 // ¥”¡Ω∏ˆgps◊¯±Íµ„£®æ≠Œ≥∂»£©ªÒµ√¡Ωµ„µƒ÷±œﬂæ‡¿Î£¨µ•Œª «√◊
u32 CalcDistance(u32 fLati1, u32 fLong1, u32 fLati2, u32 fLong2)
{
  const float EARTH_RADIUS = 6378.137;


  u32 radLat1 = rad(fLati1);
  u32 radLat2 = rad(fLati2);
  u32 a = radLat1 - radLat2;
  u32 b = rad(fLong1) - rad(fLong2);
  u32 s = 2 * asin(sqrt(pow(sin(a/2),2) + cos(radLat1)*cos(radLat2)*pow(sin(b/2),2)));
  s = s * EARTH_RADIUS;
  s = (int)(s * 10000000) / 10000;
  return s;
 }
*/
double rad(double d)
{	
	const double PI = 3.1415926535898;	
	return d * PI / 180.0;
}

int CalcDistance(u32 lng1, u32 lat1, u32 lng2, u32 lat2)
{	
	double longitude1,longitude2,latitude1,latitude2;	
	double a,b,s,radLat1,radLat2;	
	double tmp1,tmp2,tmp3;	
	const float EARTH_RADIUS = 6378.137;		
	longitude1 = lng1/100000.0; 
	longitude2 = lng2/100000.0; 	
	latitude1 = lat1/100000.0;	
	latitude2 = lat2/100000.0;	
	radLat1 = rad(latitude1);	
	radLat2 = rad(latitude2);	
	a = radLat2 - radLat2;	
	if(a<0) 	
		a = radLat2 - radLat1;	
	b = rad(longitude1) - rad(longitude2);	
	if(b<0) 	
		b =  rad(longitude2) - rad(longitude1); 
	s = (long double)2 * asin(sqrt(pow(sin(a/2),2) + cos(radLat1)*cos(radLat2)*pow(sin(b/2),2)));	
	s = s * EARTH_RADIUS;											
	s = (int)(s * 10000000) / 10000;			//?°¡	
	return s;
}

u32 my_sqrt(u32 number) 
{
    u32 new_guess;
    u32 last_guess;
 
    if (number < 0) {
        return -1;
    }
    if(number==0) return 0;
    new_guess = 1;
    do {
        last_guess = new_guess;
        new_guess = (last_guess + number / last_guess) / 2;
    } while (new_guess != last_guess);
 
    return new_guess;
}

u32 gpsDistance( u32 x1,u32 y1,u32 x2,u32 y2)
{
	u32 distance = 0;
	//u32 result = (x1-x2)*(x1-x2)+(y1-y2)*(y1-y2);
	
	distance=my_sqrt( (x1-x2)*(x1-x2)+(y1-y2)*(y1-y2) );
	return distance;
}

u32 GetTwoGPsDistance(u32 x1,u32 y1,u32 x2,u32 y2)
{
	u32 distance;

	distance = gpsDistance(x1,y1,x2,y2);
	return distance ;
}

void NMEA_set_latitude_longitude_zero(void)
{
	nlatitude = 0 ;
	nlongitude = 0 ;
	memset(&g_lhat_gps, 0, sizeof(g_lhat_gps));
	
	//memset(&back_gps_data, 0, sizeof(back_gps_data));
	memset(&st_back_gps_data, 0, sizeof(st_back_gps_data));
	n_gps_count = 0 ;
}

void loss_point_data_set_latitude_longitude_zero(void)
{
	memset(&st_loss_point_gps, 0, sizeof(mmi_gps_loss_point_gps_data_struct));
	//memset(&loss_point_gps_data, 0, sizeof(loss_point_gps_data));
	loss_point_count = 0 ;
}

void original_data_set_latitude_longitude_zero(void)
{
	memset(&st_shangxun_point_gps, 0, sizeof(mmi_gps_original_data_struct));
	n_gps_count = 0 ;
}


void mmi_old_latitude_longitude_zero(void)
{
	old_latitude = 0 ;
	old_longitude = 0 ;
}


kal_bool get_gps_latitude_value(void)
{
	kal_prompt_trace(MOD_TST, "get_gps_latitude_value g_lhat_gps.longitude=%d,g_goto_sleep_flag=%d ", g_lhat_gps.longitude,g_goto_sleep_flag);
	if (g_lhat_gps.longitude != 0)
	{
		return KAL_TRUE ;
	}

	return KAL_FALSE ;
}

kal_bool mmi_set_sleep_flag(void)
{
	return g_goto_sleep_flag ;
}

//#define MMI_PCM_GPIO_ONOFF				48
#define MMI_5V_DC_ONOFF					3//5//15
#define MMI_BUZZER_GPIO_DC_ONOFF		28//33
//#define MMI_LED_GPIO_ONOFF				32
//#define MMI_BLE_EXT_INPUT_GPIO			14//29
//#define MMI_BLE_EXT_OUPUT_GPIO1		28
//#define MMI_BLE_EXT_OUPUT_GPIO2		27
#define MMI_BLE_HEART_GPIO				8//26
//#define MMI_BLE_HEARTBEAT_GPIO			4//	1
//#define MMI_PORTABLE_SOURCE_GPIO		49

//#define MMI_MOTO_EN_GPIO				3//5//3//5
#define MMI_MOTO_IA_GPIO				30//27
#define MMI_MOTO_IB_GPIO				27//30

#define MMI_MT2503_TO_RESET_BLE_GPIO	5
#define MMI_MT2503_TO_POWER_CONTROL_GPIO	46



static void mmi_Delayus(kal_uint32 data)
{
	kal_uint16 i;
	while(data--)
	{
		for(i=0;i<43;i++){}
	}
}

static void mmi_Delayms(kal_uint32 data)
{
	while(data--)
	{
		mmi_Delayus(1000);
	}
}

void mmi_ble_heart_delay(kal_uint16 time)
{   //Time delay function
	kal_uint16 i;
	
	for(i = 0; i < time; i++);                //Software loop for time delay
}

/*
void mmi_pcm_gpio_onoff(U8 onoff)
{
	GPIO_ModeSetup(MMI_PCM_GPIO_ONOFF, 0);
	GPIO_InitIO(1, MMI_PCM_GPIO_ONOFF);
	if(onoff)
		GPIO_WriteIO(1, MMI_PCM_GPIO_ONOFF);
	else
		GPIO_WriteIO(1, MMI_PCM_GPIO_ONOFF);
}
*/
void lhat_5v_dc_onoff(U8 onoff)
{
	GPIO_ModeSetup(MMI_5V_DC_ONOFF, 0);
	GPIO_InitIO(1, MMI_5V_DC_ONOFF);
	if(onoff)
		GPIO_WriteIO(1, MMI_5V_DC_ONOFF);
	else
		GPIO_WriteIO(0, MMI_5V_DC_ONOFF);
}

void lhat_buzzer_onoff(U8 onoff)
{
	GPIO_ModeSetup(MMI_BUZZER_GPIO_DC_ONOFF, 0);
	GPIO_InitIO(1, MMI_BUZZER_GPIO_DC_ONOFF);
	if(onoff)
		GPIO_WriteIO(1, MMI_BUZZER_GPIO_DC_ONOFF);
	else
		GPIO_WriteIO(0, MMI_BUZZER_GPIO_DC_ONOFF);
}
/*
void lhat_led_ext_onoff(U8 onoff)
{
	GPIO_ModeSetup(MMI_LED_GPIO_ONOFF, 0);
	GPIO_InitIO(1, MMI_LED_GPIO_ONOFF);
	if(onoff)
		GPIO_WriteIO(1, MMI_LED_GPIO_ONOFF);
	else
		GPIO_WriteIO(0, MMI_LED_GPIO_ONOFF);
}

void lhat_get_blue_ext_gpio_status(void)
{
	GPIO_ModeSetup(MMI_BLE_EXT_INPUT_GPIO, 0);
	GPIO_InitIO(INPUT, MMI_BLE_EXT_INPUT_GPIO);
	if (1 == GPIO_ReadIO(MMI_BLE_EXT_INPUT_GPIO))
	{
		g_ble_ext_gpio_in_flag = KAL_TRUE ;
	}
	else
	{
		g_ble_ext_gpio_in_flag = KAL_FALSE;
	}
}
*/

/*kal_bool lhat_get_blue_heartbeat_status(void)
{
	GPIO_ModeSetup(MMI_BLE_HEARTBEAT_GPIO, 0);
	GPIO_InitIO(INPUT, MMI_BLE_HEARTBEAT_GPIO);
	if (1 == GPIO_ReadIO(MMI_BLE_HEARTBEAT_GPIO))
	{
		//g_ble_heartbeat_in_flag = KAL_TRUE ;
		return KAL_TRUE;
	}
	else
	{
		//g_ble_heartbeat_in_flag = KAL_FALSE;
		return KAL_FALSE;
	}
}*/


void lhat_ble_output1_gpio(U8 onoff)
{
	/*GPIO_ModeSetup(MMI_BLE_EXT_OUPUT_GPIO1, 0);
	GPIO_InitIO(1, MMI_BLE_EXT_OUPUT_GPIO1);
	if(onoff)
		GPIO_WriteIO(1, MMI_BLE_EXT_OUPUT_GPIO1);
	else
		GPIO_WriteIO(0, MMI_BLE_EXT_OUPUT_GPIO1);
	*/
}

void lhat_ble_output2_gpio(U8 onoff)
{
	/*GPIO_ModeSetup(MMI_BLE_EXT_OUPUT_GPIO2, 0);
	GPIO_InitIO(1, MMI_BLE_EXT_OUPUT_GPIO2);
	if(onoff)
		GPIO_WriteIO(1, MMI_BLE_EXT_OUPUT_GPIO2);
	else
		GPIO_WriteIO(0, MMI_BLE_EXT_OUPUT_GPIO2);
	*/
}

void lhat_MOTO_IA_gpio(U8 onoff)
{
	GPIO_ModeSetup(MMI_MOTO_IA_GPIO, 0);
	GPIO_InitIO(1, MMI_MOTO_IA_GPIO);
	if(onoff)
		GPIO_WriteIO(1, MMI_MOTO_IA_GPIO);
	else
		GPIO_WriteIO(0, MMI_MOTO_IA_GPIO);
}

void lhat_MOTO_IB_gpio(U8 onoff)
{
	GPIO_ModeSetup(MMI_MOTO_IB_GPIO, 0);
	GPIO_InitIO(1, MMI_MOTO_IB_GPIO);
	if(onoff)
		GPIO_WriteIO(1, MMI_MOTO_IB_GPIO);
	else
		GPIO_WriteIO(0, MMI_MOTO_IB_GPIO);
}



void lhat_ble_heart_gpio(U8 onoff)
{
	GPIO_ModeSetup(MMI_BLE_HEART_GPIO, 0);
	GPIO_InitIO(1, MMI_BLE_HEART_GPIO);
	if(onoff)
		GPIO_WriteIO(1, MMI_BLE_HEART_GPIO);
	else
		GPIO_WriteIO(0, MMI_BLE_HEART_GPIO);
}

void mmi_set_ble_ouput_time_handle(void)
{
	kal_prompt_trace(MOD_TST, "mmi_set_ble_ouput_time_handle,");
	//mmi_ble_heart_delay(200) ;
	lhat_ble_output1_gpio(0);
	lhat_ble_output2_gpio(0);
	StopTimer(GIS_TIMER_ID_23);
}


void mmi_set_ble_ouput_timer_status(void)
{
	srv_nw_info_service_availability_enum availablity = srv_nw_info_get_service_availability(MMI_SIM1);

	kal_prompt_trace(MOD_TST, "mmi_set_ble_ouput_timer_status,availablity=%d,back_gps_latitude=%s",availablity,back_gps_latitude);
	
	if(SRV_NW_INFO_SA_FULL_SERVICE == availablity)
	{
		lhat_ble_output1_gpio(1);
		mmi_Delayms(200);
		lhat_ble_output2_gpio(1);
	}
	else if ((SRV_NW_INFO_SA_NO_SERVICE == availablity) || (SRV_NW_INFO_SA_LIMITED_SERVICE == availablity))
	{
		lhat_ble_output1_gpio(0);
		mmi_Delayms(200);
		lhat_ble_output2_gpio(1);
	}
	else if (strlen(back_gps_latitude) == 0)
	{
		lhat_ble_output1_gpio(1);
		mmi_Delayms(200);
		lhat_ble_output2_gpio(0);
	}
	else
	{
		lhat_ble_output1_gpio(0);
		mmi_Delayms(200);
		lhat_ble_output2_gpio(0);
	}
	StartTimer(GIS_TIMER_ID_23, 2500, mmi_set_ble_ouput_time_handle);
}

/*void mmi_set_ble_heart_gpio_time_handle(void)
{
	kal_prompt_trace(MOD_TST, "mmi_set_ble_heart_gpio_time_handle,");
	StopTimer(GIS_TIMER_ID_26);
	lhat_ble_heart_gpio(1);
	mmi_Delayms(500);
	lhat_ble_heart_gpio(0);
	StartTimer(GIS_TIMER_ID_26, 120000, mmi_set_ble_heart_gpio_time_handle);
}

void mmi_set_ble_heart_gpio_status(void)
{
	//StopTimer(GIS_TIMER_ID_26);
	StartTimer(GIS_TIMER_ID_26, 120000, mmi_set_ble_heart_gpio_time_handle);
}*/

void lhat_portable_source_gpio(U8 onoff)
{
	/*GPIO_ModeSetup(MMI_PORTABLE_SOURCE_GPIO, 0);
	GPIO_InitIO(1, MMI_PORTABLE_SOURCE_GPIO);
	if(onoff)
		GPIO_WriteIO(1, MMI_PORTABLE_SOURCE_GPIO);
	else
		GPIO_WriteIO(0, MMI_PORTABLE_SOURCE_GPIO);
	*/
}

void mmi_portable_source_open_time_handle(void)
{
	kal_prompt_trace(MOD_TST, "mmi_portable_source_time_handle");
	
	lhat_portable_source_gpio(0);
	StopTimer(GIS_TIMER_ID_27);
}

void mmi_portable_source_open_timer_status(void)
{
	lhat_portable_source_gpio(1);
	StartTimer(GIS_TIMER_ID_27, 200, mmi_portable_source_open_time_handle);
}

void mmi_portable_source_close_time_handle(void)
{
	kal_prompt_trace(MOD_TST, "mmi_portable_source_time_handle");
	
	/*lhat_portable_source_gpio(0);
	mmi_Delayms(200);
	lhat_portable_source_gpio(1);
	mmi_Delayms(200);
	lhat_portable_source_gpio(0);*/
	StopTimer(GIS_TIMER_ID_28);
}

void mmi_portable_source_close_timer_status(void)
{
	lhat_portable_source_gpio(1);
	StartTimer(GIS_TIMER_ID_28, 200, mmi_portable_source_close_time_handle);
}

void mmi_stop_motor_handle(void)
{
	kal_prompt_trace(MOD_TST, "mmi_stop_motor_handle");

	lhat_5v_dc_onoff(0);
	lhat_MOTO_IA_gpio(0);
	lhat_MOTO_IB_gpio(0);
	StopTimer(GIS_TIMER_ID_29);
}

void mmi_motor_open_timer(void)
{
	StopTimer(GIS_TIMER_ID_29);
	StartTimer(GIS_TIMER_ID_29, 200, mmi_stop_motor_handle);
}

/*
void lhat_pmc_onoff(U8 onoff)
{
	GPIO_ModeSetup(48, 0);
	GPIO_InitIO(1, 48);
	if(onoff)
		GPIO_WriteIO(1, 48);
	else
		GPIO_WriteIO(0, 48);
}*/

void mmi_gps_power_onoff(U8 onoff)
{
#if defined(__LHAT_ACC_ON_NO_LOWPOWER__)
	GPIO_ModeSetup(19, 0);
	GPIO_InitIO(1, 19);
	if(onoff)
		GPIO_WriteIO(1, 19);
	else
		GPIO_WriteIO(1, 19);
#else
	GPIO_ModeSetup(19, 0);
	GPIO_InitIO(1, 19);
	if(onoff)
		GPIO_WriteIO(1, 19);
	else
		GPIO_WriteIO(0, 19);
#endif	
}

void mmi_gps_open_again(void)
{
	
	mmi_gps_power_onoff(1);
	lhat_open_gps();
	//StartTimer(GIS_TIMER_ID_2, 1000, gprs_heartbeat_time);
}

void mmi_set_gps_array_zore(void)
{
	//memset(back_gps_data,0,sizeof(back_gps_data));
	memset(st_back_gps_data,0,sizeof(st_back_gps_data));
	back_count = 0 ;
	//old_longitude = 0 ;
	//old_latitude = 0 ;
}

int chkport_ext(int port)  
{  
    if (port < 0 && port > 65536)  
    {  
        return -1;  
    }
	else  
    {  
    	g_ip_addr.port= port ;
		kal_prompt_trace(MOD_TST, "g_ip_addr.port=%d",g_ip_addr.port);
        return 0;  
    }  
}  

int chkip_ext(char *ip)  
{  
	int ret = 0;
	int i = 0;  
	int count[2] = {0};  
	const char *s = ".";  	
	char TempIP[50];  
	//255.255.255.255
	int IPAddr[4] = {0};  
	//    int pos[3] = {0};  
	kal_prompt_trace(MOD_TST, "chkip =%s",ip);	
	ret = chkip_domain(ip);
	if(ret<0)
	{			
		kal_prompt_trace(MOD_TST, "not support domain");
		return -1;
	}

	memset(TempIP, 0, sizeof(TempIP));  
	memcpy(TempIP, ip, sizeof(TempIP));  
    for(i = 0; i < strlen(TempIP); i++)  
    {  
        if(TempIP[0] != '.' && TempIP[i] == '.'   
            && TempIP[i+1] != '\0' && TempIP[i+1] != '.')  
        {  
            count[0]++;  
        }    
        if(!isdigit(TempIP[i]))  
        {  
            count[1]++;  
        }  
          
    }  
   
    if(count[0] != 3 || count[1] != 3)  
    {  
        return -1;  
    }  
      
    IPAddr[0] = atoi(strtok(TempIP, s));  
    IPAddr[1] = atoi(strtok(NULL, s));  
    IPAddr[2] = atoi(strtok(NULL, s));  
    IPAddr[3] = atoi(strtok(NULL, s));  
  	
    if ((IPAddr[0] >= 0&&IPAddr[0] <= 255)&&(IPAddr[1] >= 0&&IPAddr[1] <= 255)  
        &&(IPAddr[2] >= 0&&IPAddr[2] <= 255)&&(IPAddr[3] >= 0&&IPAddr[3] <= 255))  
    {     
    	g_ip_addr.addr[0] = IPAddr[0] ;
		g_ip_addr.addr[1] = IPAddr[1] ;
		g_ip_addr.addr[2] = IPAddr[2] ;
		g_ip_addr.addr[3] = IPAddr[3] ;
		kal_prompt_trace(MOD_TST, "g_ip_addr.addr=%d.%d.%d.%d",g_ip_addr.addr[0],g_ip_addr.addr[1],g_ip_addr.addr[2],g_ip_addr.addr[3]);
        return 0;         
    }  
    else  
    {  
        return -1;  
    }  
}  

void mmi_lhat_check_ip_port_status(void)
{
	int ret_data = 0 ;
	
	//ret_data = chkport_ext(lhat_device.port);
	//ret_data = chkip_ext(lhat_device.ip);
}

void mmi_set_time_fre_value(U8 nn_time_value)
{
	switch(nn_time_value)
	{
		case 0:
			{
				nTimeFre = 20 ;	
			}
			break ;
		case 1:
			{
				nTimeFre = 30 ;		
			}
			break ;
		case 2:
			{
				nTimeFre = 60 ;		
			}
			break ;
		case 3:
			{
				nTimeFre = 90 ;	
			}
			break ;
		case 4:
			{
				nTimeFre = 120 ;	
			}
			break ;	
		case 5:
			{
				nTimeFre = 180 ;	
			}
			break ;
		case 6:
			{
				nTimeFre = 300 ;	
			}
			break ;
		case 7:
			{
				nTimeFre = 600 ;	
			}
			break ;
		case 8:
			{
				nTimeFre = 3600 ;	
			}
			break ;
		case 9:
			{
				nTimeFre = 24 * 3600 ;	
			}
			break ;
		default:
			{
				nTimeFre = 60 ;	
			}
			break ;	
	}
	kal_prompt_trace(MOD_TST, "mmi_set_time_fre_value nTimeFre=%d ", nTimeFre);
}

void setTel(char *tel)
{
	int ret = 0;
	ret = chksim(tel);
	if(!ret)
	{
		memset(lhat_device.num,0,sizeof(lhat_device.num));
		memcpy(lhat_device.num,tel,strlen(tel));
	}
}
void setFre(int num)
{
	if(num<0||num>=10)
		return ;
	switch(num)
	{
		case 0:
			lhat_device.fre = 60;//20;
			break;
		case 1:
			lhat_device.fre = 30;
			break;
		case 2:
			lhat_device.fre = 60;
			break;
		case 3:
			lhat_device.fre = 90;
			break;
		case 4:
			lhat_device.fre = 120;
			break;
		case 5:
			lhat_device.fre = 180;
			break;
		case 6:
			lhat_device.fre = 300;
			break;
		case 7:
			lhat_device.fre = 600;
			break;
		case 8:
			lhat_device.fre = 3600;
			break;
		case 9:
			lhat_device.fre = 24*3600;
			break;
		case 10:
			lhat_device.fre = 2*24*3600;
			break;
		case 11:
			lhat_device.fre = 3*24*3600;
			break;
		case 12:
			lhat_device.fre = 4*24*3600;
			break;
		case 13:
			lhat_device.fre = 5*24*3600;
			break;
		default:
			lhat_device.fre = 60;
			break;		
	}

	nTimeFre = lhat_device.fre ;
	kal_prompt_trace(MOD_TST, " setFre nTimeFre=%d",nTimeFre );
}

U8 getFre(unsigned long int num_fre)
{
	U8 ret_val = 0 ;
	//if(num<0||num>=10)
	//	return ;
	switch(num_fre)
	{
		case 20:
			ret_val = 0;
			break;
		case 30:
			ret_val = 1;
			break;
		case 60:
			ret_val = 2 ;
			break;
		case 90:
			ret_val = 3 ;
			break;
		case 120:
			ret_val = 4 ;
			break;
		case 180:
			ret_val = 5 ;
			break;
		case 300:
			ret_val = 6 ;
			break;
		case 600:
			ret_val = 7 ;
			break;
		case 3600:
			ret_val = 8 ;
			break;
		case 14400:
			ret_val = 9 ;
			break;
		default:
			ret_val = 2 ;
			break;		
	}
	return ret_val ;
}

unsigned long int GetFreValue(void)
{
	return lhat_device.fre;
}

unsigned char Get_s_f_value(void)
{
	return lhat_device.s_f_value ;
}

void setAlarm(unsigned int alarm)
{
	//if(lhat_device.alarm_set!=alarm)
	{
		lhat_device.alarm_set = alarm;
		kal_prompt_trace(MOD_TST, "lhat_device.alarm_set =%d",lhat_device.alarm_set);
	}
	//lhat_device.alarm_set |= 1<<WARN_NOPOWER;
}

//"s_m":"1064851540967"
int parseTelString(char *buff)
{
	int ret = 0;
	char *pHead = NULL;
	char i = 0;
	char tel[16] = {0};
	if(!buff)
		return -1;
	pHead = strstr(buff,"s_m");
	if(!pHead&&pHead+6)
		return -1;
	pHead +=6;
	while(*pHead!='"')
	{
		tel[i] = *pHead;
		i++;
		pHead++;
		if(i>=sizeof(tel))
			return -1;
	}
	
	kal_prompt_trace(MOD_TST, "parseTelString =%s",tel);
	setTel(tel);	
}

int parseParamString(char *buff,int cmd)
{
	int value = 0;
	int ret = 0;
	char *pHead = NULL;
	char i = 0;
	char tmp[16] = {0};
	if(!buff)
		return -1;
	
	switch(cmd)
	{
		case 0:
		{
			//c_f":0
			pHead = strstr(buff,"c_f");
			if(pHead&&pHead+5)
			{
				pHead += 5 ;
				while(*pHead!='}')
				{
					tmp[i] = *pHead;
					i++;
					pHead++;
					if(i>=sizeof(tmp))
						return -1;
				}
				value = atoi((char *)tmp);			
				kal_prompt_trace(MOD_TST, "CHANGE,C_F =%d",value);
			}
		}
		break;	
		case 1:						//fre
		{
			pHead = strstr(buff,"u_t");
			if(pHead&&pHead+5)
			{
				pHead += 5 ;
				while(*pHead!=',')
				{
					tmp[i] = *pHead;
					i++;
					pHead++;
					if(i>=sizeof(tmp))
						return -1;
				}
				value = atoi((char *)tmp);			
				kal_prompt_trace(MOD_TST, "fre,U_T =%d",value);
			}
		}
		break;	
		case 2:
		{
			//s_f":1,
			pHead = strstr(buff,"s_f");
			if(pHead&&pHead+5)
			{
				pHead += 5 ;
				while(*pHead!=',')
				{
					tmp[i] = *pHead;
					i++;
					pHead++;
					if(i>=sizeof(tmp))
						return -1;
				}
				value = atoi((char *)tmp);			
				kal_prompt_trace(MOD_TST, "shutdown,S_F =%d",value);
			}
		}
		break;		
		case 3:
		{
			//s_t":1,
			pHead = strstr(buff,"s_t");
			if(pHead&&pHead+5)
			{
				pHead += 5 ;
				while(*pHead!=',')
				{
					tmp[i] = *pHead;
					i++;
					pHead++;
					if(i>=sizeof(tmp))
						return -1;
				}
				value = atoi((char *)tmp);			
				kal_prompt_trace(MOD_TST, "alarmset,S_T =%d",value);
			}
		}
		break;	
		case 4:
		{
			//r_f":1,
			pHead = strstr(buff,"r_f");
			if(pHead&&pHead+5)
			{
				pHead += 5 ;
				while(*pHead!=',')
				{
					tmp[i] = *pHead;
					i++;
					pHead++;
					if(i>=sizeof(tmp))
						return -1;
				}
				value = atoi((char *)tmp);			
				kal_prompt_trace(MOD_TST, "reset,R_F =%d",value);
			}
		}
		break;	
		case 5:
		{
			//s_f":1,
			pHead = strstr(buff,"u_f");
			if(pHead&&pHead+5)
			{
				pHead += 5 ;
				while(*pHead!=',')
				{
					tmp[i] = *pHead;
					i++;
					pHead++;
					if(i>=sizeof(tmp))
						return -1;
				}
				value = atoi((char *)tmp);			
				kal_prompt_trace(MOD_TST, "update,U_F =%d",value);
			}
		}
		break;	
		default:
			break;
	}
	return value;
}
int parseSeverRsp_string(char *buff)
{
#if 0
	unsigned int value;
	char *pHead;
	int ret = 0;
	kal_prompt_trace(MOD_TST, "parseSeverRsp_string" );
	ret = parseParamString(buff,0);			//c_f	7		//…Ë÷√±Í÷æ
	if(ret == 1)
	//if(1)				//test
	{	
		value = parseParamString(buff,1);	//u_t			//…œ¥´º‰∏Ù
		if(value!=-1)
			setFre(value);
#if defined(__LHAT_TREE_YEAR_COMMON__)		
		if (value >=9 && value <=13)
		{
			send_mcu_count = ((value - 8) % 8 ) * 24 ;
			gprs_set_mcu_time_fun();
		}
#endif		
		value = parseParamString(buff,2);	//s_f			//∂œ”ÕµÁ	
		if(value == 1)
		{
			//TUDO
			lhat_device.s_f_value = value ;
			if (lhat_device.s_f_value == 1)
			{
				mmi_pcm_gpio_onoff(1);
			}
			else
			{
				mmi_pcm_gpio_onoff(0);
			}
		}
		value = parseParamString(buff,3);	//s_t			//±®æØ…Ë÷√
		if(value!=-1)
			setAlarm(value);
		//value = parseParamString(buff,5);	//u_f			//»Ìº˛∏¸–¬±Í÷æ 
		parseTelString(buff);				//s_m			//µÁª∞∫≈¬Î	
		//–¥»ÎNVRAM
		Clear_NVRAM_Write();
		Restore_NVRAM_Write(0);
		value = parseParamString(buff,4);	//r_f			//÷ÿ∆Ù
		if(value == 1)
		{
			//TUDO
			kal_prompt_trace(MOD_TST, " restart system " );
			AlmATPowerReset(MMI_FALSE, 30);
		}

		return 0;	
	}
	else
	{
		return -1;
	}
	
#endif

}

#endif

#if defined(__LHAT_SHANGXUN_COMMON__) || defined(__LHAT_GUANGXIN_COMMON__)
kal_bool mmi_get_bike_lock_open_status(void)
{
	return b_bike_lock_open_flag ;
}

kal_bool mmi_get_bike_lock_close_init_status(void)
{
	return b_bike_lock_close_flag ;
}

void mmi_set_bike_lock_open_status(kal_bool b_bike_lock_value)
{
	b_bike_lock_open_flag = b_bike_lock_value ;
}

void mmi_set_bike_lock_close_init_status(kal_bool b_bike_lock_value)
{
	b_bike_lock_close_flag = b_bike_lock_value ;
}

void mmi_set_gps_updata_status(kal_uint8 nn_updata_status)
{
	n_gps_updata_status = nn_updata_status ;
}

kal_uint8 mmi_get_gps_updata_status(void)
{
	kal_prompt_trace(MOD_TST, "n_gps_updata_status=%d", n_gps_updata_status);
	return n_gps_updata_status ;
}

void mmi_heart_gps_updata_status(void)
{
	if (g_ext_vol_check_flag==KAL_TRUE)
	{
		mmi_set_gps_updata_status(9) ;
	}
	else if (((KAL_TRUE == mmi_get_bike_lock_open_status()) && (KAL_FALSE == mmi_get_bike_lock_close_init_status())) || ( KAL_TRUE== mmi_get_bike_lock_status()))
	{
		mmi_set_gps_updata_status(6) ;
	}
	else if (((KAL_FALSE == mmi_get_bike_lock_open_status()) && (KAL_TRUE == mmi_get_bike_lock_close_init_status())) || (KAL_TRUE == mmi_get_eint2_status()))
	{
		if(KAL_TRUE == mmi_get_eint2_status())
		{
			mmi_set_gps_updata_status(7) ;
		}
		else if ((((KAL_FALSE == mmi_get_bike_lock_open_status()) && (KAL_TRUE == mmi_get_bike_lock_close_init_status()))) && ((2 == g_gsensor_moive_flag) || (1 == g_gsensor_moive_flag)))
		{
			mmi_set_gps_updata_status(8) ;
		}
		else
		{
			mmi_set_gps_updata_status(7) ;
		}
		
	}
	else
	{
		mmi_set_gps_updata_status(0) ;
	}
}

kal_uint8 mmi_get_bike_lock_unlock_status(void)
{
	kal_uint8 ret_data = 0 ;
	
	if (((KAL_TRUE == mmi_get_bike_lock_open_status()) && (KAL_FALSE == mmi_get_bike_lock_close_init_status())) || ( KAL_TRUE== mmi_get_bike_lock_status()))
	{
		return 1 ;
	}
	else if (((KAL_FALSE == mmi_get_bike_lock_open_status()) && (KAL_TRUE == mmi_get_bike_lock_close_init_status())) || (KAL_TRUE == mmi_get_eint2_status()))
	{
		return 0 ;
	}
	else
	{
		return 2 ;
	}

}

#endif

#if defined(__LHAT_RECEIVE_SMS__)
void mmi_gprs_set_start_goto_tcp(void)
{
	b_start_tcp_flag = KAL_TRUE ;
}

kal_bool mmi_grps_get_tcp_status(void)
{
	return b_start_tcp_flag ;
}

void mmi_gprs_set_tcp_status_to_zero(void)
{
	b_start_tcp_flag = KAL_FALSE;
}

kal_int32 mmi_get_latitude_value(void)
{
	return old_latitude;
}

kal_int32 mmi_get_longitude_value(void)
{
	return old_longitude;
}

#endif

#if 1//defined(__LHAT_HUAYUAN_SHANDONG__)
void mmi_set_gpio_time_handle(void)
{
	kal_prompt_trace(MOD_TST, "mmi_set_gpio_time_handle,");
	
	lhat_buzzer_onoff(0);
	//lhat_led_ext_onoff(0);
	mmi_Delayms(18);
	lhat_buzzer_onoff(1);
	//lhat_led_ext_onoff(1);
	mmi_Delayms(100);
	lhat_buzzer_onoff(0);
	//lhat_led_ext_onoff(0);
	StopTimer(GIS_TIMER_ID_20);
}


void mmi_set_gpio_timer_status(void)
{
	StopTimer(GIS_TIMER_ID_20);
	lhat_buzzer_onoff(1);
	//lhat_led_ext_onoff(1);
	StartTimer(GIS_TIMER_ID_20, 100, mmi_set_gpio_time_handle);
}

void mmi_set_gpio_warning_time_handle(void)
{
	kal_prompt_trace(MOD_TST, "mmi_set_gpio_warning_time_handle,");

	StopTimer(GIS_TIMER_ID_25);

	n_warning_count++ ;
	if (n_warning_count >=15)
	{
		lhat_buzzer_onoff(0);
		//lhat_led_ext_onoff(0);
		n_warning_count = 0 ;
		return ;
	}
	if (n_warning_count%2 == 0)
	{
		lhat_buzzer_onoff(0);
		//lhat_led_ext_onoff(0);
	}
	else
	{
		lhat_buzzer_onoff(1);
		//lhat_led_ext_onoff(1);
	}
	
	StartTimer(GIS_TIMER_ID_25, 300, mmi_set_gpio_warning_time_handle);
}

void mmi_set_gpio_warning_timer_status(void)
{
	lhat_buzzer_onoff(1);
	//lhat_led_ext_onoff(1);
	n_warning_count = 0 ;
	StopTimer(GIS_TIMER_ID_25);
	StartTimer(GIS_TIMER_ID_25, 300, mmi_set_gpio_warning_time_handle);
}

void mmi_set_bike_lock_status(kal_bool bb_eint_flag)
{
	g_bike_lock_flag = bb_eint_flag ;
}

kal_bool mmi_get_bike_lock_status(void)
{
	return g_bike_lock_flag ;
}

void mmi_set_eint2_status(kal_bool bb_eint_flag)
{
	g_ext_eint2_flag = bb_eint_flag ;
}

kal_bool mmi_get_eint2_status(void)
{
	return g_ext_eint2_flag ;
}

void mmi_set_acc_status_value(U8 nn_acc_satus)
{
	n_acc_status = nn_acc_satus ;
}

U8 mmi_get_acc_status_value(void)
{
	return n_acc_status  ;
}

void mmi_set_acc_status_zero(void)
{
	n_acc_status = 0 ;
}


#endif

#if 1
//extern int base64_encode(const unsigned char *in,  unsigned long len,   
//                        unsigned char *out)  ;
void test_code(void)
{
	unsigned char random[16] = {0};
	unsigned char out[16] = {0};
	unsigned char i = 0;

	for(i=0;i<16;i++)
	{
		random[i] = (rand()%255+1);
	}
	//base64_encode(random, 16, out);
}
#endif

#if defined(__LHAT_HUAYUAN__)


void gprs_send_buf_reply(kal_int8 num)
{
	srv_nw_info_service_availability_enum availablity = srv_nw_info_get_service_availability(MMI_SIM1);
	
	if(SRV_NW_INFO_SA_FULL_SERVICE == availablity)
	{
		kal_int32 ret=0;
		int len = 0,i = 0;
		//int len1 = 0;
		char kph[20];
		char msl[20];
		char  gps_info_buffer[500] = {0};
		char  gps_info_buffer1[500] = {0};
		kal_int32 test_longitude = 0;//11385715 ;
		kal_int32 test_latitude  = 0;//2256911 ;
		//char *gps_info_buffer = NULL ;
		//unsigned char temp_buff[400] = "dddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddd";
		kal_prompt_trace(MOD_TST, "g_lhat_gps.east_west=%d, g_lhat_gps.east_west=%d,", g_lhat_gps.east_west, g_lhat_gps.north_south);
		
		memset(gps_info_buffer, 0, sizeof(gps_info_buffer));
		
		switch(num)
		{
			case 1:
			case 2:	
				{
					if(g_lhat_gps.east_west || g_lhat_gps.north_south)
					{
						char jingdu1[12] = {0}, weidu1[12] = {0};
						int len1 = 0;
						char nnlatitude[8] = {0};
						char nnlongitude[7] = {0};
						kal_int32 test_longitude11 = 11385715 ;
						kal_int32 test_latitude11  = 2256911 ;
						
						char simid[13] = "1064851542568";
						//u32 nnlatitude = 0 , nnlongitude = 0 ;
						
						jingdu_change(jingdu1);
						squeeze(jingdu1,'.');
						//memset(nnlatitude, 0, sizeof(nnlatitude));
						//strncpy(nnlatitude,jingdu1,8);
						test_latitude11 = atoi(jingdu1);
						test_latitude  = test_latitude11 / 10 ;
						
						//test_latitude  = 11385715 ;
						old_latitude = test_latitude ;
						//nlatitude = test_latitude ;
						
						weidu_change(weidu1);
						squeeze(weidu1,'.');
						//memset(nnlongitude, 0, sizeof(nnlongitude));
						//strncpy(nnlongitude,weidu1,7);
						test_longitude11 = atoi(weidu1);
						test_longitude  = test_longitude11 / 10 ;

						//test_longitude  = 2256911 ;
						old_longitude = test_latitude ;
						
						//NMEA_GPRMC_Analysis(jingdu,weidu);
						//nlatitude = nlatitude /10 ;
						//nlongitude = nlongitude / 10 ;
						kal_prompt_trace(MOD_TST, "nnlatitude = %d, nnlongitude =%d, simid=%s-g_lhat_imei=%s,back_count =%d,batt_percent=%d", test_latitude, test_longitude,simid,g_lhat_imei,back_count,g_phone_batt_percent);
						
						len = sprintf(gps_info_buffer,"POST /api/v1/car_manage/device HTTP/1.1\r\n");
						len += sprintf(gps_info_buffer + len,"Host: staging.hua1268.com\r\n");
						//len += sprintf(gps_info_buffer + len,"Host: hua1268.com\r\n");
						len += sprintf(gps_info_buffer + len,"Cache-Control: no-cache\r\n");
						len += sprintf(gps_info_buffer + len,"Content-Type: application/x-www-form-urlencoded\r\n");
						//len += sprintf(gps_info_buffer + len,"Content-Type: application-www-form-urlencoded\r\n");

						{
#ifdef __LHAT_WIDE_VOLTAGE_DEVICE__				
							len1 += sprintf(gps_info_buffer1,"Content-length: %d\r\n",((64) + (n_gps_count * 17)));	//πÃ∂®≥§∂»
#else
							len1 += sprintf(gps_info_buffer1,"Content-length: %d\r\n",((64)  ));  //πÃ∂®≥§∂»
#endif							
						}

						len1 += sprintf(gps_info_buffer1 + len1,"\r\ndata=%s,",g_lhat_imei);
						len1 += sprintf(gps_info_buffer1 + len1,"%s,",simid);
#ifdef __LHAT_WIDE_VOLTAGE_DEVICE__	
						for(i = 0 ; i < n_gps_count ; i++)
						{
							//len1 += sprintf(gps_info_buffer1 + len1,"%d,",back_gps_data[i][0]);
							//len1 += sprintf(gps_info_buffer1 + len1,"%d,",back_gps_data[i][1]);
							len1 += sprintf(gps_info_buffer1 + len1,"%d,",st_back_gps_data[i].st_latitude);
							len1 += sprintf(gps_info_buffer1 + len1,"%d,",st_back_gps_data[i].st_longitude);
							len1 += sprintf(gps_info_buffer1 + len1,"%s,",st_back_gps_data[i].st_speed);
							len1 += sprintf(gps_info_buffer1 + len1,"%s,",st_back_gps_data[i].st_degree);
						}
#endif						
						len1 += sprintf(gps_info_buffer1 + len1,"%d,",test_longitude);
						len1 += sprintf(gps_info_buffer1 + len1,"%d,",test_latitude);
						len1 += sprintf(gps_info_buffer1 + len1,"%d,",80000);
						len1 += sprintf(gps_info_buffer1 + len1,"22,0,0");	
						//len1 += sprintf(gps_info_buffer1 + len1,"22,%d,0",g_phone_batt_percent);	
						//kal_prompt_trace(MOD_TST, "dingtian:%s",gps_info_buffer1);
						len += sprintf(gps_info_buffer + len,"%s",gps_info_buffer1);	
						kal_prompt_trace(MOD_TST, "dingtian num=%d, len=%d, gps_info_buffer=%s", num, len, gps_info_buffer1);
						
					}
				}
				
				break;
			default:
				break ;
		}
		len = strlen(gps_info_buffer);
		ret = soc_send(g_socket_id, gps_info_buffer, len, 0);
		kal_prompt_trace(MOD_TST, "reply=%d, %d, %s", ret, len, gps_info_buffer);
		//kal_prompt_trace(MOD_TST, "%s",temp_buff);
		if(ret < 0)
		{
			g_heart_beat = 0;
			n_gprs_send_fail_count++ ;
			if (n_gprs_send_fail_count > 30)
			{
				n_gprs_send_fail_count = 0 ;
				g_start_to_gps_flag = 0 ;
			}
			//StopTimer(GIS_TIMER_ID_2);
			//StartTimer(GIS_TIMER_ID_2, 1000, gprs_heartbeat_time);
		}
		else if((1 == num || 2 == num)&& (g_lhat_gps.east_west || g_lhat_gps.north_south))
			memset(&g_lhat_gps, 0, sizeof(g_lhat_gps));

		if (ret >= 0)
		{
			g_heart_beat = 1 ;
		}
	}
}




kal_uint32 mmi_get_gsensor_old_data(void)
{
	return gsensor_old_data ;
}




#elif defined(__LHAT_HUAYUAN_PROTOCOL__) || defined(__LHAT_TREE_YEAR_COMMON__)
extern void mmi_lmc_get_iccid_req(void);
void gprs_send_buf_reply(kal_int8 num)
{
	srv_nw_info_service_availability_enum availablity = srv_nw_info_get_service_availability(MMI_SIM1);
	kal_uint32 device_product = 80000;
	kal_uint8 product_id = 22;
	kal_uint8 nn_status = 0;
	kal_uint8 nn_vbat = 80;
	kal_uint8 nn_interval = 0;
	kal_uint8 nn_version = 100;
	kal_uint8 nn_gps_gnss = 0;
	kal_uint32 nn_sos_mode = 0;//|= 1<<WARN_NOPOWER;
	kal_uint8 nn_break_power_status = 0;
	kal_uint8 nn_speed = 5,nn_deriction = 6;
	kal_uint32 nn_mileage = 0;
	kal_uint8 nn_reserve1 = 0,nn_reserve2 = 0,nn_reserve3 = 0,nn_reserve4 = 0,nn_reserve5 = 0,nn_reserve6 = 0,nn_reserve7 = 0,nn_reserve8 = 0;
	kal_uint16 nn_gps_count = 1;
	
	
	if(SRV_NW_INFO_SA_FULL_SERVICE == availablity)
	{
		kal_int32 ret=0;
		kal_int16 len = 0,i = 0;
		//int len1 = 0;
		char kph[20];
		char msl[20];
		char  gps_info_buffer[2000] = {0};
		char  gps_info_buffer1[2000] = {0};
		kal_int32 test_longitude = 0;//11385715 ;
		kal_int32 test_latitude  = 0;//2256911 ;
		int nn_distance = 0 ;
		kal_prompt_trace(MOD_TST, "g_lhat_gps.east_west=%d, g_lhat_gps.east_west=%d,", g_lhat_gps.east_west, g_lhat_gps.north_south);
		mmi_lmc_get_iccid_req();
		
		memset(gps_info_buffer, 0, sizeof(gps_info_buffer));
		switch(num)
		{
			case 1:
			case 2:	
				{
					if(g_lhat_gps.east_west || g_lhat_gps.north_south)						
					{
						char jingdu1[12] = {0}, weidu1[12] = {0};
						char tmp1[6] = {0},tmp2[6] = {0};
						float speed_gps = 0, degree_gps = 0;
						kal_int32 len1 = 0;
						char nnlatitude[8] = {0};
						char nnlongitude[7] = {0};
						kal_int32 test_longitude11 = 11385715 ;
						kal_int32 test_latitude11  = 2256911 ;
						
						char simid[13] = "1000000000000";
						//u32 nnlatitude = 0 , nnlongitude = 0 ;
						
						jingdu_change(jingdu1);
						squeeze(jingdu1,'.');
						test_latitude11 = atoi(jingdu1);
						test_latitude  = test_latitude11 / 10 ;

						weidu_change(weidu1);
						squeeze(weidu1,'.');
						test_longitude11 = atoi(weidu1);
						test_longitude  = test_longitude11 / 10 ;

#if defined(__LHAT_WIDE_VOLTAGE_DEVICE__) || defined(__LHAT_SUNLIGHT_ENERGY_CHARGE_IN_COMMON__)	
#if defined(__LHAT_SUNLIGHT_ENERGY_CHARGE_IN_COMMON__)	
						if(b_sunlight_charge_in_flag == KAL_TRUE)
#endif							
						if (b_send_gps_data_flag == KAL_FALSE)
						{
							if (n_gps_count < 150)
							{
								st_back_gps_data[n_gps_count].st_longitude= test_longitude ;
								st_back_gps_data[n_gps_count].st_latitude = test_latitude ;
								
								sprintf(tmp1, "%05.1f", g_lhat_gps.lhat_ground_speed);
								memcpy(st_back_gps_data[n_gps_count].st_speed,tmp1,sizeof(tmp1));
								kal_prompt_trace(MOD_TST, "st_back_gps_data.ground_speed =%s", st_back_gps_data[n_gps_count].st_speed);
								sprintf(tmp2, "%03d", (int)g_lhat_gps.lhat_trace_degree);
								memcpy(st_back_gps_data[n_gps_count].st_degree,tmp2,sizeof(tmp2));
								kal_prompt_trace(MOD_TST, "st_back_gps_data.ground_speed =%s", st_back_gps_data[n_gps_count].st_degree);
								n_gps_count++ ;
							}
							else
							{
								n_gps_count = 149 ;
								st_back_gps_data[n_gps_count].st_longitude= test_longitude ;
								st_back_gps_data[n_gps_count].st_latitude = test_latitude ;
								
								sprintf(tmp1, "%05.1f", g_lhat_gps.lhat_ground_speed);
								memcpy(st_back_gps_data[n_gps_count].st_speed,tmp1,sizeof(tmp1));
								kal_prompt_trace(MOD_TST, "st_back_gps_data.ground_speed =%s", st_back_gps_data[n_gps_count].st_speed);
								sprintf(tmp2, "%03d", (int)g_lhat_gps.lhat_trace_degree);
								memcpy(st_back_gps_data[n_gps_count].st_degree,tmp2,sizeof(tmp2));
								kal_prompt_trace(MOD_TST, "st_back_gps_data.ground_speed =%s", st_back_gps_data[n_gps_count].st_degree);
							}
							save_mileage += CalcDistance(save_longitude,save_latitude,test_longitude,test_latitude);
							save_longitude = test_longitude ;
							save_latitude = test_latitude ;
							b_send_gps_data_flag = KAL_TRUE ;
						}
#endif						

#if defined(__LHAT_LOSS_POINT_SAVE_TO_SEND__) || defined(__LHAT_SUNLIGHT_ENERGY_CHARGE_IN_COMMON__)
#if defined(__LHAT_SUNLIGHT_ENERGY_CHARGE_IN_COMMON__)	
						if(b_sunlight_charge_in_flag == KAL_FALSE)
#endif	
						if (b_send_gps_data_flag == KAL_FALSE)
						{
							if (loss_point_count < 150)
							{
								st_loss_point_gps[loss_point_count].st_longitude = test_longitude ;
								st_loss_point_gps[loss_point_count].st_latitude = test_latitude ;
								
								sprintf(tmp1, "%05.1f", g_lhat_gps.lhat_ground_speed);
								memcpy(st_loss_point_gps[loss_point_count].st_speed,tmp1,sizeof(tmp1));
								kal_prompt_trace(MOD_TST, "st_loss_point_gps->ground_speed =%s", st_loss_point_gps[loss_point_count].st_speed);
								sprintf(tmp2, "%03d", (int)g_lhat_gps.lhat_trace_degree);
								memcpy(st_loss_point_gps[loss_point_count].st_degree,tmp2,sizeof(tmp2));
								kal_prompt_trace(MOD_TST, "st_loss_point_gps->ground_speed =%s", st_loss_point_gps[loss_point_count].st_degree);
								loss_point_count++ ;
							}
							else
							{
								loss_point_count = 149 ;
								
								st_loss_point_gps[loss_point_count].st_longitude = test_longitude ;
								st_loss_point_gps[loss_point_count].st_latitude = test_latitude ;
								sprintf(tmp1, "%05.1f", g_lhat_gps.lhat_ground_speed);
								memcpy(st_loss_point_gps[loss_point_count].st_speed,tmp1,sizeof(tmp1)) ;
								sprintf(tmp2, "%03d", (int)g_lhat_gps.lhat_trace_degree);
								memcpy(st_loss_point_gps[loss_point_count].st_degree,tmp2,sizeof(tmp2));
							}
							save_mileage += CalcDistance(save_longitude,save_latitude,test_longitude,test_latitude);
							save_longitude = test_longitude ;
							save_latitude = test_latitude ;
							b_send_gps_data_flag = KAL_TRUE ;
						}
#endif

						//ground_speed_change(&speed_gps);
						//trace_degree_change(&degree_gps);
						kal_prompt_trace(MOD_TST, "speed_gps=%f,degree_gps=%f", g_lhat_gps.lhat_ground_speed,g_lhat_gps.lhat_trace_degree);

						if (old_latitude != 0)
						{
							nn_distance = CalcDistance(old_latitude,old_longitude,test_latitude,test_longitude);
						}
						//test_latitude  = 11385715 ;
						

						kal_prompt_trace(MOD_TST, "nn_distance=%d,save_mileage=%d", nn_distance,save_mileage);
						
						if (nn_distance <=50 && nn_distance >= 0 && old_latitude != 0)
						{
							nn_status = mmi_get_acc_status_value() ; 					// 0 = normal  1 = sos  2 = Acc on 3 = acc off  4= other
							nn_vbat = g_phone_batt_percent ;		// 9000+nn_vbat 
							nn_interval = getFre(lhat_device.fre);//n_interval ;				// 0-20, 1-30, 2-60, 3-90, 4-120, 5-180, 6-300, 7 -600, 8-3600, 9-24x3600
							nn_version = MTK2503_HUA_VERSION ;		// version 1.0.0
							nn_gps_gnss = n_gps_gnss ;				// 0-gps , 1-gnss, 2-gps+gnss, 3-lbs, 4-wifi
							nn_sos_mode = mmi_get_warning_status() ;				// 0-normal, 1-∂œµÁ±®æØ, 2-’Ò∂Ø±®æØ,3-µÁ◊”Œß¿∏±®æØ, 4-“∆∂Ø±®æØ, 5-≤–∂±®æØ, 6-≥¨ÀŸ±®æØ, 7-µÕ—π±®æØ
							nn_break_power_status = Get_s_f_value() ;			// 0-normal, 1-∂œ”Õ/∂œµÁ, 2-other 
							//nn_reserve1 = 0 ;
							//nn_reserve2 = 0 ;
							nn_reserve3 = g_cell_info.serv_info.gci.mcc ;
							nn_reserve4 = g_cell_info.serv_info.gci.mnc ;
							nn_reserve5 = g_cell_info.nbr_cell_info[0].gci.lac ;
							nn_reserve6 = g_cell_info.nbr_cell_info[0].gci.ci ;
							nn_reserve7 = 0 ;
							nn_reserve8 = 0 ;
							nn_mileage = save_mileage ;
#if defined(__LHAT_WIDE_VOLTAGE_DEVICE__) 						
							nn_gps_count = n_gps_count + 1;			// gps count
#elif defined(__LHAT_SUNLIGHT_ENERGY_CHARGE_IN_COMMON__)	
							if(b_sunlight_charge_in_flag == KAL_TRUE)
							{
								nn_gps_count = n_gps_count ;	
							}
							else
							{
								nn_gps_count = loss_point_count ;
							}
#elif defined(__LHAT_LOSS_POINT_SAVE_TO_SEND__)
							nn_gps_count = loss_point_count ;//+ 1;
#endif
						//}
							//get_Vbat_Voltage();
							
							kal_prompt_trace(MOD_TST, "nnlatitude =%d, nnlongitude=%d,simid=%s,g_lhat_imei=%s,batt_percent=%d,n_gps_count=%d,nn_distance=%d", test_latitude, test_longitude,iccid_buf,g_lhat_imei,g_phone_batt_percent,n_gps_count,nn_distance);

////////////////////////////////////////
							len1 += sprintf(gps_info_buffer1 + len1,"data=%s,",g_lhat_imei);
							
							len1 += sprintf(gps_info_buffer1 + len1,"%s,",simid);
							len1 += sprintf(gps_info_buffer1 + len1,"%d,",device_product);
							len1 += sprintf(gps_info_buffer1 + len1,"%d,",product_id);
							len1 += sprintf(gps_info_buffer1 + len1,"%d,",nn_status);
							len1 += sprintf(gps_info_buffer1 + len1,"%d,",9000+nn_vbat);
							len1 += sprintf(gps_info_buffer1 + len1,"%d,",nn_interval);
							len1 += sprintf(gps_info_buffer1 + len1,"%d,",nn_version);
							len1 += sprintf(gps_info_buffer1 + len1,"%d,",nn_gps_gnss);
							len1 += sprintf(gps_info_buffer1 + len1,"%08d,",90000000+nn_sos_mode);
							len1 += sprintf(gps_info_buffer1 + len1,"%d,",nn_break_power_status);
							//len1 += sprintf(gps_info_buffer1 + len1,"%d,",nn_reserve1);
							if (strlen(iccid_buf)== 20)
							{
								len1 += sprintf(gps_info_buffer1 + len1,"%s,",iccid_buf);
							}
							else
							{
								len1 += sprintf(gps_info_buffer1 + len1,"0,",iccid_buf);
							}
							//len1 += sprintf(gps_info_buffer1 + len1,"%d,",nn_reserve2);
							//len1 += sprintf(gps_info_buffer1 + len1,"%.1f,",nn_mileage/1000.0);
							len1 += sprintf(gps_info_buffer1 + len1,"%d,",nn_mileage);
							len1 += sprintf(gps_info_buffer1 + len1,"%d,",nn_reserve3);
							len1 += sprintf(gps_info_buffer1 + len1,"%d,",nn_reserve4);
							len1 += sprintf(gps_info_buffer1 + len1,"%d,",nn_reserve5);
							len1 += sprintf(gps_info_buffer1 + len1,"%d,",nn_reserve6);
							len1 += sprintf(gps_info_buffer1 + len1,"%d,",nn_reserve7);
							len1 += sprintf(gps_info_buffer1 + len1,"%d,",nn_reserve8);
							len1 += sprintf(gps_info_buffer1 + len1,"%03d,",nn_gps_count);

#if defined(__LHAT_WIDE_VOLTAGE_DEVICE__) || defined(__LHAT_SUNLIGHT_ENERGY_CHARGE_IN_COMMON__)						
							//for(i = 0 ; i < (nn_gps_count - 1) ; i++)
#if defined(__LHAT_SUNLIGHT_ENERGY_CHARGE_IN_COMMON__)	
							if(b_sunlight_charge_in_flag == KAL_TRUE)
#endif									
							for(i = 0 ; i < (nn_gps_count) ; i++)
							{
								
								//len1 += sprintf(gps_info_buffer1 + len1,"%d,",back_gps_data[i][1]);
								//len1 += sprintf(gps_info_buffer1 + len1,"%d,",back_gps_data[i][0]);
								len1 += sprintf(gps_info_buffer1 + len1,"%d,",st_back_gps_data[i].st_latitude);
								len1 += sprintf(gps_info_buffer1 + len1,"%d,",st_back_gps_data[i].st_longitude);
								len1 += sprintf(gps_info_buffer1 + len1,"%s,",st_back_gps_data[i].st_speed);
								len1 += sprintf(gps_info_buffer1 + len1,"%s,",st_back_gps_data[i].st_degree);
								kal_prompt_trace(MOD_TST, "st_back_gps_data[%d]=%d, %d ,%s,%s", i, st_back_gps_data[i].st_latitude, st_back_gps_data[i].st_longitude,st_back_gps_data[i].st_speed,st_back_gps_data[i].st_degree);
							}
#endif						

#if defined(__LHAT_LOSS_POINT_SAVE_TO_SEND__) || defined(__LHAT_SUNLIGHT_ENERGY_CHARGE_IN_COMMON__)
#if defined(__LHAT_SUNLIGHT_ENERGY_CHARGE_IN_COMMON__)	
							if(b_sunlight_charge_in_flag == KAL_FALSE)
#endif		

							//for(i = 0 ; i < (nn_gps_count -1) ; i++)
							for(i = 0 ; i < (nn_gps_count) ; i++)
							{
								
								len1 += sprintf(gps_info_buffer1 + len1,"%d,",st_loss_point_gps[i].st_latitude);
								len1 += sprintf(gps_info_buffer1 + len1,"%d,",st_loss_point_gps[i].st_longitude);
								len1 += sprintf(gps_info_buffer1 + len1,"%s,",st_loss_point_gps[i].st_speed);
								len1 += sprintf(gps_info_buffer1 + len1,"%s,",st_loss_point_gps[i].st_degree);
								//len1 += sprintf(gps_info_buffer1 + len1,"%03d,",loss_mileage_array[i]);
								kal_prompt_trace(MOD_TST, "loss_point_gps_data[%d]=%d, %d,%s,%s", i, st_loss_point_gps[i].st_latitude, st_loss_point_gps[i].st_longitude,st_loss_point_gps[i].st_speed,st_loss_point_gps[i].st_degree);
							}
#endif
							
							//len1 += sprintf(gps_info_buffer1 + len1,"%d,",test_latitude);
							//len1 += sprintf(gps_info_buffer1 + len1,"%d,",test_longitude);
							//sprintf(tmp1, "%05.1f", g_lhat_gps.lhat_ground_speed);
							//len1 += sprintf(gps_info_buffer1 + len1,"%s,",tmp1);
							//sprintf(tmp2, "%03d", (int)g_lhat_gps.lhat_trace_degree);
							//len1 += sprintf(gps_info_buffer1 + len1,"%s",tmp2);
							//len1 += sprintf(gps_info_buffer1 + len1,"%03d",nn_mileage);
							kal_prompt_trace(MOD_TST, "test_latitude=%d,test_longitude=%d, ,%s,%s,len1=%d",  test_latitude,test_longitude,tmp1,tmp2,len1);

////////////////////////////////////////
							
							len = sprintf(gps_info_buffer,"POST /api/v1/device HTTP/1.1\r\n");
							//len += sprintf(gps_info_buffer + len,"Host: staging.hua1268.com\r\n");
							//len += sprintf(gps_info_buffer + len,"Host: test.hua1268.com\r\n");
							//len += sprintf(gps_info_buffer + len,"Host: hua1268.com\r\n");
							len += sprintf(gps_info_buffer + len,"Host: gps.hua1268.com\r\n");
							len += sprintf(gps_info_buffer + len,"Cache-Control: no-cache\r\n");
							len += sprintf(gps_info_buffer + len,"Content-Type: application/x-www-form-urlencoded\r\n");
/*
#if defined(__LHAT_WIDE_VOLTAGE_DEVICE__)
							//len1 += sprintf(gps_info_buffer1,"Content-length: %d\r\n",((101) + (n_gps_count* 17)));	
							if ((strlen(lhat_device.num)== 13) || (strlen(lhat_device.num)== 11) || (strlen(iccid_buf)== 20))
							{
								len1 += sprintf(gps_info_buffer1,"Content-length: %d\r\n",((117+strlen(iccid_buf)) + (n_gps_count* 29)));
							}
							else
							{
								//len1 += sprintf(gps_info_buffer1,"Content-length: %d\r\n",((107) + (n_gps_count* 29)));
								len1 += sprintf(gps_info_buffer1,"Content-length: %d\r\n",((117+strlen(iccid_buf)) + (n_gps_count* 29)));
							}
#elif defined(__LHAT_LOSS_POINT_SAVE_TO_SEND__)
							//len1 += sprintf(gps_info_buffer1,"Content-length: %d\r\n",((101) + (loss_point_count* 17)));	
							if ( (strlen(iccid_buf)== 20))
							{
								len1 += sprintf(gps_info_buffer1,"Content-length: %d\r\n",((117+strlen(iccid_buf)) + (loss_point_count* 29)));
							}
							else
							{
								//len1 += sprintf(gps_info_buffer1,"Content-length: %d\r\n",((107) + (loss_point_count* 29)));
								len1 += sprintf(gps_info_buffer1,"Content-length: %d\r\n",((117+strlen(iccid_buf)) + (n_gps_count* 29)));
							}
#else
							{
								len1 += sprintf(gps_info_buffer1,"Content-length: %d\r\n",((64)  ));  //πÃ∂®≥§∂»
							}
#endif
*/
							len += sprintf(gps_info_buffer  + len,"Content-length: %d\r\n\r\n",(len1));

/*
							len1 += sprintf(gps_info_buffer1 + len1,"\r\ndata=%s,",g_lhat_imei);
							
							len1 += sprintf(gps_info_buffer1 + len1,"%s,",simid);
							len1 += sprintf(gps_info_buffer1 + len1,"%d,",device_product);
							len1 += sprintf(gps_info_buffer1 + len1,"%d,",product_id);
							len1 += sprintf(gps_info_buffer1 + len1,"%d,",nn_status);
							len1 += sprintf(gps_info_buffer1 + len1,"%d,",9000+nn_vbat);
							len1 += sprintf(gps_info_buffer1 + len1,"%d,",nn_interval);
							len1 += sprintf(gps_info_buffer1 + len1,"%d,",nn_version);
							len1 += sprintf(gps_info_buffer1 + len1,"%d,",nn_gps_gnss);
							len1 += sprintf(gps_info_buffer1 + len1,"%08d,",nn_sos_mode);
							len1 += sprintf(gps_info_buffer1 + len1,"%d,",nn_break_power_status);
							//len1 += sprintf(gps_info_buffer1 + len1,"%d,",nn_reserve1);
							if (strlen(iccid_buf)== 20)
							{
								len1 += sprintf(gps_info_buffer1 + len1,"%s,",iccid_buf);
							}
							else
							{
								len1 += sprintf(gps_info_buffer1 + len1,"0,",iccid_buf);
							}
							len1 += sprintf(gps_info_buffer1 + len1,"%d,",nn_reserve2);
							len1 += sprintf(gps_info_buffer1 + len1,"%d,",nn_reserve3);
							len1 += sprintf(gps_info_buffer1 + len1,"%d,",nn_reserve4);
							len1 += sprintf(gps_info_buffer1 + len1,"%d,",nn_reserve5);
							len1 += sprintf(gps_info_buffer1 + len1,"%d,",nn_reserve6);
							len1 += sprintf(gps_info_buffer1 + len1,"%d,",nn_reserve7);
							len1 += sprintf(gps_info_buffer1 + len1,"%d,",nn_reserve8);
							len1 += sprintf(gps_info_buffer1 + len1,"%03d,",nn_gps_count);

#if defined(__LHAT_WIDE_VOLTAGE_DEVICE__)						
							for(i = 0 ; i < (nn_gps_count - 1) ; i++)
							{
								
								len1 += sprintf(gps_info_buffer1 + len1,"%d,",back_gps_data[i][1]);
								len1 += sprintf(gps_info_buffer1 + len1,"%d,",back_gps_data[i][0]);
								kal_prompt_trace(MOD_TST, "back_gps_data[%d]=%d, %d", i, back_gps_data[i][0], back_gps_data[i][1]);
							}
#endif						

#if defined(__LHAT_LOSS_POINT_SAVE_TO_SEND__)
							for(i = 0 ; i < (nn_gps_count -1) ; i++)
							{
								
								len1 += sprintf(gps_info_buffer1 + len1,"%d,",st_loss_point_gps[i].st_latitude);
								len1 += sprintf(gps_info_buffer1 + len1,"%d,",st_loss_point_gps[i].st_longitude);
								len1 += sprintf(gps_info_buffer1 + len1,"%s,",st_loss_point_gps[i].st_speed);
								len1 += sprintf(gps_info_buffer1 + len1,"%s,",st_loss_point_gps[i].st_degree);
								len1 += sprintf(gps_info_buffer1 + len1,"%03d,",loss_mileage_array[i]);
								kal_prompt_trace(MOD_TST, "loss_point_gps_data[%d]=%d, %d,%s,%s,%03d", i, st_loss_point_gps[i].st_latitude, st_loss_point_gps[i].st_longitude,st_loss_point_gps[i].st_speed,st_loss_point_gps[i].st_degree,loss_mileage_array[i]);
							}
#endif
							
							len1 += sprintf(gps_info_buffer1 + len1,"%d,",test_latitude);
							len1 += sprintf(gps_info_buffer1 + len1,"%d,",test_longitude);
							sprintf(tmp, "%04.1f", g_lhat_gps.lhat_ground_speed);
							len1 += sprintf(gps_info_buffer1 + len1,"%s,",tmp);
							sprintf(tmp, "%03d", (int)g_lhat_gps.lhat_trace_degree);
							len1 += sprintf(gps_info_buffer1 + len1,"%s,",tmp);
							len1 += sprintf(gps_info_buffer1 + len1,"%03d",nn_mileage);
							kal_prompt_trace(MOD_TST, "test_longitude=%d, test_latitude=%d,data_len =%d,%03d,%03d,%03d",  test_longitude, test_latitude,(107 + (loss_point_count* 29)),nn_speed,nn_deriction,nn_mileage);
*/
							len += sprintf(gps_info_buffer + len,"%s",gps_info_buffer1);	
							
							kal_prompt_trace(MOD_TST, "len=%d, %s", len, gps_info_buffer1);
						}
						old_latitude = test_latitude ;
						//test_longitude  = 2256911 ;
						old_longitude = test_longitude ;
					}
				}
				break;
			
		}
		len = strlen(gps_info_buffer);
		ret = soc_send(g_socket_id, gps_info_buffer, len, 0);
		kal_prompt_trace(MOD_TST, "reply=%d, %d, %s", ret, len, gps_info_buffer);
		if(ret < 0)
		{
			g_heart_beat = 0;
			n_gprs_send_fail_count++ ;
			if (n_gprs_send_fail_count > 5)
			{
				n_gprs_send_fail_count = 0 ;
				g_start_to_gps_flag = 0 ;
			}
			//StopTimer(GIS_TIMER_ID_2);
			//StartTimer(GIS_TIMER_ID_2, 1000, gprs_heartbeat_time);
		}
		else if((1 == num || 2 == num)&& (g_lhat_gps.east_west || g_lhat_gps.north_south))
			memset(&g_lhat_gps, 0, sizeof(g_lhat_gps));

		if (ret >= 0)
		{
			g_heart_beat = 1 ;
			g_start_to_gps_flag = 2 ;
		}
	}
}
#elif defined(__LHAT_SHANGXUN_COMMON__)
extern void mmi_lmc_get_iccid_req(void);

void gprs_send_buf_reply(kal_int8 num)
{
	srv_nw_info_service_availability_enum availablity = srv_nw_info_get_service_availability(MMI_SIM1);
	kal_uint32 device_product = 80006;
	kal_uint8 product_id = 01;
	kal_uint8 nn_status = 0;
	kal_uint8 nn_vbat = 80;
	kal_uint8 nn_interval = 0;
	kal_uint8 nn_version = 101;
	kal_uint8 nn_gps_gnss = 0;
	kal_uint32 nn_sos_mode = 0;//|= 1<<WARN_NOPOWER;
	kal_uint8 nn_break_power_status = 0;
	kal_uint8 nn_speed = 5,nn_deriction = 6;
	kal_uint32 nn_mileage = 0;
	kal_uint8 nn_reserve1 = 0,nn_reserve2 = 0,nn_reserve3 = 0,nn_reserve4 = 0,nn_reserve5 = 0,nn_reserve6 = 0,nn_reserve7 = 0,nn_reserve8 = 0;
	kal_bool nn_lock_status = KAL_FALSE ;
	kal_uint16 nn_gps_count = 1;
	
	//869013021002111,80006,01,0,080,2,100,2,0,12345678901234567890,0,0,0,0,0,0,0,001,11384456,2259653#
	
	if(SRV_NW_INFO_SA_FULL_SERVICE == availablity)
	{
		kal_int32 ret=0;
		kal_int16 len = 0,i = 0;
		//int len1 = 0;
		char kph[20];
		char msl[20];
		//char  gps_info_buffer[2000] = {0};
		char  gps_info_buffer1[2000] = {0};
		kal_int32 test_longitude = 0;//11385715 ;
		kal_int32 test_latitude  = 0;//2256911 ;
		int nn_distance = 0 ;
		kal_prompt_trace(MOD_TST, "g_lhat_gps.east_west=%d, g_lhat_gps.east_west=%d,", g_lhat_gps.east_west, g_lhat_gps.north_south);
		mmi_lmc_get_iccid_req();
		
		//memset(gps_info_buffer, 0, sizeof(gps_info_buffer));
		memset(gps_info_buffer1, 0, sizeof(gps_info_buffer1));
		switch(num)
		{
			case 1:
			case 2:	
				{
					//if(g_lhat_gps.east_west || g_lhat_gps.north_south)						
					{
						char jingdu1[11] = {0}, weidu1[11] = {0};
						char tmp1[6] = {0},tmp2[6] = {0};
						float speed_gps = 0, degree_gps = 0;
						kal_int32 len1 = 0;
						unsigned int voltage = 0;
						char nnlatitude[8] = {0};
						char nnlongitude[7] = {0};
						kal_int32 test_longitude11 = 11385715 ;
						kal_int32 test_latitude11  = 2256911 ;
												
						jingdu_change(jingdu1);
						//squeeze(jingdu1,'.');
						//test_latitude11 = atoi(jingdu1);
						//test_latitude  = test_latitude11 / 10 ;

						weidu_change(weidu1);
						//squeeze(weidu1,'.');
						//test_longitude11 = atoi(weidu1);
						//test_longitude  = test_longitude11 / 10 ;

						if ((b_send_gps_data_flag == KAL_FALSE)&&(g_lhat_gps.east_west || g_lhat_gps.north_south))
						{
							if (n_gps_count < 150)
							{
								
								//st_shangxun_point_gps[n_gps_count].st_longitude= test_longitude ;
								//st_shangxun_point_gps[n_gps_count].st_latitude = test_latitude ;
								memcpy(st_shangxun_point_gps[n_gps_count].st_longitude,weidu1,sizeof(weidu1));
								memcpy(st_shangxun_point_gps[n_gps_count].st_latitude,jingdu1,sizeof(jingdu1));
								
								//sprintf(tmp1, "%05.1f", g_lhat_gps.lhat_ground_speed);
								//memcpy(st_shangxun_point_gps[n_gps_count].st_speed,tmp1,sizeof(tmp1));
								//kal_prompt_trace(MOD_TST, "st_shangxun_point_gps.ground_speed =%s", st_shangxun_point_gps[n_gps_count].st_speed);
								//sprintf(tmp2, "%03d", (int)g_lhat_gps.lhat_trace_degree);
								//memcpy(st_shangxun_point_gps[n_gps_count].st_degree,tmp2,sizeof(tmp2));
								//kal_prompt_trace(MOD_TST, "st_shangxun_point_gps.ground_speed =%s", st_shangxun_point_gps[n_gps_count].st_degree);
								n_gps_count++ ;
							}
							else
							{
								n_gps_count = 149 ;
								//back_gps_data[n_gps_count][0] = test_longitude ;
								//back_gps_data[n_gps_count][1] = test_latitude ;
								
								//st_shangxun_point_gps[n_gps_count].st_longitude= test_longitude ;
								//st_shangxun_point_gps[n_gps_count].st_latitude = test_latitude ;
								memcpy(st_shangxun_point_gps[n_gps_count].st_longitude,weidu1,sizeof(weidu1));
								memcpy(st_shangxun_point_gps[n_gps_count].st_latitude,jingdu1,sizeof(jingdu1));
								
								//sprintf(tmp1, "%05.1f", g_lhat_gps.lhat_ground_speed);
								//memcpy(st_shangxun_point_gps[n_gps_count].st_speed,tmp1,sizeof(tmp1));
								//kal_prompt_trace(MOD_TST, "st_shangxun_point_gps.ground_speed =%s", st_shangxun_point_gps[n_gps_count].st_speed);
								//sprintf(tmp2, "%03d", (int)g_lhat_gps.lhat_trace_degree);
								//memcpy(st_shangxun_point_gps[n_gps_count].st_degree,tmp2,sizeof(tmp2));
								//kal_prompt_trace(MOD_TST, "st_shangxun_point_gps.ground_speed =%s", st_shangxun_point_gps[n_gps_count].st_degree);
							}
							if (strlen(weidu1) != 0)
							{
								memcpy(back_gps_longitude,weidu1,sizeof(weidu1));
								memcpy(back_gps_latitude,jingdu1,sizeof(jingdu1));
							}
							
							b_send_gps_data_flag = KAL_TRUE ;
						}
						
						
						//if (nn_distance <=50 && nn_distance >= 0 && old_latitude != 0)
						{
							nn_status = mmi_get_gps_updata_status() ; 					// 0 = normal  1 = sos  2 = Acc on 3 = acc off  4= other
							nn_vbat = g_phone_batt_percent ;		// 9000+nn_vbat 
							nn_interval = getFre(lhat_device.fre);//n_interval ;				// 0-20, 1-30, 2-60, 3-90, 4-120, 5-180, 6-300, 7 -600, 8-3600, 9-24x3600
							nn_version = MTK2503_HUA_VERSION ;		// version 1.0.0
							nn_gps_gnss = n_gps_gnss ;				// 0-gps , 1-gnss, 2-gps+gnss, 3-lbs, 4-wifi
							//nn_sos_mode = mmi_get_warning_status() ;				// 0-normal, 1-∂œµÁ±®æØ, 2-’Ò∂Ø±®æØ,3-µÁ◊”Œß¿∏±®æØ, 4-“∆∂Ø±®æØ, 5-≤–∂±®æØ, 6-≥¨ÀŸ±®æØ, 7-µÕ—π±®æØ
							nn_break_power_status = mmi_get_bike_lock_unlock_status() ;			// 0-normal, 1-∂œ”Õ/∂œµÁ, 2-other 
							if(KAL_TRUE == mmi_get_eint2_status())
							{
								nn_reserve1 = 1 ;
							}
							else if (KAL_TRUE == mmi_get_bike_lock_status())
							{
								nn_reserve1 = 2 ;
							}
							else
							{
								nn_reserve1 = 0 ;
							}
							
							voltage = Current_Voltage_ex(DCL_AUX_ADC_CHANNEL);
							nn_reserve2 = lhat_get_vcharger_in_status() ;
							nn_reserve3 = 0 ;
							nn_reserve4 = 0 ;
							nn_reserve5 = get_vbat_voltage_percent_ext(voltage) ;
							nn_reserve6 = 0 ;
							nn_mileage = save_latitude ;
							nn_lock_status = mmi_get_eint2_status();
#if defined(__LHAT_WIDE_VOLTAGE_DEVICE__) ||  defined(__LHAT_SHANGXUN_COMMON__)							
							nn_gps_count = n_gps_count ;//+ 1;			// gps count
#elif defined(__LHAT_LOSS_POINT_SAVE_TO_SEND__)
							nn_gps_count = loss_point_count ;//+ 1;
#endif
							
							kal_prompt_trace(MOD_TST, "nnlatitude =%d, nnlongitude=%d,iccid_buf=%s,g_lhat_imei=%s,batt_percent=%d,n_gps_count=%d,nn_distance=%d", test_latitude, test_longitude,iccid_buf,g_lhat_imei,g_phone_batt_percent,n_gps_count,nn_distance);

////////////////////////////////////////
							//len1 += sprintf(gps_info_buffer1 + len1,"data=%s,",g_lhat_imei);
							len1 += sprintf(gps_info_buffer1 + len1,"%s,",g_lhat_imei);
							len1 += sprintf(gps_info_buffer1 + len1,"%d,",device_product);
							len1 += sprintf(gps_info_buffer1 + len1,"%02d,",product_id);
							len1 += sprintf(gps_info_buffer1 + len1,"%d,",nn_status);
							len1 += sprintf(gps_info_buffer1 + len1,"%03d,",nn_vbat);
							len1 += sprintf(gps_info_buffer1 + len1,"%d,",nn_interval);
							len1 += sprintf(gps_info_buffer1 + len1,"%d,",nn_version);
							len1 += sprintf(gps_info_buffer1 + len1,"%d,",nn_gps_gnss);
							len1 += sprintf(gps_info_buffer1 + len1,"%d,",nn_break_power_status);
							if (strlen(iccid_buf)== 20)
							{
								len1 += sprintf(gps_info_buffer1 + len1,"%s,",iccid_buf);
							}
							else
							{
								len1 += sprintf(gps_info_buffer1 + len1,"0,",iccid_buf);
							}
							len1 += sprintf(gps_info_buffer1 + len1,"%d,",nn_lock_status);
							len1 += sprintf(gps_info_buffer1 + len1,"%d,",nn_reserve1);
							len1 += sprintf(gps_info_buffer1 + len1,"%d,",nn_reserve2);
							len1 += sprintf(gps_info_buffer1 + len1,"%d,",nn_reserve3);
							len1 += sprintf(gps_info_buffer1 + len1,"%d,",nn_reserve4);
							len1 += sprintf(gps_info_buffer1 + len1,"%03d,",nn_reserve5);
							len1 += sprintf(gps_info_buffer1 + len1,"%d,",nn_reserve6);
							//nn_gps_count  = 1 ;
							//len1 += sprintf(gps_info_buffer1 + len1,"%03d",nn_gps_count);

#if defined(__LHAT_WIDE_VOLTAGE_DEVICE__) || defined(__LHAT_SHANGXUN_COMMON__)			
							if ((strlen(weidu1) != 0) && (nn_gps_count >= 1))
							{
								len1 += sprintf(gps_info_buffer1 + len1,"%03d",nn_gps_count);
								for(i = 0 ; i < (nn_gps_count) ; i++)
								{
									len1 += sprintf(gps_info_buffer1 + len1,",%s,",st_shangxun_point_gps[i].st_latitude);
									len1 += sprintf(gps_info_buffer1 + len1,"%s",st_shangxun_point_gps[i].st_longitude);
									kal_prompt_trace(MOD_TST, "st_shangxun_point_gps[%d]=%s, %s ", i, st_shangxun_point_gps[i].st_latitude, st_shangxun_point_gps[i].st_longitude);
								}
							}
							else
							{
								if (strlen(back_gps_latitude) != 0)
								{
									nn_gps_count = 1 ;
									len1 += sprintf(gps_info_buffer1 + len1,"%03d",nn_gps_count);
									len1 += sprintf(gps_info_buffer1 + len1,",%s,",back_gps_latitude);
									len1 += sprintf(gps_info_buffer1 + len1,"%s",back_gps_longitude);
									kal_prompt_trace(MOD_TST, "st_shangxun_point_gps[]=%s, %s ", back_gps_latitude, back_gps_longitude);
								}
								else
								{
									nn_gps_count = 0 ;
									len1 += sprintf(gps_info_buffer1 + len1,"%03d",nn_gps_count);
									//len1 += sprintf(gps_info_buffer1 + len1,",%s,",back_gps_latitude);
									//len1 += sprintf(gps_info_buffer1 + len1,"%s",back_gps_longitude);
									kal_prompt_trace(MOD_TST, "st_shangxun_point_gps[]=%s, %s ", back_gps_latitude, back_gps_longitude);
								}
							}
							//len1 += sprintf(gps_info_buffer1 + len1,"%d,",test_latitude);
							//len1 += sprintf(gps_info_buffer1 + len1,"%d",test_longitude);
							
#endif						

							len1 += sprintf(gps_info_buffer1 + len1,"#",nn_gps_count);

							kal_prompt_trace(MOD_TST, "test_latitude=%d,test_longitude=%d, ,%s,%s,len1=%d",  test_latitude,test_longitude,tmp1,tmp2,len1);
							
							kal_prompt_trace(MOD_TST, "len=%d, %s", len1, gps_info_buffer1);
						}
						old_latitude = test_latitude ;
						old_longitude = test_longitude ;
					}
				}
				break;
			
		}
		len = strlen(gps_info_buffer1);
		ret = soc_send(g_socket_id, gps_info_buffer1, len, 0);
		kal_prompt_trace(MOD_TST, "reply=%d, %d, %s", ret, len, gps_info_buffer1);
		if(ret < 0)
		{
			g_heart_beat = 0;
			n_gprs_send_fail_count++ ;
			if (n_gprs_send_fail_count > 10)
			{
				n_gprs_send_fail_count = 0 ;
				g_start_to_gps_flag = 0 ;
			}
		}
		else if((1 == num || 2 == num)&& (g_lhat_gps.east_west || g_lhat_gps.north_south))
			memset(&g_lhat_gps, 0, sizeof(g_lhat_gps));

		if (ret >= 0)
		{
			g_heart_beat = 1 ;
			//g_start_to_gps_flag = 2 ;
			mmi_gprs_socket_close_handle();
			//g_heart_beat = 1 ;
			//g_start_to_gps_flag = 2 ;
		}
	}
}

#elif defined(__LHAT_GUANGXIN_COMMON__)
extern void mmi_lmc_get_iccid_req(void);
extern kal_uint32 get_Vbat_Voltage(void);

//char *encrypt_key = "400A442135D1F2723A7E841AEC92A02D";
char encrypt_key[80] ={0};
char *encrypt_key_base = "12345678901234567890123456789012345678901234567890123456789012345678";


void gprs_send_buf_reply(kal_int8 num)
{
	srv_nw_info_service_availability_enum availablity = srv_nw_info_get_service_availability(MMI_SIM1);
	kal_uint32 device_product = 80008;
	kal_uint8 product_id = 01;
	kal_uint8 nn_status = 0;
	kal_uint8 nn_vbat = 80;
	kal_uint8 nn_interval = 0;
	kal_uint8 nn_version = 101;
	kal_uint8 nn_gps_gnss = 0;
	kal_uint32 nn_sos_mode = 0;//|= 1<<WARN_NOPOWER;
	kal_uint8 nn_break_power_status = 0;
	kal_uint8 nn_speed = 5,nn_deriction = 6;
	kal_uint32 nn_mileage = 0;
	kal_uint8 nn_reserve1 = 0,nn_reserve2 = 0,nn_reserve3 = 0,nn_reserve4 = 0,nn_reserve5 = 0,nn_reserve6 = 0,nn_reserve7 = 0,nn_reserve8 = 0;
	kal_bool nn_lock_status = KAL_FALSE ;
	kal_uint16 nn_gps_count = 1;
	
	//869013021002111,80006,01,0,080,2,100,2,0,12345678901234567890,0,0,0,0,0,0,0,001,11384456,2259653#
	
	if(SRV_NW_INFO_SA_FULL_SERVICE == availablity)
	{
		kal_int32 ret=0;
		kal_int16 len = 0,i = 0;
		//int len1 = 0;
		char kph[20];
		char msl[20];
		//char  gps_info_buffer[2000] = {0};
		char  gps_info_buffer1[1000] = {0};
		kal_int32 test_longitude = 0;//11385715 ;
		kal_int32 test_latitude  = 0;//2256911 ;
		int nn_distance = 0 ;
		kal_prompt_trace(MOD_TST, "g_lhat_gps.east_west=%d, g_lhat_gps.east_west=%d,", g_lhat_gps.east_west, g_lhat_gps.north_south);
		mmi_lmc_get_iccid_req();
		
		//memset(gps_info_buffer, 0, sizeof(gps_info_buffer));
		memset(gps_info_buffer1, 0, sizeof(gps_info_buffer1));
		//test_code();
		//lhat_websocket_main();
		switch(num)
		{
			case 1:
			case 2:	
				{
					//if(g_lhat_gps.east_west || g_lhat_gps.north_south)						
					{
						char jingdu1[11] = {0}, weidu1[11] = {0};
						char tmp1[6] = {0},tmp2[6] = {0};
						float speed_gps = 0, degree_gps = 0;
						kal_int32 len1 = 0;
						unsigned int voltage = 0;
						char nnlatitude[8] = {0};
						char nnlongitude[7] = {0};
						kal_int32 test_longitude11 = 11385715 ;
						kal_int32 test_latitude11  = 2256911 ;
												
						jingdu_change(jingdu1);
						//squeeze(jingdu1,'.');
						//test_latitude11 = atoi(jingdu1);
						//test_latitude  = test_latitude11 / 10 ;

						weidu_change(weidu1);
						//squeeze(weidu1,'.');
						//test_longitude11 = atoi(weidu1);
						//test_longitude  = test_longitude11 / 10 ;

						if ((b_send_gps_data_flag == KAL_FALSE)&&(g_lhat_gps.east_west || g_lhat_gps.north_south))
						{
							if (n_gps_count < 150)
							{
								
								memcpy(st_shangxun_point_gps[n_gps_count].st_longitude,weidu1,sizeof(weidu1));
								memcpy(st_shangxun_point_gps[n_gps_count].st_latitude,jingdu1,sizeof(jingdu1));
								
								n_gps_count++ ;
							}
							else
							{
								n_gps_count = 149 ;
								memcpy(st_shangxun_point_gps[n_gps_count].st_longitude,weidu1,sizeof(weidu1));
								memcpy(st_shangxun_point_gps[n_gps_count].st_latitude,jingdu1,sizeof(jingdu1));
							}
							if (strlen(weidu1) != 0)
							{
								memcpy(back_gps_longitude,weidu1,sizeof(weidu1));
								memcpy(back_gps_latitude,jingdu1,sizeof(jingdu1));
							}
							
							b_send_gps_data_flag = KAL_TRUE ;
						}
						
						
						//if (nn_distance <=50 && nn_distance >= 0 && old_latitude != 0)
						{
							nn_status = mmi_get_gps_updata_status() ; 					// 0 = normal  1 = sos  2 = Acc on 3 = acc off  4= other
							nn_vbat = g_phone_batt_percent ;		// 9000+nn_vbat 
							nn_interval = getFre(lhat_device.fre);//n_interval ;				// 0-20, 1-30, 2-60, 3-90, 4-120, 5-180, 6-300, 7 -600, 8-3600, 9-24x3600
							nn_version = MTK2503_HUA_VERSION ;		// version 1.0.0
							nn_gps_gnss = n_gps_gnss ;				// 0-gps , 1-gnss, 2-gps+gnss, 3-lbs, 4-wifi
							//nn_sos_mode = mmi_get_warning_status() ;				// 0-normal, 1-∂œµÁ±®æØ, 2-’Ò∂Ø±®æØ,3-µÁ◊”Œß¿∏±®æØ, 4-“∆∂Ø±®æØ, 5-≤–∂±®æØ, 6-≥¨ÀŸ±®æØ, 7-µÕ—π±®æØ
							nn_break_power_status = mmi_get_bike_lock_unlock_status() ;			// 0-normal, 1-∂œ”Õ/∂œµÁ, 2-other 
							if(KAL_TRUE == mmi_get_eint2_status())
							{
								nn_reserve1 = 1 ;
							}
							else if (KAL_TRUE == mmi_get_bike_lock_status())
							{
								nn_reserve1 = 2 ;
							}
							else
							{
								nn_reserve1 = 0 ;
							}
							
							voltage = Current_Voltage_ex(DCL_AUX_ADC_CHANNEL);
							nn_reserve2 = lhat_get_vcharger_in_status() ;
							nn_reserve3 = 0 ;
							nn_reserve4 = 0 ;
							nn_reserve5 = 0;//get_vbat_voltage_percent_ext(voltage) ;
							nn_reserve6 = 0 ;
							nn_mileage = save_latitude ;
							nn_lock_status = 0;//mmi_get_eint2_status();
							nn_gps_count = n_gps_count ;//+ 1;			// gps cou
							
							kal_prompt_trace(MOD_TST, "nnlatitude =%d, nnlongitude=%d,iccid_buf=%s,g_lhat_imei=%s,batt_percent=%d,n_gps_count=%d,nn_distance=%d", test_latitude, test_longitude,iccid_buf,g_lhat_imei,g_phone_batt_percent,n_gps_count,nn_distance);

////////////////////////////////////////
							//len1 += sprintf(gps_info_buffer1 + len1,"data=%s,",g_lhat_imei);
							len1 += sprintf(gps_info_buffer1 + len1,"%s,",g_lhat_imei);
							len1 += sprintf(gps_info_buffer1 + len1,"%d,",device_product);
							len1 += sprintf(gps_info_buffer1 + len1,"%02d,",product_id);
							len1 += sprintf(gps_info_buffer1 + len1,"%d,",nn_status);
							len1 += sprintf(gps_info_buffer1 + len1,"%03d,",nn_vbat);
							len1 += sprintf(gps_info_buffer1 + len1,"%d,",nn_interval);
							len1 += sprintf(gps_info_buffer1 + len1,"%d,",nn_version);
							len1 += sprintf(gps_info_buffer1 + len1,"%d,",nn_gps_gnss);
							len1 += sprintf(gps_info_buffer1 + len1,"%d,",nn_break_power_status);
							if (strlen(iccid_buf)== 20)
							{
								len1 += sprintf(gps_info_buffer1 + len1,"%s,",iccid_buf);
							}
							else
							{
								len1 += sprintf(gps_info_buffer1 + len1,"0,",iccid_buf);
							}
							len1 += sprintf(gps_info_buffer1 + len1,"%d,",nn_lock_status);
							len1 += sprintf(gps_info_buffer1 + len1,"%d,",nn_reserve1);
							len1 += sprintf(gps_info_buffer1 + len1,"%d,",nn_reserve2);
							len1 += sprintf(gps_info_buffer1 + len1,"%d,",nn_reserve3);
							len1 += sprintf(gps_info_buffer1 + len1,"%d,",nn_reserve4);
							len1 += sprintf(gps_info_buffer1 + len1,"%d,",nn_reserve5);
							len1 += sprintf(gps_info_buffer1 + len1,"%d,",nn_reserve6);
							//nn_gps_count  = 1 ;
							//len1 += sprintf(gps_info_buffer1 + len1,"%03d",nn_gps_count);
	
							if ((strlen(weidu1) != 0) && (nn_gps_count >= 1))
							{
								len1 += sprintf(gps_info_buffer1 + len1,"%03d",nn_gps_count);
								for(i = 0 ; i < (nn_gps_count) ; i++)
								{
									len1 += sprintf(gps_info_buffer1 + len1,",%s,",st_shangxun_point_gps[i].st_latitude);
									len1 += sprintf(gps_info_buffer1 + len1,"%s",st_shangxun_point_gps[i].st_longitude);
									kal_prompt_trace(MOD_TST, "st_shangxun_point_gps[%d]=%s, %s ", i, st_shangxun_point_gps[i].st_latitude, st_shangxun_point_gps[i].st_longitude);
								}
							}
							else
							{
								if (strlen(back_gps_latitude) != 0)
								{
									nn_gps_count = 1 ;
									len1 += sprintf(gps_info_buffer1 + len1,"%03d",nn_gps_count);
									len1 += sprintf(gps_info_buffer1 + len1,",%s,",back_gps_latitude);
									len1 += sprintf(gps_info_buffer1 + len1,"%s",back_gps_longitude);
									kal_prompt_trace(MOD_TST, "st_shangxun_point_gps[]=%s, %s ", back_gps_latitude, back_gps_longitude);
								}
								else
								{
									nn_gps_count = 0 ;
									len1 += sprintf(gps_info_buffer1 + len1,"%03d",nn_gps_count);
									//len1 += sprintf(gps_info_buffer1 + len1,",%s,",back_gps_latitude);
									//len1 += sprintf(gps_info_buffer1 + len1,"%s",back_gps_longitude);
									kal_prompt_trace(MOD_TST, "st_shangxun_point_gps[]=%s, %s ", back_gps_latitude, back_gps_longitude);
								}
							}
							//len1 += sprintf(gps_info_buffer1 + len1,"%d,",test_latitude);
							//len1 += sprintf(gps_info_buffer1 + len1,"%d",test_longitude);				

							len1 += sprintf(gps_info_buffer1 + len1,"#",nn_gps_count);

							kal_prompt_trace(MOD_TST, "test_latitude=%d,test_longitude=%d, ,%s,%s,len1=%d",  test_latitude,test_longitude,tmp1,tmp2,len1);
							
							kal_prompt_trace(MOD_TST, "len=%d, %s", len1, gps_info_buffer1);
						}
						old_latitude = test_latitude ;
						old_longitude = test_longitude ;
					}
				}
				break;
				case 3:	// LOGIN
					{
						
						/*kal_int32 len1 = 0;
						kal_int8 msg_id = 0;
						kal_int64 nmac_addr=0;
						
						len1 += sprintf(gps_info_buffer1,"LOGIN %d\r\n",msg_id);
						//g_lhat_imei={"866873020454024"};
						len1 += sprintf(gps_info_buffer1+len1,"D43639B816C8 400A442135D1F2723A7E841AEC92A02D %s\r\n",g_lhat_imei);
						kal_prompt_trace(MOD_TST, "len=%d, %s", len1, gps_info_buffer1);
						*/
						int msg_id = 0;
						//char base64_mac[10] = {0};
						//int len_websocket = 0;
						kal_int8 nnindex = 0;
						//unsigned char out[16] = {0};
						//char websocket_key[30] = {0};
						//char websocket[512] = {0};
						//char in[16] = {0},i=0;
						kal_int32 len1 = 0,len = 0;
						kal_int32 nnCtrlerSN = 12345;
						kal_int32 nnBMSDN = 67890;
						int ret = 0;
						//char *BLE_MAC_back = "0c592825a8d4";
						//char *login = "LOGIN";
						//char *encrypt_key = "8BD403C6283C39B7C75EC2453C399955";
						//unsigned char socket_buff[512] = {0};
#ifdef AES128
						//printf("\nTesting AES128\n\n");
						//kal_prompt_trace(MOD_TST, "\nTesting AES128\n\n");
#endif
						//memset(websocket,0,sizeof(websocket));
						/*createwebsocketkey(websocket_key);

						//len1 += sprintf(gps_info_buffer1,"LOGIN %d\r\n",msg_id);
						len1 += sprintf(gps_info_buffer1,"%s","GET / HTTP/1.1\r\n");
						len1 += sprintf(gps_info_buffer1+len1,"%s","Upgrade: websocket\r\n");
						len1 += sprintf(gps_info_buffer1+len1,"%s","Connection: Upgrade\r\n");
						len1 += sprintf(gps_info_buffer1+len1,"%s","Host: 114.55.100.103:80\r\n");
						len1 += sprintf(gps_info_buffer1+len1,"%s","Sec-WebSocket-Key: ");
						len1 += sprintf(gps_info_buffer1+len1,"%s\r\n",websocket_key);
						len1 += sprintf(gps_info_buffer1+len1,"%s","Origin: http://gps.hua1268.com/\r\n");
						//len1 += sprintf(gps_info_buffer1+len1,"%s","Origin: 114.55.100.103:80\r\n");
						len1 += sprintf(gps_info_buffer1+len1,"%s","Sec-WebSocket-Version: 13\r\n\r\n");


						len1 = strlen(gps_info_buffer1);
						ret = soc_send(g_socket_id, gps_info_buffer1, len1, 0);
						kal_prompt_trace(MOD_TST, "reply=%d, %d, %s", ret, len1, gps_info_buffer1);
						if(ret < 0)
						{
							return ;
						}
*/
						memset(gps_info_buffer1, 0, sizeof(gps_info_buffer1));
						
						//printf("key = %s\r\n",AES_KEY);
						//kal_prompt_trace(MOD_TST, "key = %s\r\n",AES_KEY);
						//strcpy(in,MAC);
						/*deviceid--->base64+L*/
						//base64_encode(device_id,6,base64_mac);
						/*len = strlen(in);
						for(i=0;i<(16-len);i++)
						{
							in[i+len] = 0x30;		//'0' macÂú∞ÂùÄÂêéÊ∑ªÂä†0 Ë°•Ë∂≥16byte
						}
						in[16] = '\0';				//end
						//printf("Âä†ÂØÜÊ∫ê:%s\r\n",in);
						kal_prompt_trace(MOD_TST, "key old:%s,base64_mac=%s\r\n",in,base64_mac);
						test_encrypt_ecb(in,out);
						//printf("\r\n");
						kal_prompt_trace(MOD_TST, "\r\n");
						test_decrypt_ecb(out);
						//printf("\r\n");
						kal_prompt_trace(MOD_TST, "\r\n");
						*/

						
						//login
#if 1						
						len = 0; 
						/*<LOGIN>*/
						len += sprintf(gps_info_buffer1,"LOGIN %d\r\n%s ",msg_id,BLE_MAC.ble_mac);//base64_mac device_id
						//len += sprintf(gps_info_buffer1,"LOGIN %d\r\n%s ",msg_id,"d61437bce3e3");//D61437BCE3E3
						//len += sprintf(gps_info_buffer1,"LOGIN %d\r\n%s ",msg_id,"aaa1b458b");
						//len += sprintf(gps_info_buffer1,"LOGIN %d\r\n%s ",msg_id,BLE_MAC_back);
						//<EncryptData>
						/*for(i=0;i<16;i++)
						{
							len1 += sprintf(gps_info_buffer1+len1,"%02x",out[i]);
						}*/
						len += sprintf(gps_info_buffer1+len,"%s",encrypt_key);
						len += sprintf(gps_info_buffer1+len," %d",nnindex);
						
						//len1 += sprintf(gps_info_buffer1+len1," %d %s\r\n",randomVal(),g_lhat_imei);
						len += sprintf(gps_info_buffer1+len," %s",g_lhat_imei);
						len += sprintf(gps_info_buffer1+len," %d",nnCtrlerSN);
						len += sprintf(gps_info_buffer1+len," %d\r\n",nnBMSDN);
						
						//printf("socket buf :%s",socket_buff);
						kal_prompt_trace(MOD_TST, "socket buf :%s\r\n",gps_info_buffer1);

						//	len = build_websocket_msg_unmask(socket_buff,strlen(socket_buff));	
						/*websocket data frame with msk*/
						//len = build_websocket_msg(gps_info_buffer1,strlen(gps_info_buffer1));
						//printf("websocket len:%d\r\n",len);
						kal_prompt_trace(MOD_TST, "websocket len:%d\r\n",len);

						//for(i=0;i<len;i++)
						//	kal_prompt_trace(MOD_TST, "%02x",gps_info_buffer1[i]);

						kal_prompt_trace(MOD_TST, "send data = %s %d\r\n%s %s %s\r\n",msg_id,lhat_device.num,encrypt_key,g_lhat_imei);

						len1 = strlen(gps_info_buffer1);
						ret = soc_send(g_socket_id, gps_info_buffer1, len1, 0);
						kal_prompt_trace(MOD_TST, "reply=%d, %d, %s", ret, len1, gps_info_buffer1);

						if(ret < 0)
						{
							g_heart_beat = 0;
							n_gprs_send_fail_count++ ;
							if (n_gprs_send_fail_count > 10)
							{
								n_gprs_send_fail_count = 0 ;
								g_start_to_gps_flag = 0 ;
							}
						}
						if (ret >= 0)
						{
							g_heart_beat = 1 ;
							memset(&g_lhat_gps, 0, sizeof(g_lhat_gps));
							mmi_gprs_socket_close_handle();
							g_gprs_login_status_flag=KAL_TRUE;
							//g_gprs_login_status_flag=KAL_TRUE;
							return ;
						}
#endif						
					}
					break ;
				case 4:	// CONFIG
					{
						{
							kal_int32 len1 = 0;
							kal_int8 msg_id = 2;
							kal_int8 nnresult = 0;
							
							len1 += sprintf(gps_info_buffer1,"CONFIG %d\r\n",msg_id);
							len1 += sprintf(gps_info_buffer1+len1,"%d\r\n",nnresult);
							kal_prompt_trace(MOD_TST, "len=%d, %s", len1, gps_info_buffer1);

							kal_prompt_trace(MOD_TST, "send data = %s %s\r\n",gps_info_buffer1,g_lhat_imei);

							len1 = strlen(gps_info_buffer1);
							ret = soc_send(g_socket_id, gps_info_buffer1, len1, 0);
							kal_prompt_trace(MOD_TST, "reply=%d, %d, %s", ret, len1, gps_info_buffer1);

							if(ret < 0)
							{
								g_heart_beat = 0;
								n_gprs_send_fail_count++ ;
								if (n_gprs_send_fail_count > 10)
								{
									n_gprs_send_fail_count = 0 ;
									//g_start_to_gps_flag = 0 ;
								}
							}
							if (ret >= 0)
							{
								g_heart_beat = 1 ;
								memset(&g_lhat_gps, 0, sizeof(g_lhat_gps));
								mmi_gprs_socket_close_handle();
								return ;
							}	
						}

					}
					break ;	
				/*case 5:	// LOCATION
					
						{
							kal_int32 len1 = 0;
							kal_int8 msg_id = 3;
							kal_int8 gps_lbs = 1;
							kal_int8 nnvibrate = 0;
							kal_int32 nnVol = 357;
							kal_int64 nmac_addr=0;
							char jingdu1[11] = {"120.1581529"}, weidu1[11] = {"30.1790538"};
							char jingdu12[11] = {0}, weidu12[11] = {0};
							
							RTC_CTRL_GET_TIME_T rtc_time;
							DCL_HANDLE rtc_handler = 0;
							
							jingdu_change(jingdu12);
							weidu_change(weidu12);

							nnVol = get_Vbat_Voltage();
							nnVol = nnVol/10000;
							
							rtc_handler = DclRTC_Open(DCL_RTC,FLAGS_NONE);
							DclRTC_Control(rtc_handler, RTC_CMD_GET_TIME, (DCL_CTRL_DATA_T *)& rtc_time);
							DclRTC_Close(rtc_handler);	
							kal_prompt_trace(MOD_TST, "u1Year=%2d,u1Mon=%2d,u1Day=%2d,u1Hour=%2d,u1Min=%2d,u1Sec=%2d", rtc_time.u1Year,rtc_time.u1Mon,rtc_time.u1Day,rtc_time.u1Hour,rtc_time.u1Min,rtc_time.u1Sec);

							len1 += sprintf(gps_info_buffer1,"LOCATION %d\r\n",msg_id);
							len1 += sprintf(gps_info_buffer1+len1,"%4d-%d-%d %d:%d:%d,",2000+rtc_time.u1Year,rtc_time.u1Mon,rtc_time.u1Day,rtc_time.u1Hour,rtc_time.u1Min,rtc_time.u1Sec);
							if((strlen(weidu12) != 0) && (strlen(jingdu12) != 0))
							{
								len1 += sprintf(gps_info_buffer1+len1,"%s,%s,",jingdu12,weidu12);
							}
							else
							{
								len1 += sprintf(gps_info_buffer1+len1,"%s,%s,",jingdu1,weidu1);
							}
							len1 += sprintf(gps_info_buffer1+len1,"%d,",gps_lbs);
							len1 += sprintf(gps_info_buffer1+len1,"%d,",nnvibrate);
							len1 += sprintf(gps_info_buffer1+len1,"%d\r\n",nnVol);
							kal_prompt_trace(MOD_TST, "len=%d, %s", len1, gps_info_buffer1);
							len = build_websocket_msg(gps_info_buffer1,strlen(gps_info_buffer1));
							kal_prompt_trace(MOD_TST, "send data = %s \r\n",gps_info_buffer1);
							
						}
					break ;	*/
				case 6:	// OPEN
					{
						kal_int32 len1 = 0;
						//kal_int32 nnUserId = 123456789;
						//kal_int32 nnTradeNo = 1;
						//U32 ntimestamp = 0 ;
						kal_int8 msg_id = 4;
						//kal_int8 nnIndexKey = 0;
						kal_int8 rescode = 0 ;

						//ntimestamp = mmi_gprs_get_timestamp();
						
						len1 += sprintf(gps_info_buffer1,"OPEN %d\r\n",msg_id);
						len1 += sprintf(gps_info_buffer1+len1,"%d OK\r\n",rescode);

						len1 = strlen(gps_info_buffer1);
						ret = soc_send(g_socket_id, gps_info_buffer1, len1, 0);
						kal_prompt_trace(MOD_TST, "reply=%d, %d, %s", ret, len1, gps_info_buffer1);

						//g_gprs_record_status_flag
						if(ret < 0)
						{
							g_heart_beat = 0;
							n_gprs_send_fail_count++ ;
							if (n_gprs_send_fail_count > 10)
							{
								n_gprs_send_fail_count = 0 ;
								//g_start_to_gps_flag = 0 ;
							}
						}
						if (ret >= 0)
						{
							g_heart_beat = 1 ;
							memset(&g_lhat_gps, 0, sizeof(g_lhat_gps));
							mmi_gprs_socket_close_handle();
							return ;
						}	
						
					}
					break ;		
				case 7:	// REPORT
					{
						kal_int32 len1 = 0;
						kal_int8 msg_id = 5;
						kal_int8 nnType = 0;
						kal_int8 nnCalibrateStatus = 0;
						kal_int8 nnRunningCtrl = 0;
						kal_int8 nnRunningStatus = 0;
						kal_int8 nnTradeNo = 0;
						kal_int8 nnTradeType = 3;
						kal_int8 nnINdexKey = 0;
						kal_int8 nnLocationType = 1;
						kal_int8 nnVibrate = 0;
						kal_int16 nnVol1 = 357;
						kal_int16 nnCur1 = 557;
						kal_int16 nnSpeed = 30;
						kal_int16 nnRPM = 100;
						kal_int16 nnErrCode = 0;
						kal_int16 nnKms = 101;
						kal_int64 nmac_addr=0;
						kal_int32 nnTradeDate=n_data_value;
						kal_int32 nnUserId=0;
						U32 ntimestamp = 0 ;
						char jingdu1[12] = {"120.158152"}, weidu1[12] = {"30.179053"};
						char jingdu12[12] = {0}, weidu12[12] = {0};
						//RTC_CTRL_GET_TIME_T rtc_time;
						//DCL_HANDLE rtc_handler = 0;
						
						//rtc_handler = DclRTC_Open(DCL_RTC,FLAGS_NONE);
						//DclRTC_Control(rtc_handler, RTC_CMD_GET_TIME, (DCL_CTRL_DATA_T *)& rtc_time);
						//DclRTC_Close(rtc_handler);	
						//kal_prompt_trace(MOD_TST, "u1Year=%2d,u1Mon=%2d,u1Day=%2d,u1Hour=%2d,u1Min=%2d,u1Sec=%2d", rtc_time.u1Year,rtc_time.u1Mon,rtc_time.u1Day,rtc_time.u1Hour,rtc_time.u1Min,rtc_time.u1Sec);

						jingdu_change(jingdu12);
						weidu_change(weidu12);
						ntimestamp = mmi_gprs_get_timestamp();
						//nnTradeDate = mmi_gprs_get_timestamp();

						if(KAL_TRUE == mmi_get_eint2_status())
						{
							nnType = 2 ;
						}
						else if (KAL_TRUE == mmi_get_bike_lock_status())
						{
							nnType = 0 ;
						}
						else if (KAL_TRUE == mmi_get_bike_lock_open_status())
						{
							nnType = 3 ;
						}
						else
						{
							nnType = 4 ;
						}
						len1 += sprintf(gps_info_buffer1,"REPORT %d\r\n",msg_id);
						len1 += sprintf(gps_info_buffer1+len1,"%d,",nnType);
						//len1 += sprintf(gps_info_buffer1+len1,"%4d-%d-%d %d:%d:%d,",2000+rtc_time.u1Year,rtc_time.u1Mon,rtc_time.u1Day,rtc_time.u1Hour,rtc_time.u1Min,rtc_time.u1Sec);
						len1 += sprintf(gps_info_buffer1+len1,"%d,",ntimestamp);
						len1 += sprintf(gps_info_buffer1+len1,"%d,",nnVol1);
						len1 += sprintf(gps_info_buffer1+len1,"%d,",nnCur1);
						len1 += sprintf(gps_info_buffer1+len1,"%d,",nnSpeed);
						len1 += sprintf(gps_info_buffer1+len1,"%d,",nnRPM);
						len1 += sprintf(gps_info_buffer1+len1,"%d,",nnErrCode);
						len1 += sprintf(gps_info_buffer1+len1,"%d,",nnCalibrateStatus);
						len1 += sprintf(gps_info_buffer1+len1,"%d,",nnRunningCtrl);
						len1 += sprintf(gps_info_buffer1+len1,"%d,",nnRunningStatus);
						len1 += sprintf(gps_info_buffer1+len1,"%d,",nnKms);
						len1 += sprintf(gps_info_buffer1+len1,"%d,",nnTradeDate);
						len1 += sprintf(gps_info_buffer1+len1,"%s,",s_user_id);
						len1 += sprintf(gps_info_buffer1+len1,"%s,",s_trade_no);
						len1 += sprintf(gps_info_buffer1+len1,"%d,",ntimestamp);
						len1 += sprintf(gps_info_buffer1+len1,"%s,",encrypt_key);
						len1 += sprintf(gps_info_buffer1+len1,"%d,",nnINdexKey);
						if((strlen(weidu12) != 0) && (strlen(jingdu12) != 0))
						{
							len1 += sprintf(gps_info_buffer1+len1,"%s,%s,",jingdu12,weidu12);
						}
						else
						{
							len1 += sprintf(gps_info_buffer1+len1,"%s,%s,",jingdu1,weidu1);
						}
							
						//len1 += sprintf(gps_info_buffer1+len1,"%d,",nnLongitude);
						//len1 += sprintf(gps_info_buffer1+len1,"%d,",nnLatitude);
						len1 += sprintf(gps_info_buffer1+len1,"%d,",nnLocationType);
						
						len1 += sprintf(gps_info_buffer1+len1,"%d\r\n",nnVibrate);
						kal_prompt_trace(MOD_TST, "len=%d, %s", len1, gps_info_buffer1);
						//len = build_websocket_msg(gps_info_buffer1,strlen(gps_info_buffer1));
						kal_prompt_trace(MOD_TST, "send data = %s \r\n",gps_info_buffer1);

						len1 = strlen(gps_info_buffer1);
						ret = soc_send(g_socket_id, gps_info_buffer1, len1, 0);
						kal_prompt_trace(MOD_TST, "reply=%d, %d, %s", ret, len1, gps_info_buffer1);

						//g_gprs_record_status_flag
						if(ret < 0)
						{
							g_heart_beat = 0;
							n_gprs_send_fail_count++ ;
							if (n_gprs_send_fail_count > 10)
							{
								n_gprs_send_fail_count = 0 ;
								//g_start_to_gps_flag = 0 ;
							}
						}
						if (ret >= 0)
						{
							g_heart_beat = 1 ;
							memset(&g_lhat_gps, 0, sizeof(g_lhat_gps));
							mmi_gprs_socket_close_handle();
							return ;
						}	
					}
					break ;		
				case 8:	// BMS REPORT 
					{
						kal_int32 len1 = 0;
						kal_int8 msg_id = 6;
						kal_int8 nnType = 1;
						kal_int8 nnElec = 0;
						kal_int16 nnTemp = 257;
						kal_int16 nnElectricity = 157;
						kal_int16 nnChargeDisChargeCounts = 30;
						kal_int16 nnMaxChargeInterval = 100;
						kal_int16 nnCurChargeInterval = 1;
						kal_int16 nnCapacity = 0;
						kal_int16 nnVol2 = 357;
						kal_int16 nnCur2 = 557;
						U32 ntimestamp = 0 ;
						char jingdu1[11] = {"120.1581529"}, weidu1[11] = {"30.1790538"};
						
						ntimestamp = mmi_gprs_get_timestamp();
						
						len1 += sprintf(gps_info_buffer1,"BMSREPORT %d\r\n",msg_id);
						len1 += sprintf(gps_info_buffer1+len1,"%d,",nnType);
						//len1 += sprintf(gps_info_buffer1+len1,"%4d-%d-%d %d:%d:%d,",2000+rtc_time.u1Year,rtc_time.u1Mon,rtc_time.u1Day,rtc_time.u1Hour,rtc_time.u1Min,rtc_time.u1Sec);
						len1 += sprintf(gps_info_buffer1+len1,"%d,",ntimestamp);
						len1 += sprintf(gps_info_buffer1+len1,"%d,",nnTemp);
						len1 += sprintf(gps_info_buffer1+len1,"%d,",nnElectricity);
						len1 += sprintf(gps_info_buffer1+len1,"%d,",nnChargeDisChargeCounts);
						len1 += sprintf(gps_info_buffer1+len1,"%d,",nnCurChargeInterval);
						len1 += sprintf(gps_info_buffer1+len1,"%d,",nnCapacity);
						len1 += sprintf(gps_info_buffer1+len1,"%d,",nnElec);
						len1 += sprintf(gps_info_buffer1+len1,"%d,",nnVol2);
						len1 += sprintf(gps_info_buffer1+len1,"%d\r\n",nnCur2);
						kal_prompt_trace(MOD_TST, "len=%d, %s", len1, gps_info_buffer1);
						//len = build_websocket_msg(gps_info_buffer1,strlen(gps_info_buffer1));
						kal_prompt_trace(MOD_TST, "send data = %s \r\n",gps_info_buffer1);

						len1 = strlen(gps_info_buffer1);
						ret = soc_send(g_socket_id, gps_info_buffer1, len1, 0);
						kal_prompt_trace(MOD_TST, "reply=%d, %d, %s", ret, len1, gps_info_buffer1);
						
						if(ret < 0)
						{
							g_heart_beat = 0;
							n_gprs_send_fail_count++ ;
							if (n_gprs_send_fail_count > 10)
							{
								n_gprs_send_fail_count = 0 ;
								//g_start_to_gps_flag = 0 ;
							}
						}
						if (ret >= 0)
						{
							g_heart_beat = 1 ;
							memset(&g_lhat_gps, 0, sizeof(g_lhat_gps));
							mmi_gprs_socket_close_handle();
							return ;
						}	
					}
					break ;		

				case 9:	// GETCONFIG
					{
						kal_int32 len1 = 0;
						kal_int8 msg_id = 7;
						kal_int8 nn_ping_period = 1;
						kal_int8 nn_gps_period = 0;
						kal_int8 nn_bVirate = 0;
						kal_int8 nn_bSMS = 0;

						len1 += sprintf(gps_info_buffer1,"GETCONFIG %d\r\n",msg_id);
						len1 += sprintf(gps_info_buffer1+len1,"%d,",nn_ping_period);
						len1 += sprintf(gps_info_buffer1+len1,"%d,",nn_gps_period);
						len1 += sprintf(gps_info_buffer1+len1,"%d,",nn_bVirate);
						len1 += sprintf(gps_info_buffer1+len1,"%d,",nn_bSMS);
						len1 += sprintf(gps_info_buffer1+len1,"%s,",g_ip_addr.addr);
						len1 += sprintf(gps_info_buffer1+len1,"%d\r\n",g_ip_addr.port);
						kal_prompt_trace(MOD_TST, "len=%d, %s", len1, gps_info_buffer1);
						//len = build_websocket_msg(gps_info_buffer1,strlen(gps_info_buffer1));
						kal_prompt_trace(MOD_TST, "send data = %s \r\n",gps_info_buffer1);
						
						len1 = strlen(gps_info_buffer1);
						ret = soc_send(g_socket_id, gps_info_buffer1, len1, 0);
						kal_prompt_trace(MOD_TST, "reply=%d, %d, %s", ret, len1, gps_info_buffer1);
						
						if(ret < 0)
						{
							g_heart_beat = 0;
							n_gprs_send_fail_count++ ;
							if (n_gprs_send_fail_count > 10)
							{
								n_gprs_send_fail_count = 0 ;
								//g_start_to_gps_flag = 0 ;
							}
						}
						if (ret >= 0)
						{
							g_heart_beat = 1 ;
							memset(&g_lhat_gps, 0, sizeof(g_lhat_gps));
							mmi_gprs_socket_close_handle();
							return ;
						}	
					}
					break ;		
				case 10:	// RIDINGMODE
					{
						kal_int32 len1 = 0;
						kal_int8 msg_id = 8;
						kal_int8 nnType = 1;
						kal_int8 nnResCode = 0;

						len1 += sprintf(gps_info_buffer1,"RIDINGMODE %d\r\n",msg_id);
						len1 += sprintf(gps_info_buffer1+len1,"%d,",nnType);
						len1 += sprintf(gps_info_buffer1+len1,"%s OK\r\n",nnResCode);
						kal_prompt_trace(MOD_TST, "len=%d, %s", len1, gps_info_buffer1);
						//len = build_websocket_msg(gps_info_buffer1,strlen(gps_info_buffer1));
						kal_prompt_trace(MOD_TST, "send data = %s \r\n",gps_info_buffer1);
						
						len1 = strlen(gps_info_buffer1);
						ret = soc_send(g_socket_id, gps_info_buffer1, len1, 0);
						kal_prompt_trace(MOD_TST, "reply=%d, %d, %s", ret, len1, gps_info_buffer1);
						
						if(ret < 0)
						{
							g_heart_beat = 0;
							n_gprs_send_fail_count++ ;
							if (n_gprs_send_fail_count > 10)
							{
								n_gprs_send_fail_count = 0 ;
								//g_start_to_gps_flag = 0 ;
							}
						}
						if (ret >= 0)
						{
							g_heart_beat = 1 ;
							memset(&g_lhat_gps, 0, sizeof(g_lhat_gps));
							mmi_gprs_socket_close_handle();
							return ;
						}	
					}
					break ;			
				default:
					break;
			
		}
		len = strlen(gps_info_buffer1);
		ret = soc_send(g_socket_id, gps_info_buffer1, len, 0);
		kal_prompt_trace(MOD_TST, "reply=%d, %d, %s", ret, len, gps_info_buffer1);
		if(ret < 0)
		{
			g_heart_beat = 0;
			n_gprs_send_fail_count++ ;
			if (n_gprs_send_fail_count > 10)
			{
				n_gprs_send_fail_count = 0 ;
				g_start_to_gps_flag = 0 ;
			}
		}
		else if((1 == num || 2 == num)&& (g_lhat_gps.east_west || g_lhat_gps.north_south))
			memset(&g_lhat_gps, 0, sizeof(g_lhat_gps));

		if (ret >= 0)
		{
			g_heart_beat = 1 ;
			//g_start_to_gps_flag = 2 ;
			mmi_gprs_socket_close_handle();
			//g_heart_beat = 1 ;
			//g_start_to_gps_flag = 2 ;
		}
	}
}


#endif

void gprs_open_close_gps(U8 pre_riding_flag)
{
	if(0 == g_guard_flag)
	{
		if(0 == pre_riding_flag && 1 == g_riding_flag)
			lhat_open_gps();
		else if(1 == pre_riding_flag && 0 == g_riding_flag)
			mdi_gps_uart_close(mdi_get_gps_port(), MDI_GPS_UART_MODE_LOCATION, lhat_gps_callback);
	}
}

void mmi_encrypt_key_init(void)
{
	u8 len =0;
	char  gps_info_buffer1[200] = {0};
	char *encrypt_key_base = "12345678901234567890123456789012345678901234567890123456789012345678";
	
	len += sprintf(encrypt_key,"%s",encrypt_key_base);
	len += sprintf(encrypt_key+len,"%s",BLE_MAC.ble_mac);
}

#if defined(__LHAT_HUAYUAN__)
void gprs_socket_receive(void)
{
	char *pch;
	int ret;
	static char recv_buff_a[500] = {0};
	int cmd = 0;
	char *p2 = NULL, *p3 = NULL;
	char *pHead = NULL; 
	U16 heart_time = 60000, gps_time = 30000;
	U8 pre_riding_flag = g_riding_flag;
	
	memset(recv_buff_a, 0, sizeof(recv_buff_a));
	ret = soc_recv(g_socket_id, recv_buff_a, sizeof(recv_buff_a) - 1, 0);
	kal_prompt_trace(MOD_TST, "gprs_socket_receive HUAYUAN=%d, %d, %s", ret, g_receive_times, recv_buff_a);
	if(ret == -2 && g_receive_times < 2)
	{
		g_receive_times++;
		StartTimer(GIS_TIMER_ID_4, 600, gprs_socket_receive);
		return;
	}
	pHead = strstr(recv_buff_a,"Content-Type");
	pHead = strstr(pHead,"ETag");
	pHead = strstr(pHead,"Request-Id");
	kal_prompt_trace(MOD_TST, "pHead1=%s", pHead);
	//pHead = strstr(pHead,"X-Runtime");
	pHead = strstr(pHead,"status");
	
	kal_prompt_trace(MOD_TST, "pHead2=%s", pHead);
	if(strstr(pHead,"success"))
	{
		//char *pHead = NULL; 
				
		//pHead = strstr(recv_buff_a,"request_count");
		kal_prompt_trace(MOD_TST, "gprs_socket_receive111=%s", pHead);
		//memset(back_gps_data,0,sizeof(back_gps_data));
		//back_count = 0 ;
		//old_longitude = 0 ;
		//old_latitude = 0 ;
		mmi_set_gps_array_zore();
	}

}

#if 0
void gprs_socket_receive(void)
{
		char *pch;
		int ret;
#ifdef GPRS_TEST
		//static char recv_buff_a[500] = {"{\"rs\":1,\"cmd\":1}"};
		//static char recv_buff_a[500] = {"{\"rs\":1,\"cmd\":98,\"info\":[{\"nn\":\"‘¡B88888\",\"mb\":\"18888888888\"}]}"};
		static char recv_buff_a[500] = {"{\"rs\":1,\"cmd\":9,\"info\":{\"st\":1,\"sp\":15,\"max\":0,\"ts\":\"15:00-18:00\"}}}"};
#else
		static char recv_buff_a[500] = {0};
#endif
		int cmd = 0;
		char *p2 = NULL, *p3 = NULL;
		char *pHead = NULL; 
#ifdef GPRS_TEST	
		//memset(recv_buff_a, 0, sizeof(recv_buff_a));

		//recv_buff_a = {"rs:1,cmd:1"};
		ret = 1;
#else
		memset(recv_buff_a, 0, sizeof(recv_buff_a));
		ret = soc_recv(g_socket_id, recv_buff_a, sizeof(recv_buff_a) - 1, 0);
#endif
		kal_prompt_trace(MOD_TST, "gprs_socket_receive=%d, %d, %s", ret, g_receive_times, recv_buff_a);
		if(ret == -2 && g_receive_times < 2)
		{
			g_receive_times++;
			StartTimer(GIS_TIMER_ID_4, 600, gprs_socket_receive);
			return;
		}
	
			pHead = strstr(recv_buff_a,"cmd");
			kal_prompt_trace(MOD_TST, "gprs_socket_receive pHead=%s ",pHead);
			pHead +=5 ;
			cmd = atoi(pHead);
			kal_prompt_trace(MOD_TST, "gprs_socket_receive cmd=%d ", cmd);
			switch(cmd)
			{
				case 1:
					/*
					resquest:
					   {"gid":"123asx244","cmd":1,"info":{"pc":1,"vs":1.0,"lng":37.089,"lat":138.2456}}
						response:
						{"rs":1,"cmd":1}or{"rs":0,"cmd":1}
					*/
					{
						U8 receive_rs = 1 ;
	
						pHead = strstr(recv_buff_a,"rs");
	
						pHead +=4 ;
						receive_rs= (U8)atoi(pHead);
						kal_prompt_trace(MOD_TST, "gprs_socket_receive cmd=%d receive_rs=%d", cmd,receive_rs);
					}
					break;
				case 2:
					/*
					resquest:
					   {"gid":"123asx244","cmd":2,"info":{"pc":1,"vs":1.0,"lng":37.089,"lat":138.2456}}
					   response:
					   {"rs":1,"cmd":2}or{"rs":0,"cmd":2}
					*/
					{
						char *pHead = NULL; 
						U8 receive_rs = 1 ;
	
						pHead = strstr(recv_buff_a,"rs");
	
						pHead +=4 ;
						receive_rs= (U8)atoi(pHead);
						kal_prompt_trace(MOD_TST, "gprs_socket_receive cmd=%d receive_rs=%d", cmd,receive_rs);
					}
					break;
				case 99:
					/*
					{"gid":"123asx244","cmd":99,info:{"pwd":"123456","mb":18888888888}}
						{"rs":1,"cmd":99}or{"rs":0,"cmd":99}
					*/
					{
						char *pHead = NULL; 
						U8 receive_rs = 1 ;
	
						pHead = strstr(recv_buff_a,"rs");
	
						pHead +=4 ;
						receive_rs= (U8)atoi(pHead);
						kal_prompt_trace(MOD_TST, "gprs_socket_receive cmd=%d receive_rs=%d", cmd,receive_rs);
					}
					break;
				case 98:
					/*
					{"gid":"123asx244","cmd":98}
						 response:
						 {"rs":1,"cmd":98,"info":[{"nn":"‘¡B88888","mb":"18888888888"},{"nn":"¿∂¥Û±º","mb":"17777777777"}]}  or  {"rs":0,,"cmd":98}
					*/
					{
						char *pHead = NULL; 
						U8 receive_rs = 1 ;
	
						pHead = strstr(recv_buff_a,"nn");
	
						pHead +=5 ;
						kal_prompt_trace(MOD_TST, "gprs_socket_receive cmd=%d receive_nn=%s", cmd,pHead);
	
						pHead = strstr(recv_buff_a,"mb");
						pHead +=5 ;
						kal_prompt_trace(MOD_TST, "gprs_socket_receive cmd=%d receive_mb=%s", cmd,pHead);
					}
					break;
				case 9:
					/*
					{"gid":"123asx244","cmd":9}
				   response:
				   {"rs":1,"cmd":9,"info":{"st":"1","sp":15,"max":0,"ts":"15:00-18:00"}} or {"rs":0,"cmd":9}
					*/
					{
						char *pHead = NULL; 
						U8 receive_rs = 1 ;
						U8 receive_st = 0 ;
						U8 receive_sp = 0 ;
						U8 receive_max = 0 ;
	
						pHead = strstr(recv_buff_a,"st");
						pHead +=4 ;
						receive_st = (U8)atoi(pHead);
						kal_prompt_trace(MOD_TST, "gprs_socket_receive cmd=%d receive_st=%d", cmd,receive_st);
	
						pHead = strstr(recv_buff_a,"sp");
						pHead +=4 ;
						receive_sp = (U8)atoi(pHead);
						kal_prompt_trace(MOD_TST, "gprs_socket_receive cmd=%d receive_sp=%d", cmd,receive_sp);
	
						pHead = strstr(recv_buff_a,"max");
						pHead +=5 ;
						receive_max = (U8)atoi(pHead);
						kal_prompt_trace(MOD_TST, "gprs_socket_receive cmd=%d receive_max=%d", cmd,receive_max);
	
						pHead = strstr(recv_buff_a,"ts");
						pHead +=5 ;
						kal_prompt_trace(MOD_TST, "gprs_socket_receive cmd=%d receive_ts=%s", cmd,pHead);
					}
					break;
				default:
					break;
			}
}
#endif

#elif defined(__LHAT_HUAYUAN_PROTOCOL__) || defined(__LHAT_TREE_YEAR_COMMON__)

void gprs_socket_receive(void)
{
	char *pch;
	int ret;
	static char recv_buff_a[500] = {0};
	int cmd = 0;
	char *p2 = NULL, *p3 = NULL;
	char *pHead = NULL; 
	U16 heart_time = 60000, gps_time = 30000;
	U8 pre_riding_flag = g_riding_flag;
	
	memset(recv_buff_a, 0, sizeof(recv_buff_a));
	ret = soc_recv(g_socket_id, recv_buff_a, sizeof(recv_buff_a) - 1, 0);
	kal_prompt_trace(MOD_TST, "gprs_socket_receive HUAYUAN_PROTOCOL=%d, %d, %s", ret, g_receive_times, recv_buff_a);
	if(ret == -2 && g_receive_times < 2)
	{
		g_receive_times++;
		StartTimer(GIS_TIMER_ID_4, 600, gprs_socket_receive);
		return;
	}
	pHead = strstr(recv_buff_a,"Content-Length");//pHead = strstr(recv_buff_a,"Content-Type");
	//pHead = strstr(pHead,"ETag");
	//pHead = strstr(pHead,"Request-Id");
	kal_prompt_trace(MOD_TST, "pHead1=%s", pHead);
	//pHead = strstr(pHead,"X-Runtime");
	pHead = strstr(pHead,"status");
	if (strstr(pHead,"error"))
	{
		kal_prompt_trace(MOD_TST, "receive error!!!!");
	}
	else if(strstr(pHead,"success"))	
	{
		parseSeverRsp_string(pHead);
	}

	kal_prompt_trace(MOD_TST, "pHead2=%s", pHead);

	/*if(strstr(pHead,"success"))
	{
		//char *pHead = NULL; 
		U8 nn_u_t = 0 ;
		U8 nn_s_f = 0 ;
		U8 nn_s_t = 0 ;
		U8 nn_r_f = 0 ;
		U8 nn_u_f = 0 ;
		U8 nn_c_f = 0 ;
				
		//pHead = strstr(recv_buff_a,"request_count");
		kal_prompt_trace(MOD_TST, "receive111=%s", pHead);
		pHead = strstr(pHead,"u_t");
		pHead += 2;
		nn_u_t = atoi(pHead) ;
		
		pHead = strstr(pHead,"s_f");
		pHead += 2;
		nn_s_f = atoi(pHead) ;

		pHead = strstr(pHead,"s_t");
		pHead += 2;
		nn_s_t = atoi(pHead) ;

		pHead = strstr(pHead,"r_f");
		pHead += 2;
		nn_r_f = atoi(pHead) ;

		pHead = strstr(pHead,"u_f");
		pHead += 2;
		nn_u_f = atoi(pHead) ;

		pHead = strstr(pHead,"c_f");
		pHead += 2;
		nn_c_f = atoi(pHead) ;
		
		if (nn_c_f == 1)
		{
			mmi_set_time_fre_value(nn_u_t);
		}
		//memset(back_gps_data,0,sizeof(back_gps_data));
		//back_count = 0 ;
		//old_longitude = 0 ;
		//old_latitude = 0 ;
		mmi_set_gps_array_zore();
	}*/
	mmi_set_gps_array_zore();
}



#elif defined(__LHAT_SHANGXUN_COMMON__)

int handleMsg(u8 cmd,char *msg)
{
	char tmp[20] = {0};
	char i=0;
	int data,data2;
	int ret = 0;
	U32 ntimestamp = 0 ;
	char *p=NULL;
	if(!msg)	return -1;
	switch(cmd)
	{
		case 1:			//ø™À¯
		{
			p = strstr(msg,"b_lock");
			if(p&&p+8)
			{
				p = p+8;
				while(*p!='}')
				{
					tmp[i] = *p;	
					i++;
					p++;
					if(i>=2)
						return;
				}
				data = atoi((char *)tmp);
				/*if((data == 1)&&(ntimestamp<=30))
				{
					kal_prompt_trace(MOD_TST,"open lock\r\n");
					//ø™À¯
					//mmi_set_bike_lock_open_status(KAL_TRUE);
					//mmi_set_bike_lock_close_init_status(KAL_FALSE);
					//lhat_5v_dc_onoff(1);
					//mmi_set_gpio_timer_status();
					//mmi_set_eint2_status(KAL_TRUE);
					mmi_set_bike_lock_status(KAL_TRUE);
				///////////TUDO		
				}
				else
				{
					if(ntimestamp>30)
					{
						kal_prompt_trace(MOD_TST," lock fail time stamp 60 \r\n");
					}
					else
					{
						kal_prompt_trace(MOD_TST," lock fail lock 0 \r\n");
					}
					
				}*/
				kal_prompt_trace(MOD_TST,"b_lock =%d\r\n",data);
				
			}
			i = 0 ;
			memset(tmp,0,sizeof(tmp));
			p = strstr(msg,"time_stamp");
			if(p&&p+12)
			{
				p = p+12;
				while(*p!=',')
				{
					tmp[i] = *p;	
					i++;
					p++;
					if(i>=12)
						return;
				}
				data2 = atoi((char *)tmp);
				ntimestamp = mmi_gprs_get_timestamp();
				kal_prompt_trace(MOD_TST,"mmi_gprs_get_timestamp =%d,data2=%d\r\n",ntimestamp,data2);
				ntimestamp = ntimestamp-data2;
				ntimestamp = abs(ntimestamp);
				kal_prompt_trace(MOD_TST,"ntimestamp =%d\r\n",ntimestamp);
				if((data == 1)&&(ntimestamp<=20))
				{
					kal_prompt_trace(MOD_TST,"open lock\r\n");
					//ø™À¯
					//mmi_set_bike_lock_open_status(KAL_TRUE);
					//mmi_set_bike_lock_close_init_status(KAL_FALSE);
					//lhat_5v_dc_onoff(1);
					//mmi_set_gpio_timer_status();
					//mmi_set_eint2_status(KAL_TRUE);
					mmi_set_bike_lock_status(KAL_TRUE);
				///////////TUDO		
				}
				else
				{
					if(ntimestamp>20)
					{
						kal_prompt_trace(MOD_TST," lock fail time stamp 20 \r\n");
					}
					else
					{
						kal_prompt_trace(MOD_TST," lock fail lock 0 \r\n");
					}
					
				}
			}
			
			
		}
		break;
		case 2:			//–ﬁ∏ƒIP
		{		
			p = strstr(msg,"b_set_ip_flag");
			if(p&&p+15)
			{
				p = p+15;
				while(*p!=',')
				{
					tmp[i] = *p;	
					i++;
					p++;
					if(i>=2)
						return;
				}
				data = atoi((char *)tmp);
				if(data == 1)
				{
					memset(tmp,0,sizeof(tmp));
					i = 0;
					p = strstr(msg,"IP");
					if(p&&p+5)
					{
						p = p+5;
						while(*p!='\"')
						{
							tmp[i] = *p;
							p++;
							i++;
							if(i>=17)
								return ;
						}
						tmp[i] = '\0';			//add by lxj
						kal_prompt_trace(MOD_TST,"ip :%s\r\n",tmp);
						//tudo
						ret = chkip(tmp);
						if(!ret)
						{
							//save
							memset(lhat_device.ip,0,sizeof(lhat_device.ip));	//add by lxj
							memcpy(lhat_device.ip,tmp,strlen(tmp));
							kal_prompt_trace(MOD_TST, "recv IP:%s ",tmp);
						}
						else
						{
							return -1 ;
						}
					}


					memset(tmp,0,sizeof(tmp));
					i = 0;
					p = strstr(msg,"PORT");
					if(p&&p+7)
					{
						p = p+7;
						while(*p!='}')
						{
							tmp[i] = *p;
							p++;
							i++;
							if(i>=7)
								return ;
						}
						data = atoi((char *)tmp);
						kal_prompt_trace(MOD_TST,"PORT :%d\r\n",data);
						
						//tudo
						ret = chkport(data);
						if(!ret)
						{
							//save
							lhat_device.port = data;
							kal_prompt_trace(MOD_TST, "recv port:%d ",data);
						}
						//AlmATPowerReset(MMI_FALSE, 10);
					}
					chkport_ext(lhat_device.port);
					chkip_ext(lhat_device.ip);
					gprs_socket_connect();
				}
			}
			
			//add by lxj
			Clear_NVRAM_Write();
			Restore_NVRAM_Write(0);
		}
		break;	
		defualt:break;
	}
}




void gprs_socket_receive(void)
{

	int ret,i=0; 
	char tmp[20] = {0};
	static char recv_buff_a[500] = {0};
	int cmd = 0;

	char *pHead = NULL; 
	
	memset(recv_buff_a, 0, sizeof(recv_buff_a));
	ret = soc_recv(g_socket_id, recv_buff_a, sizeof(recv_buff_a) - 1, 0);
	kal_prompt_trace(MOD_TST, "shangxun=%d, %d, %s", ret, g_receive_times, recv_buff_a);
	if(ret == -2 && g_receive_times < 2)
	{
		g_receive_times++;
		StartTimer(GIS_TIMER_ID_4, 600, gprs_socket_receive);
		return;
	}
	
	/*if(strstr(recv_buff_a,"success"))
	{
		//char *pHead = NULL; 
		pHead = strstr(recv_buff_a,",\"status");
		if(pHead&&pHead+10)
		{
			pHead = pHead+10;
			while(*pHead!=',')
			{
				tmp[i] = *pHead;
				pHead++;
				i++;
				if(i>=5)
					return;
			}
			cmd = atoi(tmp);
			//kal_prompt_trace(MOD_TST,"cmd = %d\r\n",cmd);
			handleMsg(cmd,recv_buff_a);						
			//mmi_set_gps_array_zore();
	}*/
	

}
}
#elif defined(__LHAT_GUANGXIN_COMMON__)

int handleMsg(u8 cmd,char *msg)
{
	char tmp[20] = {0};
	char i=0;
	int data,data2;
	int ret = 0;
	U32 ntimestamp = 0 ;
	char *p=NULL;
	if(!msg)	return -1;
	kal_prompt_trace(MOD_TST,"handleMsg cmd=%d\r\n",cmd);
	switch(cmd)
	{
		case 1:			//ø™À¯
		{
			p = strstr(msg,"b_lock");
			kal_prompt_trace(MOD_TST,"handleMsg p=%s\r\n",p);
			if(p&&p+8)
			{
				p = p+8;
				while(*p!='}')
				{
					tmp[i] = *p;	
					i++;
					p++;
					if(i>=2)
						return;
				}
				data = atoi((char *)tmp);
				/*if((data == 1)&&(ntimestamp<=30))
				{
					kal_prompt_trace(MOD_TST,"open lock\r\n");
					//ø™À¯
					//mmi_set_bike_lock_open_status(KAL_TRUE);
					//mmi_set_bike_lock_close_init_status(KAL_FALSE);
					//lhat_5v_dc_onoff(1);
					//mmi_set_gpio_timer_status();
					//mmi_set_eint2_status(KAL_TRUE);
					mmi_set_bike_lock_status(KAL_TRUE);
				///////////TUDO		
				}
				else
				{
					if(ntimestamp>30)
					{
						kal_prompt_trace(MOD_TST," lock fail time stamp 60 \r\n");
					}
					else
					{
						kal_prompt_trace(MOD_TST," lock fail lock 0 \r\n");
					}
					
				}*/
				kal_prompt_trace(MOD_TST,"b_lock =%d\r\n",data);
				
			}
			i = 0 ;
			memset(tmp,0,sizeof(tmp));
			p = strstr(msg,"time_stamp");
			kal_prompt_trace(MOD_TST,"time_stamp p=%s\r\n",p);
			if(p&&p+12)
			{
				p = p+12;
				kal_prompt_trace(MOD_TST,"time_stamp111 p=%s\r\n",p);
				while(*p!=',')
				{
					tmp[i] = *p;	
					i++;
					p++;
					if(i>=15)
						return;
				}
				data2 = atoi((char *)tmp);
				ntimestamp = mmi_gprs_get_timestamp();
				kal_prompt_trace(MOD_TST,"mmi_gprs_get_timestamp =%d,data2=%d\r\n",ntimestamp,data2);
				ntimestamp = ntimestamp-data2;
				ntimestamp = abs(ntimestamp);
				kal_prompt_trace(MOD_TST,"ntimestamp =%d\r\n",ntimestamp);
				if((data == 1)&&(ntimestamp<=20))
				{
					kal_prompt_trace(MOD_TST,"open lock\r\n");
					//ø™À¯
					//mmi_set_bike_lock_open_status(KAL_TRUE);
					//mmi_set_bike_lock_close_init_status(KAL_FALSE);
					//lhat_5v_dc_onoff(1);
					//mmi_set_gpio_timer_status();
					//mmi_set_eint2_status(KAL_TRUE);
					lhat_5v_dc_onoff(1);
					//lhat_portable_source_gpio(1);
					//mmi_portable_source_open_timer_status();
					mmi_set_gpio_timer_status();
					//b_lock_unclok_alarm_flag = KAL_TRUE ;
					//b_motor_open_flag=KAL_TRUE;
					//lhat_MOTO_IA_gpio(1);
					lhat_MOTO_IB_gpio(1);
					mmi_motor_open_timer();

					mmi_ble_start_open_lock_time();
		
					//mmi_set_bike_lock_status(KAL_TRUE);
				///////////TUDO		
				}
				else
				{
					if(ntimestamp>20)
					{
						kal_prompt_trace(MOD_TST," lock fail time stamp 20 \r\n");
					}
					else
					{
						kal_prompt_trace(MOD_TST," lock fail lock 0 \r\n");
					}
					
				}
			}
			
			
		}
		break;
		case 2:			//–ﬁ∏ƒIP
		{		
			p = strstr(msg,"b_set_ip_flag");
			if(p&&p+15)
			{
				p = p+15;
				while(*p!=',')
				{
					tmp[i] = *p;	
					i++;
					p++;
					if(i>=2)
						return;
				}
				data = atoi((char *)tmp);
				if(data == 1)
				{
					memset(tmp,0,sizeof(tmp));
					i = 0;
					p = strstr(msg,"IP");
					if(p&&p+5)
					{
						p = p+5;
						while(*p!='\"')
						{
							tmp[i] = *p;
							p++;
							i++;
							if(i>=17)
								return ;
						}
						tmp[i] = '\0';			//add by lxj
						kal_prompt_trace(MOD_TST,"ip :%s\r\n",tmp);
						//tudo
						ret = chkip(tmp);
						if(!ret)
						{
							//save
							memset(lhat_device.ip,0,sizeof(lhat_device.ip));	//add by lxj
							memcpy(lhat_device.ip,tmp,strlen(tmp));
							kal_prompt_trace(MOD_TST, "recv IP:%s ",tmp);
						}
						else
						{
							return -1 ;
						}
					}


					memset(tmp,0,sizeof(tmp));
					i = 0;
					p = strstr(msg,"PORT");
					if(p&&p+6)
					{
						p = p+6;
						while(*p!='}')
						{
							tmp[i] = *p;
							p++;
							i++;
							if(i>=7)
								return ;
						}
						data = atoi((char *)tmp);
						kal_prompt_trace(MOD_TST,"PORT :%d\r\n",data);
						
						//tudo
						ret = chkport(data);
						if(!ret)
						{
							//save
							lhat_device.port = data;
							kal_prompt_trace(MOD_TST, "recv port:%d ",data);
						}
						//AlmATPowerReset(MMI_FALSE, 10);
					}
					chkport_ext(lhat_device.port);
					chkip_ext(lhat_device.ip);
					gprs_socket_connect();
				}
			}
			
			//add by lxj
			Clear_NVRAM_Write();
			Restore_NVRAM_Write(0);
		}
		break;	
		case 3:
			{
				p = strstr(msg,"connect");
				kal_prompt_trace(MOD_TST, "recv connect:%s ",p);
				if(p&&p+9)
				{
					p = p+9;
					while(*p!='}')
					{
						tmp[i] = *p;	
						i++;
						p++;
						if(i>=2)
							return;
					}
					data = atoi((char *)tmp);
					kal_prompt_trace(MOD_TST, "recv connect data:%d ",data);
				}
			}
			break ;
			
		defualt:break;
	}
}




void gprs_socket_receive(void)
{
	
	int ret,i=0; 
	char tmp[500] = {0};
	char recv_buff_a[500] = {0};
	int cmd = 0;
	char *p=NULL;

	memset(recv_buff_a, 0, sizeof(recv_buff_a));
	ret = soc_recv(g_socket_id, recv_buff_a, sizeof(recv_buff_a) - 1, 0);
	kal_prompt_trace(MOD_TST, "guangxin=%d, %d, %s", ret, g_receive_times, recv_buff_a);
	kal_prompt_trace(MOD_TST, "%s",  recv_buff_a);
	if(ret == -2 && g_receive_times < 2)
	{
		g_receive_times++;
		StartTimer(GIS_TIMER_ID_4, 600, gprs_socket_receive);
		return;
	}

	if(strstr(recv_buff_a,"LOGIN"))
	{
		if(strstr(recv_buff_a,"OK"))
		{
			g_gprs_login_status_flag=KAL_TRUE;
		}
		else
		{
			g_gprs_login_status_flag=KAL_FALSE;
		}
	}
	/*else if(strstr(recv_buff_a,"RECORD"))
	{
		
	}*/
	else if(strstr(recv_buff_a,"OPEN"))
	{
		u16 posx;
		u8 dx;	
		u8 nindex =0 ;
		u32 temp;
		p = strstr(recv_buff_a,"OPEN");

		// date
		p = strstr(recv_buff_a,"\r\n");
		//posx=NMEA_Comma_Pos(recv_buff_a,1);	
		//posx=lhat_rn_Comma_Pos(recv_buff_a,1);
		kal_prompt_trace(MOD_TST,"OPEN ,p= %s\r\n",p);
		if(p)
		{
			//p = p+posx;
			kal_prompt_trace(MOD_TST,"OPEN p= %s\r\n",p);
			while(*p!=',')
			{
				tmp[i] = *p;
				p++;
				i++;
				if(i>=25)
					return;
			}
			n_data_value = atoi(tmp);
			kal_prompt_trace(MOD_TST,"OPEN  n_data_value=%d\r\n",n_data_value);
		}

		// user id
		p = strstr(recv_buff_a,"\r\n");
		posx=NMEA_Comma_Pos(p,1);	
		//p = strstr(recv_buff_a,"OPEN");
		if(p)
		{
			i=0;
			p = p+posx;
			memset(tmp,0,sizeof(tmp));
			kal_prompt_trace(MOD_TST,"OPEN user id p=%s\r\n",p);
			while(*p!=',')
			{
				tmp[i] = *p;
				p++;
				i++;
				if(i>=60)
				{
					kal_prompt_trace(MOD_TST,"OPEN user id i=%d\r\n",i);
					return;
				}
			}
			//n_user_id = atoi(tmp);
			memcpy(s_user_id,tmp,i);
			//kal_prompt_trace(MOD_TST,"OPEN  tmp=%s,i=%d\r\n",tmp,i);
			kal_prompt_trace(MOD_TST,"OPEN  n_user_id=%s,i=%d\r\n",s_user_id,i);
		}

		// trade no
		p = strstr(recv_buff_a,"\r\n");
		posx=NMEA_Comma_Pos(p,2);	
		//p = strstr(recv_buff_a,"OPEN");
		if(p)
		{
			i=0;
			p = p+posx;
			memset(tmp,0,sizeof(tmp));
			kal_prompt_trace(MOD_TST,"OPEN p= %s\r\n",p);
			while(*p!=',')
			{
				tmp[i] = *p;
				p++;
				i++;
				if(i>=40)
					return;
			}
			//n_trade_no = atoi(tmp);
			//memcpy(s_trade_no,tmp,i);
			kal_prompt_trace(MOD_TST,"OPEN  n_trade_no=%s,i=%d\r\n",s_trade_no,i);
		}
		
		// EncryptData
		p = strstr(recv_buff_a,"\r\n");
		posx=NMEA_Comma_Pos(p,3); 
		//p = strstr(recv_buff_a,"OPEN");
		if(p)
		{
			i=0;
			p = p+posx;
			memset(tmp,0,sizeof(tmp));
			kal_prompt_trace(MOD_TST,"OPEN p= %s\r\n",p);
			while(*p!=',')
			{
				tmp[i] = *p;
				p++;
				i++;
				if(i>=45)
					return;
			}
			//n_trade_no = atoi(tmp);
			memcpy(s_EncryptData,tmp,i);
			kal_prompt_trace(MOD_TST,"OPEN	s_EncryptData=%s,i=%d\r\n",s_EncryptData,i);
		}
		
		// IndexKey
		/*p = strstr(recv_buff_a,"\r\n");
		posx=NMEA_Comma_Pos(p,4); 
		//p = strstr(recv_buff_a,"OPEN");
		if(p)
		{
			i=0;
			p = p+posx;
			memset(tmp,0,sizeof(tmp));
			kal_prompt_trace(MOD_TST,"OPEN p= %s\r\n",p);
			while(*p!=',')
			{
				tmp[i] = *p;
				p++;
				i++;
				if(i>=8)
					return;
			}
			nindex = atoi(tmp);
			kal_prompt_trace(MOD_TST,"OPEN	n_trade_no=%d,i=%d\r\n",nindex,i);
		}*/
		
		//if(strstr(recv_buff_a,"OK"))
		//{
			//g_gprs_open_status_flag=KAL_TRUE;
			lhat_5v_dc_onoff(1);
			mmi_set_gpio_timer_status();
			lhat_MOTO_IB_gpio(1);
			mmi_motor_open_timer();
			mmi_ble_start_open_lock_time();
			kal_prompt_trace(MOD_TST,"OPEN = %s\r\n",recv_buff_a);
		//}
		//else
		//{
		//	g_gprs_open_status_flag=KAL_FALSE;
		//}
	}
	else if(strstr(recv_buff_a,"BMSREPORT"))	// BMS report
	{
		u8 n_type_value = 0 ;
		u8 posx=0;
		u8 n_res_code = 0 ;
		
		// type
		p = strstr(recv_buff_a,"\r\n");
		kal_prompt_trace(MOD_TST,"BMSREPORT ,p= %s\r\n",p);
		if(p)
		{
			while(*p!=',')
			{
				tmp[i] = *p;
				p++;
				i++;
				if(i>=3)
					return;
			}
			n_type_value = atoi(tmp);
			kal_prompt_trace(MOD_TST,"BMSREPORT  n_type_value=%d\r\n",n_type_value);
		}
		
		// ResCode
		p = strstr(recv_buff_a,"\r\n");
		posx=NMEA_Comma_Pos(p,1);	
		if(p)
		{
			i=0;
			p = p+posx;
			memset(tmp,0,sizeof(tmp));
			kal_prompt_trace(MOD_TST,"BMSREPORT ResCode p=%s\r\n",p);
			while(*p!=',')
			{
				tmp[i] = *p;
				p++;
				i++;
				if(i>=6)
				{
					kal_prompt_trace(MOD_TST,"BMSREPORT user id i=%d\r\n",i);
					return;
				}
			}
			n_res_code = atoi(tmp);
			kal_prompt_trace(MOD_TST,"OPEN  BMSREPORT=%s,i=%d\r\n",n_res_code,i);
		}
	}
	else if(strstr(recv_buff_a,"REPORT"))	// …œ±®√¸¡Ó
	{
		if(strstr(recv_buff_a,"OK"))
		{
			kal_prompt_trace(MOD_TST,"REPORT ok = %s\r\n",recv_buff_a);
		}
		else
		{
			kal_prompt_trace(MOD_TST,"REPORT fail = %s\r\n",recv_buff_a);
		}
	}
	else if(strstr(recv_buff_a,"RIDINGMODE"))	//
	{
		u8 n_type_value = 0 ;
		u8 n_runningmode_value=0;
		u8 n_vibrate=0;
		u8 n_indexkey=0;
		char ip_string[]={0};
		u16 n_port =0;
		u8 posx=0;

		// type
		/*p = strstr(recv_buff_a,"\r\n");
		kal_prompt_trace(MOD_TST,"RIDINGMODE ,p= %s\r\n",p);
		if(p)
		{
			while(*p!=',')
			{
				tmp[i] = *p;
				p++;
				i++;
				if(i>=6)
					return;
			}
			n_type_value = atoi(tmp);
			kal_prompt_trace(MOD_TST,"RIDINGMODE  n_type_value=%d\r\n",n_type_value);
		}

		// runningMode
		p = strstr(recv_buff_a,"\r\n");
		posx=NMEA_Comma_Pos(p,1);	
		if(p)
		{
			i=0;
			p = p+posx;
			memset(tmp,0,sizeof(tmp));
			kal_prompt_trace(MOD_TST,"RIDINGMODE runningMode p=%s\r\n",p);
			while(*p!=',')
			{
				tmp[i] = *p;
				p++;
				i++;
				if(i>=6)
				{
					kal_prompt_trace(MOD_TST,"RIDINGMODE runningMode i=%d\r\n",i);
					return;
				}
			}
			n_runningmode_value = atoi(tmp);
			kal_prompt_trace(MOD_TST,"RIDINGMODE  n_runningmode_value=%d,i=%d\r\n",n_runningmode_value,i);
		}*/
		p = strstr(recv_buff_a,"\r\n");
		kal_prompt_trace(MOD_TST,"RIDINGMODE ,p= %s\r\n",p);
		if(p)
		{
			while(*p!=',')
			{
				tmp[i] = *p;
				p++;
				i++;
				if(i>=6)
					return;
			}
			n_runningmode_value = atoi(tmp);
			kal_prompt_trace(MOD_TST,"RIDINGMODE  n_runningmode_value=%d\r\n",n_runningmode_value);
		}

		// EncryptData
		p = strstr(recv_buff_a,"\r\n");
		posx=NMEA_Comma_Pos(p,1); 
		if(p)
		{
			i=0;
			p = p+posx;
			memset(tmp,0,sizeof(tmp));
			kal_prompt_trace(MOD_TST,"RIDINGMODE p= %s\r\n",p);
			while(*p!=',')
			{
				tmp[i] = *p;
				p++;
				i++;
				if(i>=45)
					return;
			}
			memcpy(s_EncryptData,tmp,i);
			kal_prompt_trace(MOD_TST,"RIDINGMODE	s_EncryptData=%s,i=%d\r\n",s_EncryptData,i);
		}

		// IndexKey
		p = strstr(recv_buff_a,"\r\n");
		posx=NMEA_Comma_Pos(p,2);	
		if(p)
		{
			i=0;
			p = p+posx;
			memset(tmp,0,sizeof(tmp));
			kal_prompt_trace(MOD_TST,"RIDINGMODE IndexKey p=%s\r\n",p);
			while(*p!=',')
			{
				tmp[i] = *p;
				p++;
				i++;
				if(i>=6)
				{
					kal_prompt_trace(MOD_TST,"RIDINGMODE IndexKey i=%d\r\n",i);
					return;
				}
			}
			n_indexkey = atoi(tmp);
			kal_prompt_trace(MOD_TST,"RIDINGMODE  IndexKey=%d,i=%d\r\n",n_indexkey,i);
		}
		g_gprs_riding_status_flag=KAL_TRUE;
	}
	else if(strstr(recv_buff_a,"CONFIG"))	// CONFIG
	{
		u16 n_ping_period = 0 ;
		u8 n_gps_period=0;
		u8 n_vibrate=0;
		u8 n_bsms=0;
		char ip_string[]={0};
		u16 n_port =0;
		u8 posx=0;
		
		// ping preiod
		p = strstr(recv_buff_a,"\r\n");
		kal_prompt_trace(MOD_TST,"CONFIG ,p= %s\r\n",p);
		if(p)
		{
			while(*p!=',')
			{
				tmp[i] = *p;
				p++;
				i++;
				if(i>=8)
					return;
			}
			n_ping_period = atoi(tmp);
			kal_prompt_trace(MOD_TST,"CONFIG  n_ping_period=%d\r\n",n_ping_period);
		}
		
		// gps period
		p = strstr(recv_buff_a,"\r\n");
		posx=NMEA_Comma_Pos(p,1);	
		if(p)
		{
			i=0;
			p = p+posx;
			memset(tmp,0,sizeof(tmp));
			kal_prompt_trace(MOD_TST,"CONFIG gps period p=%s\r\n",p);
			while(*p!=',')
			{
				tmp[i] = *p;
				p++;
				i++;
				if(i>=6)
				{
					kal_prompt_trace(MOD_TST,"CONFIG gps period i=%d\r\n",i);
					return;
				}
			}
			n_gps_period = atoi(tmp);
			kal_prompt_trace(MOD_TST,"CONFIG  n_gps_period=%d,i=%d\r\n",n_gps_period,i);
		}

		// bVibrate
		p = strstr(recv_buff_a,"\r\n");
		posx=NMEA_Comma_Pos(p,2);	
		if(p)
		{
			i=0;
			p = p+posx;
			memset(tmp,0,sizeof(tmp));
			kal_prompt_trace(MOD_TST,"CONFIG bVibrate p=%s\r\n",p);
			while(*p!=',')
			{
				tmp[i] = *p;
				p++;
				i++;
				if(i>=6)
				{
					kal_prompt_trace(MOD_TST,"CONFIG bVibrate i=%d\r\n",i);
					return;
				}
			}
			n_vibrate = atoi(tmp);
			kal_prompt_trace(MOD_TST,"CONFIG  n_vibrate=%d,i=%d\r\n",n_vibrate,i);
		}

		// bSMS
		p = strstr(recv_buff_a,"\r\n");
		posx=NMEA_Comma_Pos(p,3);	
		if(p)
		{
			i=0;
			p = p+posx;
			memset(tmp,0,sizeof(tmp));
			kal_prompt_trace(MOD_TST,"CONFIG bSMS p=%s\r\n",p);
			while(*p!=',')
			{
				tmp[i] = *p;
				p++;
				i++;
				if(i>=6)
				{
					kal_prompt_trace(MOD_TST,"CONFIG bSMS i=%d\r\n",i);
					return;
				}
			}
			n_bsms = atoi(tmp);
			kal_prompt_trace(MOD_TST,"CONFIG  n_bsms=%d,i=%d\r\n",n_bsms,i);
		}

		// IP
		p = strstr(recv_buff_a,"\r\n");
		posx=NMEA_Comma_Pos(p,4);	
		if(p)
		{
			i=0;
			p = p+posx;
			memset(tmp,0,sizeof(tmp));
			kal_prompt_trace(MOD_TST,"CONFIG IP p=%s\r\n",p);
			while(*p!=',')
			{
				tmp[i] = *p;
				p++;
				i++;
				if(i>=18)
				{
					kal_prompt_trace(MOD_TST,"CONFIG IP i=%d\r\n",i);
					return;
				}
			}
			tmp[i] = '\0';
			//tudo
			ret = chkip(tmp);
			if(!ret)
			{
				//save
				memset(lhat_device.ip,0,sizeof(lhat_device.ip));	//add by lxj
				memcpy(lhat_device.ip,tmp,strlen(tmp));
				kal_prompt_trace(MOD_TST, "recv IP:%s ",tmp);
			}
			else
			{
				return  ;
			}
		}

		// PORT
		p = strstr(recv_buff_a,"\r\n");
		posx=NMEA_Comma_Pos(p,5);	
		if(p)
		{
			i=0;
			p = p+posx;
			memset(tmp,0,sizeof(tmp));
			kal_prompt_trace(MOD_TST,"CONFIG port p=%s\r\n",p);
			while(*p!=',')
			{
				tmp[i] = *p;
				p++;
				i++;
				if(i>=6)
				{
					kal_prompt_trace(MOD_TST,"CONFIG port i=%d\r\n",i);
					return;
				}
			}
			n_port = atoi(tmp);
			kal_prompt_trace(MOD_TST,"CONFIG  port=%d,i=%d\r\n",n_port,i);
			//add by lxj
			Clear_NVRAM_Write();
			Restore_NVRAM_Write(0);
		}
		g_config_status_flag=KAL_TRUE;
	}
	else if(strstr(recv_buff_a,"GETCONFIG"))	// GETCONFIG
	{
		g_getconfig_status_flag=KAL_TRUE;
	}
	else if(strstr(recv_buff_a,"BREATH"))	// ∆Ô––Ω· ¯…œ±®
	{
		g_breath_heart_flag=KAL_TRUE;
		kal_prompt_trace(MOD_TST,"----BREATH in to 2503\r\n");
	}
	else
	{
		kal_prompt_trace(MOD_TST,"----fail fail = %s\r\n",recv_buff_a);
	}
	
}


#endif

#if defined(__LHAT_SHANGXUN_COMMON__) || defined(__LHAT_GUANGXIN_COMMON__)

void gprs_socket_notify(void * msg)
{
	app_soc_notify_ind_struct * soc_notify = (app_soc_notify_ind_struct *)msg;

	kal_prompt_trace(MOD_TST, "gprs_socket_notify=%d, %d, %d", soc_notify->event_type,
		soc_notify->socket_id, g_socket_id);

	switch (soc_notify->event_type)
	{
		case SOC_READ:
			if(soc_notify->socket_id == g_socket_id)
			{
				gprs_socket_receive();
			}
			break;
		case SOC_CONNECT:
			if(soc_notify->socket_id == g_socket_id)
			{
				//gprs_send_buf_reply(1);
				b_is_connect_tcp_flag = KAL_TRUE ;
				//b_close_service_connect_flag = KAL_FALSE;
			}
			break;
		case SOC_CLOSE:
			kal_prompt_trace(MOD_CC, "TCP∂œø™\n");
			{
				gprs_socket_connect();	
				b_close_service_connect_flag = KAL_TRUE ;
				g_gprs_login_status_flag=KAL_FALSE;
				b_is_connect_tcp_flag = KAL_FALSE ;
				//g_start_to_gps_flag=0;
			}
			break;
		case SOC_WRITE :		// add by dingtian
		default :
			{
				kal_prompt_trace(MOD_TST, "gprs_socket_notify soc_write");
			}
			break ;
			
	}
}

#endif

void gprs_soc_close(void)
{	
    soc_close(g_socket_id);
	soc_shutdown(g_socket_id, SHUT_RDWR);
	kal_prompt_trace(MOD_TST, "gprs_soc_close=%d", g_socket_id);
	g_socket_id = -1;
}

void gprs_soc_close_all(void)
{	
	U8 i = 0 ;

	for (i = 0 ; i <=5 ; i++)
	{
	    soc_close(i);
		soc_shutdown(i, SHUT_RDWR);
		kal_prompt_trace(MOD_TST, "gprs_soc_close_all=%d", i);
		g_socket_id = -1;
	}
}



#if defined(__LHAT_SHANGXUN_COMMON__) || defined(__LHAT_GUANGXIN_COMMON__)	

void gprs_socket_connect(void)
{
	kal_uint8 val = KAL_TRUE;
	kal_int8 soc_ret;

	if(srv_nw_info_get_service_availability(MMI_SIM1) != SRV_NW_INFO_SA_FULL_SERVICE)
		return;
	gprs_soc_close_all();//gprs_soc_close();
	g_start_to_gps_flag=0;
	g_heart_beat = 0;
	gprs_acct_id();
	g_socket_id = soc_create(SOC_PF_INET, SOC_SOCK_STREAM, 0, MOD_MMI, g_acct_id);
	kal_prompt_trace(MOD_TST, "gprs_socket_connect=%d, %d", g_acct_id, g_socket_id);
	if (g_socket_id >= 0)
	{
		soc_setsockopt(g_socket_id, SOC_NBIO, &val, sizeof(val));
		val = SOC_CONNECT | SOC_READ | SOC_WRITE | SOC_CLOSE;
		soc_setsockopt(g_socket_id, SOC_ASYNC, &val, sizeof(val));
	} 
	else
		g_socket_id = -1;
	soc_ret = soc_connect(g_socket_id, &g_ip_addr);

	kal_prompt_trace(MOD_TST, "soc_ret = %d, %d, %d, %d, %d, %d", soc_ret, g_ip_addr.addr[0], g_ip_addr.addr[1],
		g_ip_addr.addr[2], g_ip_addr.addr[3], g_ip_addr.port);
	if(soc_ret >= 0)
		gprs_send_buf_reply(1);
	else if(soc_ret == SOC_WOULDBLOCK)
	{
		SetProtocolEventHandler(gprs_socket_notify, MSG_ID_APP_SOC_NOTIFY_IND);
		//g_heart_beat = 1;	// dddd
	}
	else
		g_start_to_gps_flag = 0 ;
}

void gprs_heartbeat_time(void)
{
	U16 heart_time = 1000;//15000;
	mmi_sim_enum usable_sim = srv_nw_usab_get_one_usable_sim();
	
	StopTimer(GIS_TIMER_ID_2);
	//g_goto_sleep_flag = KAL_FALSE;
	kal_prompt_trace(MOD_TST, "g_heart_beat =%d, usable_sim=%d,g_lhat_imei=%s batt_percent=%d,b_is_connect =%d,g_gps_no_gprs_count=%d", g_heart_beat, usable_sim,g_lhat_imei,g_phone_batt_percent,b_is_connect_tcp_flag,g_gps_no_gprs_count);
	kal_prompt_trace(MOD_TST, "g_start_to_gps_flag =%d, b_is_connect_tcp_flag=%d,send_gps_data_fail_count=%d", g_start_to_gps_flag, b_is_connect_tcp_flag,send_gps_data_fail_count);
	{
		//lhat_gps_power_onoff(1) ;
		//lhat_open_gps();
		
		if(MMI_SIM1 == usable_sim)
		{
			
			if((g_start_to_gps_flag == 1 || g_heart_beat == 0) && (b_is_connect_tcp_flag != KAL_FALSE))//else if(g_heart_beat == 0)// if (g_start_to_gps_flag == 1)
			{
				kal_prompt_trace(MOD_TST, "g_gprs_login_status_flag =%d, g_gprs_record_status_flag=%d,", g_gprs_login_status_flag,g_gprs_record_status_flag);
				if(KAL_FALSE==g_gprs_login_status_flag)
				{
					gprs_send_buf_reply(3);
					heart_time = 1000;
					if (g_heart_beat == 1)
					{
						g_heart_beat = 0 ;
						return ;
					}
					if (g_first_flag == KAL_FALSE)
					{
						n_first_connect_count++;
						if (n_first_connect_count >=16)
						{
							n_first_connect_count = 0 ;
							gprs_socket_connect();
						}
					}
				}
				else if(KAL_TRUE==g_gprs_record_status_flag)
				{
					gprs_send_buf_reply(7);//REPORT
					//gprs_send_buf_reply(1);
					heart_time = 1000;
				}
				else if(KAL_TRUE==g_gprs_riding_status_flag)
				{
					gprs_send_buf_reply(10);//RIDINGMODE
					heart_time = 1000;
				}
				else if(KAL_TRUE==g_getconfig_status_flag)
				{
					gprs_send_buf_reply(9);//RIDINGMODE
					heart_time = 1000;
				}
				else if(KAL_TRUE==g_config_status_flag)
				{
					gprs_send_buf_reply(4);//RIDINGMODE
					heart_time = 1000;
				}
				/*else if(KAL_TRUE==g_gprs_open_status_flag)
				{
					gprs_send_buf_reply(6);// OPEN
					heart_time = 1000;
				}*/
				else if(KAL_TRUE==g_gprs_login_status_flag)
				{
					/*if (g_first_flag == KAL_FALSE)
					{
						n_first_connect_count++;
						if (n_first_connect_count >=16)
						{
							n_first_connect_count = 0 ;
							gprs_socket_connect();
						}
					}*/
				}
				else
				{
					
				}
			}
			else if (g_start_to_gps_flag == 0)//if (g_start_to_gps_flag == 0 || g_heart_beat == 0)
			{
				{
					g_start_to_gps_flag = 1 ;
					//if (KAL_TRUE == mmi_get_bike_lock_open_status())
					{
						if (KAL_FALSE == b_is_connect_tcp_flag)
						{
							gprs_socket_connect();
						}
					}
				}
			}
			else
			{
				if (g_heart_beat == 1)
				{
					// 
					//g_heart_beat = 0 ;
					//g_start_to_gps_flag = 0 ;
					//StopTimer(GIS_TIMER_ID_2);
					return ;
				}
				else if ((b_is_connect_tcp_flag == KAL_TRUE) && (g_heart_beat == 0))
				{
					send_gps_data_fail_count +=1 ;
					if (send_gps_data_fail_count >=8)
					{
						//gprs_send_buf_reply(2);
						g_start_to_gps_flag = 0 ;
						send_gps_data_fail_count = 0 ;
					}
				}
				else
				{
					send_gps_data_fail_count +=1 ;
					gprs_send_buf_reply(3);
					heart_time = 1000;
					if (send_gps_data_fail_count >=15)
					{
						//gprs_send_buf_reply(2);
						g_start_to_gps_flag = 0 ;
						send_gps_data_fail_count = 0 ;
					}
				}
			}
		}
		if(1 == g_riding_flag)
			heart_time = 1000;//15000;//heart_time = 30000;

		kal_prompt_trace(MOD_TST, "g_heart_beat =%d, usable_sim=%d,g_lhat_imei=%s g_phone_batt_percent=%d,g_gps_no_gprs_count=%d", g_heart_beat, usable_sim,g_lhat_imei,g_phone_batt_percent,g_gps_no_gprs_count);
		
		
	}

	{
		g_gps_no_gprs_count +=1 ;

		if (g_gps_no_gprs_count >= 30)
		{
			g_gps_no_gprs_count = 0 ;
			kal_prompt_trace(MOD_TST, "goto sleep mode 111111111");
#if defined(__LHAT_GOTO_SLEEP__)			
			//mmi_gps_power_onoff(0);
			if ((g_first_flag != KAL_FALSE) 		
				&& (KAL_TRUE == mmi_get_bike_lock_close_init_status())
				)
			{
				lhat_gps_power_onoff(0) ;
			}
#endif
			//mdi_gps_uart_close(mdi_get_gps_port(), MDI_GPS_UART_MODE_LOCATION, lhat_gps_callback);
			//if ((KAL_TRUE == mmi_get_bike_lock_open_status()) && (KAL_FALSE== mmi_get_bike_lock_close_init_status()))
			//{
			//	gprs_soc_close();
			//}
			
			StopTimer(GIS_TIMER_ID_10);
			StopTimer(GIS_TIMER_ID_2);
			StopTimer(GIS_TIMER_ID_21);
			srv_backlight_to_sleep_mode();
			{
				//lhat_sleep_handle = L1SM_GetHandle();
				//L1SM_SleepEnable(lhat_sleep_handle);
			}
			mmi_old_latitude_longitude_zero();
			
#if 0//defined(__LHAT_LOSS_POINT_SAVE_TO_SEND__) && defined(__LHAT_HUAYUAN_PROTOCOL__) //&& (!defined(__LHAT_1MIN_GET_GPS_COMMON__))
			if(g_lhat_gps.east_west || g_lhat_gps.north_south)
			{
				char jingdu1[12] = {0}, weidu1[12] = {0};
				kal_int32 test_longitude11 = 0 ;
				kal_int32 test_latitude11  = 0 ;
				kal_int32 test_latitude = 0 ;
				kal_int32 test_longitude  = 0 ;
				char tmp[6] = {0};


				jingdu_change(jingdu1);
				squeeze(jingdu1,'.');
				test_latitude11 = atoi(jingdu1);
				test_latitude  = test_latitude11 / 10 ;
				
				weidu_change(weidu1);
				squeeze(weidu1,'.');
				test_longitude11 = atoi(weidu1);
				test_longitude	= test_longitude11 / 10 ;

				
				if (loss_point_count < 150)
				{
					st_loss_point_gps[loss_point_count].st_longitude = test_longitude ;
					st_loss_point_gps[loss_point_count].st_latitude = test_latitude ;
					sprintf(tmp, "%05.1f", g_lhat_gps.lhat_ground_speed);
					//st_loss_point_gps[loss_point_count].st_speed = tmp ;
					memcpy(st_loss_point_gps[loss_point_count].st_speed,tmp,sizeof(tmp)) ;
					sprintf(tmp, "%03d", (int)g_lhat_gps.lhat_trace_degree);
					//st_loss_point_gps[loss_point_count].st_degree = tmp ;
					memcpy(st_loss_point_gps[loss_point_count].st_degree,tmp,sizeof(tmp));
					//loss_point_gps_data[loss_point_count][4] = 0 ;
					//oss_mileage_array[loss_point_count] =  8;
					loss_point_count ++ ;
				}
				else
				{
					loss_point_count = 149 ;
					
					st_loss_point_gps[loss_point_count].st_longitude = test_longitude ;
					st_loss_point_gps[loss_point_count].st_latitude = test_latitude ;
					sprintf(tmp, "%05.1f", g_lhat_gps.lhat_ground_speed);
					//st_loss_point_gps[loss_point_count].st_speed = tmp ;
					memcpy(st_loss_point_gps[loss_point_count].st_speed,tmp,sizeof(tmp)) ;
					sprintf(tmp, "%03d", (int)g_lhat_gps.lhat_trace_degree);
					//st_loss_point_gps[loss_point_count].st_degree = tmp ;
					memcpy(st_loss_point_gps[loss_point_count].st_degree,tmp,sizeof(tmp));
					//loss_mileage_array[loss_point_count] =  8;
				}
				//save_mileage += CalcDistance(save_longitude,save_latitude,test_longitude,test_latitude);
				//save_longitude = test_longitude ;
				//save_latitude = test_latitude ;
			}
#endif

			//NMEA_set_latitude_longitude_zero();
			memset(&g_lhat_gps, 0, sizeof(g_lhat_gps));
			//g_gps_time_count = 0 ;
			g_start_to_gps_flag = 0 ;
			g_goto_sleep_flag = KAL_TRUE ;
			g_gsensor_moive_flag = 0 ;
			g_heart_beat = 0 ;
			n_gprs_send_fail_count = 0 ;
			send_gps_data_fail_count = 0 ;
			//b_is_connect_tcp_flag = KAL_FALSE ;
			b_send_gps_data_flag = KAL_FALSE ;
			kal_prompt_trace(MOD_TST, "goto sleep mode 2222222222222");		

			//srv_backlight_to_sleep_mode();
			NMEA_set_latitude_longitude_zero();
			loss_point_data_set_latitude_longitude_zero();
			original_data_set_latitude_longitude_zero();
			//save_1min_point_data_set_latitude_longitude_zero();
			//mmi_old_latitude_longitude_zero();
			mmi_set_acc_status_zero();
			mmi_set_warning_status_zero();
			//g_gps_time_count = 0 ;
			//g_start_to_gps_flag = 0 ;
			//g_goto_sleep_flag = KAL_TRUE ;
			//g_gsensor_moive_flag = 0 ;
			g_gps_no_gprs_count = 0 ;
			//n_gprs_send_fail_count = 0 ;
			//send_gps_data_fail_count = 0 ;
			//n_no_connect_to_server_count=0 ;
#if defined(__LHAT_SHANGXUN_COMMON__)||defined(__LHAT_GARQI_COMMON__)	
			//if ((KAL_TRUE == mmi_get_bike_lock_open_status()) && (KAL_FALSE== mmi_get_bike_lock_close_init_status()))
			//{
			//	b_is_connect_tcp_flag = KAL_FALSE ;
			//}
			b_close_service_connect_flag = KAL_FALSE;
			
			if(g_ext_vol_check_flag==KAL_TRUE)
			{
				g_ext_vol_check_flag=KAL_FALSE ;
			}
#else	
			//b_is_connect_tcp_flag = KAL_FALSE ;
#endif
			b_send_gps_data_flag = KAL_FALSE ;


			kal_prompt_trace(MOD_TST, "goto sleep mode 2222222222222");

#if defined(__LHAT_GOTO_SLEEP__)
			//mmi_gps_power_onoff(0);
#if defined(__LHAT_SHANGXUN_COMMON__)||defined(__LHAT_GARQI_COMMON__)		
			if(KAL_TRUE==b_lock_unclok_alarm_flag)
			{
				//lhat_gps_power_onoff(0) ;
				//b_lock_unclok_alarm_flag=KAL_FALSE;
			}
			else
			{
				lhat_gps_power_onoff(0) ;
				b_lock_unclok_alarm_flag=KAL_FALSE;
			}
#else		
			if ((g_first_flag != KAL_FALSE) 
			&& (KAL_TRUE == mmi_get_bike_lock_close_init_status() && KAL_FALSE == mmi_get_bike_lock_open_status())				
			)
			{
				lhat_gps_power_onoff(0) ;
			}
#endif
#endif
			n_no_connect_to_server_count++;
			if((n_no_connect_to_server_count>=30)&&(KAL_TRUE == mmi_get_bike_lock_close_init_status()))
			{
				AlmATPowerReset(MMI_FALSE, 5);
			}

			return ;
		}
	}
	
	StartTimer(GIS_TIMER_ID_2, heart_time, gprs_heartbeat_time);
}

#endif

#if defined(__LHAT_HUAYUAN__) || defined(__LHAT_HUAYUAN_LIANHAI__) || defined(__LHAT_HUAYUAN_FEIMAOTUI__) || defined(__LHAT_TIANQIN_XINGMAI_COMMON__)
extern void bmt_get_adc_vbat_ext(void);
extern void uem_get_aux_adc_value(void);
extern void Restore_NVRAM_Read(void);
//extern void lhat_5v_dc_onoff(U8 onoff);

void mmi_gprs_heartbeat_time_handle(void)
{
	//kal_uint32 ret_data = 0 ;
	U16 heart_time = 1000;//15000;
	
	StopTimer(GIS_TIMER_ID_11);

	if (g_goto_sleep_flag == KAL_TRUE)
	{
		g_gps_time_sleep_count +=1 ;
		if (g_gsensor_moive_flag == 0)
		{
			g_gsensor_moive_flag = bma250_get_moive_flag();//g_gsensor_moive_flag = bma250_get_moive_flag();
		}
		kal_prompt_trace(MOD_TST, "g_goto_sleep_flag=%d,g_gsensor_moive_flag=%d", g_goto_sleep_flag,g_gsensor_moive_flag);

		if ((g_gps_time_sleep_count >= nTimeFre)
#if defined(__LHAT_RECEIVE_SMS__)			
			|| (mmi_grps_get_tcp_status() == KAL_TRUE)
#endif			
			)
		{
#if 0//def GPRS_TEST
			//gprs_socket_receive();
			bmt_get_adc_vbat_ext();
			//uem_get_aux_adc_value();
			//Restore_NVRAM_Read();
			//lhat_5v_dc_onoff(1);
			//lhat_buzzer_onoff(1);
			//lhat_led_ext_onoff(1);
			//lhat_pmc_onoff(1);
			//srv_sms_test_sms_pror();
			//srv_sms_test_sms_pror_ext();
#endif
			g_gps_time_sleep_count = 0 ;
			bma250_set_gsensor_shake_count_zero();
			if ((g_gsensor_moive_flag == 1) || (g_gsensor_moive_flag == 2)
#if defined(__LHAT_RECEIVE_SMS__)			
				|| (mmi_grps_get_tcp_status() == KAL_TRUE)
#endif						
				)
			{
				g_goto_sleep_flag = KAL_FALSE ;
#if defined(__LHAT_GOTO_SLEEP__)				
				lhat_gps_power_onoff(1) ;
#endif
				lhat_open_gps();
				
				StopTimer(GIS_TIMER_ID_2);
				StartTimer(GIS_TIMER_ID_2, 1000, gprs_heartbeat_time);
#ifdef __LHAT_LBS__				
				//lhat_get_lbs_infor();
#endif
			}
		}
	}
	
	StartTimer(GIS_TIMER_ID_11, heart_time, mmi_gprs_heartbeat_time_handle);
}

#elif defined(__LHAT_SHANGXUN_COMMON__) || defined(__LHAT_GUANGXIN_COMMON__)

extern kal_bool bma250_get_vibrate_flag(void);
void mmi_gprs_heartbeat_time_handle(void)
{
	//kal_uint32 ret_data = 0 ;
	unsigned int vbat_ex = 0;
	U16 heart_time = 1000;//15000;
	u32 vol;
	double adc;
	kal_bool	b_bike_unlock_flag = KAL_FALSE ;
	kal_bool	b_bike_lock_flag = KAL_FALSE ;
	
	StopTimer(GIS_TIMER_ID_11);
	vbat_ex  = Current_Voltage_ex(DCL_AUX_ADC_CHANNEL);
	kal_prompt_trace(MOD_TST, " voltage = %d,bike_lock_open_status =%d,lock_close_init_=%d,g_goto_sleep_flag=%d,g_gps_time_sleep_count=%d",vbat_ex,mmi_get_bike_lock_open_status(),mmi_get_bike_lock_close_init_status(),g_goto_sleep_flag,g_gps_time_sleep_count);
	//VIB_WARNDET();
	//NOPOWER_WARNDET();
	mmi_heart_gps_updata_status();
	//mmi_get_gps_updata_status();
	//lhat_get_blue_ext_gpio_status();

	//if ((KAL_TRUE == mmi_get_bike_lock_open_status()) && (KAL_FALSE== mmi_get_bike_lock_close_init_status()))
	if (g_goto_sleep_flag != KAL_FALSE)
	{	// bike unlock
		g_gps_time_sleep_count +=1 ;
		g_gps_long_no_send_count +=1 ;
		g_gprs_no_send_count=0;
		//srv_backlight_to_sleep_mode();

		if (KAL_TRUE == mmi_get_bike_lock_open_status())
		{
			if (g_gsensor_moive_flag == 0)
			{
				//g_gsensor_moive_flag = bma250_get_moive_flag();
				//VIB_WARNDET();
			}
		}
		else
		{
			//g_gsensor_moive_flag = bma250_get_moive_flag();
		}
	
		kal_prompt_trace(MOD_TST, "g_goto_sleep_flag=%d,g_gsensor_moive_flag=%d", g_goto_sleep_flag,g_gsensor_moive_flag);
		b_bike_unlock_flag = mmi_get_eint2_status();
		b_bike_lock_flag = mmi_get_bike_lock_status();

#if defined(__LHAT_WIDE_VOLTAGE_DEVICE__) || defined(__LHAT_SHANGXUN_COMMON__) || defined(__LHAT_GUANGXIN_COMMON__)
		if ( 
#if defined(__LHAT_LOCK_OPEN_GPS_COMMON__)			
			 ( g_gps_time_sleep_count % 5 == 0 )
			&& ((g_gsensor_moive_flag == 1) || (g_gsensor_moive_flag == 2))
			&& (KAL_TRUE == mmi_get_bike_lock_open_status())
#else
			( g_gps_time_sleep_count % 2 == 0 )
			//&& ((g_gsensor_moive_flag == 1) || (g_gsensor_moive_flag == 2))
			//&& (KAL_TRUE == mmi_get_bike_lock_open_status())
			&&(KAL_TRUE == mmi_get_bike_lock_close_init_status())
			&&(KAL_TRUE==b_lock_unclok_alarm_flag)
#endif
			)
		{
			if(g_lhat_gps.east_west || g_lhat_gps.north_south)
			{
				char jingdu1[11] = {0}, weidu1[11] = {0};
				char tmp[6] = {0};
				kal_int32 test_longitude11 = 0 ;
				kal_int32 test_latitude11  = 0 ;
				kal_int32 test_latitude = 0 ;
				kal_int32 test_longitude  = 0 ;

				jingdu_change(jingdu1);
				//squeeze(jingdu1,'.');
				//test_latitude11 = atoi(jingdu1);
				//test_latitude  = test_latitude11 / 10 ;
				
				weidu_change(weidu1);
				//squeeze(weidu1,'.');
				//test_longitude11 = atoi(weidu1);
				//test_longitude  = test_longitude11 / 10 ;

				if (n_gps_count < 150)
				{
					
					memcpy(st_shangxun_point_gps[n_gps_count].st_longitude,weidu1,sizeof(weidu1));
					memcpy(st_shangxun_point_gps[n_gps_count].st_latitude,jingdu1,sizeof(jingdu1));

					kal_prompt_trace(MOD_TST, "st_shangxun_point_gps[n_gps_count].st_longitude =%s,st_shangxun_point_gps[n_gps_count].st_latitude=%s", st_shangxun_point_gps[n_gps_count].st_longitude,st_shangxun_point_gps[n_gps_count].st_latitude);
					n_gps_count +=1 ;
				}
				else
				{
					n_gps_count = 149 ;
					
					memcpy(st_shangxun_point_gps[n_gps_count].st_longitude,weidu1,sizeof(weidu1));
					memcpy(st_shangxun_point_gps[n_gps_count].st_latitude,jingdu1,sizeof(jingdu1));
				}
				memcpy(back_gps_longitude,weidu1,sizeof(weidu1));
				memcpy(back_gps_latitude,jingdu1,sizeof(jingdu1));
			}
			if((KAL_TRUE==b_lock_unclok_alarm_flag) && (g_first_flag != KAL_FALSE))
			{
				n_lock_unclok_alarm_count++;
				if(n_lock_unclok_alarm_count>20)
				{
					n_lock_unclok_alarm_count=0;
					b_lock_unclok_alarm_flag=KAL_FALSE;
					lhat_gps_power_onoff(0) ;
					mmi_gprs_socket_close_handle();
				}
			}
		}
		
#endif


		if (/*(g_gps_time_sleep_count >= nTimeFre)
#if defined(__LHAT_RECEIVE_SMS__)			
			|| (mmi_grps_get_tcp_status() == KAL_TRUE)
#endif	
			&& (g_goto_sleep_flag == KAL_TRUE)
			&& (KAL_TRUE == mmi_get_bike_lock_open_status())
			|| */(KAL_TRUE == b_bike_unlock_flag)
			|| (KAL_TRUE == b_bike_lock_flag)
			//|| ((KAL_TRUE == b_close_service_connect_flag) && (KAL_TRUE == mmi_get_bike_lock_close_init_status()))
			//|| ((KAL_TRUE == b_close_service_connect_flag) && (KAL_TRUE == mmi_get_bike_lock_open_status()))
			|| ((g_gps_long_no_send_count >= 180) && (KAL_TRUE == mmi_get_bike_lock_open_status()))
			|| ((g_gps_long_no_send_count >= 21600) && (KAL_TRUE == mmi_get_bike_lock_close_init_status()))
			//|| (KAL_TRUE == g_ble_ext_gpio_in_flag)
			|| ( (g_gsensor_moive_flag == 1) && (KAL_TRUE == mmi_get_bike_lock_close_init_status()))
			//|| (g_ext_vol_check_flag==KAL_TRUE)
			|| ((KAL_TRUE==b_lock_unclok_alarm_flag)&&(n_gps_count!=0))
			|| (KAL_FALSE==g_gprs_login_status_flag)
			|| (KAL_TRUE==g_gprs_riding_status_flag)
			|| (KAL_TRUE==g_getconfig_status_flag)
			|| (KAL_TRUE==g_config_status_flag)
			)			
		{
			g_gps_time_sleep_count = 0 ;
			bma250_set_gsensor_shake_count_zero();

			if (
#if defined(__LHAT_LOCK_OPEN_GPS_COMMON__)				
				((g_gsensor_moive_flag == 1 || g_gsensor_moive_flag == 2) && (KAL_TRUE == mmi_get_bike_lock_open_status()))
				||
#endif				
				(KAL_TRUE== b_bike_unlock_flag) 
				|| (KAL_TRUE== b_bike_lock_flag) 
				//|| ((KAL_TRUE == b_close_service_connect_flag) && (KAL_TRUE == mmi_get_bike_lock_close_init_status()))
				//|| ((KAL_TRUE == b_close_service_connect_flag) && (KAL_TRUE == mmi_get_bike_lock_open_status()))
				|| ((g_gps_long_no_send_count >= 180) && (KAL_TRUE == mmi_get_bike_lock_open_status()))
				|| ((g_gps_long_no_send_count >= 21600) && (KAL_TRUE == mmi_get_bike_lock_close_init_status()))
				//|| (KAL_TRUE == g_ble_ext_gpio_in_flag)
				//|| ( (g_gsensor_moive_flag == 1) && (KAL_TRUE == mmi_get_bike_lock_close_init_status()))
				//|| (g_ext_vol_check_flag==KAL_TRUE)
				|| ((KAL_TRUE==b_lock_unclok_alarm_flag)&&(n_gps_count!=0))
				|| (KAL_FALSE==g_gprs_login_status_flag)
				|| (KAL_TRUE==g_gprs_riding_status_flag)
				|| (KAL_TRUE==g_getconfig_status_flag)
				|| (KAL_TRUE==g_config_status_flag)
				)
			{
				g_goto_sleep_flag = KAL_FALSE;

				if ((KAL_TRUE== b_bike_unlock_flag)
					||(KAL_TRUE== b_bike_lock_flag)
					//||(KAL_TRUE == g_ble_ext_gpio_in_flag)
					//||((g_gsensor_moive_flag == 1) && (KAL_TRUE == mmi_get_bike_lock_close_init_status()))
					|| ((g_gps_long_no_send_count >= 180) && (KAL_TRUE == mmi_get_bike_lock_open_status()))
					|| ((g_gps_long_no_send_count >= 21600) && (KAL_TRUE == mmi_get_bike_lock_close_init_status()))
					//|| ((KAL_TRUE==b_lock_unclok_alarm_flag)&&(n_gps_count!=0))
					)
					{
#if defined(__LHAT_GOTO_SLEEP__)				
						lhat_gps_power_onoff(1) ;
#endif
						lhat_open_gps();
					} 
				//kal_prompt_trace(MOD_TST, "g_gps_long_no_send_count=%d,mmi_get_bike_lock_open_status()=%d",g_gps_long_no_send_count,mmi_get_bike_lock_open_status());
				if(((g_gps_long_no_send_count >= 180) && (KAL_TRUE == mmi_get_bike_lock_open_status())&&(KAL_TRUE==g_gprs_login_status_flag))
					|| (KAL_TRUE== b_bike_lock_flag)
					|| (KAL_TRUE== b_bike_unlock_flag)
					)
				{
					g_gprs_record_status_flag=KAL_TRUE;
				}

				if((g_gsensor_moive_flag == 1) && (KAL_TRUE == mmi_get_bike_lock_close_init_status()))
				{
					bma250_set_gsensor_shake_count_zero();
				}
				
				StopTimer(GIS_TIMER_ID_2);
				StartTimer(GIS_TIMER_ID_2, 1000, gprs_heartbeat_time);
				g_gps_long_no_send_count = 0 ;
				/*if (KAL_TRUE == g_ble_ext_gpio_in_flag)
				{
					g_ble_ext_gpio_in_flag  = KAL_FALSE ;
					b_ble_to_lock_in_flag = KAL_TRUE ;
					mmi_set_bike_lock_status(KAL_TRUE);
					kal_prompt_trace(MOD_TST, "bbbbbbbbbbbbbbb=%d", g_ble_ext_gpio_in_flag);
				}*/
				if(KAL_TRUE==b_lock_unclok_alarm_flag)
				{
					b_lock_unclok_alarm_flag=KAL_FALSE;
				}
			}
		}
#if 0//defined(__LHAT_RECEIVE_SMS__)	
		else if ((mmi_grps_get_tcp_status() == KAL_TRUE))
		{
			g_gps_time_sleep_count = 0 ;
			g_gps_long_no_send_count = 0 ;
			bma250_set_gsensor_shake_count_zero();
				g_goto_sleep_flag = KAL_FALSE ;
#if defined(__LHAT_GOTO_SLEEP__)				
				lhat_gps_power_onoff(1) ;
#endif
				lhat_open_gps();
				
				StopTimer(GIS_TIMER_ID_2);
				StartTimer(GIS_TIMER_ID_2, 1000, gprs_heartbeat_time);
#ifdef __LHAT_LBS__				
				//lhat_get_lbs_infor();
#endif
		}
#endif		
#ifdef __LHAT_LBS__				
			//lhat_get_lbs_infor();
#endif

	}
	else
	{
		// bike lock 
		g_gprs_no_send_count++;
		if(g_gprs_no_send_count>=60)
		{
			mmi_gprs_socket_close_handle();
		}
		kal_prompt_trace(MOD_TST, "aaaaaaaaaaaaaaaaaaaaaaa=%d,n_no_connect_to_server_count=%d", g_goto_sleep_flag,n_no_connect_to_server_count);
	}
	
	StartTimer(GIS_TIMER_ID_11, heart_time, mmi_gprs_heartbeat_time_handle);
}

#endif


void gprs_gps_init(void)
{
	//if (g_first_flag == 0 )
	//{
	//	lhat_open_gps();	
	//	g_first_flag = 1 ;
	//}
		
	//gprs_heartbeat_time();
	//lhat_open_gps();
	//sample_gps_open();
	//g_riding_flag = 1 ;
	//gprs_open_close_gps(0);
	//mmi_parse_guard_data(pch);
	//lhat_open_gps();
	//gprs_socket_receive();
	//if (g_first_flag == 0)
	//{
		//gprs_test_socket_receive();
	//	g_first_flag = 1 ;
	//}
	
}

unsigned int mmi_get_warning_status(void)
{
	return uploadInfo.alarm ;
}

void mmi_set_warning_status_zero(void)
{
	//uploadInfo.alarm_upload = uploadInfo.alarm;
	uploadInfo.alarm = 0;	
}

//ADCºÏ≤‚
unsigned int adc2vol(kal_uint16 adcVoltage)
{
	unsigned int voltage = 0;
	//float tmp = (float)adcVoltage*3.3;
	//kal_prompt_trace(MOD_TST, "+++++++measureVoltage tmp = %f ++++++",tmp);
	//voltage = 8*adcVoltage;		
	//voltage = 27*adcVoltage;			//2.8/1024 x10000

#if  defined(__LHAT_SHANGXUN_COMMON__) || defined(__LHAT_GUANGXIN_COMMON__)
	voltage = 54*adcVoltage;		//add by lxj
#else
	voltage = 177*adcVoltage;			//2.8 1024 3m 47k
#endif	
	//kal_prompt_trace(MOD_TST, "+++++++measureVoltage = %d ++++++",adcVoltage);
	//kal_prompt_trace(MOD_TST, "+++++++adc_adc2vol = %d ++++++",voltage);
	return voltage;
}
unsigned int measure_Voltage(kal_uint8 chann)
{
	kal_uint16 adcvalue = 0;
	adcvalue = ADC_GetData(chann);
   	return adc2vol(adcvalue);
}
unsigned int Current_Voltage_ex(DCL_ADC_CHANNEL_TYPE_ENUM ch)
{
	unsigned int voltage = 0;
	kal_uint8 adc_channel;
   	DCL_HANDLE adc_handle;
   	ADC_CTRL_GET_PHYSICAL_CHANNEL_T adc_ch;
	//kal_prompt_trace(MOD_TST, "+++++++Current_Voltage_ex voltage = %d ++++++",voltage);
   	adc_handle = DclSADC_Open(DCL_ADC, FLAGS_NONE);
   	if(adc_handle == DCL_HANDLE_INVALID)
   	{
    	ASSERT(0);   
   	}
   	adc_ch.u2AdcName = ch;
   	DclSADC_Control(adc_handle, ADC_CMD_GET_CHANNEL, (DCL_CTRL_DATA_T *)&adc_ch); 
	adc_channel = adc_ch.u1AdcPhyCh;
	voltage = measure_Voltage(adc_channel);
    DclSADC_Close(adc_handle);
	return voltage;
}


//end
#if 1//defined(__LHAT_HUAYUAN_SHANDONG__)



//÷–∂œ
//const char buzzer_enable_pin = 	33;//36;		//GPIO_PORT_36;			//ok
//const char led_enable_pin = 32;//35;			//GPIO_PORT_35;			//ok
//const char pcm_enable_pin = 48;			//GPIO_PORT_48;			//ok
//const char moto_enable_pin = 0;			//GPIO_PORT_0;			

const char LIMIT_EINT_NO = 15;//19;//GPIO36 //6; //gpio7 		----	eint6
const char BLE_IN_EINT_NO = 1;//12;	//gpio14 	---- eint 12
const char Key_EINT_NO = 19;
//const char Key_EINT_NO = 12;////19;//15;	//GPIO25//17;	//gpio34    -------	eint17


U8 BLE_Polarity = LEVEL_HIGH;
U8 KEY_Polarity = LEVEL_HIGH;//LEVEL_HIGH;//LEVEL_LOW;
U8 LIMIT_Polarity = LEVEL_HIGH;//LEVEL_LOW;


/*
void MOTORenable(int on)
{
	if(on)
		GPIO_WriteIO(1, moto_enable_pin);
	else
		GPIO_WriteIO(0, moto_enable_pin);		
}

void Buzzerenable(int on)
{
	if(on)
		GPIO_WriteIO(1, buzzer_enable_pin);
	else
		GPIO_WriteIO(0, buzzer_enable_pin);		
}
void Ledenable(int on)
{
	if(on)
		GPIO_WriteIO(1, led_enable_pin);
	else
		GPIO_WriteIO(0, led_enable_pin);		
}


void pcmenable(int on)
{
	GPIO_ModeSetup(pcm_enable_pin, 0);
	GPIO_InitIO(0, pcm_enable_pin);
	if(on)
		GPIO_WriteIO(1, pcm_enable_pin);
	else
		GPIO_WriteIO(0, pcm_enable_pin);		

}
*/
/*
static void ACC_EINT_CB(void)
{

	kal_prompt_trace(MOD_TST, "++++++ACC_EINT_CB+++++++");
	if (ACC_Polarity == LEVEL_HIGH)
	{
		//¥Úª∆Ù∂Ø
		mmi_set_acc_status_value(2) ;
		ACC_Polarity = LEVEL_LOW;
		EINT_Registration(ACC_EINT_NO, KAL_TRUE, ACC_Polarity, ACC_EINT_CB, KAL_FALSE);
		EINT_Set_Sensitivity(ACC_EINT_NO, KAL_FALSE);
		EINT_SW_Debounce_Modify(ACC_EINT_NO,10);
		//if(lhat_device.alarm_set&1<<WARN_MOVE)
		//	uploadInfo.alarm_cur |=1<<WARN_MOVE;
		//else
		//	uploadInfo.alarm_cur &=~1<<WARN_MOVE;	
	}
	else
	{
		//œ®ª
		mmi_set_acc_status_value(3) ;
		ACC_Polarity = LEVEL_HIGH;
		EINT_Registration(ACC_EINT_NO, KAL_TRUE,ACC_Polarity, ACC_EINT_CB, KAL_FALSE);
		EINT_Set_Sensitivity(ACC_EINT_NO, KAL_FALSE);
		EINT_SW_Debounce_Modify(ACC_EINT_NO,10);	
   }   
   	EINT_UnMask(ACC_EINT_NO);

}
*/

static void KEY_EINT_CB(void)
{

	if (KEY_Polarity == LEVEL_HIGH)
	{
		kal_prompt_trace(MOD_TST, "++++++KEY_EINT_CB+++++++ high");
		KEY_Polarity =  LEVEL_LOW;
		EINT_Registration(Key_EINT_NO, KAL_TRUE, KEY_Polarity, KEY_EINT_CB, KAL_FALSE);
		EINT_Set_Sensitivity(Key_EINT_NO, LEVEL_SENSITIVE);
		EINT_SW_Debounce_Modify(Key_EINT_NO,10);// 32768Hz, set debounce time
		//EINT_Set_Polarity(Key_EINT_NO, KAL_FALSE);
		//EINT_Set_HW_Debounce(Key_EINT_NO, 2);
		//mmi_set_eint2_status(KAL_TRUE);
		mmi_set_bike_lock_open_status(KAL_TRUE);
		mmi_set_bike_lock_close_init_status(KAL_FALSE);
		mmi_set_bike_lock_status(KAL_TRUE);
		StopTimer(GIS_TIMER_ID_31);
	}
	else
	{
		kal_prompt_trace(MOD_TST, "++++++KEY_EINT_CB+++++++ low");
		KEY_Polarity = LEVEL_HIGH;
		EINT_Registration(Key_EINT_NO, KAL_TRUE, KEY_Polarity, KEY_EINT_CB, KAL_FALSE);
		EINT_Set_Sensitivity(Key_EINT_NO, LEVEL_SENSITIVE);
		EINT_SW_Debounce_Modify(Key_EINT_NO,10);// 32768Hz, set debounce time
		//EINT_Set_Polarity(Key_EINT_NO, KAL_FALSE);
		//EINT_Set_HW_Debounce(Key_EINT_NO, 2);
		//mmi_set_eint2_status(KAL_TRUE);
		//lhat_5v_dc_onoff(0);
		//mmi_set_bike_lock_status(KAL_TRUE);
		mmi_set_eint2_status(KAL_TRUE);
		//mmi_set_gpio_timer_status();
		mmi_set_bike_lock_open_status(KAL_FALSE);
		mmi_set_bike_lock_close_init_status(KAL_TRUE);
		//lhat_portable_source_gpio(0);
		//mmi_portable_source_close_timer_status();

	}  
	EINT_UnMask(Key_EINT_NO);
}

/*	static void KEY_EINT_CB(void)
	{
	
		if (KEY_Polarity == LEVEL_HIGH)
		{
			kal_prompt_trace(MOD_TST, "++++++KEY_EINT_CB+++++++ high");
			KEY_Polarity =	LEVEL_LOW;
			//EINT_Registration(Key_EINT_NO, KAL_TRUE, KEY_Polarity, KEY_EINT_CB, KAL_FALSE);
			//EINT_Set_Sensitivity(Key_EINT_NO, LEVEL_SENSITIVE);
			//EINT_SW_Debounce_Modify(Key_EINT_NO,10);// 32768Hz, set debounce time
			EINT_Set_Polarity(Key_EINT_NO, KEY_Polarity);
			//EINT_Set_HW_Debounce(Key_EINT_NO, 2);
			//i_set_eint2_status(KAL_TRUE);
			mmi_set_bike_lock_open_status(KAL_TRUE);
			mmi_set_bike_lock_close_init_status(KAL_FALSE);
			mmi_set_bike_lock_status(KAL_TRUE);
			StopTimer(GIS_TIMER_ID_31);
		}
		else
		{
			kal_prompt_trace(MOD_TST, "++++++KEY_EINT_CB+++++++ low");
			KEY_Polarity = LEVEL_HIGH;
			//EINT_Registration(Key_EINT_NO, KAL_TRUE, KEY_Polarity, KEY_EINT_CB, KAL_FALSE);
			//EINT_Set_Sensitivity(Key_EINT_NO, LEVEL_SENSITIVE);
			//EINT_SW_Debounce_Modify(Key_EINT_NO,10);// 32768Hz, set debounce time
			EINT_Set_Polarity(Key_EINT_NO, KEY_Polarity);
			//EINT_Set_HW_Debounce(Key_EINT_NO, 2);
			//i_set_eint2_status(KAL_TRUE);
			//at_5v_dc_onoff(0);
			//i_set_bike_lock_status(KAL_TRUE);
			mmi_set_eint2_status(KAL_TRUE);
			//i_set_gpio_timer_status();
			mmi_set_bike_lock_open_status(KAL_FALSE);
			mmi_set_bike_lock_close_init_status(KAL_TRUE);
			//at_portable_source_gpio(0);
			//i_portable_source_close_timer_status();
	
		}  
		//EINT_UnMask(Key_EINT_NO);
	}
*/

/*static void KEY_EINT_CB(void)
{
	//
	
	
	if (KEY_Polarity == LEVEL_HIGH)
	{
		kal_prompt_trace(MOD_TST, "++++++KEY_EINT_CB+++++++ high");
		KEY_Polarity =	LEVEL_LOW;
		EINT_Registration(Key_EINT_NO, KAL_TRUE, KEY_Polarity, KEY_EINT_CB, KAL_FALSE);
		EINT_Set_Sensitivity(Key_EINT_NO, LEVEL_SENSITIVE);
		EINT_SW_Debounce_Modify(Key_EINT_NO,10);// 32768Hz, set debounce time
		//EINT_Set_Polarity(Key_EINT_NO, KAL_FALSE);
		//EINT_Set_HW_Debounce(Key_EINT_NO, 2);
		//mmi_set_eint2_status(KAL_TRUE);
		mmi_set_bike_lock_open_status(KAL_TRUE);
		mmi_set_bike_lock_close_init_status(KAL_FALSE);
		mmi_set_bike_lock_status(KAL_TRUE);
		StopTimer(GIS_TIMER_ID_31);
	}
	else
	{
		kal_prompt_trace(MOD_TST, "++++++KEY_EINT_CB+++++++ low");
		KEY_Polarity = LEVEL_HIGH;
		EINT_Registration(Key_EINT_NO, KAL_TRUE, KEY_Polarity, KEY_EINT_CB, KAL_FALSE);
		EINT_Set_Sensitivity(Key_EINT_NO, LEVEL_SENSITIVE);
		EINT_SW_Debounce_Modify(Key_EINT_NO,10);// 32768Hz, set debounce time
		//EINT_Set_Polarity(Key_EINT_NO, KAL_FALSE);
		//EINT_Set_HW_Debounce(Key_EINT_NO, 2);
		//mmi_set_eint2_status(KAL_TRUE);
		//lhat_5v_dc_onoff(0);
		//mmi_set_bike_lock_status(KAL_TRUE);
		mmi_set_eint2_status(KAL_TRUE);
		//mmi_set_gpio_timer_status();
		mmi_set_bike_lock_open_status(KAL_FALSE);
		mmi_set_bike_lock_close_init_status(KAL_TRUE);
		//lhat_portable_source_gpio(0);
		//mmi_portable_source_close_timer_status();

		//lhat_5v_dc_onoff(1);
		//lhat_MOTO_IB_gpio(1);
		//mmi_motor_open_timer();
		
	}  
	EINT_UnMask(Key_EINT_NO);
}
*/
static void LIMIT_EINT_CB(void)
{
	//kal_prompt_trace(MOD_TST, "++++++LIMIT_EINT_CB+++++++");
	/*if(!GPIO_ReadIO(LIMIT_EINT_NO))
	{
		MOTORenable(0);
		Ledenable(1);
		Buzzerenable(1);
	}
	else
	{
		//MOTORenable(0);
		Ledenable(0);
		Buzzerenable(0);	
	}*/
	if (LIMIT_Polarity == LEVEL_HIGH)
	{
		kal_prompt_trace(MOD_TST, "++++++LIMIT_EINT_CB+++++++ high");
		LIMIT_Polarity =  LEVEL_LOW;
		EINT_Registration(LIMIT_EINT_NO, KAL_TRUE, LIMIT_Polarity, LIMIT_EINT_CB, KAL_FALSE);
		EINT_Set_Sensitivity(LIMIT_EINT_NO, LEVEL_SENSITIVE);
		EINT_SW_Debounce_Modify(LIMIT_EINT_NO,1);// 32768Hz, set debounce time
		//EINT_Set_Polarity(LIMIT_EINT_NO, KAL_FALSE);
		//EINT_Set_HW_Debounce(LIMIT_EINT_NO, 2);
		//lhat_5v_dc_onoff(0);
		//mmi_set_eint2_status(KAL_FALSE);
		//mmi_set_bike_lock_open_status(KAL_FALSE);
		//mmi_set_bike_lock_close_init_status(KAL_TRUE);
		
	}
	else
	{
		kal_prompt_trace(MOD_TST, "++++++LIMIT_EINT_CB+++++++ low");
		LIMIT_Polarity = LEVEL_HIGH;
		EINT_Registration(LIMIT_EINT_NO, KAL_TRUE, LIMIT_Polarity, LIMIT_EINT_CB, KAL_FALSE);
		EINT_Set_Sensitivity(LIMIT_EINT_NO, LEVEL_SENSITIVE);
		EINT_SW_Debounce_Modify(LIMIT_EINT_NO,1);// 32768Hz, set debounce time
		//EINT_Set_Polarity(LIMIT_EINT_NO, KAL_FALSE);
		//EINT_Set_HW_Debounce(LIMIT_EINT_NO, 2);
		//lhat_5v_dc_onoff(0);
		//b_motor_open_flag=KAL_FALSE;
		//StopTimer(GIS_TIMER_ID_29);
		//mmi_set_eint2_status(KAL_TRUE);
		//mmi_set_bike_lock_open_status(KAL_TRUE);
		//mmi_set_bike_lock_close_init_status(KAL_FALSE);
		lhat_5v_dc_onoff(1);
		//lhat_MOTO_IB_gpio(1);
		lhat_MOTO_IA_gpio(1);
		mmi_motor_open_timer();
		
	} 
	EINT_UnMask(LIMIT_EINT_NO);

}

static void BLE_IN_EINT_CB(void)
{
	if (LIMIT_Polarity == LEVEL_HIGH)
	{
		kal_prompt_trace(MOD_TST, "++++++BLE_IN_EINT_CB+++++++ high");
		LIMIT_Polarity =  LEVEL_LOW;
		EINT_Registration(BLE_IN_EINT_NO, KAL_TRUE, LIMIT_Polarity, LIMIT_EINT_CB, KAL_FALSE);
		EINT_Set_Sensitivity(BLE_IN_EINT_NO, LEVEL_SENSITIVE);
		EINT_SW_Debounce_Modify(BLE_IN_EINT_NO,10);// 32768Hz, set debounce time
		mmi_set_bike_lock_open_status(KAL_TRUE);
		mmi_set_bike_lock_close_init_status(KAL_FALSE);
		
	}
	else
	{
		kal_prompt_trace(MOD_TST, "++++++BLE_IN_EINT_CB+++++++ low");
		LIMIT_Polarity = LEVEL_HIGH;
		EINT_Registration(BLE_IN_EINT_NO, KAL_TRUE, LIMIT_Polarity, LIMIT_EINT_CB, KAL_FALSE);
		EINT_Set_Sensitivity(BLE_IN_EINT_NO, LEVEL_SENSITIVE);
		EINT_SW_Debounce_Modify(BLE_IN_EINT_NO,10);// 32768Hz, set debounce time
		
	} 
	EINT_UnMask(BLE_IN_EINT_NO);

}

extern void mmi_lmc_get_iccid_req(void);

void mmi_init_led_buzzer_gpio(void)
{
	//lhat_led_ext_onoff(0);
	lhat_buzzer_onoff(0);
}

void IO_Init(void)
{
//	EDGE_SENSITIVE
/*	GPIO_ModeSetup(moto_enable_pin, 0);
	GPIO_InitIO(0, moto_enable_pin);

	GPIO_ModeSetup(buzzer_enable_pin, 0);
	GPIO_InitIO(0, buzzer_enable_pin);
	
	GPIO_ModeSetup(led_enable_pin, 0);
	GPIO_InitIO(0, led_enable_pin);
*/
	/*GPIO_ModeSetup(36, 0);
	GPIO_InitIO(1, 36);
	GPIO_WriteIO(0, 36);

	
	GPIO_ModeSetup(35, 0);
	GPIO_InitIO(1, 35);
	GPIO_WriteIO(0, 35);
*/
	//lhat_led_ext_onoff(0);
	//lhat_buzzer_onoff(0);

	
	kal_prompt_trace(MOD_TST, "++++++IO_Init,EINT_Registration+++++++");


// 	EINT_Mask(LS_EINT_NO);
//	EINT_SW_Debounce_Modify(LS_EINT_NO, 10);
//	EINT_Registration(LS_EINT_NO, KAL_TRUE, KAL_FALSE, limit_switch_eint_hdlr, KAL_FALSE);
//	EINT_Set_Sensitivity(LS_EINT_NO, KAL_TRUE);		//false:level int,true:edge
//	EINT_Set_Polarity(LS_EINT_NO, KAL_FALSE);
//	EINT_Set_HW_Debounce(LS_EINT_NO, 2);



#if defined(__LHAT_HUAYUAN_SHANDONG__) || defined(__LHAT_SHANGXUN_COMMON__) || defined(__LHAT_GUANGXIN_COMMON__)
	EINT_Mask(LIMIT_EINT_NO);
	EINT_Registration(LIMIT_EINT_NO, KAL_TRUE, LIMIT_Polarity, LIMIT_EINT_CB, KAL_FALSE);	//œ¬Ωµ—ÿ¥•∑¢
	EINT_Set_Sensitivity(LIMIT_EINT_NO, EDGE_SENSITIVE);		//±ﬂ—ÿ¥•∑¢
	EINT_SW_Debounce_Modify(LIMIT_EINT_NO,0);	
	EINT_Set_Polarity(LIMIT_EINT_NO, KAL_FALSE);
	EINT_Set_HW_Debounce(LIMIT_EINT_NO, 0);
////
	/*EINT_Mask(Key_EINT_NO);
	EINT_Registration(Key_EINT_NO, KAL_TRUE, KEY_Polarity, KEY_EINT_CB, KAL_FALSE);
	EINT_Set_Sensitivity(Key_EINT_NO, LEVEL_SENSITIVE);
	EINT_SW_Debounce_Modify(Key_EINT_NO,10);// 32768Hz, set debounce time
	EINT_Set_Polarity(Key_EINT_NO, KAL_FALSE);
	EINT_Set_HW_Debounce(Key_EINT_NO, 2);*/
#endif
/*	EINT_Mask(BLE_IN_EINT_NO);
	EINT_Registration(BLE_IN_EINT_NO, KAL_TRUE, BLE_Polarity, BLE_IN_EINT_CB, KAL_FALSE);
	EINT_Set_Sensitivity(BLE_IN_EINT_NO, LEVEL_SENSITIVE);
	EINT_SW_Debounce_Modify(BLE_IN_EINT_NO,10);// 32768Hz, set debounce time	
	EINT_Set_Polarity(BLE_IN_EINT_NO, KAL_FALSE);
	EINT_Set_HW_Debounce(BLE_IN_EINT_NO, 2);
*/
	

	//mmi_lmc_get_iccid_req();

}


void IO_Init_ext(void)
{
	/*GPIO_ModeSetup(46, 0);
	GPIO_InitIO(1, 46);
	GPIO_WriteIO(1, 46);
	GPIO_ModeSetup(9, 0);
	GPIO_InitIO(1, 9);
	GPIO_WriteIO(1, 9);
	*/
	
	GPIO_ModeSetup(Key_EINT_NO, 2); 
	GPIO_PullSelSetup(Key_EINT_NO, KAL_TRUE);
	GPIO_PullenSetup(Key_EINT_NO, KAL_TRUE);
	/*
	GPIO_ModeSetup(20, 0);
	GPIO_InitIO(1, 20);
	GPIO_WriteIO(0, 20);

	GPIO_ModeSetup(0, 0);
	GPIO_InitIO(1, 0);
	GPIO_WriteIO(0, 0);

	GPIO_ModeSetup(1, 0);
	GPIO_InitIO(1, 1);
	GPIO_WriteIO(0, 1);

	GPIO_ModeSetup(25, 0);
	GPIO_InitIO(1,25);
	GPIO_WriteIO(0, 25);

	GPIO_ModeSetup(5, 0);
	GPIO_InitIO(1,5);
	GPIO_WriteIO(0, 5);

	GPIO_ModeSetup(4, 0);
	GPIO_InitIO(1,4);
	GPIO_WriteIO(0, 4);

	GPIO_ModeSetup(8, 0);
	GPIO_InitIO(1,8);
	GPIO_WriteIO(0, 8);

	GPIO_ModeSetup(9, 0);
	GPIO_InitIO(1,9);
	GPIO_WriteIO(0, 9);

	//GPIO_ModeSetup(36, 0);
	//GPIO_InitIO(1,36);
	//GPIO_WriteIO(0, 36);

	GPIO_ModeSetup(32, 0);
	GPIO_InitIO(1,32);
	GPIO_WriteIO(0, 32);

	GPIO_ModeSetup(35, 0);
	GPIO_InitIO(1,35);
	GPIO_WriteIO(0, 35);

	GPIO_ModeSetup(31, 0);
	GPIO_InitIO(1,31);
	GPIO_WriteIO(0, 31);

	GPIO_ModeSetup(33, 0);
	GPIO_InitIO(1,33);
	GPIO_WriteIO(0, 33);

	GPIO_ModeSetup(34, 0);
	GPIO_InitIO(1,34);
	GPIO_WriteIO(0, 34);
	

	GPIO_ModeSetup(47, 0);
	GPIO_InitIO(1,47);
	GPIO_WriteIO(0, 47);

	GPIO_ModeSetup(50, 0);
	GPIO_InitIO(1,50);
	GPIO_WriteIO(0, 50);

	GPIO_ModeSetup(10, 0);
	GPIO_InitIO(1,10);
	GPIO_WriteIO(0, 10);

	GPIO_ModeSetup(27, 0);
	GPIO_InitIO(1,27);
	GPIO_WriteIO(0, 27);

	GPIO_ModeSetup(28, 0);
	GPIO_InitIO(1,28);
	GPIO_WriteIO(0, 28);

	GPIO_ModeSetup(29, 0);
	GPIO_InitIO(1,29);
	GPIO_WriteIO(0, 29);

	GPIO_ModeSetup(43, 0);
	GPIO_InitIO(1,43);
	GPIO_WriteIO(0, 43);

	GPIO_ModeSetup(44, 0);
	GPIO_InitIO(1,44);
	GPIO_WriteIO(0, 44);
	*/

	kal_prompt_trace(MOD_TST, "++++++IO_Init_ext,EINT_Registration+++++++");

#if defined(__LHAT_HUAYUAN_SHANDONG__) || defined(__LHAT_SHANGXUN_COMMON__) || defined(__LHAT_GUANGXIN_COMMON__)
////
#if 1
	EINT_Mask(Key_EINT_NO);
	//EINT_Registration(Key_EINT_NO, KAL_TRUE, KEY_Polarity, KEY_EINT_CB, KAL_TRUE);
	EINT_Registration(Key_EINT_NO, KAL_TRUE, KEY_Polarity, KEY_EINT_CB, KAL_FALSE);
	EINT_Set_Sensitivity(Key_EINT_NO, LEVEL_SENSITIVE);
	EINT_SW_Debounce_Modify(Key_EINT_NO,2);// 32768Hz, set debounce time
	EINT_Set_Polarity(Key_EINT_NO, KAL_FALSE);
	EINT_Set_HW_Debounce(Key_EINT_NO, 2);
#else
	EINT_Mask(Key_EINT_NO);
	EINT_Registration(Key_EINT_NO, KAL_TRUE, KEY_Polarity, KEY_EINT_CB, KAL_TRUE);
	//EINT_Registration(Key_EINT_NO, KAL_TRUE, KEY_Polarity, KEY_EINT_CB, KAL_FALSE);
	EINT_Set_Sensitivity(Key_EINT_NO, LEVEL_SENSITIVE);
	EINT_SW_Debounce_Modify(Key_EINT_NO,2);// 32768Hz, set debounce time
	//EINT_Set_Polarity(Key_EINT_NO, KAL_FALSE);
	//EINT_Set_HW_Debounce(Key_EINT_NO, 2);


#endif
	
#endif

}

void mmi_gprs_socket_close_handle(void)
{
	//g_heart_beat = 0;
	//gprs_socket_receive();
	g_first_flag = KAL_TRUE ;
#if defined( __LHAT_HUAYUAN__) || defined(__LHAT_HUAYUAN_PROTOCOL__) || defined(__LHAT_HUAYUAN_QILI__) 	|| defined(__LHAT_HUAYUAN_LIANHAI__) || defined(__LHAT_HUAYUAN_ANTAI__) || defined(__LHAT_HUAYUAN_FEIMAOTUI__) || defined(__LHAT_HUAYUAN_SHANDONG__) || defined(__LHAT_TIANQIN_XINGMAI_COMMON__) || defined(__LHAT_TREE_YEAR_COMMON__) || defined(__LHAT_SHANGXUN_COMMON__) || defined(__LHAT_GUANGXIN_COMMON__)
	kal_prompt_trace(MOD_TST, "goto sleep mode 111111111");

	//mdi_gps_uart_close(mdi_get_gps_port(), MDI_GPS_UART_MODE_LOCATION, lhat_gps_callback);
#if defined(__LHAT_SHANGXUN_COMMON__) || defined(__LHAT_GUANGXIN_COMMON__)
	//if ((KAL_TRUE == mmi_get_bike_lock_open_status()) && (KAL_FALSE== mmi_get_bike_lock_close_init_status()))
	//{
	//	gprs_soc_close();
	//}
#else
	gprs_soc_close();
#endif
	StopTimer(GIS_TIMER_ID_10);
	StopTimer(GIS_TIMER_ID_2);
	StopTimer(GIS_TIMER_ID_21);
	//StopTimer(GIS_TIMER_ID_11);//dingtian
	srv_backlight_to_sleep_mode();
	{
		//lhat_sleep_handle = L1SM_GetHandle();
		//L1SM_SleepEnable(lhat_sleep_handle);
	}
	NMEA_set_latitude_longitude_zero();
	//loss_point_data_set_latitude_longitude_zero();
	//original_data_set_latitude_longitude_zero();
	//save_1min_point_data_set_latitude_longitude_zero();
	mmi_old_latitude_longitude_zero();
	mmi_set_acc_status_zero();
	mmi_set_warning_status_zero();
	//g_gps_time_count = 0 ;
	//g_start_to_gps_flag = 0 ;
	g_goto_sleep_flag = KAL_TRUE ;
	g_gsensor_moive_flag = 0 ;
	g_gps_no_gprs_count = 0 ;
	n_gprs_send_fail_count = 0 ;
	send_gps_data_fail_count = 0 ;
	n_no_connect_to_server_count=0 ;
#if defined(__LHAT_SHANGXUN_COMMON__) || defined(__LHAT_GUANGXIN_COMMON__)	
	//if ((KAL_TRUE == mmi_get_bike_lock_open_status()) && (KAL_FALSE== mmi_get_bike_lock_close_init_status()))
	//{
	//	b_is_connect_tcp_flag = KAL_FALSE ;
	//}
	
	b_close_service_connect_flag = KAL_FALSE;
	if(KAL_TRUE == mmi_get_eint2_status())
	{
		mmi_set_eint2_status(KAL_FALSE);
		mmi_set_bike_lock_open_status(KAL_FALSE);
		mmi_set_bike_lock_close_init_status(KAL_TRUE);
		//lhat_portable_source_gpio(0);
		//mmi_portable_source_close_timer_status();
		b_lock_unclok_alarm_flag = KAL_TRUE ;
	}
	if(KAL_TRUE == mmi_get_bike_lock_status())
	{
		mmi_set_bike_lock_status(KAL_FALSE);
		
		//ø™À¯
		mmi_set_bike_lock_open_status(KAL_TRUE);
		mmi_set_bike_lock_close_init_status(KAL_FALSE);
		//lhat_5v_dc_onoff(1);
		//mmi_portable_source_open_timer_status();
		//mmi_set_gpio_timer_status();
		//b_lock_unclok_alarm_flag = KAL_TRUE ;
		b_motor_open_flag=KAL_TRUE;
		b_lock_unclok_alarm_flag=KAL_FALSE;
		//lhat_MOTO_IA_gpio(1);
		//mmi_motor_open_timer();
	}
	/*if(KAL_TRUE == b_ble_to_lock_in_flag)
	{
		b_ble_to_lock_in_flag = KAL_FALSE ;
		mmi_set_ble_ouput_timer_status();
		//ø™À¯
		mmi_set_bike_lock_open_status(KAL_TRUE);
		mmi_set_bike_lock_close_init_status(KAL_FALSE);
		lhat_5v_dc_onoff(1);
		//lhat_portable_source_gpio(1);
		mmi_portable_source_open_timer_status();
		mmi_set_gpio_timer_status();
		//mmi_set_eint2_status(KAL_TRUE);
		//mmi_set_bike_lock_status(KAL_TRUE);
		b_lock_unclok_alarm_flag = KAL_TRUE ;
		b_motor_open_flag=KAL_TRUE;
		lhat_MOTO_IA_gpio(1);
		mmi_motor_open_timer();
		
	}*/
	if(8 == mmi_get_gps_updata_status())
	{
		mmi_set_gpio_warning_timer_status();
		//b_lock_unclok_alarm_flag = KAL_TRUE ;
	}
	if(g_ext_vol_check_flag==KAL_TRUE)
	{
		g_ext_vol_check_flag=KAL_FALSE ;
		//mmi_set_gpio_timer_status();
		//mmi_set_gpio_warning_timer_status();
	}
	if(KAL_TRUE==g_gprs_record_status_flag)
	{
		g_gprs_record_status_flag=KAL_FALSE;
	}
	if(KAL_TRUE==g_gprs_riding_status_flag)
	{
		g_gprs_riding_status_flag=KAL_FALSE;
	}
	if(KAL_TRUE==g_getconfig_status_flag)
	{
		g_getconfig_status_flag=KAL_FALSE;
	}
	if(KAL_TRUE==g_config_status_flag)
	{
		g_config_status_flag=KAL_FALSE;
	}
	if(g_get_ble_mac_addr_nv_flag==KAL_TRUE)
	{
		S16 error; 
		//Clear_NVRAM_Write();
		//Restore_NVRAM_Write(0);
		WriteRecord(NVRAM_LHAT_BLE_MAC_BIT_LID,1,&BLE_MAC,sizeof(BLE_MAC),&error);
		g_get_ble_mac_addr_nv_flag=KAL_FALSE;
		kal_prompt_trace(MOD_TST, "********g_get_ble_mac_addr_nv_flag=%d****BLE_MAC.ble_mac=%s",g_get_ble_mac_addr_nv_flag,BLE_MAC.ble_mac);
	}
#else	
	//b_is_connect_tcp_flag = KAL_FALSE ;
#endif
	b_send_gps_data_flag = KAL_FALSE ;


	kal_prompt_trace(MOD_TST, "goto sleep mode 2222222222222");

#if defined(__LHAT_GOTO_SLEEP__)
	//mmi_gps_power_onoff(0);
#if defined(__LHAT_SHANGXUN_COMMON__) || defined(__LHAT_GUANGXIN_COMMON__)	
#if defined(__LHAT_LOCK_OPEN_GPS_COMMON__)	
	if ((g_first_flag != KAL_FALSE) 
	&& (KAL_TRUE == mmi_get_bike_lock_close_init_status() && KAL_FALSE == mmi_get_bike_lock_open_status())				
	)
	{
		lhat_gps_power_onoff(0) ;
	}
#else
	if(KAL_TRUE==b_lock_unclok_alarm_flag)
	{
		//lhat_gps_power_onoff(0) ;
		//b_lock_unclok_alarm_flag=KAL_FALSE;
	}
	else
	{
		lhat_gps_power_onoff(0) ;
		b_lock_unclok_alarm_flag=KAL_FALSE;
	}
#endif	
#else		
	if ((g_first_flag != KAL_FALSE) 
	&& (KAL_TRUE == mmi_get_bike_lock_close_init_status() && KAL_FALSE == mmi_get_bike_lock_open_status())				
	)
	{
		lhat_gps_power_onoff(0) ;
	}
#endif
#endif

#endif
}

kal_bool mmi_get_alarm_trigger_status(void)
{
	return b_alarm_trigger_flag;
}

void mmi_get_alarm_trigger_zero(void)
{
	b_alarm_trigger_flag = KAL_FALSE ;
}

//void mmi_set_alarm_trigger_status(kal_uint32 nnstatus)
//{
//	b_alarm_trigger_flag = KAL_TRUE ;
//}

void mmi_gprs_alarm_trigger_switch(void)
{
	kal_prompt_trace(MOD_TST, "VIB warning=%d,NOPOWER warning=%d,Disassemble warning=%d", (((lhat_device.alarm_set-90000000)%1000000)/100000),((lhat_device.alarm_set-90000000)/1000000),(((lhat_device.alarm_set-90000000)%1000)/100 ));
	if (((lhat_device.alarm_set-90000000)%1000000)/100000 == 1)	// VIB warning
	{
		if ((1 == g_gsensor_moive_flag) || (2 == g_gsensor_moive_flag))
		{
			b_alarm_trigger_flag = KAL_TRUE ;
			uploadInfo.alarm += 100000 ;
			kal_prompt_trace(MOD_TST, "lhat_device.alarm_set=%d,b_alarm_trigger_flag=%d", lhat_device.alarm_set,b_alarm_trigger_flag);
			//return ;
		}
	}
	if (((lhat_device.alarm_set-90000000)/1000000) == 1)	// NOPOWER warning
	{
		unsigned int voltage = 0;
		voltage = Current_Voltage_ex(DCL_AUX_ADC_CHANNEL);
		if(voltage <= 0)
		{
			b_alarm_trigger_flag = KAL_TRUE ;
			uploadInfo.alarm += 1000000 ;
			//return ;
		}
		else
		{
			b_alarm_trigger_flag = KAL_FALSE ;
		}
		kal_prompt_trace(MOD_TST, "voltage=%d,b_alarm_trigger_flag=%d", voltage,b_alarm_trigger_flag);
	}
	if (((lhat_device.alarm_set-90000000)%1000)/100 == 1)	// Disassemble warning
	{
		//uploadInfo.alarm = 10000 ;
		//b_alarm_trigger_flag = KAL_TRUE ;
		//return ;
		
	}
	if (KAL_TRUE == b_alarm_trigger_flag)
	{
		return ;
	}
	
	b_alarm_trigger_flag = KAL_FALSE;
}

void mmi_gprs_get_rtc_24_hour_status(void)
{
	applib_time_struct curr_time;
	GetDateTime(&curr_time);

	if ((curr_time.nHour == 23) && (curr_time.nMin >=58))
	{
		save_mileage =  0 ;
	}
}
#endif


#if __UART3_SUPPORT__
#define	MORSUN_GPS_MAX_READ_LENGTH 2048
kal_uint8 Morsun_gps_input_data[MORSUN_GPS_MAX_READ_LENGTH];
static u8 BLE_FLAG = 0;
kal_uint8 n_ble_send_imei_count = 0 ;

void mmi_get_ble_mac_addr(void)
{
	//memcpy(BLE_MAC,lhat_handle.num,sizeof(lhat_device.num));
}
extern void UART_dafault_tx_cb(UART_PORT port);

void buffer_process(char* buffer,unsigned int len)
{

	char imei[100] = {0};
	if(len==0)
		return;
	else
	{
		char *p=NULL ;
		char tmp[30] = {0};
		char i=0;
		int cmd,data2;
		int len1=0;
		S16 error; 
		
		//TUDO 
		kal_prompt_trace(MOD_TST, "++++++++UART3 RECEIVE BUFF :%s++++++++", buffer);
		/*if(strstr(buffer,g_lhat_imei+7))
		{
			BLE_FLAG = 1;
		}
		else
			return ;
		*/
		p = strstr(buffer,"tart");
		if(p)
		{
			p = strstr(buffer,"cmd");
			p = p+4;
			while(*p!=',')
			{
				tmp[i] = *p;	
				i++;
				p++;
				if(i>=4)
					return;
			}
			cmd = atoi((char *)tmp);
			kal_prompt_trace(MOD_TST, "++++++++uart1 buffer_process cmd=%d++++++++",cmd);
			switch(cmd)
			{
				case 1:
					{
					}
					break;
				case 2:
					{
						/*p = strstr(buffer,"addr1");
						p = p+6;
						while(*p!=',')
						{
							tmp[i] = *p;	
							i++;
							p++;
							if(i>=4)
								return;
						}
						data2 = atoi((char *)tmp);
						kal_prompt_trace(MOD_TST, "++++++++uart1 buffer_process addr1=%02x++++++++",data2);
						memcpy(BLE_MAC,data2,2);
						//BLE_MAC[0] = data2;
						
						p = strstr(buffer,"addr2");
						p = p+6;
						while(*p!=',')
						{
							tmp[i] = *p;	
							i++;
							p++;
							if(i>=4)
								return;
						}
						data2 = atoi((char *)tmp);
						kal_prompt_trace(MOD_TST, "++++++++uart1 buffer_process addr2=%02x++++++++",data2);
						//BLE_MAC[1] = data2;
						memcpy(BLE_MAC+2,data2,2);

						p = strstr(buffer,"addr3");
						p = p+6;
						while(*p!=',')
						{
							tmp[i] = *p;	
							i++;
							p++;
							if(i>=4)
								return;
						}
						data2 = atoi((char *)tmp);
						kal_prompt_trace(MOD_TST, "++++++++uart1 buffer_process addr3=%02x++++++++",data2);
						//BLE_MAC[2] = data2;
						memcpy(BLE_MAC+4,data2,2);

						p = strstr(buffer,"addr4");
						p = p+6;
						while(*p!=',')
						{
							tmp[i] = *p;	
							i++;
							p++;
							if(i>=4)
								return;
						}
						data2 = atoi((char *)tmp);
						kal_prompt_trace(MOD_TST, "++++++++uart1 buffer_process addr4=%02x++++++++",data2);
						//BLE_MAC[3] = data2;
						memcpy(BLE_MAC+6,data2,2);

						p = strstr(buffer,"addr5");
						p = p+6;
						while(*p!=',')
						{
							tmp[i] = *p;	
							i++;
							p++;
							if(i>=4)
								return;
						}
						data2 = atoi((char *)tmp);
						kal_prompt_trace(MOD_TST, "++++++++uart1 buffer_process addr5=%02x++++++++",data2);
						//BLE_MAC[4] = data2;
						memcpy(BLE_MAC+8,data2,2);

						p = strstr(buffer,"addr6");
						p = p+6;
						while(*p!=',')
						{
							tmp[i] = *p;	
							i++;
							p++;
							if(i>=4)
								return;
						}
						data2 = atoi((char *)tmp);
						kal_prompt_trace(MOD_TST, "++++++++uart1 buffer_process addr6=%02x++++++++",data2);
						//BLE_MAC[5] = data2;
						memcpy(BLE_MAC+10,data2,2);
						kal_prompt_trace(MOD_TST, "++++++++uart1 buffer_process BLE_MAC=%s++++++++",BLE_MAC);
						*/
						i=0;
						memset(tmp,0,sizeof(tmp));
						p = strstr(buffer,"mac");
						p = p+4;
						kal_prompt_trace(MOD_TST, "++++++++uart1 buffer_process p=%s++++++++",p);
						while(*p!=',')
						{
							tmp[i] = *p;	
							i++;
							p++;
							if(i>=16)
								return;
						}
						kal_prompt_trace(MOD_TST, "++++++++uart1 buffer_process tmp=%s,i=%d++++++++",tmp,i);
						//memset(lhat_device.num,0,sizeof(lhat_device.num));
						//memcpy(lhat_device.num,tmp,i);
						//memcpy(BLE_MAC,tmp,i);
						len1 += sprintf(encrypt_key,"%s",encrypt_key_base);
						len1 += sprintf(encrypt_key+len1,"%s",tmp);
						//strcpy(encrypt_key,tmp);
						kal_prompt_trace(MOD_TST, "++++++++uart1 buffer_process encrypt_key=%s++++++++",encrypt_key);
						//memcpy(lhat_handle.num,tmp,i);
						//memset(lhat_handle.num,0,sizeof(lhat_handle.num));
						//sprintf(lhat_handle.num,"%s",tmp);
						memset(lhat_device.num,0,sizeof(lhat_device.num));
						sprintf(lhat_device.num,"%s",tmp);
						//memset(BLE_MAC.ble_mac,0,sizeof(BLE_MAC.ble_mac));
						//memset(lhat_device.ble_mac,0,sizeof(lhat_device.ble_mac));
						sprintf(BLE_MAC.ble_mac,"%s",tmp);
						//sprintf(lhat_device.ble_mac,"%s",tmp);
						kal_prompt_trace(MOD_TST, "++++++++uart1 buffer_process lhat_device.num=%s+++++++",BLE_MAC.ble_mac);
						U_Close(uart_port3, U_GetOwnerID(uart_port3));
						//g_save_nv_flag = KAL_TRUE;
						
						//memcpy(lhat_device.num,BLE_MAC,sizeof(lhat_device.num));
						//Clear_NVRAM_Write();
						//Restore_NVRAM_Write(0);
						//mmi_encrypt_key_init();
						
						//WriteRecord(NVRAM_LHAT_BLE_MAC_BIT_LID,1,tmp,sizeof(lhat_device.ble_mac),&error); 
						//WriteRecord(NVRAM_LHAT_BLE_MAC_BIT_LID,1,(void*)&BLE_MAC,sizeof(BLE_MAC),&error);
						//WriteRecord(NVRAM_LHAT_BLE_MAC_BIT_LID,1,&BLE_MAC,sizeof(BLE_MAC),&error);
						//WriteRecord(NVRAM_LHAT_BLE_MAC_BIT_LID,1,"1234567890",12,&error);
						
					}
					break;
				case 3:
					{
						char *buf="abcdefg";
						U_PutBytes_VFIFO(uart_port3,buf,strlen(buf),MOD_MMI);	
					}
					break ;
				default:
					{
					}
					break;	
			}
		}
		else
		{
			kal_prompt_trace(MOD_TST, "++++++++UART3 RECEIVE BUFF :fail++++++++");
		}
		
	}
}


void Uart3_readyToRead_ind_hdler(UART_PORT port)
{
	U32 len = 0;
	memset(Morsun_gps_input_data,0,sizeof(Morsun_gps_input_data));
	{
		len = U_GetBytes_VFIFO(
			uart_port3,
			(kal_uint8*)Morsun_gps_input_data,
			MORSUN_GPS_MAX_READ_LENGTH,
			NULL,
			MOD_MMI);
		buffer_process((S8 *)Morsun_gps_input_data, len);

	}
}

void UART3_dafault_tx_cb(UART_PORT port)
{
	
}

void Uart_Init(void)
{
#ifdef WIN32
#else
	module_type module_id = MOD_MMI;
	kal_bool bSuccess = KAL_FALSE;
	char buffer[100];
	kal_uint8 zf_uart_sleep_handle;

	UARTDCBStruct dcb =
	{
		UART_BAUD_115200, //init
		len_8,      //dataBits;
		sb_1,       // stopBits;
		pa_none,    // parity; 
		fc_none,
		0x11,   // xonChar;
		0x13,   // xoffChar;
		KAL_TRUE
	};
	
	//_zf_++,20160528,÷ªΩ¯»Î“ª¥Œ£¨∑¿÷πÀ¿ª˙	    
	//zf_uart_sleep_handle = L1SM_GetHandle();	
	//L1SM_SleepDisable(zf_uart_sleep_handle);	
	//_zf_--,20160528,÷ªΩ¯»Î“ª¥Œ£¨∑¿÷πÀ¿ª˙
	
	U_Close(uart_port3, U_GetOwnerID(uart_port3));
	UART_HWInit(uart_port3);
	dcb.baud = (UART_baudrate)UART_BAUD_115200;
	U_SetOwner (uart_port3, module_id);
	U_SetDCBConfig(uart_port3, &dcb, module_id);
	bSuccess = U_Open(uart_port3, module_id);
	
	kal_prompt_trace(MOD_TST, "++++++++UART3 Init  :%d++++++++", bSuccess);
	ASSERT(bSuccess == KAL_TRUE);
	U_Register_TX_cb(uart_port3, module_id, UART3_dafault_tx_cb);
	U_Register_RX_cb(uart_port3, module_id, Uart3_readyToRead_ind_hdler);
	UART_TurnOnPower(uart_port3, KAL_TRUE);
#endif
}

void UART3_send_data_to_ble(kal_uint8 cmd)
{
	char buf[200]={0};
	kal_uint16 buff_len=0;

	switch(cmd)
	{
		case 11:
			{
				buff_len += sprintf(buf+buff_len,"start,cmd:%d,",cmd);	
				buff_len += sprintf(buf+buff_len,"%s,",s_BMS_control_data_buff);	
				buff_len += sprintf(buf+buff_len,"end");	
			}
			break ;
		case 12:
			{
				buff_len += sprintf(buf+buff_len,"start,cmd:%d,",cmd);	
				buff_len += sprintf(buf+buff_len,"%s,",s_BMS_infor_data_buff);	
				buff_len += sprintf(buf+buff_len,"end");	
			}
			break ;
		case 14:
			{
				buff_len += sprintf(buf+buff_len,"start,cmd:%d,",cmd);	
				buff_len += sprintf(buf+buff_len,"%s,",s_BMS_ID_data_buff);	
				buff_len += sprintf(buf+buff_len,"end");	
			}
			break ;	
		default:
			break ;
	}

	U_PutBytes_VFIFO(uart_port3,buf,strlen(buf),MOD_MMI);
}

void UART3_send_ble_to_get_mac_addr(void)
{
	kal_uint8 buf[6] = {0xaa,0x02,0x0d,0x0a};
	U_PutBytes_VFIFO(uart_port3,buf,strlen(buf),MOD_MMI);
}

void mmi_ble_mac_addr_ok(void)
{
	//char *BLE_MAC_old = "101112131415";

	//if(strstr(BLE_MAC,"101112131415"))
	//{
	//	kal_prompt_trace(MOD_TST, "mmi_ble_mac_addr_ok=%s", (BLE_MAC));
	//}
	//else
	//{
	//	UART3_send_ble_to_get_mac_addr();
	//	kal_prompt_trace(MOD_TST, "mmi_ble_mac_addr_fail=%s", (BLE_MAC));
	//}
}

void mmi_uart3_get_ble_mac_addr_timer_handle(void)
{
	StopTimer(GIS_TIMER_ID_24);
	mmi_ble_mac_addr_ok();
	if(n_ble_send_imei_count>=5)
	{
		n_ble_send_imei_count=0;
		return ;
	}
	StartTimer(GIS_TIMER_ID_24,5000,mmi_uart3_get_ble_mac_addr_timer_handle);
}

void mmi_uart3_get_ble_mac_addr_timer(void)
{
	//StopTimer(GIS_TIMER_ID_24);
	//mmi_get_ble_mac_addr();
	//mmi_ble_mac_addr_ok();
	//StartTimer(GIS_TIMER_ID_24,5000,mmi_uart3_get_ble_mac_addr_timer_handle);
#if defined(__UART3_SUPPORT__)			
	//Uart1_Init();
#endif	
}

/*void perun_trace(char *fmt, ...) 
{ 
	int i = 0;
	va_list list; 
	char buf[2048]; 
	char *ptr = (char*)buf; 
	memset(buf, 0, sizeof(buf)); 
	strcpy(ptr, "[Perun_trace]: "); 
	ptr += strlen("[Perun_trace]: "); 
	va_start(list, fmt); 
	vsprintf(ptr, fmt, list); 
	va_end(list); 
	//strcat(buf, "\r\n"); 
	buf[2047] = 0; 
	//putUartBytes(buf,strlen(buf));	
	
	//U_PutBytes_VFIFO(uart_port3,buff,len,MOD_MMI);
	U_PutBytes_VFIFO(uart_port3,buf,strlen(buf),MOD_MMI);
	//UART_Close(uart_port3,hgDVTuart);
} 

void BleIDSend(void)
{
	//char *buf = "ABCDEFG123456";
	char buff[50] = {0};
	StopTimer(GIS_TIMER_ID_24);

	n_ble_send_imei_count++;	
	if(strlen(g_lhat_imei)!=15)	
	{
		StartTimer(GIS_TIMER_ID_24,1800,BleIDSend);
		return ;
	}
	if(!BLE_FLAG)
	{
		if(n_ble_send_imei_count >5)
		{
			n_ble_send_imei_count = 0 ;
			StopTimer(GIS_TIMER_ID_24);
			U_Close(uart_port3, U_GetOwnerID(uart_port3));
			return;
		}
		kal_prompt_trace(MOD_TST, "++++++++imei:%s++++++++n_ble_send_imei_count=%d", (g_lhat_imei+7),n_ble_send_imei_count);
		sprintf(buff,"$%s\r\n",(g_lhat_imei+7));
		perun_trace("%s",buff);
		StartTimer(GIS_TIMER_ID_24,10*1000,BleIDSend);
	}
	else
	{
		
		kal_prompt_trace(MOD_TST, "++++CLOSE UART3+++++");
		U_Close(uart_port3, U_GetOwnerID(uart_port3));
		return;
	}
}
*/
#endif

#if 0//__UART1_SUPPORT__
//#define	MORSUN_GPS_MAX_READ_LENGTH 2048
//kal_uint8 uar1_gps_input_data[MORSUN_GPS_MAX_READ_LENGTH];
#define	UART1_GPS_MAX_READ_LENGTH 2048
kal_uint8 uar1_ble_input_data[UART1_GPS_MAX_READ_LENGTH];
BMS_infor_param bms_infor ;
BMS_controller_state cont_status;

//static u8 BLE_FLAG = 0;
//kal_uint8 n_ble_send_imei_count = 0 ;

extern void uaert1__dafault_tx_cb(UART_PORT port);


static kal_bool checkout_receive_start_end_data(char* receive_buf,unsigned char size)
{
	unsigned char receive_crc = 0x00,cal_crc = 0x00;
	unsigned char i = 0;
	if((receive_buf[0] == 0x3A )
		&& (receive_buf[1] == 0x21 )
		&& (receive_buf[size-1] == 0x0a )
		&& (receive_buf[size-2] == 0x0d) 
		)
	{
		receive_crc = receive_buf[size-4];
		
		for(i = 1;i<(size-5);i++)
		{
				cal_crc += receive_buf[i];
		}
		kal_prompt_trace(MOD_TST, "+++++checkout_receive_start_end_data cal_crc=%02x receive_crc=%02x\r\n",cal_crc,receive_crc);
		if(cal_crc!=receive_crc)
		{

			return KAL_FALSE;
		}
		else
		{
			return KAL_TRUE;
		}
	}
	else
	{
		kal_prompt_trace(MOD_TST, "+++++checkout_receive_start_end_data fail\r\n");
		return KAL_FALSE;
	}
}

void uaert1_buffer_process(char* buffer,unsigned int len)
{

	//char imei[100] = {0};
	if(len==0)
		return;
	else
	{
		unsigned char i =0 ;
		unsigned char cmd =0 ;
		//int buff_len = 0;
		//char tmp[100] = {0};
		//TUDO 
		//kal_prompt_trace(MOD_TST, "++++++++UART1 RECEIVE BUFF :++++++++\r\n");
		
		//for(i=0;i<len;i++)
		//{
		//	kal_prompt_trace(MOD_TST, " %02x", buffer[i]);
		//}
		//kal_prompt_trace(MOD_TST, "\r\n");

		if(KAL_FALSE ==checkout_receive_start_end_data(buffer,len))
		{
			return ;
		}
		
		cmd = buffer[2];
		switch(cmd)
		{
			case 0x10:
				{
				}
				break;
			case 0x11:
				{
					cont_status.cont_current_vbat = (buffer[4])&0xff;
					cont_status.cont_current_vbat |= (buffer[5]<<8&0xff00);

					cont_status.cont_current_cur = (buffer[6]&0xff);

					cont_status.cont_current_speed = (buffer[7])&0xff;
					cont_status.cont_current_speed |= (buffer[8]<<8&0xff00);

					cont_status.cont_current_rpm = (buffer[9]&0xff);

					cont_status.cont_current_fault = (buffer[10]&0xff);

					cont_status.cont_current_result = (buffer[11]&0xff);

					cont_status.cont_current_distance = (buffer[12])&0xff;
					cont_status.cont_current_distance |= (buffer[13<<8]&0xff00);

					cont_status.cont_current_lock_status = (buffer[14]&0xff);

					//buff_len=0;
					//memset(tmp,0,sizeof(tmp));
					for(i=0;i<len;i++)
					{
						//buff_len =sprintf(tmp+buff_len,"%02x ",buffer[i]);
						kal_prompt_trace(MOD_TST, "%02x ", buffer[i]);
					}
					//kal_prompt_trace(MOD_TST, "cont_current_vbat =%s",tmp);
					kal_prompt_trace(MOD_TST, "cont_status =%04x",cont_status.cont_current_vbat);
					kal_prompt_trace(MOD_TST, "cont_current_cur =%02x",cont_status.cont_current_cur);
					kal_prompt_trace(MOD_TST, "cont_current_speed =%04x",cont_status.cont_current_speed);
					kal_prompt_trace(MOD_TST, "cont_current_rpm =%02x",cont_status.cont_current_rpm);
					kal_prompt_trace(MOD_TST, "cont_current_fault =%02x",cont_status.cont_current_fault);
					kal_prompt_trace(MOD_TST, "cont_current_result =%02x",cont_status.cont_current_result);
					kal_prompt_trace(MOD_TST, "cont_current_distance =%04x",cont_status.cont_current_distance);
					kal_prompt_trace(MOD_TST, "cont_current_lock_status =%02x",cont_status.cont_current_lock_status);
					//memcpy(s_BMS_control_data_buff,buffer,len);
				}
				break;
			case 0x12:
				{
					bms_infor.bms_vbat_tmp = (buffer[4]&0xff);
					bms_infor.bms_vbat_tmp |= (buffer[5]<<8&0xff00);

					bms_infor.bms_residue_vbat = (buffer[6]&0xff);
					bms_infor.bms_residue_vbat |= (buffer[7]<<8&0xff00);

					bms_infor.bms_charge_count = (buffer[8]&0xff);
					bms_infor.bms_charge_count |= (buffer[9]<<8&0xff00);

					bms_infor.bms_history_charge_count = (buffer[10]&0xff);
					bms_infor.bms_history_charge_count |= (buffer[11]<<8&0xff00);

					bms_infor.bms_current_charge_interval = (buffer[12]&0xff);
					bms_infor.bms_current_charge_interval |= (buffer[13]<<8&0xff00);

					bms_infor.bms_charge_full_vbat = (buffer[14]&0xff);
					bms_infor.bms_charge_full_vbat |= (buffer[15]<<8&0xff00);

					bms_infor.bms_hediantai = (buffer[16]&0xff);
					bms_infor.bms_hediantai |= (buffer[17]<<8&0xff00);

					bms_infor.bms_vbat_value = (buffer[18]&0xff);
					bms_infor.bms_vbat_value |= (buffer[19]<<8&0xff00);

					bms_infor.bms_cur_value = (buffer[20]&0xff);
					bms_infor.bms_cur_value |= (buffer[21]<<8&0xff00);

					//buff_len = 0;
					//memset(tmp,0,sizeof(tmp));
					for(i=0;i<len;i++)
					{
						//buff_len =sprintf(tmp+buff_len,"%02x ",buffer[i]);
						kal_prompt_trace(MOD_TST, "%02x ", buffer[i]);
					}
					//kal_prompt_trace(MOD_TST, "bms_infor =%s",tmp);
					
					kal_prompt_trace(MOD_TST, "bms_vbat_tmp =%04x",bms_infor.bms_vbat_tmp);
					kal_prompt_trace(MOD_TST, "bms_residue_vbat =%04x",bms_infor.bms_residue_vbat);
					kal_prompt_trace(MOD_TST, "bms_charge_count =%04x",bms_infor.bms_charge_count);
					kal_prompt_trace(MOD_TST, "bms_history_charge_count =%04x",bms_infor.bms_history_charge_count);
					kal_prompt_trace(MOD_TST, "bms_current_charge_interval =%04x",bms_infor.bms_current_charge_interval);
					kal_prompt_trace(MOD_TST, "bms_charge_full_vbat =%04x",bms_infor.bms_charge_full_vbat);
					kal_prompt_trace(MOD_TST, "bms_hediantai =%04x",bms_infor.bms_hediantai);
					kal_prompt_trace(MOD_TST, "bms_vbat_value =%04x",bms_infor.bms_vbat_value);
					kal_prompt_trace(MOD_TST, "bms_vbat_value =%04x",bms_infor.bms_cur_value);
					//memcpy(s_BMS_infor_data_buff,buffer,len);
				}
				break;
			case 0x14:
				{
					//kal_prompt_trace(MOD_TST, "s_BMS_control_Data ");	
					//buff_len=0;
					//memset(tmp,0,sizeof(tmp));
					for(i=0;i<32;i++)
					{
						s_BMS_control_Data[i] = buffer[i+4];
						//buff_len =sprintf(tmp+buff_len,"%02x ",s_BMS_control_Data[i]);
						kal_prompt_trace(MOD_TST, "cont %02x ", buffer[i+4]);
					}
					//kal_prompt_trace(MOD_TST, "s_BMS_control_Data =%s",tmp);
					
					//buff_len=0;
					//memset(tmp,0,sizeof(tmp));
					for(i=0;i<32;i++)
					{
						s_BMS_ID_Data[i] = buffer[i+4+32];
						kal_prompt_trace(MOD_TST, "id %02x ", buffer[i+4+32]);
					}
					//kal_prompt_trace(MOD_TST, "s_BMS_ID_Data =%s",tmp);
					//memcpy(s_BMS_ID_data_buff,buffer,len);
				}
				break;	
			default:
				break ;		
		}
	}
}


void Uart1_readyToRead_ind_hdler(UART_PORT port)
{
	U32 len = 0;
	memset(uar1_ble_input_data,0,sizeof(uar1_ble_input_data));
	{
		len = U_GetBytes_VFIFO(
			uart_port1,
			(kal_uint8*)uar1_ble_input_data,
			MORSUN_GPS_MAX_READ_LENGTH,
			NULL,
			MOD_MMI);
		uaert1_buffer_process((S8 *)uar1_ble_input_data, len);

	}
}

void UART1_dafault_tx_cb(UART_PORT port)
{
	
}

void Uart1_Init(void)
{
#ifdef WIN32
#else
	module_type module_id = MOD_MMI;
	kal_bool bSuccess = KAL_FALSE;
	char buffer[100];
	kal_uint8 zf_uart_sleep_handle;

	UARTDCBStruct dcb =
	{
		UART_BAUD_4800, //init
		len_8,      //dataBits;
		sb_1,       // stopBits;
		pa_none,    // parity; 
		fc_none,
		0x11,   // xonChar;
		0x13,   // xoffChar;
		KAL_TRUE
	};
	
	//_zf_++,20160528,÷ªΩ¯»Î“ª¥Œ£¨∑¿÷πÀ¿ª˙	    
	zf_uart_sleep_handle = L1SM_GetHandle();	
	L1SM_SleepDisable(zf_uart_sleep_handle);	
	//_zf_--,20160528,÷ªΩ¯»Î“ª¥Œ£¨∑¿÷πÀ¿ª˙
	
	U_Close(uart_port1, U_GetOwnerID(uart_port1));
	UART_HWInit(uart_port1);
	dcb.baud = (UART_baudrate)UART_BAUD_4800;
	U_SetOwner (uart_port1, module_id);
	U_SetDCBConfig(uart_port1, &dcb, module_id);
	bSuccess = U_Open(uart_port1, module_id);
	
	kal_prompt_trace(MOD_TST, "++++++++UART1 Init  :%d++++++++", bSuccess);
	ASSERT(bSuccess == KAL_TRUE);
	U_Register_TX_cb(uart_port1, module_id, UART1_dafault_tx_cb);
	U_Register_RX_cb(uart_port1, module_id, Uart1_readyToRead_ind_hdler);
	UART_TurnOnPower(uart_port1, KAL_TRUE);
#endif
}

void mmi_test_uart1_timer_handle(void)
{
	char *buf="abcdefg";
	StopTimer(GIS_TIMER_ID_30);

	U_PutBytes_VFIFO(uart_port1,buf,strlen(buf),MOD_MMI);
	
	StartTimer(GIS_TIMER_ID_30, 1000, mmi_test_uart1_timer_handle);
}

void test_uart1_timer(void)
{
	StartTimer(GIS_TIMER_ID_32, 1000, mmi_test_uart1_timer_handle);	
}

#endif

U32 mmi_gprs_get_timestamp(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    applib_time_struct time;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* Get cur time */
    applib_dt_get_rtc_time(&time);
    /* Cur time to utc */
    /* return applib_dt_mytime_2_utc_sec(&time, MMI_FALSE); */
    return applib_dt_sec_local_to_utc(applib_dt_mytime_2_utc_sec(&time, MMI_FALSE));
}

#if defined(__LHAT_GUANGXIN_COMMON__)
void mmi_ble_heartbeat_time_handle(void)
{
	StopTimer(GIS_TIMER_ID_30);

	//lhat_get_blue_ext_gpio_status();

	/*if(g_ble_ext_gpio_in_flag == KAL_TRUE)
	{
		n_ble_send_count = 0 ;
		n_ble_receive_count=0;
		kal_prompt_trace(MOD_TST, "++++mmi_ble_heartbeat_time_handle+++++g_ble_ext_gpio_in_flag=%d",g_ble_ext_gpio_in_flag);
	}*/
	n_ble_send_count+=1;
	kal_prompt_trace(MOD_TST, "++++n_ble_send_count+++++=%d",n_ble_send_count);
	if(n_ble_send_count>=300)
	{
		lhat_ble_heart_gpio(1);
		mmi_Delayms(500);
		lhat_ble_heart_gpio(0);
		n_ble_send_count = 0 ;
		n_ble_receive_count+=1;
		kal_prompt_trace(MOD_TST, "++++mmi_ble_heartbeat_time_handle+++++n_ble_send_count=%d",n_ble_send_count);
	}

	if(n_ble_receive_count>=180)
	{
		n_ble_receive_count=0;
		n_ble_send_count=0;
		// reset to nrf51822
		kal_prompt_trace(MOD_TST, "++++mmi_ble_heartbeat_time_handle+++++n_ble_receive_count=%d",n_ble_receive_count);
	}

	n_breath_heart_count++;
	if(KAL_TRUE==g_breath_heart_flag)
	{
		g_breath_heart_flag=KAL_FALSE;
		n_breath_heart_count=0;
	}
	if(n_breath_heart_count>600)
	{
		g_gprs_login_status_flag=KAL_FALSE;
	}
	
	StartTimer(GIS_TIMER_ID_30, 1000, mmi_ble_heartbeat_time_handle);
}

void mmi_ble_start_open_lock_handle(void)
{
	lhat_5v_dc_onoff(1);
	mmi_set_gpio_timer_status();
	//lhat_MOTO_IB_gpio(1);
	lhat_MOTO_IA_gpio(1);
	mmi_motor_open_timer();
	mmi_set_bike_lock_open_status(KAL_FALSE);
	mmi_set_bike_lock_close_init_status(KAL_TRUE);
	StopTimer(GIS_TIMER_ID_31);
}

void mmi_ble_start_open_lock_time(void)
{
	StopTimer(GIS_TIMER_ID_31);

	StartTimer(GIS_TIMER_ID_31, 30000, mmi_ble_start_open_lock_handle);
}

#endif

#if 0


// Enable both ECB and CBC mode. Note this can be done before including aes.h or at compile-time.
// E.g. with GCC by using the -D flag: gcc -c aes.c -DCBC=0 -DECB=1
#define CBC 1
#define ECB 1

#include "aes.h"
#include "base64.h"

static void phex(uint8_t* str);
static void test_encrypt_ecb(unsigned char *in,unsigned char *out);

static void test_decrypt_ecb(char *in);

static void test_encrypt_ecb_verbose(void);
//#define ipaddr "192.168.1.22"
//#define PORT 4444

#define ipaddr "114.55.100.103"
#define PORT 80

/*static int locationPackage(char *buff)
{
	double lng = 113.90692;
	double lat = 22.58243;
	int buff_len = 0;
	time_t timep; 
	struct tm *p; 
	time(&timep); 
	p=gmtime(&timep); 
	printf("%d-%d-%d ",(1900+p->tm_year), (1+p->tm_mon),p->tm_mday); 
	printf("%d:%d:%d\r\n", p->tm_hour, p->tm_min, p->tm_sec); 

	if(!buff)	return;
	buff_len += sprintf(buff+buff_len,"LOCATION 1\r\n");
	buff_len += sprintf(buff+buff_len,"%04d-%02d-%02d %02d:%02d:%02d,",
	(1900+p->tm_year),(1+p->tm_mon),p->tm_mday,(p->tm_hour+8),p->tm_min,p->tm_sec);
	buff_len += sprintf(buff+buff_len,"%.5f,%.6f,1,0,400\r\n",lng,lat);
	printf("location:%s,%d\r\n",buff,buff_len);		
	return buff_len;
}



static int socket_open(void)
{
	int socketld;
	struct sockaddr_in serv_addr;
	int status;
	socketld = socket(AF_INET, SOCK_STREAM, 0);
	if(socketld<0)
	{
		//printf("error,create a socket failed!\n");
		kal_prompt_trace(MOD_TST, "error,create a socket failed!");
		return -1;
	}
	memset(&serv_addr,0,sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);
	if(inet_pton(AF_INET, ipaddr, &serv_addr.sin_addr) <= 0 )

	 { 
		//printf("ÂàõÂª∫ÁΩëÁªúËøûÊé•Â§±Ë¥•,Êú¨Á∫øÁ®ãÂç≥Â∞ÜÁªàÊ≠¢--inet_pton error!\n"); 
		kal_prompt_trace(MOD_TST, "error,create a socket failed!");
		exit(0);
	 }								      
	status = connect(socketld,(struct sockaddr *)&serv_addr,sizeof(serv_addr));
	if(status!=0)
	{
		printf("error,Connecting failed!,%d\n",status);
		//closesocket(socketld);
		return -1;
	}
	printf("connected!\r\n");

	return socketld;

}

static int socket_recv(int socketfd)
{
	char *p;
	int h,i;
        fd_set t_set1; 
	char buf[4096];
	struct timeval  tv; 
	FD_ZERO(&t_set1); 
       FD_SET(socketfd, &t_set1); 
	while(1)
	{ 
               sleep(2); 
               tv.tv_sec= 0; 
               tv.tv_usec= 0; 
               h= 0; 
               printf("--------------->1\n"); 
               h= select(socketfd+1,&t_set1,NULL,NULL,&tv); 
               printf("--------------->2\n"); 
 
               //if (h == 0) continue; 
                if(h < 0) 
				{ 
					close(socketfd); 
					printf("Âú®ËØªÂèñÊï∞ÊçÆÊä•ÊñáÊó∂SELECTÊ£ÄÊµãÂà∞ÂºÇÂ∏∏ÔºåËØ•ÂºÇÂ∏∏ÂØºËá¥Á∫øÁ®ãÁªàÊ≠¢ÔºÅ\n"); 
					return-1; 
				}; 
 
               if(h > 0){ 
                       memset(buf, 0, 4096); 
                       i= read(socketfd, buf, 4095); 
                       if(i==0){ 
                               close(socketfd); 
                               printf("ËØªÂèñÊï∞ÊçÆÊä•ÊñáÊó∂ÂèëÁé∞ËøúÁ´ØÂÖ≥Èó≠ÔºåËØ•Á∫øÁ®ãÁªàÊ≠¢ÔºÅ\n"); 
                               return-1; 
                      } 
					  
                       printf("%s\n", buf); 
					   close(socketfd); 
					   return 0;
               } 
       } 
       close(socketfd); 
 
       return 0; 

}
*/




/*
GET /webfin/websocket/ HTTP/1.1
Host: 114.55.100.103:80
Upgrade: websocket
Connection: Upgrade
Sec-WebSocket-Key: xqBt3ImNzJbYqRINxEFlkg==
Origin: 114.55.100.103:80
Sec-WebSocket-Version: 13
*/

/*
GET /chat HTTP/1.1
Host: server.example.com
Upgrade: websocket
Connection: Upgrade
Sec-WebSocket-Key: dGhlIHNhbXBsZSBub25jZQ==
Origin: http://example.com
Sec-WebSocket-Protocol: chat, superchat
Sec-WebSocket-Version: 13




*/



// prints string as hex
static void phex(uint8_t* str)
{

#ifdef AES128
    uint8_t len = 16;
#elif defined(AES192)
    uint8_t len = 24;
#elif defined(AES256)
    uint8_t len = 32;
#endif

    unsigned char i;
    for(i = 0; i < len; ++i)
        //printf("%.2x", str[i]);
		kal_prompt_trace(MOD_TST, "%.2x",str[i]);
    //printf("\n");
	kal_prompt_trace(MOD_TST, "\n");
}

static void test_encrypt_ecb_verbose(void)
{
    // Example of more verbose verification

    uint8_t i, buf[64], buf2[64];

    // 128bit key
    uint8_t key[16] =        { (uint8_t) 0x2b, (uint8_t) 0x7e, (uint8_t) 0x15, (uint8_t) 0x16, (uint8_t) 0x28, (uint8_t) 0xae, (uint8_t) 0xd2, (uint8_t) 0xa6, (uint8_t) 0xab, (uint8_t) 0xf7, (uint8_t) 0x15, (uint8_t) 0x88, (uint8_t) 0x09, (uint8_t) 0xcf, (uint8_t) 0x4f, (uint8_t) 0x3c };
    // 512bit text
    uint8_t plain_text[64] = { (uint8_t) 0x6b, (uint8_t) 0xc1, (uint8_t) 0xbe, (uint8_t) 0xe2, (uint8_t) 0x2e, (uint8_t) 0x40, (uint8_t) 0x9f, (uint8_t) 0x96, (uint8_t) 0xe9, (uint8_t) 0x3d, (uint8_t) 0x7e, (uint8_t) 0x11, (uint8_t) 0x73, (uint8_t) 0x93, (uint8_t) 0x17, (uint8_t) 0x2a,
                               (uint8_t) 0xae, (uint8_t) 0x2d, (uint8_t) 0x8a, (uint8_t) 0x57, (uint8_t) 0x1e, (uint8_t) 0x03, (uint8_t) 0xac, (uint8_t) 0x9c, (uint8_t) 0x9e, (uint8_t) 0xb7, (uint8_t) 0x6f, (uint8_t) 0xac, (uint8_t) 0x45, (uint8_t) 0xaf, (uint8_t) 0x8e, (uint8_t) 0x51,
                               (uint8_t) 0x30, (uint8_t) 0xc8, (uint8_t) 0x1c, (uint8_t) 0x46, (uint8_t) 0xa3, (uint8_t) 0x5c, (uint8_t) 0xe4, (uint8_t) 0x11, (uint8_t) 0xe5, (uint8_t) 0xfb, (uint8_t) 0xc1, (uint8_t) 0x19, (uint8_t) 0x1a, (uint8_t) 0x0a, (uint8_t) 0x52, (uint8_t) 0xef,
                               (uint8_t) 0xf6, (uint8_t) 0x9f, (uint8_t) 0x24, (uint8_t) 0x45, (uint8_t) 0xdf, (uint8_t) 0x4f, (uint8_t) 0x9b, (uint8_t) 0x17, (uint8_t) 0xad, (uint8_t) 0x2b, (uint8_t) 0x41, (uint8_t) 0x7b, (uint8_t) 0xe6, (uint8_t) 0x6c, (uint8_t) 0x37, (uint8_t) 0x10 };

    memset(buf, 0, 64);
    memset(buf2, 0, 64);

    // print text to encrypt, key and IV
    //printf("ECB encrypt verbose:\n\n");
    //printf("plain text:\n");
	kal_prompt_trace(MOD_TST, "ECB encrypt verbose:\n\n");
	kal_prompt_trace(MOD_TST, "plain text:\n");
    for(i = (uint8_t) 0; i < (uint8_t) 4; ++i)
    {
        phex(plain_text + i * (uint8_t) 16);
    }
    //printf("\n");
    kal_prompt_trace(MOD_TST, "\n");

    //printf("key:\n");
	kal_prompt_trace(MOD_TST, "key:\n");
    phex(key);
    //printf("\n");
    kal_prompt_trace(MOD_TST, "\n");

    // print the resulting cipher as 4 x 16 byte strings
    //printf("ciphertext:\n");
	kal_prompt_trace(MOD_TST, "ciphertext:\n");
    for(i = 0; i < 4; ++i)
    {
        AES_ECB_encrypt(plain_text + (i*16), key, buf+(i*16), 16);
        phex(buf + (i*16));
    }
    //printf("\n");
	kal_prompt_trace(MOD_TST, "\n");
}


void test_encrypt_ecb(unsigned char *in,unsigned char *out)
{
	unsigned char i=0;
#ifdef AES128
    uint8_t key[] = {0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c};
#endif
	if(!in||!out)		return ;
	AES_ECB_encrypt(in, AES_KEY, out, 16);

	//printf("ECB encrypt: ");
	kal_prompt_trace(MOD_TST, "ECB encrypt: ");
	for(i=0;i<16;i++)
	{
		//printf("%02x ",out[i]);
		kal_prompt_trace(MOD_TST, "%02x ",out[i]);
	}

}


void test_decrypt_ecb(char *in)
{
	unsigned char out[16] = {0},i=0;
	if(!in)	return ;
	AES_ECB_decrypt(in, AES_KEY, out, 16);
	//printf("ECB decrypt: ");
	kal_prompt_trace(MOD_TST, "ECB decrypt: ");
	for(i=0;i<16;i++)
	{
		//printf("%02x ",out[i]);
		kal_prompt_trace(MOD_TST, "%02x ",out[i]);
	}
}

unsigned int build_websocket_msg_unmask(unsigned char* buf,unsigned int len)
{
//Mask 0x01010101
	unsigned int i,j;
	unsigned char  msg[200];
	memset(msg,0,sizeof(msg));
	msg[0] = 0x81;
	msg[1] = len;
	for(i=0;i<len;i++)
	{
		msg[i+2] = buf[i];
	}	
	memcpy(buf,msg,len+2);
	return len+2;
	
}



unsigned int build_websocket_msg(unsigned char* buf,unsigned int len)
{
//Mask 0x01010101
	unsigned int i,j;
	unsigned char  msg[200];
	msg[0] = 0x81;
	msg[1] = (len | 0x80);
	msg[2] = 0x01;
	msg[3] = 0x01;
	msg[4] = 0x01;
	msg[5] = 0x01;

	for(i = 0,j = 0;i < len;i++,j++)	
	{
		buf[i] = buf[i]^msg[j%4+2];
	}
	
	memcpy(&msg[6],buf,len);
	memcpy(buf,msg,len+6);
//	printf("%02x\r\n",len);
	return len + 6;
}

unsigned char randomVal(void)
{
	unsigned char val = 0;
	srand(time(0));
	val = (rand()%255+1);
	return val;

}


char *data = "LOGIN 0\r\nLkHBlC9X0 8BD403C6283C39B7C75EC2453C399955 179 898602b6111730031645\r\n";
void createwebsocketkey(char *out)
{
	unsigned char i = 0;
	unsigned char random[16] = {0};
	if(!out)	return;
	//srand(time(0));  
	for(i=0;i<16;i++)
	{
		random[i] = (rand()%255+1);
	}
	base64_encode(random, 16, out);
	//printf("websocket key = %s\r\n",out);
	kal_prompt_trace(MOD_TST, "websocket key = %s\r\n",out);
}

int lhat_websocket_main(void)
{
	int msg_id = 0;
	char base64_mac[10] = {0};
	int len_websocket = 0;
	unsigned char out[16] = {0};
	char websocket_key[30] = {0};
	char websocket[512] = {0};
	char in[16] = {0},i=0,len = 0,len1;
	int sockld,ret = 0;
	char *login = "LOGIN";
	unsigned char socket_buff[512] = {0};
#ifdef AES128
    //printf("\nTesting AES128\n\n");
	kal_prompt_trace(MOD_TST, "\nTesting AES128\n\n");
#endif
	memset(websocket,0,sizeof(websocket));
	createwebsocketkey(websocket_key);
	
#if 0	
	len_websocket += sprintf(websocket+len_websocket,"%s","GET /webfin/websocket/ HTTP/1.1\r\n");
	len_websocket += sprintf(websocket+len_websocket,"%s","Host: 114.55.100.103:80\r\n");
	len_websocket += sprintf(websocket+len_websocket,"%s","Upgrade: websocket\r\n");
	len_websocket += sprintf(websocket+len_websocket,"%s","Connection: Upgrade\r\n");
//	len_websocket += sprintf(websocket+len_websocket,"%s","Sec-WebSocket-Key: xqBt3ImNzJbYqRINxEFlkg==\r\n");
	len_websocket += sprintf(websocket+len_websocket,"%s","Sec-WebSocket-Key: ");
	len_websocket += sprintf(websocket+len_websocket,"%s\r\n",websocket_key);

	len_websocket += sprintf(websocket+len_websocket,"%s","Origin: 114.55.100.103:80\r\n");
	len_websocket += sprintf(websocket+len_websocket,"%s","Sec-WebSocket-Version: 13\r\n");
#endif
//

	len_websocket += sprintf(websocket+len_websocket,"%s","GET / HTTP/1.1\r\n");
	len_websocket += sprintf(websocket+len_websocket,"%s","Upgrade: websocket\r\n");
	len_websocket += sprintf(websocket+len_websocket,"%s","Connection: Upgrade\r\n");
	len_websocket += sprintf(websocket+len_websocket,"%s","Host: 114.55.100.103:80\r\n");
	len_websocket += sprintf(websocket+len_websocket,"%s","Sec-WebSocket-Key: ");
	len_websocket += sprintf(websocket+len_websocket,"%s\r\n",websocket_key);
	len_websocket += sprintf(websocket+len_websocket,"%s","Origin: http://gps.hua1268.com/\r\n");
	len_websocket += sprintf(websocket+len_websocket,"%s","Sec-WebSocket-Version: 13\r\n\r\n");

	//printf("key = %s\r\n",AES_KEY);
	kal_prompt_trace(MOD_TST, "key = %s\r\n",AES_KEY);
	strcpy(in,MAC);
	/*deviceid--->base64+L*/
	base64_encode(device_id,6,base64_mac);
	len = strlen(in);
	for(i=0;i<(16-len);i++)
	{
		in[i+len] = 0x30;		//'0' macÂú∞ÂùÄÂêéÊ∑ªÂä†0 Ë°•Ë∂≥16byte
	}
	in[16] = '\0';				//end
	//printf("Âä†ÂØÜÊ∫ê:%s\r\n",in);
	kal_prompt_trace(MOD_TST, "key old:%s\r\n",in);
	test_encrypt_ecb(in,out);
	//printf("\r\n");
	kal_prompt_trace(MOD_TST, "\r\n");
	test_decrypt_ecb(out);
	//printf("\r\n");
	kal_prompt_trace(MOD_TST, "\r\n");
#if 1			//login
	len = 0; 
	/*<LOGIN>*/
	len += sprintf(socket_buff+len,"%s %d\r\nL%s ",login,msg_id,base64_mac);
	/*<EncryptData>*/
	for(i=0;i<16;i++)
	{
		len += sprintf(socket_buff+len,"%02x",out[i]);
	}
	/*<random> <imei>*/
	len += sprintf(socket_buff+len," %d %s\r\n",randomVal(),IMEI);
	//printf("socket buf :%s",socket_buff);
	kal_prompt_trace(MOD_TST, "socket buf :%s\r\n",socket_buff);

//	len = build_websocket_msg_unmask(socket_buff,strlen(socket_buff));	
	/*websocket data frame with msk*/
	len = build_websocket_msg(socket_buff,strlen(socket_buff));
	//printf("websocket len:%d\r\n",len);
	kal_prompt_trace(MOD_TST, "websocket len:%d\r\n",len);

	for(i=0;i<len;i++)
		//printf("%02x",socket_buff[i]);
		kal_prompt_trace(MOD_TST, "%02x",socket_buff[i]);
#else 
	len = locationPackage(socket_buff);
	len1 = build_websocket_msg_unmask(socket_buff,len);	
	for(i=0;i<len1;i++)
		printf("%02x",socket_buff[i]);
#endif
	
//////////socket//////////////////
#if 0
	sockld = socket_open();
	if(sockld<0)
	{
		//printf("open failed!!\r\n");
		kal_prompt_trace(MOD_TST, "open failed!!\r\n");
		return -1;
	}
	printf("open ok!!\n");
	printf("%s",websocket);
	ret = write(sockld,websocket,strlen(websocket)); 
	if(ret>0)
	{
		printf("websocket send suc\r\n");
	}
//	ret = write(sockld,data,strlen(data)); 
	/*<socket send>*/
	ret = write(sockld,socket_buff,len); 
	if(ret < 0)
	{ 
		printf("ÂèëÈÄÅÂ§±Ë¥•ÔºÅÈîôËØØ‰ª£Á†ÅÊòØ%dÔºåÈîôËØØ‰ø°ÊÅØÊòØ'%s'\n",errno, strerror(errno)); 
		exit(0); 
	}
	else
	{
//		printf("%s",data);
		printf("%s",socket_buff);
		printf("Ê∂àÊÅØÂèëÈÄÅÊàêÂäüÔºåÂÖ±ÂèëÈÄÅ‰∫Ü%d‰∏™Â≠óËäÇÔºÅ\n\n", ret); 
	} 

	socket_recv(sockld);
	sleep(30);
#endif

/////////////////////////////////////	
    return 0;
}

void lhat_websocket_login(void *pmac_addr,kal_uint8 nmsgid,kal_uint8 ndeviceid)
{
		int msg_id = 0;
		char base64_mac[10] = {0};
		int len_websocket = 0;
		unsigned char out[16] = {0};
		char websocket_key[30] = {0};
		char websocket[512] = {0};
		char in[16] = {0},i=0,len = 0,len1;
		int sockld,ret = 0;
		char *login = "LOGIN";
		unsigned char socket_buff[512] = {0};
#ifdef AES128
		//printf("\nTesting AES128\n\n");
		kal_prompt_trace(MOD_TST, "\nTesting AES128\n\n");
#endif
		memset(websocket,0,sizeof(websocket));
		createwebsocketkey(websocket_key);
		
#if 0	
		len_websocket += sprintf(websocket+len_websocket,"%s","GET /webfin/websocket/ HTTP/1.1\r\n");
		len_websocket += sprintf(websocket+len_websocket,"%s","Host: 114.55.100.103:80\r\n");
		len_websocket += sprintf(websocket+len_websocket,"%s","Upgrade: websocket\r\n");
		len_websocket += sprintf(websocket+len_websocket,"%s","Connection: Upgrade\r\n");
	//	len_websocket += sprintf(websocket+len_websocket,"%s","Sec-WebSocket-Key: xqBt3ImNzJbYqRINxEFlkg==\r\n");
		len_websocket += sprintf(websocket+len_websocket,"%s","Sec-WebSocket-Key: ");
		len_websocket += sprintf(websocket+len_websocket,"%s\r\n",websocket_key);
	
		len_websocket += sprintf(websocket+len_websocket,"%s","Origin: 114.55.100.103:80\r\n");
		len_websocket += sprintf(websocket+len_websocket,"%s","Sec-WebSocket-Version: 13\r\n");
#endif
	//
	
		len_websocket += sprintf(websocket+len_websocket,"%s","GET / HTTP/1.1\r\n");
		len_websocket += sprintf(websocket+len_websocket,"%s","Upgrade: websocket\r\n");
		len_websocket += sprintf(websocket+len_websocket,"%s","Connection: Upgrade\r\n");
		len_websocket += sprintf(websocket+len_websocket,"%s","Host: 114.55.100.103:80\r\n");
		len_websocket += sprintf(websocket+len_websocket,"%s","Sec-WebSocket-Key: ");
		len_websocket += sprintf(websocket+len_websocket,"%s\r\n",websocket_key);
		len_websocket += sprintf(websocket+len_websocket,"%s","Origin: http://gps.hua1268.com/\r\n");
		len_websocket += sprintf(websocket+len_websocket,"%s","Sec-WebSocket-Version: 13\r\n\r\n");
	
		//printf("key = %s\r\n",AES_KEY);
		kal_prompt_trace(MOD_TST, "key = %s\r\n",AES_KEY);
		strcpy(in,pmac_addr);
		/*deviceid--->base64+L*/
		base64_encode(device_id,6,base64_mac);
		len = strlen(in);
		for(i=0;i<(16-len);i++)
		{
			in[i+len] = 0x30;		//'0' macÂú∞ÂùÄÂêéÊ∑ªÂä†0 Ë°•Ë∂≥16byte
		}
		in[16] = '\0';				//end
		//printf("Âä†ÂØÜÊ∫ê:%s\r\n",in);
		kal_prompt_trace(MOD_TST, "key old:%s\r\n",in);
		test_encrypt_ecb(in,out);
		//printf("\r\n");
		kal_prompt_trace(MOD_TST, "\r\n");
		test_decrypt_ecb(out);
		//printf("\r\n");
		kal_prompt_trace(MOD_TST, "\r\n");
			//login
		len = 0; 
		/*<LOGIN>*/
		len += sprintf(socket_buff+len,"LOGIN %d\r\nL%s ",nmsgid,base64_mac);
		/*<EncryptData>*/
		for(i=0;i<16;i++)
		{
			len += sprintf(socket_buff+len,"%02x",out[i]);
		}
		/*<random> <imei>*/
		len += sprintf(socket_buff+len," %d %s\r\n",randomVal(),g_lhat_imei);
		//printf("socket buf :%s",socket_buff);
		kal_prompt_trace(MOD_TST, "socket buf :%s\r\n",socket_buff);
	
	//	len = build_websocket_msg_unmask(socket_buff,strlen(socket_buff));	
		/*websocket data frame with msk*/
		len = build_websocket_msg(socket_buff,strlen(socket_buff));
		//printf("websocket len:%d\r\n",len);
		kal_prompt_trace(MOD_TST, "websocket len:%d\r\n",len);
	
		for(i=0;i<len;i++)
			//printf("%02x",socket_buff[i]);
			kal_prompt_trace(MOD_TST, "%02x",socket_buff[i]);

		
}



#endif

