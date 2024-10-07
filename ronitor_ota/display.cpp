#include "display.h"


TFT_eSPI tft = TFT_eSPI();  // Invoke library, pins defined in User_Setup.h
TFT_eSprite second_line = TFT_eSprite(&tft);
TFT_eSprite moon_background = TFT_eSprite(&tft);
TFT_eSprite moon_sprite = TFT_eSprite(&tft);

unsigned long color_settings[16] = {TFT_ORANGE,TFT_OLIVE,TFT_BLACK,TFT_WHITE,TFT_WHITE,TFT_BLACK,TFT_CYAN,TFT_WHITE,TFT_GREEN,TFT_GREENYELLOW,TFT_YELLOW,TFT_RED,TFT_GOLD,TFT_SKYBLUE,TFT_VIOLET};

FlickerFreePrint<TFT_eSPI> second_screen(&tft, color_settings[9], TFT_BLACK);
FlickerFreePrint<TFT_eSPI> hour_screen(&tft, color_settings[6], TFT_BLACK);
FlickerFreePrint<TFT_eSPI> minute_screen(&tft, color_settings[8], TFT_BLACK);
FlickerFreePrint<TFT_eSPI> dot_screen(&tft, color_settings[7], TFT_BLACK);
FlickerFreePrint<TFT_eSPI> day_screen(&tft, color_settings[11], TFT_BLACK);
FlickerFreePrint<TFT_eSPI> date_screen(&tft, color_settings[10], TFT_BLACK);
FlickerFreePrint<TFT_eSPI> temp_screen(&tft, color_settings[13], TFT_BLACK);
FlickerFreePrint<TFT_eSPI> humi_screen(&tft, color_settings[14], TFT_BLACK);
FlickerFreePrint<TFT_eSPI> moon_screen(&tft, color_settings[12], TFT_BLACK);

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "time.nist.gov", 6*3600, 60000);

// notes in the melody:
int melody[] = {

  NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4
};

// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = {

  4, 8, 8, 4, 4, 4, 4, 4
};

unsigned long lastDisplay = 0;
float temperature = 0;
float humidity = 0;
float *temp;
unsigned long unix_epoch;

char clock_string[10];
char old_clock_string[10];
int second_digit=0;
int old_second_digit=0;
int flag = 0;


char* city = "Dhaka";
char second_line_text[7][20];
char* country = "BD";
char condition[6] = "Cloud";
char condition_old[6];
int counter=0;
int cycle = 0;
int Icon;
int moon_icon=0 ;
int moon_illumination=0; 


void setColors(){
}

void showMoon(int moon_icon,float angle){
  moon_sprite.createSprite(60,60);
  moon_sprite.setSwapBytes(true); 
  switch(moon_icon){
    case 0: moon_sprite.pushImage(0,0,60,60,moon_0);break;
    case 1: moon_sprite.pushImage(0,0,60,60,moon_1);break;
    case 2: moon_sprite.pushImage(0,0,60,60,moon_2);break;
    case 3: moon_sprite.pushImage(0,0,60,60,moon_3);break;
    case 4: moon_sprite.pushImage(0,0,60,60,moon_4);break;
    case 5: moon_sprite.pushImage(0,0,60,60,moon_5);break;
    case 6: moon_sprite.pushImage(0,0,60,60,moon_6);break;
    case 7: moon_sprite.pushImage(0,0,60,60,moon_7);break;
    case 8: moon_sprite.pushImage(0,0,60,60,moon_8);break;
    case 9: moon_sprite.pushImage(0,0,60,60,moon_9);break;
    case 10: moon_sprite.pushImage(0,0,60,60,moon_10);break;
    case 11: moon_sprite.pushImage(0,0,60,60,moon_11);break;
    case 12: moon_sprite.pushImage(0,0,60,60,moon_12);break;
    case 13: moon_sprite.pushImage(0,0,60,60,moon_13);break;
    case 14: moon_sprite.pushImage(0,0,60,60,moon_14);break;
    case 15: moon_sprite.pushImage(0,0,60,60,moon_15);break;
    default : TJpgDec.drawFsJpg(0, 0, "/icons/05d.jpg", LittleFS);

    }
  moon_background.createSprite(60,60);
  //moon_sprite.pushSprite(165, 178); // Push the background sprite to the TFT display
  moon_sprite.pushRotated(&moon_background, angle, TFT_TRANSPARENT);
  moon_background.pushSprite(165, 178);
  //moon_sprite.deleteSprite();
  moon_sprite.setSwapBytes(false); 
}


