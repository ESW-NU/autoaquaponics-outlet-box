#include <BluetoothSerial.h>

const int ledPIN = 4;
//const int LED2 = 26;
BluetoothSerial SerialBT;
byte BTData;

/* Check if Bluetooth configurations are enabled in the SDK */
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

/*
void Bt_Status (esp_spp_cb_event_t event, esp_spp_cb_param_t *param) {

  if (event == ESP_SPP_SRV_OPEN_EVT) {
    Serial.println ("Client Connected");
    digitalWrite (LED2, HIGH);
    // Do stuff if connected
  }

  else if (event == ESP_SPP_CLOSE_EVT ) {
    Serial.println ("Client Disconnected");
    digitalWrite (LED2, LOW);
    // Do stuff if not connected
  }
}*/

void setup()
{
  Serial.begin(115200);
  pinMode(ledPIN, OUTPUT);
  //pinMode (LED2, OUTPUT);

  // Define the Bt_Status callback
  //SerialBT.register_callback (Bt_Status);

  SerialBT.begin("ESP32-cam");
  Serial.println("Bluetooth Started! Ready to pair...");
}

void loop()
{
  if(SerialBT.available())
  {
    BTData = SerialBT.read();
    Serial.write(BTData);
  }

  /* If received Character is 1, then turn ON the LED
     You can also compare the received data with decimal equivalent
     48 for 0 and 49 for 1 
     if(BTData == 48) or if(BTData == 49) */
  if(BTData == '1')
  {
    digitalWrite(ledPIN, HIGH);
  }
  
  /* If received Character is 0, then turn OFF the LED */
  if(BTData == '0')
  {
    digitalWrite(ledPIN, LOW);
  }
}