// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Http.h"
#include "HttpActor.generated.h"

UCLASS()
class MTVS3_3RD_API AHttpActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHttpActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	#pragma region UI
	UPROPERTY(EditAnywhere, Category = "Default|UI")
	TSubclassOf<class UStartWidget> StartUIFactory;
	UPROPERTY()
	class UStartWidget* StartUI;

	void InitStartUI();
#pragma endregion

	// Button_SignIn을 눌렀을 때 요청할 함수
	void ReqPostLogin(const FText& ID , const FText& Password);

	// 응답 받을 함수
	void OnResPostLogin(FHttpRequestPtr Request , FHttpResponsePtr Response , bool bConnectedSuccessfully);

	UFUNCTION(BlueprintCallable, Category = "HTTP")
	void ReqPostRoomState(const FString& AccessToken , const FString& PartnerID , int32 RoomNumber);

	void OnResPostRoomState(FHttpRequestPtr Request , FHttpResponsePtr Response , bool bWasSuccessful);
};
