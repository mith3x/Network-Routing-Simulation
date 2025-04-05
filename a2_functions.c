#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "a2_data_structures.h"
#include "a2_functions.h"

#define BUFFER_SIZE 100

// Helper: Insert node into linked list in alphabetical order.
static void insert_node_sorted(Node **head, Node *newNode) {
    if (*head == NULL || strcmp(newNode->name, (*head)->name) < 0) {
        newNode->next = *head;
        *head = newNode;
    } else {
        Node *current = *head;
        while (current->next && strcmp(newNode->name, current->next->name) >= 0) {
            current = current->next;
        }
        newNode->next = current->next;
        current->next = newNode;
    }
}

// Reads nodes from "nodeinfo1.txt" and creates a sorted linked list.
int get_node_info(Node **nodeList) {
    FILE *fp = fopen("nodeinfo1.txt", "r");
    if (!fp) {
        perror("Error opening nodeinfo1.txt");
        return 1;
    }
    char buffer[BUFFER_SIZE];
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        // Remove newline if present.
        buffer[strcspn(buffer, "\r\n")] = '\0';
        char *token = strtok(buffer, ";");
        if (!token) continue;
        Node *newNode = (Node *)malloc(sizeof(Node));
        if (!newNode) {
            perror("Memory allocation error");
            fclose(fp);
            return 1;
        }
        strncpy(newNode->name, token, MAX_NAME_LEN);
        token = strtok(NULL, ";");
        if (token)
            strncpy(newNode->ip, token, MAX_IP_LEN);
        newNode->routingTable = NULL;
        newNode->next = NULL;
        // Insert in sorted order.
        insert_node_sorted(nodeList, newNode);
        printf("Added node %s with IP %s\n", newNode->name, newNode->ip);
    }
    fclose(fp);
    return 0;
}

// Prints all nodes in the list.
int print_nodes(Node *nodeList) {
    printf("\nNodes in network:\n");
    while (nodeList) {
        printf("Node %s, IP: %s\n", nodeList->name, nodeList->ip);
        nodeList = nodeList->next;
    }
    return 0;
}

// Frees the nodes list.
int free_nodes(Node *nodeList) {
    Node *temp;
    while (nodeList) {
        // Free routing table entries for each node.
        RouteEntry *entry = nodeList->routingTable;
        while (entry) {
            RouteEntry *tempEntry = entry;
            entry = entry->next;
            free(tempEntry);
        }
        temp = nodeList;
        nodeList = nodeList->next;
        free(temp);
    }
    return 0;
}

// Helper: Checks if a node with the given name exists in the node list.
Node* find_node(Node *nodeList, const char *name) {
    while (nodeList) {
        if (strcmp(nodeList->name, name) == 0)
            return nodeList;
        nodeList = nodeList->next;
    }
    return NULL;
}

