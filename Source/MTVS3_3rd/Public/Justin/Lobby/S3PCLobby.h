// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "S3PCLobby.generated.h"

/**
 * 
 */

class ULobbyWidget;

UCLASS()
class MTVS3_3RD_API AS3PCLobby : public APlayerController
{
	GENERATED_BODY()
	
public:
	void SetHost(bool _bIsHost);
	bool GetHost() const;
	void RemovePlayer();
	void AddPlayer();
	void HandleButtonPress();

	UFUNCTION(BlueprintCallable)
	void What();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly , Category = "Settings")
	TSubclassOf<ULobbyWidget> LobbyWidgetClass;

private:
	TObjectPtr<ULobbyWidget> LobbyWidget;
	bool bIsHost;
};
