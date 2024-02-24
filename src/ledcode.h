#pragma once

#include <headers.h>
#define colorSaturation 64

NeoPixelBus<NeoGrbFeature, NeoEsp8266Dma800KbpsMethod> strip(NUM_LEDS);
NeoBuffer<NeoBufferMethod<NeoGrbFeature>> klokImage(NUM_LEDS, 1, NULL);
NeoBuffer<NeoBufferMethod<NeoGrbFeature>> targetKlokImage(NUM_LEDS, 1, NULL);
NeoPixelAnimator animations(NUM_LEDS, NEO_CENTISECONDS);


RgbColor red(colorSaturation, 0, 0);
RgbColor green(0, colorSaturation, 0);
RgbColor blue(0, 0, colorSaturation);
RgbColor white(colorSaturation);
RgbColor black(0);

// define a custom shader object that provides brightness support
// based upon the NeoShaderBase
template <typename T_COLOR_FEATURE>
class BrightnessShader : public NeoShaderBase
{
public:
  BrightnessShader()
      : NeoShaderBase(),
        _brightness(48) // default to full bright
  {
  }

  // required for a shader object, it will be called for
  // every pixel
  void Apply(uint16_t index, uint8_t *pDest, const uint8_t *pSrc)
  {
    // we don't care what the index is so we ignore it
    //
    // to apply our brightness shader,
    // use the source color, modify, and apply to the destination

    // for every byte in the pixel,
    // scale the source value by the brightness and
    // store it in the destination byte
    const uint8_t *pSrcEnd = pSrc + T_COLOR_FEATURE::PixelSize;
    while (pSrc != pSrcEnd)
    {
      *pDest++ = (*pSrc++ * (uint16_t(_brightness) + 1)) >> 8;
    }
  }

  // provide an accessor to set brightness
  void setBrightness(uint8_t brightness)
  {
    _brightness = brightness;
    Dirty(); // must call dirty when a property changes
  }

  // provide an accessor to get brightness
  uint8_t getBrightness()
  {
    return _brightness;
  }

private:
  uint8_t _brightness;
};

// create an instance of our shader object with the same feature as our buffer
BrightnessShader<NeoGrbFeature> shader;

void ledSetup()
{
  randomSeed(analogRead(0));
  strip.Begin();
  clearLEDS();
}

void clearLEDS()
{
  strip.ClearTo(black);
  animations.StopAll();
  strip.Show();
  // ledTransit=true;
  // showFace(false);
}

void FadeAll(uint8_t darkenBy)
{
  RgbColor color;
  for (int indexPixel = OFFSET; indexPixel < strip.PixelCount(); indexPixel++)
  {
    color = strip.GetPixelColor(indexPixel);
    color.Darken(darkenBy);
    setColor(indexPixel, color);
  }
}




void ledShowClockface()
{

  clearLEDTo(klokImage, black);

  RgbColor color;
  RgbColor backgroundColor;

  color=red;

    setColor(0, color.Dim(0));
    setColor(1, color.Dim(20));
    setColor(2, color.Dim(40));
    setColor(3, color.Dim(60));
    setColor(4, color.Dim(80));
    setColor(5, color.Dim(100));
   
  showFace(true);
}

byte calcBrightness()
{
  byte brightness = 100;
  return brightness;
}

void setColor(int ledNr, NeoBufferMethod<NeoGrbFeature>::ColorObject color)
{
  // check if color changed
  // RgbColor currentColor = getColor(klokImage, ledNr);
  // RgbColor targetColor = color;
  klokImage.SetPixelColor(ledNr, 0, color);
}

RgbColor getColor(NeoBuffer<NeoBufferMethod<NeoGrbFeature>> &image, int ledNr)
{
  return image.GetPixelColor(ledNr, 0);
}

void clearLEDTo(NeoBuffer<NeoBufferMethod<NeoGrbFeature>> &image, NeoBufferMethod<NeoGrbFeature>::ColorObject color)
{
  image.ClearTo(color);
}

void showFace(bool doTransit)
{
  shader.setBrightness(150);
 
    klokImage.Render<BrightnessShader<NeoGrbFeature>>(strip, shader);
  //   for (int i=0;i<NUM_LEDS;i++){
  //     RgbColor color = strip.GetPixelColor(i);
  //     color=color.Dim(random(120));
  //     strip.SetPixelColor(i, color); 
  // }
    strip.Show();
}

RgbColor HueToRgbColor(int colorValue)
{
  if (colorValue == -1)
  {
    return RgbColor(white);
  }
  else
  {
    return RgbColor(HslColor(colorValue / 255.0, 1.0f, 0.5f));
  }
}
