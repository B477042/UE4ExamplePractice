// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
//chapter 10 설명
/*
	플레이어를 감지하는 콜리전 박스와 아이템 상자를 시각화해주는 스태틱메시로 나뉜다
	루트 컴포넌트는 플레이어와 의 겹침을 감지할 박스 콜리전 컴포넌트를 사용하고 자식에는 스태틱메시 컴포넌트를 추가한다

	/Game/InfinityBladeGrassLands/Environments/Breakables/StaticMesh/Box/SM_Env_Breakables_Box1.SM_Env_Breakables_Box1
*/

#include "ArenaBattle.h"
#include "GameFramework/Actor.h"
#include "ABItemBox.generated.h"

UCLASS()
class ARENABATTLE_API AABItemBox : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AABItemBox();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
public:
	
	UPROPERTY(VisibleAnywhere, Category = Box)
		UBoxComponent*Trigger;//반응을위해?
	UPROPERTY(VisibleAnywhere, Category = Box)
		UStaticMeshComponent* Box;

};
