#pragma once
#include "headers.h"
bool validJSON = true;

Configuration *config;
Configuration workingConfig;
DynamicJsonDocument json(CONFIGSIZE);

void verifyConfigFile()
{

  debug_println(F("Verifying config"));
  File configFile = LittleFS.open("/config.json", "r");
  if (configFile)
  {
    auto deserializeError = deserializeJson(json, configFile);

    if (!deserializeError)
    {
      serializeJson(json, Serial);
      // Configuration conf = JSON2config(json);
      // printConfig(conf);
    }
    else
    {
      debug_println(F("failed to load json config"));
    }
  }
}


DynamicJsonDocument config2JSON(Configuration &conf)
{
 // DynamicJsonDocument doc(CONFIGSIZE);
  json.clear();
  JsonObject system = json[F("system")].to<JsonObject>();
  system[F("ntp_server")] = conf.ntp_server;
  system[F("hostname")] = conf.hostname;

  JsonObject jsonClockface = json[F("clockface")].to<JsonObject>();
  jsonClockface[F("wordGridHorizontal")] = NUM_COLS;
  jsonClockface[F("wordGridVertical")] = NUM_ROWS;
  jsonClockface[F("extraLEDs")] = OFFSET;

  // JsonArray layout = jsonClockface[F("layout")].to<JsonArray>();

  // for (uint8_t i = 0; i < NUMWORDS; i++)
  // {
  //   JsonObject wordConfig = layout.add<JsonObject>();
  //   wordConfig[F("word")] = config.clockface[i].label;
  //   JsonArray ledLayout = wordConfig[F("leds")].to<JsonArray>();
  //   for (uint8_t j = 0; j < sizeof(config.clockface[i].leds)/sizeof(int); j++)
  //   {
  //     ledLayout.add(config.clockface[i].leds[j]);
  //   }
  // }

  // JsonObject background = layout.add<JsonObject>();
  // background[F("background")] = config.clockface[NUMWORDS].label;
  // JsonArray ledLayout = background[F("leds")].to<JsonArray>();
  // for (uint8_t j = 0; j < sizeof(config.clockface[j].leds)/sizeof(int); j++)
  // {
  //   ledLayout.add(config.clockface[NUMWORDS].leds[j]);
  // }
  JsonObject colors = json[F("colors")].to<JsonObject>();

  switch (conf.ledMode)
  {
  case LedMode::singleColor:
    colors[F("ledMode")] = "singleColor";
    break;
  case LedMode::rainbowColor:
    colors[F("ledMode")] = "rainbowColor";
    break;
  case LedMode::hourlyColor:
    colors[F("ledMode")] = "hourlyColor";
    break;
  case LedMode::wordColor:
    colors[F("ledMode")] = "wordColor";
    break;
  default:
    break;
  }


  JsonObject colors_ledConfig = colors[F("ledConfig")].to<JsonObject>();

  JsonObject colors_ledConfig_singleColor = colors_ledConfig[F("singleColor")].to<JsonObject>();
  copyColorToJson(conf.singleColor.color,colors_ledConfig_singleColor,"color");
  copyColorToJson(conf.singleColor.backgroundColor,colors_ledConfig_singleColor,"backgroundColor");

  JsonObject colors_ledConfig_rainbowColor = colors_ledConfig[F("rainbowColor")].to<JsonObject>();
  colors_ledConfig_rainbowColor[F("cycleTime")] = conf.rainbowColor.cycleTime;
  copyColorToJson(conf.rainbowColor.backgroundColor,colors_ledConfig_rainbowColor,"backgroundColor");

  JsonObject colors_ledConfig_hourlyColor = colors_ledConfig[F("hourlyColor")].to<JsonObject>();
  JsonArray colors_ledConfig_hourlyColor_color = colors_ledConfig_hourlyColor[F("color")].to<JsonArray>();
  for (int i = 0; i < 24; i++)
  {
    copyColorToJson(conf.hourlyColor.color[i],colors_ledConfig_hourlyColor_color);
  }
  copyColorToJson(conf.hourlyColor.backgroundColor,colors_ledConfig_hourlyColor,"backgroundColor");

  JsonObject colors_ledConfig_wordColor = colors_ledConfig[F("wordColor")].to<JsonObject>();
  JsonArray colors_ledConfig_wordColor_color = colors_ledConfig_wordColor[F("color")].to<JsonArray>();
  for (int i = 0; i < NUMWORDS; i++)
  {
        copyColorToJson(conf.wordColor.color[i],colors_ledConfig_wordColor_color);
  }
copyColorToJson(conf.wordColor.backgroundColor,colors_ledConfig_wordColor,"backgroundColor");
  JsonObject brightness = json[F("brightness")].to<JsonObject>();
  ;
  switch (conf.brightnessMode)
  {
  case BrightnessMode::fixedBrightness:
    brightness[F("brightnessMode")] = "fixedBrightness";
    break;
  case BrightnessMode::ldrBrightness:
    brightness[F("brightnessMode")] = "ldrBrightness";
    break;
  case BrightnessMode::timeBrightness:
    brightness[F("brightnessMode")] = "timeBrightness";
    break;
  default:
    break;
  }

  JsonObject brightness_settings = brightness[F("settings")].to<JsonObject>();
  brightness_settings[F("fixedBrightness")][F("brightness")] = conf.fixedBrightness.brightness;

  JsonObject brightness_settings_ldrBrightness = brightness_settings[F("ldrBrightness")].to<JsonObject>();
  JsonObject brightness_settings_ldrBrightness_ldrRange = brightness_settings_ldrBrightness[F("ldrRange")].to<JsonObject>();
  brightness_settings_ldrBrightness_ldrRange[F("dark")] = conf.ldrBrightness.ldrRange.dark;
  brightness_settings_ldrBrightness_ldrRange[F("bright")] = conf.ldrBrightness.ldrRange.bright;

  JsonObject brightness_settings_ldrBrightness_brightness = brightness_settings_ldrBrightness[F("brightness")].to<JsonObject>();
  brightness_settings_ldrBrightness_brightness[F("min")] = conf.ldrBrightness.brightness.min;
  brightness_settings_ldrBrightness_brightness[F("max")] = conf.ldrBrightness.brightness.max;

  JsonObject brightness_settings_timeBrightness = brightness_settings[F("timeBrightness")].to<JsonObject>();
  JsonObject brightness_settings_timeBrightness_hourSlot = brightness_settings_timeBrightness[F("timeSlot")].to<JsonObject>();
  brightness_settings_timeBrightness_hourSlot[F("startHour")] = conf.timeBrightness.timeSlot.startHour;
  brightness_settings_timeBrightness_hourSlot[F("endHour")] = conf.timeBrightness.timeSlot.endHour;

  JsonObject brightness_settings_timeBrightness_brightness = brightness_settings_timeBrightness[F("brightness")].to<JsonObject>();
  brightness_settings_timeBrightness_brightness[F("min")] = conf.timeBrightness.brightness.min;
  brightness_settings_timeBrightness_brightness[F("max")] = conf.timeBrightness.brightness.max;

  if (strlen(conf.wifiConfig.static_ip) != 0)
  {
    json[F("wifi")][F("static_ip")] = conf.wifiConfig.static_ip;
    json[F("wifi")][F("static_gw")] = conf.wifiConfig.static_gw;
    json[F("wifi")][F("static_sn")] = conf.wifiConfig.static_sn;
    json[F("wifi")][F("static_dns1")] = conf.wifiConfig.static_dns1;
    json[F("wifi")][F("static_dns2")] = conf.wifiConfig.static_dns2;
  }

 // json[F("checksum")] = conf.checksum; 

  return json;
}

