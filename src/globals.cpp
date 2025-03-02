#include "globals.h"

namespace globals {
	namespace angles {
		qangle_t    m_view = {};
		qangle_t    m_anim = {};
	}

    namespace game {
        namespace user {
            bool can_shoot = true;
            bool is_frozen = false;
            bool last_shot = false;
            float lastShotTime = 0.f;
            float lastScopeTime = 0.f;
            bool TryingToShoot = false;

            std::unordered_map<int, ShotData> m_ShotData;
            int ShotCount = 0;
        }
    }

    namespace config
    {
        ImVec4 backgroundColor = ImColor(0, 0, 0, 255);
        ImVec4 colorScene = ImColor(25, 25, 25, 255);
        ImVec4 secondaryColor = ImColor(100, 125, 208, 255);
        ImVec4 textColor = ImColor(255, 255, 255, 255);

        KeyBind menuKey = { VK_RSHIFT, 2, true };
        KeyBind QuickPeekKey = { 0, 0, false };
        KeyBind forceBaimKey = { 0, 0, false };
        KeyBind DoubleTapKey = { 0, 0, false };
        KeyBind HideshotsKey = { 0, 0, false };
        KeyBind IdealTickKey = { 0, 0, false };
        KeyBind HitchanceOverrideKey = { 0, 0, false };
        KeyBind MinDamageOverrideKey = { 0, 0, false };
        KeyBind ThirdPersonKey = { 0, 0, false };

        // Aimbot
        bool aimbotEnabled = false;
        bool aimbotAutoShoot = false;
        bool silentAimEnabled = false;
        bool aimbotAutowall = false;
        bool aimbotAutoscope = false;
        bool aimbotAutostop = false;
        bool aimbotQuickPeek = false;
        bool aimbotResolver = false;

        // Aimbot Hitboxes
        Hitbox hitboxTable[8] = {
            { "Head", false, { HITBOX_HEAD } },
            { "Neck", false, { HITBOX_NECK } },
            { "Upper Torso", false, { HITBOX_CHEST } },
            { "Lower Torso", false,  { HITBOX_STOMACH } },
            { "Pelvis", false, { HITBOX_PELVIS } },
            { "Arms", false, { HITBOX_LEFT_FOREARM, HITBOX_RIGHT_FOREARM } },
            { "Legs", false, { HITBOX_LEFT_THIGH, HITBOX_RIGHT_THIGH } },
            { "Feet", false, { HITBOX_LEFT_FOOT, HITBOX_RIGHT_FOOT } }
        };

        // Aimbot Multipoint
        Hitbox multipointTable[8] = {
            { "Head", false, { HITBOX_HEAD } },
            { "Neck", false, { HITBOX_NECK } },
            { "Upper Torso", false, { HITBOX_CHEST } },
            { "Lower Torso", false,  { HITBOX_STOMACH } },
            { "Pelvis", false, { HITBOX_PELVIS } },
            { "Arms", false, { HITBOX_LEFT_FOREARM, HITBOX_RIGHT_FOREARM } },
            { "Legs", false, { HITBOX_LEFT_THIGH, HITBOX_RIGHT_THIGH } },
            { "Feet", false, { HITBOX_LEFT_FOOT, HITBOX_RIGHT_FOOT } }
        };

        // Autostop
        Combostruct autostopTable[7] = {
            { "Early", false, 1 },
            { "Between Shots", false, 2 },
            { "Aggressive", false, 3 },
            { "Passive", false, 4 },
            { "Smart", false, 5 },
            { "Crouch", false, 6 },
            { "In Air", false, 7 },
        };

        float multipointHead = 0.0f;
        float multipointBody = 0.0f;

        // Aimbot targeting
        int aimbotTargetSelection = 0; // 0 - Distance, 1 - Health, 2 - Hitchance
        bool preferSafepoint = false;
        float aimbotHitchance = 100.0f;
        float HitchanceOverride = 1.0f;
        float aimbotMinDamage = 110.0f;
        float MinDamageOverride = 1.0f;

        // Anti-Aim
        bool antiAimEnabled = false;
        float antiAimPitch = 0;
        float antiAimYaw = 0;
        float antiAimYawModifier = 0;
        int antiAimYawBase = 0;
        int antiAimYawMode = 0;
        int antiAimYawModifierMode = 0; // 0 - Off, 1 - Static, 2 - Fake, 3 - Jitter, 4 - Spin, 5 - Random, 6 - Distort, 7 - Dynamic
        int antiAimPitchMode = 0; // 0 - Off, 1 - Static, 2 - Down, 3 - Up, 4 - Random, 5 - Alternate, 6 - Fake Down, 7 - Fake Up

        // Visuals

        visualStruct chamsTable[3] = { // 0 = Enemy, 1 = Friendly, 2 = Self
            {false, 0, ImVec4(1.f, 0.f, 0.f, 1.f), ImVec4(1.f, 1.f, 1.f, 1.f)},
            {false, 0, ImVec4(0.f, 1.f, 0.f, 1.f), ImVec4(1.f, 1.f, 1.f, 1.f)},
            {false, 0, ImVec4(0.f, 0.f, 1.f, 1.f), ImVec4(1.f, 1.f, 1.f, 1.f)}
        };

        ESPConfig espConfig; 

        bool visualsEnabled = false;
        bool chamsEnabled = false;
        bool espEnabled = false;

        visualStruct glowTable[3] = { // 0 = Enemy, 1 = Friendly, 2 = Self
            {false, 0, ImVec4(1.f, 0.f, 0.f, 1.f), ImVec4(1.f, 1.f, 1.f, 1.f)},
            {false, 0, ImVec4(0.f, 1.f, 0.f, 1.f), ImVec4(1.f, 1.f, 1.f, 1.f)},
            {false, 0, ImVec4(0.f, 0.f, 1.f, 1.f), ImVec4(1.f, 1.f, 1.f, 1.f)}
        };

        bool glowEnabled = false;
        float worldFov = 90.0f;
        bool nightMode = false;
        float nightScale = 0.01f;
        bool worldModulation = false;
        ImColor skyColor = ImColor(0, 0, 0);

        // Misc
        bool bunnyHop = false;
        bool autoStrafe = false;
        float thirdPersonDistance = 150.f;
        bool disableRecoil = false;
        bool disablePostProcessing = false;

        // Configurations
        std::string configName = "default.cfg";

    }

	HMODULE			m_module = nullptr;
	c_local_player	m_local = {};
	c_user_cmd*		m_cur_cmd = nullptr;
	bool			m_packet = true;
}