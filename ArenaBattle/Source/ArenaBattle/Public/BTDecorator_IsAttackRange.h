// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ArenaBattle.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_IsAttackRange.generated.h"

/**
	this is AI Decorate Class ; is any player in attack range
 *  this decorate do not use blackboard Data to find Player 
 */
UCLASS()
class ARENABATTLE_API UBTDecorator_IsAttackRange : public UBTDecorator
{
	GENERATED_BODY()
public:
	UBTDecorator_IsAttackRange();
protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)const override;


private:

};
