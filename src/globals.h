#pragma once
#include "common_includes.h"
#include "sdk/interfaces.h"
#include "singleton.h"
#include "menu/menu.h"

namespace globals {
    namespace angles {
        extern qangle_t		m_view;
        extern qangle_t		m_anim;
    }

    namespace game {
        extern auto ConsoleLog(std::string_view msg, col_t color) -> void;
    }

    struct visualStruct {
        bool enabled;
        int type;
        ImVec4 primary;
        ImVec4 secondary;
    };

	namespace config
	{

        extern ImVec4 backgroundColor;
        extern ImVec4 colorScene;
        extern ImVec4 secondaryColor;

        extern KeyBind menuKey;
        extern KeyBind QuickPeekKey;
        extern KeyBind forceBaimKey;
        extern KeyBind DoubleTapKey;
        extern KeyBind HideshotsKey;
        extern KeyBind IdealTickKey;
        extern KeyBind HitchanceOverrideKey;
        extern KeyBind MinDamageOverrideKey;
        extern KeyBind ThirdPersonKey;

        // Aimbot
        extern bool aimbotEnabled;
        extern bool aimbotAutoShoot;
        extern bool silentAimEnabled;
        extern bool aimbotAutowall;
        extern bool aimbotAutoscope;
        extern bool aimbotAutostop;
        extern bool aimbotQuickPeek;
        extern bool aimbotResolver;

        // Aimbot Hitboxes
        extern Hitbox hitboxTable[8];

        // Aimbot Multipoint
        extern Hitbox multipointTable[8];

        extern float multipointHead;
        extern float multipointBody;

        // Aimbot targeting
        extern int aimbotTargetSelection; // 0 - Distance, 1 - Health, 2 - Hitchance
        extern bool preferSafepoint;
        extern float aimbotHitchance;
        extern float HitchanceOverride;
        extern float aimbotMinDamage;
        extern float MinDamageOverride;

        // Autostop
        extern Combostruct autostopTable[6];

        // Anti-Aim
        extern bool antiAimEnabled;
        extern int antiAimPitch;
        extern int antiAimYaw;
        extern int antiAimYawMode; // 0 - Static, 1 - Fake, 2 - Jitter, 3 - Spin, 4 - Random, 5 - Distort, 6 - Dynamic
        extern int antiAimPitchMode; // 0 - Static, 1 - Down, 2 - Up, 3 - Random, 4 - Alternate, 5 - Fake Down, 6 - Fake Up

        // Visuals

        extern visualStruct chamsTable[3];

        extern bool visualsEnabled;
        extern bool chamsEnabled;
        extern bool espEnabled;
        extern bool espEnemies;
        extern bool espTeammates;
        extern bool espSelf;
        extern int espDistance;
        extern bool espHealth;
        extern bool espWeapon;
        extern bool espSkeleton;
        extern bool espBox;
        extern ImColor espColor; // White color by default

        extern visualStruct glowTable[3];

        extern bool glowEnabled;
        extern float worldFov;
        extern bool nightMode;
        extern float nightScale;
        extern ImColor skyColor;

        // Misc
        extern bool bunnyHop;
        extern bool autoStrafe;
        extern float thirdPersonDistance;
        extern bool disableRecoil;
        extern bool disablePostProcessing;

        // Configurations
        extern std::string configName;

	}

    extern col_t ImColorToCol_T(const ImColor& imColor);

	extern HMODULE			m_module;
	extern c_local_player	m_local;
	extern c_user_cmd*		m_cur_cmd;
	extern bool				m_packet;
}