void copyString(char *configVal, JsonVariantConst variant)
{
  if (variant.isNull())
  {
    configVal[0]=0;
  }
  else
  {
    const char *value = variant.as<const char *>();
    strlcpy(configVal, value, strlen(value) + 1);
  }
}

void copyInt(uint8_t &configVal, JsonVariantConst variant)
{
  const int value = variant.as<const int>();
  memmove(&configVal, &value, sizeof(uint8_t));
}

void copyColor(colorDef &configVal, JsonVariantConst variant)
{
  const char *value = variant.as<const char *>();
  hexToColorDef(value, &configVal);
}

bool JSON2config(const JsonDocument &doc, Configuration &conf)
{
  reportmem("Start config");
  // debug_println(F("Got following JSON"));
  // serializeJson(doc,Serial);

  bool validJSON = true;
  // debug_println(json[F("system")][F("ntp_server")].as<String>());
  for (int i = 0; i < NUMKEYS; i++)
  {
    if (!doc.containsKey(configKeys[i]))
    {
     // debug_print(F("Missing key "));
     // debug_println(configKeys[i]);
      validJSON = false;
    }
  }
  if (validJSON)
  {

     wordGridHorizontal = json[F("clockface")][F("wordGridHorizontal")].as<unsigned int>();
     wordGridVertical = json[F("clockface")][F("wordGridVertical")].as<unsigned int>();
     extraLEDs = json[F("clockface")][F("extraLEDs")].as<unsigned int>();
     totalLeds = wordGridHorizontal * wordGridVertical + extraLEDs;
     JsonArray layout = json[F("clockface")][F("layout")];
     totalWords = layout.size() - 1;

    // debug_printf("Total words: %d\n", totalWords);
    // debug_printf("Total leds: %d\n", totalLeds);
     reportmem("Conversion started");

    if (json[F("system")][F("ntp_server")])
    {
      copyString(conf.ntp_server, json[F("system")][F("ntp_server")]);
    }
  reportmem("ntp");

    if (json[F("system")][F("hostname")])
    {
      copyString(conf.hostname, json[F("system")][F("hostname")]);
    }
      reportmem("hostname");

    //     /// Load clockface

    //      uint16_t clockSize=totalLeds * sizeof(int) + (totalWords + 1) * sizeof(int) + (totalLeds * sizeof(char) + (totalWords + 1) * sizeof(char)) + 20 * sizeof(char);
    //        debug_printf("Malloc needed: %d\n", clockSize); */
    //  memory needed for clockface:
    //  totalleds*sizeof(int)+(numwords+1)*sizeof(int) + (numleds * sizeof(char) + (numwords+1)*nullterminator) + 20*sizeof(char)
     //ClockfaceWord* cw=(ClockfaceWord *)malloc(totalLeds * sizeof(int) + (totalWords + 1) * sizeof(int) + (totalLeds * sizeof(char) + (totalWords + 1) * sizeof(char)) + 20 * sizeof(char));
    // conf.clockface = (ClockfaceWord *)malloc(totalLeds * sizeof(int) + (totalWords + 1) * sizeof(int) + (totalLeds * sizeof(char) + (totalWords + 1) * sizeof(char)) + 20 * sizeof(char));
    conf.clockface = (ClockfaceWord *)malloc((totalWords + 1)*sizeof(ClockfaceWord));
    if (!conf.clockface)
    {
      Serial.println("ERROR malloc conf.clockface");
    }
    for (int iWord = 0; iWord < totalWords + 1; iWord++)
    {
     // JsonArray jsonLeds = layout[iWord][F("leds")].to<JsonArray>();

      conf.clockface[iWord].label=(char*) malloc((layout[iWord][F("word")]).as<String>().length()+1);
      if (!conf.clockface[iWord].label)
      {
        Serial.println("ERROR malloc conf.clockface label");
      }
      copyString( conf.clockface[iWord].label,layout[iWord][F("word")]);
     // copyArray(conf.clockface[iWord].leds,jsonLeds);
     // copyString( conf.clockface[iWord].label,layout[iWord][F("word")]);
    //  conf.clockface[iWord].isActive = methodStringToMethod(layout[iWord][F("function")].as<String>());
     // conf.clockface[iWord].colorCodeInTable = iWord;
    }

    ///

    JsonObjectConst jsonColors = json[F("colors")].as<JsonObjectConst>();
    String ledMode = jsonColors[F("ledMode")].as<String>();
    if (ledMode == F("singleColor"))
    {
      conf.ledMode = LedMode::singleColor;
    }
    else if (ledMode == F("hourlyColor"))
    {
      conf.ledMode = LedMode::hourlyColor;
    }
    else if (ledMode == F("wordColor"))
    {
      conf.ledMode = LedMode::wordColor;
    }
    else if (ledMode == F("rainbowColor"))
    {
      conf.ledMode = LedMode::rainbowColor;
    }
    copyColor(conf.singleColor.color, jsonColors[F("ledConfig")][F("singleColor")][F("color")]);
    copyColor(conf.singleColor.backgroundColor, jsonColors[F("ledConfig")][F("singleColor")][F("backgroundColor")]);
    JsonObjectConst hourlyColor = jsonColors[F("ledConfig")][F("hourlyColor")];
    for (int i = 0; i < 24; i++)
    {
      copyColor(conf.hourlyColor.color[i], hourlyColor[F("color")][i]);
    }
    copyColor(conf.hourlyColor.backgroundColor, hourlyColor[F("backgroundColor")]);

    JsonObjectConst wordColor = jsonColors[F("ledConfig")][F("wordColor")];
    conf.wordColor.color = (colorDef *)malloc(totalWords * sizeof(colorDef));
    if (!conf.wordColor.color )
    {
      Serial.println("ERROR malloc conf.wordcolor.color");
    }
    for (int i = 0; i < totalWords; i++)
    {
      copyColor(conf.wordColor.color[i], wordColor[F("color")][i]);
    }
    copyColor(conf.wordColor.backgroundColor, wordColor[F("backgroundColor")]);

    conf.rainbowColor.cycleTime = jsonColors[F("ledConfig")][F("rainbowColor")][F("cycleTime")].as<unsigned long>();
    copyColor(conf.rainbowColor.backgroundColor, jsonColors[F("ledConfig")][F("rainbowColor")][F("backgroundColor")]);


    JsonVariantConst jsonBrightnessMode=json[F("brightness")][F("brightnessMode")];
    const char* brightnessMode = jsonBrightnessMode.as<const char*>();
    if (strcmp(brightnessMode,"fixedBrightness"))
    {
      conf.brightnessMode = BrightnessMode::fixedBrightness;
    }
    else if (strcmp(brightnessMode,"ldrBrightness"))
    {
      conf.brightnessMode = BrightnessMode::ldrBrightness;
    }
    else if (strcmp(brightnessMode,"timeBrightness"))
    {
      conf.brightnessMode = BrightnessMode::timeBrightness;
    }

    JsonObjectConst jsonBrightnessSettings = json[F("brightness")][F("settings")];
    copyInt(conf.fixedBrightness.brightness, jsonBrightnessSettings[F("fixedBrightness")][F("brightness")]);
    copyInt(conf.ldrBrightness.brightness.min, jsonBrightnessSettings[F("ldrBrightness")][F("brightness")][F("min")]);
    copyInt(conf.ldrBrightness.brightness.max, jsonBrightnessSettings[F("ldrBrightness")][F("brightness")][F("max")]);
    copyInt(conf.ldrBrightness.ldrRange.dark, jsonBrightnessSettings[F("ldrBrightness")][F("ldrRange")][F("dark")]);
    copyInt(conf.ldrBrightness.ldrRange.bright , jsonBrightnessSettings[F("ldrBrightness")][F("ldrRange")][F("bright")]);
    copyInt(conf.timeBrightness.brightness.min, jsonBrightnessSettings[F("timeBrightness")][F("brightness")][F("min")]);
    copyInt(conf.timeBrightness.brightness.max, jsonBrightnessSettings[F("timeBrightness")][F("brightness")][F("max")]); 
    copyInt(conf.timeBrightness.timeSlot.startHour, jsonBrightnessSettings[F("timeBrightness")][F("timeSlot")][F("startHour")]);
    copyInt(conf.timeBrightness.timeSlot.endHour, jsonBrightnessSettings[F("timeBrightness")][F("timeSlot")][F("endHour")]);

   reportmem("brightness");
 

    if (json[F("wifi")][F("static_ip")])
    {
      copyString(conf.wifiConfig.static_ip, json[F("wifi")][F("static_ip")]);
    }
    if (json[F("wifi")][F("static_gw")])
    {
      copyString(conf.wifiConfig.static_gw, json[F("wifi")][F("static_gw")]);
    }
    if (json[F("wifi")][F("static_sn")])
    {
      copyString(conf.wifiConfig.static_sn, json[F("wifi")][F("static_sn")]);
    }
    if (json[F("wifi")][F("static_dns1")])
    {
      copyString(conf.wifiConfig.static_dns1, json[F("wifi")][F("static_dns1")]);
    }
    if (json[F("wifi")][F("static_dns2")])
    {
      copyString(conf.wifiConfig.static_dns2, json[F("wifi")][F("static_dns2")]);
    } 
  }
  else
  {
    debug_println(F("Invalid JSON config"));
  }

  return true;
}


