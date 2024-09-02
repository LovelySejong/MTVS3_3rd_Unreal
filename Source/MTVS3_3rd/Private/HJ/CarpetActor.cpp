﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "HJ/CarpetActor.h"
#include "Components/BoxComponent.h"
#include "MTVS3_3rdGameMode.h"
#include "HJ/HJ_Player.h"

// Sets default values
ACarpetActor::ACarpetActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	SetRootComponent(BoxComp);
	BoxComp->SetBoxExtent(FVector(50.0f , 50.0f , 10.0f));

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent);
	ConstructorHelpers::FObjectFinder<UStaticMesh> TempMesh(TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Cube1.Cube1'"));
	if ( TempMesh.Succeeded() )
	{
		MeshComp->SetStaticMesh(TempMesh.Object);
		MeshComp->SetRelativeScale3D(FVector(1.0f , 1.0f , 0.2f));
	}
}

// Called when the game starts or when spawned
void ACarpetActor::BeginPlay()
{
	Super::BeginPlay();
	
	BoxComp->OnComponentBeginOverlap.AddDynamic(this , &ACarpetActor::OnMyBoxBeginOverlap);
	BoxComp->OnComponentEndOverlap.AddDynamic(this , &ACarpetActor::OnMyBoxEndOverlap);
}

// Called every frame
void ACarpetActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACarpetActor::OnMyBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent , AActor* OtherActor , UPrimitiveComponent* OtherComp , int32 OtherBodyIndex , bool bFromSweep , const FHitResult& SweepResult)
{
	auto* gm = CastChecked<AMTVS3_3rdGameMode>(GetWorld()->GetAuthGameMode());
	if ( OtherActor->IsA<AHJ_Player>() )
	{
		if ( this->ActorHasTag(FName("Carpet1")) )
		{
			gm->CheckCarpet(1, 1);
		}
		else if ( this->ActorHasTag(FName("Carpet2")) )
		{
			gm->CheckCarpet(2, 1);
		}
	}
}

void ACarpetActor::OnMyBoxEndOverlap(UPrimitiveComponent* OverlappedComponent , AActor* OtherActor , UPrimitiveComponent* OtherComp , int32 OtherBodyIndex)
{
	auto* gm = CastChecked<AMTVS3_3rdGameMode>(GetWorld()->GetAuthGameMode());
	if ( OtherActor->IsA<AHJ_Player>() )
	{
		if ( this->ActorHasTag(FName("Carpet1")) )
		{
			gm->CheckCarpet(1, -1);
		}
		else if ( this->ActorHasTag(FName("Carpet2")) )
		{
			gm->CheckCarpet(2, -1);
		}
	}
}

