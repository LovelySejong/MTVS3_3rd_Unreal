// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "LoginPawn.generated.h"

class US3GameInstance;

UCLASS()
class MTVS3_3RD_API ALoginPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ALoginPawn();

	UFUNCTION(BlueprintCallable)
	void StartConnection();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	IOnlineIdentityPtr OnlineIdentity;
	FOnLoginCompleteDelegate OnLoginCompleteDelegate;

	UFUNCTION(BlueprintCallable)
	void Test_CreateSession();
	UFUNCTION(BlueprintCallable)
	void Test_JoinSession();

	virtual void OnCreateSessionCompleted(FName SessionName , bool bWasSuccessful);
	virtual void OnFindSessionsCompleted(bool bWasSuccessful);
	virtual void OnJoinSessionCompleted(FName SessionName , EOnJoinSessionCompleteResult::Type Result);
	virtual void OnDestroySessionComplete(FName SessionName , bool bWasSuccessful);

	virtual void Test_OnFindSessionsCompleted(bool bWasSuccessful);

private:
	void OnLoginCompleted(int32 LocalUserNum , bool bWasSuccessful , const FUniqueNetId& UserId , const FString& Error);
	void OnNetworkFail(UWorld* World , UNetDriver* Driver , ENetworkFailure::Type Type , const FString& Error);
	TObjectPtr<US3GameInstance> GI;
	int32 ResultIndex;
};
