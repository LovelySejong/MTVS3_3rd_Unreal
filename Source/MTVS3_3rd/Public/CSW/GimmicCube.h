// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractionActor.h"
#include "GameFramework/Actor.h"
#include "GimmicCube.generated.h"

UCLASS()
class MTVS3_3RD_API AGimmicCube : public AInteractionActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGimmicCube();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void InteractCube(AMTVS3_3rdCharacter* Character);

	UFUNCTION(BlueprintImplementableEvent)
	void InteractCubeEnd();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float X;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float Y;
	

private:
	AMTVS3_3rdCharacter* Character;
	
	UPROPERTY(EditDefaultsOnly)
	class UInputMappingContext* CubeMappingContext;

	UPROPERTY(EditDefaultsOnly)
	class UInputAction* DragAction;

	UPROPERTY(EditDefaultsOnly)
	UInputAction* ClickAction;

	UPROPERTY(EditDefaultsOnly)
	UInputAction* CloseAction;

	UPROPERTY()
	UUserWidget* CubeWidget;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> CubeWidgetFactory;
	
	
	void Drag(const struct FInputActionValue& Value);
	void Press(const struct FInputActionValue& Value);
	void Release(const struct FInputActionValue& Value);
	void Close(const struct FInputActionValue& Value);

	
	
	bool bPress;
};
