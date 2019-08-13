// Fill out your copyright notice in the Description page of Project Settings.

#include "BTDecorator_IsAttackRange.h"
#include"ABAIController.h"
#include"ABCharacter.h"
#include"BehaviorTree/BlackboardComponent.h"

UBTDecorator_IsAttackRange::UBTDecorator_IsAttackRange()
{


}


bool UBTDecorator_IsAttackRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)const 
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);
	auto ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)return false;

	auto Target = Cast<AABCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AABAIController::TargetKey));
	if (nullptr == Target)return false;

	//attack range is 2m
	bResult = (Target->GetDistanceTo(ControllingPawn) <= 200.0f);
	return bResult;


}