
#pragma once
#include <ArduinoJson.h>
#include <NeoPixelBus.h>
#include <NeoPixelAnimator.h>
#include <EEPROM.h>
#include <utils.h>
#include <FS.h>
#include <LittleFS.h>
#include <coredecls.h> // crc32()

#define filesystem (LittleFS)

#define HOSTNAME_MAX 256
#define CONFIGSIZE 8192
#define NUM_ROWS 10
#define NUM_COLS 11
#define OFFSET 4 // 4 minute dots
#define MATRIX_LEDS NUM_ROWS *NUM_COLS
#define NUM_LEDS NUM_ROWS *NUM_COLS + OFFSET
#define NUMWORDS 27
#define NUMKEYS 5
String configKeys[NUMKEYS] = {"system", "clockface", "colors", "brightness", "checksum"};

uint8_t wordGridHorizontal;
uint8_t wordGridVertical;
uint8_t extraLEDs;
uint16_t totalLeds = 0;
uint8 totalWords = 0;

typedef enum
{
  alwaysOn,
  isJust,
  fiveMinute,
  tenMinute,
  quarter,
  twenty,
  past,
  before,
  wholeHour,
  halfHour,
  firstMinute,
  secondMinute,
  thirdMinute,
  fourthMinute,
  isOneActive,
  isTwoActive,
  isThreeActive,
  isFourActive,
  isFiveActive,
  isSixActive,
  isSevenActive,
  isEightActive,
  isNineActive,
  isTenActive,
  isElevenActive,
  isTwelveActive
} isActiveMethod;
static char *isActiveMethodStrings[] = {"alwaysOn", "isJust", "fiveMinute", "tenMinute", "quarter", "twenty", "past", "before", "wholeHour", "halfHour", "firstMinute", "secondMinute", "thirdMinute", "fourthMinute", "isOneActive", "isTwoActive", "isThreeActive", "isFourActive", "isFiveActive", "isSixActive", "isSevenActive", "isEightActive", "isNineActive", "isTenActive", "isElevenActive", "isTwelveActive"};

struct ClockfaceWord
{
  uint8_t *leds;
  int colorCodeInTable;
  char* label;
  isActiveMethod isActive;
};

#define BOOTANIMTIME 5000
String version = "0.5";

extern int PixelCount;

enum LedMode
{
  singleColor,
  wordColor,
  hourlyColor,
  rainbowColor
};

enum BrightnessMode
{
  fixedBrightness,
  ldrBrightness,
  timeBrightness
};

struct colorDef
{
  uint8_t r;
  uint8_t g;
  uint8_t b;
};

struct BrightnessDef
{
  uint8_t min;
  uint8_t max;
};

struct LdrRangeDef
{
  uint8_t dark;
  uint8_t bright;
};

struct WifiConfig
{
  char static_ip[16] = "";
  char static_gw[16] = "";
  char static_sn[16] = "";
  char static_dns1[16] = "";
  char static_dns2[16] = "";
};
struct SingleColorConfig
{
  colorDef color;
  colorDef backgroundColor;
};

struct RainbowColorConfig
{
  unsigned long cycleTime;
  colorDef backgroundColor;
};

struct WordColorConfig
{
  colorDef *color;
  colorDef backgroundColor;
};

struct HourlyColorConfig
{
  colorDef color[24];
  colorDef backgroundColor;
};

struct FixedBrightnessConfig
{
  uint8_t brightness;
};

struct LDRBrightnessConfig
{
  LdrRangeDef ldrRange;
  BrightnessDef brightness;
};

struct TimeSlotDef{
  uint8_t startHour;
  uint8_t endHour;
};

struct TimeBrightnessConfig
{
  TimeSlotDef timeSlot;
  BrightnessDef brightness;
};

struct Configuration
{
  char ntp_server[64];
  char hostname[64];
  ClockfaceWord *clockface;
  LedMode ledMode;
  WifiConfig wifiConfig;
  SingleColorConfig singleColor;
  RainbowColorConfig rainbowColor;
  WordColorConfig wordColor;
  HourlyColorConfig hourlyColor;
  BrightnessMode brightnessMode;
  FixedBrightnessConfig fixedBrightness;
  LDRBrightnessConfig ldrBrightness;
  TimeBrightnessConfig timeBrightness;
};

void clearLEDS();
void FadeAll(uint8_t);
void ledShowClockface();
byte calcBrightness();

void setColor(int, NeoBufferMethod<NeoGrbFeature>::ColorObject);
RgbColor getColor(NeoBuffer<NeoBufferMethod<NeoGrbFeature>> &, int);
void clearLEDTo(NeoBuffer<NeoBufferMethod<NeoGrbFeature>> &, NeoBufferMethod<NeoGrbFeature>::ColorObject);
void showFace(bool);
RgbColor HueToRgbColor(int);
isActiveMethod methodStringToMethod(String);