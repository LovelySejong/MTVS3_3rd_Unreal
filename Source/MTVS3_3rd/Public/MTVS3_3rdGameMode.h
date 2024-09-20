// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MTVS3_3rdGameMode.generated.h"

UCLASS(minimalapi)
class AMTVS3_3rdGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AMTVS3_3rdGameMode();
	//protected:
	//	virtual APlayerController* Login(UPlayer* NewPlayer , ENetRole InRemoteRole , const FString& Portal , const FString& Options , const FUniqueNetIdRepl& UniqueId , FString& ErrorMessage) override;
	//
	//	virtual void PostLogin(APlayerController* NewPlayer) override;
public:
	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;

	virtual UClass* GetDefaultPawnClassForController_Implementation(AController* InController) override;



protected:
	UPROPERTY(EditDefaultsOnly , Category = Settings)
	TSubclassOf<ACharacter> HostCharacter;
	UPROPERTY(EditDefaultsOnly , Category = Settings)
	TSubclassOf<ACharacter> GuestCharacter;


	//virtual void HandleSeamlessTravelPlayer(AController*& C) override;
	//virtual void PostSeamlessTravel() override;
	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;
private:
	TMap<FString , bool> PlayersJoined;
};



