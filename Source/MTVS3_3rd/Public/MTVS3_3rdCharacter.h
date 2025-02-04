﻿// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "MTVS3_3rdCharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class AMTVS3_3rdCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Mesh, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* Mesh1P;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* InteractAction;

	UPROPERTY()
	FTimerHandle handle;

	UPROPERTY()
	class AInteractionActor* CurrentActor;

	UPROPERTY()
	class AInteractionActor* OutlineActor;

public:
#pragma region UI
	UPROPERTY(EditAnywhere, Category = "Default|UI")
	TSubclassOf<class UHintWidget> HintUIFactory;
	UPROPERTY()
	class UHintWidget* HintUI;

	UPROPERTY(EditAnywhere, Category = "Default|UI")
	TSubclassOf<class UQuizWidget> QuizUIFactory;
	UPROPERTY()
	class UQuizWidget* QuizUI;

	void InitQuizHintUI();

	bool bIsHintActive = false;

	UFUNCTION(BlueprintCallable)
	void UseHint();
	UFUNCTION(BlueprintCallable)
	void UseQuiz(int num);
#pragma endregion
	
public:
	AMTVS3_3rdCharacter();

protected:
	virtual void BeginPlay();
	virtual void Tick(float DeltaSeconds) override;

public:
		
	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

protected:
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */

	void Look(const FInputActionValue& Value);

	UFUNCTION(Server, Unreliable)
	void ServerLook(FVector2D Axis);

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastLook(FVector2D Axis);

	void Interact(const FInputActionValue& Value);
	
	void InteractEnd(const FInputActionValue& Value);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface


public:
	void ResetPlayerInputComponent();

	UPROPERTY()
	class AGimmicCube* cube;
	
	/** Returns Mesh1P subobject **/
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

	void SetCurrentActor(AInteractionActor* actor);\
	void SetOutlineActor(AInteractionActor* actor);



	UFUNCTION(Server, Unreliable)
	void Server_ExampleSetOwner(AActor* InteractableActor);
};

