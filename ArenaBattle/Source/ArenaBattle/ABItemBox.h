// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
//chapter 10 설명
/*
	플레이어를 감지하는 콜리전 박스와 아이템 상자를 시각화해주는 스태틱메시로 나뉜다
	루트 컴포넌트는 플레이어와 의 겹침을 감지할 박스 콜리전 컴포넌트를 사용하고 자식에는 스태틱메시 컴포넌트를 추가한다

	/Game/InfinityBladeGrassLands/Environments/Breakables/StaticMesh/Box/SM_Env_Breakables_Box1.SM_Env_Breakables_Box1

	아이템의 습득
	빈손의 플레이어에게 아이템을 쥐어주는 기능
	배치한 상자에 클래스 정보를 저장할 속성을 추가, 플레이어가 이에템 상자의 영역에 들어왔을 때 아이템을 생성하도록 기능을 구현

	클래스 정보를 저장할 때, 변수로 UCLASS를 선언해도 되지만 이를 사용하면,
	현재 프로젝트에 사용하는 모든 언리얼 오브젝트의 선언이 보이게 된다
	특정 클래스와 상속받은 클래스들로 목록을 한정하는 TSubclassof 라는 키워드를 제공한다
	이를 사용하면 목록에서 아이템 상자와 이를 선언한 클래스 목록만 볼 수 있다.

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
	//chapter 10, make box can scan player pawn to do something
	virtual void PostInitializeComponents() override;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
public:
	
	UPROPERTY(VisibleAnywhere, Category = Box)
		UBoxComponent*Trigger;//반응을위해?
	UPROPERTY(VisibleAnywhere, Category = Box)
		UStaticMeshComponent* Box;
	//chapter 10
	//TSubvlassOf를 이용하여 AABWeapon의 정보를 저장
	UPROPERTY(EditInstanceOnly, Category = Box)
		TSubclassOf<class AABWeapon> WeaponItemClass;
	//chaptert 10 add box effect
	UPROPERTY(VisibleAnywhere, Category = Effect)
		UParticleSystemComponent* Effect;

	//chapter 10
private:
	UFUNCTION()
		void OnCharacterOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep,const  FHitResult& SweepResult);
	UFUNCTION()
		void OnEffectFinished(class UParticleSystemComponent*PSystem);
};
