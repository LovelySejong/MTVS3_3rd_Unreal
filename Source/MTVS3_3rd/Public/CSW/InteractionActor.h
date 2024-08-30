// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractionActor.generated.h"

UCLASS()
class MTVS3_3RD_API AInteractionActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractionActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintImplementableEvent)
	void OnCursor();
	
	UFUNCTION(BlueprintImplementableEvent)
	void OffCursor();

	UFUNCTION(BlueprintImplementableEvent)
	void Interact(class AMTVS3_3rdCharacter* caller);
};
