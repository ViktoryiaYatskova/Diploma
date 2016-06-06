#ifndef CONFIG
#define CONFIG

namespace CoonsPatches {
    enum PATCH_TYPES {
        BERNSTEIN,
        HERMITE
    };

    enum SURFACE_TYPES {
        HYPERBOLIC_PARABOLOID,
        ELLIPTIC_PARABOLOID,
        CONE,
        CUBIC
    };

    enum NORMAL_CALC_METHOD {
        METHOD1,
        METHOD2,
        METHOD3,
        METHOD4
    };

    const double PRECISION = 0.00001;
    const double STEP = 0.05;
    const bool DEBUG_MODE = false;//true;
    const bool SHOW_ADDITIONAL_VECTORS = false;

    const int PATCH_TYPE = PATCH_TYPES::BERNSTEIN;
    const int SURFACE_TYPE = SURFACE_TYPES::ELLIPTIC_PARABOLOID;
    const int NORMAL_METHOD = NORMAL_CALC_METHOD::METHOD1;
}

#endif // CONFIG

