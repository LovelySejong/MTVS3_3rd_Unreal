// Copyright Epic Games, Inc. All Rights Reserved.

#include "MTVS3_3rdGameMode.h"
#include "MTVS3_3rdCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "HJ/MTVS3_3rdPlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"
#include "GameFramework/PlayerState.h"
#include "MTVS3_3rdPlayerController.h"
#include "HJ/MTVS3_3rdGameState.h"

AMTVS3_3rdGameMode::AMTVS3_3rdGameMode()
	: Super()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter_SchoolBoy.BP_FirstPersonCharacter_SchoolBoy_C"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;
}

void AMTVS3_3rdGameMode::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> ActorList;
	GetSeamlessTravelActorList(false , ActorList);

	bUseSeamlessTravel = true;

	//auto GS = GetGameState<AMTVS3_3rdGameState>();
	//if ( GS )
	//{
	//	for ( auto CurrPS : GS->PlayerArray )
	//	{
	//		for ( auto& Actor : ActorList )
	//		{
	//			auto PS = Cast<AMTVS3_3rdPlayerState>(Actor);
	//			if ( PS )
	//			{
	//				UE_LOG(LogTemp , Log , TEXT("AMTVS3_3rdGameState::BeginPlay() bIsHost: %d") , PS->bIsHost);
	//				UE_LOG(LogTemp , Log , TEXT("AMTVS3_3rdGameState::BeginPlay() HostID: %s") , *PS->HostID);
	//				UE_LOG(LogTemp , Log , TEXT("AMTVS3_3rdGameState::BeginPlay() GuestID: %s") , *PS->GuestID);

	//				if ( PS->bIsHost )
	//				{
	//					GS->SetHostID(PS->GetHostID());
	//				}
	//				else GS->SetGuestID(PS->GetGuestID());

	//				if ( CurrPS->GetUniqueId().ToString() == PS->GetUniqueId().ToString() )
	//				{
	//					UE_LOG(LogTemp,Warning, TEXT("Same UniqueId [%s]"), *CurrPS->GetUniqueId().ToString());
	//				}
	//			}
	//		}
	//	}	
	//}	
}

# pragma region 주석 처리
//APlayerController* AMTVS3_3rdGameMode::Login(UPlayer* NewPlayer , ENetRole InRemoteRole , const FString& Portal , const FString& Options , const FUniqueNetIdRepl& UniqueId , FString& ErrorMessage)
//{
//    UE_LOG(LogTemp , Warning , TEXT("NewPlayer :%s") , *UniqueId->ToString());
//
//    FString Temp = TEXT("IsHost");
//    FString Value = UGameplayStatics::ParseOption(Options, Temp);
//    int32 IsHost = FCString::Atoi(*Value);
//    
//    PlayersJoined.Add(UniqueId->ToString() , (bool)IsHost);
//
//    return Super::Login(NewPlayer , InRemoteRole , Portal , Options , UniqueId , ErrorMessage);
//}
//
//void AMTVS3_3rdGameMode::PostLogin(APlayerController* NewPlayer)
//{
//    Super::PostLogin(NewPlayer);
//    
//    bool* bIsHost = PlayersJoined.Find(*NewPlayer->PlayerState->GetUniqueId()->ToString());
//    if ( bIsHost )
//    {
//        auto PC = Cast<AMTVS3_3rdPlayerController>(NewPlayer);
//        if ( PC )
//        {
//            UE_LOG(LogTemp , Warning , TEXT("Inside PostLogin: %s") , *NewPlayer->PlayerState->GetUniqueId()->ToString());
//            PC->bIsHost = *bIsHost;
//            PC->OnRep_bIsHost();
//        }
//    }
//}
# pragma endregion

AActor* AMTVS3_3rdGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
	//auto GS = GetGameState<AMTVS3_3rdGameState>();
	//if ( GS )
	//{
	//    UE_LOG(LogTemp , Warning , TEXT("bTest :%d") , GS->bTest);
	//}
	//else UE_LOG(LogTemp , Warning , TEXT("Cast to GameState failed"));

	UE_LOG(LogTemp , Warning , TEXT("ChoosePlayerStart_Implementation"));

	TArray<AActor*> ActorList;
	GetSeamlessTravelActorList(false , ActorList);
	TArray<AActor*> PlayerStarts;
	UGameplayStatics::GetAllActorsOfClass(GetWorld() , APlayerStart::StaticClass() , PlayerStarts);

	for ( auto& Actor : ActorList )
	{
		auto PS = Cast<AMTVS3_3rdPlayerState>(Actor);
		if ( PS && PS->GetUniqueId().ToString() == Player->PlayerState->GetUniqueId().ToString() )
		{
			for ( AActor* PlayerStart : PlayerStarts )
			{
				if ( PS->bIsHost && PlayerStart->Tags.Contains("Host") )
				{
					UE_LOG(LogTemp , Warning , TEXT("return: PlayerStart : Host"));
					return PlayerStart; // Return the PlayerStart for the host
				}
				else if ( !PS->bIsHost && PlayerStart->Tags.Contains("Guest") )
				{
					UE_LOG(LogTemp , Warning , TEXT("return: PlayerStart : Guest"));
					return PlayerStart; // Return the PlayerStart for the client
				}
				else UE_LOG(LogTemp , Warning , TEXT("PlayerStart loaded unsuccessful"));
			}
		}
		else UE_LOG(LogTemp , Warning , TEXT("PS not valid"));
	}

	//   AMTVS3_3rdPlayerController* PC = Cast<AMTVS3_3rdPlayerController>(GetWorld()->GetFirstPlayerController());
	//   if ( !PC ) return Super::ChoosePlayerStart_Implementation(Player);
	//   AMTVS3_3rdPlayerState* PS = Cast<AMTVS3_3rdPlayerState>(PC->PlayerState);

	   //TArray<AActor*> PlayerStarts;
	   //UGameplayStatics::GetAllActorsOfClass(GetWorld() , APlayerStart::StaticClass() , PlayerStarts);

	//   for ( AActor* PlayerStart : PlayerStarts )
	//   {
	//       if ( PS->bIsHost && PlayerStart->Tags.Contains("PlayerStart_Host") )
	//       {
	//           UE_LOG(LogTemp , Warning , TEXT("return: PlayerStart_Host"));
	//           return PlayerStart; // Return the PlayerStart for the host
	//       }
	//       else if ( !PS->bIsHost && PlayerStart->Tags.Contains("PlayerStart_Guest") )
	//       {
	//           UE_LOG(LogTemp , Warning , TEXT("return: PlayerStart_Guest"));
	//           return PlayerStart; // Return the PlayerStart for the client
	//       }
	//       else UE_LOG(LogTemp , Warning , TEXT("PlayerStart loaded unsuccessful"));
	//   }

	return Super::ChoosePlayerStart_Implementation(Player);
}

