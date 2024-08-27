// Fill out your copyright notice in the Description page of Project Settings.


#include "Justin/S3GameInstance.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"

US3GameInstance::US3GameInstance()
{

}

void US3GameInstance::Init()
{
	if ( IOnlineSubsystem* SubSystem = IOnlineSubsystem::Get() )
	{
		SessionInterface = SubSystem->GetSessionInterface();
		if ( SessionInterface.IsValid() )
		{
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this , &US3GameInstance::OnCreateSessionCompleted);
		}
	}

	Super::Init();
}

void US3GameInstance::CreateServer()
{
	FOnlineSessionSettings SessionSettings; 
	SessionSettings.bAllowJoinInProgress = false;
	SessionSettings.bIsDedicated = false;
	SessionSettings.bIsLANMatch = true; 
	SessionSettings.bShouldAdvertise = true;
	SessionSettings.bUsesPresence = true;
	SessionSettings.NumPublicConnections = 2;

	SessionInterface->CreateSession(0 , "MySession" , SessionSettings);
}

void US3GameInstance::JoinServer()
{
}

void US3GameInstance::OnCreateSessionCompleted(FName SessionName , bool bWasSuccessful)
{
	if ( bWasSuccessful )
	{
		GetWorld()->ServerTravel(TEXT("/Game/KHJ/Maps/KHJ_FirstPersonMap?listen"));
	}
	else
	{
		UE_LOG(LogTemp , Warning , TEXT("Create Session Unsuccessful"));
	}
}

