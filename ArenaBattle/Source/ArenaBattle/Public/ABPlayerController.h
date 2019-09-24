// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ArenaBattle.h"
#include "GameFramework/PlayerController.h"
#include "ABPlayerController.generated.h"

/**
 * 
 */
//chpater 14
/*
	PlayerController에 HUD UI를 띄우는 기능을 추가 시킨다
	UI Asset의 Reference를 복사해 클래스 정보를 불러들인다
	CreateWidget 함수로 위젯의 인스턴스를 생성한 후 이를 플레이어의 하면에 띄워준다.

	Next Step. ABPlayerController에 HUD Widget과 PlayerState를 연결한다
*/
UCLASS()
class ARENABATTLE_API AABPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AABPlayerController();

	virtual void PostInitializeComponents()override;
	virtual void Possess(APawn* aPawn)override;
	

	class UABHUDWidget* GetHUDWidget() const;//chapter 14 HUD UI
protected:
	virtual void BeginPlay()override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
		TSubclassOf<class UABHUDWidget>HUDWidgetClass;//chapter 14 HUD UI
private:
	UPROPERTY()
		class UABHUDWidget* HUDWidget;//Chapter 14 HUD UI


};
