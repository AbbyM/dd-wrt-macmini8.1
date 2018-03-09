
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>
#include <syslog.h>
#include <signal.h>
#include <string.h>
#include <sys/klog.h>
#include <sys/types.h>
#include <sys/mount.h>
#include <sys/reboot.h>
#include <sys/stat.h>
#include <sys/sysmacros.h>
#include <sys/time.h>
#include <sys/utsname.h>
#include <sys/wait.h>
#include <dirent.h>

#include <epivers.h>
#include <bcmnvram.h>
#include <shutils.h>
#include <rc.h>
#include <netconf.h>
#include <nvparse.h>
#include <bcmdevs.h>

#include <wlutils.h>
#include <utils.h>
#include <cyutils.h>
#include <code_pattern.h>
#include <cy_conf.h>
#include <typedefs.h>
#include <bcmnvram.h>
#include <bcmutils.h>
#include <shutils.h>
#include <wlutils.h>
#include <cy_conf.h>
#include <arpa/inet.h>

#include <revision.h>
#include <airbag.h>
#include "crc.c"
#include "servicemanager.c"
#include "services.c"
#include "mtd.c"
#include "hotplug.c"
#include "nvram.c"
#include "mtd_main.c"
#include "ledtool.c"
#include "check_ps.c"
//#include "resetbutton.c"
#include "process_monitor.c"
#include "listen.c"
#include "radio_timer.c"
#include "watchdog.c"
#ifdef HAVE_WOL
#include "wol.c"
#endif
#ifdef HAVE_MADWIFI
#include "roaming_daemon.c"
#endif
#ifdef HAVE_GPIOWATCHER
#include "gpiowatcher.c"
#endif
#ifdef HAVE_WLANLED
#include "wlanled.c"
#endif
#include "ttraff.c"
#ifdef HAVE_WPS
#include "wpswatcher.c"
#endif
#ifdef HAVE_WIVIZ
#include "autokill_wiviz.c"
#include "run_wiviz.c"
#endif
#ifdef HAVE_QTN
#include "qtn_monitor.c"
#endif
#include "event.c"
#include "gratarp.c"
#include "ntp.c"

struct MAIN {
	char *callname;
	char *execname;
	int (*exec) (int argc, char **argv);
};
extern char *getSoftwareRevision(void);
static int softwarerevision_main(int argc, char **argv)
{
	fprintf(stdout, "%s\n", getSoftwareRevision());
	return 0;
}

