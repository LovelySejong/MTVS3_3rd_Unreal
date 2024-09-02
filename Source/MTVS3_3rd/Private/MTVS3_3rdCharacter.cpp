// Copyright Epic Games, Inc. All Rights Reserved.

#include "MTVS3_3rdCharacter.h"
#include "MTVS3_3rdProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "CSW/GimmicCube.h"
#include "CSW/InteractionActor.h"
#include "Engine/LocalPlayer.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AMTVS3_3rdCharacter

AMTVS3_3rdCharacter::AMTVS3_3rdCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
		
	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	//Mesh1P->SetRelativeRotation(FRotator(0.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

}

void AMTVS3_3rdCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
}

void AMTVS3_3rdCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	FVector start = FirstPersonCameraComponent->GetComponentLocation();
	FVector end = start + 1000.f * FirstPersonCameraComponent->GetForwardVector();
	FHitResult res;

	bool bHit = GetWorld()->LineTraceSingleByChannel(res, start, end, ECC_Visibility);
	if (bHit)
	{
		AInteractionActor* actor = Cast<AInteractionActor>(res.GetActor());
		if (actor)
		{
			if (CurrentActor != actor)
			{
				SetCurrentActor(actor);
				CurrentActor->OnCursor();
			}
		}
		else
		{
			SetCurrentActor(nullptr);
		}
	}
	else
	{
		SetCurrentActor(nullptr);
	}
}

//////////////////////////////////////////////////////////////////////////// Input

void AMTVS3_3rdCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMTVS3_3rdCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMTVS3_3rdCharacter::Look);

		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &AMTVS3_3rdCharacter::Interact);
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Completed, this, &AMTVS3_3rdCharacter::InteractEnd);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AMTVS3_3rdCharacter::ResetPlayerInputComponent()
{
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
		{
			EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMTVS3_3rdCharacter::Look);
			EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &AMTVS3_3rdCharacter::Interact);
		}
	}
}

void AMTVS3_3rdCharacter::SetCurrentActor(AInteractionActor* actor)
{
	if (CurrentActor)
		CurrentActor->OffCursor();
	CurrentActor = actor;
}


void AMTVS3_3rdCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void AMTVS3_3rdCharacter::Look(const FInputActionValue& Value)
{
	if (cube)
		return ;
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AMTVS3_3rdCharacter::Interact(const FInputActionValue& Value)
{
	if (CurrentActor)
		CurrentActor->Interact(this);
}

void AMTVS3_3rdCharacter::InteractEnd(const FInputActionValue& Value)
{
	if (CurrentActor)
		CurrentActor->InteractEnd();
}