/***************************************************************************************
**                          Update progress bar
***************************************************************************************/
void drawProgress(uint8_t percentage, String text) {
  tft.loadFont(SMALL_FONT);
  tft.setTextDatum(BC_DATUM);
  tft.setTextColor(TFT_ORANGE, TFT_BLACK);
  tft.setTextPadding(240);
  tft.drawString(text, 120, 260);
//ui.drawProgressBar(10, 269, 240 - 20, 15, percentage, TFT_WHITE, TFT_BLUE);
  tft.setTextPadding(0);
  tft.unloadFont();
}


void PlaySound(){
      /*
      for (int thisNote = 0; thisNote < 8; thisNote++) {
    // to calculate the note duration, take one second divided by the note type.

    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.

    int noteDuration = 1000 / noteDurations[thisNote];

    tone(BUZZERPIN, melody[thisNote], noteDuration);
    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(BUZZERPIN);

  }  */
      
      
      tone(BUZZERPIN, 523) ; //DO note 523 Hz
      delay (100); 
      tone(BUZZERPIN, 587) ; //RE note ...
      delay (100); 
      tone(BUZZERPIN, 659) ; //MI note ...
      delay (100); 
      tone(BUZZERPIN, 783) ; //FA note ...
      delay (100); 
      tone(BUZZERPIN, 880) ; //SOL note ...
      delay (100); 
      tone(BUZZERPIN, 987) ; //LA note ...
      delay (100); 
      tone(BUZZERPIN, 1046) ; // SI note ...
      delay (100); 
      noTone(BUZZERPIN) ; //Turn off the pin attached to the tone()
}

void DontWorry(){
  tft.loadFont(SMALL_FONT, LittleFS);
  tft.setCursor(50, 50);
  tft.setTextColor(TFT_ORANGE, TFT_BLACK);     // set text color to yellow and black background
  tft.printf("Don't Worry !");
  tft.setCursor(20, 80);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);     // set text color to yellow and black background
  tft.printf("Open your phone");
  tft.setCursor(5, 110);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);     // set text color to yellow and black background
  tft.printf("Connect WiFi: iQub3");
  }