static struct MAIN maincalls[] = {
	// {"init", NULL, &main_loop},
	{"ip-up", "ipup", NULL},
	{"ip-down", "ipdown", NULL},
	{"ipdown", "disconnected_pppoe", NULL},
	{"udhcpc_tv", "udhcpc_tv", NULL},
	{"udhcpc", "udhcpc", NULL},
	{"mtd", NULL, mtd_main},
	{"hotplug", NULL, hotplug_main},
	{"nvram", NULL, nvram_main},
	{"ttraff", NULL, ttraff_main},
	{"filtersync", "filtersync", NULL},
	{"filter", "filter", NULL},
	{"setpasswd", "setpasswd", NULL},
	{"ipfmt", "ipfmt", NULL},
	{"restart_dns", "restart_dns", NULL},
	{"ledtool", NULL, ledtool_main},
	{"check_ps", NULL, check_ps_main},
//      {"resetbutton", NULL, resetbutton_main},
	{"process_monitor", NULL, process_monitor_main},
	{"listen", NULL, listen_main},
	{"radio_timer", NULL, radio_timer_main},
#ifdef HAVE_MADWIFI
	{"roaming_daemon", NULL, roaming_daemon_main},
#endif
#ifdef HAVE_GPIOWATCHER
	{"gpiowatcher", NULL, gpiowatcher_main},
#endif
#ifdef HAVE_WLANLED
	{"wlanled", NULL, wlanled_main},
#endif
#ifdef HAVE_WPS
	{"wpswatcher", NULL, wpswatcher_main},
#endif
#ifdef HAVE_PPTPD
	{"poptop", "pptpd_main", NULL},
#endif
	{"redial", "redial", NULL},
#ifndef HAVE_RB500
	// {"resetbutton", NULL, &resetbutton_main},
#endif
	// {"wland", NULL, &wland_main},
	{"hb_connect", "hb_connect", NULL},
	{"hb_disconnect", "hb_disconnect", NULL},
	{"gpio", "gpio", NULL},
	{"beep", "beep", NULL},
	{"ledtracking", "ledtracking", NULL},
	// {"listen", NULL, &listen_main},
	// {"check_ps", NULL, &check_ps_main},
	{"ddns_success", "ddns_success", NULL},
	// {"process_monitor", NULL, &process_monitor_main},
	// {"radio_timer", NULL, &radio_timer_main},
	// {"ttraf", NULL, &ttraff_main},
#ifdef HAVE_WIVIZ
	{"run_wiviz", NULL, &run_wiviz_main},
	{"autokill_wiviz", NULL, &autokill_wiviz_main},
#endif
	{"site_survey", "site_survey", NULL},
#ifdef HAVE_WOL
	{"wol", NULL, &wol_main},
#endif
	{"event", NULL, &event_main},
//    {"switch", "switch", NULL},
#ifdef HAVE_MICRO
	{"brctl", "brctl", NULL},
#endif
	{"getbridgeprio", "getbridgeprio", NULL},
#ifdef HAVE_NORTHSTAR
	{"rtkswitch", "rtkswitch", NULL},
#endif
	{"setuserpasswd", "setuserpasswd", NULL},
	{"getbridge", "getbridge", NULL},
	{"getmask", "getmask", NULL},
	{"stopservices", NULL, stop_services_main},
#ifdef HAVE_PPPOESERVER
	{"addpppoeconnected", "addpppoeconnected", NULL},
	{"delpppoeconnected", "delpppoeconnected", NULL},
	{"addpppoetime", "addpppoetime", NULL},
#endif
	{"startservices", NULL, start_services_main},
	{"start_single_service", NULL, start_single_service_main},
	{"restart_f", NULL, restart_main_f},
	{"restart", NULL, restart_main},
	{"stop_running", NULL, stop_running_main},
	{"softwarerevision", NULL, softwarerevision_main},
#if !defined(HAVE_MICRO) || defined(HAVE_ADM5120) || defined(HAVE_WRK54G)
	{"watchdog", NULL, &watchdog_main},
#endif
	// {"nvram", NULL, &nvram_main},
#ifdef HAVE_ROAMING
//      {"roaming_daemon", NULL, &roaming_daemon_main},
	{"supplicant", "supplicant", NULL},
#endif
	{"get_wanface", "get_wanface", NULL},
#ifndef HAVE_XSCALE
	// {"ledtool", NULL, &ledtool_main},
#endif
#ifdef HAVE_REGISTER
	{"regshell", NULL, &reg_main},
#endif
	{"gratarp", NULL, &gratarp_main},
	{"get_nfmark", "get_nfmark", NULL},
#ifdef HAVE_IPV6
	{"dhcp6c-state", "dhcp6c_state",},
#endif
#ifdef HAVE_QTN
	{"qtn_monitor", NULL, &qtn_monitor_main},
#endif
};

