// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ArenaBattle.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "ABPawn.generated.h"

UCLASS()
class ARENABATTLE_API AABPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AABPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void PostInitializeComponents()override;
	virtual void PossessedBy(AController* NewController)override;
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnyWhere, Category = Collison)
		UCapsuleComponent* Capsule;
	UPROPERTY(VisibleAnyWhere, Category = Visual)
		USkeletalMeshComponent*Mesh;
	UPROPERTY(VisibleAnyWhere, Category = Movement)
		UFloatingPawnMovement *Movement;
	UPROPERTY(VisibleAnyWhere, Category = Camera)
		USpringArmComponent *SpringArm;
	UPROPERTY(VisibleAnyWhere, Category = Camera)
		UCameraComponent* Camera;


private:
	//input process
	//if forward, float value 1
	//else backward, float value is -1
	void UpDown(float);

	//if leftward, float value -1
	//else rightward float value 1
	void LeftRight(float);

};
