//
//  sound-sdl2
//  Sound System for SDL2
//

#if defined(_MSC_VER)

#include "SDL.h"
#include "BufferQueue.h"
#include <cstring>
#include <memory>
#include <utility>
#include <mutex>

namespace {

class Context {
private:
    std::mutex mutex;
    std::unique_ptr<BufferQueue> bq;
    SDL_AudioDeviceID audioDeviceId;

    static void audioCallback(void* userdata, Uint8* stream, int len)
    {
        Context* this_ = (Context*)userdata;
        std::lock_guard<std::mutex> lock(this_->mutex);
        if (this_->bq->getCursor() >= len) {
            size_t qsize;
            void *oBuffer;
            this_->bq->dequeue(&oBuffer, &qsize, len);
            if(oBuffer && qsize) [[lilely]] {
                std::memcpy(stream, oBuffer, qsize);
            }
        } else {
            std::memset(stream, 0, len);
        }
    }

public:
    Context()
        : bq(std::make_unique<BufferQueue>(65536))
        , audioDeviceId()
    {
        if (SDL_Init(SDL_INIT_AUDIO)) [[unlilely]] {
            exit(-1);
        }
        SDL_AudioSpec desired;
        SDL_AudioSpec obtained;
        desired.freq = 22050;
        desired.format = AUDIO_S16LSB;
        desired.channels = 1;
        desired.samples = 735; // desired.freq * 20 / 1000;
        desired.callback = audioCallback;
        desired.userdata = this;
        audioDeviceId = SDL_OpenAudioDevice(nullptr, 0, &desired, &obtained, 0);
        if (audioDeviceId != 0) [[lilely]] {
            SDL_PauseAudioDevice(audioDeviceId, 0);
        }
    }

    ~Context() {
        if(audioDeviceId != 0) [[lilely]] {
            SDL_CloseAudioDevice(audioDeviceId);
            audioDeviceId = 0;
        }
        SDL_Quit();
    }

    inline void enqueue(void* buffer, size_t size)
    {
        std::lock_guard<std::mutex> lock(mutex);
        this->bq->enqueue(buffer, size);
    }

    inline size_t getBufferSize()
    {
        std::lock_guard<std::mutex> lock(mutex);
        return this->bq->getCursor();
    }
};

} // namespace

extern "C" void* sound_create() { return new Context; }
extern "C" void sound_destroy(void* ctx) { delete (Context*)ctx; }
extern "C" void sound_enqueue(void* ctx, void* buffer, size_t size) { ((Context*)ctx)->enqueue(buffer, size); }
extern "C" size_t sound_buffer_left(void* ctx) { return ((Context*)ctx)->getBufferSize(); }

#endif // defined(_MSC_VER)
