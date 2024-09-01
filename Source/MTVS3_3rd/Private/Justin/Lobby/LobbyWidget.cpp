// Fill out your copyright notice in the Description page of Project Settings.


#include "Justin/Lobby/LobbyWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Justin/Lobby/S3PCLobby.h"

void ULobbyWidget::Init(bool bHost)
{
	bIsHost = bHost;

	auto PC = GetOwningPlayer<AS3PCLobby>();
	if ( PC )
	{
		Button->OnPressed.AddDynamic(PC , &AS3PCLobby::HandleButtonPress);
	}

	PlayerOne_Image->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	if ( bIsHost )
	{
		PlayerTwo_Image->SetVisibility(ESlateVisibility::Collapsed);
		PlayerTwoReady_Image->SetVisibility(ESlateVisibility::Collapsed);
		Ready_Text->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		PlayerTwo_Image->SetVisibility(ESlateVisibility::Visible);
		PlayerTwoReady_Image->SetVisibility(ESlateVisibility::Visible);
		StartGame_Text->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void ULobbyWidget::RemovePlayer()
{
	PlayerTwo_Image->SetVisibility(ESlateVisibility::Collapsed);
	PlayerTwoReady_Image->SetVisibility(ESlateVisibility::Collapsed);
}

void ULobbyWidget::AddPlayer()
{
	PlayerTwo_Image->SetVisibility(ESlateVisibility::Visible);
	PlayerTwoReady_Image->SetVisibility(ESlateVisibility::Visible);
}

void ULobbyWidget::SetReady(bool bReady)
{
	if ( bReady )
	{
		FSlateColor Color;
		Color = FLinearColor(FVector4(0 , 0 , 1, 1));
		PlayerTwoReady_Image->SetBrushTintColor(Color);
	}
	else
	{
		FSlateColor Color;
		Color = FLinearColor(FVector4(1 , 0 , 0, 1));
		PlayerTwoReady_Image->SetBrushTintColor(Color);
	}
}