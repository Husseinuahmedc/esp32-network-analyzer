#ifndef WS_SERVER_H
#define WS_SERVER_H

#include <ESPAsyncWebServer.h>

void initServer();
void cleanupClients();
void sendScanData(String json);

#endif