int main(int argc, char **argv)
{
	airbag_init();
	char *base = strrchr(argv[0], '/');
	base = base ? base + 1 : argv[0];
	int i;
	int ret = 0;
	for (i = 0; i < sizeof(maincalls) / sizeof(struct MAIN); i++) {
		if (strstr(base, maincalls[i].callname)) {
			if (maincalls[i].execname)
				return start_main(maincalls[i].execname, argc, argv);
			if (maincalls[i].exec)
				return maincalls[i].exec(argc, argv);
		}
	}

	if (strstr(base, "startservice_f")) {
		if (argc < 2) {
			puts("try to be professional\n");
			goto out;
		}
		if (argc == 3 && !strcmp(argv[2], "-f"))
			start_service_force_f(argv[1]);
		else
			start_service_f(argv[1]);
		goto out;
	}
	if (strstr(base, "startservice")) {
		if (argc < 2) {
			puts("try to be professional\n");
			goto out;
		}
		if (argc == 3 && !strcmp(argv[2], "-f"))
			start_service_force(argv[1]);
		else
			start_service(argv[1]);
		goto out;
	}

	if (strstr(base, "stopservice_f")) {
		if (argc < 2) {
			puts("try to be professional\n");
			goto out;
		}
		if (argc == 3 && !strcmp(argv[2], "-f"))
			stop_service_force_f(argv[1]);
		else
			stop_service_f(argv[1]);
		goto out;
	}

	if (strstr(base, "stopservice")) {
		if (argc < 2) {
			puts("try to be professional\n");
			goto out;
		}
		if (argc == 3 && !strcmp(argv[2], "-f"))
			stop_service_force(argv[1]);
		else
			stop_service(argv[1]);
		goto out;
	}
#ifndef HAVE_RB500
#if (!defined(HAVE_X86) && !defined(HAVE_RB600)) || defined(HAVE_WDR4900)
	/* 
	 * erase [device] 
	 */
	if (strstr(base, "erase")) {
		int brand = getRouterBrand();

		if (brand == ROUTER_MOTOROLA || brand == ROUTER_MOTOROLA_V1 || brand == ROUTER_MOTOROLA_WE800G || brand == ROUTER_RT210W || brand == ROUTER_BUFFALO_WZRRSG54) {
			if (argv[1] && strcmp(argv[1], "nvram")) {
				fprintf(stderr, "Sorry, erasing nvram will turn this router into a brick\n");
				goto out;
			}
		} else {
			if (argv[1]) {
				ret = mtd_erase(argv[1]);
				goto out;
			} else {
				fprintf(stderr, "usage: erase [device]\n");
				ret = EINVAL;
				goto out;
			}
		}
		goto out;
	}

	/* 
	 * write [path] [device] 
	 */
	if (strstr(base, "write")) {
		if (argc >= 3) {
			ret = mtd_write(argv[1], argv[2]);
			goto out;
		} else {
			fprintf(stderr, "usage: write [path] [device]\n");
			ret = EINVAL;
			goto out;
		}
	}
#else
	if (strstr(base, "erase")) {
		if (argv[1] && strcmp(argv[1], "nvram")) {
			fprintf(stderr, "Erasing configuration data...\n");
			eval("mount", "/usr/local", "-o", "remount,rw");
			eval("rm", "-f", "/tmp/nvram/*");	// delete nvram database
			unlink("/tmp/nvram/.lock");	// delete nvram
			// database
			eval("rm", "-f", "/etc/nvram/*");	// delete nvram database
			eval("mount", "/usr/local", "-o", "remount,ro");
		}
		goto out;
	}
#endif
#endif

	/* 
	 * rc [stop|start|restart ] 
	 */
	else if (strstr(base, "rc")) {
		if (argv[1]) {
			if (strncmp(argv[1], "start", 5) == 0) {
				ret = kill(1, SIGUSR2);
			} else if (strncmp(argv[1], "stop", 4) == 0) {
				ret = kill(1, SIGINT);
			} else if (strncmp(argv[1], "restart", 7) == 0) {
				ret = kill(1, SIGHUP);
			}
			goto out;
		} else {
			fprintf(stderr, "usage: rc [start|stop|restart]\n");
			ret = EINVAL;
			goto out;
		}
	}

	ret = 1;
      out:;
	airbag_deinit();
	return ret;
}
