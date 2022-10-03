#include <string>
#include <vector>

enum SoundType { SOUND_MUSIC, SOUND_CHUNK };

class Sound
{
public:
    SoundType type;
    Mix_Music* music = NULL;
    Mix_Chunk* chunk = NULL;

    Sound()
    {

    }

    void load(SoundType type, std::string path)
    {
        this->type = type;
        load(path);
    }

    void load(std::string path)
    {
        if (this->type == SOUND_MUSIC)
        {
            music = Mix_LoadMUS(path.c_str());
        }
        else if (this->type == SOUND_CHUNK)
        {
            chunk = Mix_LoadWAV(path.c_str());
        }
    }

    void play()
    {
        if (type == SOUND_MUSIC)
        {
            Mix_PlayMusic(music, 0);
        }
        else if (type == SOUND_CHUNK)
        {
            Mix_PlayChannel(-1, chunk, 0);
        }
    }
};

class Texture 
{
public:
    SDL_Texture* texture = NULL;
    int x, y, w, h;
    int originW, originH;
    double angle = 0;

    Texture()
    {

    }

    Texture(std::string path)
    {
        load(path);
    }

    void load(std::string path)
    {
        SDL_Surface* load = IMG_Load(path.c_str());
        this->w = load->w;
        this->h = load->h;
        originW = this->w;
        originH = this->h;
        texture = SDL_CreateTextureFromSurface(renderer, load);// IMG_Load(path.c_str()));
        SDL_FreeSurface(load);
    }

    void render()
    {
        SDL_Rect rect = { x, y, w, h };
        SDL_RenderCopyEx(renderer, texture, NULL, &rect, angle, NULL, SDL_FLIP_NONE);
    }

    void renderAt(int x, int y, double angle)
    {
        int xx = x - w / 2, yy = y - h / 2;
        SDL_Rect rect = { xx, yy, w, h };
        SDL_RenderCopyEx(renderer, texture, NULL, &rect, angle, NULL, SDL_FLIP_NONE);
    }

    void copyFrom(Texture from)
    {
        texture = from.texture;
        x = from.x; y = from.y;
        w = from.w; h = from.h;
        originW = from.originW; originH = from.originH;
        angle = from.angle;
    }

    void setAngle(double angle)
    {
        this->angle = angle;
    }

    void setPos(int x, int y)
    {
        this->x = x;
        this->y = y;
    }

    void setCenterPos(int x, int y)
    {
        setPos(x - w / 2, y - h / 2);
    }

    void setSize(int w, int h)
    {
        this->w = w;
        this->h = h;
    }

    void setScale(float s)
    {
        w = originW * s;
        h = originH * s;
        int xCenter = x + originW / 2, yCenter = y + originH / 2;
        setCenterPos(xCenter, yCenter);
    }

    void setPosSize(int x, int y, int w, int h)
    {
        setPos(x, y);
        setSize(w, h);
    }

    void setCenterPosSize(int x, int y, int w, int h)
    {
        setCenterPos(x, y);
        setSize(w, h);
    }

    void setColor(Uint8 r, Uint8 g, Uint8 b)
    {
        SDL_SetTextureColorMod(texture, r, g, b);
    }

    void setAlpha(Uint8 a)
    {
        SDL_SetTextureAlphaMod(texture, a);
    }
};

//enum AnimeType {ANIME_GIF, ANIME_ANIMATION};
enum AnimationType {ANIMATION_FADE, ANIMATION_ZOOM, ANIMATION_MOVE};
struct Animation
{
    AnimationType type;
    float in, out;
    int x1, y1, x2, y2;
    Uint32 start, stop;

    Animation(AnimationType type, float in, float out, Uint32 start, Uint32 stop)
    {
        this->type = type;
        this->in = in;
        this->out = out;
        this->start = start;
        this->stop = stop;
    }

    Animation(AnimationType type, int x1, int y1, int x2, int y2, Uint32 start, Uint32 stop)
    {
        this->type = type;
        this->x1 = x1; this->y1 = y1;
        this->x2 = x2; this->y2 = y2;
        this->start = start; this->stop = stop;
    }
};

class Anime
{
public:
    Texture* target;
    //AnimeType type;
    std::vector<Animation> animations;
    Uint32 startTime;
    bool enableLoop = false;

    Anime()
    {
        //type = ANIME_ANIMATION;
    }

    Anime(Texture* texture)
    {
        setTarget(texture);
    }

    void setTarget(Texture* texture)
    {
        this->target = texture;
    }

    void addAnimetion(AnimationType type, float in, float out, Uint32 start, Uint32 stop)
    {
        Animation motion(type, in, out, start, stop);
        animations.push_back(motion);
    }

    void addAnimetion(AnimationType type, int x1, int y1, int x2, int y2, Uint32 start, Uint32 stop)
    {
        Animation motion(type, x1, y1, x2, y2, start, stop);
        animations.push_back(motion);
    }

    /*void load(std::string path, int frame, int fps)
    {
        if (type == ANIME_GIF)
        {
            this->fps = fps;
        }
    }*/

    void drive(Uint32 time)
    {
        for (std::vector<Animation>::iterator i = animations.begin(); i < animations.end(); i++)
        {
            if (i->type == ANIMATION_FADE)
            {
                applyFade(*i, time);
            }
            else if(i->type == ANIMATION_MOVE)
            {
                applyMove(*i, time);
            }
            else if (i->type == ANIMATION_ZOOM)
            {
                applyZoom(*i, time);
            }
        }
    }

    void play()
    {
        startTime = SDL_GetTicks();
        drive(startTime);
    }

    void applyFade(Animation motion, Uint32 time)
    {
        float in = motion.in, out = motion.out;
        Uint32 start = motion.start, stop = motion.stop;
        Uint32 current = time - startTime;
        if (enableLoop || (current >= start && current <= stop))
        {
            float p = (float)((current - start) % (stop - start)) / (stop - start);
            float a = in + p * (out - in);
            target->setAlpha(a);
        }
    }

    void applyMove(Animation motion, Uint32 time)
    {
        int x1 = motion.x1, y1 = motion.y1;
        int x2 = motion.x2, y2 = motion.y2;
        Uint32 start = motion.start, stop = motion.stop;
        Uint32 current = time - startTime;
        if (enableLoop || (current >= start && current <= stop))
        {
            float p = (float)((current - start) % (stop - start)) / (stop - start);
            //printf("time: %I32u, current: %I32u, p: %f\n", time, current, p);
            int x = x1 + p * (x2 - x1);
            int y = y1 + p * (y2 - y1);
            target->setCenterPos(x, y);
        }
    }

    void applyZoom(Animation motion, Uint32 time)
    {
        float in = motion.in, out = motion.out;
        Uint32 start = motion.start, stop = motion.stop;
        Uint32 current = time - startTime;
        if (enableLoop || (current >= start && current <= stop))
        {
            float p = (float)((current - start) % (stop - start)) / (stop - start);
            float s = in + p * (out - in);
            target->setScale(s);
        }
    }

    void setLoop(bool enable)
    {
        enableLoop = enable;
    }
};
