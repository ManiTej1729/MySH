#include "../universal_header.h"

void get_man_page(char *prompt) {
    char *hostname = "man.he.net";
    int port = 80;  // HTTP port
    char request[MAX_STR_LEN], response[MAX_STR_LEN];
    
    // Creating socket
    int client_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (client_sock < 0) {
        printf("\033[33mError creating socket\033[0m\n");
        exit(1);
    }

    // Resolving hostname
    struct hostent *server = gethostbyname(hostname);
    if (server == NULL) {
        printf("\033[33mNo such host: %s\033[0m\n", hostname);
        exit(1);
    }

    // Populating server address struct
    struct sockaddr_in server_address_struct;
    memset(&server_address_struct, 0, sizeof(server_address_struct));
    server_address_struct.sin_family = AF_INET;
    server_address_struct.sin_port = htons(80);  // HTTP port 80
    memcpy(&server_address_struct.sin_addr.s_addr, server->h_addr, server->h_length);

    // Connect to the server
    if (connect(client_sock, (struct sockaddr *)&server_address_struct, sizeof(server_address_struct)) < 0) {
        perror("Error connecting to server");
        exit(1);
    }

    // Create the GET request for the specified command
    snprintf(request, sizeof(request),
             "GET /?topic=%s&section=all HTTP/1.1\r\n"
             "Host: %s\r\n"
             "User-Agent: iMan/1.0\r\n"
             "Connection: close\r\n\r\n",
             prompt, hostname);

    // Send the request
    if (send(client_sock, request, strlen(request), 0) < 0) {
        perror("Error sending request");
        exit(1);
    }

    // Receive the response
    int start = 0;
    int inside_angular_bkts = 0;
    int bytes_received;
    while ((bytes_received = recv(client_sock, response, sizeof(response) - 1, 0)) > 0) {
        response[bytes_received] = '\0';

        int len = strlen(response);
        for (int i = 0; i < len; i++) {
            if (response[i] == '<') {
                inside_angular_bkts = 1;
                continue;
            }
            else if (response[i] == '>') {
                inside_angular_bkts = 0;
                continue;
            }
            if (start == 0 && strncasecmp("STRONG", response + i, 6) == 0) {
                start = 1;
            }
            if (start == 1 && inside_angular_bkts == 0) {
                printf("\033[32;40m%c\033[0m", response[i]);
            }
        }
    }

    if (bytes_received < 0) {
        perror("Error receiving response");
    }

    // Close the socket
    close(client_sock);
}
