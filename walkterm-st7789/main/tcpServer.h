#include <sys/param.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include "esp_log.h"
#include "st7789.h"
#include "fontx.h"

#define PORT 1234

uint8_t ascii[64];
uint16_t xpos = (CONFIG_WIDTH-1)-16;
uint16_t ypos = 0;

FontxFile fx16G[2];

void tcp_server_task(TFT_t * dev) {
    static const char *TAG = "TCP_SERVER";

    lcdSetFontDirection(dev, 1);
    InitFontx(fx16G,"/fonts/ILGH16XB.FNT",""); // 8x16Dot Gothic

    char rx_buffer[128];
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);
    int listen_sock, client_sock;

    // Create a socket
    listen_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
    if (listen_sock < 0) {
        ESP_LOGE(TAG, "Unable to create socket: errno %d", errno);
        return;
    }

    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);

    // Bind the socket
    if (bind(listen_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        ESP_LOGE(TAG, "Socket unable to bind: errno %d", errno);
        close(listen_sock);
        return;
    }

    // Listen for connections
    if (listen(listen_sock, 1) < 0) {
        ESP_LOGE(TAG, "Error during listen: errno %d", errno);
        close(listen_sock);
        return;
    }

    ESP_LOGI(TAG, "Socket listening on port %d", PORT);

    while (1) {
        client_sock = accept(listen_sock, (struct sockaddr *)&client_addr, &addr_len);
        if (client_sock < 0) {
            ESP_LOGE(TAG, "Unable to accept connection: errno %d", errno);
            break;
        }
        ESP_LOGI(TAG, "Client connected");

        // Receive data
        int len = recv(client_sock, rx_buffer, sizeof(rx_buffer) - 1, 0);
        if (len < 0) {
            ESP_LOGE(TAG, "Recv failed: errno %d", errno);
        } else {
            rx_buffer[len] = '\0';
            ESP_LOGI(TAG, "Received %d bytes: '%s'", len, rx_buffer);
            send(client_sock, rx_buffer, len, 0); // Echo data back
            strcpy((char *)ascii, rx_buffer);
            lcdDrawString(dev, fx16G, xpos, ypos, ascii, GREEN);
        }

        close(client_sock);
    }

    close(listen_sock);
}
