// Fill out your copyright notice in the Description page of Project Settings.


#include "HJ/HJ_Player.h"
#include "MTVS3_3rdProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Engine/LocalPlayer.h"
#include "HJ/HintWidget.h"
#include "HJ/QuizWidget.h"

// Sets default values
AHJ_Player::AHJ_Player()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f , 96.0f);

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f , 0.f , 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	//Mesh1P->SetRelativeRotation(FRotator(0.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-30.f , 0.f , -150.f));

}

// Called when the game starts or when spawned
void AHJ_Player::BeginPlay()
{
	Super::BeginPlay();
	
#pragma region UI
	InitQuizHintUI();
#pragma endregion
}

// Called to bind functionality to input
void AHJ_Player::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindingsI
	if ( UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent) )
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction , ETriggerEvent::Started , this , &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction , ETriggerEvent::Completed , this , &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction , ETriggerEvent::Triggered , this , &AHJ_Player::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction , ETriggerEvent::Triggered , this , &AHJ_Player::Look);
	}
}

#pragma region UI
void AHJ_Player::InitQuizHintUI()
{
	HintUI = CastChecked<UHintWidget>(CreateWidget(GetWorld() , HintUIFactory));
	if ( HintUI )
	{
		HintUI->AddToViewport();
		HintUI->SetActiveHintPanel(false);
	}

	QuizUI = CastChecked<UQuizWidget>(CreateWidget(GetWorld() , QuizUIFactory));
	if ( QuizUI )
	{
		QuizUI->AddToViewport();
		QuizUI->SetActiveQuizPanel(false, 0);
	}
}

void AHJ_Player::UseHint()
{
	bIsHintActive = !bIsHintActive;
	HintUI->SetActiveHintPanel(bIsHintActive);
}

void AHJ_Player::UseQuiz(int num)
{
	QuizUI->SetActiveQuizPanel(true, num);
}
#pragma endregion

void AHJ_Player::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if ( Controller != nullptr )
	{
		// add movement 
		AddMovementInput(GetActorForwardVector() , MovementVector.Y);
		AddMovementInput(GetActorRightVector() , MovementVector.X);
	}
}

void AHJ_Player::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if ( Controller != nullptr )
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}