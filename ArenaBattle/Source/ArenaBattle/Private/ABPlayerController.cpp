// Fill out your copyright notice in the Description page of Project Settings.

#include "ABPlayerController.h"
#include"ABHUDWidget.h"
#include"ABPlayerState.h"
#include"ABCharacter.h"

AABPlayerController::AABPlayerController()
{
	static ConstructorHelpers::FClassFinder<UABHUDWidget> UI_HUD_C(TEXT("/Game/Book/UI/UI_HUD.UI_HUD_C"));
	if (UI_HUD_C.Succeeded())
	{
		HUDWidgetClass = UI_HUD_C.Class;
	}
}


void AABPlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	ABLOG_S(Warning);
}

void AABPlayerController::Possess(APawn *aPawn)
{
	ABLOG_S(Warning);
	Super::Possess(aPawn);
}

UABHUDWidget * AABPlayerController::GetHUDWidget() const
{
	return HUDWidget;
}

void AABPlayerController::NPCKill(AABCharacter * KilledNPC) const
{
	ABPlayerState->AddExp(KilledNPC->GetDropExp());
}

void AABPlayerController::AddGameScore()
{
	ABPlayerState->AddGameScore();
}

void AABPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);

	//chapter 14 HUD UI
	/*
		�����ڿ��� �޾ƿ�  WidgetClass ������ �޾Ƽ� viewport�� ��� �÷��̾ �� �� �ְ� ���ش�
	*/
	HUDWidget = CreateWidget<UABHUDWidget>(this, HUDWidgetClass);
	HUDWidget->AddToViewport();

	//chapter 14 HUD UI
	/*
		HUD Widget�� Player State�� �����Ѵ�
	*/
	 ABPlayerState = Cast<AABPlayerState>(PlayerState);
	ABCHECK(nullptr != ABPlayerState);
	HUDWidget->BindPlayerState(ABPlayerState);
	ABPlayerState->OnPlayerStateChanged.Broadcast();


}