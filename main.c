#include <stdio.h>
#include <stdlib.h>
#include "a2_data_structures.h"
#include "a2_functions.h"

int main(void) {
    Node *nodeList = NULL;
    int rc;
    
    // 1. Get node information and build the node list.
    rc = get_node_info(&nodeList);
    if (rc != 0) {
        fprintf(stderr, "Error in get_node_info()\n");
        return EXIT_FAILURE;
    }
    print_nodes(nodeList);
    
    // 2. Process link information (and handle invalid nodes).
    rc = get_link_info(nodeList, "linkinfo1.txt");
    if (rc != 0) {
        fprintf(stderr, "Error in get_link_info()\n");
        return EXIT_FAILURE;
    }
    
    // 3. Build routing tables from routeinfo1.txt.
    rc = get_route_info(nodeList, "routeinfo1.txt");
    if (rc != 0) {
        fprintf(stderr, "Error in get_route_info()\n");
        return EXIT_FAILURE;
    }
    
    // 4. Simulate packet generation and forwarding.
    printf("\n--- Simulating Packet Forwarding (Initial Routes) ---\n");
    rc = packet_gen_and_forward(nodeList, 10);
    if (rc != 0) {
        fprintf(stderr, "Error in packet_gen_and_forward()\n");
        return EXIT_FAILURE;
    }
    
    // 5. Process rerouting information (simulate link failure update).
    rc = get_reroute_info(nodeList, "rerouteinfo.txt");
    if (rc != 0) {
        fprintf(stderr, "Error in get_reroute_info()\n");
        return EXIT_FAILURE;
    }
    
    // 6. Simulate packet generation and forwarding after rerouting.
    printf("\n--- Simulating Packet Forwarding (After Rerouting) ---\n");
    rc = packet_gen_and_forward(nodeList, 10);
    if (rc != 0) {
        fprintf(stderr, "Error in packet_gen_and_forward() after reroute\n");
        return EXIT_FAILURE;
    }
    
    // Cleanup.
    free_nodes(nodeList);
    return EXIT_SUCCESS;
}
