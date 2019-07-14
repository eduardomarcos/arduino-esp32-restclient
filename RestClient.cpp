#include "RestClient.h"

// Uncomment to debug
// #define ESP32_RESTCLIENT_DEBUG

#ifdef ESP32_RESTCLIENT_DEBUG
#define DEBUG_PRINT(x) Serial.print(x);
#else
#define DEBUG_PRINT(x)
#endif
//TODO: Add Timeout to send and read requests

const char *contentType = "text/plain";

RestClient::RestClient(const char *_host, const int _port)
{
    port = _port;
    host = _host;
    num_headers = 0;
}

int RestClient::begin(const char *ssid, const char *pass)
{
    WiFi.begin(ssid, pass);
    DEBUG_PRINT("\n[Connecting] [");
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        DEBUG_PRINT("-");
    }

    DEBUG_PRINT("]\n");
    DEBUG_PRINT("[Connected to: ");
    DEBUG_PRINT(ssid);
    DEBUG_PRINT("]\n");
    DEBUG_PRINT("[IP address: ");
    DEBUG_PRINT(WiFi.localIP());
    DEBUG_PRINT("]\n");
    return WiFi.status();
}

int RestClient::get(const char *path)
{
    return request("GET", path, NULL);
}

int RestClient::post(const char *path, const char *body)
{
    return request("POST", path, body);
}

void RestClient::setHeader(const char *header)
{
    headers[num_headers] = header;
    num_headers++;
}

void RestClient::setContentType(const char *contentTypeValue)
{
    contentType = contentTypeValue;
}

void RestClient::write(const char *string)
{
    DEBUG_PRINT(string);
    client_s.print(string);
}

void RestClient::writeHeaders()
{
    for (int i = 0; i < num_headers; i++)
    {
        write(headers[i]);
        write("\r\n");
    }
}

void RestClient::writeBody(const char *body)
{
    if (body != NULL)
    {
        char contentLength[30];
        sprintf(contentLength, "Content-Length: %d\r\n", strlen(body));
        write(contentLength);

        write("Content-Type: ");
        write(contentType);
        write("\r\n");
    }

    write("\r\n");

    if (body != NULL)
    {
        write(body);
        write("\r\n");
        write("\r\n");
    }
}

int RestClient::request(const char *method, const char *path, const char *body)
{
    int statusCode = -1;
    if (!client_s.connect(host, port))
    {
        DEBUG_PRINT("[Connection failed]\n");
        return 0;
    }
    DEBUG_PRINT("[Connected to: ");
    DEBUG_PRINT(host);
    DEBUG_PRINT("]\n");
    DEBUG_PRINT("[Request: \n");

    write(method);
    write(" ");
    write(path);
    write(" ");
    write("HTTP/1.1\r\n");
    writeHeaders();
    write("Host: ");
    write(host);
    write("\r\n");
    write("Connection: close\r\n");
    writeBody(body);

    DEBUG_PRINT("][End Request]\n");
    delay(100);
    DEBUG_PRINT("[Reading Response]\n");
    statusCode = readResponse();
    DEBUG_PRINT("[End Read Response]\n");
    DEBUG_PRINT("[Stoping client]\n");
    num_headers = 0;
    client_s.stop();
    delay(50);
    DEBUG_PRINT("[Client stopped]\n");
    return statusCode;
}

int RestClient::readResponse()
{
    boolean inStatus = false;
    char statusCode[4];
    int i = 0;
    int code = 0;

    while (client_s.connected() && client_s.available())
    {
        if (client_s.available())
        {
            char c = client_s.read();
            DEBUG_PRINT(c);

            if (c == ' ' && !inStatus)
            {
                inStatus = true;
            }

            if (inStatus && i < 3 && c != ' ')
            {
                statusCode[i] = c;
                i++;
            }
            if (i == 3)
            {
                statusCode[i] = '\0';
                code = atoi(statusCode);
            }
        }
    }
    return code;
}