void RTC_display()
{
  char day_matrix[7][4] = {"SUN", "MON", "TUE", "WED", "THU", "FRI", "SAT"};
  byte x_pos[7] = {29, 29, 23, 11, 17, 29, 17};
  static byte previous_dow = 0;

  
  tft.loadFont(SMALL_FONT,LittleFS);
  tft.setCursor(0, 0);
  tft.setTextDatum(MC_DATUM);
  tft.setTextColor(color_settings[0], TFT_BLACK);     // set text color to yellow and black background
  tft.drawString(city,65,16);

  if(!counter){
  tft.fillRoundRect(130, 3, 50, 25, 1, color_settings[1]);
  tft.setCursor(138, 5);
  tft.setTextDatum(BC_DATUM);
  tft.setTextColor(color_settings[2], color_settings[1]);     // set text color to yellow and black background
  tft.printf(country);
  counter=1;
  }
  
  if(strcmp(condition_old,condition)!=0){
  tft.setTextWrap(false);
  tft.setTextColor(color_settings[5], color_settings[4]);     // set text color to yellow and black background
  tft.fillRoundRect(170, 67, 70, 25, 1, color_settings[4]);
  tft.setCursor(172, 68);               // move cursor to position (4, 27) pixel
  tft.setTextDatum(MC_DATUM);
  tft.drawString(condition,206,80);
  strcpy(condition_old,condition);
  }
  
  //tft.setCursor(2, 35);
  //tft.setTextColor(TFT_WHITE, TFT_BLACK);     // set text color to yellow and black background
  second_line.loadFont(SMALL_FONT,LittleFS);
  second_line.setColorDepth(8);
  second_line.createSprite(180, 32);
  second_line.fillSprite(TFT_BLACK);
  second_line.setCursor(2, 35);
  second_line.setTextColor(color_settings[3]); // White text, no background
  second_line.setTextDatum(MC_DATUM);  // Bottom right coordinate datum
  second_line.drawString(second_line_text[cycle%6],second_line.width()/2, second_line.height()/2);
  second_line.pushSprite(2, 35);
  second_line.unloadFont();


  unsigned long now = millis();
  if (now - lastDisplay > 2*1000) {
    lastDisplay = now;
    (cycle==5) ? cycle=0 : cycle++;
    }
  
//  unsigned long now = millis();
  if (now - lastDisplay > 2*1000) {
    lastDisplay = now;
    (cycle==5) ? cycle=0 : cycle++;
    }
  
  tft.setSwapBytes(true); // Swap the byte order for pushImage() - corrects endianness

 // TJpgDec.drawFsJpg(180, 0, "/icons/08d.jpg", LittleFS);


  if(Icon == 1)
   TJpgDec.drawFsJpg(180, 0, "/icons/01d.jpg", LittleFS);
  else if(Icon == 2)
   TJpgDec.drawFsJpg(180, 0, "/icons/02d.jpg", LittleFS);
  else if(Icon == 3)
   TJpgDec.drawFsJpg(180, 0, "/icons/02d.jpg", LittleFS);
  else if(Icon == 4)
   TJpgDec.drawFsJpg(180, 0, "/icons/04d.jpg", LittleFS);
  else if(Icon == 5)
   TJpgDec.drawFsJpg(180, 0, "/icons/05d.jpg", LittleFS);
  else if(Icon == 6)
   TJpgDec.drawFsJpg(180, 0, "/icons/08d.jpg", LittleFS);
  else if(Icon == 7)
   TJpgDec.drawFsJpg(180, 0, "/icons/07d.jpg", LittleFS);
  else if(Icon == 8)
   TJpgDec.drawFsJpg(180, 0, "/icons/08d.jpg", LittleFS);
   else if(Icon == 11)
   TJpgDec.drawFsJpg(180, 0, "/icons/11d.jpg", LittleFS);
   else if(Icon == 12)
   TJpgDec.drawFsJpg(180, 0, "/icons/10d.jpg", LittleFS);
   else if(Icon == 13)
   TJpgDec.drawFsJpg(180, 0, "/icons/09d.jpg", LittleFS);
   else if(Icon == 14)
   TJpgDec.drawFsJpg(180, 0, "/icons/09d.jpg", LittleFS);
   else if(Icon == 5)
   TJpgDec.drawFsJpg(180, 0, "/icons/11d.jpg", LittleFS);
   else if(Icon == 16)
   TJpgDec.drawFsJpg(180, 0, "/icons/11d.jpg", LittleFS);
   else if(Icon == 17)
   TJpgDec.drawFsJpg(180, 0, "/icons/11d.jpg", LittleFS);
   else if(Icon == 18)
   TJpgDec.drawFsJpg(180, 0, "/icons/10d.jpg", LittleFS);
   else if(Icon == 19)
   TJpgDec.drawFsJpg(180, 0, "/icons/08d.jpg", LittleFS);
   else if(Icon == 20)
   TJpgDec.drawFsJpg(180, 0, "/icons/08d.jpg", LittleFS);
   else if(Icon == 21)
   TJpgDec.drawFsJpg(180, 0, "/icons/08d.jpg", LittleFS);
   else if(Icon == 22)
   TJpgDec.drawFsJpg(180, 0, "/icons/13d.jpg", LittleFS);
   else if(Icon == 23)
   TJpgDec.drawFsJpg(180, 0, "/icons/13d.jpg", LittleFS);
   else if(Icon == 24)
   TJpgDec.drawFsJpg(180, 0, "/icons/24.jpg", LittleFS);
   else if(Icon == 25)
   TJpgDec.drawFsJpg(180, 0, "/icons/25.jpg", LittleFS);
   else if(Icon == 26)
   TJpgDec.drawFsJpg(180, 0, "/icons/13n.jpg", LittleFS);
   else if(Icon == 29)
   TJpgDec.drawFsJpg(180, 0, "/icons/25.jpg", LittleFS);
   else if(Icon == 30)
   TJpgDec.drawFsJpg(180, 0, "/icons/30.jpg", LittleFS);
   else if(Icon == 31)
   TJpgDec.drawFsJpg(180, 0, "/icons/31.jpg", LittleFS);
   else if(Icon == 32)
   TJpgDec.drawFsJpg(180, 0, "/icons/32.jpg", LittleFS);
   else if(Icon == 33)
   TJpgDec.drawFsJpg(180, 0, "/icons/01n.jpg", LittleFS);
   else if(Icon == 34)
   TJpgDec.drawFsJpg(180, 0, "/icons/02n.jpg", LittleFS);
   else if(Icon == 35)
   TJpgDec.drawFsJpg(180, 0, "/icons/02n.jpg", LittleFS);
   else if(Icon == 36)
   TJpgDec.drawFsJpg(180, 0, "/icons/04n.jpg", LittleFS);
   else if(Icon == 37)
   TJpgDec.drawFsJpg(180, 0, "/icons/05n.jpg", LittleFS);
   else if(Icon == 38)
   TJpgDec.drawFsJpg(180, 0, "/icons/08d.jpg", LittleFS);
   else if(Icon == 39)
   TJpgDec.drawFsJpg(180, 0, "/icons/10n.jpg", LittleFS);
   else if(Icon == 40)
   TJpgDec.drawFsJpg(180, 0, "/icons/09n.jpg", LittleFS);
   else if(Icon == 41)
   TJpgDec.drawFsJpg(180, 0, "/icons/10n.jpg", LittleFS);
   else if(Icon == 42)
   TJpgDec.drawFsJpg(180, 0, "/icons/11n.jpg", LittleFS);
   else if(Icon == 43)
     TJpgDec.drawFsJpg(180, 0, "/icons/07d.jpg", LittleFS);
  else 
    TJpgDec.drawFsJpg(180, 0, "/icons/08d.jpg", LittleFS);


  showMoon(moon_icon,90-116.027);
  
  tft.setSwapBytes(false); // Swap the byte order for pushImage() - corrects endianness

  tft.setCursor(182, 150);
  tft.setTextColor(color_settings[12], TFT_BLACK);     // set text color to yellow and black background
  char moon_per[4];
  sprintf(moon_per,"%d%%",moon_illumination);
  moon_screen.print(moon_per);
  
  free(moon_per);
  // print date
  tft.setCursor(0, 150);
  tft.setTextColor(color_settings[11], TFT_BLACK);     // set text color to yellow and black background
  char date_string[10];
  sprintf(date_string, "%02u/%02u/%04u", day(unix_epoch), month(unix_epoch), year(unix_epoch) );
  date_screen.print(date_string);
  // print time, 
  free(date_string);

 //Print Day
 
  tft.setCursor(130, 150);
  int index = weekday(unix_epoch)-1;
  //Serial.println(day_matrix[index]);
  tft.setTextColor(color_settings[12], TFT_BLACK);     // set text color to yellow and black background
  day_screen.print(day_matrix[index]);
 
    tft.setTextColor(TFT_RED,TFT_BLACK);     // set text color to white and black background
    tft.setCursor(2,180);              // move cursor to position (43, 10) pixel
    char temp_data[20];
    sprintf(temp_data,"Temp:%.1f°C",temperature);
    //Serial.println(temp_data);
    temp_screen.print(temp_data);
    tft.setTextColor(TFT_BLUE,TFT_BLACK);     // set text color to white and black background
    tft.setCursor(2, 210);               // move cursor to position (4, 27) pixel
    char humi_data[20];
    sprintf(humi_data,"Humi:%.1f%%",humidity);
    humi_screen.print(humi_data);
    tft.unloadFont();

    tft.loadFont(MEDIUM_FONT,LittleFS);
    tft.setTextColor(TFT_WHITE,TFT_BLACK);     // set text color to white and black background
    tft.setCursor(190, 100);               // move cursor to position (4, 27) pixel
    char Second_string[2];
    second_digit = second(unix_epoch);
    sprintf(Second_string,"%02u",second_digit);
    second_screen.print(Second_string);

    free(Second_string);
    
    tft.unloadFont();
    if(second_digit!=old_second_digit)
    {
      old_second_digit = second_digit;
      flag = flag ^ 1;
    }
    
    tft.loadFont(BIG_FONT,LittleFS);
    tft.setTextColor(TFT_WHITE,TFT_BLACK);     // set text color to white and black background
    tft.setCursor(2, 75);               // move cursor to position (4, 27) pixel
    sprintf(clock_string,"%02u", hour(unix_epoch)); 
    hour_screen.print(clock_string);

    free(clock_string);
    
    tft.setTextColor(TFT_WHITE,TFT_BLACK);     // set text color to white and black background
    tft.setCursor(75, 75);               // move cursor to position (4, 27) pixel 
    
      if(flag)  {
        dot_screen.print(":");  
        }
      else
      {
        dot_screen.print(" ");
        }
      
   
    //dot_screen.print(":");  
    
    tft.setTextColor(TFT_WHITE,TFT_BLACK);     // set text color to white and black background
    tft.setCursor(95, 75);               // move cursor to position (4, 27) pixel
    sprintf(clock_string,"%02u", minute(unix_epoch) ); 
    minute_screen.print(clock_string);
    tft.unloadFont();
    free(clock_string);
}


