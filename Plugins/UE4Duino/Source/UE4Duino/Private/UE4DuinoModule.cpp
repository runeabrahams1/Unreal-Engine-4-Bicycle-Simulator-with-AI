#include "UE4DuinoPCH.h"
#include "UE4DuinoModule.h"

IMPLEMENT_MODULE(UE4DuinoModule, UE4Duino);

DEFINE_LOG_CATEGORY(ModuleLog)

#define LOCTEXT_NAMESPACE "UE4Duino"

UE4DuinoModule::UE4DuinoModule()
{
}

void UE4DuinoModule::StartupModule()
{
// Startup LOG MSG
	UE_LOG(ModuleLog, Warning, TEXT("UE4Duino: Log Started"));
}

void UE4DuinoModule::ShutdownModule()
{
// Shutdown LOG MSG
	UE_LOG(ModuleLog, Warning, TEXT("UE4Duino: Log Ended"));
}