// Fill out your copyright notice in the Description page of Project Settings.

#include "ABGameMode.h"
#include"ABPlayerController.h"
#include"ABCharacter.h"
#include"ABPlayerState.h"//chapter 14 Sync UI to Player State

AABGameMode::AABGameMode()
{

	DefaultPawnClass = AABCharacter::StaticClass();
	PlayerControllerClass = AABPlayerController::StaticClass();
	PlayerStateClass = AABPlayerState::StaticClass();
}
void AABGameMode::PostLogin(APlayerController* NewPlayer)
{
	//ABLOG(Warning, TEXT("PostLogin Begin"));
	Super::PostLogin(NewPlayer);
//	ABLOG(Warning, TEXT("PostLogin End"));

	//chapter 14
	//PlayerState Ŭ������ AABPlayerState�� ����
	//player �Ӽ��� �Ϸ��ϴ� Post Login�Լ� ���� ���� �����Ѱ� ����
	auto ABPlayerState = Cast<AABPlayerState>(NewPlayer->PlayerState);
	ABCHECK(nullptr != ABPlayerState);
	ABPlayerState->InitPlayerData();

}