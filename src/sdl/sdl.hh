#ifndef __SDL_HH
#define __SDL_HH

namespace sdl {
class SDL {
   public:
    static void initialize();
    static void shutdown();

    static bool isInitialized() { return initialized; }

    static int displayWidth();
    static int displayHeight();

   private:
    static bool initialized;
};
}  // namespace sdl

#endif
