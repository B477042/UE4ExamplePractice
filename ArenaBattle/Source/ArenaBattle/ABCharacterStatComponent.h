// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ArenaBattle.h"
#include "Components/ActorComponent.h"
#include "ABCharacterStatComponent.generated.h"


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
private:
	struct FABCharacterData* CurrentStatData = nullptr;
	//save Level data
	UPROPERTY(EditInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
		int32 Level;
	//save current HP
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
		float CurrentHP;
};
