#include "esp_camera.h"
#include "string.h"
#include "soc/soc.h"          
#include "soc/rtc_cntl_reg.h"


#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp32-hal-psram.h"
#include "Arduino.h"
#include <WiFi.h>

#define CAMERA_MODEL_AI_THINKER
#define DEGUB_ESP

#ifdef DEGUB_ESP
  #define DBG(x) Serial.println(x)
#else 
  #define DBG(...)
#endif

using namespace std;

const char* ssid = "Challence";
const char* password = "0001923880";

camera_fb_t *fb = NULL;  
static camera_config_t camera_config;

extern int LED =  4; // Light
String WiFiAddr = "";

void startCameraServer();

// Set Static IP address
IPAddress local_IP(192, 168, 137, 10);
// Set Gateway IP address
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress primaryDNS(8, 8, 8, 8); //optional
IPAddress secondaryDNS(8, 8, 4, 4); //optional

//---------------------------------------------------
void setup()
{
  #ifdef DEGUB_ESP
    Serial.begin(115200);
    Serial.setDebugOutput(true);
  #endif
  esp_log_level_set("*", ESP_LOG_VERBOSE);
  ESP_LOGE(TAG, "Free heap: %u", xPortGetFreeHeapSize());

  pinMode(LED, OUTPUT); //Light
  digitalWrite(LED, LOW);

  //use lq camera settings if the module credentials are not set yet.

  camera_config.ledc_channel = LEDC_CHANNEL_0;
  camera_config.ledc_timer   = LEDC_TIMER_0;
  camera_config.pin_d0       = 5;
  camera_config.pin_d1       = 18;
  camera_config.pin_d2       = 19;
  camera_config.pin_d3       = 21;
  camera_config.pin_d4       = 36;
  camera_config.pin_d5       = 39;
  camera_config.pin_d6       = 34;
  camera_config.pin_d7       = 35;
  camera_config.pin_xclk     = 0;
  camera_config.pin_pclk     = 22;
  camera_config.pin_vsync    = 25;
  camera_config.pin_href     = 23;
  camera_config.pin_sscb_sda = 26;
  camera_config.pin_sscb_scl = 27;
  camera_config.pin_pwdn     = 32;
  camera_config.pin_reset    = -1;
  camera_config.xclk_freq_hz = 20000000;
  camera_config.pixel_format = PIXFORMAT_JPEG;
  
  camera_config.frame_size = FRAMESIZE_VGA;  // set picture size, FRAMESIZE_VGA (640x480)
  /* Camera Sizes
  VGA
  CIF
  QVGA
  HQVGA
  QQVGA
  */
   
  camera_config.jpeg_quality = 15;           // quality of JPEG output. 0-63 lower means higher quality
  camera_config.fb_count = 2;              // 1: Wait for V-Synch // 2: Continous Capture (Video)

  //Use hq camera settings once the module is credentials are set.



 
 
  

  esp_err_t err = esp_camera_init(&camera_config);
  if (err != ESP_OK)
  {
    digitalWrite(LED, HIGH);
    sleep(0.25);
    digitalWrite(LED, LOW);
    sleep(0.25);
    digitalWrite(LED, HIGH);
    sleep(0.25);
    digitalWrite(LED, LOW);
    sleep(0.25);
    digitalWrite(LED, HIGH);
    sleep(0.25);
    digitalWrite(LED, LOW);
    sleep(0.25);
    Serial.println("Camera init failed with error 0x%x");
    Serial.println(err);
    
    delay(3000);
    ESP.restart();
    return;
  }
    if(psramFound())
    {
      Serial.println("PSRAM found and loaded");
    }
    ESP_LOGE(TAG, "Free heap after setup: %u", xPortGetFreeHeapSize());
    Serial.println("Setup done.");

  if (!WiFi.config(local_IP, gateway, subnet)) {
    Serial.println("STA Failed to configure");
  }
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(LED, HIGH);
    delay(250);
    digitalWrite(LED, LOW);
    delay(250);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  digitalWrite(LED, HIGH);
  sleep(0.5);
  digitalWrite(LED, LOW);

  startCameraServer();

  Serial.print("Camera Ready! Use 'http://");
  Serial.print(WiFi.localIP());
  WiFiAddr = WiFi.localIP().toString();
  Serial.println("' to connect");
}

//---------------------------FUNCTIONS-------------------------

//----------------------------------- LOOP  --------------------
void loop()
{
}