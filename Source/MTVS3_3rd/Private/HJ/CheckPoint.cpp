﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "HJ/CheckPoint.h"
#include "Components/BoxComponent.h"
#include "HJ/HJ_Player.h"
#include "Components/StaticMeshComponent.h"
#include "MTVS3_3rdGameMode.h"

// Sets default values
ACheckPoint::ACheckPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	SetRootComponent(BoxComp);
	BoxComp->SetBoxExtent(FVector(112.5f , 50.0f , 150.0f));

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent);
	ConstructorHelpers::FObjectFinder<UStaticMesh> TempMesh(TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Cube1.Cube1'"));
	if ( TempMesh.Succeeded() )
	{
		MeshComp->SetStaticMesh(TempMesh.Object);
		MeshComp->SetRelativeScale3D(FVector(2.25f , 1.0f , 3.0f));
	}
}

// Called when the game starts or when spawned
void ACheckPoint::BeginPlay()
{
	Super::BeginPlay();
	
	BoxComp->OnComponentBeginOverlap.AddDynamic(this , &ACheckPoint::OnMyBoxBeginOverlap);
}

// Called every frame
void ACheckPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACheckPoint::OnMyBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent , AActor* OtherActor , UPrimitiveComponent* OtherComp , int32 OtherBodyIndex , bool bFromSweep , const FHitResult& SweepResult)
{
	auto* gm = CastChecked<AMTVS3_3rdGameMode>(GetWorld()->GetAuthGameMode());
	if ( OtherActor->IsA<AHJ_Player>() )
	{
		if ( this->ActorHasTag(FName("TutorialEnd")) )
		{
			this->Destroy();
			gm->OnQuiz1Start();
		}
		else if ( this->ActorHasTag(FName("Quiz1End")) )
		{
			this->Destroy();
			gm->OnQuiz2Start();
		}
		else if ( this->ActorHasTag(FName("Quiz2End")) )
		{
			this->Destroy();
			gm->OnMeetingStart();
		}
		else if ( this->ActorHasTag(FName("MeetingEnd")) )
		{
			this->Destroy();
			gm->OnQuiz3Start();
		}
		else if ( this->ActorHasTag(FName("Quiz3End")) )
		{
			this->Destroy();
			gm->OnQuiz4Start();
		}
		else if ( this->ActorHasTag(FName("Quiz4End")) )
		{
			this->Destroy();
			gm->OnGameClear();
		}
	}
}