// Fill out your copyright notice in the Description page of Project Settings.

#include "BTService_Detect.h"
#include"ABAIController.h"
#include"ABCharacter.h"
#include"BehaviorTree/BlackboardComponent.h"
#include"DrawDebugHelpers.h"

UBTService_Detect::UBTService_Detect()
{
	NodeName = TEXT("Detect");
	Interval = 1.0f;
}

void UBTService_Detect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)return;
	UWorld* World = ControllingPawn->GetWorld();
	FVector Center = ControllingPawn->GetActorLocation();
	//set Detect Range to 6m
	float DetectRadius = 600.0f;

	if (nullptr == World)return;
	//management Multiply target
	TArray<FOverlapResult>OverlapResults;
	FCollisionQueryParams CollisionQueryParam(NAME_None, false, ControllingPawn);
	//overlap Multiple Target
	bool bResult = World->OverlapMultiByChannel(
		OverlapResults, Center, FQuat::Identity, ECollisionChannel::ECC_GameTraceChannel12, 
		FCollisionShape::MakeSphere(DetectRadius), CollisionQueryParam);
	//chapter12. AI can tracking Player
	if (bResult)
	{
		for (auto OverlapResult : OverlapResults)
		{
			//set ABCharacter pointer to Detected  Character
			AABCharacter * ABCharacter = Cast<AABCharacter>(OverlapResult.GetActor());
			//if Detected ABCharacter is controlled by PlayerController
			if (ABCharacter&&ABCharacter->GetController()->IsPlayerController())
			{
				//set Blackboard data target to Deteceted character
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(AABAIController::TargetKey, ABCharacter);
				//draw Purple Shpere
				DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Purple,false,0.2f);
				//Draw Point Detected Player
				DrawDebugPoint(World, ABCharacter->GetTargetLocation(), 10.0f, FColor::Orange, false, 0.2f);
				return;
			}
			//if can not find player controlled Character. Set Target Value to Nullptr
			else
			{
				
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(AABAIController::TargetKey, nullptr);
			}
		}
	}



	//Draw Debug Sphere to see
	DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Cyan, false, 0.2f);


}