void printConfig(Configuration &conf)
{
  serializeJson(config2JSON(conf), Serial);
}

void loadConfiguration(Configuration *conf)
{

  debug_println(F("Loading configuration from filesystem"));
  validJSON = false;
  File configFile = LittleFS.open("/config.json", "r");
  if (configFile)
  {
    Serial.println(F("opened config file"));
    /*size_t size = configFile.size();
    std::unique_ptr<char[]> buf(new char[size]);
    configFile.readBytes(buf.get(), size);*/
    reportmem(F("file voor deserialize"));
    auto deserializeError = deserializeJson(json, configFile);
    reportmem(F("file na  deserialize"));
    configFile.close();
    if (!deserializeError)
    {
      // serializeJson(json,Serial);
     // serializeJsonPretty(json, Serial);
      validJSON = true;
      JSON2config(json, *conf);
    }
    else
    {
      debug_println(deserializeError.c_str());
      debug_println(F("failed to load json config"));
    }

  }
  else
  {
    debug_println(F("Cannot open config file"));
  }


  if (!validJSON)
  {
    debug_println(F("Invalid JSON"));
    Serial.println(F("Invalid JSON"));
    // loadDefaultConfiguration();
  }

  // copyConfig(config, oldConfig);
}

void configurationSetup()
{
  reportmem("test");
  config = (Configuration *)malloc(sizeof(Configuration));
  if (!config)
  {
    Serial.println("ERROR malloc configurationSetup");
  }

    reportmem("test");
  loadConfiguration(config);
}

void saveConfiguration(Configuration &conf)
{

  debug_println(F("Need to save configuration to Filesystem"));
  File configFile = LittleFS.open("/config.json", "w");
  if (!configFile)
  {
    debug_println(F("failed to open config file for writing"));
  }
  json = config2JSON(conf);
  serializeJson(json, configFile);
  configFile.close();
  debug_println(F("Closed file"));
}

void copyConfig(Configuration &confA, Configuration &confB)
{
  memcpy(&confA, &confB, sizeof(Configuration));
}
