// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ArenaBattle.h"
#include "GameFramework/PlayerState.h"
#include "ABPlayerState.generated.h"

/**
 *
 Player State 클래스에는 이미 FString PlayerName과 float Score 속성이 이미 설계되어 있다.
 PlayerName속성은 게임 상에 나타날 NickName으로 활용하고 Score는 점수로 활용할 수 있다

 */

/*
	chapter 14 HUD UI
	PlayerController에서 띄워주는 HUD에 대하여
	데이터를 연동시켜준다
	델리게이트는 플레이어 데이터가 변경될 때 HUD에 신호를 보내  HUD가 관련 UI위젯을 업데이트 하도록 만든다

	EXP UI
	Player의 Exp 정보를 저장
*/


DECLARE_MULTICAST_DELEGATE(FOnPlayerStateChangedDelegate);//chapter 14

UCLASS()
class ARENABATTLE_API AABPlayerState : public APlayerState
{
	GENERATED_BODY()
public:
	AABPlayerState();
	int32 GetGameScore()const;
	int32 GetCharacterLevel()const;
	void InitPlayerData();
	//chapter 14 EXP
	float GetExpRatio()const;
	bool AddExp(int32 IncomeExp);

	//chapter 14 HUD UI. 델리게이트 변수
	FOnPlayerStateChangedDelegate OnPlayerStateChanged;

protected:
	UPROPERTY(Transient)
		int32 GameScore;
	UPROPERTY(Transient)
		int32 CharacterLevel;
	//chapter 14 Exp
	UPROPERTY(Transient)
		int32 Exp;
private:
	//chapter 14
	void SetCharacterLevel(int32 NewCharacterLevel);
	struct FABCharacterData* CurrentStatData;
};
