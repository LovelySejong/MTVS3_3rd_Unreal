// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MTVS3_3rdPlayerController.generated.h"

class UInputMappingContext;

/**
 *
 */
UCLASS()
class MTVS3_3RD_API AMTVS3_3rdPlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:

	/** Input Mapping Context to be used for player input */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputMappingContext* InputMappingContext;

	// Begin Actor interface
protected:

	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
public:
	UPROPERTY(ReplicatedUsing = OnRep_bIsHost)
	bool bIsHost; 
	
	UFUNCTION()
	void OnRep_bIsHost();
};
