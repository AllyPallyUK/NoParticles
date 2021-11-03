#include "main.hpp"
#include "ModConfig.hpp"

#include "beatsaber-hook/shared/utils/il2cpp-utils.hpp"
#include "questui/shared/BeatSaberUI.hpp"
#include "questui/shared/QuestUI.hpp"
#include "UnityEngine/Vector3.hpp"
#include "config-utils/shared/config-utils.hpp"

#include "GlobalNamespace/BombExplosionEffect.hpp"
#include "GlobalNamespace/NoteCutParticlesEffect.hpp"

using namespace GlobalNamespace;
using namespace QuestUI;

static ModInfo modInfo; // Stores the ID and version of our mod, and is sent to the modloader upon startup
DEFINE_CONFIG(ModConfig);

// Loads the config from disk using our modInfo, then returns it for use
Configuration& getConfig() {
    static Configuration config(modInfo);
    config.Load();
    return config;
}

// Returns a logger, useful for printing debug messages
Logger& getLogger() {
    static Logger* logger = new Logger(modInfo);
    return *logger;
}

// Define variables for later use if needed


// Define hooks
MAKE_HOOK_MATCH(NoteCutParticlesEffect_SpawnParticles, &GlobalNamespace::NoteCutParticlesEffect::SpawnParticles, void, GlobalNamespace::NoteCutParticlesEffect* self, UnityEngine::Vector3 cutPoint, UnityEngine::Vector3 cutNormal, UnityEngine::Vector3 saberDir, float saberSpeed, UnityEngine::Vector3 noteMovementVec, UnityEngine::Color32 color, int sparkleParticlesCount, int explosionParticlesCount, float LifetimeMultiplier
) {

    if(getModConfig().notesEnabled.GetValue())
    {
        NoteCutParticlesEffect_SpawnParticles(self, cutPoint, cutNormal, saberDir, saberSpeed, noteMovementVec, color, sparkleParticlesCount,explosionParticlesCount,LifetimeMultiplier);
    }
    else
    {
        return;
    }
};

MAKE_HOOK_MATCH(BombExplosionEffect_SpawnExplosion, &GlobalNamespace::BombExplosionEffect::SpawnExplosion, void, GlobalNamespace::BombExplosionEffect* self, UnityEngine::Vector3 pos
){

    if(getModConfig().bombsEnabled.GetValue())
    {
        BombExplosionEffect_SpawnExplosion(self, pos);
    }
    else
    {
        return;
    }
};

//MAKE_HOOK_MATCH(SceneManager_SetActiveScene, &UnityEngine::SceneManagement::SceneManager::SetActiveScene, bool, UnityEngine::SceneManagement::Scene scene)
//{
//    return SceneManager_SetActiveScene(scene);
//}

void DidActivate(HMUI::ViewController* self, bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling){
    if(firstActivation){
        UnityEngine::GameObject* container = BeatSaberUI::CreateScrollableSettingsContainer(self->get_transform());

        BeatSaberUI::CreateText(container->get_transform(), "No Particles v1 - AllyPallyUK");

        AddConfigValueToggle(container->get_transform(), getModConfig().notesEnabled);

        AddConfigValueToggle(container->get_transform(), getModConfig().bombsEnabled);
    }
    else{
        // If not first time activated
    }
};

// Called at the early stages of game loading
extern "C" void setup(ModInfo& info) {
    info.id = ID;
    info.version = VERSION;
    modInfo = info;
	
    getConfig().Load(); // Load the config file
    getLogger().info("Completed setup!");
}

// Called later on in the game loading - a good time to install function hooks
extern "C" void load() {
    il2cpp_functions::Init();
    getModConfig().Init(modInfo);

    LoggerContextObject logger = getLogger().WithContext("load");

    QuestUI::Init();                                                                    
    QuestUI::Register::RegisterModSettingsViewController(modInfo, DidActivate);         // <----- This Adds a button in the settings menu
    QuestUI::Register::RegisterMainMenuModSettingsViewController(modInfo, DidActivate); // <----- This Adds a button in the main menu
    getLogger().info("Successfully installed Settings UI!");

    getLogger().info("Installing hooks...");
    INSTALL_HOOK(getLogger(), NoteCutParticlesEffect_SpawnParticles);
    INSTALL_HOOK(getLogger(), BombExplosionEffect_SpawnExplosion);
    getLogger().info("Installed all hooks!");
};