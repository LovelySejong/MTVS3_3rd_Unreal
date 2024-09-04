// Fill out your copyright notice in the Description page of Project Settings.


#include "Justin/Lobby/LobbyWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

void ULobbyWidget::Init(bool bHost)
{
	bIsHost = bHost;

	PlayerOne_Image->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	if ( bIsHost )
	{
		PlayerTwo_Image->SetVisibility(ESlateVisibility::Collapsed);
		Ready_Text->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		PlayerTwo_Image->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		StartGame_Text->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void ULobbyWidget::RemovePlayer()
{
	UE_LOG(LogTemp , Warning , TEXT("RemovePlayer called"));
	PlayerTwo_Image->SetVisibility(ESlateVisibility::Collapsed);
}

void ULobbyWidget::AddPlayer()
{
	UE_LOG(LogTemp , Warning , TEXT("AddPlayer called"));
	PlayerTwo_Image->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}
