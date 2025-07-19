#ifndef __HAL_WLAN_H__
#define __HAL_WLAN_H__

#define WLAN_MAX_MAP_NUM 4
#define WLAN_MAX_MAP_CLIENT_NUM 32

typedef struct
{
    char client_intf_mac[18];
    char client_client_mac[18];
    int client_medium;
    char client_rssi[8];
}wifi_map_client_info;

typedef struct
{
    char bh_intf_mac[18];
    char bh_client_mac[18];
    int bh_medium;
    char bh_rssi[8];
    int bh_sta;
}wifi_map_bh_info;

typedef struct
{
    char intf_mac[18];
    int intf_medium;
}wifi_map_intf_info;

typedef struct
{
    char almac[18];
    int role;
    wifi_map_intf_info intf_sta[WLAN_MAX_MAP_CLIENT_NUM];
    wifi_map_bh_info bh_sta[WLAN_MAX_MAP_CLIENT_NUM];
    wifi_map_client_info client_sta[WLAN_MAX_MAP_CLIENT_NUM];
}wlan_map_topology;

typedef struct
{
    wlan_map_topology map_device[WLAN_MAX_MAP_NUM];
}LAN_MAP_TOPOLOGY;

#endif//__HAL_WLAN_H__
