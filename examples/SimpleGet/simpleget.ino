#include <RestClient.h>

RestClient client = RestClient("google.es", 443);

void setup()
{
    Serial.begin(115200);
    delay(1000);

    client.begin("ssid", "password");

    int statusCode = client.get("/");
    Serial.print("Status code from server: ");
    Serial.println(statusCode);
}

void loop()
{
   
}
