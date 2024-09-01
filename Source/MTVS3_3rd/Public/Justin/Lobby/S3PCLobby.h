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
	bool IsHost() const;
	bool IsReady() const;

	void RemovePlayer();
	void AddPlayer();
	
	UFUNCTION(BlueprintCallable)
	void What();
	
	UFUNCTION()
	void HandleButtonPress();

	UFUNCTION(Server, Reliable)
	void Server_CheckCanStart();

	UFUNCTION(Server , Reliable)
	void Server_SetReady();

protected:
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(EditDefaultsOnly , Category = "Settings")
	TSubclassOf<ULobbyWidget> LobbyWidgetClass;

	UPROPERTY(EditDefaultsOnly , Category = "Settings")
	TObjectPtr<UImage> Ready_Image;

private:
	TObjectPtr<ULobbyWidget> LobbyWidget;
	bool bIsHost;

	UPROPERTY(ReplicatedUsing = OnRep_bIsReady)
	bool bIsReady;
	UFUNCTION()
	void OnRep_bIsReady();
};
