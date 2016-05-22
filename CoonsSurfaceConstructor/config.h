#ifndef CONFIG
#define CONFIG

namespace CoonsPatches {
    enum PATCH_TYPES {
        BERNSTEIN,
        HERMITE
    };
    const double PRECISION = 0.000001;
    const int STEPS = 15;
    const bool DEBUG_MODE = false;//true;
    const bool SHOW_ADDITIONAL_VECTORS = DEBUG_MODE;//false;

    const int PATCH_TYPE = PATCH_TYPES::BERNSTEIN;//true;
}

#endif // CONFIG

