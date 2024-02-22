/*
A few colour codes:

code  color
0x0000  Black
0xFFFF  White
0xBDF7  Light Gray
0x7BEF  Dark Gray
0xF800  Red
0xFFE0  Yellow
0xFBE0  Orange
0x79E0  Brown
0x7E0 Green
0x7FF Cyan
0x1F  Blue
0xF81F  Pink

 */
#include <ESP8266WiFi.h>
#include <TFT_eSPI.h> // Hardware-specific library
#include <SPI.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

#define LTBLUE    0xB6DF
#define LTTEAL    0xBF5F
#define LTGREEN   0xBFF7
#define LTCYAN    0xC7FF
#define LTRED     0xFD34
#define LTMAGENTA 0xFD5F
#define LTYELLOW  0xFFF8
#define LTORANGE  0xFE73
#define LTPINK    0xFDDF
#define LTPURPLE  0xCCFF
#define LTGREY    0xE71C
#define BLUE      0x001F
#define TEAL      0x0438
#define GREEN     0x07E0
#define CYAN      0x07FF
#define RED       0xF800
#define MAGENTA   0xF81F
#define YELLOW    0xFFE0
#define ORANGE    0xFC00
#define PINK      0xF81F
#define PURPLE    0x8010
#define GREY      0xC618
#define WHITE     0xFFFF
#define BLACK     0x0000
#define DKBLUE    0x000D
#define DKTEAL    0x020C
#define DKGREEN   0x03E0
#define DKCYAN    0x03EF
#define DKRED     0x6000
#define DKMAGENTA 0x8008
#define DKYELLOW  0x8400
#define DKORANGE  0x8200
#define DKPINK    0x9009
#define DKPURPLE  0x4010
#define DKGREY    0x4A49

#define TFT_GREY 0x5AEB
#define DISCONNECT_MESSAGE "!DISCONNECT"
#define REQUEST "REQUEST_DATA"
#define HEADER 128

//Setup WiFi connection
const char* ssid     = "##Your Wifi Name";
const char* password = "##Your Wifi Password";
char path[] = "/";
char host[] = "Your PC's local IP address (should be displayed when running server_ESP.py)";
int port = 5050;
WiFiClient client;

