﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "S3LobbyGMBase.generated.h"

/**
 * 
 */

class AS3PCLobby;

UCLASS()
class MTVS3_3RD_API AS3LobbyGMBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AS3LobbyGMBase();
	//virtual void PreLogin(const FString& Options , const FString& Address , const FUniqueNetIdRepl& UniqueId , FString& ErrorMessage) override;
	virtual APlayerController* Login(UPlayer* NewPlayer , ENetRole InRemoteRole , const FString& Portal , const FString& Options , const FUniqueNetIdRepl& UniqueId , FString& ErrorMessage) override;
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;

	bool IsReadyToPlay();

protected:
	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;

private:
	bool bHasHost;
	APlayerController* HostPC;
	bool bIsFull;

	TMap<FString , FString> PlayersJoined;
};
