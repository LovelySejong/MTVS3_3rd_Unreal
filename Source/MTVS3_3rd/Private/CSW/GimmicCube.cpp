// Fill out your copyright notice in the Description page of Project Settings.


#include "CSW/GimmicCube.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "MTVS3_3rdCharacter.h"
#include "Blueprint/UserWidget.h"

// Sets default values
AGimmicCube::AGimmicCube()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AGimmicCube::BeginPlay()
{
	Super::BeginPlay();

	CubeWidget = CreateWidget<UUserWidget>(GetWorld(), CubeWidgetFactory);
}

// Called every frame
void AGimmicCube::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGimmicCube::InteractCube(AMTVS3_3rdCharacter* character)
{
	this->Character = character;
	if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			// Set the priority of the mapping to 1, so that it overrides the Jump action with the Fire action when using touch input
			Subsystem->AddMappingContext(CubeMappingContext, 1);
		}

		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
		{
			EnhancedInputComponent->BindAction(ClickAction, ETriggerEvent::Started, this, &AGimmicCube::Press);
			EnhancedInputComponent->BindAction(ClickAction, ETriggerEvent::Completed, this, &AGimmicCube::Release);
			EnhancedInputComponent->BindAction(DragAction, ETriggerEvent::Triggered, this, &AGimmicCube::Drag);
			EnhancedInputComponent->BindAction(CloseAction, ETriggerEvent::Triggered, this, &AGimmicCube::Close);
		}
	}

	if (CubeWidget)
		CubeWidget->AddToViewport();
}



void AGimmicCube::Drag(const FInputActionValue& Value)
{
	FVector2D DragAxisVector = Value.Get<FVector2D>();

	if (bPress)
	{
		X = DragAxisVector.X;
		Y = DragAxisVector.Y;
	}
	else
	{
		X = 0;
		Y = 0;
	}
		
}

void AGimmicCube::Press(const FInputActionValue& Value)
{
	bPress = true;
}

void AGimmicCube::Release(const FInputActionValue& Value)
{
	bPress = false;
}

void AGimmicCube::Close(const FInputActionValue& Value)
{
	if (Character == nullptr)
	{
		return;
	}

	if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->RemoveMappingContext(CubeMappingContext);
		}
	}
	// Character->ResetPlayerInputComponent();
	if (CubeWidget)
		CubeWidget->RemoveFromParent();
}




