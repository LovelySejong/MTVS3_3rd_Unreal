// Fill out your copyright notice in the Description page of Project Settings.


#include "Justin/Lobby/S3LobbyGMBase.h"
#include "Justin/Lobby/S3PCLobby.h"
#include "GameFramework/GameSession.h"
#include "Net/UnrealNetwork.h"
#include "Justin/S3GameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "HJ/MTVS3_3rdPlayerState.h"

AS3LobbyGMBase::AS3LobbyGMBase()
{
	bHasHost = false;
	bIsFull = false;
	
}
APlayerController* AS3LobbyGMBase::Login(UPlayer* NewPlayer , ENetRole InRemoteRole , const FString& Portal , const FString& Options , const FUniqueNetIdRepl& UniqueId , FString& ErrorMessage)
{
	FString Temp = TEXT("username");
	FString Username = UGameplayStatics::ParseOption(Options , Temp);

	PlayersJoined.Add(UniqueId->ToString() , Username);

	UE_LOG(LogTemp , Warning , TEXT("NewPlayer :%s, Username: %s, PlayersJoined Size: %d") , *UniqueId->ToString(), *Username, PlayersJoined.Num());

	return Super::Login(NewPlayer , InRemoteRole , Portal , Options , UniqueId , ErrorMessage);
}

//void AS3LobbyGMBase::PreLogin(const FString& Options , const FString& Address , const FUniqueNetIdRepl& UniqueId , FString& ErrorMessage)
//{
//	if ( !bIsFull )
//	{
//		UE_LOG(LogTemp , Warning , TEXT("Login success %s") , *UniqueId.ToString());
//		Super::PreLogin(Options , Address , UniqueId , ErrorMessage);
//	}
//	else
//	{
//		ErrorMessage = TEXT("Server is full. Try again later.");
//		UE_LOG(LogTemp , Warning , TEXT("Login Failed %s") , *UniqueId.ToString());
//	}
//}

void AS3LobbyGMBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	auto Pawn = NewPlayer->GetPawn();
	UE_LOG(LogTemp , Warning , TEXT("[PostLogin] PlayerCharacter: %s") , *GetNameSafe(Pawn));

	if ( !Cast<AS3PCLobby>(NewPlayer)->IsHost() ) // is Client 
	{
		bIsFull = true;
		auto PC = Cast<AS3PCLobby>(HostPC);
		if ( PC )
		{
			PC->AddPlayer();
			bUseSeamlessTravel = true;
		}
		else UE_LOG(LogTemp , Warning , TEXT("ERROR"));
	}
	else HostPC = NewPlayer;
}

void AS3LobbyGMBase::Logout(AController* Exiting)
{
	auto PC = Cast<AS3PCLobby>(Exiting);
	if ( PC )
	{
		if ( PC->IsHost() )
		{
			auto GI = Cast<US3GameInstance>(GetWorld()->GetGameInstance());
			if ( GI ) GI->DestroyServer();
		}
		else
		{
			PC = Cast<AS3PCLobby>(HostPC);
			PC->RemovePlayer();
			bIsFull = false;
			bUseSeamlessTravel = false;
		}
	}

	Super::Logout(Exiting);
}

bool AS3LobbyGMBase::IsReadyToPlay()
{
	bool bIsReadyToStart = false;

	for ( FConstPlayerControllerIterator it = GetWorld()->GetPlayerControllerIterator(); it; ++it )
	{
		auto PC = Cast<AS3PCLobby>(*it);
		if ( PC->IsHost() ) continue;
		else bIsReadyToStart = PC->IsReady();
	}
	return bIsReadyToStart;
}

void AS3LobbyGMBase::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);

	auto Pawn = NewPlayer->GetPawn();

	UE_LOG(LogTemp , Warning , TEXT("[HandleStartingNewPlayer] PlayerCharacter: %s") , *GetNameSafe(Pawn));
	UE_LOG(LogTemp , Warning , TEXT("----------[HandleStartingNewPlayer]---------"));

	auto PC = CastChecked<AS3PCLobby>(NewPlayer);
	// Host의 게임 인스턴스 bIsHost에는 true를 저장하고
	// Guest의 게임 인스턴스 bIsHost에는 false를 저장

	if(auto pawn = Cast<AMTVS3_3rdPlayerState>(NewPlayer->GetPawn()))
    {
        UE_LOG(LogTemp,Warning,TEXT("[AS3LobbyGMBase] Pawn possessed: %s"), *GetNameSafe(pawn));
    }
    else UE_LOG(LogTemp,Warning,TEXT("[AS3LobbyGMBase] No Pawn possessed"));

	if ( PC )
	{
		if ( bHasHost )
		{
			PC->SetHost(false);
			
			if(auto PS = PC->GetPlayerState<AMTVS3_3rdPlayerState>())
			{
				PS->SetHost(false);
				UE_LOG(LogTemp,Warning,TEXT("[PreClientTravel] bIsHost:%d"), PS->bIsHost);
			}
		}
		else
		{
			bHasHost = true;
			PC->SetHost(true);
			
			if(auto PS = PC->GetPlayerState<AMTVS3_3rdPlayerState>())
			{
				PS->SetHost(true);
				UE_LOG(LogTemp,Warning,TEXT("[PreClientTravel] bIsHost: %d"), PS->bIsHost);
			}
		}
	}


}