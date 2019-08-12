// Fill out your copyright notice in the Description page of Project Settings.

#include "ABAIController.h"
#include"NavigationSystem.h"
#include"Blueprint/AIBlueprintHelperLibrary.h"

AABAIController::AABAIController()
{
	RepeatInterval = 3.0f;
}

void AABAIController::Possess(APawn* InPawn)
{
	Super::Possess(InPawn);
	GetWorld()->GetTimerManager().SetTimer(RepeatTimerHandle, this, &AABAIController::OnRepeatTimer, RepeatInterval, true);
}

void AABAIController::UnPossess()
{
	Super::UnPossess();
	GetWorld()->GetTimerManager().ClearTimer(RepeatTimerHandle);

}

void AABAIController::OnRepeatTimer()
{
	auto CurrentPawn = GetPawn();
	ABCHECK(nullptr != CurrentPawn);
	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	if (nullptr == NavSystem)return;

	FNavLocation NextLocation;
	if (NavSystem->GetRandomPointInNavigableRadius(FVector::ZeroVector, 500.0f, NextLocation))
	{
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, NextLocation.Location);
		ABLOG(Warning, TEXT("Lext Location : %s"), *NextLocation.Location.ToString());
	}
}