// Copyright Epic Games, Inc. All Rights Reserved.


#include "MTVS3_3rdPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "Justin/S3GameInstance.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/PlayerState.h"

void AMTVS3_3rdPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// get the enhanced input subsystem
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		// add the mapping context so we get controls
		Subsystem->AddMappingContext(InputMappingContext, 0);
	}

	//if ( HasAuthority() )
	//{
	//	if(PlayerState ) UE_LOG(LogTemp , Warning , TEXT("BeginPlay AMTV33_PlayerController %s"), *PlayerState->GetUniqueId()->ToString());
	//}
}

//void AMTVS3_3rdPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
//{
//	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
//
//	DOREPLIFETIME(AMTVS3_3rdPlayerController, bIsHost);
//}
//
//void AMTVS3_3rdPlayerController::OnRep_bIsHost()
//{
//	if ( HasAuthority() )
//	{
//		if ( bIsHost )
//		{
//			UE_LOG(LogTemp , Warning , TEXT("HasAuthority & IsHost"));
//		}
//		else
//		{
//			UE_LOG(LogTemp , Warning , TEXT("HasAuthority & NotHost"));
//		}
//	}
//	else 
//	{
//		if ( bIsHost )
//		{
//			UE_LOG(LogTemp , Warning , TEXT("NoAuthority & IsHost"));
//		}
//		else UE_LOG(LogTemp , Warning , TEXT("NoAuthority & NotHost"));
//	}
//	
//}