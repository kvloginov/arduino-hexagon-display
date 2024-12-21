#include <GyverGFX.h>

// #define MX_LED_AMOUNT 128
#define MX_LED_AMOUNT 37
#define MX_XY_VIRT_W 13
#define MX_XY_VIRT_H 13
#define MX_REAL_H (MX_XY_VIRT_H / 2) + 1
// not mapped pixel
#define NM UINT8_MAX

// BallMatrix
class BallMatrix : public GyverGFX
{
public:
    BallMatrix()
    {
        setModeXY();
    }

    // strip
    void setLED(uint8_t idx, uint32_t color);
    uint32_t getLED(uint8_t idx);
    void begin();
    void update();
    void setBright(uint8_t bright);
    void clear();

    // gfx
    void fastSet(int x, int y)
    {
        if (_buf16 != getColor())
        {
            _buf16 = getColor();
            _buf32 = getColor24full();
        }
        setLED(x, y, _buf32);
    }

    // matrix
    int ledXY(int x, int y);

    void setLED(int x, int y, uint32_t color)
    {
        int led = ledXY(x, y);
        if (led != NM)
            setLED(led, color);
    }
    uint32_t getLED(int x, int y)
    {
        int led = ledXY(x, y);
        return (led != NM) ? getLED(led) : 0;
    }

    void setModeXY()
    {
        _diag_mode = false;
        size(MX_XY_VIRT_W, MX_XY_VIRT_H);
    }

private:
    bool _diag_mode = false;
    uint16_t _buf16;
    uint32_t _buf32;
};

// TODO: remove all public variables
extern BallMatrix matrix;

struct LedSmoothed
{
    uint8_t led;
    uint8_t brightness; // 0-255
};

class HexoPolarSystemRing
{
    uint8_t radius;
    std::vector<uint8_t> leds;

public:
    HexoPolarSystemRing(uint8_t radius, std::vector<uint8_t> leds)
    {
        this->radius = radius;
        this->leds = leds;
    }
    uint8_t getLedsCount()
    {
        return leds.size();
    }
    std::vector<uint8_t> getLeds() { return leds; }
    std::vector<LedSmoothed> getLedByFract(float_t fract);
};

class HexoPolarSystem
{
    // from radius 0 to radius N
    std::vector<HexoPolarSystemRing> rings;

public:
    std::vector<LedSmoothed> getLedByFract(uint8_t radius, float_t fract);
    uint8_t getRingsCount()
    {
        return rings.size();
    }
    HexoPolarSystemRing getRing(uint8_t radius)
    {
        return rings[radius];
    }

    HexoPolarSystem(std::vector<HexoPolarSystemRing> rings)
    {
        this->rings = rings;
    }
};

// TODO: remove all public variables
extern HexoPolarSystem hexoPolarSystem;