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
	//PlayerState 클래스에 AABPlayerState를 지정
	//player 속성을 완료하는 Post Login함수 같은 곳에 지정한게 좋다
	auto ABPlayerState = Cast<AABPlayerState>(NewPlayer->PlayerState);
	ABCHECK(nullptr != ABPlayerState);
	ABPlayerState->InitPlayerData();

}