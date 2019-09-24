// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include"ArenaBattle.h"
#include "Blueprint/UserWidget.h"
#include "ABHUDWidget.generated.h"

/**
 * 
	ABPlayerState의 Multi cast delegate에서  HUD의 정보에 대한 데이터가 변경됐다면 알려주어서 
	그 관련 정보들을 수정시켜준다. 
	변경됐다는 신호가 delegate로 전달되면 HUD Widget에서는 그것들을 업데이트만 해주면 된다
 */
UCLASS()
class ARENABATTLE_API UABHUDWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void BindCharacterStat(class UABCharacterStatComponent* CharacterStat);
	void BindPlayerState(class AABPlayerState* PlayerState);

protected:
	virtual void NativeConstruct() override;
	void UpdateCharacterStat();
	void UpdatePlayerState();


private:
	TWeakObjectPtr<class UABCharacterStatComponent> CurrentCharacterStat;
	TWeakObjectPtr<class AABPlayerState>CurrentPlayerState;

	UPROPERTY()
		class UProgressBar* HPBar;
	UPROPERTY()
		class UProgressBar* ExpBar;
	UPROPERTY()
		class UTextBlock*PlayerName;
	UPROPERTY()
		class UTextBlock*PlayerLevel;
	UPROPERTY()
		class UTextBlock* CurrentScore;
	UPROPERTY()
		class UTextBlock* HighScore;
	
};
