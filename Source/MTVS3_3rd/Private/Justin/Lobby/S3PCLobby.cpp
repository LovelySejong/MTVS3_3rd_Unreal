// Fill out your copyright notice in the Description page of Project Settings.


#include "Justin/Lobby/S3PCLobby.h"
#include "Justin/Lobby/LobbyWidget.h"
#include "Blueprint/UserWidget.h"
#include "Net/UnrealNetwork.h"
#include "Justin/Lobby/S3LobbyGMBase.h"
#include "Justin/S3GameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "HJ/MTVS3_3rdPlayerState.h"
#include "HJ/MTVS3_3rdGameState.h"

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
		// 서버 rpc
		auto PS = GetPlayerState<AMTVS3_3rdPlayerState>();
		if ( !PS )
		{
			UE_LOG(LogTemp , Warning , TEXT("PS is null in AS3PCLobby::BeginPlay()"));
			return;
		}
		auto GI = Cast<US3GameInstance>(GetWorld()->GetGameInstance());
		if ( !GI ) return;
		if ( PS->bIsHost )
		{
			PS->ServerRPCSetHostNickname(GI->GetPlayerNickname());
			PS->ServerRPCSetHostId(GI->GetPlayerID());			
		}
	}
}

void AS3PCLobby::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AS3PCLobby , bIsReady);
}

#pragma region HJ
void AS3PCLobby::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	if ( PlayerState )
	{
		UE_LOG(LogTemp , Log , TEXT("PlayerState has been replicated to the client!"));
		// PlayerState가 클라이언트에 복제되었을 때의 로직 추가
		// 서버 rpc
		auto PS = GetPlayerState<AMTVS3_3rdPlayerState>();
		if ( !PS )
		{
			UE_LOG(LogTemp , Warning , TEXT("PS is null in AS3PCLobby::OnRep_PlayerState()"));
			return;
		}
		auto GI = Cast<US3GameInstance>(GetWorld()->GetGameInstance());
		if ( !GI ) return;
		PS->ServerRPCSetGuestNickname(GI->GetPlayerNickname());
		PS->ServerRPCSetGuestId(GI->GetPlayerID());
		
		//if ( false == PS->bIsHost )
		//{
		//	GS->ServerRPCSetGuestNickname(GI->GetPlayerNickname());
		//}
	}
}
#pragma endregion

void AS3PCLobby::SetHost(bool _bIsHost)
{
	bIsHost = _bIsHost;

	//if ( HasAuthority() )
	//{
	//	Client_SetHost(_bIsHost);
	//}
}

//void AS3PCLobby::Client_SetHost_Implementation(bool _bIsHost)
//{
//	SetHost(_bIsHost);
//}

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
