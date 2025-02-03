#include "globals.h"

namespace globals {
	namespace angles {
		qangle_t    m_view = {};
		qangle_t    m_anim = {};
	}

    namespace game {
        auto ConsoleLog(std::string_view msg, col_t color) -> void
        {
            static constexpr int LS_HIGHEST_SEVERITY = 4;

            using find_channel_fn_t = int(__cdecl*)(const char* name);
            static const auto s_channel_id = ((find_channel_fn_t)GetProcAddress(GetModuleHandleA("tier0.dll"), "LoggingSystem_FindChannel"))("Console");

            using log_direct_fn_t = int(__cdecl*)(int id, int severity, col_t color, const char* msg);
            static const auto s_log_direct = (log_direct_fn_t)GetProcAddress(GetModuleHandleA("tier0.dll"), "LoggingSystem_LogDirect");

            std::string output = "[PRIMEVAL] | ";
            output.append(msg);
            output.append("\n");

            s_log_direct(s_channel_id, LS_HIGHEST_SEVERITY, color, output.data());
        }
    }

    namespace config
    {
        ImVec4 backgroundColor = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
        ImVec4 colorScene = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
        ImVec4 secondaryColor = ImVec4(0.85f, 0.85f, 0.85f, 1.00f);

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

        float multipointHead = 0.0f;
        float multipointBody = 0.0f;

        // Aimbot targeting
        int aimbotTargetSelection = 0; // 0 - Distance, 1 - Health, 2 - Hitchance
        bool preferSafepoint = false;
        float aimbotHitchance = 100.0f;
        float HitchanceOverride = 1.0f;
        float aimbotMinDamage = 110.0f;
        float MinDamageOverride = 1.0f;

        // Autostop
        Combostruct autostopTable[6] = {
            { "Early", false, 1 },
            { "Between Shots", false, 2 },
            { "Aggressive", false, 3 },
            { "Passive", false, 4 },
            { "Smart", false, 5 },
            { "Jumpscout", false, 6 },
        };

        // Anti-Aim
        bool antiAimEnabled = false;
        int antiAimPitch = 0;
        int antiAimYaw = 0;
        int antiAimYawMode = 0; // 0 - Static, 1 - Fake, 2 - Jitter, 3 - Spin, 4 - Random, 5 - Distort, 6 - Dynamic
        int antiAimPitchMode = 0; // 0 - Static, 1 - Down, 2 - Up, 3 - Random, 4 - Alternate, 5 - Fake Down, 6 - Fake Up

        // Visuals

        visualStruct chamsTable[3] = { // 0 = Enemy, 1 = Friendly, 2 = Self
            {false, 0, ImVec4(1.f, 0.f, 0.f, 1.f), ImVec4(1.f, 1.f, 1.f, 1.f)},
            {false, 0, ImVec4(0.f, 1.f, 0.f, 1.f), ImVec4(1.f, 1.f, 1.f, 1.f)},
            {false, 0, ImVec4(0.f, 0.f, 1.f, 1.f), ImVec4(1.f, 1.f, 1.f, 1.f)}
        };

        bool visualsEnabled = false;
        bool chamsEnabled = false;
        bool espEnabled = false;
        bool espEnemies = false;
        bool espTeammates = false;
        bool espSelf = false;
        int espDistance = 500;
        bool espHealth = false;
        bool espWeapon = false;
        bool espSkeleton = false;
        bool espBox = false;
        ImColor espColor = ImColor(255, 255, 255); // White color by default

        visualStruct glowTable[3] = { // 0 = Enemy, 1 = Friendly, 2 = Self
            {false, 0, ImVec4(1.f, 0.f, 0.f, 1.f), ImVec4(1.f, 1.f, 1.f, 1.f)},
            {false, 0, ImVec4(0.f, 1.f, 0.f, 1.f), ImVec4(1.f, 1.f, 1.f, 1.f)},
            {false, 0, ImVec4(0.f, 0.f, 1.f, 1.f), ImVec4(1.f, 1.f, 1.f, 1.f)}
        };

        bool glowEnabled = false;
        float worldFov = 90.0f;
        bool nightMode = false;
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

    col_t ImColorToCol_T(const ImColor& imColor) {
        int r = static_cast<int>(imColor.Value.x * 255.f);
        int g = static_cast<int>(imColor.Value.y * 255.f);
        int b = static_cast<int>(imColor.Value.z * 255.f);
        int a = static_cast<int>(imColor.Value.w * 255.f);

        return col_t(r, g, b, a);
    }

	HMODULE			m_module = nullptr;
	c_local_player	m_local = {};
	c_user_cmd*		m_cur_cmd = nullptr;
	bool			m_packet = true;
}