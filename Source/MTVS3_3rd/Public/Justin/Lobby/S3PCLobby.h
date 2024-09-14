// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "S3PCLobby.generated.h"

/**
 *
 */

class ULobbyWidget;
class UImage;

UCLASS()
class MTVS3_3RD_API AS3PCLobby : public APlayerController
{
	GENERATED_BODY()

public:
	AS3PCLobby();

	void SetHost(bool _bIsHost);

	//UFUNCTION(Client, Reliable)
	//void Client_SetHost(bool _bIsHost);

	bool IsHost() const;
	bool IsReady() const;

	//Only Called in server---------------------
	void RemovePlayer();
	void AddPlayer();
	//---------------------

	UFUNCTION(Server , Reliable)
	void Server_CheckCanStart();

	UFUNCTION(Server , Reliable)
	void Server_SetReady();

protected:
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(EditDefaultsOnly , Category = "Settings")
	TSubclassOf<ULobbyWidget> LobbyWidgetClass;

private:
	TObjectPtr<ULobbyWidget> LobbyWidget;
	bool bIsHost;

	UPROPERTY(ReplicatedUsing = OnRep_bIsReady)
	bool bIsReady;
	UFUNCTION()
	void OnRep_bIsReady();
};
