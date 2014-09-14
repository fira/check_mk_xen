#include <sys/time.h>
#include <time.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <linux/kdev_t.h>

#include <xenstat.h>

#define __GNU_SOURCE

int main(void) {
	xenstat_handle *handle = xenstat_init();
	if(!handle) {
		fputs("Internal error in xenstats, failed to initialize handle\n", stderr);
		exit(1);
	}

	xenstat_node *node = xenstat_get_node(handle, XENSTAT_ALL); 
	if(!node) {
		fputs("Failed to get statistics from libxenlight\n", stderr);
		exit(2);
	}

	printf("<<<xen_info>>>\n"); 
	printf("version\t%s\n", xenstat_node_xen_version(node));
	printf("total_memory\t%llu\n", xenstat_node_tot_mem(node));
	printf("free_memory\t%llu\n", xenstat_node_free_mem(node));
//	printf("freeable_memory\t%li\n", xenstat_node_freeable_mb(node));
	printf("cpu_amount\t%i\n", xenstat_node_num_cpus(node));
	printf("cpu_speed\t%llu\n", xenstat_node_cpu_hz(node));

	int domains_amount = xenstat_node_num_domains(node);
	if(!domains_amount) exit(0);
	printf("domains\t%i\n", domains_amount);

	xenstat_domain** domains = malloc(sizeof(xenstat_domain*) * domains_amount);
	

	for(int i=0; i<domains_amount; i++) {
		xenstat_domain* domain = xenstat_node_domain_by_index(node, i);
		if(xenstat_domain_id(domain)) {
			printf("<<<<%s>>>>\n", xenstat_domain_name(domain));
		} else{ printf("<<<<>>>>\n"); }
		printf("<<<xen_domain>>>\n");
		printf("id\t%u\n", xenstat_domain_id(domain));
	}

	return 0;
}
