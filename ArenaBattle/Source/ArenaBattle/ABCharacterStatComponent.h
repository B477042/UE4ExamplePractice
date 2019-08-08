// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ArenaBattle.h"
#include "Components/ActorComponent.h"
#include "ABCharacterStatComponent.generated.h"

//HP가 0이하가 됐는지 ABCharacter에 알려주기 위한 델리게이트
DECLARE_MULTICAST_DELEGATE(FOnHPIsZeroDelegate);
//Stat의 변동 값에 따라 hp bar의 표시를 바꿔줄 델리게이트
DECLARE_MULTICAST_DELEGATE(FOnHPChangedDelegate);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARENABATTLE_API UABCharacterStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UABCharacterStatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	//chapter 11 , same role = Actor-> PostInitializeComponents
	virtual void InitializeComponent()override;
public:	
	// Called every frame
	//we dont need tick logic
	//because we will calaculate Stat value, when data had been changed. So we dont need to call tick every frames
	//virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//this is only way to control Level
	void SetNewLevel(int32 NewLevel);
	//if ABCharacter call TakeDamage Function, this Component must calculate remain HP
	//So we make SetDamage Function and GetAttack to calculate Character's HP
	void SetDamage(float NewDamage);
	void SetHP(float NewHP);


	float GetAttack();
	float GetHPRatio();

	FOnHPIsZeroDelegate OnHPIsZero;
	FOnHPChangedDelegate OnHPChanged;
private:
	struct FABCharacterData* CurrentStatData = nullptr;
	//save Level data
	UPROPERTY(EditInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
		int32 Level;
	//save current HP,Transient 조사
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
		float CurrentHP;
};