UClass* AMTVS3_3rdGameMode::GetDefaultPawnClassForController_Implementation(AController* InController)
{

	UE_LOG(LogTemp , Warning , TEXT("----------GetDefaultPawnClassForController_Implementation---------"));

	/*AMTVS3_3rdPlayerController* PC = Cast<AMTVS3_3rdPlayerController>(GetWorld()->GetFirstPlayerController());
	if ( !PC ) return Super::GetDefaultPawnClassForController_Implementation(InController);;
	AMTVS3_3rdPlayerState* PS = Cast<AMTVS3_3rdPlayerState>(PC->PlayerState);
	if ( !PS ) return Super::GetDefaultPawnClassForController_Implementation(InController);*/

	UClass* CharacterClassToReturn = nullptr;

	TArray<AActor*> ActorList;
	GetSeamlessTravelActorList(false , ActorList);
	for ( auto& Actor : ActorList )
	{
		auto PS = Cast<AMTVS3_3rdPlayerState>(Actor);
		if ( PS && PS->GetUniqueId().ToString() == InController->PlayerState->GetUniqueId().ToString() )
		{
			CharacterClassToReturn = PS->bIsHost ? HostCharacter : GuestCharacter;

			UE_LOG(LogTemp , Warning , TEXT("[GetDefaultPawnClassForController_Implementation] %s") , *GetNameSafe(CharacterClassToReturn));
			return CharacterClassToReturn;
		}
		else UE_LOG(LogTemp , Warning , TEXT("PS not valid"));
	}

	// bIsHost 값에 따라 다른 캐릭터 블루프린트를 반환
	/*if ( PS->bIsHost )
	{
		CharacterClassToReturn = StaticLoadClass(APawn::StaticClass() , nullptr , TEXT("/Game/KHJ/Blueprints/BP_HJHost.BP_HJHost_C"));
	}
	else
	{
		CharacterClassToReturn = StaticLoadClass(APawn::StaticClass() , nullptr , TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter_SchoolBoy_C"));
	}

	if ( CharacterClassToReturn )
	{
		UE_LOG(LogTemp , Warning , TEXT("Character blueprint loaded successfully: %s") , *CharacterClassToReturn->GetName());
		return CharacterClassToReturn;
	}
	else UE_LOG(LogTemp , Warning , TEXT("Character blueprint loaded unsuccessful"));*/

	UE_LOG(LogTemp , Warning , TEXT("[GetDefaultPawnClassForController_Implementation] Failed to get pawn class"));
	// 기본 동작으로 돌아가서 기본 Pawn 클래스를 반환
	return Super::GetDefaultPawnClassForController_Implementation(InController);
}

void AMTVS3_3rdGameMode::OnGameClear()
{
	GetWorld()->ServerTravel("/Game/LovelySejong/EndingLevel?listen");
}

void AMTVS3_3rdGameMode::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	// If players should start as spectators, leave them in the spectator state
	if ( !bStartPlayersAsSpectators && !MustSpectate(NewPlayer) && PlayerCanRestart(NewPlayer) )
	{
		// Otherwise spawn their pawn immediately
		//RestartPlayer(NewPlayer);
		Super::HandleStartingNewPlayer_Implementation(NewPlayer);

		UE_LOG(LogTemp , Warning , TEXT("=========HandleStartingNewPlayer_Implementation=========="));

		if ( auto pawn = Cast<AMTVS3_3rdPlayerState>(NewPlayer->GetPawn()) )
		{
			UE_LOG(LogTemp , Warning , TEXT("[AMTVS3_3RdGameMode] Pawn possessed: %s") , *GetNameSafe(pawn));
		}
		else UE_LOG(LogTemp , Warning , TEXT("[AMTVS3_3RdGameMode] No Pawn possessed"));

		//if ( auto nice = Cast<AMTVS3_3rdPlayerState>(NewPlayer->PlayerState) )
		//{
		//	UE_LOG(LogTemp , Warning , TEXT("[%s]NewPlayer PlayerState: %s") , *nice->GetUniqueId().ToString() , *nice->AccessToken);
		//}
		//else UE_LOG(LogTemp , Warning , TEXT("NewPlayer PlayerState: no player state"));

		//for ( auto& P : ActorList )
		//{
		//	auto nice = Cast<AMTVS3_3rdPlayerState>(P);

		//	if ( nice )
		//	{
		//		UE_LOG(LogTemp , Warning , TEXT("[%s] ActorList: %s") , *nice->GetUniqueId().ToString() , *nice->AccessToken);
		//	}
		//	else UE_LOG(LogTemp , Warning , TEXT("ActorList not valid"));
		//}
	}
}
