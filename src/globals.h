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

    struct ShotData {
        int shotIndex;
        int attackerIndex;
        int targetIndex;
        float time;
        float hitChance;
        e_hitboxes targetHitbox;
        c_base_combat_weapon* weapon;
        vec3_t impactPos;
        int damage;
        int hitgroup;
        bool kill;
        bool processed = false;
        std::string message;
    };

    struct visualStruct {
        bool enabled;
        int type;
        ImVec4 primary;
        ImVec4 secondary;
    };

    enum class EntityType {
        Player,
        NonPlayer
    };

    struct EntityData {
        i_client_entity* entity;
        EntityType type;

        bool operator==(const EntityData& other) const {
            return entity == other.entity && type == other.type;
        }
    };

    struct ESPSettings {
        bool enabled = false;
        float distance = 1000.f;
        bool box = false;
        bool healthText = false;
        bool healthBar = false;
        bool weapon = false;
        bool skeleton = false;
        ImColor boxPrimary = ImColor(255, 255, 255, 255);
        ImColor boxSecondary = ImColor(0, 0, 0, 255); 
        ImColor healthTextPrimary = ImColor(255, 255, 255, 255);
        ImColor HealthPrimary = ImColor(0, 255, 0, 255); 
        ImColor WeaponPrimary = ImColor(255, 255, 255, 255);
        ImColor SkeletonPrimary = ImColor(255, 255, 255, 255); 
    };

    struct ESPConfig {
        ESPSettings enemy;
        ESPSettings teammate;
        ESPSettings self;
    };

    namespace game {
        namespace user {
            extern bool can_shoot;
            extern bool is_frozen;
            extern bool last_shot;
            extern float lastShotTime;
            extern float lastScopeTime;
            extern bool TryingToShoot;

            extern std::unordered_map<int, ShotData> m_ShotData;
            extern int ShotCount;
        }
    }

	namespace config
	{

        extern ImVec4 backgroundColor;
        extern ImVec4 colorScene;
        extern ImVec4 textColor;
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

        // Autostop
        extern Combostruct autostopTable[7];

        extern float multipointHead;
        extern float multipointBody;

        // Aimbot targeting
        extern int aimbotTargetSelection; // 0 - Distance, 1 - Health, 2 - Hitchance
        extern bool preferSafepoint;
        extern float aimbotHitchance;
        extern float HitchanceOverride;
        extern float aimbotMinDamage;
        extern float MinDamageOverride;

        // Anti-Aim
        extern bool antiAimEnabled;
        extern float antiAimPitch;
        extern float antiAimYaw;
        extern float antiAimYawModifier;
        extern int antiAimYawBase;
        extern int antiAimYawMode;
        extern int antiAimYawModifierMode; // 0 - Static, 1 - Fake, 2 - Jitter, 3 - Spin, 4 - Random, 5 - Distort, 6 - Dynamic
        extern int antiAimPitchMode; // 0 - Static, 1 - Down, 2 - Up, 3 - Random, 4 - Alternate, 5 - Fake Down, 6 - Fake Up

        // Visuals

        extern visualStruct chamsTable[3];

        extern ESPConfig espConfig;

        extern bool visualsEnabled;
        extern bool chamsEnabled;
        extern bool espEnabled;

        extern visualStruct glowTable[3];

        extern bool glowEnabled;
        extern float worldFov;
        extern bool nightMode;
        extern float nightScale;
        extern bool worldModulation;
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

	extern HMODULE			m_module;
	extern c_local_player	m_local;
	extern c_user_cmd*		m_cur_cmd;
	extern bool				m_packet;
}