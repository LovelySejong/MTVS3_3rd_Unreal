// Fill out your copyright notice in the Description page of Project Settings.


#include "HJ/MTVS3_3rdPlayerState.h"
#include "Justin/Lobby/S3PCLobby.h"
#include "Net/UnrealNetwork.h"
#include "Justin/Lobby/LobbyWidget.h"

#pragma region HJ 
//void AMTVS3_3rdPlayerState::CopyProperties(APlayerState* NewPlayerState)
//{
//	Super::CopyProperties(NewPlayerState);
//
//	if ( AMTVS3_3rdPlayerState* MyNewPlayerState = Cast<AMTVS3_3rdPlayerState>(NewPlayerState) )
//	{
//		// 기존 PlayerState의 데이터를 새로운 PlayerState로 복사
//		MyNewPlayerState->bIsHost = bIsHost;
//		MyNewPlayerState->PlayerNickname = PlayerNickname;
//		MyNewPlayerState->AccessToken = AccessToken;
//	}
//}

void AMTVS3_3rdPlayerState::SetHost(bool _bIsHost)
{
	bIsHost = _bIsHost;
	//PlayerNum += PlayerCount;
	//bIsHost = PlayerNum == 2 ? 1 : 0;
	//UE_LOG(LogTemp , Warning , TEXT("===== Player Num is %d"), PlayerNum);
}

void AMTVS3_3rdPlayerState::SetHostNickname(const FString& hostNickname)
{
	HostNickname = hostNickname;
}

FString AMTVS3_3rdPlayerState::GetHostNickname() const
{
	if ( HostNickname.IsEmpty() )
	{
		UE_LOG(LogTemp , Warning , TEXT("HostNickname is empty, returning default value."));
		return TEXT("Host");
	}
	return HostNickname;
}

void AMTVS3_3rdPlayerState::SetGuestNickname(const FString& guestNickname)
{
	GuestNickname = guestNickname;
}

FString AMTVS3_3rdPlayerState::GetGuestNickname() const
{
	if ( GuestNickname.IsEmpty() )
	{
		UE_LOG(LogTemp , Warning , TEXT("GuestNickname is empty, returning default value."));
		return TEXT("Guest");
	}
	return GuestNickname;
}

void AMTVS3_3rdPlayerState::SetHostID(const FString& hostID)
{
	HostID = hostID;
}

FString AMTVS3_3rdPlayerState::GetHostID() const
{
	if ( HostID.IsEmpty() )
	{
		UE_LOG(LogTemp , Warning , TEXT("HostID is empty, returning default value."));
		return TEXT("HostID");
	}
	return HostID;
}
void AMTVS3_3rdPlayerState::SetGuestID(const FString& guestID)
{
	GuestID = guestID;
}
FString AMTVS3_3rdPlayerState::GetGuestID() const
{
	if ( GuestID.IsEmpty() )
	{
		UE_LOG(LogTemp , Warning , TEXT("GuestID is empty, returning default value."));
		return TEXT("GuestID");
	}
	return GuestID;
}
#pragma endregion

# pragma region HJ 멀티플레이
void AMTVS3_3rdPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMTVS3_3rdPlayerState , bIsHost);
	DOREPLIFETIME(AMTVS3_3rdPlayerState , HostNickname);
	DOREPLIFETIME(AMTVS3_3rdPlayerState , GuestNickname);
}

// Host 닉네임 RPC
void AMTVS3_3rdPlayerState::ServerRPCSetHostNickname_Implementation(const FString& hostName)
{
	// 서버는 hostName을 정하고
	SetHostNickname(hostName);
	// 멀티캐스트로 hostName을 알린다.
	MulticastRPCSetHostNickname(hostName);

	auto PC = Cast<AS3PCLobby>(GetOwner());
	if ( !PC )
	{
		UE_LOG(LogTemp , Warning , TEXT("PC is null in ServerRPCSetHostNickname"));
	}
	if ( PC->IsLocalPlayerController() )
	{
		OnRep_HostNickname(); // 호스트 자기 자신만 UI 업데이트
	}
}

void AMTVS3_3rdPlayerState::MulticastRPCSetHostNickname_Implementation(const FString& hostName)
{
	SetHostNickname(hostName);
}

void AMTVS3_3rdPlayerState::OnRep_HostNickname()
{
	// 클라이언트는 OnRep_HostNickname에 따라 UI 업데이트
	UpdateHostUI(GetHostNickname());
}

void AMTVS3_3rdPlayerState::UpdateHostUI(const FString& hostName)
{
	auto PC = Cast<AS3PCLobby>(GetOwner());
	if ( !PC )
	{
		UE_LOG(LogTemp , Warning , TEXT("PC is null in UpdateHostUI"));
		return;
	}

	auto LobbyUI = Cast<ULobbyWidget>(PC->LobbyWidget);
	if ( !LobbyUI )
	{
		UE_LOG(LogTemp , Warning , TEXT("LobbyUI is null in UpdateHostUI"));
		return;
	}

	LobbyUI->SetNameText(1 , hostName);
}

// Guest 닉네임 RPC
void AMTVS3_3rdPlayerState::ServerRPCSetGuestNickname_Implementation(const FString& guestName)
{
	// 서버는 guestName을 정하고
	SetGuestNickname(guestName);
	// 멀티캐스트로 guestName을 알린다.
	MulticastRPCSetGuestNickname(guestName);
}

void AMTVS3_3rdPlayerState::MulticastRPCSetGuestNickname_Implementation(const FString& guestName)
{
	// 클라이언트도 guestName 정하고
	SetGuestNickname(guestName);
	// UI 업데이트 해준다.
	OnRep_GuestNickname();
}

void AMTVS3_3rdPlayerState::OnRep_GuestNickname()
{
	UpdateGuestUI(GetGuestNickname());
	UpdateHostUI(GetHostNickname());
}

void AMTVS3_3rdPlayerState::UpdateGuestUI(const FString& guestName)
{
	auto PC = Cast<AS3PCLobby>(GetOwner());
	auto LobbyUI = Cast<ULobbyWidget>(PC->LobbyWidget);
	if ( PC && LobbyUI )
	{
		LobbyUI->SetNameText(2 , guestName);
	}
}
#pragma endregion