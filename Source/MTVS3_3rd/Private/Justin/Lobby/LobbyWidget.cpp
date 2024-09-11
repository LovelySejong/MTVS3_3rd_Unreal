// Fill out your copyright notice in the Description page of Project Settings.


#include "Justin/Lobby/LobbyWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/CircularThrobber.h"
#include "Justin/Lobby/S3PCLobby.h"
#include "Kismet/GameplayStatics.h"
#include "Justin/S3GameInstance.h"

void ULobbyWidget::Init(bool bHost)
{
	bIsHost = bHost;

	auto PC = GetOwningPlayer<AS3PCLobby>();
	if ( PC )
	{
		Button_StartOFF->OnPressed.AddDynamic(this , &ULobbyWidget::StartOFFPressed);
		Button_ReadyON->OnPressed.AddDynamic(this , &ULobbyWidget::ReadyONPressed);

		Button_StartON->OnPressed.AddDynamic(this , &ULobbyWidget::StartONPressed);
		Button_ReadyOFF->OnPressed.AddDynamic(this , &ULobbyWidget::ReadyOFFPressed);

		Button_Quit->OnPressed.AddDynamic(this , &ULobbyWidget::QuitPressed);
	}

	if ( bIsHost )
	{
		US3GameInstance* GI = GetWorld()->GetGameInstance<US3GameInstance>();
		if ( GI )
		{
			FString Nickname = GI->GetPlayerNickname();
			SetNameText(1 , Nickname); // 첫 번째 플레이어의 닉네임 설정
		}
		DisplayPlayerTwo(false);

		DisplayButton(EButtonType::NOTSTART);
		ReadyIcon->SetVisibility(ESlateVisibility::Hidden);
		NotReadyIcon->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		DisplayPlayerTwo(true);

		DisplayButton(EButtonType::NOTREADY);
	}
}

void ULobbyWidget::RemovePlayer()
{
	DisplayPlayerTwo(false);

	DisplayButton(EButtonType::NOTSTART);
	ReadyIcon->SetVisibility(ESlateVisibility::Hidden);
	NotReadyIcon->SetVisibility(ESlateVisibility::Hidden);
}

void ULobbyWidget::AddPlayer()
{
	DisplayPlayerTwo(true);
}

void ULobbyWidget::SetReady(bool bReady)
{
	if ( bIsHost )
	{
		bReady ? DisplayButton(EButtonType::START) : DisplayButton(EButtonType::NOTSTART);
	}
	else
	{
		bReady ? DisplayButton(EButtonType::READY) : DisplayButton(EButtonType::NOTREADY);
	}
}

void ULobbyWidget::SetNameText(int num , const FString& Name)
{
	if ( num == 1 && Name1Text )
	{
		Name1Text->SetText(FText::FromString(Name));
	}
	else if ( num == 2 && Name2Text )
	{
		Name2Text->SetText(FText::FromString(Name));
	}
}

void ULobbyWidget::QuitPressed()
{
	//Open login Level
	UGameplayStatics::OpenLevel(GetWorld() , "/Game/Justin/LoginLevel");
}

void ULobbyWidget::StartONPressed()
{
	auto PC = GetOwningPlayer<AS3PCLobby>();
	if ( PC && ensureAlways(PC->IsReady()) )
	{
		PC->Server_CheckCanStart();
	}
}

void ULobbyWidget::StartOFFPressed()
{
	auto PC = GetOwningPlayer<AS3PCLobby>();
	if ( PC && ensureAlways(!PC->IsReady()) )
	{
		//Play Error sound. Can't start Game.
		UE_LOG(LogTemp , Warning , TEXT("Client is not ready. Can't Start Game"));
	}
}

void ULobbyWidget::ReadyONPressed()
{
	auto PC = GetOwningPlayer<AS3PCLobby>();
	if ( PC )
	{
		PC->Server_SetReady();
	}
}

void ULobbyWidget::ReadyOFFPressed()
{
	auto PC = GetOwningPlayer<AS3PCLobby>();
	if ( PC )
	{
		PC->Server_SetReady();
	}
}

void ULobbyWidget::DisplayButton(EButtonType Type)
{
	switch ( Type )
	{
	case EButtonType::READY:
	{
		Button_ReadyON->SetVisibility(ESlateVisibility::Visible);
		Button_ReadyOFF->SetVisibility(ESlateVisibility::Collapsed);
		Button_StartON->SetVisibility(ESlateVisibility::Collapsed);
		Button_StartOFF->SetVisibility(ESlateVisibility::Collapsed);

		ReadyIcon->SetVisibility(ESlateVisibility::Visible);
		NotReadyIcon->SetVisibility(ESlateVisibility::Hidden);

		break;
	}
	case EButtonType::NOTREADY:
	{
		Button_ReadyON->SetVisibility(ESlateVisibility::Collapsed);
		Button_ReadyOFF->SetVisibility(ESlateVisibility::Visible);
		Button_StartON->SetVisibility(ESlateVisibility::Collapsed);
		Button_StartOFF->SetVisibility(ESlateVisibility::Collapsed);

		ReadyIcon->SetVisibility(ESlateVisibility::Hidden);
		NotReadyIcon->SetVisibility(ESlateVisibility::Visible);

		break;
	}
	case EButtonType::START:
	{
		Button_ReadyON->SetVisibility(ESlateVisibility::Collapsed);
		Button_ReadyOFF->SetVisibility(ESlateVisibility::Collapsed);
		Button_StartON->SetVisibility(ESlateVisibility::Visible);
		Button_StartOFF->SetVisibility(ESlateVisibility::Collapsed);

		ReadyIcon->SetVisibility(ESlateVisibility::Visible);
		NotReadyIcon->SetVisibility(ESlateVisibility::Hidden);

		break;
	}
	case EButtonType::NOTSTART:
	{
		Button_ReadyON->SetVisibility(ESlateVisibility::Collapsed);
		Button_ReadyOFF->SetVisibility(ESlateVisibility::Collapsed);
		Button_StartON->SetVisibility(ESlateVisibility::Collapsed);
		Button_StartOFF->SetVisibility(ESlateVisibility::Visible);

		ReadyIcon->SetVisibility(ESlateVisibility::Hidden);
		NotReadyIcon->SetVisibility(ESlateVisibility::Visible);

		break;
	}
	}
}

void ULobbyWidget::DisplayPlayerTwo(bool bDisplay)
{
	US3GameInstance* GI = GetWorld()->GetGameInstance<US3GameInstance>();
	if ( GI )
	{
		FString Nickname = GI->GetPlayerNickname();
		SetNameText(2 , Nickname); // 두 번째 플레이어의 닉네임 설정
	}
	if ( bDisplay )
	{
		Image_P2->SetVisibility(ESlateVisibility::Visible);
		Name2Text->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		Image_P2->SetVisibility(ESlateVisibility::Hidden);
		Name2Text->SetVisibility(ESlateVisibility::Hidden);
	}

	DisplayP2Status(bDisplay);	
}

void ULobbyWidget::DisplayP2Status(bool bDisplay)
{
	if ( bDisplay )
	{
		ReadyIcon->SetVisibility(ESlateVisibility::Hidden);
		NotReadyIcon->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		ReadyIcon->SetVisibility(ESlateVisibility::Hidden);
		NotReadyIcon->SetVisibility(ESlateVisibility::Hidden);
	}
}