void YouTube(String channelName,String sSubs,String sViews,String sVideos)
{
  Serial.println(sSubs);
  tft.setTextSize(1);                 // text size = 1
  tft.loadFont(SMALL_FONT,LittleFS);
  tft.fillScreen(TFT_BLACK);
  //.tft.drawBitmap(40, 10, yt, 43, 30,TFT_RED);
  tft.setCursor(5,50);
  tft.setTextColor(TFT_RED,TFT_BLACK);     // set text color to white and black background
  tft.print(channelName);
  //tft.drawBitmap(10, 70, subscriber, 28, 23,TFT_WHITE); 
  tft.setTextColor(TFT_WHITE,TFT_BLACK);     // set text color to white and black background

  tft.setCursor(10,105); 
  tft.print(sSubs);
  //tft.drawBitmap(55, 70, ytView, 25, 25,TFT_WHITE); 
  tft.setCursor(55,105);
  tft.print(sViews);
  //tft.drawBitmap(100, 70, vid, 25, 25,TFT_WHITE); 
  tft.setCursor(100,105);
  tft.print(sVideos);
  tft.unloadFont();
}


void YouTubeSubs(String channelName,int sSubs)
{
  Serial.println(sSubs);
  tft.setTextSize(1);                 // text size = 1
  tft.fillScreen(TFT_BLACK);
//  tft.drawBitmap(40, 10, yt, 43, 30,TFT_RED);
  tft.setCursor(5,50);
  tft.loadFont(BIG_FONT, LittleFS);
  tft.setTextColor(TFT_RED,TFT_BLACK);     // set text color to white and black background
  tft.print(channelName);
  //tft.unloadFont();
  //tft.loadFont(AA_FONT_LARGE);
  //tft.drawBitmap(50, 80, subscriber, 28, 23,TFT_WHITE); 
  tft.setTextColor(TFT_WHITE,TFT_BLACK);     // set text color to white and black background
  tft.setCursor(40,105);
  tft.print(sSubs);
  tft.unloadFont();
}

bool tft_output(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t* bitmap)
{
   // Stop further decoding as image is running off bottom of screen
  if ( y >= tft.height() ) return 0;

  // This function will clip the image block rendering automatically at the TFT boundaries
  tft.pushImage(x, y, w, h, bitmap);

  // Return 1 to decode next block
  return 1;
}
