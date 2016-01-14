#include <SoftwareSerial.h>

SoftwareSerial _wifi2(12, 13); // RX, TX
String _strResponse;

void _WriteCommand(const char *  val)
{
   _wifi2.write(val);
   //Serial.write(val);
   _wifi2.write("\r\n");
   //Serial.write("\r\n");
}
void _ReadResponse(void)
{
   _strResponse = "";
   uint16_t timeout = 500;
   do
   {
      if ( _wifi2.available())
      {
         char s=(unsigned char)_wifi2.read();
          _strResponse += s;
          //Serial.write(s);
      }
      delay(1);
      timeout--;
   }while ( timeout > 0);
}
String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length()-1;

  for(int i=0; i<=maxIndex && found<=index; i++){
    if(data.charAt(i)==separator || i==maxIndex){
        found++;
        strIndex[0] = strIndex[1]+1;
        strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  }

  return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
}
//------------------------------------------------------------------------------
// FONCTION : WIFI_Connect
//
// DESCRIPTION : Test de la presence du module wifi
//------------------------------------------------------------------------------
bool WIFI_Connect(void)
{
   _WriteCommand("AT");
   _ReadResponse();
   _strResponse = getValue(_strResponse,'\n',2);
   if ( _strResponse.startsWith("OK"))
   {
      return true;
   }
   else
   {
      return false;
   }
}

//------------------------------------------------------------------------------
// FONCTION : WIFI_GetIp
//
// DESCRIPTION : affichage de l'adresse IP
//------------------------------------------------------------------------------
String WIFI_GetIp(void)
{
   _WriteCommand("AT+CIFSR");
   _ReadResponse();
   return getValue(_strResponse,'\n',1);
}

void WIFI_ListAP(void)
{
   _WriteCommand("AT+CWLAP");
   _ReadResponse();

   _strResponse = getValue(_strResponse,'\n',2);
   Serial.println(_strResponse);
}


void WIFI_SetDeviceMode(void)
{
   _WriteCommand("AT+CWMODE=1");
   _ReadResponse();
}

void setup() 
{
  // put your setup code here, to run once:
  Serial.begin(57600);
  delay(100);
  Serial.println();
  Serial.println("********************************");
  Serial.println("****                        ****");
  Serial.println("********************************");
  Serial.println();
  Serial.print("Initialisatin Wifi : ... ");
  
  _wifi2.begin(9600);

  if ( WIFI_Connect() )
  {
    Serial.println("OK");
    WIFI_SetDeviceMode();
    Serial.println("Listing wifi a dispo : ...");
    WIFI_ListAP();
  }
 /* else
  {
    Serial.println("ERROR");
  }*/
}

void loop() {
  // put your main code here, to run repeatedly:
   if (_wifi2.available())
    Serial.write(_wifi2.read());
  if (Serial.available())
    _wifi2.write(Serial.read());
}
