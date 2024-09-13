// Fill out your copyright notice in the Description page of Project Settings.


#include "HJ/MTVS3_3rdPlayerState.h"

#pragma region HJ 
void AMTVS3_3rdPlayerState::CopyProperties(APlayerState* NewPlayerState)
{
	Super::CopyProperties(NewPlayerState);

	if ( AMTVS3_3rdPlayerState* MyNewPlayerState = Cast<AMTVS3_3rdPlayerState>(NewPlayerState) )
	{
		// 기존 PlayerState의 데이터를 새로운 PlayerState로 복사
		MyNewPlayerState->bIsHost = bIsHost;
		MyNewPlayerState->PlayerNickname = PlayerNickname;
		MyNewPlayerState->AccessToken = AccessToken;
	}
}

void AMTVS3_3rdPlayerState::SetHost(bool _bIsHost)
{
	bIsHost = _bIsHost;
	//PlayerNum += PlayerCount;
	//bIsHost = PlayerNum == 2 ? 1 : 0;
	//UE_LOG(LogTemp , Warning , TEXT("===== Player Num is %d"), PlayerNum);
}

void AMTVS3_3rdPlayerState::SetAccessToken(const FString& InAccessToken)
{
	AccessToken = InAccessToken;
}

void AMTVS3_3rdPlayerState::SetPlayerNickname(const FString& Nickname)
{
	if ( Nickname.IsEmpty() )
	{
		UE_LOG(LogTemp , Warning , TEXT("Nickname is empty, setting to default value."));
		PlayerNickname = TEXT("Player");
	}
	else
	{
		PlayerNickname = Nickname;
	}
}

FString AMTVS3_3rdPlayerState::GetPlayerNickname() const
{
	if ( PlayerNickname.IsEmpty() )
	{
		UE_LOG(LogTemp , Warning , TEXT("PlayerNickname is empty, returning default value."));
		return TEXT("Player");
	}
	return PlayerNickname;
}
#pragma endregion