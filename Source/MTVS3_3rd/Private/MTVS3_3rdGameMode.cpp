// Copyright Epic Games, Inc. All Rights Reserved.

#include "MTVS3_3rdGameMode.h"
#include "MTVS3_3rdCharacter.h"
#include "UObject/ConstructorHelpers.h"

AMTVS3_3rdGameMode::AMTVS3_3rdGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