// Reads link information from linkinfo1.txt and detects invalid nodes.
int get_link_info(Node *nodeList, const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        perror("Error opening link info file");
        return 1;
    }
    InvalidNode *invalidHead = NULL, *invalidTail = NULL;
    char buffer[BUFFER_SIZE];
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        buffer[strcspn(buffer, "\r\n")] = '\0';
        // Expected format: node1;node2;cost;bandwidth
        char *token = strtok(buffer, ";");
        if (!token) continue;
        char node1[MAX_NAME_LEN];
        char node2[MAX_NAME_LEN];
        strncpy(node1, token, MAX_NAME_LEN);
        token = strtok(NULL, ";");
        if (!token) continue;
        strncpy(node2, token, MAX_NAME_LEN);
        // Check if both nodes exist.
        if (!find_node(nodeList, node1)) {
            // Add to invalid list.
            InvalidNode *inv = (InvalidNode *)malloc(sizeof(InvalidNode));
            strncpy(inv->name, node1, MAX_NAME_LEN);
            inv->next = NULL;
            if (!invalidHead)
                invalidHead = invalidTail = inv;
            else {
                invalidTail->next = inv;
                invalidTail = inv;
            }
            printf("Invalid node detected: %s\n", node1);
        }
        if (!find_node(nodeList, node2)) {
            InvalidNode *inv = (InvalidNode *)malloc(sizeof(InvalidNode));
            strncpy(inv->name, node2, MAX_NAME_LEN);
            inv->next = NULL;
            if (!invalidHead)
                invalidHead = invalidTail = inv;
            else {
                invalidTail->next = inv;
                invalidTail = inv;
            }
            printf("Invalid node detected: %s\n", node2);
        }
        // We are not storing valid link info for simulation purposes.
    }
    fclose(fp);
    // Print invalid nodes.
    if (invalidHead) {
        printf("\nList of invalid nodes:\n");
        InvalidNode *curr = invalidHead;
        while (curr) {
            printf("Invalid Node: %s\n", curr->name);
            curr = curr->next;
        }
    }
    // Free invalid nodes list.
    while (invalidHead) {
        InvalidNode *temp = invalidHead;
        invalidHead = invalidHead->next;
        printf("Deleting invalid node: %s\n", temp->name);
        free(temp);
    }
    return 0;
}

// Reads routing info from routeinfo1.txt and builds each node's routing table.
// Each line format: src;dst;nextHop;cost
int get_route_info(Node *nodeList, const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        perror("Error opening route info file");
        return 1;
    }
    char buffer[BUFFER_SIZE];
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        buffer[strcspn(buffer, "\r\n")] = '\0';
        char *token = strtok(buffer, ";");
        if (!token) continue;
        char src[MAX_NAME_LEN], dst[MAX_NAME_LEN], nextHop[MAX_NAME_LEN];
        int cost;
        strncpy(src, token, MAX_NAME_LEN);
        token = strtok(NULL, ";");
        if (!token) continue;
        strncpy(dst, token, MAX_NAME_LEN);
        token = strtok(NULL, ";");
        if (!token) continue;
        strncpy(nextHop, token, MAX_NAME_LEN);
        token = strtok(NULL, ";");
        if (!token) continue;
        cost = atoi(token);
        // Find source node.
        Node *srcNode = find_node(nodeList, src);
        if (!srcNode) {
            printf("Source node %s not found in node list.\n", src);
            continue;
        }
        // Create new routing table entry.
        RouteEntry *newEntry = (RouteEntry *)malloc(sizeof(RouteEntry));
        strncpy(newEntry->destination, dst, MAX_NAME_LEN);
        strncpy(newEntry->nextHop, nextHop, MAX_NAME_LEN);
        newEntry->cost = cost;
        // Insert at front (as required).
        newEntry->next = srcNode->routingTable;
        srcNode->routingTable = newEntry;
        printf("For node %s, added route to %s via %s with cost %d\n", 
               src, dst, nextHop, cost);
    }
    fclose(fp);
    return 0;
}

