#include<Arduino.h>
#include<WiFi.h>
#include<HTTPClient.h>
void setup() 
{
  Serial.begin(115200);
  Serial.println();
  for(uint8_t t=4;t>0;t--)
  {
    Serial.printf("[setup] Wait %d...\n",t);
    Serial.flush();
    delay(1000);
  }

  WiFi.begin("sunil","8790225440");  
}
void loop() 
{
  if(WiFi.status()==WL_CONNECTED)
  {
    HTTPClient http;
    Serial.println("HTTP Begin...");
    http.begin("https://thingspeak.com/channels/277528/field/1/last.txt");
    Serial.println("HTTP GET...\n");
    int httpcode=http.GET();
    Serial.printf("The HttpCode is %d\n",httpcode);
    if(httpcode > 0)
    {
      Serial.printf("HTTP GET... code:%d\n",httpcode);
      if(httpcode == HTTP_CODE_OK)
      {
        int len=http.getSize();
        Serial.printf("The length is %d\n",len);
        uint8_t buff[128]={0};
        WiFiClient *stream = http.getStreamPtr();
        //Serial.println(*stream);
        Serial.print("The Http Value is:");
        Serial.println(http.connected());
        
        while(http.connected() && (len >0 || len==-1))
        {
          size_t size=stream->available();
          //Serial.print("size is:");Serial.println(size);
          if(size)
          {
            int c=stream->readBytes(buff,((size > sizeof(buff)) ? sizeof(buff):size));
            Serial.write(buff,c);
            Serial.println();
            //Serial.println(buff,c);
            Serial.printf("the C value is %d\n",c);
            if(len>0)
            {
              len -=c;
              Serial.printf("the Length2 is %d\n",len);
            }
             
           }delay(1);
          }
          Serial.println();
          Serial.println("HTTP Connection closed or file end\n");  
        }
      }
      else
      {
        Serial.printf("HTTP get... failed,error: %s\n",http.errorToString(httpcode).c_str());
        
      }
      http.end();
  }
  delay(1000);
}
