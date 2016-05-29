#ifndef CONFIG
#define CONFIG

namespace CoonsPatches {
    enum PATCH_TYPES {
        BERNSTEIN,
        HERMITE
    };
    const double PRECISION = 0.00001;
    const double STEP = 0.05;
    const bool DEBUG_MODE = false;//true;
    const bool SHOW_ADDITIONAL_VECTORS = false;

    const int PATCH_TYPE = PATCH_TYPES::BERNSTEIN;//true;
}

#endif // CONFIG

