﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "HJ/CheckPoint.h"
#include "Components/BoxComponent.h"
//#include "HJ/HJ_Player.h"
#include "Components/StaticMeshComponent.h"
#include "HJ/MTVS3_3rdGameState.h"
#include "MTVS3_3rdCharacter.h"

// Sets default values
ACheckPoint::ACheckPoint()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	SetRootComponent(BoxComp);
	BoxComp->SetBoxExtent(FVector(112.5f , 25.0f , 150.0f));

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent);
	ConstructorHelpers::FObjectFinder<UStaticMesh> TempMesh(TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Cube1.Cube1'"));
	if ( TempMesh.Succeeded() )
	{
		MeshComp->SetStaticMesh(TempMesh.Object);
		MeshComp->SetRelativeScale3D(FVector(2.25f , 0.5f , 3.0f));
	}

	BoxComp2 = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp2"));
	BoxComp2->SetupAttachment(RootComponent);
	BoxComp2->SetBoxExtent(FVector(112.5f , 25.0f , 150.0f));
	BoxComp2->SetRelativeLocation(FVector(0 , -50 , 0));

	MeshComp2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp2"));
	MeshComp2->SetupAttachment(BoxComp2);
	ConstructorHelpers::FObjectFinder<UStaticMesh> TempMesh2(TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Cube1.Cube1'"));
	if ( TempMesh2.Succeeded() )
	{
		MeshComp2->SetStaticMesh(TempMesh2.Object);
		MeshComp2->SetRelativeScale3D(FVector(2.25f , 0.5f , 3.0f));
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
	auto* gs = GetWorld()->GetGameState<AMTVS3_3rdGameState>();
	if ( OtherActor->IsA<AMTVS3_3rdCharacter>() )
	{
		if ( this->ActorHasTag(FName("TutorialEnd")) )
		{
			this->Destroy();
			//if ( HasAuthority() ) gs->OnQuiz1Start();
			gs->OnQuiz1Start();
		}
		else if ( this->ActorHasTag(FName("Quiz1End")) )
		{
			this->Destroy();
			//if ( HasAuthority() ) gs->OnQuiz2Start();
			gs->OnQuiz2Start();
		}
		else if ( this->ActorHasTag(FName("Quiz2End")) )
		{
			this->Destroy();
			//if ( HasAuthority() ) gs->OnMeetingStart();
			gs->OnMeetingStart();
		}
		else if ( this->ActorHasTag(FName("MeetingEnd")) )
		{
			this->Destroy();
			//if ( HasAuthority() ) gs->OnQuiz3Start();
			gs->OnQuiz3Start();
		}
		else if ( this->ActorHasTag(FName("Quiz3End")) )
		{
			this->Destroy();
			//if ( HasAuthority() ) gs->OnQuiz4Start();
			gs->OnQuiz4Start();
		}
		else if ( this->ActorHasTag(FName("Quiz4End")) )
		{
			this->Destroy();
			//if ( HasAuthority() ) gs->OnGameClear();
			gs->OnGameClear();
		}
	}

}