// Reads updated routing info from rerouteinfo.txt and updates the routing tables.
// Each line format: src;dst;newNextHop;newCost
int get_reroute_info(Node *nodeList, const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        perror("Error opening reroute info file");
        return 1;
    }
    char buffer[BUFFER_SIZE];
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        buffer[strcspn(buffer, "\r\n")] = '\0';
        char *token = strtok(buffer, ";");
        if (!token) continue;
        char src[MAX_NAME_LEN], dst[MAX_NAME_LEN], newNextHop[MAX_NAME_LEN];
        int newCost;
        strncpy(src, token, MAX_NAME_LEN);
        token = strtok(NULL, ";");
        if (!token) continue;
        strncpy(dst, token, MAX_NAME_LEN);
        token = strtok(NULL, ";");
        if (!token) continue;
        strncpy(newNextHop, token, MAX_NAME_LEN);
        token = strtok(NULL, ";");
        if (!token) continue;
        newCost = atoi(token);
        // Find source node.
        Node *srcNode = find_node(nodeList, src);
        if (!srcNode) {
            printf("Source node %s not found for rerouting.\n", src);
            continue;
        }
        // Search for routing entry for destination.
        RouteEntry *entry = srcNode->routingTable;
        while (entry && strcmp(entry->destination, dst) != 0) {
            entry = entry->next;
        }
        if (entry) {
            // Update the entry.
            printf("Updating route for node %s to destination %s: ", src, dst);
            printf("Old nextHop: %s, Old cost: %d; ", entry->nextHop, entry->cost);
            strncpy(entry->nextHop, newNextHop, MAX_NAME_LEN);
            entry->cost = newCost;
            printf("New nextHop: %s, New cost: %d\n", entry->nextHop, entry->cost);
        } else {
            // If no entry exists, add new entry.
            RouteEntry *newEntry = (RouteEntry *)malloc(sizeof(RouteEntry));
            strncpy(newEntry->destination, dst, MAX_NAME_LEN);
            strncpy(newEntry->nextHop, newNextHop, MAX_NAME_LEN);
            newEntry->cost = newCost;
            newEntry->next = srcNode->routingTable;
            srcNode->routingTable = newEntry;
            printf("For node %s, added new reroute entry to %s via %s with cost %d\n",
                   src, dst, newNextHop, newCost);
        }
    }
    fclose(fp);
    return 0;
}

// Helper: Retrieve a random node from the node list.
static Node* get_random_node(Node *nodeList, int count) {
    int r = rand() % count;
    Node *current = nodeList;
    while (r-- && current)
        current = current->next;
    return current;
}

// Simulate packet generation and forwarding.
// Generates numPackets packets, then forwards each packet based on the routing table.
int packet_gen_and_forward(Node *nodeList, int numPackets) {
    // Count nodes in the list.
    int count = 0;
    Node *temp = nodeList;
    while (temp) {
        count++;
        temp = temp->next;
    }
    if (count < 2) {
        printf("Not enough nodes for packet simulation.\n");
        return 1;
    }
    int totalPackets = 0;
    int totalSize = 0;
    srand(time(NULL)); // seed random generator
    for (int i = 0; i < numPackets; i++) {
        Packet pkt;
        // Choose distinct random source and destination.
        Node *srcNode = get_random_node(nodeList, count);
        Node *dstNode = get_random_node(nodeList, count);
        while (dstNode == srcNode) {
            dstNode = get_random_node(nodeList, count);
        }
        strncpy(pkt.src, srcNode->name, MAX_NAME_LEN);
        strncpy(pkt.dst, dstNode->name, MAX_NAME_LEN);
        pkt.size = rand() % (1500 - 100 + 1) + 100;
        printf("\nPacket %d: %s -> %s, size %d bytes\n", i+1, pkt.src, pkt.dst, pkt.size);
        totalPackets++;
        totalSize += pkt.size;
        // Simulate packet forwarding.
        char current[MAX_NAME_LEN];
        strncpy(current, pkt.src, MAX_NAME_LEN);
        printf("Forwarding path: %s", current);
        while (strcmp(current, pkt.dst) != 0) {
            // Find current node.
            Node *currNode = find_node(nodeList, current);
            if (!currNode) {
                printf("\nError: current node %s not found!\n", current);
                break;
            }
            // Look for a routing entry that matches the destination.
            RouteEntry *entry = currNode->routingTable;
            int found = 0;
            while (entry) {
                if (strcmp(entry->destination, pkt.dst) == 0) {
                    printf(" -> %s", entry->nextHop);
                    strncpy(current, entry->nextHop, MAX_NAME_LEN);
                    found = 1;
                    break;
                }
                entry = entry->next;
            }
            if (!found) {
                printf("\nNo routing entry from node %s for destination %s\n", currNode->name, pkt.dst);
                break;
            }
        }
        printf("\nPacket forwarded. Current packet size: %d bytes; Total packets: %d; Total size: %d bytes\n",
               pkt.size, totalPackets, totalSize);
    }
    return 0;
}
