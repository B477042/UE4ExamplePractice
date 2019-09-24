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
	PlayerController�� HUD UI�� ���� ����� �߰� ��Ų��
	UI Asset�� Reference�� ������ Ŭ���� ������ �ҷ����δ�
	CreateWidget �Լ��� ������ �ν��Ͻ��� ������ �� �̸� �÷��̾��� �ϸ鿡 ����ش�.

	Next Step. ABPlayerController�� HUD Widget�� PlayerState�� �����Ѵ�
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
