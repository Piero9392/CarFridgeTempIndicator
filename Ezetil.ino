#include <OneWire.h>                // Library for 1-Wire protocol (used by DS18B20 temperature sensors)
#include <DallasTemperature.h>      // Library for Dallas (Maxim) 1-Wire temperature sensors
#include <U8g2lib.h>                // Library for controlling the OLED display

#define ONE_WIRE_BUS 2              // Pin for the 1-Wire data bus, connected to the DS18B20 sensor
OneWire oneWire(ONE_WIRE_BUS);      // Initialize 1-Wire communication on the specified pin
DallasTemperature sensors(&oneWire);// Pass the 1-Wire instance to DallasTemperature library

// Initialize OLED display (128x64 resolution) using I2C, with no reset pin defined
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

void setup(void) {
  sensors.begin();                  // Start communication with the temperature sensor
  u8g2.begin();                     // Initialize the OLED display
  
  const char* text = "Ezetil";      // Text to display on OLED during startup animation
  int len = 0;                      // Initialize character count for animation
  u8g2.setFont(u8g2_font_ncenB24_tr); // Set font size for the startup text
  
  int16_t w = u8g2.getStrWidth(text); // Calculate the width of the startup text
  int16_t startX = (u8g2.getDisplayWidth() - w) / 2; // Center text horizontally
  int16_t startY = (u8g2.getDisplayHeight() + 24) / 2; // Center text vertically

  // Animation loop to display "Ezetil" one character at a time
  while (len <= strlen(text)) {
    u8g2.clearBuffer();            // Clear the screen buffer before each frame
    u8g2.setCursor(startX, startY); // Position the cursor for centered text
    for (int i = 0; i < len; i++) {
      u8g2.print(text[i]);          // Print each character up to the current length
    }
    u8g2.sendBuffer();              // Send the buffer to the display
    len++;                          // Increase character count
    delay(100);                     // Delay for smooth animation
  }
  delay(2000);                      // Pause briefly before starting temperature display
}

void loop() {
  sensors.requestTemperatures();     // Request temperature data from the sensor
  int temperature = sensors.getTempCByIndex(0); // Read temperature in Celsius

  u8g2.clearBuffer();                // Clear the screen buffer
  u8g2.setFont(u8g2_font_fub42_tn);  // Set a larger font for temperature display

  // Convert temperature to string for display
  String tempString = String(temperature);
  int16_t textWidth = u8g2.getStrWidth(tempString.c_str()); // Calculate text width
  int16_t centerX = (u8g2.getWidth() - textWidth) / 2;      // Center text horizontally
  int16_t centerY = (u8g2.getHeight() + u8g2.getAscent()) / 2; // Center text vertically

  u8g2.setCursor(centerX, centerY);  // Set cursor position for centered temperature text
  u8g2.print(tempString);            // Display the temperature on the OLED
  u8g2.sendBuffer();                 // Send the buffer to the display
  delay(1000);                       // Update the display every second
}
