// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ArenaBattle.h"
#include "GameFramework/PlayerState.h"
#include "ABPlayerState.generated.h"

/**
 *
 Player State Ŭ�������� �̹� FString PlayerName�� float Score �Ӽ��� �̹� ����Ǿ� �ִ�.
 PlayerName�Ӽ��� ���� �� ��Ÿ�� NickName���� Ȱ���ϰ� Score�� ������ Ȱ���� �� �ִ�

 */

/*
	chapter 14 HUD UI
	PlayerController���� ����ִ� HUD�� ���Ͽ�
	�����͸� ���������ش�
	��������Ʈ�� �÷��̾� �����Ͱ� ����� �� HUD�� ��ȣ�� ����  HUD�� ���� UI������ ������Ʈ �ϵ��� �����

	EXP UI
	Player�� Exp ������ ����
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

	//chapter 14 HUD UI. ��������Ʈ ����
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
