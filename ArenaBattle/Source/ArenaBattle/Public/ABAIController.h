// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ArenaBattle.h"
#include "AIController.h"
#include "ABAIController.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLE_API AABAIController : public AAIController
{
	GENERATED_BODY()
public:
	AABAIController();
	virtual void Possess(APawn* NewPawn)override;


	static const FName HomePosKey;
	static const FName PatrolPosKey;
	static const FName TargetKey;

	//chapter14
	void RunAI();
	void StopAI();

private:
	//BT file
	UPROPERTY()
		class UBehaviorTree* BTAsset;
	//BB file
	UPROPERTY()
		class UBlackboardData* BBAsset;
};
