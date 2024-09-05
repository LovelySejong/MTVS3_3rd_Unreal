// Fill out your copyright notice in the Description page of Project Settings.


#include "Justin/Lobby/S3PCLobby.h"
#include "Justin/Lobby/LobbyWidget.h"
#include "Blueprint/UserWidget.h"
#include "Net/UnrealNetwork.h"
#include "Justin/Lobby/S3LobbyGMBase.h"
#include "Justin/S3GameInstance.h"

AS3PCLobby::AS3PCLobby()
{
	bIsReady = false;
}

void AS3PCLobby::BeginPlay()
{
	Super::BeginPlay();

	if ( IsLocalPlayerController() )
	{
		LobbyWidget = CreateWidget<ULobbyWidget>(this , LobbyWidgetClass);
		UE_LOG(LogTemp , Warning , TEXT("S3PCLobby BeginPlay Host : %d") , bIsHost);
		if ( ensure(LobbyWidget) )
		{
			LobbyWidget->Init(bIsHost);
			LobbyWidget->AddToViewport();
		}
	}
}

void AS3PCLobby::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AS3PCLobby , bIsReady);
}

void AS3PCLobby::SetHost(bool _bIsHost)
{
	bIsHost = _bIsHost;
}

bool AS3PCLobby::IsHost() const
{
	return bIsHost;
}

void AS3PCLobby::RemovePlayer()
{
	if ( bIsHost && LobbyWidget )
	{
		bIsReady = false;
		if ( HasAuthority() ) OnRep_bIsReady();
		LobbyWidget->RemovePlayer();		
	}
}

void AS3PCLobby::AddPlayer()
{
	if ( bIsHost && LobbyWidget )
	{
		UE_LOG(LogTemp , Warning , TEXT("AddpLayer Success"));
		LobbyWidget->AddPlayer();
	}
	else UE_LOG(LogTemp , Warning , TEXT("AddpLayer failed"));
}

void AS3PCLobby::Server_CheckCanStart_Implementation()
{
	auto GM = GetWorld()->GetAuthGameMode<AS3LobbyGMBase>();
	if ( GM && GM->IsReadyToPlay() )
	{
		UE_LOG(LogTemp , Warning , TEXT("Start Game!"));

		auto GI = GetWorld()->GetGameInstance<US3GameInstance>();
		if ( GI )
		{
			GI->StartSession();
		}	
	}
	else UE_LOG(LogTemp , Warning , TEXT("Players not ready!"));
}

void AS3PCLobby::Server_SetReady_Implementation()
{
	for ( FConstPlayerControllerIterator it = GetWorld()->GetPlayerControllerIterator(); it; ++it )
	{
		auto PC = Cast<AS3PCLobby>(*it);
		PC->bIsReady = !PC->bIsReady;
		if ( PC->HasAuthority() ) PC->OnRep_bIsReady();
		UE_LOG(LogTemp , Warning , TEXT("Player is Ready: %d") , PC->bIsReady);
	}
}

void AS3PCLobby::OnRep_bIsReady()
{
	if ( LobbyWidget ) LobbyWidget->SetReady(bIsReady);
}

bool AS3PCLobby::IsReady() const
{
	return bIsReady;
}
