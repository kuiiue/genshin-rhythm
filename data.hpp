//#include <vector>
#include <deque>
#include <algorithm>
#include <sstream>
#include <iostream>

enum KeyType { KEY_NOTE, KEY_HOLD };

class Key 
{
public:
    KeyType type;
    int track;
    Uint32 start;
    Uint32 stop;
    int id;

    Key(KeyType type, int track, Uint32 start, Uint32 stop)
    {
        this->type = type;
        this->track = track;
        this->start = start;
        this->stop = stop;
    }
};

class Track 
{
public:
    int id;
    std::deque<Key> keys;

    Track(int id)
    {
        this->id = id;
    }

    void addKey(Key key)
    {
        //printf("add: id=%d, type=%d, track=%d, start=%lu, stop=%lu\n", id, key.type, key.track, key.start, key.stop);
        keys.push_back(key);
    }

    void addKey(KeyType type, Uint32 start, Uint32 stop)
    {
        Key key(type, id, start, stop);
        keys.push_back(key);
    }

    void printKeys()
    {
        for (int i = 0; i < keys.size(); i++)
        {
            Key t = keys.at(i);
            printf("print: id=%d, i=%d, type=%d, track=%d, start=%lu, stop=%lu\n", id, i, t.type, t.track, t.start, t.stop);
        }
    }

    struct CompareStruct
    {
        bool operator()(Key k1, Key k2)
        {
            return k1.start < k2.start;
        }
    } CompareObject;

    void sort()
    {
        std::sort(keys.begin(), keys.end(), CompareObject);
    }
};

class Chart
{
public:
    std::deque<Track> tracks;

    Chart()
    {
        for (int i = 0; i < 7; i++)
        {
            tracks.push_back(i);
        }
    }

    void load(std::string path)
    {
        std::stringstream stream;
        SDL_RWops* file = SDL_RWFromFile(path.c_str(), "r");
        size_t size = SDL_RWsize(file);
        char buff[1024] = {};
        int flag = 1;
        do
        {
            flag = SDL_RWread(file, buff, 1024, 1);
            //content += buff;
            stream << buff;
        } while (flag);
        //std::cout << "contentStream:" << std::endl << contentStream.str() << std::endl;
        std::string line;
        while (std::getline(stream, line))
        {
            if (line[0] == '#')
            {
                continue;
            }
            //std::cout << "line: " << line << std::endl;
            std::stringstream lines;
            lines << line;
            std::string temp;
            lines >> temp;
            std::string type(temp);
            lines >> temp;
            int id = std::stoi(temp);
            lines >> temp;
            Uint32 start = std::stoul(temp);
            Uint32 stop = 0;
            if (type == "hold")
            {
                lines >> temp;
                stop = std::stoul(temp);
            }
            //printf("type=%s, id=%d, start=%lu, stop=%lu\n", type.c_str(), id, start, stop);
            KeyType kt = KEY_NOTE;
            if (type == "hold")
            {
                kt = KEY_HOLD;
            }
            addKey(id, kt, start, stop);
            /*Key k(kt, id, start, stop);
            tracks.at(id).addKey(k);*/
        }
        for (int i = 0; i < tracks.size(); i++)
        {
            tracks.at(i).sort();
            //tracks.at(i).printKeys();
        }
    }

    void addKey(int track, KeyType type, Uint32 start, Uint32 stop)
    {
        //Key key(type, track, start, stop);
        tracks.at(track).addKey(type, start, stop);
    }
};
