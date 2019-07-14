#include <RestClient.h>

RestClient client = RestClient("host", 443);

void setup()
{
    Serial.begin(115200);
    delay(1000);

    client.begin("ssid", "password");
}

void loop()
{
    char postParameter[10];
    String postValue = "value=10";
    postValue.toCharArray(postParameter, 10);

    client.setHeader("Authorization: Basic FIUFHfpUHFpUFH39=");
    int statusCode = client.post("/post/write?db=dbName", postParameter);
    Serial.print("Status code from server: ");
    Serial.println(statusCode);

    delay(2000);
}
