/*
 * wl_ath9k.c 
 * Copyright (C) 2010 Christian Scheele <chris@dd-wrt.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License version 2.1
 * as published by the Free Software Foundation
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */
#ifndef __UCLIBC__
//#define __DEFINED_float_t
#endif
#include <string.h>
#include <unistd.h>
#include <typedefs.h>
#include <bcmutils.h>
#include <wlutils.h>
#include <shutils.h>
#include <utils.h>
#include <bcmnvram.h>
#include <math.h>

#include <sys/types.h>
#include <sys/file.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>
#include <getopt.h>
#include <err.h>

// netlink:
#include <netlink/genl/genl.h>
#include <netlink/genl/family.h>
#include <netlink/genl/ctrl.h>
#include <netlink/msg.h>
#include <netlink/attr.h>

#include <linux/nl80211.h>

// dummy TBD 
int getassoclist_ath9k(char *ifname, unsigned char *list)
{
	unsigned int *count = (unsigned int *)list;
	struct mac80211_info *mac80211_info;
	struct wifi_client_info *wc;
	char *l = (char *)list;
	mac80211_info = mac80211_assoclist(ifname);
	l += 4;
	count[0] = 0;
	for (wc = mac80211_info->wci; wc; wc = wc->next) {

		ether_atoe(wc->mac, l);
		l += 6;
		count[0]++;
	}
	free_wifi_clients(mac80211_info->wci);
	free(mac80211_info);
	return count[0];
}

// dummy TBD 
int getRssi_ath9k(char *ifname, unsigned char *mac)
{
	unsigned char rmac[32];
	ether_etoa(mac, rmac);

	struct mac80211_info *mac80211_info;
	struct wifi_client_info *wc;
	mac80211_info = mac80211_assoclist(ifname);
	for (wc = mac80211_info->wci; wc; wc = wc->next) {
		if (!strcmp(wc->mac, rmac)) {
			free_wifi_clients(mac80211_info->wci);
			free(mac80211_info);
			return wc->signal;
		}
	}
	free_wifi_clients(mac80211_info->wci);
	free(mac80211_info);
	return 0;
}

// dummy TBD 
int getUptime_ath9k(char *ifname, unsigned char *mac)
{
	unsigned char rmac[32];
	ether_etoa(mac, rmac);
	struct mac80211_info *mac80211_info;
	struct wifi_client_info *wc;
	mac80211_info = mac80211_assoclist(ifname);
	for (wc = mac80211_info->wci; wc; wc = wc->next) {
		if (!strcmp(wc->mac, rmac)) {
			free_wifi_clients(mac80211_info->wci);
			free(mac80211_info);
			return wc->uptime;
		}
	}
	free_wifi_clients(mac80211_info->wci);
	free(mac80211_info);
	return 0;
}

// dummy TBD 
int getNoise_ath9k(char *ifname, unsigned char *mac)
{
	unsigned char rmac[32];
	ether_etoa(mac, rmac);
	struct mac80211_info *mac80211_info;
	struct wifi_client_info *wc;
	mac80211_info = mac80211_assoclist(ifname);
	for (wc = mac80211_info->wci; wc; wc = wc->next) {
		if (!strcmp(wc->mac, rmac)) {
			free_wifi_clients(mac80211_info->wci);
			free(mac80211_info);
			return wc->noise;
		}
	}
	free_wifi_clients(mac80211_info->wci);
	free(mac80211_info);
	return 0;
}

int getTxRate_ath9k(char *ifname, unsigned char *mac)
{
	unsigned char rmac[32];
	ether_etoa(mac, rmac);
	struct mac80211_info *mac80211_info;
	struct wifi_client_info *wc;
	mac80211_info = mac80211_assoclist(ifname);
	for (wc = mac80211_info->wci; wc; wc = wc->next) {
		if (!strcmp(wc->mac, rmac)) {
			free_wifi_clients(mac80211_info->wci);
			free(mac80211_info);
			return wc->txrate;
		}
	}
	free_wifi_clients(mac80211_info->wci);
	free(mac80211_info);
	return 0;
}

int getRxRate_ath9k(char *ifname, unsigned char *mac)
{
	unsigned char rmac[32];
	ether_etoa(mac, rmac);
	struct mac80211_info *mac80211_info;
	struct wifi_client_info *wc;
	mac80211_info = mac80211_assoclist(ifname);
	for (wc = mac80211_info->wci; wc; wc = wc->next) {
		if (!strcmp(wc->mac, rmac)) {
			free_wifi_clients(mac80211_info->wci);
			free(mac80211_info);
			return wc->rxrate;
		}
	}
	free_wifi_clients(mac80211_info->wci);
	free(mac80211_info);
	return 0;
}
