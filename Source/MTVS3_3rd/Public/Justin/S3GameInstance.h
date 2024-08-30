// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"
#include "S3GameInstance.generated.h"

/**
 * 
 */
UCLASS()
class MTVS3_3RD_API US3GameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	US3GameInstance();

public:
	IOnlineSessionPtr SessionInterface;
	TSharedPtr<FOnlineSessionSearch> SessionSearch;

	UFUNCTION(BlueprintCallable)
	void CreateServer();

	UFUNCTION(BlueprintCallable)
	void FindServer();

	UFUNCTION(BlueprintCallable)
	void JoinServer(int32 Index);
	UFUNCTION(BlueprintCallable)
	void DestroyServer();

protected:
	virtual void Init() override;
	virtual void Shutdown() override;

private:
};
