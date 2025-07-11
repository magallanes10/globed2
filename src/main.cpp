#include <defs/all.hpp>

#include <asp/Log.hpp>

#include <hooks/all.hpp>
#include <audio/manager.hpp>
#include <crypto/box.hpp>
#include <managers/settings.hpp>
#include <managers/web.hpp>
#include <ui/error_check_node.hpp>
#include <ui/notification/panel.hpp>
#include <util/all.hpp>

using namespace geode::prelude;

void setupAsp();
void setupErrorCheckNode();
void printDebugInfo();

namespace globed { void platformSetup(); }

#ifdef GLOBED_VOICE_SUPPORT
// TODO this hook kinda doesnt work bc early load and i dont even know if it would do anything
static void FMODSystemInitHook(FMOD::System* system, int channels, FMOD_INITFLAGS flags, void* dd) {
    log::debug("fmod system init hooked, changing to {} channels", MAX_AUDIO_CHANNELS);
    system->init(MAX_AUDIO_CHANNELS, flags, dd);
}
#endif // GLOBED_VOICE_SUPPORT

$on_mod(Loaded) {
#ifdef GLOBED_VOICE_SUPPORT
    // (void) Mod::get()->hook(
    //     reinterpret_cast<void*>(
    //         geode::addresser::getNonVirtual(
    //             &FMOD::System::init
    //         )
    //     ),
    //     &FMODSystemInitHook,
    //     "FMOD::System::init",
    //     tulip::hook::TulipConvention::Stdcall
    // ).expect("failed to hook fmod").unwrap();
#endif // GLOBED_VOICE_SUPPORT

    CryptoBox::initLibrary();
    setupAsp();
    setupErrorCheckNode();

#ifdef GLOBED_VOICE_SUPPORT
    GlobedAudioManager::get().preInitialize();
#endif

#if defined(GLOBED_DEBUG) && GLOBED_DEBUG
    printDebugInfo();
#endif

    globed::platformSetup();

    WebRequestManager::get().fetchCountry();
}

void setupAsp() {
    using namespace asp;

    asp::setLogFunction([](LogLevel level, auto message) {
        switch (level) {
            case LogLevel::Trace: [[fallthrough]];
            case LogLevel::Debug: log::debug("[asp] {}", message); break;
            case LogLevel::Info: log::info("[asp] {}", message); break;
            case LogLevel::Warn: log::warn("[asp] {}", message); break;
            case LogLevel::Error: log::error("[asp] {}", message); break;
        }
    });
}

// error check node runs on every scene and shows popups/notifications if an error has occured in another thread
void setupErrorCheckNode() {
    auto ecn = ErrorCheckNode::create();
    ecn->setID("error-check-node"_spr);
    ecn->retain();
    ecn->onEnter(); // needed so scheduled methods get called

    auto notif = GlobedNotificationPanel::create();
    notif->setID("notification-panel"_spr);
    notif->persist();
}

// just debug printing
void printDebugInfo() {
    std::string version = Mod::get()->getVersion().toVString();
    unsigned int fv = 0;

#ifdef GLOBED_LINK_TO_FMOD
    FMODAudioEngine::sharedEngine()->m_system->getVersion(&fv);
#endif

    log::warn("=== Globed {} has been loaded in debug mode ===", version);
    log::info("Platform: {} ({}-endian)", GLOBED_PLATFORM_STRING, GLOBED_LITTLE_ENDIAN ? "little" : "big");
    log::info("FMOD version: {}", fv);
#ifdef GLOBED_VOICE_SUPPORT
    log::info("Voice chat support: true (opus version: {})", GlobedAudioManager::getOpusVersion());
#else
    log::info("Voice chat support: false");
#endif
    log::info("Libsodium version: {} (CryptoBox algorithm: {})", CryptoBox::sodiumVersion(), CryptoBox::algorithm());
}

// asp::Thread<> g_printer;
// asp::Channel<std::tuple<std::string, std::string, asp::time::Duration>> g_ch;
// static bool g_init = false;

// $on_mod(Loaded) {
//     g_printer.setLoopFunction([](auto&) {
//         auto t = g_ch.popTimeout(asp::time::Duration::fromSecs(1));
//         if (!t) return;

//         log::info("[FPFF] - {} -> {} (taken {})", std::get<0>(t.value()), std::get<1>(t.value()), std::get<2>(t.value()).toString());
//     });

//     g_printer.start();
// }

// #include <Geode/modify/CCFileUtils.hpp>
// class $modify(CCFileUtils) {
//     static void onModify(auto& self) {
//         self.setHookPriority("cocos2d::CCFileUtils::fullPathForFilename", -1000000);
//     }
//     gd::string fullPathForFilename(const char* f, bool p) {
//         auto start = asp::time::Instant::now();
//         auto res = CCFileUtils::fullPathForFilename(f, p);
//         // g_ch.push({f, res, start.elapsed()});

//         log::info("[FPFF] - {} -> {} (taken {})", f, res, start.elapsed().toString());
//         return res;
//     }
// };