//Setup Date and time client
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");
//Week Days
String weekDays[7]={"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};

//Month names
String months[12]={"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

//Setup clock
TFT_eSPI tft = TFT_eSPI();       // Invoke custom library
TFT_eSprite spr = TFT_eSprite(&tft);
uint32_t targetTime = 0;                    // for next 1 second timeout
static uint8_t conv2d(const char* p); // Forward declaration needed for IDE 1.6.x
uint8_t hh = conv2d(__TIME__), mm = conv2d(__TIME__ + 3), ss = conv2d(__TIME__ + 6); // Get H, M, S from compile time
byte omm = 99, oss = 99;
byte xcolon = 0, xsecs = 0;
unsigned int colour = 0;

//System info
String C_temp;
String C_pow;
String G_temp;
String G_pow;
String G_mem;
String C_usg;
String G_usg;
String R_mem;
float C_Max = 0;
float G_Max = 0;
//Info Position
int C_labelx = 0;
int C_labely = 0;
int G_labelx = 0;
int G_labely = 87;
int R_labelx = 0;
int R_labely = 193;
int date_xpos = 0;
int date_ypos = 215;

unsigned long sTime;
unsigned long timer;
int interval = 750;

void setup(void) {
  Serial.begin(9600);
  tft.init();
  tft.setRotation(0);
  tft.fillScreen(TFT_BLACK);

  tft.setTextSize(2);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.println("Connecting to ");
  tft.println(ssid);
  delay(2000);
  WiFi.begin(ssid, password);
  tft.setTextWrap(false);
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(2);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    tft.print(".");
  }
  tft.println("");
  tft.println("WiFi connected");  
  //tft.println("IP address: ");
  //tft.println(WiFi.localIP());
  delay(2000);

  if (client.connect(host, port)) {
    tft.println("Connected");
    client.println("Hello from ESP8266");
    client.setNoDelay(true);
    client.print("REQUEST_DATA");
  } else {
    tft.println("Connection failed.");
    tft.println("Please Reset");
  }
  
  //Init time client
  timeClient.begin();
  // Set offset time in seconds to adjust for your timezone, for example:
  // GMT +1 = 3600
  // GMT +8 = 28800
  // GMT -1 = -3600
  // GMT 0 = 0
  timeClient.setTimeOffset(28800);
  
  tft.fillScreen(TFT_BLACK);
  tft.setTextSize(1);
  tft.setTextColor(TFT_YELLOW, BLACK);
  targetTime = millis() + 1000;
  timer = millis();

  /*
  spr.setColorDepth(8);
  spr.createSprite(200, 50);
  spr.fillSprite(0x0000);
  spr.drawRoundRect(0,0,200,16,8,0xFFFF);
  spr.fillRoundRect(3,3,6,10,5,0xFFFF);
  spr.drawString("Connecting to WiFi",100,40,2);
  spr.pushSprite(20,110);
  delay(2000);
  spr.deleteSprite();
  */
  drawUI();
}

void loop() {
  String data;
  Serial.println("Starting...");
  sTime = millis();
  if (millis() - timer > 500 && client.connected() && client.available() > 5) {
    data = client.readStringUntil('$$');
    Serial.print("Read time:");
    Serial.println(millis() - sTime);
    sTime = millis();
    if (data.indexOf("//") > 0) {
      FilterStr(data, C_temp, G_temp, C_usg, G_usg, G_mem, R_mem);
      tft.setTextColor(RED, BLACK);
      C_Max = max(C_temp.toFloat(), C_Max);
      G_Max = max(G_temp.toFloat(), G_Max);
      tft.drawString(C_usg, C_labelx + 150, C_labely, 2);
      tft.drawString(C_temp, C_labelx + 80, C_labely + 30, 7);
      tft.drawString(String(C_Max), C_labelx + 200, C_labely + 65, 2);
      tft.drawString(G_usg, G_labelx + 150, G_labely, 2);
      tft.drawString(G_temp, G_labelx + 80, G_labely + 30, 7);
      tft.drawString(String(G_Max), G_labelx + 200, G_labely + 65, 2);
      tft.drawString(G_mem, G_labelx + 100, G_labely + 85, 2);
      tft.drawString(R_mem, R_labelx + 100, R_labely, 2);
      Serial.print("Show time:");
      Serial.println(millis() - sTime);
      sTime = millis();
    }
    client.print("REQUEST_DATA");
    tft.setTextColor(YELLOW, BLACK);
    Serial.print("Write time:");
    Serial.println(millis() - sTime);
    sTime = millis();
  } else if (!client.connected()){
    tft.setTextColor(TFT_RED, BLACK);
    tft.drawString("Host disconnected.", 0, 0, 4);
    tft.drawString("Reconnecting", 0, 30, 4);
    if(client.connect(host, port)){
      tft.drawString("Connection Restored", 0, 0, 4);
      drawUI();
      client.print("REQUEST_DATA");
    }else{
      tft.drawString("Reconnection Failed", 0, 30, 4);
    }
  }
  Serial.println("Ending...");
  tft.setTextColor(YELLOW, BLACK);
  //Update Date and Time
  timeClient.update();
  String weekDay = weekDays[timeClient.getDay()];
  tft.drawString(weekDay, date_xpos + 85, date_ypos, 4);
  
  unsigned long epochTime = timeClient.getEpochTime();
  struct tm *ptm = gmtime ((time_t *)&epochTime); 
  int monthDay = ptm->tm_mday;
  tft.drawString(String(monthDay), date_xpos, date_ypos, 4);

  int currentMonth = ptm->tm_mon+1;
  String currentMonthName = months[currentMonth-1];
  tft.drawString(currentMonthName, date_xpos + 30, date_ypos, 4);

  String formattedTime = timeClient.getFormattedTime();
  tft.drawString(formattedTime, date_xpos + 135, date_ypos, 4);

}


// Function to extract numbers from compile time string
static uint8_t conv2d(const char* p) {
  uint8_t v = 0;
  if ('0' <= *p && *p <= '9')
    v = *p - '0';
  return 10 * v + *++p - '0';
}


void FilterStr(String &data, String &C_temp, String &G_temp, String &C_usg, String &G_usg, String &G_mem, String &R_mem){
  String delimiter = "//";
  String token;
  size_t pos = 0;
  size_t temp_pos = 0;
  while ((pos = data.indexOf(delimiter)) != std::string::npos){
    token = data.substring(0, pos);
    if(token.startsWith("CPU Temp")){
      temp_pos = token.indexOf(":");
      token = token.substring(temp_pos + 1, temp_pos + 5);
      C_temp = token;
    }else if(token.startsWith("CPU Usage")){
      temp_pos = token.indexOf(":");
      token = token.substring(temp_pos + 1, temp_pos + 7);
      C_usg = token;
    }else if(token.startsWith("GPU Temp")){
      temp_pos = token.indexOf(":");
      token = token.substring(temp_pos + 1, temp_pos + 5);
      G_temp = token;
    }else if(token.startsWith("GPU Usage")){
      temp_pos = token.indexOf(":");
      token = token.substring(temp_pos + 1, temp_pos + 7);
      G_usg = token;
    }else if(token.startsWith("Memory Usage")){
      temp_pos = token.indexOf(":");
      token = token.substring(temp_pos + 1, temp_pos + 7);
      R_mem = token;
      Serial.println(R_mem);
    }else if(token.startsWith("GPU Memory")){
      temp_pos = token.indexOf(":");
      token = token.substring(temp_pos + 1, temp_pos + 7);
      G_mem = token;
    }
    data.remove(0, pos + delimiter.length());
  }
}


void drawUI(){
  //Setup the UI
  tft.fillScreen(BLACK);
  tft.setTextColor(GREEN);
  tft.drawString("CPU", C_labelx, C_labely, 2);
  tft.drawString("Usage:", C_labelx + 100, C_labely, 2);
  tft.drawString("Temp:", C_labelx,  C_labely + 40, 4);
  tft.drawString("`C", C_labelx + 190, C_labely + 30, 2);
  tft.drawString("MAX", C_labelx + 205, C_labely + 45, 2);
  tft.drawFastHLine(0, 85, 240, WHITE);
  tft.drawString("GPU", G_labelx, G_labely, 2);
  tft.drawString("Usage:", G_labelx + 100, G_labely, 2);
  tft.drawString("Temp:", G_labelx, G_labely + 40, 4);
  tft.drawString("`C", G_labelx + 190, G_labely + 30, 2);
  tft.drawString("MAX", G_labelx + 205, G_labely + 45, 2);
  tft.drawString("GPU Memory:", G_labelx, G_labely + 85, 2);
  tft.drawFastHLine(0, 190, 240, WHITE);
  tft.drawString("RAM Usage:", R_labelx, R_labely, 2);
  tft.drawFastHLine(0, 212, 240, WHITE);
}


void backupClk(){
  //Update Clock
  if (targetTime < millis()) {
    // Set next update for 1 second later
    targetTime = millis() + 1000;

    // Adjust the time values by adding 1 second
    ss++;              // Advance second
    if (ss == 60) {    // Check for roll-over
      ss = 0;          // Reset seconds to zero
      omm = mm;        // Save last minute time for display update
      mm++;            // Advance minute
      if (mm > 59) {   // Check for roll-over
        mm = 0;
        hh++;          // Advance hour
        if (hh > 23) { // Check for 24hr roll-over (could roll-over on 13)
          hh = 0;      // 0 for 24 hour clock, set to 1 for 12 hour clock
        }
      }
    }


    // Update digital time
    int xpos = 135;
    int ypos = 215; // Top left corner ot clock text, about half way down
    int ysecs = ypos;

    if (omm != mm) { // Redraw hours and minutes time every minute
      omm = mm;
      // Draw hours and minutes
      if (hh < 10) xpos += tft.drawChar('0', xpos, ypos, 4); // Add hours leading zero for 24 hr clock
      xpos += tft.drawNumber(hh, xpos, ypos, 4);             // Draw hours
      xcolon = xpos; // Save colon coord for later to flash on/off later
      xpos += tft.drawChar(':', xpos, ypos, 4);
      if (mm < 10) xpos += tft.drawChar('0', xpos, ypos, 4); // Add minutes leading zero
      xpos += tft.drawNumber(mm, xpos, ypos, 4);             // Draw minutes
      xsecs = xpos; // Sae seconds 'x' position for later display updates
    }
    if (oss != ss) { // Redraw seconds time every second
      oss = ss;
      xpos = xsecs;

      if (ss % 2) { // Flash the colons on/off
        tft.setTextColor(0x39C4, TFT_BLACK);        // Set colour to grey to dim colon
        tft.drawChar(':', xcolon, ypos, 4);     // Hour:minute colon
        xpos += tft.drawChar(':', xsecs, ysecs, 4); // Seconds colon
        tft.setTextColor(TFT_YELLOW, TFT_BLACK);    // Set colour back to yellow
      }
      else {
        tft.drawChar(':', xcolon, ypos, 4);     // Hour:minute colon
        xpos += tft.drawChar(':', xsecs, ysecs, 4); // Seconds colon
      }

      //Draw seconds
      if (ss < 10) xpos += tft.drawChar('0', xpos, ysecs, 4); // Add leading zero
      tft.drawNumber(ss, xpos, ysecs, 4);                     // Draw seconds
    }
  }
}
