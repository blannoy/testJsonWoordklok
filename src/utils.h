#pragma once
#include <headers.h>

#define debug_begin(...)         \
  do                             \
  {                              \
    if (DEBUG)                   \
    {                            \
      Serial.begin(__VA_ARGS__); \
      while (!Serial)            \
        ;                        \
    }                            \
  } while (0)
#define debug_print(...)         \
  do                             \
  {                              \
    if (DEBUG)                   \
      Serial.print(__VA_ARGS__); \
  } while (0)
#define debug_println(...)         \
  do                               \
  {                                \
    if (DEBUG)                     \
      Serial.println(__VA_ARGS__); \
  } while (0)
#define debug_printf(...)  \
  do                       \
  {                        \
    if (DEBUG)             \
      printf(__VA_ARGS__); \
  } while (0)
#define debug_println_wait(...)    \
  do                               \
  {                                \
    if (DEBUG)                     \
      Serial.println(__VA_ARGS__); \
    delay(2000);                   \
  } while (0)

#ifdef DEBUGMEM
#define reportmem(...)                                                     \
  do                                                                       \
  {                                                                        \
    if (DEBUGMEM)                                                          \
    {                                                                      \
      char stack;                                                          \
      float ratio = (float)ESP.getFreeContStack() / (float)beginFreeStack; \
        Serial.print(__VA_ARGS__);                                         \
        Serial.print(F(" - "));                                            \
        Serial.print(F(" stack "));                                        \
        Serial.print(stack_start - &stack);                                \
        Serial.print(F(" - "));                                            \
        Serial.print(ESP.getFreeContStack());                              \
        Serial.print(F(" / "));                                            \
        Serial.print(beginFreeStack);                                      \
        Serial.print(F(" heap "));                                         \
        Serial.print(ESP.getFreeHeap());                                   \
        Serial.print(F(" !!!!!!WARNING!!!!!"));                            \
        Serial.print(ratio);                                               \
        Serial.println();                                                  \
    }                                                                      \
  } while (0)
#else
#define reportmem(...)
#endif

char *stack_start;
uint32_t beginFreeStack;

void initMemLog()
{
#ifdef DEBUGMEM
  char stack;
  stack_start = &stack;
  beginFreeStack = ESP.getFreeContStack();
#endif
}

void randomColor(char hexArray[8])
{
  HslColor randomHueColor = HslColor(((float)random(360)) / 360, 1, 0.5);
  RgbColor randomRgbColor = RgbColor(randomHueColor);
  snprintf(hexArray, 8, "#%02X%02X%02X", randomRgbColor.R, randomRgbColor.G, randomRgbColor.B);
}

void hexToColorDef(const char *hex,colorDef* rgb)
{
  long long number = strtoll(&hex[1], NULL, 16);

  // Split them up into r, g, b values
  rgb->r = (uint8_t) (number >> 16);
  rgb->g = (uint8_t) (number >> 8 & 0xFF);
  rgb->b = (uint8_t) (number & 0xFF);
  }

void copyColorToJson(colorDef& color,JsonObject& jsonColor, const char* key){
   char hexColor[8];
   sprintf(hexColor,"#%02X%02X%02X",color.r,color.g,color.b);
   jsonColor[key]=hexColor;
}

void copyColorToJson(colorDef& color,JsonArray& jsonColor){
   char hexColor[8];
   sprintf(hexColor,"#%02X%02X%02X",color.r,color.g,color.b);
   jsonColor.add(hexColor);
}

RgbColor hexToRgb(const char *hex)
{
  colorDef rgb;
  hexToColorDef(hex,&rgb);
  return RgbColor(rgb.r, rgb.g, rgb.b);
}
