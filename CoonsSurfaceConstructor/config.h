#ifndef CONFIG
#define CONFIG

namespace CoonsPatches {
    enum PATCH_TYPES {
        BERNSTEIN,
        HERMITE
    };
    const double PRECISION = 0.000001;
    const int STEPS = 25;
    const bool DEBUG_MODE = false;//true;

    const int PATCH_TYPE = PATCH_TYPES::BERNSTEIN;//true;
}

#endif // CONFIG

