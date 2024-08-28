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
protected:
	virtual void Init() override;

protected:
	UFUNCTION(BlueprintCallable)
	void CreateServer();
	UFUNCTION(BlueprintCallable)
	void JoinServer();

	virtual void OnCreateSessionCompleted(FName SessionName, bool bWasSuccessful);
	virtual void OnFindSessionsCompleted(bool bWasSuccessful);
	virtual void OnJoinSessionCompleted(FName SessionName , EOnJoinSessionCompleteResult::Type Result);


private:
	TSharedPtr<FOnlineSessionSearch> SessionSearch;
	IOnlineSessionPtr SessionInterface;
};
