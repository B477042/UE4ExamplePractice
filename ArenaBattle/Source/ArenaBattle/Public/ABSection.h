// Fill out your copyright notice in the Description page of Project Settings.

//Charpter13 infinity create Section Of Map
#pragma once

#include "ArenaBattle.h"
#include "GameFramework/Actor.h"
#include "ABSection.generated.h"

UCLASS()
class ARENABATTLE_API AABSection : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AABSection();

	//Actor's function. This function is called when change on editor. Such as Actor Property or Transform etc...
	virtual void OnConstruction(const FTransform& Transform) override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
//
//public:	
//	// Called every frame
//	virtual void Tick(float DeltaTime) override;
private:
	//enum class for statement
	enum class ESectionState :uint8
	{
	READY = 0,
	BATTLE, 
	COMPLETE
	};
	void SetState(ESectionState NewState);
	ESectionState CurrentState;
	void OperateGates(bool bOpen = true);

	UFUNCTION()
		void OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor*OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void OnGateTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor*OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	UPROPERTY(VisibleAnywhere, Category = Mesh, Meta = (AllowPrivateAccess = true))
		TArray<UStaticMeshComponent*> GateMeshes;

	UPROPERTY(VisibleAnywhere, Category = Trigger, Meta = (AllowPrivateAccess = true))
		TArray<UBoxComponent*> GateTriggers;

	UPROPERTY(VisibleAnywhere, Category = Mesh, Meta = (AllowPrivateAccess = true))
		UStaticMeshComponent* Mesh;
	UPROPERTY(VisibleAnywhere, Category = Trigger, Meta = (AllowPrivateAccess = true))
		UBoxComponent* Trigger;

private:
	UPROPERTY(EditAnywhere, Category = State, Meta = (AllowPrivateAccess = true))
		bool bNoBattle;
};
