enum HintType { HINT_PERFACT, HINT_GOOD, HINT_BAD };
enum KeyMotion { KEY_UP, KEY_DOWN }; 

class ComboDisplay
{
public:
    Texture numberTexture[10];
    int numberW, numberH;
    int centerX, centerY;
    Texture textTexture;
    Anime numberAnime, textAnime;
    int textW, textH;
    int combo = 0;

    ComboDisplay(int x, int y)
    {
        setCenterPos(x, y);
    }

    void setCenterPos(int x, int y)
    {
        centerX = x;
        centerY = y;
    }

    void loadNumberTexture(std::string path)
    {
        
    }

    void loadTextTexture(std::string path)
    {
        textTexture.load(path);
    }

    void setNumberAnime()
    {
        this->numberAnime = numberAnime;
    }

    void setTextAnime()
    {
        this->textAnime = textAnime;
    }

    void render()
    {

    }

    void drive(Uint32 time)
    {

    }

    void resetAnime()
    {

    }

    void addCombo()
    {
        resetAnime();
        combo++;
    }

    void resetCombo()
    {
        combo = 0;
    }
};

class HintDisplay
{
public:
    Texture textTexture[3];
    Texture backgroundTexture[3][2];
    int textW, textH;
    int backgroundW, backgroundH;
    int centerX, centerY;
    Anime textAnime, backgroundAnime;

    HintDisplay(int x, int y)
    {
        setCenterPos(x, y);
    }

    void setCenterPos(int x, int y)
    {
        centerX = x;
        centerY = y;
    }

    void loadTextTexture(std::string path)
    {

    }

    void loadBackgroundTexture(std::string path)
    {

    }

    void render(HintType type)
    {
        if (type == HINT_PERFACT)
        {

        }
        else if (type == HINT_GOOD)
        {

        }
        else if (type == HINT_BAD)
        {

        }
    }

    void drive(Uint32 time)
    {

    }

    void resetAnime()
    {

    }
};

class KeyDisplay
{
public:
    Key* key = NULL;
    int centerX = 0, centerY = 0;
    double angle = 0;
    ComboDisplay* comboDisplay = NULL;
    HintDisplay* hintDisplay = NULL;
    bool isEnabled = false;
    Texture holdBarTexture;
    Texture noteTexture, holdTexture;
    Anime effectTexture, effectAnime;

    KeyDisplay(Key* key)
    {
        this->key = key;
    }

    void calculateAngle()
    {

    }

    void calculateCenterPos()
    {

    }

    void render()
    {

    }

    void current(Uint32 time)
    {

    }

    void generateHoldBar()
    {

    }

    void disable()
    {
        isEnabled = false;
    }

    void enable()
    {
        isEnabled = true;
    }

    void event(KeyMotion motion)
    {

    }
};

class TrackDisplay
{
public:
    Track* track = NULL;
    int startX = 0, startY = 0;
    int bottomX = 0, bottomY = 0;
    int stopX = 0, stopY = 0;
    std::vector<KeyDisplay> keyDisplays;

    TrackDisplay(Track* track)
    {
        this->track = track;
    }

    void setPos(int arrayPos[7][2])
    {

    }

    void current(Uint32 time)
    {

    }

    void event(KeyMotion motion)
    {

    }

    KeyDisplay* getCurrentKeyDisplay()
    {

    }
};

class ChartDisplay
{
public:
    Chart* chart = NULL;
    Uint32 startTime = 0;
    std::vector<TrackDisplay> trackDisplays;

    ChartDisplay(Chart* chart)
    {
        this->chart = chart;
    }

    void init()
    {

    }

    void start(Uint32 tick)
    {
        startTime = tick;
    }

    void current(Uint32 tick)
    {
        Uint32 currentTime = tick - startTime;
        for (std::vector<TrackDisplay>::iterator i = trackDisplays.begin(); i < trackDisplays.end(); i++)
        {
            i->current(currentTime);
        }
    }

    void event(SDL_Event e)
    {
        
    }

    void drawConnectBar()
    {

    }

    void getCurrentKeyDisplays()
    {

    }
};

