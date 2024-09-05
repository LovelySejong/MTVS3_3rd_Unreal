// Fill out your copyright notice in the Description page of Project Settings.


#include "HJ/StartWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/Button.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Justin/Login/LoginPawn.h"

void UStartWidget::NativeConstruct()
{
	Super::NativeConstruct();
	SetActiveLoginUI(false);
	SetActiveRoadingUI(false);
	SetActiveConnectFailUI(false);

	Button_Start->OnClicked.AddDynamic(this , &UStartWidget::OnStartButtonClicked);
	Button_Quit->OnClicked.AddDynamic(this , &UStartWidget::OnQuitButtonClicked);
	Button_SignIn->OnClicked.AddDynamic(this , &UStartWidget::OnSignInButtonClicked);
	Button_SignUp->OnClicked.AddDynamic(this , &UStartWidget::OnSignUpButtonClicked);
	Button_X->OnClicked.AddDynamic(this , &UStartWidget::OnXButtonClicked);
	Button_FailQuit->OnClicked.AddDynamic(this , &UStartWidget::OnFailQuitButtonClicked);
}

void UStartWidget::OnStartButtonClicked()
{
	SetActiveLoginUI(true);
}

void UStartWidget::OnQuitButtonClicked()
{
	auto* pc = GetWorld()->GetFirstPlayerController();
	UKismetSystemLibrary::QuitGame(GetWorld() , pc , EQuitPreference::Quit , false);
}

void UStartWidget::SetActiveLoginUI(bool value)
{
	if (value ) LoginPanel->SetVisibility(ESlateVisibility::Visible);
	else LoginPanel->SetVisibility(ESlateVisibility::Hidden);
}

void UStartWidget::OnSignInButtonClicked()
{
	/*StartPanel->SetVisibility(ESlateVisibility::Hidden);
	FName LevelName = FName(TEXT("MatchingLevel"));
	UGameplayStatics::OpenLevel(this , LevelName);
	auto* pc = GetWorld()->GetFirstPlayerController();
	pc->SetInputMode(FInputModeGameOnly());*/

	//check if User successfully logged in.
	//if yes, allow player to connect
	auto Pawn = GetOwningPlayerPawn<ALoginPawn>();
	if ( Pawn )
	{
		Pawn->StartConnection(this);
	}
	//else ask user to sign in	
}

void UStartWidget::OnSignUpButtonClicked()
{
	FString URL = TEXT("https://descriptive-manchego-543.notion.site/In-Sink-aa4edfa5ebde4cf29cd7dc89c1ec014b?pvs=4");
	FPlatformProcess::LaunchURL(*URL , nullptr , nullptr);
}

void UStartWidget::OnXButtonClicked()
{
	SetActiveLoginUI(false);
}

void UStartWidget::SetActiveRoadingUI(bool value)
{
	if ( value ) RoadingPanel->SetVisibility(ESlateVisibility::Visible);
	else RoadingPanel->SetVisibility(ESlateVisibility::Hidden);
}

void UStartWidget::SetActiveConnectFailUI(bool value)
{
	if ( value ) ConnectFailPanel->SetVisibility(ESlateVisibility::Visible);
	else ConnectFailPanel->SetVisibility(ESlateVisibility::Hidden);
}

void UStartWidget::OnFailQuitButtonClicked()
{
	FString mapName = UGameplayStatics::GetCurrentLevelName(GetWorld());
	UGameplayStatics::OpenLevel(GetWorld() , FName(*mapName));
}

